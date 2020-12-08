#include "GeoCommandSketchPolyline.h"
#include <BRepBuilderAPI_MakeEdge.hxx>
#include "geometry/geometrySet.h"
#include "geometry/geometryData.h"
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>
namespace Command
{
	GeoCommanSketchPolyline::GeoCommanSketchPolyline(MainWidget::SketchViewProvider* v, GUI::MainWindow* m, MainWidget::PreWindow* p)
		:GeocommandSketchBase(v, p, m)
	{
	}



	bool GeoCommanSketchPolyline::execute()
	{
		if (_pointList.size() < 2) return false;

		TopoDS_Compound aRes;
		BRep_Builder aBuilder;
		aBuilder.MakeCompound(aRes);
		for (int i = 0; i < _pointList.size() - 1; ++i)
		{
			BRepBuilderAPI_MakeEdge edge(_pointList[i], _pointList[i + 1]);
			aBuilder.Add(aRes, edge);
		}

		if (_isClose && _pointList.size() > 2)
		{
			BRepBuilderAPI_MakeEdge edge(_pointList.last(), _pointList.first());
			aBuilder.Add(aRes, edge);
		}

		if (aRes.IsNull())  return false;
		_resShape = new TopoDS_Shape;
		*_resShape = aRes;
		emit showSketch(_resShape);
		return true;
	}

	void GeoCommanSketchPolyline::undo()
	{
		emit removeSketch(_resShape);
	}

	void GeoCommanSketchPolyline::redo()
	{
		emit showSketch(_resShape);
	}

	void GeoCommanSketchPolyline::isClose(bool c)
	{
		_isClose = c;
	}



}