#include "GeoCommandSketchLine.h"
#include <gp_Pnt.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <TopoDS_Edge.hxx>
#include "geometry/geometryData.h"


namespace Command
{
	GeoCommanSketchLine::GeoCommanSketchLine(MainWidget::SketchViewProvider* v, GUI::MainWindow* m, MainWidget::PreWindow* p)
		:GeocommandSketchBase(v, p, m)
	{
	}

	bool GeoCommanSketchLine::execute()
	{
		if (_pointList.size() != 2) return false;

		gp_Pnt pt1 = _pointList[0];
		gp_Pnt pt2 = _pointList[1];
		if (pt1.Distance(pt2) < 1e-6) return false;

		const TopoDS_Edge& aedge = BRepBuilderAPI_MakeEdge(pt1, pt2);
		if (aedge.IsNull())  return false;
		_resShape = new TopoDS_Shape;
		*_resShape = aedge;

		emit showSketch(_resShape);
		return GeoCommandBase::execute();
	}

	void GeoCommanSketchLine::undo()
	{
		emit removeSketch(_resShape);
	}

	void GeoCommanSketchLine::redo()
	{
		emit showSketch(_resShape);
	}



}