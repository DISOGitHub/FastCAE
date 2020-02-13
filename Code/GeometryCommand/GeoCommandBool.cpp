#include "GeoCommandBool.h"
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include <TopoDS_Shape.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgo_Fuse.hxx>
#include <BRepAlgo_Section.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS.hxx>
#include "GeoCommandCommon.h"
#include "geometry/geometryParaBoolOperation.h"

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
		if (shape == nullptr) return false;

		const int id = Geometry::GeometrySet::getMaxID() + 1;
		name = name.arg(id);
		Geometry::GeometrySet* newset = new Geometry::GeometrySet;
		newset->setName(name);
		newset->setShape(shape);
		newset->appendSubSet(_body1);
		newset->appendSubSet(_body2);

		_result = newset;

		if (_isEdit)
		{
			newset->setName(_editSet->getName());
			_geoData->replaceSet(newset, _editSet);

			const int n = _editSet->getSubSetCount();
			for (int i = 0; i < n; ++i)
			{
				Geometry::GeometrySet* subset = _editSet->getSubSetAt(i);
				//_editSet->removeSubSet(subset);
				_geoData->appendGeometrySet(subset);
			}
		}
		else
		{
			newset->setName(name);
			_geoData->appendGeometrySet(newset);
		}
		Geometry::GeometryParaBoolOperation* para = new Geometry::GeometryParaBoolOperation;

		para->setInputBody1(_body1);
		para->setInputBody2(_body2);
		para->setType(_type);  
		newset->setParameter(para);

		//_geoData->appendGeometrySet(newset);
		_geoData->removeTopGeometrySet(_body1);
		_geoData->removeTopGeometrySet(_body2);
		emit removeDisplayActor(_body1);
		emit removeDisplayActor(_body2);
		emit showSet(newset);
		emit updateGeoTree();

		GeoCommandBase::execute();
		return true;
	}

	void CommandBool::undo()
	{
		if (_isEdit)
		{
			_geoData->replaceSet(_editSet,_result);
			emit removeDisplayActor(_result);//c2
			
			const int n = _result->getSubSetCount();//Ìí¼Ób2,b3
			for (int i = 0; i < n; ++i)
			{
				Geometry::GeometrySet* subset = _result->getSubSetAt(i);
				_geoData->appendGeometrySet(subset);
				emit showSet(subset);
			}


			const int m = _editSet->getSubSetCount();//É¾³ýb1,b2.
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
				//_editSet->removeSubSet(subset);
				_geoData->appendGeometrySet(subset);
				emit showSet(subset);
			}
			_geoData->removeTopGeometrySet(_body1);
			_geoData->removeTopGeometrySet(_body2);
			emit removeDisplayActor(_body1);
			emit removeDisplayActor(_body2);
			emit showSet(_result);

			
		}
		else
		{
			emit removeDisplayActor(_body1);
			emit removeDisplayActor(_body2);

			_geoData->removeTopGeometrySet(_body1);
			_geoData->removeTopGeometrySet(_body2);

			_result->appendSubSet(_body1);
			_result->appendSubSet(_body2);

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

	void CommandBool::setInputBody(Geometry::GeometrySet* b1, Geometry::GeometrySet* b2)
	{
		_body1 = b1;
		_body2 = b2;
	}

	TopoDS_Shape* CommandBool::cut()
	{
		TopoDS_Shape* shape1 = _body1->getShape();
		TopoDS_Shape* shape2 = _body2->getShape();
		const TopoDS_Shape& aCuttedShape = BRepAlgoAPI_Cut(*shape1, *shape2);
		if (aCuttedShape.IsNull()) return nullptr;
		auto s = new TopoDS_Shape;
		*s = aCuttedShape;
		return s;
	}

	TopoDS_Shape* CommandBool::fause()
	{
		TopoDS_Shape* shape1 = _body1->getShape();
		TopoDS_Shape* shape2 = _body2->getShape();
		BRepAlgo_Fuse fau(*shape1, *shape2);
		fau.PerformDS();
		fau.Build();
		
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
		TopoDS_Shape* shape1 = _body1->getShape();
		TopoDS_Shape* shape2 = _body2->getShape();
	
		const TopoDS_Shape& aFusedShape = BRepAlgoAPI_Common(*shape1, *shape2);
		if (!GeoCommandCommon::isEmpty(aFusedShape))
		{
			auto s = new TopoDS_Shape;
			*s = aFusedShape;
			return s;
		}
		
		BRepAlgo_Section S(*shape1, *shape2, false);
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