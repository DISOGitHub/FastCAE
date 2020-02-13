#ifndef _GEOCOMMON_H_
#define _GEOCOMMON_H_

#include "geometryAPI.h"
#include <QList>

class vtkDataSet;
class TopoDS_Shape;
//class TopoDS_Face;
class gp_Pnt;

class GEOMETRYAPI GeoCommon
{
public:
	static QList<int> getD2ElementsInShape(vtkDataSet* d, TopoDS_Shape* sh);
	static QList<int> getNodesInShape(vtkDataSet* d, TopoDS_Shape* sh);

private:
	static int VTKCellTypeToDim(int ctype);
	static bool IsPointOnFace(TopoDS_Shape *ashape, gp_Pnt *aPoint, double aTolerance);

};

	


#endif