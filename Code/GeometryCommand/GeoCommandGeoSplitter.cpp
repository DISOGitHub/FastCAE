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
namespace Command
{
	CommandGeoSplitter::CommandGeoSplitter(GUI::MainWindow* m, MainWidget::PreWindow* p)
		: GeoCommandBase(m, p)
	{
	}

	bool CommandGeoSplitter::execute()
	{
		bool success{ true };
		if (_body == nullptr || _faceBody == nullptr || _faceIndex < 0)
			success = false;
		TopoDS_Shape* box = _body->getShape();
		TopoDS_Shape* shape = _faceBody->getShape();
		TopExp_Explorer faceExp(*shape, TopAbs_FACE);
		for (int index = 0; index < _faceIndex && faceExp.More(); faceExp.Next(), ++index);
		const TopoDS_Shape& faceShape = faceExp.Current();
		if (faceShape.IsNull()) return false;

		BOPAlgo_Splitter splitter;
		splitter.AddArgument(*box);
		splitter.AddTool(faceShape);
		splitter.Perform();
		//·Ö¸îµÄÌå
		TopoDS_Compound comp;
		BRep_Builder builder;
		builder.MakeCompound(comp);

		TopExp_Explorer shapeExp(splitter.Shape(), TopAbs_SOLID);
		for (; shapeExp.More(); shapeExp.Next())
		{
			const TopoDS_Solid& anSolid = TopoDS::Solid(shapeExp.Current());
			builder.Add(comp, anSolid);
		}
		
/*
		QList<Handle(TopoDS_TShape)> tlist{};

		TopExp_Explorer comExp(comp, TopAbs_FACE);
		for (; comExp.More(); comExp.Next())
		{
			const TopoDS_Shape& shape = comExp.Current();
			const Handle(TopoDS_TShape)&  tshape = shape.TShape();
			tlist.push_back(tshape);
		}*/

 		TopoDS_Shape* mshape = new TopoDS_Shape;
		*mshape = comp;
		QString name = QString("Splitter-%1").arg(_body->getName());
		Geometry::GeometrySet* newset = new Geometry::GeometrySet;
		newset->setName(name);
		newset->setShape(mshape);
		//newset->appendSubSet(_body);
		//_result = newset;
		if (_isEdit)
		{
			newset->setName(_editSet->getName());
			//_geoData->appendGeometrySet(newset);
			_geoData->replaceSet(newset, _editSet);
		}
		else
		{
			newset->setName(name);
			_geoData->appendGeometrySet(newset);

		}
		_geoData->removeTopGeometrySet(_body);
		newset->appendSubSet(_body);
		_inputOutputHash.insert(_body ,newset);

		int k = _inputOutputHash.size();
		Geometry::GeometryParaGeoSplitter* para = new Geometry::GeometryParaGeoSplitter;
		para->setName(name);
		para->setOriSet(_body);
		para->setFaceIndex(_faceIndex);
		para->setFaceBody(_faceBody);
		newset->setParameter(para);
		GeoCommandBase::execute();
		//_resultOriginHash.insert(newset, _body);
		emit removeDisplayActor(_body);
		emit showSet(newset);
		emit updateGeoTree();
		return success;
	}

	void CommandGeoSplitter::undo()
	{
		if (_isEdit)
		{
			Geometry::GeometrySet* orset = _editSet->getSubSetAt(0);
			QList<Geometry::GeometrySet*> setlist = _inputOutputHash.values();
			Geometry::GeometrySet* resplace = setlist.back();
			_geoData->replaceSet(_editSet, resplace);
			emit showSet(resplace->getSubSetAt(0));
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

	void CommandGeoSplitter::redo()
	{
		if (_isEdit)
		{
			Geometry::GeometrySet* orset = _editSet->getSubSetAt(0);
			QList<Geometry::GeometrySet*> setlist = _inputOutputHash.values();
			Geometry::GeometrySet* resplace = setlist.back();
			_geoData->replaceSet( resplace,_editSet);
			emit showSet(orset);
			emit removeDisplayActor(resplace->getSubSetAt(0));
			emit showSet(resplace);
			emit removeDisplayActor(_editSet);


/*

			Geometry::GeometrySet* resplace = _inputOutputHash.value(orset);
			_geoData->replaceSet(resplace, _editSet);
			emit removeDisplayActor(_editSet);
			emit showSet(resplace);*/
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

	void CommandGeoSplitter::releaseResult()
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
	  
	void CommandGeoSplitter::setBody(Geometry::GeometrySet* b)
	{
		_body = b;
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