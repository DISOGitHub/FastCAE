#include "GeoCommandSketchArc.h"
#include <gp_Pnt.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <TopoDS_Edge.hxx>
#include "geometry/geometrySet.h"
#include "geometry/geometryData.h"
#include <GC_MakeArcOfCircle.hxx>
#include <Geom_Circle.hxx>
#include <GC_MakeCircle.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>
#include <gp_Lin.hxx>

namespace Command
{
	GeoCommanSketchArc::GeoCommanSketchArc(MainWidget::SketchViewProvider* v, GUI::MainWindow* m, MainWidget::PreWindow* p)
		:GeocommandSketchBase(v, p, m)
	{
	}

	bool GeoCommanSketchArc::execute()
	{
		if (_pointList.size() != 3) return false;

		gp_Pnt pt1 = _pointList[0];
		gp_Pnt pt2 = _pointList[1];
		gp_Pnt pt3 = _pointList[2];
		gp_Lin lin(pt1, gp_Dir(pt2.X() - pt1.X(), pt2.Y() - pt1.Y(), pt2.Z() - pt1.Z()));
		if (lin.Distance(pt3) == 0) return false;

		for (int i = 0; i <= _pointList.size()-1; ++i)
		{
			for (int j = 0; j < i; ++j)
				if (_pointList[i].Distance(_pointList[j]) < 1e-6) return false;
		}

		Handle(Geom_TrimmedCurve) tc = GC_MakeArcOfCircle(pt1, pt2, pt3);
		TopoDS_Edge makeArc = BRepBuilderAPI_MakeEdge(tc);
	
		Handle(Geom_Circle) c = GC_MakeCircle(pt1, pt2, pt3);
		gp_Pnt center = c->Location();
		TopoDS_Vertex centerVer = BRepBuilderAPI_MakeVertex(center);

		TopoDS_Compound aRes;
		BRep_Builder aBuilder;
		aBuilder.MakeCompound(aRes);
		aBuilder.Add(aRes, makeArc);
		aBuilder.Add(aRes, centerVer);

		if (makeArc.IsNull())  return false;
		_resShape = new TopoDS_Shape;
		*_resShape = aRes;
		emit showSketch(_resShape);
		return GeoCommandBase::execute();
	}

	void GeoCommanSketchArc::undo()
	{
		emit removeSketch(_resShape);
	}

	void GeoCommanSketchArc::redo()
	{
		emit showSketch(_resShape);
	}
}