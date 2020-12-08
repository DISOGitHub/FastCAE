#include "Smooth_VtkSetData.h"
#include "vtkCell.h"
#include "vtkPoints.h"
#include "vtkTecplotReader.h"
#include "vtkSmartPointer.h"
#include "vtkMultiBlockDataSet.h"
#include "vtkAppendFilter.h"
#include "vtkUnstructuredGrid.h"
#include "vtkPointData.h"
#include "vtkCellData.h"
#include "vtkSmoothPolyDataFilter.h"
#include "vtkFieldData.h"
#include "vtkInformation.h"
#include "vtkDataSetAttributes.h"
#include "vtkFloatArray.h"
#include "vtkPolyDataReader.h"
#include "vtkTriangleFilter.h"
#include "vtkDataSetReader.h"
#include "vtkPolyDataWriter.h"
#include "Smooth_QuadrangleMesh.h"
#include <QDebug>
void Smooth_VtkSetData::Smooth_SmoothSetData(vtkDataSet *dataSet, vtkPolyData *polyData, int count)
{
    int edgeCount = 0;
    for(vtkIdType i = 0; i < dataSet->GetNumberOfCells(); i++)
    {
        vtkCell* cell = dataSet->GetCell(i);
        edgeCount = cell->GetNumberOfPoints();
        break;
    }
    if(edgeCount == 4)
    {
        Smooth_QuadrangleMesh mesh;
        Smooth_vtkDataSetToQuadrangleMesh(dataSet, &mesh);
        for(int i = 0; i < count; i++)
        {
            mesh.smoothPoints();
        }
		Smooth_quadrangleMeshToVtkDataSet(&mesh, polyData);

    } 
	else if (edgeCount == 3)
	{
		Smooth_TriangleMesh mesh;
		Smooth_vtkPolyDataToTriangleMesh(dataSet, &mesh);
		for (int i = 0; i < count; i++)
		{
			mesh.smoothPoints();
		}
		Smooth_triangleMeshToVtkPolyData(&mesh, polyData);
	}
	
}

void Smooth_VtkSetData::Smooth_triangleMeshToVtkPolyData(Smooth_TriangleMesh *mesh, vtkPolyData *outPolyData)
{
	vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    std::vector<vtkFloatArray*> pointsArrays, cellArrays;
    if(mesh->getPointCount() > 0)
    {
        Smooth_Point& p = mesh->getPointRef(0);
        for(int i = 0; i < (int)p.arrayDatas.size(); i++)
        {
            vtkFloatArray* pointArray = vtkFloatArray::New();
            Smooth_Point::POINT_ARRAY& array = p.arrayDatas[i];
            pointArray->SetNumberOfComponents(array.arrayData.size());
            pointArray->SetName(array.name.data());
            pointsArrays.push_back(pointArray);
        }
    }

    if(mesh->getFacetCount() > 0)
    {
        Smooth_TriangleFacet& f = mesh->getFacetRef(0);
        for(int i = 0; i < (int)f.arrayDatas.size(); i++)
        {
            vtkFloatArray* cellArray = vtkFloatArray::New();
            Smooth_TriangleFacet::POINT_ARRAY& array = f.arrayDatas[i];
            cellArray->SetNumberOfComponents(array.arrayData.size());
            cellArray->SetName(array.name.data());
            cellArrays.push_back(cellArray);
        }
    }

    auto points = vtkSmartPointer<vtkPoints>::New();
    for(size_t i = 0; i < mesh->getPointCount(); i++)
    {
        Smooth_Point& p = mesh->getPointRef(i);
        points->InsertPoint(i, p.x, p.y, p.z);
        for(int j = 0; j < (int)p.arrayDatas.size(); j++)
        {
            Smooth_Point::POINT_ARRAY& array = p.arrayDatas[j];
            vtkFloatArray* vtkArray = pointsArrays[j];

            double *locationValue = (double *) malloc(array.arrayData.size() * sizeof(double));
            for(int k = 0; k < (int)array.arrayData.size(); k++)
            {
                locationValue[k] = array.arrayData[k];
                vtkArray->InsertNextTuple(locationValue);
            }
        }
    }

    for(size_t i = 0; i < pointsArrays.size(); i++)
    {
        vtkFloatArray* vtkArray = pointsArrays[i];
        polyData->GetPointData()->AddArray(vtkArray);
    }


    auto polys = vtkSmartPointer<vtkCellArray>::New();

    for(size_t i = 0; i < mesh->getFacetCount(); i++)
    {
        Smooth_TriangleFacet& f = mesh->getFacetRef(i);
        polys->InsertNextCell(3);
        polys->InsertCellPoint(f.v1);
        polys->InsertCellPoint(f.v2);
        polys->InsertCellPoint(f.v3);


        for(int j = 0; j < (int)f.arrayDatas.size(); j++)
        {
            Smooth_TriangleFacet::POINT_ARRAY& array = f.arrayDatas[j];
            vtkFloatArray* vtkArray = cellArrays[j];

            double *locationValue = (double *) malloc(array.arrayData.size() * sizeof(double));
            for(int k = 0; k < (int)array.arrayData.size(); k++)
            {
                locationValue[k] = array.arrayData[k];
                vtkArray->InsertNextTuple(locationValue);
            }
        }

    }

    for(size_t i = 0; i < cellArrays.size(); i++)
    {
        vtkFloatArray* vtkArray = cellArrays[i];
        polyData->GetCellData()->AddArray(vtkArray);
    }

    polyData->SetPoints(points);
    polyData->SetPolys(polys);
	outPolyData->DeepCopy(polyData);
}

