#include "GeoSketchCreater.h"
#include "MainWidgets/sketchViewProvider.h"
#include "MainWidgets/preWindow.h"
#include "geometry/geometryData.h"
#include "GeoCommandList.h"
#include <Geom_Plane.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>
#include "GeoCommandSketchComplete.h"
#include "GeoCommandSketchLine.h"
#include "GeoCommandSketchRect.h"
#include <GeoCommandSketchCircle.h>
#include <GeoCommandSketchArc.h>
#include <GeoCommandSketchPolyline.h>
#include <GeoCommandSketchSpline.h>
#include <vtkActor.h>
#include <IVtkOCC_Shape.hxx>
#include <IVtkTools_ShapeDataSource.hxx>
#include <vtkPolyDataMapper.h>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <TopoDS_Edge.hxx>
#include <gp_Pnt.hxx>
#include <GeomAPI_ProjectPointOnCurve.hxx>
#include <BRep_Tool.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <gp_Circ.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <GC_MakeCircle.hxx>
#include <QDebug>

namespace Command
{
	SketchCreater::SketchCreater(MainWidget::SketchViewProvider* p, MainWidget::PreWindow* pre, GUI::MainWindow* m, vtkActor* actor)
		:_view(p), _preWindow(pre), _mainWindow(m), _tempActor(actor)
	{
		_geoData = Geometry::GeometryData::getInstance();
		_commandList = GeoComandList::getInstance();
		connect(this, SIGNAL(reRender()), _preWindow, SIGNAL(reRenderSig()));
	}

	SketchCreater::~SketchCreater()
	{

	}

	void SketchCreater::setSketchType(ModuleBase::SketchType t)
	{
		_sketchType = t;
		if (_sketchType==ModuleBase::SketchPolyline) _pointList.clear();
		_commandList->startSketchMode(t != ModuleBase::SketchNone);
		if (t == ModuleBase::SketchNone)
		{
			auto c = new GeoCommandSketchComplete(_mainWindow, _preWindow);
			_commandList->executeCommand(c);
		}
	}

	void SketchCreater::appendPoint(gp_Pnt &pt)
	{
		if (_sketchType == ModuleBase::SketchNone) return;
		gp_Ax3* p = _geoData->getSketchPlane();
		Handle(Geom_Plane) gplane = new Geom_Plane(*p);
		GeomAPI_ProjectPointOnSurf projector1(pt, gplane);
		gp_Pnt point = projector1.NearestPoint();
		
		bool append = true;
		if (!_pointList.isEmpty())
		{
			gp_Pnt last = _pointList.last();
			if (last.Distance(point) < 1e-5)
				append = false;
		}
		if (append)
		{
			_pointList.append(point);
			this->exec();
		}
		
	}

	void SketchCreater::setTempPoint(gp_Pnt &pt)
	{
		
		_tempPnt = pt;
		this->generateTempShape();
	}

	void SketchCreater::exec()
	{
		bool ok = false;
		switch (_sketchType)
		{
		case ModuleBase::SketchLine:
			ok = createLine(); break;
		case ModuleBase::SketchRect:
			ok = createRect(); break;
		case ModuleBase::SketchCircle:
			ok = creatCircle(); break;
		case ModuleBase::SketchArc:
			ok = createArc(); break;

		default:
			break;
		}
		if (ok)
		{
			_pointList.clear();
			_tempActor->SetVisibility(false);
			emit reRender();
		}
	}

	void SketchCreater::onMouseRightRelease()
	{
		bool ok = false;
		switch (_sketchType)
		{
		case ModuleBase::SketchPolyline:
			ok = createPolyline(); break;
		case ModuleBase::SketchSpline:
			ok = creatSpline(); break;
		default:break;
		}

		if (ok)
		{
			_pointList.clear();
			_tempActor->SetVisibility(false);
			emit reRender();
		}
	}


	void SketchCreater::onMouseMiddleRelease()
	{
		bool ok = false;
		switch (_sketchType)
		{
		case ModuleBase::SketchPolyline:
			ok = createPolyline(true); break;
		default:break;
		}

		if (ok)
		{
			_pointList.clear();
			_tempActor->SetVisibility(false);
			emit reRender();
		}
	}


	bool SketchCreater::createLine()
	{
		if (_pointList.size() != 2) return false;
		auto command = new GeoCommanSketchLine(_view, _mainWindow, _preWindow);
		command->setPoints(_pointList);
		return _commandList->executeCommand(command);
	}

