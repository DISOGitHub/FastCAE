#include "geometrySetViewData.h"
#include "geometry/geometrySet.h"
#include <QDebug>
#include <vtkPolyData.h>
#include <TopoDS_Shape.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS.hxx>
#include <vtkAppendPolyData.h>
#include <IVtkOCC_Shape.hxx>
#include <IVtkTools_ShapeDataSource.hxx>
#include <vtkCleanPolyData.h>
#include <vtkPolyDataNormals.h>
#include <vtkTriangle.h>
#include <vtkSmartPointer.h>
#include <vtkPointData.h>
#include <vtkUnsignedCharArray.h>
#include <vtkCellData.h>
#include "settings/busAPI.h"
#include "settings/GraphOption.h"
#include "geometryViewObject.h"
#include "moduleBase/ModuleType.h"


namespace MainWidget
{
	GeoSetViewObject::GeoSetViewObject(Geometry::GeometrySet* set)
		:_geoSet(set) {}


	GeoSetViewObject::~GeoSetViewObject()
	{
		if (nullptr != _facePoly) _facePoly->Delete();
		if (nullptr != _edgePoly) _edgePoly->Delete();
		if (nullptr != _pointPoly) _pointPoly->Delete();

		QList<GeometryViewObject*> vs = _pointViewObjs.values();
		vs.append(_edgeViewObjs.values());
		vs.append(_faceViewObjs.values());
		vs.append(_solidViewObjs.values());

		for (auto v : vs) delete v;
		_pointViewObjs.clear();
		_edgeViewObjs.clear();
		_faceViewObjs.clear();
		_solidViewObjs.clear();
		_faceObjHash.clear();
	}

	Geometry::GeometrySet* GeoSetViewObject::getGeometrySet()
	{
		return _geoSet;
	}

	void GeoSetViewObject::setFacePoly(vtkPolyData* p)
	{
		_facePoly = p;
		QColor color = Setting::BusAPI::instance()->getGraphOption()->getGeometrySurfaceColor();
		unsigned char colorc[3] = { color.red(),color.green(),color.blue() };
		vtkSmartPointer<vtkUnsignedCharArray> cellcolor = vtkSmartPointer<vtkUnsignedCharArray>::New();
		cellcolor->SetNumberOfComponents(3);
		const int n = _facePoly->GetNumberOfCells();
		for (int i = 0; i < n; ++i)
			cellcolor->InsertNextTypedTuple(colorc);
		_facePoly->GetCellData()->SetScalars(cellcolor);
		QList<GeometryViewObject*> objs = _faceViewObjs.values();
		for (auto obj : objs)
			obj->setConnectionData(_facePoly, this);
	}


	vtkPolyData* GeoSetViewObject::getFacePoly()
	{
		return _facePoly;
	}

	void GeoSetViewObject::setEdgePoly(vtkPolyData* p)
	{
		_edgePoly = p;
		QColor color = Setting::BusAPI::instance()->getGraphOption()->getGeometryCurveColor();
		unsigned char colorc[3] = { color.red(),color.green(),color.blue() };
		vtkSmartPointer<vtkUnsignedCharArray> cellcolor = vtkSmartPointer<vtkUnsignedCharArray>::New();
		cellcolor->SetNumberOfComponents(3);
		const int n = _edgePoly->GetNumberOfCells();
		for (int i = 0; i < n; ++i)
			cellcolor->InsertNextTypedTuple(colorc);
		_edgePoly->GetCellData()->SetScalars(cellcolor);
		QList<GeometryViewObject*> objs = _edgeViewObjs.values();
		for (auto obj : objs)
			obj->setConnectionData(_edgePoly, this);
	}


	vtkPolyData* GeoSetViewObject::getEdgePoly()
	{
		return _edgePoly;
	}

