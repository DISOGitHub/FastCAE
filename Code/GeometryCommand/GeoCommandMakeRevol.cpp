#include "GeoCommandMakeRevol.h"
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
#include "GeoCommandCommon.h"
#include <BRepAdaptor_Curve.hxx>
#include "geometry/geometryParaRevol.h"

namespace Command
{

	GeoCommandMakeRevol::GeoCommandMakeRevol(GUI::MainWindow* m, MainWidget::PreWindow* p)
		:GeoCommandBase(m,p)
	{

	}
	bool GeoCommandMakeRevol::execute()
	{
		QList<Geometry::GeometrySet*> setList = _shapeHash.uniqueKeys();
		if (setList.size() < 1) return false;
		bool success = false;

		gp_Ax1 anAxis;
		anAxis.SetLocation(gp_Pnt(_basicPoint[0], _basicPoint[1], _basicPoint[2]));
		double vec[3];
		getVec(vec);
		anAxis.SetDirection(gp_Dir(vec[0], vec[1], vec[2]));
		TopoDS_Shape relvo;
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
		double d = _degree*PI / 180.00;
		if (!_isSolid)
		{
			TopoDS_Compound comp;
			BRep_Builder builder;
			builder.MakeCompound(comp);

			for (std::vector<TopoDS_Wire>::iterator it = wireList.begin(); it != wireList.end(); ++it)
			{
				TopoDS_Wire wire = *it;
				if (wire.IsNull()) continue;
				
				relvo = BRepPrimAPI_MakeRevol(wire, anAxis, d, true);
				if (!relvo.IsNull())
					builder.Add(comp, relvo);
			}
			relvo = comp;
		}
		else
		{
			TopoDS_Shape face = GeoCommandCommon::makeFace(wireList);
			if (face.IsNull()) return false;
			relvo = BRepPrimAPI_MakeRevol(face, anAxis, d, true);

		}
		TopoDS_Shape* shape = new TopoDS_Shape;
		*shape = relvo;
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
		newset->setName(_extName);

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

		Geometry::GeometryParaRevol *para = new Geometry::GeometryParaRevol;
		para->setName(_extName);
		para->setShapeHash(_shapeHash);
		para->setBasciPoint(_basicPoint);
		para->setDegree(_degree);
		para->setMethod(_optionmethod);
		if (_optionmethod == 0)
			para->setAxisEdge(_aixsetedge);
		else
			para->setCoor(_coor);
		para->setReverse(_reverse);
		para->setSolid(_isSolid);
		_result->setParameter(para);
		
		emit showSet(newset);
		emit updateGeoTree();
		return true;
	}
	void GeoCommandMakeRevol::undo()
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

	void GeoCommandMakeRevol::redo()
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

	void GeoCommandMakeRevol::setOptionMethod(int i)
	{
		_optionmethod = i;
	}

	void GeoCommandMakeRevol::setAixSetEdge(QPair<Geometry::GeometrySet*, int> s)
	{
		_aixsetedge = s;
	}

	void GeoCommandMakeRevol::getVec(double * vec)
	{
		if (_optionmethod == 0)
		{
			if (_aixsetedge.first == nullptr || _aixsetedge.second < 0) return;
			TopoDS_Shape* shape = _aixsetedge.first->getShape();

			TopExp_Explorer edgeExp(*shape, TopAbs_EDGE);
			for (int k = 0; k < _aixsetedge.second; ++k) edgeExp.Next();

			const TopoDS_Edge &edge = TopoDS::Edge(edgeExp.Current());
			if (edge.IsNull()) return;

			BRepAdaptor_Curve adapt(edge);
			if (adapt.GetType() != GeomAbs_Line) return;
			gp_Lin line = adapt.Line();
			gp_Dir direc = line.Direction();
			vec[0] = direc.X();
			vec[1] = direc.Y();
			vec[2] = direc.Z();
		}
		else
		{
			for (int i = 0; i < 3; i++)
			{
				vec[i] = _coor[i];
			}
			double mod = vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
			if (mod < 0.000001) return;
		}
		if (_reverse==true)
		{
			for (int i = 0; i < 3; ++i)
				vec[i] *= -1;
		}
	}

	void GeoCommandMakeRevol::setCoor(double* coor)
	{
		for (int i = 0; i < 3; i++)
		{
			_coor[i] = coor[i];
		}
	}
	void GeoCommandMakeRevol::setReverse(bool s)
	{
		_reverse = s;
	}

	void GeoCommandMakeRevol::setShapeList(QMultiHash<Geometry::GeometrySet*, int> s)
	{
		_shapeHash = s;
	}

	void GeoCommandMakeRevol::setName(QString name)
	{
		_extName = name;
	}

	void GeoCommandMakeRevol::isSolid(bool ischecked)
	{
		_isSolid = ischecked;
	}

	void GeoCommandMakeRevol::setDegree(double d)
	{
		_degree = d;
	}

	void GeoCommandMakeRevol::releaseResult()
	{
		if (_result != nullptr) delete _result;
		_result = nullptr;
	}

	void GeoCommandMakeRevol::setBasicPoint(double* bp)
	{
		for (int i = 0; i < 3; ++i)
		{
			_basicPoint[i] = bp[i];
		}
	}
}
