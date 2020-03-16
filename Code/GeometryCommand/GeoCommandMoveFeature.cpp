#include "GeoCommandMoveFeature.h"
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include <TopoDS_Shape.hxx>
#include <gp_Ax2.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include "geometry/geometryParaMakeMove.h"

namespace Command
{
	CommandMoveFeature::CommandMoveFeature(GUI::MainWindow* m, MainWidget::PreWindow* p)
		: GeoCommandBase(m, p)
	{
	}

	bool CommandMoveFeature::execute()
	{
		this->getVec();
		gp_Vec vec(_vector[0], _vector[1], _vector[2]);
		

		if (_isEdit) //编辑模式下将原来的模型压进列表
		{
			Geometry::GeometryModelParaBase* pm = _editSet->getParameter();
			Geometry::GeometryParaMakeMove* p = dynamic_cast<Geometry::GeometryParaMakeMove*>(pm);
			if (p == nullptr) return false;

			Geometry::GeometrySet* ori = p->getOriSet();
			if (ori == nullptr) return false;
			if (!p->getSaveOrigion())
			{
				_geoData->appendGeometrySet(ori);
				_editSet->removeSubSet(ori);
			}
			//_bodys.append(ori);
		}

		bool success = false;
		const int count = _bodys.size();
		for (int i = 0; i < count; ++i)
		{
			Geometry::GeometrySet* set = _bodys.at(i);
			TopoDS_Shape* shape = set->getShape();
			gp_Trsf aTrsf;
			aTrsf.SetTranslation(vec);
			BRepBuilderAPI_Transform aBRespTrsf(*shape, aTrsf, true);
			aBRespTrsf.Build();
			if (!aBRespTrsf.IsDone()) continue;
			const TopoDS_Shape& resShape = aBRespTrsf.Shape();
			if (resShape.IsNull()) continue;

			success = true;

			TopoDS_Shape* mshape = new TopoDS_Shape;
			*mshape = resShape;
			QString name = QString("Move-%1").arg(set->getName());

			Geometry::GeometrySet* newset = new Geometry::GeometrySet;
			newset->setName(name);
			newset->setShape(mshape);
			_geoData->appendGeometrySet(newset);

			Geometry::GeometryParaMakeMove* para = new Geometry::GeometryParaMakeMove;
			para->setOriSet(set);
			para->setBodyList(_bodys);
			para->setOptionIndex(_optionindex);
			para->setSaveOrigion(_saveOrigin);
			para->setBasePoint(_startpt);
			para->setEndPoint(_endpt);
			para->setReverse(_reverse);
			para->setLength(_length);
			para->setDirection(_dir);
			newset->setParameter(para);

			emit showSet(newset);
			_resultOriginHash.insert(newset, set);

			if (!_saveOrigin)
			{
				emit removeDisplayActor(set);
				newset->appendSubSet(set);
				_geoData->removeTopGeometrySet(set);
			}
			if (_isEdit)
			{
				_geoData->removeTopGeometrySet(_editSet);
				_releaseEdit = true;     //标记释放状态
				_releasenew = false;
			}
			
		}
		emit updateGeoTree();
		GeoCommandBase::execute();
		return success;
	}

	void CommandMoveFeature::undo()
	{
		if (_isEdit)
		{
			Geometry::GeometryModelParaBase* pm = _editSet->getParameter();
			Geometry::GeometryParaMakeMove* p = dynamic_cast<Geometry::GeometryParaMakeMove*>(pm);
			if (p == nullptr) return;
			Geometry::GeometrySet* ori = p->getOriSet();
			if (ori == nullptr) return;
			auto newset = _resultOriginHash.key(ori);
			if (newset == nullptr) return;
			_geoData->replaceSet(_editSet, newset);
			if (p->getSaveOrigion())
			{
				newset->removeSubSet(ori);
				_editSet->removeSubSet(ori);
				_geoData->removeTopGeometrySet(ori);
				_geoData->appendGeometrySet(ori);
				emit showSet(ori);
				emit showSet(_editSet);
				emit removeDisplayActor(newset);
			}
			else
			{
				newset->removeSubSet(ori);
				_editSet->appendSubSet(ori);
				_geoData->removeTopGeometrySet(ori);

				emit removeDisplayActor(ori);
				emit showSet(_editSet);
				emit removeDisplayActor(newset);
			}
			emit updateGeoTree();
			_releasenew = true;
			_releaseEdit = false;
			return;
		}

		QList<Geometry::GeometrySet*> geoList = _resultOriginHash.keys();
		for (int i = 0; i < geoList.size(); ++i)
		{
			Geometry::GeometrySet* set = geoList.at(i);
			if (!_saveOrigin)
			{
				auto ori = _resultOriginHash.value(set);
				set->removeSubSet(ori);
				_geoData->appendGeometrySet(ori);
				emit showSet(ori);
			}
			_geoData->removeTopGeometrySet(set);
			emit removeDisplayActor(set);
		}
		emit updateGeoTree();
		/*QList<Geometry::GeometrySet*> geoList = _resultOriginHash.keys();
		for (int i = 0; i < geoList.size(); ++i)
		{
			Geometry::GeometrySet* set = geoList.at(i);
			if (!_saveOrigin)
			{
				auto ori = _resultOriginHash.value(set);
				set->removeSubSet(ori);
				_geoData->appendGeometrySet(ori);
				emit showSet(ori);
			}
			_geoData->removeTopGeometrySet(set);
			emit removeDisplayActor(set);
		}
		emit updateGeoTree();*/
	}

