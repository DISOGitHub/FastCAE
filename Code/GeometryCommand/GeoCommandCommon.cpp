#include "GeoCommandCommon.h"
#include <BRepBuilderAPI_MakeWire.hxx>
#include <ShapeFix_Wire.hxx>
#include <BRepBndLib.hxx>
#include <Bnd_Box.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepCheck_Analyzer.hxx>
#include <ShapeAnalysis.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <ShapeFix_Shape.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <IntTools_FClass2d.hxx>
#include <Geom_Plane.hxx>
#include <ShapeAnalysis_Surface.hxx>
#include <BRep_Tool.hxx>
#include <BRep_Builder.hxx>
#include <vector>
#include <algorithm>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <BRepTools_ReShape.hxx>
#include <BRepBuilderAPI_Copy.hxx>
#include <ModelRefine.h>
#include <QDebug>
#include "geometry/geometrySet.h"
#include <Geom_TrimmedCurve.hxx>
#include <GeomConvert.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <GCPnts_UniformAbscissa.hxx>
#include <CPnts_AbscissaPoint.hxx>
#include <BRepGProp.hxx>

namespace Command
{
	std::vector<TopoDS_Wire> GeoCommandCommon::bulidWire(std::list<TopoDS_Edge>& edgeList)
	{
		std::vector<TopoDS_Wire> wires;

		while (edgeList.size() > 0)
		{
			BRepBuilderAPI_MakeWire mkWire;
			mkWire.Add(edgeList.front());
			edgeList.pop_front();

			TopoDS_Wire new_wire = mkWire.Wire(); // current new wire

			bool found = false;
			do {
				found = false;
				for (std::list<TopoDS_Edge>::iterator pE = edgeList.begin(); pE != edgeList.end(); ++pE) 
				{
					mkWire.Add(*pE);
					if (mkWire.Error() != BRepBuilderAPI_DisconnectedWire) 
					{
						found = true;
						edgeList.erase(pE);
						new_wire = mkWire.Wire();
						break;
					}
				}
			} while (found);

			// Fix any topological issues of the wire
			ShapeFix_Wire aFix;
			aFix.SetPrecision(Precision::Confusion());
			aFix.Load(new_wire);
			aFix.FixReorder();
			aFix.FixConnected();
			aFix.FixClosed();
			wires.push_back(aFix.Wire());
		}

		return wires;
	}

	TopoDS_Shape GeoCommandCommon::makeFace(std::vector<TopoDS_Wire>& w)
	{
		if (w.empty())
			return TopoDS_Shape();

		std::vector<TopoDS_Wire> wires = w;
		std::sort(wires.begin(), wires.end(), Wire_Compare());
		std::list<TopoDS_Wire> wire_list;
		wire_list.insert(wire_list.begin(), wires.rbegin(), wires.rend());

		// separate the wires into several independent faces
		std::list< std::list<TopoDS_Wire> > sep_wire_list;
		while (!wire_list.empty()) 
		{
			std::list<TopoDS_Wire> sep_list;
			TopoDS_Wire wire = wire_list.front();
			wire_list.pop_front();
			sep_list.push_back(wire);

			std::list<TopoDS_Wire>::iterator it = wire_list.begin();
			while (it != wire_list.end()) 
			{
				if (isInside(wire, *it)) 
				{
					sep_list.push_back(*it);
					it = wire_list.erase(it);
				}
				else 
				{
					++it;
				}
			}

			sep_wire_list.push_back(sep_list);
		}

		if (sep_wire_list.size() == 1)
		{
			std::list<TopoDS_Wire>& wires = sep_wire_list.front();
			return makeFace(wires);
		}
		else if (sep_wire_list.size() > 1)
		{
			TopoDS_Compound comp;
			BRep_Builder builder; 
			builder.MakeCompound(comp);
			for (std::list< std::list<TopoDS_Wire> >::iterator it = sep_wire_list.begin(); it != sep_wire_list.end(); ++it) 
			{
				TopoDS_Shape aFace = makeFace(*it);
				if (!aFace.IsNull())
					builder.Add(comp, aFace);
			}
			if (comp.Closed()==false)
			{
				return TopoDS_Shape();//no closed.
			}
			return comp;
		}
		else 
		{
			return TopoDS_Shape(); // error
		}

	}

