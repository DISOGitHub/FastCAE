#include "sketchViewProvider.h"
#include "preWindow.h"
#include "moduleBase/PreWindowInteractorStyle.h"
#include "geometry/geometryData.h"
#include "GeometryCommand/GeoSketchCreater.h"
#include "GeometryWidgets/SketchPointWidget.h"
#include "mainWindow/mainWindow.h"
#include <gp_Ax3.hxx>
#include <gp_Pln.hxx>
#include <gp_Lin.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <TopoDS_Face.hxx>
#include <IVtkOCC_Shape.hxx>
#include <IVtkTools_ShapeDataSource.hxx>
#include <IVtkTools_DisplayModeFilter.hxx>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkActor.h>
#include <vtkPolyLine.h>
#include <vtkPolyData.h>
#include <vtkCellArray.h>
#include <vtkFloatArray.h>
#include <vtkCellData.h>
#include <vtkLookupTable.h>
#include <Geom_Plane.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>
#include <QDebug>

namespace MainWidget
{
	SketchViewProvider::SketchViewProvider(GUI::MainWindow* mainwindow, PreWindow* preWin)
		:_mainWindow(mainwindow), _preWindow(preWin)
	{
		_interactionStyle = _preWindow->getInteractionStyle();
		connect(_interactionStyle, SIGNAL(mouseReleasePoint(double*)), this, SLOT(onMouseRelease(double*)));
		connect(_interactionStyle, SIGNAL(mouseMovePoint(double*)), this, SLOT(onMouseMove(double*)));
		connect(_interactionStyle, SIGNAL(mouseRightUp()), this, SLOT(onRightMoustRelease()));
		connect(_interactionStyle, SIGNAL(mouseMiddleUp()), this, SLOT(onMiddleMouseUp()));
		connect(this, SIGNAL(showSketch(TopoDS_Shape*)), this, SLOT(showSketchSlot(TopoDS_Shape*)));
		connect(this, SIGNAL(hideSketch(TopoDS_Shape*)), this, SLOT(removeSketchSlot(TopoDS_Shape*)));
		connect(this, SIGNAL(showMessage(QString)), _mainWindow, SIGNAL(sendInfoToStatesBar(QString)));
		connect(this, SIGNAL(showDialog(QDialog*)), _mainWindow, SIGNAL(showDialogSig(QDialog*)));

		_geoData = Geometry::GeometryData::getInstance();
		_tempActor = vtkActor::New();
		_tempActor->SetVisibility(false);
		_tempActor->GetProperty()->SetRepresentationToWireframe();
		_tempActor->GetProperty()->SetColor(1, 1, 1);
		_tempActor->GetProperty()->SetLineWidth(3);

		_preWindow->AppendActor(_tempActor, ModuleBase::D3, false, false);
		_creater = new Command::SketchCreater(this, _preWindow, _mainWindow, _tempActor);
	}

	SketchViewProvider::~SketchViewProvider()
	{
		if (_axisXActor != nullptr) _axisXActor->Delete();
		if (_axisYActor != nullptr) _axisYActor->Delete();
		if (_tempActor != nullptr) _tempActor->Delete();
		removeSketchActors();
		if (_pointWidget != nullptr) delete _pointWidget;
	}

	void SketchViewProvider::setSketchType(ModuleBase::SketchType t)
	{
		_sketchType = t;
		_creater->setSketchType(t);
		if (_sketchType == ModuleBase::SketchNone)
			removeSketchActors();
	}

