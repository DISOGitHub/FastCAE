#include "FluidMeshPreProcess.h"
#include <BRep_Builder.hxx>
#include <TopoDS_Compound.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopExp.hxx>
#include <math.h>

#define  TOLANCEDIV 1000

namespace Gmsh
{
	void Gmsh::FluidMeshPreProcess::setFluidField(QList<double*> c)
	{
		_fluidField = c;
	}

	void FluidMeshPreProcess::mergeFluidField(TopoDS_Compound* c, QMultiHash<int, int> solids)
	{
		_compand = c;
		_solidHash = solids;

		BRep_Builder aBuilder;
		aBuilder.MakeCompound(*_compand);

		//gp_Pnt pt0(-50, -50, -50); gp_Pnt pt1(50, 50, 50);
		gp_Pnt pt0(_fluidField[0][0], _fluidField[0][1], _fluidField[0][2]); gp_Pnt pt1(_fluidField[1][0], _fluidField[1][1], _fluidField[1][2]);
		QList<gp_Pnt> pntlist{};

		TopoDS_Shape bigBox = BRepPrimAPI_MakeBox(pt0, pt1).Shape();
		Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
		QMultiHash<int, int>::iterator it = _solidHash.begin();
		for (; it != _solidHash.end(); it++)
		{
			Geometry::GeometrySet* set = data->getGeometrySetByID(it.key());
			if (set == nullptr) continue;
			TopoDS_Shape* shape = set->getShape(4, it.value());
			if (shape->IsNull()) continue;
			bigBox = BRepAlgoAPI_Cut(bigBox, *shape);

		}
		aBuilder.Add(*_compand, bigBox);
	}

	QList<int> FluidMeshPreProcess::getInerMember(int t)
	{
		QList<int> res;
		Bnd_Box outerBox; outerBox.SetGap(0.0);
		BRepBndLib::Add(*_compand, outerBox);
		gp_Pnt maxPt = outerBox.CornerMax();
		gp_Pnt minpt = outerBox.CornerMin();
		double dis = maxPt.Distance(minpt);

		TopAbs_ShapeEnum  type = TopAbs_FACE;
		if (t == 1) type = TopAbs_EDGE;

		TopTools_IndexedMapOfShape mapper;
		TopExp::MapShapes(*_compand, type, mapper);

		const int ns = mapper.Extent();
		for (int i = 1; i <= ns; ++i)
		{
			const TopoDS_Shape& ashape = mapper.FindKey(i);
			Bnd_Box box; box.SetGap(0.0);
			BRepBndLib::Add(ashape, box);
			if (isIner(&outerBox, &box, dis/TOLANCEDIV))
				res.append(i);
		}
		return res;
	}

	bool FluidMeshPreProcess::isIner(Bnd_Box* outer, Bnd_Box* iner,double tol)
	{
		int s[3] = { 0, 0, 0 };
		gp_Pnt maxpt = outer->CornerMax();
		gp_Pnt minpt = outer->CornerMin();

		gp_Pnt tmaxpt = iner->CornerMax();
		gp_Pnt tminpt = iner->CornerMin();

		double x1 = fabs(maxpt.X() - tmaxpt.X());
		double x2 = fabs(minpt.X() - tminpt.X());
		double tx = x1 < x2 ? x1 : x2;
		if (tx < tol) s[0] = 1;

		double y1 = fabs(maxpt.Y() - tmaxpt.Y());
		double y2 = fabs(minpt.Y() - tminpt.Y());
		double ty = y1 < y2 ? y1 : y2;
		if (ty < tol) s[1] = 1;

		double z1 = fabs(maxpt.Z() - tmaxpt.Z());
		double z2 = fabs(minpt.Z() - tminpt.Z());
		double tz = z1 < z2 ? z1 : z2;
		if (tz < tol) s[2] = 1;

		return (s[0] + s[1] + s[2] < 2);
	}

}




