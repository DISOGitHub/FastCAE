#include "GeoCommandCreateFillet.h"
#include "geometry/geometrySet.h"
#include "geometry/geometryData.h"
#include "geometry/geometryParaFillet.h"
#include <BRepFilletAPI_MakeFillet.hxx>
#include <TopExp.hxx>
#include <TopoDS.hxx>
#include <TopoDS_TShape.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopExp_Explorer.hxx>
#include <QDebug>

namespace Command
{
	CommandCreateFillet::CommandCreateFillet(GUI::MainWindow* m, MainWidget::PreWindow* p)
		:GeoCommandBase(m,p)
	{

	}

	bool CommandCreateFillet::execute()
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
			
			BRepFilletAPI_MakeFillet MF(*parent);

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
				if (faces.Size() < 2)  continue;
				MF.Add(_radius, E);
				onesucc = true;
			}
			if (!onesucc) continue;
			success = true;
			MF.Build();
			if (!MF.IsDone()) continue;

			const TopoDS_Shape& resShape = MF.Shape();

			TopoDS_Shape* successShape = new TopoDS_Shape;
			*successShape = resShape;

			const int id = Geometry::GeometrySet::getMaxID() + 1;
			QString name = _name.arg(id);

			Geometry::GeometrySet* newset = new Geometry::GeometrySet(Geometry::STEP);
			newset->setShape(successShape);
			if (_isEdit)
			{
				newset->setName(_editSet->getName());
				_geoData->replaceSet(newset, _editSet);
				emit removeDisplayActor(_editSet);
				
			}
			else
			{
				newset->setName(name);
				_geoData->appendGeometrySet(newset);
			}
			
			_geoData->removeTopGeometrySet(set);
			newset->appendSubSet(set);
			_inputOutputHash.insert(set, newset);

			Geometry::GeometryParaFillet* filet = new Geometry::GeometryParaFillet;
			filet->setGeometrySet(set);
			filet->setEdgeList(shapes);
			filet->setRadius(_radius);
			newset->setParameter(filet);

			GeoCommandBase::execute();
			emit removeDisplayActor(set);
			emit showSet(newset);

		}
		emit updateGeoTree();
		return success;
	}

	void CommandCreateFillet::undo()
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

	void CommandCreateFillet::redo()
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

	void CommandCreateFillet::releaseResult()
	{
		QList<Geometry::GeometrySet*> outputList = _inputOutputHash.values();
		const int n = outputList.size();
		for (int i = 0; i < n; ++i)
		{
			auto set = outputList.at(i);
			if (!_isEdit) set->releaseSubSet();
			delete set;
		}
		_inputOutputHash.clear();
	}

	void CommandCreateFillet::setShapeList(QMultiHash<Geometry::GeometrySet*, int>  s)
	{
		_shapeHash = s;
	}

	void CommandCreateFillet::setRadius(double d)
	{
		_radius = d;
	}

}