	void GeoSetViewObject::setPointPoly(vtkPolyData* p)
	{
		_pointPoly = p;
		vtkSmartPointer<vtkUnsignedCharArray> cellcolor = vtkSmartPointer<vtkUnsignedCharArray>::New();
		cellcolor->SetNumberOfComponents(3);
		const int n = _pointPoly->GetNumberOfPoints();
		QColor color = Setting::BusAPI::instance()->getGraphOption()->getGeometryPointColor();
		unsigned char colorc[3] = { color.red(),color.green(),color.blue() };
		for (int i = 0; i < n; ++i)
			cellcolor->InsertNextTypedTuple(colorc);
		_pointPoly->GetPointData()->SetScalars(cellcolor);
		QList<GeometryViewObject*> objs = _pointViewObjs.values();
		for (auto obj : objs)
			obj->setConnectionData(_pointPoly, this);
	}

	vtkPolyData* GeoSetViewObject::getPointPoly()
	{
		return _pointPoly;
	}

	void GeoSetViewObject::genSolidViewObj()
	{
		TopoDS_Shape* shape = _geoSet->getShape();
		TopExp_Explorer solidExp(*shape, TopAbs_SOLID);
		for (int index = 0; solidExp.More(); solidExp.Next(), ++index)
		{
			const TopoDS_Shape& asolid = solidExp.Current();
			GeometryViewObject*  solidView = new GeometryViewObject(GeometryViewObject::Solid, 0, 0, asolid.TShape());
			TopExp_Explorer faceExp(asolid, TopAbs_FACE);
			for (; faceExp.More(); faceExp.Next())
			{
				Handle(TopoDS_TShape) tshape = faceExp.Current().TShape();
				auto faceObj = _faceObjHash.key(tshape);
				if (faceObj == nullptr) continue;
				int faceindex = _faceViewObjs.key(faceObj);
				if (faceindex < 0) continue;
				solidView->appendSolidFace(faceindex);
			}
			_solidViewObjs.insert(index, solidView);
			solidView->setIndex(index);
			solidView->setConnectionData(nullptr, this);
		}
	}

	void GeoSetViewObject::appendFaceViewObj(int index, Handle(TopoDS_TShape) s, GeometryViewObject* v)
	{
		_faceObjHash.insert(v, s);
		_faceViewObjs.insert(index, v);
		v->setIndex(index);
	}

	void GeoSetViewObject::appendEdgeViewObj(int index, GeometryViewObject* v)
	{
		_edgeViewObjs.insert(index, v);
		v->setIndex(index);
	}

	void GeoSetViewObject::appendPointViewObj(int index, GeometryViewObject* v)
	{
		_pointViewObjs.insert(index, v);
		v->setIndex(index);
	}

	void GeoSetViewObject::appendSolidViewObj(int index, GeometryViewObject* v)
	{
		_solidViewObjs.insert(index, v);
		v->setIndex(index);
	}


	MainWidget::GeometryViewObject* GeoSetViewObject::getFaceViewObjByIndex(int index)
	{
		if (_faceViewObjs.contains(index))
			return  _faceViewObjs.value(index);
		return nullptr;
	}


	GeometryViewObject* GeoSetViewObject::getFaceViewObjByCellIndex(int cellIndex)
	{
		QList<GeometryViewObject*> viewObjs = _faceViewObjs.values();
		for (auto view : viewObjs)
		{
			int beg = -1, end = -1;
			view->getRange(beg, end);
			if (cellIndex >= beg && cellIndex <= end)
				return view;
		}
		return nullptr;
	}


	GeometryViewObject* GeoSetViewObject::getEdgeViewObjByCellIndex(int cellIndex)
	{
		QList<GeometryViewObject*> viewObjs = _edgeViewObjs.values();
		for (auto view : viewObjs)
		{
			int beg = -1, end = -1;
			view->getRange(beg, end);
			if (cellIndex >= beg && cellIndex <= end)
				return view;
		}
		return nullptr;
	}