	TopoDS_Shape GeoCommandCommon::makeFace(std::list<TopoDS_Wire>& wires)
	{
		if (wires.size()<1)
		{
			return TopoDS_Shape();
		}
		BRepBuilderAPI_MakeFace mkFace(wires.front());
		const TopoDS_Face& face = mkFace.Face();
		if (face.IsNull())
			return face;
		gp_Dir axis(0, 0, 1);
		BRepAdaptor_Surface adapt(face);
		if (adapt.GetType() == GeomAbs_Plane) 
		{
			axis = adapt.Plane().Axis().Direction();
		}

		wires.pop_front();
		for (std::list<TopoDS_Wire>::iterator it = wires.begin(); it != wires.end(); ++it)
		{
			BRepBuilderAPI_MakeFace mkInnerFace(*it);
			const TopoDS_Face& inner_face = mkInnerFace.Face();
			if (inner_face.IsNull())
				return inner_face; // failure
			gp_Dir inner_axis(0, 0, 1);
			BRepAdaptor_Surface adapt(inner_face);
			if (adapt.GetType() == GeomAbs_Plane) 
			{
				inner_axis = adapt.Plane().Axis().Direction();
			}
			// It seems that orientation is always 'Forward' and we only have to reverse
			// if the underlying plane have opposite normals.
			if (axis.Dot(inner_axis) < 0)
				it->Reverse();
			mkFace.Add(*it);
		}
		return validateFace(mkFace.Face());
	}

	bool GeoCommandCommon::isInside(const TopoDS_Wire& wire1, const TopoDS_Wire& wire2)
	{
		Bnd_Box box1;
		BRepBndLib::Add(wire1, box1);
		box1.SetGap(0.0);

		Bnd_Box box2;
		BRepBndLib::Add(wire2, box2);
		box2.SetGap(0.0);

		if (box1.IsOut(box2))
			return false;

		double prec = Precision::Confusion();
		prec = 1E-6;

		BRepBuilderAPI_MakeFace mkFace(wire1);
		if (!mkFace.IsDone())
			Standard_Failure::Raise("Failed to create a face from wire in sketch");
		TopoDS_Face face = validateFace(mkFace.Face());
		BRepAdaptor_Surface adapt(face);
		IntTools_FClass2d class2d(face, prec);
		Handle_Geom_Surface surf = new Geom_Plane(adapt.Plane());
		ShapeAnalysis_Surface as(surf);

		TopExp_Explorer xp(wire2, TopAbs_VERTEX);
		while (xp.More()) 
		{
			TopoDS_Vertex v = TopoDS::Vertex(xp.Current());
			gp_Pnt p = BRep_Tool::Pnt(v);
			gp_Pnt2d uv = as.ValueOfUV(p, prec);
			if (class2d.Perform(uv) == TopAbs_IN)
				return true;
			// TODO: We can make a check to see if all points are inside or all outside
			// because otherwise we have some intersections which is not allowed
			else
				return false;
			xp.Next();
		}

		return false;
	}

