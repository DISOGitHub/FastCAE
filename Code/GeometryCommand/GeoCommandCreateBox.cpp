#include "GeoCommandCreateBox.h"
#include <BRepPrimAPI_MakeBox.hxx>
#include <TopoDS.hxx>
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include "geometry/geometryParaBox.h"

namespace Command
{
	CommandCreateBox::CommandCreateBox(GUI::MainWindow* m, MainWidget::PreWindow* p)
		:GeoCommandBase(m, p)
	{
	}

	void CommandCreateBox::setName(QString name)
	{
		_name = name;
	}

	void CommandCreateBox::setLocation(double* loca)
	{
		_loaction[0] = loca[0];
		_loaction[1] = loca[1];
		_loaction[2] = loca[2];
	}

	void CommandCreateBox::setGeoPara(double* para)
	{
		_geoPara[0] = para[0];
		_geoPara[1] = para[1];
		_geoPara[2] = para[2];
	}

	bool CommandCreateBox::execute()
	{
		gp_Pnt pt(_loaction[0], _loaction[1], _loaction[2]);
		TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(pt, _geoPara[0], _geoPara[1], _geoPara[2]).Shape();
		TopoDS_Shape* shape = new TopoDS_Shape;
		*shape = aTopoBox;
		if (shape->IsNull())
		{
			delete shape;
			return false;
		}

		
		Geometry::GeometrySet* set = new Geometry::GeometrySet(Geometry::STEP);
		
		set->setShape(shape);
		_res = set;

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

		Geometry::GeometryParaBox* para = new Geometry::GeometryParaBox;
		para->setName(_name);
		para->setLocation(_loaction);
		para->setGeoPara(_geoPara);
		_res->setParameter(para);

		
		GeoCommandBase::execute();
		emit updateGeoTree();
		emit showSet(set);
		return true;
	}

	void CommandCreateBox::undo()
	{
		emit removeDisplayActor(_res);
		if (_isEdit)
		{
			_geoData->replaceSet(_editSet, _res);
			emit showSet(_editSet);
		}
		else
		{
			_geoData->removeTopGeometrySet(_res);
		}
		
		emit updateGeoTree();
	}

	void CommandCreateBox::redo()
	{
		if (_isEdit)
		{
			_geoData->replaceSet(_res, _editSet);
			emit removeDisplayActor(_editSet);
		}
		else
			_geoData->appendGeometrySet(_res);
		
		emit updateGeoTree();
		emit showSet(_res);
	}

	void CommandCreateBox::releaseResult()
	{
		if (_res != nullptr) delete _res;
		_res = nullptr;
	}



}