#include "GeoCommandMirrorFeature.h"
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include <TopoDS_Shape.hxx>
#include <gp_Ax2.hxx>
#include <TopoDS.hxx>
#include <gp_Pln.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include "geometry/geometryParaMakeMirror.h"
#include <QDebug>
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>
#include "GeoCommandCommon.h"
#include <BRepBuilderAPI_Copy.hxx>
#include <BRepTools.hxx>
#include <BRepAdaptor_Curve.hxx>
namespace Command
{
	CommandMirrorFeature::CommandMirrorFeature(GUI::MainWindow* m, MainWidget::PreWindow* p)
		: GeoCommandBase(m, p)
	{
	}

	bool CommandMirrorFeature::execute()
	{
		this->getDirection();
		gp_Pnt p(_planeLocation[0], _planeLocation[1], _planeLocation[2]);
		gp_Dir dir(_planeDirection[0], _planeDirection[1], _planeDirection[2]);
		gp_Ax2 plane(p, dir);
		
		if (_isEdit) //编辑模式下将原来的模型压进列表
		{
			Geometry::GeometryModelParaBase* pm = _editSet->getParameter();
			Geometry::GeometryParaMirrorFeature* p = dynamic_cast<Geometry::GeometryParaMirrorFeature*>(pm);
			if (p == nullptr) return false;

			Geometry::GeometrySet* ori = p->getOriSet();
			bool save = p->getSaveOrigion();
			if (ori == nullptr) return false;
			if (!save)
			{
				_geoData->appendGeometrySet(ori);
				_editSet->removeSubSet(ori);
			}
		}
		bool success = false;
		const int count = _bodys.size();
		QList<Geometry::GeometrySet*> setlist = _solidhash.uniqueKeys();
		for (Geometry ::GeometrySet* set : setlist)
		{
			//aRes=new shape+无关shape.
			TopoDS_Compound aRes;
			BRep_Builder aBuilder;
			aBuilder.MakeCompound(aRes);
			QList<int> indexList = _solidhash.values(set);
			for (int i = 0; i < indexList.size(); i++)
			{
				TopoDS_Shape shape = *set->getShape(4, indexList[i]);
				if (shape.IsNull()) return false;
			
				gp_Trsf aTrsf;
				aTrsf.SetMirror(plane);
				BRepBuilderAPI_Transform aBRespTrsf(shape, aTrsf);
				aBRespTrsf.Build();
				if (!aBRespTrsf.IsDone()) continue;
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
				TopoDS_Shape tempShape =*set->getShape(4, indexList[i]);
				*setCopyShape = GeoCommandCommon::removeShape(setShape, &tempShape);
				
			}
			if (!GeoCommandCommon::isEmpty(*setShape))
			{
				aBuilder.Add(aRes, *setShape);

			}
			success = true; 
			TopoDS_Shape* mshape = new TopoDS_Shape;
			*mshape = aRes;
			QString name = QString("Mirror-%1").arg(set->getName());
			Geometry::GeometrySet* newset = new Geometry::GeometrySet;
			newset->setName(name);
			newset->setShape(mshape);

			_geoData->appendGeometrySet(newset);
			_geoData->removeTopGeometrySet(set);
			newset->appendSubSet(set);
			_resultOriginHash.insert(newset, set);
			emit showSet(newset);
			emit removeDisplayActor(set);

			Geometry::GeometryParaMirrorFeature* para = new Geometry::GeometryParaMirrorFeature;
			set->setShape(setOriShape);
			para->setOriSet(set);
			for (int k = 0; k < indexList.size(); k++)
			{
				para->appendBody(set, indexList[k]);
			}
			para->setCurrentIndex(_typeindex);
			para->setFaceIndex(_faceIndex);
			para->setFaceBody(_faceBody);
			para->setSaveOrigion(_saveOrigin);
			para->setPlaneIndex(_planeindex);
			para->setDirection(_randomdir);
			para->setbasepoint(_basepoint);
			newset->setParameter(para);
			if (_isEdit)
			{
				_geoData->removeTopGeometrySet(_editSet);
				_releasenew = true;
				_releaseEdit = false;

			}
			
		}
		emit updateGeoTree();
		GeoCommandBase::execute();
		return success;

		/*QMultiHash<Geometry::GeometrySet*, int>::iterator iter = _solidhash.begin();
		for (; iter != _solidhash.end(); ++iter)
		{
			Geometry::GeometrySet* set =iter.key();
			const TopoDS_Shape& shape = set->getRealShape(4,iter.value());
			gp_Trsf aTrsf;
			aTrsf.SetMirror(plane);
			BRepBuilderAPI_Transform aBRespTrsf(shape, aTrsf);
			aBRespTrsf.Build();
			if (!aBRespTrsf.IsDone()) continue; 

			TopoDS_Compound aRes;
			BRep_Builder aBuilder;
			aBuilder.MakeCompound(aRes);
			aBuilder.Add(aRes, aBRespTrsf.Shape());
			
			success = true;
			TopoDS_Shape* mshape = new TopoDS_Shape;
			*mshape = aRes;
			QString name = QString("Mirror-%1").arg(set->getName());
			Geometry::GeometrySet* newset = new Geometry::GeometrySet;
			newset->setName(name);
			newset->setShape(mshape);
			_geoData->appendGeometrySet(newset);

			Geometry::GeometryParaMirrorFeature* para = new Geometry::GeometryParaMirrorFeature;
			para->setOriSet(set);
			para->setBodyList(_bodys);
			para->setCurrentIndex(_typeindex);
			para->setFaceIndex(_faceIndex);
			para->setFaceBody(_faceBody);
			para->setSaveOrigion(_saveOrigin);
			para->setPlaneIndex(_planeindex);
			para->setDirection(_randomdir);
			para->setbasepoint(_basepoint);
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
				_releasenew = true;
				_releaseEdit = false;
				
			}
		}
		emit updateGeoTree();
		GeoCommandBase::execute();
		return success;*/
	}

