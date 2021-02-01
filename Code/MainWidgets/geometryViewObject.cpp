#include "GeometryViewObject.h"
#include "geometrySetViewData.h"
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkDataArray.h>
#include "settings/busAPI.h"
#include "settings/GraphOption.h"

namespace  MainWidget
{
	GeometryViewObject::GeometryViewObject(ViewObjType type, int beg, int end, Handle(TopoDS_TShape) ts)
		:_type(type), _begin(beg), _end(end), _tshape(ts) {}


	void GeometryViewObject::getRange(int& beg, int &end)
	{
		beg = _begin; end = _end;
	}

	void GeometryViewObject::appendSolidFace(int f)
	{
		_solidFaces.append(f);
	}

	QList<int> GeometryViewObject::getSolidFace()
	{
		return _solidFaces;
	}


	void GeometryViewObject::setIndex(int index)
	{
		_index = index;
	}

	int GeometryViewObject::getIndex()
	{
		return _index;
	}


	Geometry::GeometrySet* GeometryViewObject::getGeometySet()
	{
		if (_setViewObj == nullptr) return nullptr;
		return _setViewObj->getGeometrySet();
	}

	GeoSetViewObject* GeometryViewObject::getGeoSetViewObject()
	{
		return _setViewObj;
	}


	GeometryViewObject::ViewStates GeometryViewObject::getStates()
	{
		return _states;
	}


	void GeometryViewObject::setStates(ViewStates s)
	{
		_states = s;;
	}

	void GeometryViewObject::setConnectionData(vtkPolyData* poly, GeoSetViewObject* setViewObj)
	{
		_polys = poly;
		_setViewObj = setViewObj;
	}

	void GeometryViewObject::resetColor()
	{
		QColor color;
		switch (_type)
		{
		case MainWidget::GeometryViewObject::Point:
			color = Setting::BusAPI::instance()->getGraphOption()->getGeometryPointColor(); break;
		case MainWidget::GeometryViewObject::Edge:
			color = Setting::BusAPI::instance()->getGraphOption()->getGeometryCurveColor(); break;
		case MainWidget::GeometryViewObject::Face:
		case MainWidget::GeometryViewObject::Solid:
			color = Setting::BusAPI::instance()->getGraphOption()->getGeometrySurfaceColor(); break;
		default:break;
		}
		this->setObjectColor(color);
		_states = Normal;
	}


	void GeometryViewObject::preHighLight()
	{
		QColor c = Setting::BusAPI::instance()->getGraphOption()->getPreHighLightColor();
		this->setObjectColor(c);
		_states = PreHigh;
	}


	void GeometryViewObject::highLight()
	{
		QColor c = Setting::BusAPI::instance()->getGraphOption()->getHighLightColor();
		this->setObjectColor(c);
		_states = HighLigh;
	}

	void GeometryViewObject::setObjectColor(QColor color)
	{
		vtkDataArray* dataArray = nullptr;
		if (_type == Solid)
		{
			setSolidColor(color);
			return;
		}
		if (_polys == nullptr) return;
		if (_type == Point)
			dataArray = _polys->GetPointData()->GetScalars();
		else
			dataArray = _polys->GetCellData()->GetScalars();
		for (int i = _begin; i <= _end; ++i)
		{
			dataArray->SetComponent(i, 0, color.red());
			dataArray->SetComponent(i, 1, color.green());
			dataArray->SetComponent(i, 2, color.blue());
		}
		dataArray->Modified();
	}

	void GeometryViewObject::setSolidColor(QColor color)
	{
		for (int index : _solidFaces)
		{
			auto vobj = _setViewObj->getFaceViewObjByIndex(index);
			if (vobj != nullptr) vobj->setObjectColor(color);
		}
	}

}
