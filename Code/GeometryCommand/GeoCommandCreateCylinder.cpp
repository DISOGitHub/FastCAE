#include "GeoCommandCreateCylinder.h"
#include <gp_Ax2.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include "geometry/geometryParaCylinder.h"
#include <QDebug>
namespace Command
{
	GeoCommandCreateCylinder::GeoCommandCreateCylinder(GUI::MainWindow* m, MainWidget::PreWindow* p)
		:GeoCommandBase(m, p)
	{
	}

	bool GeoCommandCreateCylinder::execute()
	{
		gp_Ax2 anAxis;
		anAxis.SetLocation(gp_Pnt(_location[0], _location[1], _location[2]));
		anAxis.SetDirection(gp_Dir(_axis[0], _axis[1], _axis[2]));

		TopoDS_Shape aTopoCylinder = BRepPrimAPI_MakeCylinder(anAxis, _radius, _length).Shape();

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

		Geometry::GeometryParaCylinder* para = new Geometry::GeometryParaCylinder;
		para->setName(_name);
		para->setLocation(_location);
		para->setRadius(&_radius);
		para->setLength(&_length);
		para->setDirection(_axis);
		_result->setParameter(para);

		GeoCommandBase::execute();
		emit updateGeoTree();
		emit showSet(set);
		return true;
	}

	void GeoCommandCreateCylinder::undo()
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

	void GeoCommandCreateCylinder::redo()
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

	void GeoCommandCreateCylinder::setLocation(double* loc)
	{
		_location[0] = loc[0];
		_location[1] = loc[1];
		_location[2] = loc[2];
	}

	void GeoCommandCreateCylinder::setAxis(double* axis)
	{
		_axis[0] = axis[0];
		_axis[1] = axis[1];
		_axis[2] = axis[2];
	}

	void GeoCommandCreateCylinder::setRadius(double r)
	{
		_radius = r;
	}

	void GeoCommandCreateCylinder::setLength(double l)
	{
		_length = l;
	}

	void GeoCommandCreateCylinder::setName(QString name)
	{
		_name = name;
	}

	void GeoCommandCreateCylinder::releaseResult()
	{
		if (_result != nullptr) delete _result;
		_result = nullptr;
	}




}