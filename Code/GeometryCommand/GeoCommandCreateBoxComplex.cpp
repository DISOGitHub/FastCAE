#include "GeoCommandCreateBoxComplex.h"
#include "geometry/geometrySet.h"
#include "geometry/geometryData.h"
#include <TopoDS_Shape.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <gp_Ax2.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
namespace Command
{
	GeoCommandCreateBoxComplex::GeoCommandCreateBoxComplex(GUI::MainWindow* m, MainWidget::PreWindow* p)
		: GeoCommandBase(m,p)
	{
	}


	void GeoCommandCreateBoxComplex::setName(QString name)
	{
		_name = name;
	}

	bool GeoCommandCreateBoxComplex::execute()
	{
		BRep_Builder aBuilder;
		TopoDS_Compound aRes;
		aBuilder.MakeCompound(aRes);
		//Ô²Öù
		gp_Ax2 cy1Axis;
		cy1Axis.SetLocation(gp_Pnt(_location[0], _location[1], _location[2]));
		cy1Axis.SetDirection(gp_Dir(0, 0, 1));
		TopoDS_Shape aTopoCylinder1 = BRepPrimAPI_MakeCylinder(cy1Axis, _cyr1, _cyh1).Shape();
		BRepFilletAPI_MakeFillet MF(aTopoCylinder1);
		int i = 0;
		for (TopExp_Explorer edgeExp(aTopoCylinder1, TopAbs_EDGE); edgeExp.More(); edgeExp.Next())
		{

			if (i == 0 || i == 2)
			{

				TopoDS_Edge anEdge = TopoDS::Edge(edgeExp.Current());
				MF.Add(_fr, anEdge);
			}
			++i;
		}
		
		MF.Build();
		if (!MF.IsDone()) return false;
		aBuilder.Add(aRes, MF.Shape());
		//box1
		double ptx, pty, ptz;
		ptx = _location[0] - _box1_w;
		pty = _location[1] - _cyr1;
		ptz = _location[2];
		gp_Pnt pt(ptx, pty, ptz);
		TopoDS_Shape aTopoBox1 = BRepPrimAPI_MakeBox(pt, _box1_w, 2 * _cyr1, _cyh1).Shape();
		BRepFilletAPI_MakeFillet MFbox1(aTopoBox1);
		ChFi3d_FilletShape FSH1 = ChFi3d_Rational;
		MFbox1.SetFilletShape(FSH1);
		int box1 = 0;
		for (TopExp_Explorer edgeBox1Exp(aTopoBox1, TopAbs_EDGE); edgeBox1Exp.More(); edgeBox1Exp.Next())
		{

			if (box1 == 8 || box1 == 12)
			{

				TopoDS_Edge anEdge = TopoDS::Edge(edgeBox1Exp.Current());
				TColgp_Array1OfPnt2d parAndRad(1, 2);
				parAndRad.SetValue(1, gp_Pnt2d(1.0, _fr));
				parAndRad.SetValue(2, gp_Pnt2d(0.0, _fr));
				MFbox1.Add(parAndRad, anEdge);
			}
			/*if (box1 == 10 || box1 == 14)
			{
				TopoDS_Edge anEdge = TopoDS::Edge(edgeBox1Exp.Current());
				TColgp_Array1OfPnt2d parAndRad(1, 2);
				parAndRad.SetValue(1, gp_Pnt2d(1.0, 0.0));
				parAndRad.SetValue(2, gp_Pnt2d(0.0, _fr));
				MFbox1.Add(parAndRad, anEdge);

			}*/
			++box1;
		}
		MFbox1.Build();
		if (!MFbox1.IsDone()) return false;
		aBuilder.Add(aRes, MFbox1.Shape());

		//box2
		double ptx1, pty1, ptz1;
		ptx1 = ptx - _box2_w;
		pty1 = pty;
		ptz1 = ptz;
		gp_Pnt pt1(ptx1, pty1, ptz1);
		TopoDS_Shape aTopoBox2 = BRepPrimAPI_MakeBox(pt1, _box2_w, 2 * _cyr1, _box2_h).Shape();
		aBuilder.Add(aRes, aTopoBox2);

		//box3¡£
		double ptx2, pty2, ptz2;
		ptx2 = ptx1 - _box1_w;
		pty2 = pty1;
		ptz2 = ptz1;
		gp_Pnt pt2(ptx2, pty2, ptz2);
		TopoDS_Shape aTopoBox3 = BRepPrimAPI_MakeBox(pt2, _box1_w, 2 * _cyr1, _cyh1).Shape();

		BRepFilletAPI_MakeFillet MFbox3(aTopoBox3);
		ChFi3d_FilletShape FSH = ChFi3d_Rational;
		MFbox3.SetFilletShape(FSH);
		int box3 = 0;
		for (TopExp_Explorer edgeBox3Exp(aTopoBox3, TopAbs_EDGE); edgeBox3Exp.More(); edgeBox3Exp.Next())
		{

			if (box3 == 8 || box3 == 12)
			{

				TopoDS_Edge anEdge = TopoDS::Edge(edgeBox3Exp.Current());
				TColgp_Array1OfPnt2d parAndRad(1, 2);
				parAndRad.SetValue(1, gp_Pnt2d(1.0, _fr));
				parAndRad.SetValue(2, gp_Pnt2d(0.0, _fr));
				MFbox3.Add(parAndRad, anEdge);
			}
			if (box3 == 10 || box3 == 14)
			{
				TopoDS_Edge anEdge = TopoDS::Edge(edgeBox3Exp.Current());
				TColgp_Array1OfPnt2d parAndRad(1, 2);
				parAndRad.SetValue(1, gp_Pnt2d(1.0, _fr));
				parAndRad.SetValue(2, gp_Pnt2d(0.0, 0.0));
				MFbox3.Add(parAndRad, anEdge);
				
			}
			++box3;
		}
		MFbox3.Build();
		if (!MFbox3.IsDone()) return false;
		aBuilder.Add(aRes, MFbox3.Shape()); 

		//box4
		double ptx3, pty3, ptz3;
		ptx3 = ptx2 - _box3_w;
		pty3 = pty2;
		ptz3 = ptz2;

		gp_Pnt pt3(ptx3, pty3, ptz3);
		TopoDS_Shape aTopoBox4 = BRepPrimAPI_MakeBox(pt3, _box3_w, 2 * _cyr1, _cyh1).Shape();

		BRepFilletAPI_MakeFillet MF1(aTopoBox4);
		int j = 0;
		for (TopExp_Explorer edgeExp1(aTopoBox4, TopAbs_EDGE); edgeExp1.More(); edgeExp1.Next())
		{

			if (j == 8 || j == 10 || j == 12||j==14)
			{

				TopoDS_Edge anEdge = TopoDS::Edge(edgeExp1.Current());
				MF1.Add(_fr, anEdge);
			}
			++j;
		}

		MF1.Build();
		if (!MF1.IsDone()) return false;
		aBuilder.Add(aRes, MF1.Shape());

		//¾µÏñ²Ù×÷
		gp_Pnt p(ptx3, pty3, ptz3);
		gp_Dir dir(1,0,0);
		gp_Ax2 plane(p, dir);
		gp_Trsf aTrsf;
		aTrsf.SetMirror(plane);
		BRepBuilderAPI_Transform aBRespTrsf(aRes, aTrsf);
		aBRespTrsf.Build();
		if (!aBRespTrsf.IsDone()) return false;
		const TopoDS_Shape& resShape = aBRespTrsf.Shape();
		if (resShape.IsNull()) return false;
		
		aBuilder.Add(aRes, resShape);
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

	void GeoCommandCreateBoxComplex::undo()
	{
		_geoData->removeTopGeometrySet(_result);

		emit removeDisplayActor(_result);
		emit updateGeoTree();
	}

	void GeoCommandCreateBoxComplex::redo()
	{
		_geoData->appendGeometrySet(_result);

		emit showSet(_result);
		emit updateGeoTree();
	}

	void GeoCommandCreateBoxComplex::releaseResult()
	{
		if (_result != nullptr) delete _result;
	}


	void GeoCommandCreateBoxComplex::setLocation(double *d)
	{
		for (int i = 0; i < 3;++i)
		{
			_location[i] = d[i];
		}
	}

	void GeoCommandCreateBoxComplex::setRandH(double cyr1, double cyh1,double fr, double box1_w,double box2_w,double box2_h,double box3_w)
	{
		_cyr1 = cyr1;
		_cyh1 = cyh1;
		_fr = fr;
		_box1_w = box1_w;
		_box2_w = box2_w;
		_box2_h = box2_h;
		_box3_w = box3_w;

	}

}