	TopoDS_Face GeoCommandCommon::validateFace(const TopoDS_Face& face)
	{
		BRepCheck_Analyzer aChecker(face);
		if (!aChecker.IsValid())
		{
			TopoDS_Wire outerwire = ShapeAnalysis::OuterWire(face);
			TopTools_IndexedMapOfShape myMap;
			myMap.Add(outerwire);

			TopExp_Explorer xp(face, TopAbs_WIRE);
			ShapeFix_Wire fix;
			fix.SetFace(face);
			fix.Load(outerwire);
			fix.Perform();
			BRepBuilderAPI_MakeFace mkFace(fix.WireAPIMake());
			while (xp.More())
			{
				if (!myMap.Contains(xp.Current())) 
				{
					fix.Load(TopoDS::Wire(xp.Current()));
					fix.Perform();
					mkFace.Add(fix.WireAPIMake());
				}
				xp.Next();
			}

			aChecker.Init(mkFace.Face());
			if (!aChecker.IsValid())
			{
				ShapeFix_Shape fix(mkFace.Face());
				fix.SetPrecision(Precision::Confusion());
				fix.SetMaxTolerance(Precision::Confusion());
				fix.SetMaxTolerance(Precision::Confusion());
				fix.Perform();
				fix.FixWireTool()->Perform();
				fix.FixFaceTool()->Perform();
				TopoDS_Face fixedFace = TopoDS::Face(fix.Shape());
				aChecker.Init(fixedFace);
				if (!aChecker.IsValid())
					return TopoDS_Face();
					//Standard_Failure::Raise("Failed to validate broken face");
				return fixedFace;
			}
			return mkFace.Face();
		}

		return face;
	}

	TopoDS_Shape GeoCommandCommon::removeSplitter(const TopoDS_Shape& shape)
	{
		if (shape.IsNull())
			Standard_Failure::Raise("Cannot remove splitter from empty shape");

		if (shape.ShapeType() == TopAbs_SOLID) 
		{
			const TopoDS_Solid &solid = TopoDS::Solid(shape);
			BRepBuilderAPI_MakeSolid mkSolid;
			TopExp_Explorer it;
			for (it.Init(solid, TopAbs_SHELL); it.More(); it.Next())
			{
				const TopoDS_Shell &currentShell = TopoDS::Shell(it.Current());
				ModelRefine::FaceUniter uniter(currentShell);
				if (uniter.process()) 
				{
					if (uniter.isModified())
					{
						const TopoDS_Shell &newShell = uniter.getShell();
						mkSolid.Add(newShell);
					}
					else 
					{
						mkSolid.Add(currentShell);
					}
				}
				else 
				{
					Standard_Failure::Raise("Removing splitter failed");
					return shape;
				}
			}
			return mkSolid.Solid();
		}
		else if (shape.ShapeType() == TopAbs_SHELL)
		{
			const TopoDS_Shell& shell = TopoDS::Shell(shape);
			ModelRefine::FaceUniter uniter(shell);
			if (uniter.process())
			{
				return uniter.getShell();
			}
			else
			{
				Standard_Failure::Raise("Removing splitter failed");
			}
		}
		else if (shape.ShapeType() == TopAbs_COMPOUND) 
		{
			BRep_Builder builder;
			TopoDS_Compound comp;
			builder.MakeCompound(comp);

			TopExp_Explorer xp;
			// solids
			for (xp.Init(shape, TopAbs_SOLID); xp.More(); xp.Next()) 
			{
				const TopoDS_Solid &solid = TopoDS::Solid(xp.Current());
				BRepTools_ReShape reshape;
				TopExp_Explorer it;
				for (it.Init(solid, TopAbs_SHELL); it.More(); it.Next()) 
				{
					const TopoDS_Shell &currentShell = TopoDS::Shell(it.Current());
					ModelRefine::FaceUniter uniter(currentShell);
					if (uniter.process())
					{
						if (uniter.isModified())
						{
							const TopoDS_Shell &newShell = uniter.getShell();
							reshape.Replace(currentShell, newShell);
						}
					}
				}
				builder.Add(comp, reshape.Apply(solid));
			}
			// free shells
			for (xp.Init(shape, TopAbs_SHELL, TopAbs_SOLID); xp.More(); xp.Next())
			{
				const TopoDS_Shell& shell = TopoDS::Shell(xp.Current());
				ModelRefine::FaceUniter uniter(shell);
				if (uniter.process()) 
				{
					builder.Add(comp, uniter.getShell());
				}
			}
			// the rest
			for (xp.Init(shape, TopAbs_FACE, TopAbs_SHELL); xp.More(); xp.Next())
			{
				if (!xp.Current().IsNull())
					builder.Add(comp, xp.Current());
			}
			for (xp.Init(shape, TopAbs_WIRE, TopAbs_FACE); xp.More(); xp.Next())
			{
				if (!xp.Current().IsNull())
					builder.Add(comp, xp.Current());
			}
			for (xp.Init(shape, TopAbs_EDGE, TopAbs_WIRE); xp.More(); xp.Next())
			{
				if (!xp.Current().IsNull())
					builder.Add(comp, xp.Current());
			}
			for (xp.Init(shape, TopAbs_VERTEX, TopAbs_EDGE); xp.More(); xp.Next())
			{
				if (!xp.Current().IsNull())
					builder.Add(comp, xp.Current());
			}

			return comp;
		}

		return shape;
	}

