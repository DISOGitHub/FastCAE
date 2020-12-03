#include "GeoCommandMakeRemoveSurface.h"
#include "geometry/geometrySet.h"
#include "geometry/geometryData.h"
#include <TopExp.hxx>
#include <TopoDS.hxx>
#include <TopoDS_TShape.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopExp_Explorer.hxx>
#include <QDebug>
#include <BRepAlgoAPI_Defeaturing.hxx>
#include "geometry/geometryParaMakeFillHole.h"
namespace Command
{
	CommandMakeRemoveSurface::CommandMakeRemoveSurface(GUI::MainWindow* m, MainWidget::PreWindow* p)
		:GeoCommandBase(m, p)
	{

	}

	bool CommandMakeRemoveSurface::execute()
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

			TopTools_ListOfShape aFeatures;// Features to remove from the shape
			for (int j = 0; j < count; ++j)
			{
				const int index = shapes.at(j);
				TopExp_Explorer edgeExp(*parent, TopAbs_FACE);
				for (int k = 0; k < index; ++k) edgeExp.Next();
				const TopoDS_Shape& shape = edgeExp.Current();
				aFeatures.Append(shape);
			}
			if (aFeatures.Size() < 1) return false;
			Standard_Boolean bRunParallel = true;     
			Standard_Boolean isHistoryNeeded = false; 
			BRepAlgoAPI_Defeaturing aDF;
			aDF.SetShape(*parent);
			aDF.AddFacesToRemove(aFeatures);
			aDF.SetRunParallel(bRunParallel);
			aDF.SetToFillHistory(isHistoryNeeded);
			aDF.Build();
			if (!aDF.IsDone())
			{
				// error treatment
				Standard_SStream aSStream;
				aDF.DumpErrors(aSStream);
				return false;
			}
			if (aDF.HasWarnings())                   // Check for the warnings
			{
				// warnings treatment
				Standard_SStream aSStream;
				aDF.DumpWarnings(aSStream);
			}
			success = true;
			
			const TopoDS_Shape &resShape = aDF.Shape();
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

			Geometry::GeometryParaMakeFillHole* para = new Geometry::GeometryParaMakeFillHole;
			para->setName(name);
			para->setInputSet(set);
			para->setFaceList(shapes);

			newset->setParameter(para);
			GeoCommandBase::execute();

			emit removeDisplayActor(set);
			emit showSet(newset);

		}

		emit updateGeoTree();
		return success;
	}

	void CommandMakeRemoveSurface::undo()
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

	void CommandMakeRemoveSurface::redo()
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

	void CommandMakeRemoveSurface::releaseResult()
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

	void CommandMakeRemoveSurface::setShapeList(QMultiHash<Geometry::GeometrySet*, int>  s)
	{
		_shapeHash = s;
	}

}