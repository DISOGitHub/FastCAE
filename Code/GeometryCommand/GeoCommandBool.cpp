#include "GeoCommandBool.h"
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include <TopoDS_Shape.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgo_Section.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS.hxx>
#include "GeoCommandCommon.h"
#include "geometry/geometryParaBoolOperation.h"
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>
#include <TopoDS_Iterator.hxx>
#include <BRepTools.hxx>
#include <QDebug>
#include <TopoDS_Builder.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepBuilderAPI_Copy.hxx>
namespace Command
{
	CommandBool::CommandBool(GUI::MainWindow* m, MainWidget::PreWindow* p)
		:GeoCommandBase(m, p)
	{

	}

	bool CommandBool::execute()
	{
		QString name;
		TopoDS_Shape* shape = nullptr;
		switch (_type)
		{
		case BoolCut:
			shape = cut();
			name = "Cut_%1";
			break;
		case BoolFause:
			name = "Fause_%1";
			shape = fause();
			break;
		case BoolCommon:
			name = "Common_%1";
			shape = common();
			break;
		default:
			break;
		}
		/*std::string filestr1 = "D:/IGS/1.brep";
		const char* ch = filestr1.c_str();
		BRepTools::Write(aChild, ch);
*/
		if (shape == nullptr) return false;

		TopoDS_Compound aRes;
		BRep_Builder aBuilder;
		aBuilder.MakeCompound(aRes);
		aBuilder.Add(aRes, *shape);
		//将set1中剩余shape和set2剩余shape组成compound，然后与新shape组合。
		TopoDS_Shape* setShape1 = _solid1.first->getShape();
		TopoDS_Shape* setShape2 = _solid2.first->getShape();
		if (setShape1 == nullptr || setShape2 == nullptr) return false;
		
		TopoDS_Shape shape1=*(_solid1.first->getShape(4, _solid1.second));
		TopoDS_Shape shape2 =* (_solid2.first->getShape(4, _solid2.second));
		if (shape1.IsNull() || shape2.IsNull()) return false;

		TopoDS_Shape* setCopyShape1 = new TopoDS_Shape;
		TopoDS_Shape* setCopyShape2 = new TopoDS_Shape;


		/*std::string filestr = "D:/IGS/1320000.brep";
		const char* ch = filestr.c_str();
		BRepTools::Write(*setShape1, ch);

		std::string filestr1 = "D:/IGS/132000011111.brep";
		const char* ch1 = filestr1.c_str();
		BRepTools::Write(shape1, ch1);*/

		*setCopyShape1 = GeoCommandCommon::removeShape(setShape1, &shape1);
		*setCopyShape2 = GeoCommandCommon::removeShape(setShape2, &shape2);


		if (!GeoCommandCommon::isEmpty(*setShape1))
		{
			/*std::string filestr = "D:/IGS/1321111.brep";
			const char* ch = filestr.c_str();
			BRepTools::Write(*setShape1, ch);*/

			aBuilder.Add(aRes, *setShape1);
		}
		if (!GeoCommandCommon::isEmpty(*setShape2))
		{
			aBuilder.Add(aRes, *setShape2);
		}
		TopoDS_Shape* newShape = new TopoDS_Shape;
		*newShape = aRes;
		const int id = Geometry::GeometrySet::getMaxID() + 1;
		name = name.arg(id);
		Geometry::GeometrySet* newset = new Geometry::GeometrySet;
		newset->setName(name);
		newset->setShape(newShape);
		_solid1.first->setShape(setCopyShape1);
		_solid2.first->setShape(setCopyShape2);
		newset->appendSubSet(_solid1.first);
		newset->appendSubSet(_solid2.first);
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
		Geometry::GeometryParaBoolOperation* para = new Geometry::GeometryParaBoolOperation;
		para->setInputBody1(_solid1);
		para->setInputBody2(_solid2);
		para->setType(_type);
		newset->setParameter(para);

		_geoData->removeTopGeometrySet(_solid1.first);
		_geoData->removeTopGeometrySet(_solid2.first);
		emit removeDisplayActor(_solid1.first);
		emit removeDisplayActor(_solid2.first);
		emit showSet(newset);
		emit updateGeoTree();

		GeoCommandBase::execute();
		return true;
	}

	void CommandBool::undo()
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

	void CommandBool::redo()
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
			_geoData->removeTopGeometrySet(_solid1.first);
			_geoData->removeTopGeometrySet(_solid2.first);
			emit removeDisplayActor(_solid1.first);
			emit removeDisplayActor(_solid2.first);
			emit showSet(_result);
		}
		else
		{
			emit removeDisplayActor(_solid1.first);
			emit removeDisplayActor(_solid2.first);

			_geoData->removeTopGeometrySet(_solid1.first);
			_geoData->removeTopGeometrySet(_solid2.first);

			_result->appendSubSet(_solid1.first);
			_result->appendSubSet(_solid2.first);

			_geoData->appendGeometrySet(_result);
			emit showSet(_result);
		}
		emit updateGeoTree();
	}

	void CommandBool::releaseResult()
	{

	}

	void CommandBool::setType(BoolType t)
	{
		_type = t;
	}

	void CommandBool::setSolid1(QPair <Geometry::GeometrySet*, int> solid1)
	{
		_solid1 = solid1;
	}

	void CommandBool::setSolid2(QPair <Geometry::GeometrySet*, int> solid2)
	{
		_solid2 = solid2;
	}

	TopoDS_Shape* CommandBool::cut()
	{
		TopoDS_Shape shape1 = *(_solid1.first->getShape(4, _solid1.second));
		TopoDS_Shape shape2 = *(_solid2.first->getShape(4, _solid2.second));
		const TopoDS_Shape& aCuttedShape = BRepAlgoAPI_Cut(shape1, shape2);
		if (aCuttedShape.IsNull()) return nullptr;
		auto s = new TopoDS_Shape;
		*s = aCuttedShape;
		return s;
	}

	TopoDS_Shape* CommandBool::fause()
	{
		TopoDS_Shape shape1 =* (_solid1.first->getShape(4, _solid1.second));
		TopoDS_Shape shape2 =* (_solid2.first->getShape(4, _solid2.second));
		if (shape1.IsNull() || shape2.IsNull()) return nullptr;
		BRepAlgoAPI_Fuse fau(shape1, shape2);
		
		if (!fau.IsDone()) return nullptr;
		const TopoDS_Shape& aFusedShape = fau.Shape();

		if (aFusedShape.IsNull()) return nullptr;
		TopoDS_Shape shape = GeoCommandCommon::removeSplitter( aFusedShape);

		auto s = new TopoDS_Shape;
		*s = shape;
		return s;
	}

	TopoDS_Shape* CommandBool::common()
	{
		TopoDS_Shape shape1 = *(_solid1.first->getShape(4, _solid1.second));
		TopoDS_Shape shape2 = *(_solid2.first->getShape(4, _solid2.second));
	
		const TopoDS_Shape& aFusedShape = BRepAlgoAPI_Common(shape1, shape2);
		if (!GeoCommandCommon::isEmpty(aFusedShape))
		{
			auto s = new TopoDS_Shape;
			*s = aFusedShape;
			return s;
		}
		
		BRepAlgo_Section S(shape1, shape2, false);
		S.Build();
		if (S.IsDone())
		{
			auto s = new TopoDS_Shape;
			*s = S.Shape();
			return s;
		}
		return nullptr;
		
	}

}