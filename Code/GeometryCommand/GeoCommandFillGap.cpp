#include "GeoCommandFillGap.h"
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include <TopoDS_Shape.hxx>
#include <TopTools_ListOfShape.hxx>
#include <BRepAlgoAPI_Defeaturing.hxx>
#include <BRepTools.hxx>
#include <BRep_Builder.hxx>
#include <TopoDS_Compound.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <BOPAlgo_MakeConnected.hxx>
#include <BOPAlgo_Builder.hxx>
#include <TopoDS_Iterator.hxx>
#include "GeoCommandCommon.h"
#include "geometry/geometryParaMakeFillGap.h"
#include <QPair>
namespace Command
{
	CommandFillGap::CommandFillGap(GUI::MainWindow* m, MainWidget::PreWindow* p)
		: GeoCommandBase(m, p)
	{
	}

	bool CommandFillGap::execute()
	{
		if (_shapelist.size() != 2) return false;

		Geometry::GeometrySet* set1 = _shapelist.front().first;
		Geometry::GeometrySet* set2 = _shapelist.back().first;
		int index1 = _shapelist.front().second;
		int index2 = _shapelist.back().second;
		TopoDS_Shape shape1 = *(set1->getShape(_optionindex + 2, index1));
		TopoDS_Shape shape2 = *(set2->getShape(_optionindex + 2, index2));
		if (shape1.IsNull() || shape2.IsNull()) return false;

		TopoDS_Shape aResult=this->getGapShape(shape1, shape2);
		if (aResult.IsNull()) return false;
		
		TopoDS_Compound aRes;
		BRep_Builder aBuilderAdd;
		aBuilderAdd.MakeCompound(aRes);
		aBuilderAdd.Add(aRes, aResult);
		//将set1中剩余shape和set2剩余shape组成compound，然后与新shape组合。
		TopoDS_Shape* setShape1 = set1->getShape();
		TopoDS_Shape* setShape2 = set2->getShape();
		if (setShape1 == nullptr || setShape2 == nullptr) return false;
		TopoDS_Shape* setCopyShape1 = new TopoDS_Shape;
		TopoDS_Shape* setCopyShape2 = new TopoDS_Shape;
		*setCopyShape1 = GeoCommandCommon::removeShape(setShape1, &shape1);
		*setCopyShape2 = GeoCommandCommon::removeShape(setShape2, &shape2);
		if (!GeoCommandCommon::isEmpty(*setShape1))	aBuilderAdd.Add(aRes, *setShape1);
		if (!GeoCommandCommon::isEmpty(*setShape2)) aBuilderAdd.Add(aRes, *setShape2);

		TopoDS_Shape* newShape = new TopoDS_Shape;
		*newShape = aRes;
		const int id = Geometry::GeometrySet::getMaxID() + 1;
		QString name = QString("FillGap_%1").arg(id);
		Geometry::GeometrySet* newset = new Geometry::GeometrySet;
		newset->setName(name);
		newset->setShape(newShape);
		set1->setShape(setCopyShape1);
		set2->setShape(setCopyShape2);
		newset->appendSubSet(set1);
		newset->appendSubSet(set2);
		_result = newset;

		if (_isEdit)
		{
			newset->setName(_editSet->getName());
			_geoData->replaceSet(newset, _editSet);
			
			const int n = _editSet->getSubSetCount();
			for (int i = 0; i < n; ++i)
			{
				Geometry::GeometrySet* subset = _editSet->getSubSetAt(i);
				_geoData->appendGeometrySet(subset);
			}
		}
		else
		{
			newset->setName(name);
			_geoData->appendGeometrySet(newset);
		}
		Geometry::GeometryParaMakeFillGap* para = new Geometry::GeometryParaMakeFillGap;
		para->setInputShapes(_shapelist);
		para->setType(_optionindex);
		newset->setParameter(para);

		_geoData->removeTopGeometrySet(set1);
		_geoData->removeTopGeometrySet(set2);
		emit removeDisplayActor(set1);
		emit removeDisplayActor(set2);
		emit showSet(newset);
		emit updateGeoTree();
		GeoCommandBase::execute();
		return true;
		
	}

