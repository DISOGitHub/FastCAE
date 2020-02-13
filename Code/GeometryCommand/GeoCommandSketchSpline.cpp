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
//		TColgp_Array1OfPnt Poles(1, nPt);
		Handle_TColgp_HArray1OfPnt aPoints = new TColgp_HArray1OfPnt(1, nPt);
//		TColStd_Array1OfReal PolesWeight(1, nPt);

		for (int i = 0; i < nPt; ++i)
		{
			gp_Pnt point = _pointList.at(i);
			aPoints->SetValue(i+1, point);
//			PolesWeight.SetValue(i+1, 1.0);
		}

// 		Standard_Integer degree(2);
// 		Standard_Integer PNum = nPt; 
// 		Standard_Integer KNum = PNum - 1; 
// 		TColStd_Array1OfReal knots(1, KNum); 
// 		for (int i = 0; i < KNum; ++i) 
// 		{
// 			knots.SetValue(i + 1, i); 
// 		}
// 		TColStd_Array1OfInteger mults(1, KNum); 
// 		for (int i = 0; i < KNum; ++i)
// 		{ 
// 			if (i == 0 || i == KNum - 1)
// 			{
// 				mults.SetValue(i + 1, degree + 1); 
// 			} 
// 			else
// 			{
// 				mults.SetValue(i + 1, 1);
// 			} 
// 		}
// 		Handle(Geom_BSplineCurve) curve = new Geom_BSplineCurve(Poles, PolesWeight, knots, mults, degree); 

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