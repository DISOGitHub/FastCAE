#include "GeoCommandCreateChamfer.h"
#include "geometry/geometrySet.h"
#include "geometry/geometryData.h"
#include <BRepFilletAPI_MakeChamfer.hxx>
#include <TopExp.hxx>
#include <TopoDS.hxx>
#include <TopoDS_TShape.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopExp_Explorer.hxx>
#include <QDebug>
#include "geometry/geometryParaChamfer.h"
namespace Command
{
	CommandCreateChamfer::CommandCreateChamfer(GUI::MainWindow* m, MainWidget::PreWindow* p)
		:GeoCommandBase(m,p)
	{

	}

	bool CommandCreateChamfer::execute()
	{

		QList<Geometry::GeometrySet*> setList = _shapeHash.uniqueKeys();
		bool success = false;

		for (int i = 0; i < setList.size(); ++i)
		{
			Geometry::GeometrySet* set = setList.at(i);
			TopoDS_Shape* parent = set->getShape();
			
			QList<int> shapes = _shapeHash.values(set);
			const int count = shapes.size();
			if (count < 1) continue;
			
			BRepFilletAPI_MakeChamfer MC(*parent);
			TopTools_IndexedDataMapOfShapeListOfShape aEdgeFaceMap;
			TopExp::MapShapesAndAncestors(*parent, TopAbs_EDGE, TopAbs_FACE, aEdgeFaceMap);
			bool onesucc = false;
			for (int j=0; j < count; ++j)
			{
				const int index = shapes.at(j);
				TopExp_Explorer edgeExp(*parent, TopAbs_EDGE);
				for (int k = 0; k < index; ++k) edgeExp.Next();

				const TopoDS_Shape& shape = edgeExp.Current();
				const TopoDS_Edge& E = TopoDS::Edge(shape);

				TopTools_ListOfShape faces = aEdgeFaceMap.FindFromKey(shape);
				if (faces.Size() < 2)  continue;;

				const TopoDS_Face& F = TopoDS::Face(faces.First());
				if (_symmetrical)
					MC.Add(_distance1, _distance1, E, F);
				else
					MC.Add(_distance1, _distance2, E, F);
				onesucc = true;
			}

			if (!onesucc) continue;
			success = true;

			MC.Build();
			if (!MC.IsDone()) continue;

			const TopoDS_Shape& resShape = MC.Shape();
			TopoDS_Shape* successShape = new TopoDS_Shape;
			*successShape = resShape;

			const int id = Geometry::GeometrySet::getMaxID() + 1;
			QString name = _name.arg(id);

			Geometry::GeometrySet* newset = new Geometry::GeometrySet(Geometry::STEP);
			newset->setName(name);
			newset->setShape(successShape);
			//_geoData->appendGeometrySet(newset);
			if (_isEdit)
			{
				newset->setName(_editSet->getName());
				_geoData->replaceSet(newset, _editSet);
			}
			else
			{
				newset->setName(name);
				_geoData->appendGeometrySet(newset);
				
			}
			_geoData->removeTopGeometrySet(set);
			newset->appendSubSet(set);
			_inputOutputHash.insert(set, newset);

			Geometry::GeometryParaChamfer* para = new Geometry::GeometryParaChamfer;
			para->setName(name);
			para->setInputSet(set);
			para->setEdgeIndexList(shapes);
			para->setCombIndex(_symmetrical);
			para->setDistance1(_distance1);
			para->setDistance2(_distance2);
			newset->setParameter(para);
			GeoCommandBase::execute();

			emit removeDisplayActor(set);
			emit showSet(newset);

		}

		emit updateGeoTree();
		return success;
	}

	void CommandCreateChamfer::undo()
	{
		if (_isEdit)
		{
			Geometry::GeometrySet* orset = _editSet->getSubSetAt(0);
			Geometry::GeometrySet* resplace = _inputOutputHash.value(orset);
			_geoData->replaceSet(_editSet, resplace);
			emit removeDisplayActor(resplace);
			emit showSet(_editSet);
		}
		else
		{
			QList<Geometry::GeometrySet*> inputList = _inputOutputHash.keys();
			const int n = inputList.size();
			for (int i = 0; i < n; ++i)
			{
				Geometry::GeometrySet* inputSet = inputList.at(i);
				Geometry::GeometrySet* outputSet = _inputOutputHash.value(inputSet);
				emit removeDisplayActor(outputSet);

				outputSet->removeSubSet(inputSet);
				_geoData->removeTopGeometrySet(outputSet);
				_geoData->appendGeometrySet(inputSet);

				emit showSet(inputSet);
			}
		}

		emit updateGeoTree();
	}

	void CommandCreateChamfer::redo()
	{
		if (_isEdit)
		{
			Geometry::GeometrySet* orset = _editSet->getSubSetAt(0);
			Geometry::GeometrySet* resplace = _inputOutputHash.value(orset);
			_geoData->replaceSet(resplace, _editSet);
			emit removeDisplayActor(_editSet);
			emit showSet(resplace);
		}
		else
		{
			QList<Geometry::GeometrySet*> inputList = _inputOutputHash.keys();
			const int n = inputList.size();
			for (int i = 0; i < n; ++i)
			{
				Geometry::GeometrySet* inputSet = inputList.at(i);
				Geometry::GeometrySet* outputSet = _inputOutputHash.value(inputSet);
				emit removeDisplayActor(inputSet);

				outputSet->appendSubSet(inputSet);
				_geoData->removeTopGeometrySet(inputSet);
				_geoData->appendGeometrySet(outputSet);

				emit showSet(outputSet);
			}

		}
		emit updateGeoTree();
	}

	void CommandCreateChamfer::releaseResult()
	{
		QList<Geometry::GeometrySet*> outputList = _inputOutputHash.values();
		const int n = outputList.size();
		for (int i = 0; i < n; ++i)
		{
			auto set = outputList.at(i);
			delete set;
		}
		_inputOutputHash.clear();
	}

	void CommandCreateChamfer::setShapeList(QMultiHash<Geometry::GeometrySet*, int>  s)
	{
		_shapeHash = s;
	}

	void CommandCreateChamfer::setDistance(double d1,double d2)
	{
		_distance1 = d1;
		_distance2 = d2;
	}

	void CommandCreateChamfer::setSymmetrical(bool is)
	{
		_symmetrical = is;
	}

}
