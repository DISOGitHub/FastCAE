#include "GeoCommandCreateVariableFillet.h"
#include <TopExp.hxx>
#include <TopoDS.hxx>
#include <TopoDS_TShape.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <QDebug>
#include "geometry/geometrySet.h"
#include "geometry/geometryData.h"
#include "geometry/geometryParaVariableFillet.h"
namespace Command
{
	CommandCreateVariableFillet::CommandCreateVariableFillet(GUI::MainWindow* m, MainWidget::PreWindow* p)
		:GeoCommandBase(m, p)
	{
	}

	bool CommandCreateVariableFillet::execute()
	{
		if (_edgeSet == nullptr) return false;

		QMap<double, double> radius = _radiusMap;
		if (!radius.contains(0.0))
			radius.insert(0.0, _basicRadius);
		if (!radius.contains(1.0))
			radius.insert(1.0, _basicRadius);

		QList<double> locs = radius.keys();
		TopoDS_Shape* shape = _edgeSet->getShape();
		BRepFilletAPI_MakeFillet Rake(*shape);
		ChFi3d_FilletShape FSH = ChFi3d_Rational;
		Rake.SetFilletShape(FSH);

		TopTools_IndexedDataMapOfShapeListOfShape aEdgeFaceMap;
		TopExp::MapShapesAndAncestors(*shape, TopAbs_EDGE, TopAbs_FACE, aEdgeFaceMap);
		TopExp_Explorer ex(*shape, TopAbs_EDGE);
		for (int i = 0; i < _edgeIndex; ++i, ex.Next());
		const TopoDS_Shape& edgeshape = ex.Current();
		const TopoDS_Edge& E = TopoDS::Edge(edgeshape);
		TopTools_ListOfShape faces = aEdgeFaceMap.FindFromKey(E);
		if (faces.Size() < 2)  return false;

		const int npt = locs.size();
		TColgp_Array1OfPnt2d parAndRad(1, npt);
		for (int i = 1; i <= npt; ++i)
		{
			double loc = locs.at(i-1);
			double rad = radius.value(loc);
			parAndRad.SetValue(i, gp_Pnt2d(loc, rad));
		}

		Rake.Add(parAndRad, E);
		Rake.Build();
		if (!Rake.IsDone()) return false;
		
		TopoDS_Shape evolvedShape = Rake.Shape();
		TopoDS_Shape* resshape = new TopoDS_Shape;
		*resshape = evolvedShape;

		const int id = Geometry::GeometrySet::getMaxID() + 1;
		QString name = _name.arg(id);
		Geometry::GeometrySet* set = new Geometry::GeometrySet;
		set->setName(name);
		set->setShape(resshape);
		set->appendSubSet(_edgeSet);
		_result = set;

		if (_isEdit)
		{
			set->setName(_editSet->getName());
			_geoData->replaceSet(set, _editSet);
		}
		else
		{
			set->setName(name);
			_geoData->appendGeometrySet(set);

		}

		_geoData->removeTopGeometrySet(_edgeSet);

		Geometry::GeometryParaVariableFillet* para = new Geometry::GeometryParaVariableFillet;
		para->setInputSet(_edgeSet);
		QPair<Geometry::GeometrySet*, int> edgpair{_edgeSet,_edgeIndex};
		para->setEdgePair(edgpair);
/*
		para->setEdgeSet(_edgeSet);
		para->setEdgeIndex(_edgeIndex);*/
		para->setRadiuMap(_radiusMap); 
		para->setBasicRadius(_basicRadius);
		
		set->setParameter(para);
		emit removeDisplayActor(_edgeSet);
		emit showSet(set);
		emit updateGeoTree();
		GeoCommandBase::execute();
		
		return true;
	}

	void CommandCreateVariableFillet::undo()
	{
		if (_isEdit)
		{
			_geoData->replaceSet(_editSet, _result);
			emit removeDisplayActor(_result);
			emit showSet(_editSet);
		}
		else
		{
			_result->removeSubSet(_edgeSet);
			_geoData->removeTopGeometrySet(_result);
			_geoData->appendGeometrySet(_edgeSet);
			emit removeDisplayActor(_result);
			emit showSet(_edgeSet);
		}
		emit updateGeoTree();
	}

	void CommandCreateVariableFillet::redo()
	{
		if (_isEdit)
		{
			_geoData->replaceSet( _result,_editSet);
			emit removeDisplayActor(_editSet);
			emit showSet(_result);
		}
		else
		{
			_result->appendSubSet(_edgeSet);
			_geoData->removeTopGeometrySet(_edgeSet);
			_geoData->appendGeometrySet(_result);
			emit removeDisplayActor(_edgeSet);
			emit showSet(_result);
		}
		emit updateGeoTree();
	}

	void CommandCreateVariableFillet::releaseResult()
	{
		if (_result != nullptr) delete _result;
	}

	void CommandCreateVariableFillet::setShape(Geometry::GeometrySet* set, int index)
	{
		_edgeSet = set;
		_edgeIndex = index;
	}

	void CommandCreateVariableFillet::setBasicRadius(double d)
	{
		_basicRadius = d;
	}

	void CommandCreateVariableFillet::setRaiudMap(QMap<double, double> m)
	{
		_radiusMap = m;
	}


}