	void SketchViewProvider::showSketchPlane(bool show)
	{
		if (!show)
		{
			if( _axisXActor != nullptr)
			{
				_preWindow->RemoveActor(_axisXActor);
				_axisXActor->Delete();
				_axisXActor = nullptr;
			}
			if (_axisYActor != nullptr)
			{
				_preWindow->RemoveActor(_axisYActor);
				_axisYActor->Delete();
				_axisYActor = nullptr;
			}
			if (_pointWidget != nullptr)
			{
				delete _pointWidget;
				_pointWidget = nullptr;
			}
			_tempActor->SetVisibility(false);
			return;
		}
		gp_Ax3* ax3 = _geoData->getSketchPlane();
		if (ax3 == nullptr) return;
		gp_Pnt o = ax3->Location();
 		gp_Dir x = ax3->XDirection();
 		gp_Dir y = ax3->YDirection();
 		double length = _preWindow->getWorldHight();
		if (length < 10.0) length = 10.0;
		length /= 4.0;
		double p0[3] = { o.X(), o.Y(), o.Z() };
		double p1[3] = { o.X() + x.X()*length, o.Y() + x.Y()*length, o.Z() + x.Z()*length };
		double p2[3] = { o.X() - x.X()*length, o.Y() - x.Y()*length, o.Z() - x.Z()*length };
		double p3[3] = { o.X() + y.X()*length, o.Y() + y.Y()*length, o.Z() + y.Z()*length };
		double p4[3] = { o.X() - y.X()*length, o.Y() - y.Y()*length, o.Z() - y.Z()*length };

		vtkSmartPointer<vtkPoints> pts = vtkSmartPointer<vtkPoints>::New();
		pts->InsertNextPoint(p1); pts->InsertNextPoint(p2); pts->InsertNextPoint(p3); pts->InsertNextPoint(p4);

		vtkSmartPointer<vtkPolyData> polyx = vtkSmartPointer<vtkPolyData>::New();
		vtkSmartPointer<vtkPolyData> polyy = vtkSmartPointer<vtkPolyData>::New();
		polyx->SetPoints(pts); polyy->SetPoints(pts);
		vtkSmartPointer<vtkPolyLine> polyLine1 = vtkSmartPointer<vtkPolyLine>::New();
		polyLine1->GetPointIds()->SetNumberOfIds(2);
		polyLine1->GetPointIds()->SetId(0, 0); polyLine1->GetPointIds()->SetId(1, 1);
		vtkSmartPointer<vtkPolyLine> polyLine2 = vtkSmartPointer<vtkPolyLine>::New();
		polyLine2->GetPointIds()->SetNumberOfIds(2);
		polyLine2->GetPointIds()->SetId(0, 2); polyLine2->GetPointIds()->SetId(1, 3);
		vtkSmartPointer<vtkCellArray> cell1 = vtkSmartPointer<vtkCellArray>::New();
		vtkSmartPointer<vtkCellArray> cell2 = vtkSmartPointer<vtkCellArray>::New();
		cell1->InsertNextCell(polyLine1); cell2->InsertNextCell(polyLine2);
		polyx->SetLines(cell1); polyy->SetLines(cell2);
		vtkSmartPointer<vtkPolyDataMapper> mapperx = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapperx->SetInputData(polyx);
		_axisXActor = vtkActor::New();
		_axisXActor->SetMapper(mapperx);
		_axisXActor->GetProperty()->SetLineWidth(3);
		_axisXActor->GetProperty()->SetColor(1, 0, 0);
		vtkSmartPointer<vtkPolyDataMapper> mappery = vtkSmartPointer<vtkPolyDataMapper>::New();
		mappery->SetInputData(polyy);
		_axisYActor = vtkActor::New();
		_axisYActor->SetMapper(mappery);
		_axisYActor->GetProperty()->SetLineWidth(3);
		_axisYActor->GetProperty()->SetColor(0,	1, 0);
		_preWindow->AppendActor(_axisXActor, ModuleBase::D3, false);
		_preWindow->AppendActor(_axisYActor, ModuleBase::D3, false);
		
		_pointWidget = new GeometryWidget::SketchPointWidget(_mainWindow, _preWindow, _creater);
		emit showDialog(_pointWidget);
		
	}