	void CommandMoveFeature::redo()
	{
		if (_isEdit)
		{
			Geometry::GeometryModelParaBase* pm = _editSet->getParameter();
			Geometry::GeometryParaMakeMove* p = dynamic_cast<Geometry::GeometryParaMakeMove*>(pm);
			if (p == nullptr) return;
			Geometry::GeometrySet* ori = p->getOriSet();
			if (ori == nullptr) return;
			auto newset = _resultOriginHash.key(ori);
			if (newset == nullptr) return;
			_geoData->replaceSet(newset, _editSet);
			if (_saveOrigin)
			{
				newset->removeSubSet(ori);
				_editSet->removeSubSet(ori);
				_geoData->removeTopGeometrySet(ori);
				_geoData->appendGeometrySet(ori);
				emit showSet(ori);
				emit showSet(newset);
				emit removeDisplayActor(_editSet);
			}
			else
			{
				_editSet->removeSubSet(ori);
				newset->appendSubSet(ori);
				_geoData->removeTopGeometrySet(ori);

				emit removeDisplayActor(ori);
				emit showSet(newset);
				emit removeDisplayActor(_editSet);
			}
			emit updateGeoTree();
			_releasenew = false;
			_releaseEdit = true;
			return;
		}
		QList<Geometry::GeometrySet*> geoList = _resultOriginHash.keys();
		for (int i = 0; i < geoList.size(); ++i)
		{
			Geometry::GeometrySet* set = geoList.at(i);
			if (!_saveOrigin)
			{
				auto ori = _resultOriginHash.value(set);
				set->appendSubSet(ori);
				_geoData->removeTopGeometrySet(ori);
				emit removeDisplayActor(ori);
			}
			_geoData->appendGeometrySet(set);
			emit showSet(set);
		}
		emit updateGeoTree();
			/*QList<Geometry::GeometrySet*> geoList = _resultOriginHash.keys();
			for (int i = 0; i < geoList.size(); ++i)
			{
			Geometry::GeometrySet* set = geoList.at(i);
			if (!_saveOrigin)
			{
			auto ori = _resultOriginHash.value(set);
			set->appendSubSet(ori);
			_geoData->removeTopGeometrySet(ori);
			emit removeDisplayActor(ori);
			}
			_geoData->appendGeometrySet(set);
			emit showSet(set);
			}
			emit updateGeoTree();*/
		
	}

	void CommandMoveFeature::releaseResult()
	{
		if (_isEdit)
		{
			if (_releasenew)
			{
				Geometry::GeometryModelParaBase* pm = _editSet->getParameter();
				Geometry::GeometryParaMakeMove* p = dynamic_cast<Geometry::GeometryParaMakeMove*>(pm);
				if (p == nullptr) return;
				Geometry::GeometrySet* ori = p->getOriSet();
				if (ori == nullptr) return;
				auto newset = _resultOriginHash.key(ori);
				if (newset == nullptr) return;
				delete newset;
			}
			if (_releaseEdit)
			{
				delete _editSet;
			}

			return;
		}

		QList<Geometry::GeometrySet*> geoList = _resultOriginHash.keys();
		for (int i = 0; i < geoList.size(); ++i)
		{
			Geometry::GeometrySet* set = geoList.at(i);
			delete set;
		}
		/*QList<Geometry::GeometrySet*> geoList = _resultOriginHash.keys();
		for (int i = 0; i < geoList.size(); ++i)
		{
			Geometry::GeometrySet* set = geoList.at(i);
			delete set;
		}*/
	}

	void CommandMoveFeature::setBodys(QList<Geometry::GeometrySet*> b)
	{
		_bodys = b;
	}

	void CommandMoveFeature::setVector(double* vec)
	{
		for (int i = 0; i < 3; ++i)
		{
			_vector[i] = vec[i];
		}
	}

	void CommandMoveFeature::setSaveOrigin(bool on)
	{
		_saveOrigin = on;
	}


	void CommandMoveFeature::setOptionIndex(int i)
	{
		_optionindex = i;

	}

	void CommandMoveFeature::setStartPt(double * startpt)
	{
		for (int i = 0; i < 3; i++)
		{
			_startpt[i] = startpt[i];
		}
	}

	void CommandMoveFeature::setEndPt(double * endpt)
	{
		for (int i = 0; i < 3; i++)
		{
			_endpt[i] = endpt[i];
		}
	}

	void CommandMoveFeature::setDir(double * dir)
	{
		for (int i = 0; i < 3; i++)
		{
			_dir[i] = dir[i];
		}
	}

	void CommandMoveFeature::setReverse(bool r)
	{
		_reverse = r;
	}

	void CommandMoveFeature::setLength(double length)
	{
		_length = length;
	}

	void CommandMoveFeature::getVec()
	{
		if ( _optionindex== 0)
		{
				_vector[0] = _endpt[0] - _startpt[0];
				_vector[1] = _endpt[1] - _startpt[1];
				_vector[2] = _endpt[2] - _startpt[2];

		}
		else
		{
			if (_length < 1e-6) return ;
			if (_reverse)
			{
				for (int i = 0; i < 3; ++i)
					_dir[i] *= -1;
			}
			double mod = sqrt(_dir[0] * _dir[0] + _dir[1] * _dir[1] + _dir[2] * _dir[2]);
			double fac = _length / mod;
			for (int i = 0; i < 3; ++i)
				_vector[i] = fac * _dir[i];

		}


	}

}