	void CommandFillGap::undo()
	{
		if (_isEdit)
		{
			_geoData->replaceSet(_editSet, _result);
			emit removeDisplayActor(_result);//c2

			const int n = _result->getSubSetCount();//添加b2,b3
			for (int i = 0; i < n; ++i)
			{
				Geometry::GeometrySet* subset = _result->getSubSetAt(i);
				_geoData->appendGeometrySet(subset);
				emit showSet(subset);
			}


			const int m = _editSet->getSubSetCount();//删除b1,b2.
			for (int i = 0; i < m; ++i)
			{
				Geometry::GeometrySet* subset = _editSet->getSubSetAt(i);
				_geoData->removeTopGeometrySet(subset);
				emit removeDisplayActor(subset);
			}
			//_geoData->appendGeometrySet(_editSet);

			emit showSet(_editSet);
		}
		else
		{
			
			_geoData->removeTopGeometrySet(_result);
			emit removeDisplayActor(_result);

			const int n = _result->getSubSetCount();
			for (int i = 0; i < n; ++i)
			{
				Geometry::GeometrySet* subset = _result->getSubSetAt(0);
				_result->removeSubSet(subset);
				_geoData->appendGeometrySet(subset);
				emit showSet(subset);
			}
		}

		emit updateGeoTree();
		
	}

	void CommandFillGap::redo()
	{
		
		if (_isEdit)
		{
			_geoData->replaceSet(_result, _editSet);
			emit removeDisplayActor(_editSet);
			const int n = _editSet->getSubSetCount();
			for (int i = 0; i < n; ++i)
			{
				Geometry::GeometrySet* subset = _editSet->getSubSetAt(i);
				_geoData->appendGeometrySet(subset);
				emit showSet(subset);
			}
			_geoData->removeTopGeometrySet(_shapelist.front().first);
			_geoData->removeTopGeometrySet(_shapelist.back().first);
			emit removeDisplayActor(_shapelist.front().first);
			emit removeDisplayActor(_shapelist.back().first);
			emit showSet(_result);
		}
		else
		{
			emit removeDisplayActor(_shapelist.front().first);
			emit removeDisplayActor(_shapelist.back().first);

			_geoData->removeTopGeometrySet(_shapelist.front().first);
			_geoData->removeTopGeometrySet(_shapelist.back().first);

			_result->appendSubSet(_shapelist.front().first);
			_result->appendSubSet(_shapelist.back().first);

			_geoData->appendGeometrySet(_result);
			emit showSet(_result);
		}
		emit updateGeoTree();
	}

	void CommandFillGap::releaseResult()
	{
			
	}

	void CommandFillGap::setEdges(QList<QPair<Geometry::GeometrySet*, int>> s)
	{
		_shapelist = s;
	}

	void CommandFillGap::setFaces(QList<QPair<Geometry::GeometrySet*, int>> s)
	{
		_shapelist = s;
	}

	void CommandFillGap::setSolids(QList<QPair<Geometry::GeometrySet*, int>> s)
	{
		_shapelist = s;
	}

	void CommandFillGap::setGapType(int i)
	{
		_optionindex = i;
	}

	TopoDS_Shape CommandFillGap::getGapShape(TopoDS_Shape shape1, TopoDS_Shape shape2)
	{
		TopTools_ListOfShape aLSObjects; //进行连接的形状
		aLSObjects.Append(shape1);
		aLSObjects.Append(shape2);

		BOPAlgo_Builder aBuilder;
		aBuilder.SetArguments(aLSObjects);
		//设置GF的选项
		//设置并行处理模式（默认为false）
		Standard_Boolean bRunParallel = Standard_True;
		aBuilder.SetRunParallel(bRunParallel);
		//设置模糊值（默认为Precision :: Confusion（））  
		Standard_Real aFuzzyValue = 1e-5;
		aBuilder.SetFuzzyValue(aFuzzyValue);
		//设置安全处理模式（默认为false）
		Standard_Boolean bSafeMode = Standard_True;
		aBuilder.SetNonDestructive(bSafeMode);
		//为重合的参数设置粘合模式（默认为关闭）
		BOPAlgo_GlueEnum aGlue = BOPAlgo_GlueShift;
		aBuilder.SetGlue(aGlue);
		//禁用/启用对倒置固体的检查（默认为true）
		Standard_Boolean bCheckInverted = Standard_False;
		aBuilder.SetCheckInverted(bCheckInverted);
		//设置OBB用法（默认为false）
		Standard_Boolean bUseOBB = Standard_True;
		aBuilder.SetUseOBB(bUseOBB);
		//执行操作 
		aBuilder.Perform();
		//检查错误
		if (aBuilder.HasErrors()) return TopoDS_Shape();
		if (aBuilder.HasWarnings()) return TopoDS_Shape();
		// result of the operation
		return aBuilder.Shape();
	}

}