	bool GeoCommandCommon::isEmpty(const TopoDS_Shape &shape)
	{
		TopExp_Explorer ptExp(shape, TopAbs_VERTEX);
		for (; ptExp.More(); ptExp.Next()) return false;

		TopExp_Explorer edgeExp(shape, TopAbs_EDGE);
		for (; edgeExp.More(); edgeExp.Next()) return false;

		TopExp_Explorer faceExp(shape, TopAbs_FACE);
		for (; faceExp.More(); faceExp.Next()) return false;
		
		return true;
	}

	TopoDS_Shape GeoCommandCommon::removeShape(TopoDS_Shape* inputShape, TopoDS_Shape* component)
	{

		TopoDS_Shape copyshape = BRepBuilderAPI_Copy(*inputShape);

		Handle(TopoDS_TShape) hand1 = inputShape->TShape();
		Handle(TopoDS_TShape) hand2 = component->TShape();
		if (hand1 == hand2)
		{
			*inputShape = TopoDS_Shape();
			return copyshape;
		}
		BRep_Builder builder;
		builder.Remove(*inputShape, *component);
		return copyshape;
	}


/*
	gp_Ax2 GeoCommandCommon::getEdgeAxis(Geometry::GeometrySet* set, int edgeindex)
	{
		TopoDS_Shape* shape = set->getShape();
		TopExp_Explorer edgeExp(*shape, TopAbs_EDGE);
		for (int index = 0; index < edgeindex && edgeExp.More(); edgeExp.Next(), ++index);

		const TopoDS_Shape& edgeShape = edgeExp.Current();
		if (edgeShape.IsNull()) return;
		const TopoDS_Edge &edgeone = TopoDS::Edge(edgeShape);
		if (edgeone.IsNull()) return;


		Standard_Real first, last;
		Handle(Geom_Curve) &curve = BRep_Tool::Curve(edgeone, first, last);
		Handle(Geom_TrimmedCurve) myTrimmed = new Geom_TrimmedCurve(curve, first, last);
		Handle(Geom_BSplineCurve) NurbsCurve = GeomConvert::CurveToBSplineCurve(myTrimmed);

		GeomAdaptor_Curve GAC(NurbsCurve);
		GCPnts_UniformAbscissa UA(GAC, 200);
		if (!UA.IsDone()) return;
		Standard_Real n = UA.NbPoints();
		for (int i = 0; i < n; ++i)
		{
			Standard_Real parami = UA.Parameter(i + 1);
			gp_Pnt tpnt;
			gp_Vec vpnt;
			curve->D1(parami, tpnt, vpnt);
		}
	}*/
	/*
	gp_Ax2 GeoCommandCommon::getFaceAxis(Geometry::GeometrySet* set, int index)
	{
		return gp_Ax2;
	}*/

}