void Smooth_VtkSetData::Smooth_vtkPolyDataToTriangleMesh(vtkDataSet *dataSet, Smooth_TriangleMesh *mesh)
{
    int polyCount = dataSet->GetNumberOfCells();
    std::vector<Smooth_Point> polyPoints;
    std::vector<Smooth_TriangleFacet> polyFacets;
    std::vector<vtkDataArray*> cellsDataArray;
    for(uint i = 0; i < dataSet->GetCellData()->GetNumberOfArrays(); i++)
    {
        vtkDataArray* dataArray = dataSet->GetCellData()->GetArray(i);
        cellsDataArray.push_back(dataArray);
    }
    for(vtkIdType i = 0; i < polyCount; i++)
    {
        vtkCell* cell = dataSet->GetCell(i);
        Smooth_TriangleFacet facet;
        for(auto j = 0; j < cell->GetNumberOfPoints(); j++)
        {
            vtkIdType number = cell->GetPointId(j);
            facet.v[j] = number;
        }
        for(size_t j = 0; j < cellsDataArray.size(); j++)
        {
            vtkDataArray* dataArray = cellsDataArray[j];
            if(dataArray)
            {
                int count = dataArray->GetNumberOfComponents();
                double* pressure = dataArray->GetTuple(i);
                Smooth_TriangleFacet::POINT_ARRAY array;
                array.name = dataArray->GetName();
                for(int k = 0; k < count; k++)
                {
                    array.arrayData.push_back(pressure[k]);
                }
                facet.arrayDatas.push_back(array);
            }
        }
        polyFacets.push_back(facet);
    }

    std::vector<vtkDataArray*> pointsDataArray;
    for(uint i = 0; i < dataSet->GetPointData()->GetNumberOfArrays(); i++)
    {
        vtkDataArray* dataArray = dataSet->GetPointData()->GetArray(i);
        pointsDataArray.push_back(dataArray);
    }

    for(vtkIdType i = 0; i < dataSet->GetNumberOfPoints(); i++)
    {
        double* pointValue = dataSet->GetPoint(i);
        Smooth_Point qPoint;
        qPoint.x = pointValue[0];
        qPoint.y = pointValue[1];
        qPoint.z = pointValue[2];

        for(size_t j = 0; j < pointsDataArray.size(); j++)
        {
            vtkDataArray* dataArray = pointsDataArray[j];
            if(dataArray)
            {
                int count = dataArray->GetNumberOfComponents();
                double* pressure = dataArray->GetTuple(i);
                Smooth_Point::POINT_ARRAY array;
                array.name = dataArray->GetName();
                for(int k = 0; k < count; k++)
                {
                    array.arrayData.push_back(pressure[k]);
                }
                qPoint.arrayDatas.push_back(array);
            }
        }
        polyPoints.push_back(qPoint);
    }
    mesh->setData(polyPoints, polyFacets);
/*
    // 去除面积0的面
    std::vector<Smooth_TriangleFacet> facets;
    for(size_t i = 0; i < mesh->getFacetCount(); i++)
    {
        Smooth_TriangleFacet& f = mesh->getFacetRef(i);
        if(f.v1 == f.v2 || f.v1 == f.v3 || f.v2 == f.v3)
        {
            continue;
        }
        facets.push_back(f);
    }
    std::vector<Smooth_Point> points = mesh->getpoints();
    mesh->setData(points, facets);
    */
    mesh->resetNormal();
}

