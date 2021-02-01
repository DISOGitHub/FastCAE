#include "GeoCommandSketchSpline.h"
#include <TColgp_Array1OfPnt.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColStd_Array1OfInteger.hxx>
#include <Geom_BSplineCurve.hxx>
#include <GeomAPI_Interpolate.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <TopoDS_Edge.hxx>
#include <Precision.hxx>


namespace Command
{
	GeoCommanSketchSpline::GeoCommanSketchSpline(MainWidget::SketchViewProvider* v, GUI::MainWindow* m, MainWidget::PreWindow* p)
		:GeocommandSketchBase(v, p, m)
	{
	}

	bool GeoCommanSketchSpline::execute()
	{
		const int nPt = _pointList.size();
		if (nPt <= 2) return false;
		Handle_TColgp_HArray1OfPnt aPoints = new TColgp_HArray1OfPnt(1, nPt);
		for (int i = 0; i < nPt; ++i)
		{
			gp_Pnt point = _pointList.at(i);
			aPoints->SetValue(i+1, point);
		}

		GeomAPI_Interpolate aInterpolater(aPoints, Standard_False, Precision::Approximation());

		aInterpolater.Perform();
		if (!aInterpolater.IsDone()) return false;
 		TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(aInterpolater.Curve());
		if (edge.IsNull()) return false;
	
		_resShape = new TopoDS_Shape;
		*_resShape = edge;
		emit showSketch(_resShape);

		return GeoCommandBase::execute();
	}

	void GeoCommanSketchSpline::undo()
	{
		emit removeSketch(_resShape);
	}

	void GeoCommanSketchSpline::redo()
	{
		emit showSketch(_resShape);
	}



}