#include "GeoCommandMakeMatrix.h"
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include <TopoDS_Shape.hxx>
#include <gp_Ax2.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>
#include "geometry/geometryParaMakeMatrix.h"
#include <QDebug>

namespace Command
{
	CommandMakeMatrix::CommandMakeMatrix(GUI::MainWindow* m, MainWidget::PreWindow* p)
		: GeoCommandBase(m, p)
	{
	}

	bool CommandMakeMatrix::execute()
	{
		bool success = false;
		const int count = _bodys.size();
	
		if (_selectLinear)
		{
			QMultiHash<Geometry::GeometrySet*, int>::iterator iter = _solidhash.begin();
			for (; iter != _solidhash.end(); ++iter)
			{
				Geometry::GeometrySet* set = iter.key();
				QList<int> indexList = _solidhash.values(set);
				TopoDS_Shape* shape = set->getShape(4, iter.value());
				
				TopoDS_Compound aRes;
				BRep_Builder aBuilder;
				aBuilder.MakeCompound(aRes);

				//ªÒ»°vectors
				QList<double*> vectors;
				if (_selectDir2)
				{
					QList<double*> vecDir1List = getDir1Vector();
					if (vecDir1List.size() < 1) success = false;
					QList<double*> vecDir2List = getDir2Vector();
					if (vecDir2List.size() < 1) success = false;
					vectors = getVecSumlist(vecDir1List, vecDir2List);
				}
				else
				{
					vectors = getDir1Vector();
				}
				if (vectors.size() < 1) success = false;
				for (int i = 0; i < vectors.size(); ++i)
				{
					gp_Vec vec(vectors[i][0], vectors[i][1], vectors[i][2]);
					gp_Trsf aTrsf;
					aTrsf.SetTranslation(vec);
					BRepBuilderAPI_Transform aBRespTrsf(*shape, aTrsf, true);
					aBRespTrsf.Build();
					if (!aBRespTrsf.IsDone()) continue;
					aBuilder.Add(aRes, aBRespTrsf.Shape());
				}
				success = true;
				TopoDS_Shape* mshape = new TopoDS_Shape;
				*mshape = aRes;
				QString name = QString("LinearMaxtrix-%1").arg(set->getName());
				Geometry::GeometrySet* newset = new Geometry::GeometrySet;
				newset->setName(name);
				newset->setShape(mshape);
				_geoData->appendGeometrySet(newset);

				Geometry::GeometryParaMatrix* para = new Geometry::GeometryParaMatrix;
				para->setOriSet(set);
				for (int k = 0; k < indexList.size(); k++)
				{
					para->appendBody(set, indexList[k]);
				}
				para->setCurrentIndex(0);
				para->setDirection1(_dir1);
				para->setReverse1(_reverse1);
				para->setDistance1(_dir1Distance);
				para->setCount1(_dir1Count);
				para->setShowDir2(_selectDir2);
				newset->setParameter(para);

				if (_selectDir2)
				{
					para->setDirection2(_dir2);
					para->setReverse2(_reverse2);
					para->setDistance2(_dir2Distance);
					para->setCount2(_dir2Count);
				}
				if (_isEdit)
				{
					_geoData->removeTopGeometrySet(_editSet);
					_releasenew = true;
					_releaseEdit = false;
				}
				emit showSet(newset);
				_resultHash.insert(newset, set);
			}
		}

		if (_selectWire)
		{
			gp_Pnt pt(_basicPoint[0], _basicPoint[1], _basicPoint[2]);
			getVecWire();
			gp_Dir vec(_axisdir[0], _axisdir[1], _axisdir[2]);
			
			gp_Ax1 ax;
			ax.SetLocation(pt);
			ax.SetDirection(vec);
		
			QMultiHash<Geometry::GeometrySet*, int>::iterator iter = _solidhash.begin();
			for (; iter != _solidhash.end(); ++iter)
			{
				Geometry::GeometrySet* set = iter.key();
				QList<int> indexList = _solidhash.values(set);
				TopoDS_Shape* shape = set->getShape(4, iter.value());
				TopoDS_Compound aRes;
				BRep_Builder aBuilder;
				aBuilder.MakeCompound(aRes);
				QList<double*> angleList = getAngleList();
				for (int i = 0; i < angleList.size(); ++i)
				{
					gp_Trsf aTrsf;
					aTrsf.SetRotation(ax, *angleList[i]);
					BRepBuilderAPI_Transform aBRespTrsf(*shape, aTrsf, true);
					aBRespTrsf.Build();
					if (!aBRespTrsf.IsDone()) continue;
					aBuilder.Add(aRes, aBRespTrsf.Shape());
					if (aRes.IsNull()) continue;
				}
					success = true;
					TopoDS_Shape* mshape = new TopoDS_Shape;
					*mshape = aRes;
					QString name = QString("WireMaxtrix-%1").arg(set->getName());
					Geometry::GeometrySet* newset = new Geometry::GeometrySet;
					newset->setName(name);
					newset->setShape(mshape);
					_geoData->appendGeometrySet(newset);
					Geometry::GeometryParaMatrix* para = new Geometry::GeometryParaMatrix;
					para->setOriSet(set);
					for (int k = 0; k < indexList.size(); k++)
					{
						para->appendBody(set, indexList[k]);
					}
					para->setCurrentIndex(1);
					para->setAxis(_basicPoint);
					para->setAxisDir(_axisdir);
					para->setAxisCount(_wireCount);
					para->setAngle(_degree);
					newset->setParameter(para);
					if (_isEdit)
					{
						_geoData->removeTopGeometrySet(_editSet);
					}
					emit showSet(newset);
					_resultHash.insert(newset, set);
			 }
		 }
		emit updateGeoTree();
		GeoCommandBase::execute();
		return success;
	}