	void CommandMirrorFeature::undo()
	{
		if (_isEdit)
		{
			Geometry::GeometryModelParaBase* pm = _editSet->getParameter();
			Geometry::GeometryParaMirrorFeature* p = dynamic_cast<Geometry::GeometryParaMirrorFeature*>(pm);
			if (p == nullptr) return;
			Geometry::GeometrySet* ori = p->getOriSet();
			if (ori == nullptr) return;
			auto newset = _resultOriginHash.key(ori);
			if (newset == nullptr) return;
			_geoData->replaceSet(_editSet, newset);
		/*	if (p->getSaveOrigion())
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
			/*if (_saveOrigin)
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

	void CommandMirrorFeature::redo()
	{
		if (_isEdit)
		{
			Geometry::GeometryModelParaBase* pm = _editSet->getParameter();
			Geometry::GeometryParaMirrorFeature* p = dynamic_cast<Geometry::GeometryParaMirrorFeature*>(pm);
			if (p == nullptr) return;
			Geometry::GeometrySet* ori = p->getOriSet();
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
	 
	void CommandMirrorFeature::releaseResult()
	{
		if (_isEdit)
		{
			if (_releasenew)
			{
				Geometry::GeometryModelParaBase* pm = _editSet->getParameter();
				Geometry::GeometryParaMirrorFeature* p = dynamic_cast<Geometry::GeometryParaMirrorFeature*>(pm);
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
	}

	void CommandMirrorFeature::setBodys(QMultiHash<Geometry::GeometrySet*, int>  b)
	{
		_solidhash = b;
	}

	void CommandMirrorFeature::setPlane(double* loc, double* dir)
	{
		for (int i = 0; i < 3; ++i)
		{
			_planeDirection[i] = dir[i];
			_planeLocation[i] = loc[i];
		}
	}

	void CommandMirrorFeature::setSaveOrigin(bool on)
	{
		_saveOrigin = on;
	}


	void CommandMirrorFeature::setTypeIndex(int i)
	{
		_typeindex = i; 
	}

	void CommandMirrorFeature::setFaceIndex(int faceIndex)
	{
		_faceIndex = faceIndex;
	}

	void CommandMirrorFeature::setFaceBody(Geometry::GeometrySet* faceBody)
	{
		_faceBody = faceBody;
	}

	void CommandMirrorFeature::setPlaneIndex(int planeindex)
	{
		_planeindex = planeindex;
	}

	void CommandMirrorFeature::setreverse(bool reverse)
	{
		_reverse = reverse;
	}

	void CommandMirrorFeature::setRandomDir(double* randomdir)
	{
		for (int i = 0; i < 3; i++)
		{
			_randomdir[i] = randomdir[i];
		}
	}

	void CommandMirrorFeature::setBasePt(double* basepoint)
	{
		for (int i = 0; i < 3; i++)
		{
			_basepoint[i] = basepoint[i];
		}
	}

	void CommandMirrorFeature::getDirection()
	{
		if (_typeindex == 0)
		{
			if (_faceBody == nullptr) return ;
			TopoDS_Shape* shape = _faceBody->getShape();
			TopExp_Explorer faceExp(*shape, TopAbs_FACE);
			for (int index = 0; index < _faceIndex && faceExp.More(); faceExp.Next(), ++index);

			const TopoDS_Shape& faceShape = faceExp.Current();
			if (faceShape.IsNull()) return ;

		/*	std::string filestr = "D:/IGS/edgeone.brep";
			const char* ch = filestr.c_str();
			BRepTools::Write(faceShape, ch);*/

			const TopoDS_Face &face = TopoDS::Face(faceShape);
			if (face.IsNull()) return ;

			BRepAdaptor_Surface adapt(face);
			if (adapt.GetType() != GeomAbs_Plane) return ;
			gp_Pln plane = adapt.Plane();
			gp_Ax1 normal = plane.Axis();

			gp_Pnt loca = normal.Location();
			gp_Dir dirt = normal.Direction();

			_planeLocation[0] = loca.X(); _planeLocation[1] = loca.Y(); _planeLocation[2] = loca.Z();
			_planeDirection[0] = dirt.X(); _planeDirection[1] = dirt.Y(); _planeDirection[2] = dirt.Z();

		}
		else if (_typeindex == 1)
		{
			switch (_planeindex)
			{
			case 0: _planeDirection[2] = 1.0; break;
			case 1: _planeDirection[1] = 1.0; break;
			case 2: _planeDirection[0] = 1.0; break;
			default: break;
			}
		}
		else if (_typeindex == 2)
		{
			for (int i = 0; i < 3;++i)
			{
				_planeLocation[i] = _basepoint[i];
				_planeDirection[i] = _randomdir[i];
			}
		}
		if (_planeDirection[0] * _planeDirection[0] + _planeDirection[1] * _planeDirection[1] + _planeDirection[2] * _planeDirection[2] < 1e-6) return;
	}

}