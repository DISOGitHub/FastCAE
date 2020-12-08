#include "GeoCommandMakeSweep.h"
#include <TopoDS_Edge.hxx>
#include <list>
#include <vector>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include "geometry/geometrySet.h"
#include "geometry/geometryData.h"
#include "GeometryCommand/GeoCommandCommon.h"
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepOffsetAPI_MakePipe.hxx>
#include "geometry/geometryParaSweep.h"

namespace Command
{
	GeoCommandMakeSweep::GeoCommandMakeSweep(GUI::MainWindow* m, MainWidget::PreWindow* p)
		: GeoCommandBase(m, p)
	{
	}

	bool GeoCommandMakeSweep::execute()
	{
		QList<Geometry::GeometrySet*> setList = _sectionEdge.uniqueKeys();
		if (setList.size() < 1) return false;
		bool success = false;

		std::list<TopoDS_Edge> edgeList;
		for (int i = 0; i < setList.size(); ++i)
		{
			Geometry::GeometrySet* set = setList.at(i);
			TopoDS_Shape* parent = set->getShape();

			QList<int> shapes = _sectionEdge.values(set);
			const int count = shapes.size();
			if (count < 1) continue;

			for (int j = 0; j < count; j++)
			{
				const int index = shapes.at(j);

				TopAbs_ShapeEnum type = TopAbs_EDGE;
				TopExp_Explorer ptExp(*parent, type);
				for (int k = 0; k < index; ++k) ptExp.Next();
				const TopoDS_Shape& shapecurrent = ptExp.Current();
				const TopoDS_Edge& edge = TopoDS::Edge(shapecurrent);
				if (edge.IsNull()) continue;
				edgeList.push_back(edge);
			}
		}
		std::vector<TopoDS_Wire> wireList = GeoCommandCommon::bulidWire(edgeList);
		if (wireList.size() < 1) return false;

		TopoDS_Shape section;
		if (!_isSolid)
		{
			TopoDS_Compound comp;
			BRep_Builder builder;
			builder.MakeCompound(comp);
			for (std::vector<TopoDS_Wire>::iterator it = wireList.begin(); it != wireList.end(); ++it)
			{
				TopoDS_Wire wire = *it;
				if (wire.IsNull()) continue;
				builder.Add(comp, wire);
			}
			section = comp;
		}
		else
		{
			section = GeoCommandCommon::makeFace(wireList);			
		}
		if (section.IsNull()) return false;

		TopoDS_Shape* pathset = _pathEdge.first->getShape();
		const int index = _pathEdge.second;
		TopAbs_ShapeEnum type = TopAbs_EDGE;
		TopExp_Explorer ptExp(*pathset, type);
		for (int k = 0; k < index; ++k) ptExp.Next();
		const TopoDS_Shape& shapecurrent = ptExp.Current();
		const TopoDS_Edge& edge = TopoDS::Edge(shapecurrent);
		TopoDS_Wire path = BRepBuilderAPI_MakeWire(edge);
		if (path.IsNull()) return false;
		BRepOffsetAPI_MakePipe aPipeMaker(path, section);
		aPipeMaker.Build();
		if (!aPipeMaker.IsDone()) return false;

		TopoDS_Shape* shape = new TopoDS_Shape;
		*shape = aPipeMaker.Shape();
		Geometry::GeometrySet* newset = new Geometry::GeometrySet(Geometry::STEP);
		newset->setShape(shape);
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
			const int id = Geometry::GeometrySet::getMaxID() + 1;
			QString name = QString("Sweep_%1").arg(id);
			newset->setName(name);
			_geoData->appendGeometrySet(newset);
		}

		Geometry::GeometryParaSweep* para = new Geometry::GeometryParaSweep;
		para->setShapeHash(_sectionEdge);
		para->setSolid(_isSolid);
		para->setPath(_pathEdge);
		_result->setParameter(para);

		GeoCommandBase::execute();
		emit showSet(newset);
		emit updateGeoTree();
		return true;
	}

	void GeoCommandMakeSweep::undo()
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

	void GeoCommandMakeSweep::redo()
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

	void GeoCommandMakeSweep::setSection(QMultiHash<Geometry::GeometrySet*, int> s)
	{
		_sectionEdge = s;
	}

	void GeoCommandMakeSweep::setPath(QPair<Geometry::GeometrySet*, int> p)
	{
		_pathEdge = p;
	}

	void GeoCommandMakeSweep::isSolid(bool is)
	{
		_isSolid = is;
	}

	void GeoCommandMakeSweep::releaseResult()
	{
		if (_result != nullptr) delete _result;
		_result = nullptr;
	}




}