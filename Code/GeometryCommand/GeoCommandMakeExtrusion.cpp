#include "GeoCommandMakeExtrusion.h"
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
#include <gp_Elips.hxx>
#include <vector>
#include <list>
#include "geometry/geometryParaExtrusion.h"
#include "GeoCommandCommon.h"
#include "python/PyAgent.h"

namespace Command
{
	GeoCommandMakeExtrusion::GeoCommandMakeExtrusion(GUI::MainWindow* m, MainWidget::PreWindow* p)
		:GeoCommandBase(m,p)
	{

	}
	bool GeoCommandMakeExtrusion::execute()
	{
		this->getAxis();
		QList<Geometry::GeometrySet*> setList = _shapeHash.uniqueKeys();
		if (setList.size() < 1) return false;
		bool success = false;

		std::list<TopoDS_Edge> edgeList;
		for (int i = 0; i < setList.size(); ++i)
		{
			Geometry::GeometrySet* set = setList.at(i);
			TopoDS_Shape* parent = set->getShape();
			
			QList<int> shapes = _shapeHash.values(set);
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
		TopoDS_Shape ResShape;
		if (!_isSolid)
		{
			TopoDS_Compound comp;
			BRep_Builder builder;
			builder.MakeCompound(comp);

			for ( std::vector<TopoDS_Wire>::iterator it = wireList.begin(); it != wireList.end(); ++it)
			{
				TopoDS_Wire wire = *it;
				if (wire.IsNull()) continue;
				TopoDS_Shape aPrism = BRepPrimAPI_MakePrism(wire, gp_Vec(_axis[0], _axis[1], _axis[2]), true, false);
				if (!aPrism.IsNull())
					builder.Add(comp, aPrism);
			}
			ResShape = comp;
			
		}
		else
		{
			TopoDS_Shape face = GeoCommandCommon::makeFace(wireList);
			if (face.IsNull()) return false;
			ResShape = BRepPrimAPI_MakePrism(face, gp_Vec(_axis[0], _axis[1], _axis[2]), true, false);

		}

		if (ResShape.IsNull())
		{
			return false;
		}

		TopoDS_Shape* shape = new TopoDS_Shape;
		*shape = ResShape;
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
			newset->setName(_extName);
			_geoData->appendGeometrySet(newset);
		}

		Geometry::GeometryParaExtrusion* para = new Geometry::GeometryParaExtrusion;
		para->setName(_extName);
		para->setShapeHash(_shapeHash);
		para->setDirection(_direcion);
		para->setDistance(_distance);
		para->setReverse(_reverse);
		para->setSolid(_isSolid);
		_result->setParameter(para);

		GeoCommandBase::execute();
		emit showSet(newset);
		emit updateGeoTree();
		return true;
	}
	void GeoCommandMakeExtrusion::undo()
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

	void GeoCommandMakeExtrusion::redo()
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


	void GeoCommandMakeExtrusion::setAxis(double* axis)
	{
		_axis[0] = axis[0];
		_axis[1] = axis[1];
		_axis[2] = axis[2];
	}

	
	void GeoCommandMakeExtrusion::setShapeList(QMultiHash<Geometry::GeometrySet*, int> s)
	{
		_shapeHash = s;
	}


	void GeoCommandMakeExtrusion::setName(QString name)
	{
		_extName = name;
	}

	void GeoCommandMakeExtrusion::isSolid(bool ischecked)
	{
		_isSolid = ischecked;
	}

	void GeoCommandMakeExtrusion::setDirection(double * dir)
	{
		for (int i = 0; i < 3;++i)
		{
			_direcion[i] = dir[i];
		}
	}

	void GeoCommandMakeExtrusion::setDistance(double dis)
	{
		_distance = dis;
	}

	void GeoCommandMakeExtrusion::setReverse(bool reverse)
	{
		_reverse = reverse;
	}

	void GeoCommandMakeExtrusion::getAxis()
	{
		for (int i = 0; i < 3;++i)
		{
			_axis[i] = _direcion[i];
		}
		if (_reverse)
		{
			for (int i = 0; i < 3; ++i)
				_axis[i] *= -1;
		}
	
		double mod = sqrt(_axis[0] * _axis[0] + _axis[1] * _axis[1] + _axis[2] * _axis[2]);
		if (mod < 1e-6) return;
		
		double factor = _distance / mod;
		for (int i = 0; i < 3; ++i)
			_axis[i] *= factor;
	}

	void GeoCommandMakeExtrusion::releaseResult()
	{
		if (_result != nullptr) delete _result;
		_result = nullptr;
	}
}
