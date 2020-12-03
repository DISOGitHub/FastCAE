#ifndef GEOCOMMANDCOMMON_H_
#define GEOCOMMANDCOMMON_H_

#include <list>
#include <vector>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>

namespace Geometry
{
	class GeometrySet;
}
namespace Command
{
	class GeoCommandCommon
	{
	public:
		GeoCommandCommon() = default;
		~GeoCommandCommon() = default;

		static std::vector<TopoDS_Wire> bulidWire(std::list<TopoDS_Edge>& edgeList);
		static TopoDS_Shape makeFace(std::vector<TopoDS_Wire>& wires);
		static bool isInside(const TopoDS_Wire&, const TopoDS_Wire&);
		static TopoDS_Face validateFace(const TopoDS_Face&);
		static TopoDS_Shape removeSplitter(const TopoDS_Shape& shape);
		static bool isEmpty(const TopoDS_Shape &shape);
		//从inputShape中移除component，返回移除之前的副本
		static TopoDS_Shape removeShape(TopoDS_Shape* inputShape, TopoDS_Shape* component);
		static gp_Ax2 getEdgeAxis(Geometry::GeometrySet* set ,int edgeindex);
		static gp_Ax2 getFaceAxis(Geometry::GeometrySet* set, int index);

	private:
		static TopoDS_Shape makeFace(std::list<TopoDS_Wire>&);

	};












	// sort bounding boxes according to diagonal length
	class Wire_Compare : public std::binary_function<const TopoDS_Wire&,
		const TopoDS_Wire&, bool>
	{
	public:
		bool operator() (const TopoDS_Wire& w1, const TopoDS_Wire& w2)
		{
			Bnd_Box box1, box2;
			if (!w1.IsNull()) 
			{
				BRepBndLib::Add(w1, box1);
				box1.SetGap(0.0);
			}

			if (!w2.IsNull()) 
			{
				BRepBndLib::Add(w2, box2);
				box2.SetGap(0.0);
			}

			return box1.SquareExtent() < box2.SquareExtent();
		}
	};
}

#endif