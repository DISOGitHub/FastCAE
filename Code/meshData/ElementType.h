#ifndef _ELEMENTTYPE_H_
#define _ELEMENTTYPE_H_

#include <QString>
#include <vtkCellType.h>
#include "meshDataAPI.h"

namespace MeshData
{
	QString MESHDATAAPI vtkCellTYpeToString(VTKCellType type);
	VTKCellType MESHDATAAPI  TypeStringToVTKCellType(QString stype);
	int MESHDATAAPI GetNodeCountByElementType(VTKCellType t);
}


#endif