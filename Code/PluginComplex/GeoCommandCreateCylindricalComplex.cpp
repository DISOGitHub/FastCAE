#include "GeoCommandCreateCylindricalComplex.h"
#include "geometry/geometrySet.h"
#include "geometry/geometryData.h"
#include <TopoDS_Shape.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <gp_Ax2.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
namespace Command
{
	GeoCommandCreateCylindricalComplex::GeoCommandCreateCylindricalComplex(GUI::MainWindow* m, MainWidget::PreWindow* p)
		: GeoCommandBase(m,p)
	{
	}


	void GeoCommandCreateCylindricalComplex::setName(QString name)
	{
		_name = name;
	}

	bool GeoCommandCreateCylindricalComplex::execute()
	{
		BRep_Builder aBuilder;
		TopoDS_Compound aRes;
		aBuilder.MakeCompound(aRes);
		
		gp_Ax2 cy1Axis;
		cy1Axis.SetLocation(gp_Pnt(_location[0], _location[1], _location[2]));
		cy1Axis.SetDirection(gp_Dir(0,1,0));
		TopoDS_Shape aTopoCylinder1 = BRepPrimAPI_MakeCylinder(cy1Axis, _cyr1, _cyh1).Shape();
		aBuilder.Add(aRes, aTopoCylinder1);

		gp_Ax2 coneAxis;
		double coneLocation[3]{_location[0], _location[1] + _cyh1, _location[2]};
		//_location[1] = _location[1] + _cyh1;
		coneAxis.SetLocation(gp_Pnt(coneLocation[0], coneLocation[1], coneLocation[2]));
		coneAxis.SetDirection(gp_Dir(0, 1, 0));
		TopoDS_Shape aTopoCone = BRepPrimAPI_MakeCone(coneAxis, _cyr1, _cor, _coh).Shape();
		aBuilder.Add(aRes, aTopoCone);

		gp_Ax2 cy2Axis;
		double cy2Location[3]{coneLocation[0],coneLocation[1]+_coh,coneLocation[2]};
		//_location[1] = _location[1] + _coh;
		cy2Axis.SetLocation(gp_Pnt(cy2Location[0], cy2Location[1], cy2Location[2]));
		cy2Axis.SetDirection(gp_Dir(0, 1, 0));
		TopoDS_Shape aTopoCylinder2 = BRepPrimAPI_MakeCylinder(cy2Axis, _cor, _cyh2).Shape();
		aBuilder.Add(aRes, aTopoCylinder2);


		gp_Ax2 cone2Axis;
		double cone2Location[3]{cy2Location[0],cy2Location[1]+_cyh2,cy2Location[2]};
		//_location[1] = _location[1] + _cyh2;
		cone2Axis.SetLocation(gp_Pnt(cone2Location[0], cone2Location[1], cone2Location[2]));
		cone2Axis.SetDirection(gp_Dir(0, 1, 0));
		TopoDS_Shape aTopoCone2 = BRepPrimAPI_MakeCone(cone2Axis, _cor, _cyr1, _coh).Shape();
		aBuilder.Add(aRes, aTopoCone2);

		gp_Ax2 cy3Axis;
		double cy3Location[3]{cone2Location[0],cone2Location[1]+_coh,cone2Location[2]};
		//_location[1] = _location[1] + _coh;
		cy3Axis.SetLocation(gp_Pnt(cy3Location[0], cy3Location[1], cy3Location[2]));
		cy3Axis.SetDirection(gp_Dir(0, 1, 0));
		TopoDS_Shape aTopoCylinder3 = BRepPrimAPI_MakeCylinder(cy3Axis, _cyr1, _cyh1).Shape();
		aBuilder.Add(aRes, aTopoCylinder3);

	/*	gp_Ax2 cy4Axis;
		_location[1] = _location[1] + _cyh1;
		cy4Axis.SetLocation(gp_Pnt(_location[0], _location[1], _location[2]));
		cy4Axis.SetDirection(gp_Dir(0, 1, 0));
		TopoDS_Shape aTopoCylinder4 = BRepPrimAPI_MakeCylinder(cy4Axis, _cyr1, _cyh1).Shape();
		aBuilder.Add(aRes, aTopoCylinder4);
*/

		TopoDS_Shape* shape = new TopoDS_Shape;
		*shape = aRes;
		if (shape->IsNull())
		{
			delete shape;
			return false;
		}
		Geometry::GeometrySet* set = new Geometry::GeometrySet;
		set->setName(_name);
		set->setShape(shape);
		_geoData->appendGeometrySet(set);
		_result = set;

		GeoCommandBase::execute();
		emit showSet(set);
		emit updateGeoTree();
		return true;
	}

	void GeoCommandCreateCylindricalComplex::undo()
	{
		_geoData->removeTopGeometrySet(_result);

		emit removeDisplayActor(_result);
		emit updateGeoTree();
	}

	void GeoCommandCreateCylindricalComplex::redo()
	{
		_geoData->appendGeometrySet(_result);

		emit showSet(_result);
		emit updateGeoTree();
	}

	void GeoCommandCreateCylindricalComplex::releaseResult()
	{
		if (_result != nullptr) delete _result;
	}


	void GeoCommandCreateCylindricalComplex::setLocation(double *d)
	{
		for (int i = 0; i < 3;++i)
		{
			_location[i] = d[i];
		}
	}

	void GeoCommandCreateCylindricalComplex::setRandH(double cyr1, double cyh1, double cor, double coh, double cyh2)
	{
		_cyr1 = cyr1;
		_cyh1 = cyh1;
		_cor = cor;
		_coh = coh;
		_cyh2 = cyh2;
	}

}