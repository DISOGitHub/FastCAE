#include "GeoCommandCreateFace.h"
#include "geometry/geometrySet.h"
#include "geometry/geometryData.h"
#include <BRepFilletAPI_MakeChamfer.hxx>
#include <TopExp.hxx>
#include <TopoDS.hxx>
#include <TopoDS_TShape.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <QDebug>
#include "GeoCommandCommon.h"
#include <list>
#include "geometry/geometryParaFace.h"
namespace Command
{
	CommandCreateFace::CommandCreateFace(GUI::MainWindow* m, MainWidget::PreWindow* p)
		:GeoCommandBase(m,p)
	{

	}

	bool CommandCreateFace::execute()
	{
		if (_shapeHash.size() < 1) return false;
		QList<Geometry::GeometrySet*> setList = _shapeHash.uniqueKeys();
		
		std::list<TopoDS_Edge> edgeList;
		for (int i = 0; i < setList.size(); ++i)
		{
			Geometry::GeometrySet* set = setList.at(i);
			TopoDS_Shape* parent = set->getShape();
			
			QList<int> shapes = _shapeHash.values(set);
			const int count = shapes.size();

			for (int j=0; j < count; ++j)
			{
				const int index = shapes.at(j);
				TopExp_Explorer edgeExp(*parent, TopAbs_EDGE);
				for (int k = 0; k < index; ++k) edgeExp.Next();

				const TopoDS_Shape& shape = edgeExp.Current();
				const TopoDS_Edge& E = TopoDS::Edge(shape);
				edgeList.push_back(E);
			}
		}

		std::vector<TopoDS_Wire> wireList = GeoCommandCommon::bulidWire(edgeList);
		TopoDS_Shape resShape = GeoCommandCommon::makeFace(wireList);
		if (resShape.IsNull() && _isEdit)
		{
			emit showSet(_editSet);
			return false;
		}
		if (resShape.IsNull()) return false;

		TopoDS_Shape* successShape = new TopoDS_Shape;
		*successShape = resShape;

		Geometry::GeometrySet* newset = new Geometry::GeometrySet(Geometry::STEP);
		newset->setName(_name);
		newset->setShape(successShape);
		//_geoData->appendGeometrySet(newset);
		_result = newset;
		if (_isEdit)
		{
			newset->setName(_editSet->getName());
			_geoData->replaceSet(newset, _editSet);
			emit removeDisplayActor(_editSet);
		}
		else
		{
			newset->setName(_name);
			_geoData->appendGeometrySet(newset);
		}
		Geometry::GeometryParaFace* para = new Geometry::GeometryParaFace;
		para->setName(_name);
		para->setShapeHash(_shapeHash);
		_result->setParameter(para);

		GeoCommandBase::execute();
		emit showSet(newset);
		emit updateGeoTree();
		return true;
	}

	void CommandCreateFace::undo()
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

	void CommandCreateFace::redo()
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

	void CommandCreateFace::releaseResult()
	{
		if (_result != nullptr) delete _result;

	}

	void CommandCreateFace::setShapeList(QMultiHash<Geometry::GeometrySet*, int>  s)
	{
		_shapeHash = s;
	}

	void CommandCreateFace::setActor(QList<vtkActor*> ac)
	{
		_actor = ac;
	}

	void CommandCreateFace::setName(QString name)
	{
		_name = name;
	}

}