	bool SketchCreater::createRect()
	{
		if (_pointList.size() != 2) return false;
		auto command = new GeoCommanSketchRect(_view, _mainWindow, _preWindow);
		command->setPoints(_pointList);
		gp_Ax3* ax3 = _geoData->getSketchPlane();
		gp_Dir y = ax3->YDirection();
		return _commandList->executeCommand(command);
	}

	bool SketchCreater::creatCircle()
	{
		if (_pointList.size() != 2) return false;
		auto command = new GeoCommanSketchCircle(_view, _mainWindow, _preWindow);
		command->setPoints(_pointList);
		return _commandList->executeCommand(command);
	}

	bool SketchCreater::createArc()
	{
		if (_pointList.size() != 3) return false;
		auto command = new GeoCommanSketchArc(_view, _mainWindow, _preWindow);
		command->setPoints(_pointList);
		return _commandList->executeCommand(command);
	}

	bool SketchCreater::createPolyline(bool close)
	{
//		_polyPtList.append(_pointList);
		auto command = new GeoCommanSketchPolyline(_view, _mainWindow, _preWindow);
		command->setPoints(_pointList);
		command->isClose(close);
		return _commandList->executeCommand(command);
	}

	bool SketchCreater::creatSpline()
	{
		auto command = new GeoCommanSketchSpline(_view, _mainWindow, _preWindow);
		command->setPoints(_pointList);
//		command->isClose(close);
		return _commandList->executeCommand(command);
	}


	void SketchCreater::generateTempShape()
	{
		TopoDS_Shape* shape = nullptr;
		switch (_sketchType)
		{
		case ModuleBase::SketchLine:
			shape = this->generateLine(); break;
		case ModuleBase::SketchRect:
			shape = this->generateRect(); break;
		case ModuleBase::SketchCircle:
			shape = this->generateCircle();break;
		case ModuleBase::SketchArc:
			shape = this->generateArc(); break;
		case ModuleBase::SketchPolyline:
		case ModuleBase::SketchSpline:
			shape = this->generatePolyLine(); break;
		default: break;
		}
		if (shape == nullptr) return;

		IVtkOCC_Shape::Handle aShapeImpl = new IVtkOCC_Shape(*shape);
		vtkSmartPointer<IVtkTools_ShapeDataSource> DS = vtkSmartPointer<IVtkTools_ShapeDataSource>::New();
		DS->SetShape(aShapeImpl);

		vtkSmartPointer<vtkPolyDataMapper> Mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		Mapper->SetInputConnection(DS->GetOutputPort());
		_tempActor->SetMapper(Mapper);
		Mapper->Update();

		_tempActor->SetVisibility(true);
		emit reRender();
		delete shape;

	}

	TopoDS_Shape* SketchCreater::generateLine()
	{
		if(_pointList.size() != 1) return nullptr;

		gp_Pnt pt1 = _pointList[0];
		if (pt1.Distance(_tempPnt) < 1e-3) return nullptr;

		const TopoDS_Edge& aedge = BRepBuilderAPI_MakeEdge(pt1, _tempPnt);
		TopoDS_Shape* shape = new TopoDS_Shape;
		*shape = aedge;
		return shape;
	}

