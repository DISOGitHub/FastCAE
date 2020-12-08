#include "GeoCommon.h"
#include <vtkCellType.h>
#include <vtkDataSet.h>
#include <vtkCell.h>
#include <vtkPoints.h>
#include <TopoDS_Face.hxx>
#include <TopoDS_Vertex.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepExtrema_DistShapeShape.hxx>
#include <BRepClass_FaceClassifier.hxx>
#include <TopExp.hxx>
#include <TopoDS.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>
#include <QDebug>

QList<int> GeoCommon::getD2ElementsInShape(vtkDataSet* d, TopoDS_Shape* sh)
{
	QList<int> ids;

	Bnd_Box box;
	BRepBndLib::Add(*sh, box);
	gp_Pnt max = box.CornerMax();
	gp_Pnt min = box.CornerMin();
	const double tol = max.Distance(min) / 10000.0;

	vtkCell *cell = nullptr;
	const int nc = d->GetNumberOfCells();
	for (int i = 0; i < nc; i++)
	{
		cell = d->GetCell(i);
		int type = cell->GetCellType();
		int dim = VTKCellTypeToDim(type);
		if (dim != 2) continue;

		bool inface = true;
		vtkIdList* points = cell->GetPointIds();
		for (int pi = 0; pi < points->GetNumberOfIds(); pi++)
		{
			int pointid = points->GetId(pi);
			double* c = d->GetPoint(pointid);
			gp_Pnt pt(c[0], c[1], c[2]);
			if (!IsPointOnFace(sh, &pt, tol))
			{
				inface = false;
				break;
			}
		}

		if (inface)
		{
			ids.append(i);
		}
	}
	
	return ids;
}


QList<int> GeoCommon::getNodesInShape(vtkDataSet* d, TopoDS_Shape* sh)
{
	QList<int> ids;

	Bnd_Box box;
	BRepBndLib::Add(*sh, box);
	gp_Pnt max = box.CornerMax();
	gp_Pnt min = box.CornerMin();
	const double tol = max.Distance(min) / 10000.0;


	const int nc = d->GetNumberOfPoints();
	for (int i = 0; i < nc; i++)
	{
		double *coor = d->GetPoint(i);
	
		gp_Pnt pt(coor[0], coor[1], coor[2]);
		if (IsPointOnFace(sh, &pt, tol))
			ids.append(i);

	}
	
	return ids;
}

QList<int> GeoCommon::getBodyElementsInShape(vtkDataSet* d, TopoDS_Shape* sh)
{
	TopAbs_ShapeEnum type = sh->ShapeType();
	QList<int> ids;
	vtkCell *cell = nullptr;
	const int nc = d->GetNumberOfCells();
	for (int i = 0; i < nc; i++)
	{
		cell = d->GetCell(i);
		int type = cell->GetCellType();
		int dim = VTKCellTypeToDim(type);
		if (dim != 3) continue;

		bool inbody = true;
		double center[3]{};
		int a = cell->GetParametricCenter(center); 
		double x[3]{};
		double weight{};
		cell->EvaluateLocation(a, center, x, &weight); 
		gp_Pnt pt(x[0], x[1], x[2]);
		if (!isPointInBody(sh, &pt))
		{
			inbody = false;
			continue;
		}
		if (inbody)
		{
			ids.append(i);
		}
	}
		
	return ids;
}

