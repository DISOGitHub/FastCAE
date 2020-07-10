#include "GeoCommandGeoSplitter.h"
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include <BOPAlgo_Splitter.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepTools.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>
#include <QDebug>
#include "geometry/geometryParaGeoSplitter.h"
#include <TopoDS_Face.hxx>
#include <gp_Pln.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRepBuilderAPI.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include "GeoCommandCommon.h"
#include <BRepBuilderAPI_Copy.hxx>
namespace Command
{
	CommandGeoSplitter::CommandGeoSplitter(GUI::MainWindow* m, MainWidget::PreWindow* p)
		: GeoCommandBase(m, p)
	{
	}

	bool CommandGeoSplitter::execute()
	{
		if (_solidhash.size() < 1 || _faceBody == nullptr || _faceIndex < 0)
			return false;
		if (_isEdit) //编辑模式下将原来的模型压进列表
		{
			Geometry::GeometryModelParaBase* pm = _editSet->getParameter();
			Geometry::GeometryParaGeoSplitter* p = dynamic_cast<Geometry::GeometryParaGeoSplitter*>(pm);
			if (p == nullptr) return false;

			Geometry::GeometrySet* ori = p->getOriSet();
			if (ori == nullptr) return false;
		
			_geoData->appendGeometrySet(ori);
			_editSet->removeSubSet(ori);

		}
		bool success = false;

		QList<Geometry::GeometrySet*> setlist = _solidhash.uniqueKeys();
		for (Geometry::GeometrySet* set : setlist)
		{

			//保存下某个set下所选中的所有solid.
			QMultiHash<Geometry::GeometrySet*, int> setToIndex{};
			//aRes=new shape+无关shape.
			TopoDS_Compound aRes;
			BRep_Builder aBuilder;
			aBuilder.MakeCompound(aRes);
			QList<int> indexList = _solidhash.values(set);
			for (int i = 0; i < indexList.size(); i++)
			{
				setToIndex.insert(set, indexList[i]);
				TopoDS_Shape shape = *set->getShape(4, indexList[i]);
				if (shape.IsNull()) return false;


				TopoDS_Shape* faceshape = _faceBody->getShape();
				TopExp_Explorer faceExp(*faceshape, TopAbs_FACE);
				for (int index = 0; index < _faceIndex && faceExp.More(); faceExp.Next(), ++index);
				const TopoDS_Shape& faceLimiteShape = faceExp.Current();
				if (faceLimiteShape.IsNull()) return false;

				//判断是否为平面。
				const TopoDS_Face &face = TopoDS::Face(faceLimiteShape);
				if (face.IsNull()) return false;
				BRepAdaptor_Surface adapt(face);
				if (adapt.GetType() != GeomAbs_Plane) return false;
				gp_Pln plane = adapt.Plane();

				BRepBuilderAPI_MakeFace mkFace(plane);
				const TopoDS_Face& faceShape = mkFace.Face();
				if (face.IsNull()) return false;
				BOPAlgo_Splitter splitter;
				splitter.AddArgument(shape);
				splitter.AddTool(faceShape);
				splitter.Perform();

				/*TopoDS_Compound comp;
				BRep_Builder builder;
				builder.MakeCompound(comp);*/
				TopExp_Explorer shapeExp(splitter.Shape(), TopAbs_SOLID);
				for (; shapeExp.More(); shapeExp.Next())
				{
					const TopoDS_Solid& anSolid = TopoDS::Solid(shapeExp.Current());
					aBuilder.Add(aRes, anSolid);
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
			QString name = QString("Splitter-%1").arg(set->getName());
			Geometry::GeometrySet* newset = new Geometry::GeometrySet;
			newset->setName(name);
			newset->setShape(mshape);

			_geoData->appendGeometrySet(newset);
			_geoData->removeTopGeometrySet(set);
			newset->appendSubSet(set);
			_inputOutputHash.insert(newset, set);
			emit showSet(newset);
			emit removeDisplayActor(set);

			Geometry::GeometryParaGeoSplitter* para = new Geometry::GeometryParaGeoSplitter;
			set->setShape(setOriShape);
			para->setOriSet(set);
			for (int k = 0; k < indexList.size(); k++)
			{
				para->appendBody(set, indexList[k]);
			}
			para->setFaceIndex(_faceIndex);
			para->setFaceBody(_faceBody);
			newset->setParameter(para);
			GeoCommandBase::execute();

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

	void CommandGeoSplitter::undo()
	{
		if (_isEdit)
		{
			Geometry::GeometryModelParaBase* pm = _editSet->getParameter();
			Geometry::GeometryParaGeoSplitter* p = dynamic_cast<Geometry::GeometryParaGeoSplitter*>(pm);
			if (p == nullptr) return;
			Geometry::GeometrySet* ori = p->getOriSet();
			if (ori == nullptr) return;
			QList<Geometry::GeometrySet*> setliist = _inputOutputHash.keys(ori);
			int sinz = setliist.size();
			auto newset = _inputOutputHash.key(ori);
			if (newset == nullptr) return;
			_geoData->replaceSet(_editSet, newset);
	
			newset->removeSubSet(ori);
			_editSet->appendSubSet(ori);
			_geoData->removeTopGeometrySet(ori);

			emit removeDisplayActor(ori);
			emit showSet(_editSet);
			emit removeDisplayActor(newset);

			emit updateGeoTree();
			_releasenew = true;
			_releaseEdit = false;
			return;
		}

		QList<Geometry::GeometrySet*> geoList = _inputOutputHash.keys();
		for (int i = 0; i < geoList.size(); ++i)
		{
			Geometry::GeometrySet* set = geoList.at(i);
	
			auto ori = _inputOutputHash.value(set);
			set->removeSubSet(ori);
			_geoData->appendGeometrySet(ori);
			emit showSet(ori);
		
			_geoData->removeTopGeometrySet(set);
			emit removeDisplayActor(set);
		}
		emit updateGeoTree();
	}

	void CommandGeoSplitter::redo()
	{
		if (_isEdit)
		{
			Geometry::GeometryModelParaBase* pm = _editSet->getParameter();
			Geometry::GeometryParaGeoSplitter* p = dynamic_cast<Geometry::GeometryParaGeoSplitter*>(pm);
			if (p == nullptr) return;
			Geometry::GeometrySet* ori = p->getOriSet();
			if (ori == nullptr) return;
			auto newset = _inputOutputHash.key(ori);
			if (newset == nullptr) return;
			_geoData->replaceSet(newset, _editSet);
			
			_editSet->removeSubSet(ori);
			newset->appendSubSet(ori);
			_geoData->removeTopGeometrySet(ori);

			emit removeDisplayActor(ori);
			emit showSet(newset);
			emit removeDisplayActor(_editSet);

			emit updateGeoTree();
			_releasenew = false;
			_releaseEdit = true;
			return;
		}
		QList<Geometry::GeometrySet*> geoList = _inputOutputHash.keys();
		for (int i = 0; i < geoList.size(); ++i)
		{
			Geometry::GeometrySet* set = geoList.at(i);
		
			auto ori = _inputOutputHash.value(set);
			set->appendSubSet(ori);
			_geoData->removeTopGeometrySet(ori);
			emit removeDisplayActor(ori);
			
			_geoData->appendGeometrySet(set);
			emit showSet(set);
		}
		emit updateGeoTree();
	}

	void CommandGeoSplitter::releaseResult()
	{
		if (_isEdit)
		{
			if (_releasenew)
			{
				Geometry::GeometryModelParaBase* pm = _editSet->getParameter();
				Geometry::GeometryParaGeoSplitter* p = dynamic_cast<Geometry::GeometryParaGeoSplitter*>(pm);
				if (p == nullptr) return;
				Geometry::GeometrySet* ori = p->getOriSet();
				if (ori == nullptr) return;
				auto newset = _inputOutputHash.key(ori);
				if (newset == nullptr) return;
				delete newset;
			}
			if (_releaseEdit)
			{
				delete _editSet;
			}

			return;
		}

		QList<Geometry::GeometrySet*> geoList = _inputOutputHash.keys();
		for (int i = 0; i < geoList.size(); ++i)
		{
			Geometry::GeometrySet* set = geoList.at(i);
			delete set;
		}
	}
	  
	void CommandGeoSplitter::setBodys(QMultiHash<Geometry::GeometrySet*, int> setidList)
	{
		_solidhash= setidList;
	}

	void CommandGeoSplitter::setFaceIndex(int faceIndex)
	{
		_faceIndex = faceIndex;
	}

	void CommandGeoSplitter::setFaceBody(Geometry::GeometrySet* faceBody)
	{
		_faceBody = faceBody;
	}

}