	TopoDS_Shape* SketchCreater::generateRect()
	{
		if (_pointList.size() != 1) return nullptr;

		gp_Pnt pt1 = _pointList[0];
		gp_Pnt pt2 = _tempPnt;
		if (pt1.Distance(pt2) < 1e-6) return nullptr;

		gp_Ax3* ax3 = _geoData->getSketchPlane();
		gp_Lin yLine(ax3->Location(), ax3->YDirection());
		BRepBuilderAPI_MakeEdge yEdge(yLine);
		gp_Lin xLine(ax3->Location(), ax3->XDirection());
		BRepBuilderAPI_MakeEdge xEdge(xLine);

		Standard_Real vfirst, vlast;
		Handle(Geom_Curve) yCurve = BRep_Tool::Curve(yEdge, vfirst, vlast);
		GeomAPI_ProjectPointOnCurve projector1(pt1, yCurve);
		gp_Pnt nearyPt1 = projector1.NearestPoint();
		GeomAPI_ProjectPointOnCurve projector2(pt2, yCurve);
		gp_Pnt nearyPt2 = projector2.NearestPoint();

		Standard_Real xfirst, xlast;
		Handle(Geom_Curve) xCurve = BRep_Tool::Curve(xEdge, xfirst, xlast);
		GeomAPI_ProjectPointOnCurve projectorx1(pt1, xCurve);
		gp_Pnt nearxPt1 = projectorx1.NearestPoint();
		GeomAPI_ProjectPointOnCurve projectorx2(pt2, xCurve);
		gp_Pnt nearxPt2 = projectorx2.NearestPoint();

		gp_Trsf yTrsf;
		gp_Vec pt1Vec((nearyPt2.X() - nearyPt1.X()), (nearyPt2.Y() - nearyPt1.Y()), (nearyPt2.Z() - nearyPt1.Z()));
		yTrsf.SetTranslation(pt1Vec);
		gp_Pnt pt3 = pt1.Transformed(yTrsf);

		gp_Trsf xTrsf;
		gp_Vec pt1xVec((nearxPt2.X() - nearxPt1.X()), (nearxPt2.Y() - nearxPt1.Y()), (nearxPt2.Z() - nearxPt1.Z()));
		xTrsf.SetTranslation(pt1xVec);
		gp_Pnt pt4 = pt1.Transformed(xTrsf);

		BRepBuilderAPI_MakePolygon makePoly(pt1, pt3, pt2, pt4);
		makePoly.Close();
		if (makePoly.Shape().IsNull())  return nullptr;
		TopoDS_Shape *shape = new TopoDS_Shape;
		*shape = makePoly.Shape();
		return shape;
	}

	TopoDS_Shape* SketchCreater::generateCircle()
	{
		if (_pointList.size() != 1) return nullptr;

		gp_Pnt pt1 = _pointList[0];
		gp_Pnt pt2 = _tempPnt;
		double d = pt1.Distance(pt2);
		if (d < 1e-6) return nullptr;
		gp_Ax3* ax = _geoData->getSketchPlane();

		gp_Circ circle(gp_Ax2(pt1, ax->Direction()), d);
		BRepBuilderAPI_MakeEdge makeCircle(circle);

		if (!makeCircle.IsDone())  return nullptr;
		TopoDS_Shape* shape = new TopoDS_Shape;
		*shape = makeCircle.Shape();
		return shape;
	}

	TopoDS_Shape* SketchCreater::generateArc()
	{
		if (_pointList.size() == 1) return this->generateLine();
		else if (_pointList.size() != 2) return nullptr;

		gp_Pnt pt1 = _pointList[0];
		gp_Pnt pt2 = _pointList[1];
		gp_Pnt pt3 = _tempPnt;
		gp_Lin lin(pt1, gp_Dir(pt2.X() - pt1.X(), pt2.Y() - pt1.Y(), pt2.Z() - pt1.Z()));
		if (lin.Distance(pt3) == 0) return nullptr;
		for (int i = 0; i <= _pointList.size() - 1; ++i)
		{
			for (int j = 0; j < i; ++j)
				if (_pointList[i].Distance(_pointList[j]) < 1e-3) return nullptr;
		}

		const TopoDS_Edge& edge1 = BRepBuilderAPI_MakeEdge(pt1, pt2);
		const TopoDS_Edge& edge2 = BRepBuilderAPI_MakeEdge(pt2, _tempPnt);
		BRep_Builder builder;
		
		TopoDS_Compound aRes;
		BRep_Builder aBuilder;
		aBuilder.MakeCompound(aRes);
		aBuilder.Add(aRes, edge1);
		aBuilder.Add(aRes, edge2);

		TopoDS_Shape* shape = new TopoDS_Shape;
		*shape =aRes;
		return shape;
	}

	TopoDS_Shape* SketchCreater::generatePolyLine()
	{
		QList<gp_Pnt> pts = _pointList;
		pts.append(_tempPnt);

		if (pts.size() < 2) return nullptr;

		TopoDS_Compound aRes;
		BRep_Builder aBuilder;
		aBuilder.MakeCompound(aRes);
		for (int i = 0; i < pts.size() - 1; ++i)
		{
			BRepBuilderAPI_MakeEdge edge(pts[i], pts[i + 1]);
			aBuilder.Add(aRes, edge);
		}

		if (aRes.IsNull())  return nullptr;
		TopoDS_Shape* shape = new TopoDS_Shape;
		*shape = aRes;
		return shape;
	}

}