int GeoCommon::VTKCellTypeToDim(int type)
{
	int dim = -1;
	switch (VTKCellType(type))
	{
	case    VTK_EMPTY_CELL: break;
	case	VTK_VERTEX:
	case	VTK_POLY_VERTEX: dim = 0; break;
	case	VTK_LINE:
	case	VTK_POLY_LINE: dim = 1; break;
	case	VTK_TRIANGLE:
	case	VTK_TRIANGLE_STRIP:
	case	VTK_POLYGON:
	case	VTK_PIXEL:
	case	VTK_QUAD: dim = 2; break;
	case	VTK_TETRA:
	case	VTK_VOXEL:
	case	VTK_HEXAHEDRON:
	case	VTK_WEDGE:
	case	VTK_PYRAMID:
	case	VTK_PENTAGONAL_PRISM:
	case	VTK_HEXAGONAL_PRISM: dim = 3; break;

		// Quadratic, isoparametric cells
	case	VTK_QUADRATIC_EDGE: dim = 1; break;
	case	VTK_QUADRATIC_TRIANGLE:
	case	VTK_QUADRATIC_QUAD:
	case	VTK_QUADRATIC_POLYGON: dim = 2; break;
	case	VTK_QUADRATIC_TETRA:
	case	VTK_QUADRATIC_HEXAHEDRON:
	case	VTK_QUADRATIC_WEDGE:
	case	VTK_QUADRATIC_PYRAMID: dim = 3; break;
	case	VTK_BIQUADRATIC_QUAD: dim = 2; break;
	case	VTK_TRIQUADRATIC_HEXAHEDRON: dim = 3; break;
	case	VTK_QUADRATIC_LINEAR_QUAD: dim = 2; break;
	case	VTK_QUADRATIC_LINEAR_WEDGE:
	case	VTK_BIQUADRATIC_QUADRATIC_WEDGE:
	case	VTK_BIQUADRATIC_QUADRATIC_HEXAHEDRON: dim = 3; break;
	case	VTK_BIQUADRATIC_TRIANGLE: dim = 2; break;

		// Cubic, isoparametric cell
	case	VTK_CUBIC_LINE: dim = 1; break;

		// Special class of cells formed by convex group of points
	case	VTK_CONVEX_POINT_SET: dim = 0; break;

		// Polyhedron cell (consisting of polygonal faces)
	case	VTK_POLYHEDRON: dim = 2; break;

		// Higher order cells in parametric form
	case	VTK_PARAMETRIC_CURVE: dim = 1; break;
	case	VTK_PARAMETRIC_SURFACE:
	case	VTK_PARAMETRIC_TRI_SURFACE:
	case	VTK_PARAMETRIC_QUAD_SURFACE: dim = 2; break;
	case	VTK_PARAMETRIC_TETRA_REGION:
	case	VTK_PARAMETRIC_HEX_REGION: dim = 3; break;

		// Higher order cells
	case	VTK_HIGHER_ORDER_EDGE: dim = 1; break;
	case	VTK_HIGHER_ORDER_TRIANGLE:
	case	VTK_HIGHER_ORDER_QUAD:
	case	VTK_HIGHER_ORDER_POLYGON: dim = 2; break;
	case	VTK_HIGHER_ORDER_TETRAHEDRON:
	case	VTK_HIGHER_ORDER_WEDGE:
	case	VTK_HIGHER_ORDER_PYRAMID:
	case	VTK_HIGHER_ORDER_HEXAHEDRON: dim = 3; break;

	case	VTK_NUMBER_OF_CELL_TYPES: dim = -1; break;
	default:  break;
	}
	return dim;
}

bool GeoCommon::IsPointOnFace(TopoDS_Shape *s, gp_Pnt *aPoint, double aTolerance)
{
	TopoDS_Vertex aVertex = BRepBuilderAPI_MakeVertex(*aPoint);
	BRepExtrema_DistShapeShape anExtrema(*s, aVertex);
	 double dis = anExtrema.Value();
	if ((anExtrema.IsDone() == Standard_True))
	{
		if (dis < aTolerance) return true;
		return false;	
	}
	return false;
};

bool GeoCommon::isPointInBody(TopoDS_Shape *s, gp_Pnt *aPoint)
{
	TopoDS_Vertex aVertex = BRepBuilderAPI_MakeVertex(*aPoint);
	TopExp_Explorer solidexp(*s, TopAbs_SOLID);
	for (; solidexp.More();solidexp.Next())
	{
		TopoDS_Shape solid = solidexp.Current();
		BRepExtrema_DistShapeShape anExtrema(solid, aVertex);
		bool insec = anExtrema.InnerSolution();
		if (insec) return true;
	}
	
	return false;
}