	void SketchViewProvider::onMouseRelease(double* pt)
	{
		if (_sketchType == ModuleBase::SketchNone) return;
		gp_Pnt gPt(pt[0], pt[1], pt[2]);
		_creater->appendPoint(gPt);
	}

	void SketchViewProvider::onMouseMove(double* pt)
	{
		if (_sketchType == ModuleBase::SketchNone) return;
		gp_Pnt gPt(pt[0], pt[1], pt[2]);
		gp_Ax3* p = _geoData->getSketchPlane();
		Handle(Geom_Plane) gplane = new Geom_Plane(*p);
		GeomAPI_ProjectPointOnSurf projector1(gPt, gplane);
		gp_Pnt point = projector1.NearestPoint();
		_creater->setTempPoint(point);
		this->pointTo2D(&point);
	}

	void SketchViewProvider::showSketchSlot(TopoDS_Shape* shape)
	{
		IVtkOCC_Shape::Handle aShapeImpl = new IVtkOCC_Shape(*shape);
		vtkSmartPointer<IVtkTools_ShapeDataSource> DS = vtkSmartPointer<IVtkTools_ShapeDataSource>::New();
		DS->SetShape(aShapeImpl);

		vtkSmartPointer<vtkPolyDataMapper> Mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		Mapper->SetInputConnection(DS->GetOutputPort());
		vtkSmartPointer<vtkActor> Actor = vtkSmartPointer<vtkActor>::New();
		Actor->SetMapper(Mapper);

		Actor->GetProperty()->SetColor(0, 0, 0);
		Actor->GetProperty()->SetRepresentationToWireframe();
		Actor->GetProperty()->SetLineWidth(3);
// 		_edgeActors.append(Actor);
// 		_actotShapeHash.insert(Actor, index);
// 		_setActors.insert(set, Actor);
//		Actor->SetVisibility(set->isVisible());
		Actor->SetPickable(false);
		_sketchActorHash.insert(shape, Actor);
		_preWindow->AppendActor(Actor, ModuleBase::D3, true, false);
	}

	void SketchViewProvider::removeSketchSlot(TopoDS_Shape* shape)
	{
		vtkActor* actor = _sketchActorHash.value(shape);
		_preWindow->RemoveActor(actor);
		_preWindow->reRender();
		_sketchActorHash.remove(shape);
	}

	void SketchViewProvider::removeSketchActors()
	{
		QList<vtkActor*> actors = _sketchActorHash.values();
		const int n = actors.size();
		for (int i = 0; i < n; ++i)
		{
			vtkActor* ac = actors.at(i);
			_preWindow->RemoveActor(ac);
		}
		_sketchActorHash.clear();
		_preWindow->reRender();
	}

	void SketchViewProvider::pointTo2D(gp_Pnt* pt)
	{
		gp_Ax3* ax3 = _geoData->getSketchPlane();
		if (ax3 == nullptr) return;
		gp_Pnt o = ax3->Location();
		gp_Dir x = ax3->XDirection();
		gp_Dir y = ax3->YDirection();

		gp_Lin xAxis(o, x);
		gp_Lin yAxis(o, y);

		gp_Dir dir(pt->X() - o.X(), pt->Y() - o.Y(), pt->Z() - o.Z());
		gp_Lin line(o, dir);

		double xcoor = yAxis.Distance(*pt);
		double ycoor = xAxis.Distance(*pt);

		double anglex = x.Angle(dir);
		double angley = y.Angle(dir);

		if (anglex > 3.141592653589793238 / 2.0) xcoor *= -1;
		if (angley > 3.141592653589793238 / 2.0) ycoor *= -1;

		QString text = QString(tr("Location: %1, %2")).arg(xcoor).arg(ycoor);
		emit showMessage(text);
	}

	void SketchViewProvider::onRightMoustRelease()
	{
		_creater->onMouseRightRelease();
	}

	void SketchViewProvider::onMiddleMouseUp()
	{
		_creater->onMouseMiddleRelease();
	}



}