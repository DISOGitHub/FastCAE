#include "GeoCommandSketchCircle.h"
#include <gp_Pnt.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include "geometry/geometryData.h"
#include <gp_Ax2.hxx>
#include <gp_Ax3.hxx>
#include <gp_Circ.hxx>
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include "geometry/geometryData.h"


namespace Command
{
	GeoCommanSketchCircle::GeoCommanSketchCircle(MainWidget::SketchViewProvider* v, GUI::MainWindow* m, MainWidget::PreWindow* p)
		:GeocommandSketchBase(v, p, m)
	{
	}

	bool GeoCommanSketchCircle::execute()
	{
		if (_pointList.size() != 2) return false;

		gp_Pnt pt1 = _pointList[0];
		gp_Pnt pt2 = _pointList[1];
		double d = pt1.Distance(pt2);
		if ( d < 1e-6) return false;
                   
		auto geodata = Geometry::GeometryData::getInstance();
		gp_Ax3* ax = geodata->getSketchPlane();

		gp_Circ circle(gp_Ax2(pt1, ax->Direction()),d);
		BRepBuilderAPI_MakeEdge makeCircle(circle);

		TopoDS_Vertex ver = BRepBuilderAPI_MakeVertex(pt1);
		TopoDS_Compound aRes;
		BRep_Builder aBuilder;
		aBuilder.MakeCompound(aRes);
		aBuilder.Add(aRes, makeCircle.Shape());
		aBuilder.Add(aRes, ver);
		
		if (makeCircle.Shape().IsNull())  return false;
		_resShape = new TopoDS_Shape;
		*_resShape = aRes;
		emit showSketch(_resShape);
		return GeoCommandBase::execute();
	}

	void GeoCommanSketchCircle::undo()
	{
		emit removeSketch(_resShape);
	}

	void GeoCommanSketchCircle::redo()
	{
		emit showSketch(_resShape);
	}
}