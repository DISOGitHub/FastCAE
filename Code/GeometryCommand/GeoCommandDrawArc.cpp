#include "GeoCommandDrawArc.h"
#include <gp_Pnt.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <TopoDS_Edge.hxx>
#include "geometry/geometrySet.h"
#include "geometry/geometryData.h"
#include <QDebug>
#include <gp_Ax2.hxx>
#include <gp_Pln.hxx>
#include <gp_Circ.hxx>
#include <gp_lin.hxx>
#include <Geom_Plane.hxx>
#include <Standard_Handle.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <Geom_Circle.hxx>
#include <GeomTools.hxx>
#include <GC_MakeCircle.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>
#include <QDebug>
namespace Command
{
	GeoCommandDrawArc::GeoCommandDrawArc(GUI::MainWindow* m, MainWidget::PreWindow* p)
		:GeoCommandBase(m, p)
	{

	}

	bool GeoCommandDrawArc::execute()
	{
		gp_Pnt* p1 = _listPnt[0];
		gp_Pnt* p2 = _listPnt[1];
		gp_Pnt* p3 = _listPnt[2];
		gp_Pln pln(gp_Pnt(_planeLocation[0], _planeLocation[1], _planeLocation[2]), gp_Dir(_planeDirection[0], _planeDirection[1], _planeDirection[2]));
		Handle(Geom_Plane) gplane =new Geom_Plane(pln);
		
		//×ø±êÍ¶Ó°
		GeomAPI_ProjectPointOnSurf projector1(*p1, gplane);
		gp_Pnt pc = projector1.NearestPoint();
		GeomAPI_ProjectPointOnSurf projector2(*p2, gplane);
		gp_Pnt p0 = projector2.NearestPoint();
		GeomAPI_ProjectPointOnSurf projector3(*p3, gplane);
		gp_Pnt pe = projector3.NearestPoint();

		Handle(Geom_TrimmedCurve) tc = GC_MakeArcOfCircle(pc, p0, pe);
		TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(tc);

		Handle(Geom_Circle) c =GC_MakeCircle(pc, p0, pe);
		gp_Pnt center = c->Location();
		qDebug() << center.X()<<center.Y()<<center.Z();
		TopoDS_Vertex centerVer = BRepBuilderAPI_MakeVertex(center);

		TopoDS_Compound aRes;
		BRep_Builder aBuilder;
		aBuilder.MakeCompound(aRes);
		aBuilder.Add(aRes, edge);
		aBuilder.Add(aRes, centerVer);

		
		TopoDS_Shape* shape = new TopoDS_Shape;
		*shape = aRes;
		Geometry::GeometrySet* newset = new Geometry::GeometrySet(Geometry::STEP);
		newset->setShape(shape);
		_geoData->appendGeometrySet(newset);
		_result = newset;

		GeoCommandBase::execute();
		emit showSet(newset);
		emit updateGeoTree();
		return true;
	}

	void GeoCommandDrawArc::setPointsList(QList<gp_Pnt*> p)
	{
		_listPnt = p;
	}
	void GeoCommandDrawArc::setPlane(double* loc, double* dir)
	{
		for (int i = 0; i < 3; ++i)
		{
			_planeDirection[i] = dir[i];
			_planeLocation[i] = loc[i];
		}
		
	}
	void GeoCommandDrawArc::undo()
	{
		_geoData->removeTopGeometrySet(_result);
		emit updateGeoTree();
		emit removeDisplayActor(_result);
	}

	void GeoCommandDrawArc::redo()
	{
		_geoData->appendGeometrySet(_result);
		emit updateGeoTree();
		emit showSet(_result);
	}

	void GeoCommandDrawArc::releaseResult()
	{
		if (_result != nullptr) delete _result;
		_result = nullptr;
	}




}