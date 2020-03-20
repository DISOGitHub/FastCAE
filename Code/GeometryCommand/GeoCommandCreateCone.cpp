#include "GeoCommandCreateCone.h"
#include <gp_Ax2.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include "geometry/geometryParaCone.h"

namespace Command
{
	GeoCommandCreateCone::GeoCommandCreateCone(GUI::MainWindow* m, MainWidget::PreWindow* p)
		:GeoCommandBase(m, p)
	{
	}

	bool GeoCommandCreateCone::execute()
	{
		gp_Ax2 anAxis;
		anAxis.SetLocation(gp_Pnt(_location[0], _location[1], _location[2]));
		anAxis.SetDirection(gp_Dir(_axis[0], _axis[1], _axis[2]));

		TopoDS_Shape aTopoCylinder = BRepPrimAPI_MakeCone(anAxis, _radius1, _radius2, _length).Shape();

		TopoDS_Shape* shape = new TopoDS_Shape;
		*shape = aTopoCylinder;
		if (shape->IsNull())
		{
			delete shape;
			return false;
		}

		Geometry::GeometrySet* set = new Geometry::GeometrySet(Geometry::STEP);
		set->setName(_name);
		set->setShape(shape);

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

		Geometry::GeometryParaCone* para = new Geometry::GeometryParaCone;
		para->setName(_name);
		para->setLocation(_location);
		para->setRadius1(&_radius1);
		para->setRadius2(&_radius2);
		para->setLength(&_length);
		para->setDirection(_axis);
		_result->setParameter(para);

		GeoCommandBase::execute();
		emit updateGeoTree();
		emit showSet(set);
		return  true;
	}

	void GeoCommandCreateCone::undo()
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

	void GeoCommandCreateCone::redo()
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

	void GeoCommandCreateCone::setLocation(double* loc)
	{
		_location[0] = loc[0];
		_location[1] = loc[1];
		_location[2] = loc[2];
	}

	void GeoCommandCreateCone::setAxis(double* axis)
	{
		_axis[0] = axis[0];
		_axis[1] = axis[1];
		_axis[2] = axis[2];
	}


	void GeoCommandCreateCone::setRadius(double r1, double r2)
	{
		_radius1 = r1;
		_radius2 = r2;
	}

	void GeoCommandCreateCone::setLength(double l)
	{
		_length = l;
	}

	void GeoCommandCreateCone::setName(QString name)
	{
		_name = name;
	}

	void GeoCommandCreateCone::releaseResult()
	{
		if (_result != nullptr) delete _result;
		_result = nullptr;
	}




}