void Smooth_VtkSetData::Smooth_quadrangleMeshToVtkDataSet(Smooth_QuadrangleMesh *mesh, vtkPolyData *outPolyData)
{
	vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    std::vector<vtkFloatArray*> pointsArrays, cellArrays;
    if(mesh->getPointCount() > 0)
    {
        Smooth_Point& p = mesh->getPointRef(0);
        for(int i = 0; i < (int)p.arrayDatas.size(); i++)
        {
            vtkFloatArray* pointArray = vtkFloatArray::New();
            Smooth_Point::POINT_ARRAY& array = p.arrayDatas[i];
            pointArray->SetNumberOfComponents(array.arrayData.size());
            pointArray->SetName(array.name.data());
            pointsArrays.push_back(pointArray);
        }
    }

    if(mesh->getFacetCount() > 0)
    {
        Smooth_QuadrangleFacet& f = mesh->getFacetRef(0);
        for(int i = 0; i < (int)f.arrayDatas.size(); i++)
        {
            vtkFloatArray* cellArray = vtkFloatArray::New();
            Smooth_QuadrangleFacet::POINT_ARRAY& array = f.arrayDatas[i];
            cellArray->SetNumberOfComponents(array.arrayData.size());
            cellArray->SetName(array.name.data());
            cellArrays.push_back(cellArray);
        }
    }

    auto points = vtkSmartPointer<vtkPoints>::New();
    for(size_t i = 0; i < mesh->getPointCount(); i++)
    {
        Smooth_Point& p = mesh->getPointRef(i);
        points->InsertPoint(i, p.x, p.y, p.z);
        for(int j = 0; j < (int)p.arrayDatas.size(); j++)
        {
            Smooth_Point::POINT_ARRAY& array = p.arrayDatas[j];
            vtkFloatArray* vtkArray = pointsArrays[j];

            double *locationValue = (double *) malloc(array.arrayData.size() * sizeof(double));
            for(int k = 0; k < (int)array.arrayData.size(); k++)
            {
                locationValue[k] = array.arrayData[k];
                vtkArray->InsertNextTuple(locationValue);
            }
        }
    }

    for(size_t i = 0; i < pointsArrays.size(); i++)
    {
        vtkFloatArray* vtkArray = pointsArrays[i];
        polyData->GetPointData()->AddArray(vtkArray);
    }


    auto polys = vtkSmartPointer<vtkCellArray>::New();

    for(size_t i = 0; i < mesh->getFacetCount(); i++)
    {
        Smooth_QuadrangleFacet& f = mesh->getFacetRef(i);
        polys->InsertNextCell(4);
        polys->InsertCellPoint(f.v1);
        polys->InsertCellPoint(f.v2);
        polys->InsertCellPoint(f.v3);
        polys->InsertCellPoint(f.v4);


        for(int j = 0; j < (int)f.arrayDatas.size(); j++)
        {
            Smooth_QuadrangleFacet::POINT_ARRAY& array = f.arrayDatas[j];
            vtkFloatArray* vtkArray = cellArrays[j];

            double *locationValue = (double *) malloc(array.arrayData.size() * sizeof(double));
            for(int k = 0; k < (int)array.arrayData.size(); k++)
            {
                locationValue[k] = array.arrayData[k];
                vtkArray->InsertNextTuple(locationValue);
            }
        }

    }

    for(size_t i = 0; i < cellArrays.size(); i++)
    {
        vtkFloatArray* vtkArray = cellArrays[i];
        polyData->GetCellData()->AddArray(vtkArray);
    }

    polyData->SetPoints(points);
    polyData->SetPolys(polys);
	outPolyData->DeepCopy(polyData);
}
void Smooth_VtkSetData::Smooth_vtkDataSetToQuadrangleMesh(vtkDataSet *dataSet, Smooth_QuadrangleMesh *mesh)
{
    vtkIdType polyCount = dataSet->GetNumberOfCells();
    std::vector<Smooth_Point> polyPoints;
    std::vector<Smooth_QuadrangleFacet> polyFacets;
    std::vector<vtkDataArray*> cellsDataArray;
    for(uint i = 0; i < dataSet->GetCellData()->GetNumberOfArrays(); i++)
    {
        vtkDataArray* dataArray = dataSet->GetCellData()->GetArray(i);
        cellsDataArray.push_back(dataArray);
    }
    for(vtkIdType i = 0; i < polyCount; i++)
    {
        vtkCell* cell = dataSet->GetCell(i);
        Smooth_QuadrangleFacet facet;
        for(auto j = 0; j < cell->GetNumberOfPoints(); j++)
        {
            vtkIdType number = cell->GetPointId(j);
            facet.v[j] = number;
        }
        for(size_t j = 0; j < cellsDataArray.size(); j++)
        {
            vtkDataArray* dataArray = cellsDataArray[j];
            if(dataArray)
            {
                int count = dataArray->GetNumberOfComponents();
                double* pressure = dataArray->GetTuple(i);
                Smooth_QuadrangleFacet::POINT_ARRAY array;
                array.name = dataArray->GetName();

                for(int k = 0; k < count; k++)
                {
                    array.arrayData.push_back(pressure[k]);
                }
                facet.arrayDatas.push_back(array);
            }
        }
        polyFacets.push_back(facet);
    }

    std::vector<vtkDataArray*> pointsDataArray;
    for(uint i = 0; i < dataSet->GetPointData()->GetNumberOfArrays(); i++)
    {
        vtkDataArray* dataArray = dataSet->GetPointData()->GetArray(i);
        pointsDataArray.push_back(dataArray);
    }

    for(vtkIdType i = 0; i < dataSet->GetNumberOfPoints(); i++)
    {
        double* pointValue = dataSet->GetPoint(i);
        Smooth_Point qPoint;
        qPoint.x = pointValue[0];
        qPoint.y = pointValue[1];
        qPoint.z = pointValue[2];

        for(size_t j = 0; j < pointsDataArray.size(); j++)
        {
            vtkDataArray* dataArray = pointsDataArray[j];
            if(dataArray)
            {
                int count = dataArray->GetNumberOfComponents();
                double* pressure = dataArray->GetTuple(i);
                Smooth_Point::POINT_ARRAY array;
                array.name = dataArray->GetName();
                for(int k = 0; k < count; k++)
                {
                    array.arrayData.push_back(pressure[k]);
                }
                qPoint.arrayDatas.push_back(array);
            }
        }
        polyPoints.push_back(qPoint);
    }
    mesh->setData(polyPoints, polyFacets);
    mesh->resetNormal();

}