	void CommandMakeMatrix::undo()
	{
		if (_isEdit)
		{
			Geometry::GeometryModelParaBase* pm = _editSet->getParameter();
			Geometry::GeometryParaMatrix* p = dynamic_cast<Geometry::GeometryParaMatrix*>(pm);
			if (p == nullptr) return;
			Geometry::GeometrySet* ori = p->getOriSet();
			if (ori == nullptr) return;
			auto newset = _resultHash.key(ori);
			if (newset == nullptr) return;
			_geoData->replaceSet(_editSet, newset);
	
			newset->removeSubSet(ori);
			_editSet->removeSubSet(ori);
			_geoData->removeTopGeometrySet(ori);
			_geoData->appendGeometrySet(ori);
			emit showSet(ori);
			emit showSet(_editSet);
			emit removeDisplayActor(newset);

			emit updateGeoTree();
			_releasenew = true;
			_releaseEdit = false;
			return;
		}

		QList<Geometry::GeometrySet*> geoList = _resultHash.keys();
		for (int i = 0; i < geoList.size(); ++i)
		{
			Geometry::GeometrySet* set = geoList.at(i);
			_geoData->removeTopGeometrySet(set);
			emit removeDisplayActor(set);
		}
		emit updateGeoTree();
	}

	void CommandMakeMatrix::redo()
	{
		if (_isEdit)
		{
			Geometry::GeometryModelParaBase* pm = _editSet->getParameter();
			Geometry::GeometryParaMatrix* p = dynamic_cast<Geometry::GeometryParaMatrix*>(pm);
			if (p == nullptr) return;
			Geometry::GeometrySet* ori = p->getOriSet();
			if (ori == nullptr) return;
			auto newset = _resultHash.key(ori);
			if (newset == nullptr) return;
			_geoData->replaceSet(newset, _editSet);

			newset->removeSubSet(ori);
			_editSet->removeSubSet(ori);
			_geoData->removeTopGeometrySet(ori);
			_geoData->appendGeometrySet(ori);
			emit showSet(ori);
			emit showSet(newset);
			emit removeDisplayActor(_editSet);
			emit updateGeoTree();
			_releasenew = false;
			_releaseEdit = true;
			return;
		}

		QList<Geometry::GeometrySet*> geoList = _resultHash.keys();
		for (int i = 0; i < geoList.size(); ++i)
		{
			Geometry::GeometrySet* set = geoList.at(i);
			_geoData->appendGeometrySet(set);
			emit showSet(set);
		}
		emit updateGeoTree();
	}

	void CommandMakeMatrix::releaseResult()
	{
		QList<Geometry::GeometrySet*> geoList = _resultHash.keys();
		for (int i = 0; i < geoList.size(); ++i)
		{
			Geometry::GeometrySet* set = geoList.at(i);
			delete set;
		}
	}

	void CommandMakeMatrix::setBodys(QMultiHash<Geometry::GeometrySet*, int> b)
	{
		_solidhash = b;
	}
	void CommandMakeMatrix::setSelectLinear(bool s)
	{
		_selectLinear = s;
	}
	void CommandMakeMatrix::setSelectWire(bool s)
	{
		_selectWire = s;
	}

	void CommandMakeMatrix::setOptionIndex(int i)
	{
		if (i == 0)
		{
			_selectLinear = true;
			_selectWire = false;
		}
		else
		{
			_selectLinear = false;
			_selectWire = true;
		}
	}

	void CommandMakeMatrix::setBasicPoint(double* bp)
	{
		for (int i = 0; i < 3; ++i)
		{
			_basicPoint[i] = bp[i];
		}
	}

