#include "GeoCommandCreateLine.h"
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS.hxx>
#include "geometry/geometrySet.h"
#include "geometry/geometryData.h"
#include "geometry/geometryParaLine.h"

namespace Command
{
	GeoCommandCreateLine::GeoCommandCreateLine(GUI::MainWindow* m, MainWidget::PreWindow* p)
		: GeoCommandBase(m,p)
	{
	}

	void GeoCommandCreateLine::setPoints(double* s)
	{
		for (int i = 0; i < 3; ++i)
		{
			_startPoint[i] = s[i];
		}
	}

	void GeoCommandCreateLine::setOptionIndex(int i)
	{
		_index = i;
	}

	void GeoCommandCreateLine::setCoor(double* c)
	{
		for (int i = 0; i < 3; i++)
		{
			_coor[i] = c[i];
		}
	}

	void GeoCommandCreateLine::setLength(double l)
	{
		_length = l;
	}

	void GeoCommandCreateLine::setDir(double *d)
	{
		for (int i = 0; i < 3; i++)
		{
			_dir[i] = d[i];
		}
	}

	void GeoCommandCreateLine::setReverse(bool b)
	{
		_reverse = b;
	}

	bool GeoCommandCreateLine::getEndPoint(double* endpt)
	{
		bool ok = true;
		if (_index==0)
		{
			for (int i = 0; i < 3;++i)
			{
				endpt[i] = _coor[i];
			}
		}
		else
		{
			double length = _length;
			if (length < 1e-6) ok = false;
			if (_reverse)
			{
				for (int i = 0; i < 3; ++i)
					_dir[i] *= -1;
			}
			double mod = sqrt(_dir[0] * _dir[0] + _dir[1] * _dir[1] + _dir[2] * _dir[2]);
			double fac = length / mod;

			
			for (int i = 0; i < 3; ++i)
				endpt[i] = _startPoint[i] + fac * _dir[i];
		}
		return ok;
	}

	void GeoCommandCreateLine::setName(QString name)
	{
		_name = name;
	}

	bool GeoCommandCreateLine::execute()
	{
		
		bool ok = this->getEndPoint(_endPoint);
	
		const TopoDS_Edge& aedge = BRepBuilderAPI_MakeEdge(gp_Pnt(_startPoint[0], _startPoint[1], _startPoint[2]), gp_Pnt(_endPoint[0], _endPoint[1], _endPoint[2]));
		if (aedge.IsNull())  return false;
		TopoDS_Shape* shape = new TopoDS_Shape;
		*shape = aedge;
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

		Geometry::GeometryParaLine* para = new Geometry::GeometryParaLine;
		para->setName(_name);
		para->setStartPoint(_startPoint);
		para->setOptionIndex(_index);
		para->setCoor(_coor);
		para->setLength(_length);
		para->setDirection(_dir);
		para->setReverse(_reverse);
		_result->setParameter(para);

		GeoCommandBase::execute();
		emit showSet(set);
		emit updateGeoTree();
		return true;
	}

	void GeoCommandCreateLine::undo()
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

	void GeoCommandCreateLine::redo()
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

	void GeoCommandCreateLine::releaseResult()
	{
		if (_result != nullptr) delete _result;
	}


}