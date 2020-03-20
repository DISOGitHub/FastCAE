#include "GeoCommandMakeLoft.h"
#include <gp_Ax2.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <TopExp_Explorer.hxx>
#include <QDebug>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <gp_Circ.hxx>
#include <gp_Elips.hxx>
#include <gp_Circ.hxx>
#include <gp_Elips.hxx>
#include <gp_Pln.hxx>
#include <TopExp.hxx>
#include <TopoDS.hxx>
#include <TopoDS_TShape.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>
#include <gp_Elips.hxx>
#include "geometry/geometryParaLoft.h"
#include "GeoCommandCommon.h"


namespace Command
{
	GeoCommandMakeLoft::GeoCommandMakeLoft(GUI::MainWindow* m, MainWidget::PreWindow* p)
		:GeoCommandBase(m,p)
	{

	}
	bool GeoCommandMakeLoft::execute()
	{
		BRepOffsetAPI_ThruSections loftGenerator(_isSolid);
		
		const int n = _shapeHash.size();
		if (n < 2) return false;
		
		for (int i = 0; i < n; ++i)
		{
			QMultiHash<Geometry::GeometrySet*, int> shape = _shapeHash.at(i);
			std::list<TopoDS_Edge> edgeList = this->bulidEdgeList(shape);
			std::vector<TopoDS_Wire> wireList = GeoCommandCommon::bulidWire(edgeList);
			if (wireList.size() != 1)  return false;
			TopoDS_Wire w = wireList.at(0);
			if (_isSolid)
			{
				if (!w.Closed()) 
					return false;
			}

			loftGenerator.AddWire(w);

		}

		loftGenerator.Build();
		if (!loftGenerator.IsDone()) return false;
	    TopoDS_Shape shapeloft = loftGenerator.Shape();
		TopoDS_Shape* shape = new TopoDS_Shape;
		*shape = shapeloft;
		if (shape->IsNull())
		{
			delete shape;
			return false;
		}
		GeoCommandBase::execute();
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
			newset->setName(_name);
			_geoData->appendGeometrySet(newset);
		}
		Geometry::GeometryParaLoft* para = new Geometry::GeometryParaLoft;
		para->setName(_name);
		para->setShapeHash(_shapeHash);
		para->setSolid(_isSolid);
		_result->setParameter(para);

		emit showSet(newset);
		emit updateGeoTree();
		return true;

	}
	void GeoCommandMakeLoft::undo()
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

	void GeoCommandMakeLoft::redo()
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

	void GeoCommandMakeLoft::setName(QString name)
	{
		_name = name;
	}

	void GeoCommandMakeLoft::isSolid(bool s)
	{
		_isSolid = s;
	}

	void GeoCommandMakeLoft::releaseResult()
	{
		if (_result != nullptr) delete _result;
		_result = nullptr;
	}

	std::list<TopoDS_Edge> GeoCommandMakeLoft::bulidEdgeList(QMultiHash<Geometry::GeometrySet*, int> s)
	{
		std::list<TopoDS_Edge> edgeList;
		QList<Geometry::GeometrySet*> setList = s.uniqueKeys();

		for (int i = 0; i < setList.size(); ++i)
		{
			Geometry::GeometrySet* set = setList.at(i);
			TopoDS_Shape* parent = set->getShape();

			QList<int> shapes = s.values(set);
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
		return edgeList;
	}

	void GeoCommandMakeLoft::setShapeList(QList<QMultiHash<Geometry::GeometrySet*, int>> s)
	{
		_shapeHash = s;
	}

}
