#include "GeoCommandCreateSphere.h"
#include <BRepPrimAPI_MakeSphere.hxx>
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include "geometry/geometryParaSphere.h"

namespace Command
{
	GeoCommandCreateSphere::GeoCommandCreateSphere(GUI::MainWindow* m, MainWidget::PreWindow* p)
		: GeoCommandBase(m, p)
	{

	}

	bool GeoCommandCreateSphere::execute()
	{
		gp_Pnt pt(_location[0], _location[1], _location[2]);
		TopoDS_Shape aTopoBox = BRepPrimAPI_MakeSphere(pt, _radius).Shape();
		TopoDS_Shape* shape = new TopoDS_Shape;
		*shape = aTopoBox;
		if (shape->IsNull())
		{
			delete shape;
			return false;
		}
		Geometry::GeometrySet* set = new Geometry::GeometrySet(Geometry::STEP);
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

		Geometry::GeometryParaSphere* para = new Geometry::GeometryParaSphere;
		para->setName(_name);
		para->setLocation(_location);
		para->setRadius(_radius);
		_result->setParameter(para);

		GeoCommandBase::execute();
		emit updateGeoTree();
		emit showSet(set);
		return true;
	}

	void GeoCommandCreateSphere::undo()
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

		emit updateGeoTree();;
	}

	void GeoCommandCreateSphere::redo()
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

	void GeoCommandCreateSphere::releaseResult()
	{
		if (_result != nullptr) delete _result;
		_result = nullptr;
	}

	void GeoCommandCreateSphere::setLocation(double* loc)
	{
		_location[0] = loc[0];
		_location[1] = loc[1];
		_location[2] = loc[2];
	}

	void GeoCommandCreateSphere::setRadius(double r)
	{
		_radius = r;
	}

	void GeoCommandCreateSphere::setName(QString name)
	{
		_name = name;
	}

}