	void CommandMakeMatrix::setAxisDir(double* dir)
	{
		for (int i = 0; i < 3; i++)
		{
			_axisdir[i] = dir[i];
		}
	}

	void CommandMakeMatrix::setDir1(double* dir)
	{
		_dir1[0] = dir[0];
		_dir1[1] = dir[1];
		_dir1[2] = dir[2];
	}

	void CommandMakeMatrix::setReverse1(bool s)
	{
		_reverse1 = s;
	}

	void CommandMakeMatrix::setDir1Distance(double d)
	{
		_dir1Distance = d;
	}

	void CommandMakeMatrix::setDir1Count(int count)
	{
		_dir1Count = count;
	}
	void CommandMakeMatrix::setDir2IsChecked(bool k)
	{
		_selectDir2 = k;
	}
	void CommandMakeMatrix::setDir2(double* dir)
	{
		_dir2[0] = dir[0];
		_dir2[1] = dir[1];
		_dir2[2] = dir[2];
	}

	void CommandMakeMatrix::setReverse2(bool s)
	{
		_reverse2 = s;
	}

	void CommandMakeMatrix::setDir2Distance(double d)
	{
		_dir2Distance = d;
	}

	void CommandMakeMatrix::setDir2Count(int count)
	{
		_dir2Count = count;
	}

	void CommandMakeMatrix::setWireCount(int s)
	{
		_wireCount = s;
	}

	void CommandMakeMatrix::setDegree(double d)
	{
		_degree = d;
	}

	void CommandMakeMatrix::setWireRevers(bool s)
	{
		_reversewire = s;
	}

	void CommandMakeMatrix::getVecWire()
	{
		double mod = _axisdir[0] * _axisdir[0] + _axisdir[1] * _axisdir[1] + _axisdir[2] * _axisdir[2];
		if (mod < 0.000001) return;

		if (_reversewire)
			for (int i = 0; i < 3; ++i)
				_axisdir[i] *= -1;
	}

	QList<double*> CommandMakeMatrix::getAngleList()
	{
		QList<double*> angleList{};
		QList<double*> tempList{};
		bool ok = true;
		for (int i = 1; i < _wireCount; ++i)
		{
			double temp = _degree*i;
			double *angle = new double;
			*angle = temp*PI / 180.00;
			angleList.append(angle);
		}
		if (angleList.size() < 1) ok = false;
		if (ok) return angleList;
		if (!ok) return tempList;
	}

	QList<double*> CommandMakeMatrix::getDir1Vector()
	{

		QList<double*> vectors{};
		if (_reverse1)
		{
			for (int i = 0; i < 3; ++i)
				_dir1[i] *= -1;
		}
		double mod = sqrt(_dir1[0] * _dir1[0] + _dir1[1] * _dir1[1] + _dir1[2] * _dir1[2]);
		for (int i = 1; i < _dir1Count; ++i)
		{
			double *veccopy = new double[3]{};
			double length = i*_dir1Distance;
			double fac = length / mod;
			for (int i = 0; i < 3; ++i)
			{
				veccopy[i] = fac * _dir1[i];
			}
			vectors.append(veccopy);	
		}
		return vectors;
	}

	QList<double*> CommandMakeMatrix::getDir2Vector()
	{

		QList<double*> vectors{};
		if (_reverse2)
		{
			for (int i = 0; i < 3; ++i)
				_dir2[i] *= -1;
		}
		double mod = sqrt(_dir2[0] * _dir2[0] + _dir2[1] * _dir2[1] + _dir2[2] * _dir2[2]);
		for (int i = 1; i < _dir2Count; ++i)
		{
			double *veccopy = new double[3]{};
			double length = i*_dir2Distance;
			double fac = length / mod;
			for (int i = 0; i < 3; ++i)
			{
				veccopy[i] = fac * _dir2[i];
			}
			vectors.append(veccopy);
		}
		return vectors;
	}
	
	QList<double*> CommandMakeMatrix::getVecSumlist(QList<double*>dir1list, QList<double*>dir2list)
	{
		QList<double*> sumList{};
		for (int i = 0; i < dir1list.size(); ++i)
		{
			for (int j = 0; j < dir2list.size(); ++j)
			{
				double *vecTmep = new double[3]{};
				vecTmep[0] = (dir1list[i][0] + dir2list[j][0]);
				vecTmep[1] = (dir1list[i][1] + dir2list[j][1]);
				vecTmep[2] = (dir1list[i][2] + dir2list[j][2]);
				sumList.append(vecTmep);
			}
		}
		sumList.append(dir1list);
		sumList.append(dir2list);
		return sumList;
	}

}