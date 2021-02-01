#include "meshCommon.h"
#include <vtkCellType.h>


int VTKCellTypeToDim(int type)
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