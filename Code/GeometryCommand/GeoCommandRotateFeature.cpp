#include "GeoCommandRotateFeature.h"
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include "geometry/geometryParaRotateFeature.h"
#include <TopoDS_Shape.hxx>
#include <gp_Ax2.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <gp_Lin.hxx>
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>
#include <TopoDS_Iterator.hxx>
#include "GeoCommandCommon.h"
#include <BRepBuilderAPI_Copy.hxx>
namespace Command
{
	CommandRotateFeature::CommandRotateFeature(GUI::MainWindow* m, MainWidget::PreWindow* p)
		: GeoCommandBase(m, p)
	{
	}

	bool CommandRotateFeature::execute()
	{
		gp_Pnt pt(_basicPoint[0], _basicPoint[1], _basicPoint[2]);

		double vector[3] = {0};
		bool ok = this->generateDir(vector);
		if (!ok) return false;

		gp_Dir vec(vector[0], vector[1], vector[2]);

		gp_Ax1 ax;
		ax.SetLocation(pt);
		ax.SetDirection(vec);

		double d = _degree*PI / 180.00;
		gp_Trsf aTrsf;
		aTrsf.SetRotation(ax, d);
		
		if (_isEdit) //编辑模式下将原来的模型压进列表
		{
			Geometry::GeometryModelParaBase* pm = _editSet->getParameter();
			Geometry::GeometryParaRotateFeature* p = dynamic_cast<Geometry::GeometryParaRotateFeature*>(pm);
			if (p == nullptr) return false;
			
			Geometry::GeometrySet* ori = p->getOriginObject();
			if (ori == nullptr) return false;
			if (!p->isSaveOrigin())
			{
				_geoData->appendGeometrySet(ori);
				_editSet->removeSubSet(ori);
			}
			//_bodys.append(ori);
		}
		
		bool success = false;
	//	const int count = _bodys.size();
		QList<Geometry::GeometrySet*> setlist = _solidHash.uniqueKeys();
		for (Geometry::GeometrySet* set : setlist)
		{

			//保存下某个set下所选中的所有solid.
			QMultiHash<Geometry::GeometrySet*, int> setToIndex{};
			//aRes=new shape+无关shape.
			TopoDS_Compound aRes;
			BRep_Builder aBuilder;
			aBuilder.MakeCompound(aRes);
			QList<int> indexList = _solidHash.values(set);
			for (int i = 0; i < indexList.size(); i++)
			{
				setToIndex.insert(set, indexList[i]);
				TopoDS_Shape shape = *set->getShape(4, indexList[i]);
				if (shape.IsNull()) return false;

				BRepBuilderAPI_Transform aBRespTrsf(shape, aTrsf, true);
				aBRespTrsf.Build();
				if (!aBRespTrsf.IsDone()) continue;
				const TopoDS_Shape& resShape = aBRespTrsf.Shape();
				if (resShape.IsNull()) continue;

				aBuilder.Add(aRes, aBRespTrsf.Shape());
				if (_saveOrigin)
				{
					aBuilder.Add(aRes, shape);
				}
			}
			//将无关的solid存在compound中。
			TopoDS_Shape* setShape = set->getShape();
			TopoDS_Shape* setCopyShape = new TopoDS_Shape;
			TopoDS_Shape* setOriShape = new TopoDS_Shape;
			*setOriShape = BRepBuilderAPI_Copy(*setShape);
			for (int i = 0; i < indexList.size(); i++)
			{
				TopoDS_Shape tempShape = *set->getShape(4, indexList[i]);
				*setCopyShape = GeoCommandCommon::removeShape(setShape, &tempShape);

			}
			if (!GeoCommandCommon::isEmpty(*setShape))
			{
				aBuilder.Add(aRes, *setShape);

			}
			success = true;
			TopoDS_Shape* mshape = new TopoDS_Shape;
			*mshape = aRes;
			QString name = QString("RotateFacture-%1").arg(set->getName());
			Geometry::GeometrySet* newset = new Geometry::GeometrySet;
			newset->setName(name);
			newset->setShape(mshape);

			_geoData->appendGeometrySet(newset);
			_geoData->removeTopGeometrySet(set);
			newset->appendSubSet(set);
			_resultOriginHash.insert(newset, set);
			emit showSet(newset);
			emit removeDisplayActor(set);

			Geometry::GeometryParaRotateFeature* para = new Geometry::GeometryParaRotateFeature;
			set->setShape(setOriShape);
			for (int k = 0; k < indexList.size(); k++)
			{
				para->appendBody(set, indexList[k]);
			}
			para->setOriginObject(set);
			para->setBasicPoint(_basicPoint);
			para->setAngle(_degree);
			para->isSaveOrigin(_saveOrigin);
			para->setMethod(_method);
			para->setEdge(_edge.first, _edge.second);
			para->setVector(_vector);
			para->isReverse(_reverse);
			newset->setParameter(para);

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
		/*bool success = false;
		QMultiHash<Geometry::GeometrySet*, int>::iterator iter = _solidHash.begin();
		for (; iter != _solidHash.end(); ++iter)
		{
			Geometry::GeometrySet* set = iter.key();
			TopoDS_Shape* shape = set->getShape(4, iter.value());
			
			BRepBuilderAPI_Transform aBRespTrsf(*shape, aTrsf, true);
			aBRespTrsf.Build();
			if (!aBRespTrsf.IsDone()) continue;
			const TopoDS_Shape& resShape = aBRespTrsf.Shape();
			if (resShape.IsNull()) continue;
			
			success = true;

			TopoDS_Shape* mshape = new TopoDS_Shape;
			*mshape = resShape;
			QString name = QString("Rotate-%1").arg(set->getName());

			Geometry::GeometrySet* newset = new Geometry::GeometrySet;
			newset->setName(name);
			newset->setShape(mshape);
			_geoData->appendGeometrySet(newset);
			this->setPara(newset, set); //保存参数
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
		return success;*/
	}

	void CommandRotateFeature::undo()
	{
		if (_isEdit)
		{
			Geometry::GeometryModelParaBase* pm = _editSet->getParameter();
			Geometry::GeometryParaRotateFeature* p = dynamic_cast<Geometry::GeometryParaRotateFeature*>(pm);
			if (p == nullptr) return;
			Geometry::GeometrySet* ori = p->getOriginObject();
			if (ori == nullptr) return;
			auto newset = _resultOriginHash.key(ori);
			if (newset == nullptr) return;
			_geoData->replaceSet(_editSet, newset);
			/*if (p->isSaveOrigin())
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
			{*/
			newset->removeSubSet(ori);
			_editSet->appendSubSet(ori);
			_geoData->removeTopGeometrySet(ori);
			emit removeDisplayActor(ori);
			emit showSet(_editSet);
			emit removeDisplayActor(newset);
			/*}*/
			emit updateGeoTree();
			_releasenew = true;
			_releaseEdit = false;
			return;
		}

		QList<Geometry::GeometrySet*> geoList = _resultOriginHash.keys();
		for (int i = 0; i < geoList.size(); ++i)
		{
			Geometry::GeometrySet* set = geoList.at(i);
			/*	if (!_saveOrigin)
			{*/
			auto ori = _resultOriginHash.value(set);
			set->removeSubSet(ori);
			_geoData->appendGeometrySet(ori);
			emit showSet(ori);
			/*}*/
			_geoData->removeTopGeometrySet(set);
			emit removeDisplayActor(set);
		}
		emit updateGeoTree();
	}

	void CommandRotateFeature::redo()
	{
		if (_isEdit)
		{
			Geometry::GeometryModelParaBase* pm = _editSet->getParameter();
			Geometry::GeometryParaRotateFeature* p = dynamic_cast<Geometry::GeometryParaRotateFeature*>(pm);
			if (p == nullptr) return;
			Geometry::GeometrySet* ori = p->getOriginObject();
			if (ori == nullptr) return;
			auto newset = _resultOriginHash.key(ori);
			if (newset == nullptr) return;
			_geoData->replaceSet(newset, _editSet);
			/*if (_saveOrigin)
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
			{*/
			_editSet->removeSubSet(ori);
			newset->appendSubSet(ori);
			_geoData->removeTopGeometrySet(ori);

			emit removeDisplayActor(ori);
			emit showSet(newset);
			emit removeDisplayActor(_editSet);
			/*}*/
			emit updateGeoTree();
			_releasenew = false;
			_releaseEdit = true;
			return;
		}
		QList<Geometry::GeometrySet*> geoList = _resultOriginHash.keys();
		for (int i = 0; i < geoList.size(); ++i)
		{
			Geometry::GeometrySet* set = geoList.at(i);
			/*if (!_saveOrigin)
			{*/
			auto ori = _resultOriginHash.value(set);
			set->appendSubSet(ori);
			_geoData->removeTopGeometrySet(ori);
			emit removeDisplayActor(ori);
			/*}*/
			_geoData->appendGeometrySet(set);
			emit showSet(set);
		}
		emit updateGeoTree();
	}

	void CommandRotateFeature::releaseResult()
	{
		if (_isEdit)
		{
			if (_releasenew)
			{
				Geometry::GeometryModelParaBase* pm = _editSet->getParameter();
				Geometry::GeometryParaRotateFeature* p = dynamic_cast<Geometry::GeometryParaRotateFeature*>(pm);
				if (p == nullptr) return;
				Geometry::GeometrySet* ori = p->getOriginObject();
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
	}

	void CommandRotateFeature::setBodys(QMultiHash<Geometry::GeometrySet*, int> bodyhash)
	{
		_solidHash = bodyhash;
	}

	void CommandRotateFeature::setVector(double* vec)
	{
		for (int i = 0; i < 3; ++i)
		{
			_vector[i] = vec[i];
		}
	}

	void CommandRotateFeature::setSaveOrigin(bool on)
	{
		_saveOrigin = on;
	}

	void CommandRotateFeature::setBasicPoint(double* bp)
	{
		for (int i = 0; i < 3; ++i)
		{
			_basicPoint[i] = bp[i];
		}
	}

	void CommandRotateFeature::setDegree(double d)
	{
		_degree = d;
	}

	void CommandRotateFeature::setMethod(int m)
	{
		_method = m;
	}

	void CommandRotateFeature::setEdge(Geometry::GeometrySet* body, int edge)
	{
		_edge.first = body;
		_edge.second = edge;
	}

	void CommandRotateFeature::setReverse(bool on)
	{
		_reverse = on;
	}

	bool CommandRotateFeature::generateDir(double* vec)
	{
		bool ok = false;
		if (_method == 0)
		{
			if (_edge.first == nullptr || _edge.second < 0) return false;
			TopoDS_Shape* shape = _edge.first->getShape();

			TopExp_Explorer edgeExp(*shape, TopAbs_EDGE);
			for (int k = 0; k < _edge.second; ++k) edgeExp.Next();

			const TopoDS_Edge &edge = TopoDS::Edge(edgeExp.Current());
			if (edge.IsNull()) return false;

			BRepAdaptor_Curve adapt(edge);
			if (adapt.GetType() != GeomAbs_Line) return false;
			gp_Lin line = adapt.Line();
			gp_Dir direc = line.Direction();
			vec[0] = direc.X();
			vec[1] = direc.Y();
			vec[2] = direc.Z();
			ok = true;
		}
		else
		{
		
			vec[0] = _vector[0];
			vec[1] = _vector[1];
			vec[2] = _vector[2];
			
			double mod = vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
			if (mod > 0.000001) ok = true;
		}

		if (_reverse)
		   for (int i = 0; i < 3; ++i)
		        vec[i] *= -1;

		return ok;
	}

	


}