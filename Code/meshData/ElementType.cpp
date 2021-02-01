#include "ElementType.h"

namespace MeshData
{
	QString vtkCellTYpeToString(VTKCellType type)
	{
		QString stype = "UNKNOW";
		switch (type)
		{
		case VTK_LINE:
			stype = "LINE2"; break;
		case VTK_TRIANGLE:
			stype = "TRI3"; break;
		case VTK_QUAD:
			stype = "QUAD4"; break;
		case VTK_TETRA:
			stype = "TET4"; break;
		case VTK_HEXAHEDRON:
			stype = "HEX8"; break;
		case VTK_WEDGE:
			stype = "WED6"; break;
		case VTK_PYRAMID:
			stype = "PYR5"; break;
		default:
			break;
		}
		return stype;
	}

	VTKCellType TypeStringToVTKCellType(QString stype)
	{
		VTKCellType cellType = VTK_EMPTY_CELL;

		if (stype == "LINE2") cellType = VTK_LINE;
		else if (stype == "TRI3") cellType = VTK_TRIANGLE;
		else if (stype == "QUAD4") cellType = VTK_QUAD;
		else if (stype == "TET4") cellType = VTK_TETRA;
		else if (stype == "HEX8") cellType = VTK_HEXAHEDRON;
		else if (stype == "WED6") cellType = VTK_WEDGE;
		else if (stype == "PYR5") cellType = VTK_PYRAMID;

		return cellType;
	}

	int GetNodeCountByElementType(VTKCellType t)
	{
		int count = 0;
		switch (t)
		{
		case VTK_LINE:count = 2; break;
		case VTK_TRIANGLE:count = 3; break;
		case VTK_QUAD:count = 4; break;
		case VTK_TETRA:count = 4; break;
		case VTK_HEXAHEDRON:count = 8; break;
		case VTK_WEDGE:count = 6; break;
		case VTK_PYRAMID:count = 5; break;
		default:
			break;
		}

		return count;
	}
}