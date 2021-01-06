#include "GeoCommandCreateDatumplane.h"
#include <gp_Ax2.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include "geometry/geometryDatum.h"
#include <gp_Pln.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <Bnd_Box.hxx>
#include <gp_Trsf.hxx>
#include <TopoDS.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Face.hxx>
#include <BRepAlgo_Section.hxx>
#include <BRepLib.hxx>
#include <BRepBndLib.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgo_Section.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include "GeoCommandCommon.h"
#include <QDebug>


namespace Command
{
	GeoCommandCreateDatumplane::GeoCommandCreateDatumplane(GUI::MainWindow* m, MainWidget::PreWindow* p)
		:GeoCommandBase(m,p)
	{

	}
	bool GeoCommandCreateDatumplane::execute()
	{
		TopoDS_Shape* faceBelong = _face->getShape();
		TopExp_Explorer faceExp(*faceBelong, TopAbs_FACE);
		for (int index = 0; index < _faceIndex && faceExp.More(); faceExp.Next(), ++index);

		const TopoDS_Shape& faceShape = faceExp.Current();
		if (faceShape.IsNull()) return false;

		Bnd_Box bndBox;
		BRepBndLib::Add(faceShape, bndBox);
		bndBox.SetGap(0.01);

		gp_Pnt cornerMin = bndBox.CornerMin();
		gp_Pnt cornerMax = bndBox.CornerMax();
		TopoDS_Shape newBox = BRepPrimAPI_MakeBox(cornerMin, cornerMax).Shape();

		gp_Pln plan(gp_Pnt(_planeLocation[0], _planeLocation[1], _planeLocation[2]), gp_Dir(_planeDirection[0], _planeDirection[1], _planeDirection[2]));
		TopoDS_Shape planeShape = BRepBuilderAPI_MakeFace(plan);

		BRepAlgoAPI_Common S(newBox, planeShape);
		S.Build();
		if (!S.IsDone()) return false;
		const TopoDS_Shape& cutShape = S.Shape();
		if (cutShape.IsNull()) return false;
		
		gp_Trsf aTrsf;
		gp_Vec vec(_axis[0], _axis[1], _axis[2]);
		aTrsf.SetTranslation(vec);
		BRepBuilderAPI_Transform aBRespTrsf(cutShape, aTrsf);
		aBRespTrsf.Build();
		if (!aBRespTrsf.IsDone()) return false;
		const TopoDS_Shape& resShape = aBRespTrsf.Shape();
		if (resShape.IsNull()) return false;


		auto s = new TopoDS_Shape;
		*s = resShape;
	    _result = new Geometry::GeometryDatum();
		_result->setDatumType(Geometry::DatumPlane);
		_result->setName(QString("Datum"));
		_result->setShape(s);
		_geoData->appendGeometryDatum(_result);
		updateGeoTree();
		emit showDatum(_result);

		return true;

    }
	void GeoCommandCreateDatumplane::undo()
	{
		_geoData->removeGeometryDatum(_result);
		emit removeDisplayDatumActor(_result); 
		updateGeoTree();
	}

	void GeoCommandCreateDatumplane::redo()
	{
		_geoData->appendGeometryDatum(_result);
		emit showDatum(_result);
		updateGeoTree();
	}

	void GeoCommandCreateDatumplane::setAxis(double* axis)
	{
		_axis[0] = axis[0];
		_axis[1] = axis[1];
		_axis[2] = axis[2];
	}

	void GeoCommandCreateDatumplane::setPlane(double* loc, double* dir)
	{
		for (int i = 0; i < 3; ++i)
		{
			_planeDirection[i] = dir[i];
			_planeLocation[i] = loc[i];
		}
	}

	void GeoCommandCreateDatumplane::setFaceBody(Geometry::GeometrySet* face, int index)
	{
		_face = face;
		_faceIndex = index;
	}

	TopoDS_Shape* GeoCommandCreateDatumplane::common(TopoDS_Shape* shape1, TopoDS_Shape* shape2)
	{
		
		BRepAlgoAPI_Common S(*shape1, *shape2);
		S.Build();
		if (S.IsDone())
		{
			auto s = new TopoDS_Shape;
			*s = S.Shape();
			return s;
		}
	}

	void GeoCommandCreateDatumplane::releaseResult()
	{
		if (_result != nullptr) delete _result;
		_result = nullptr;
	}
}
