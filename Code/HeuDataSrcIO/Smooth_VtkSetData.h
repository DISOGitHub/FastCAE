#ifndef SMOOTH_VTKSETDATA_H
#define SMOOTH_VTKSETDATA_H
#include "vtkDataSet.h"
#include "Smooth_TriangleMesh.h"
#include "vtkPolyData.h"
class Smooth_VtkSetData
{
public:
    Smooth_VtkSetData()
    {

    }
	static void Smooth_SmoothSetData(vtkDataSet* dataSet, vtkPolyData *polyData, int count = 1);
	static void Smooth_triangleMeshToVtkPolyData(Smooth_TriangleMesh *mesh, vtkPolyData *outPolyData);
    static void Smooth_vtkPolyDataToTriangleMesh(vtkDataSet *dataSet, Smooth_TriangleMesh *mesh);
	static void Smooth_quadrangleMeshToVtkDataSet(Smooth_QuadrangleMesh *mesh, vtkPolyData *outPolyData);
    static void Smooth_vtkDataSetToQuadrangleMesh(vtkDataSet *dataSet, Smooth_QuadrangleMesh *mesh);
};

#endif // SMOOTH_VTKSETDATA_H
