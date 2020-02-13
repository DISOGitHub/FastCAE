#include "GeoCommandSketchRect.h"
#include <gp_Pnt.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <TopoDS_Edge.hxx>
#include "geometry/geometrySet.h"
#include "geometry/geometryData.h"
#include <TopoDS.hxx>
#include <gp_Ax3.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <GeomAPI_ProjectPointOnCurve.hxx>
#include <BRep_Tool.hxx>
#include <gp_Lin.hxx>
namespace Command
{
	GeoCommanSketchRect::GeoCommanSketchRect(MainWidget::SketchViewProvider* v, GUI::MainWindow* m, MainWidget::PreWindow* p)
		:GeocommandSketchBase(v, p, m)
	{
	}

	bool GeoCommanSketchRect::execute()
	{
		if (_pointList.size() != 2) return false;

		gp_Pnt pt1 = _pointList[0];
		gp_Pnt pt2 = _pointList[1];
		if (pt1.Distance(pt2) < 1e-6) return false;

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
		gp_Vec pt1Vec((nearyPt2.X()-nearyPt1.X()), (nearyPt2.Y()-nearyPt1.Y()), (nearyPt2.Z()-nearyPt1.Z()));
		yTrsf.SetTranslation(pt1Vec);
		gp_Pnt pt3=	pt1.Transformed(yTrsf);

		gp_Trsf xTrsf;
		gp_Vec pt1xVec((nearxPt2.X() - nearxPt1.X()), (nearxPt2.Y() - nearxPt1.Y()), (nearxPt2.Z() - nearxPt1.Z()));
		xTrsf.SetTranslation(pt1xVec);
		gp_Pnt pt4 = pt1.Transformed(xTrsf);

		BRepBuilderAPI_MakePolygon makePoly(pt1, pt3, pt2, pt4);
		makePoly.Close();
		if (makePoly.Shape().IsNull())  return false;
		_resShape = new TopoDS_Shape;
		*_resShape = makePoly.Shape();
		emit showSketch(_resShape);
		return GeoCommandBase::execute();
	
	}
	void GeoCommanSketchRect::undo()
	{
		emit removeSketch(_resShape);
	}

	void GeoCommanSketchRect::redo()
	{
		emit showSketch(_resShape);
	}
	


}