	GeometryViewObject* GeoSetViewObject::getPointViewObjByCellIndex(int pointIndex)
	{
		QList<GeometryViewObject*> viewObjs = _pointViewObjs.values();
		for (auto view : viewObjs)
		{
			int beg = -1, end = -1;
			view->getRange(beg, end);
			if (pointIndex >= beg && pointIndex <= end)
				return view;
		}
		return nullptr;
	}


	GeometryViewObject* GeoSetViewObject::getSolidViewObjByFaceIndex(int faceIndex)
	{
		QList<GeometryViewObject*> viewObjs = _solidViewObjs.values();
		for (auto view : viewObjs)
		{
			QList<int> faces = view->getSolidFace();
			if (faces.contains(faceIndex))
				return view;
		}
		return nullptr;
	}

	QList<GeometryViewObject*> GeoSetViewObject::getViewObjectByStates(int selectModel, int sates)
	{
		ModuleBase::SelectModel m = (ModuleBase::SelectModel)selectModel;
		GeometryViewObject::ViewStates  s = (GeometryViewObject::ViewStates)sates;
		QList<GeometryViewObject*> objects;
		switch (m)
		{
		case ModuleBase::GeometryWinBody:
		case ModuleBase::GeometryBody:
			objects = _solidViewObjs.values(); break;
		case ModuleBase::GeometryWinSurface:
		case ModuleBase::GeometrySurface:
			objects = _faceViewObjs.values(); break;
		case ModuleBase::GeometryWinCurve:
		case ModuleBase::GeometryCurve:
			objects = _edgeViewObjs.values(); break;
		case ModuleBase::GeometryWinPoint:
		case ModuleBase::GeometryPoint:
			objects = _pointViewObjs.values(); break;
		default:break;
		}
		QList<GeometryViewObject*> viewObjs;
		for (auto v : objects)
			if (v->getStates() == sates)
				viewObjs.append(v);

		return viewObjs;
	}

	void GeoSetViewObject::resetColor()
	{
		QList<GeometryViewObject*> viewObjs = _faceViewObjs.values();
		for (auto v : viewObjs)
			v->resetColor();
		viewObjs = _edgeViewObjs.values();
		for (auto v : viewObjs)
			v->resetColor();
		viewObjs = _pointViewObjs.values();
		for (auto v : viewObjs)
			v->resetColor();
		viewObjs = _solidViewObjs.values();
		for (auto v : viewObjs)
			v->setStates(GeometryViewObject::Normal);
	}


	void GeoSetViewObject::highLight(bool on)
	{
		if (!on)
		{
			this->resetColor();
			return;
		}
		QList<GeometryViewObject*> viewObjs = _faceViewObjs.values();
		for (auto v : viewObjs)
			v->highLight();
		viewObjs = _edgeViewObjs.values();
		for (auto v : viewObjs)
			v->highLight();
		viewObjs = _pointViewObjs.values();
		for (auto v : viewObjs)
			v->highLight();
		viewObjs = _solidViewObjs.values();
		for (auto v : viewObjs)
			v->setStates(GeometryViewObject::HighLigh);
	}


	void GeoSetViewObject::highLightFace(int index, bool on)
	{
		auto v = _faceViewObjs.value(index);
		if (nullptr == v) return;
		if (on) v->highLight();
		else v->resetColor();
	}


	void GeoSetViewObject::highLightEdge(int index, bool on)
	{
		auto v = _edgeViewObjs.value(index);
		if (nullptr == v) return;
		if (on) v->highLight();
		else v->resetColor();
	}


	void GeoSetViewObject::highLightPoint(int index, bool on)
	{
		auto v = _pointViewObjs.value(index);
		if (nullptr == v) return;
		if (on) v->highLight();
		else v->resetColor();
	}


	void GeoSetViewObject::highLightSolid(int index, bool on)
	{
		auto v = _solidViewObjs.value(index);
		if (nullptr == v) return;
		if (on) v->highLight();
		else v->resetColor();
	}

}
