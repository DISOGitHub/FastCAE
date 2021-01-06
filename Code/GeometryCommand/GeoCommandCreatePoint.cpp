#include "GeoCommandCreatePoint.h"
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS.hxx>
#include "geometry/geometrySet.h"
#include "geometry/geometryData.h"
#include "geometry/geometryParaPoint.h"

namespace Command
{
	GeoCommandCreatePoint::GeoCommandCreatePoint(GUI::MainWindow* m, MainWidget::PreWindow* p)
		: GeoCommandBase(m,p)
	{
	}

	void GeoCommandCreatePoint::setCoordinate(double* c)
	{
		for (int i = 0; i < 3; ++i)
		{
			_coordinate[i] = c[i];
		}
	}

	void GeoCommandCreatePoint::setCorner(double* c)
	{
		for (int i = 0; i < 3; ++i)
		{
			_corner[i] = c[i];
		}
	}

	void GeoCommandCreatePoint::setPara(double* s)
	{
		for (int i = 0; i < 3; ++i)
		{
			_para[i] = s[i];
		}
	}

	void GeoCommandCreatePoint::setName(QString name)
	{
		_name = name;
	}

	bool GeoCommandCreatePoint::execute()
	{
		TopoDS_Vertex aVertex = BRepBuilderAPI_MakeVertex(gp_Pnt(_coordinate[0], _coordinate[1], _coordinate[2]));
		if(aVertex.IsNull())  return false;
		TopoDS_Shape* shape = new TopoDS_Shape;
		*shape = aVertex;
		if (shape->IsNull()) return false;

		Geometry::GeometrySet* set = new Geometry::GeometrySet;
		set->setName(_name);
		set->setShape(shape);
		//_geoData->appendGeometrySet(set);
		_result = set;

		if (_isEdit)
		{
			set->setName(_editSet->getName());
			_geoData->replaceSet(set, _editSet);
			emit removeDisplayActor(_editSet);
		}
		else
		{
			set->setName(_name);
			_geoData->appendGeometrySet(set);
		}

		Geometry::GeometryParaPoint* para = new Geometry::GeometryParaPoint;
		para->setName(_name);
		para->setPara(_para);
		para->setCorner(_corner);
		_result->setParameter(para);

		GeoCommandBase::execute();
		emit updateGeoTree();
		emit showSet(set);
		return true;
	}

	void GeoCommandCreatePoint::undo()
	{
		emit removeDisplayActor(_result);
		if (_isEdit)
		{
			_geoData->replaceSet(_editSet, _result);
			emit showSet(_editSet);
		}
		else
		{
			_geoData->removeTopGeometrySet(_result);
		}
		emit updateGeoTree();
	}

	void GeoCommandCreatePoint::redo()
	{
		if (_isEdit)
		{
			_geoData->replaceSet(_result, _editSet);
			emit removeDisplayActor(_editSet);
		}
		else
			_geoData->appendGeometrySet(_result);

		emit updateGeoTree();
		emit showSet(_result);
	}

	void GeoCommandCreatePoint::releaseResult()
	{
		if (_result != nullptr) delete _result;
	}


}
