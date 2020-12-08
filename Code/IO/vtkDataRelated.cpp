#include "vtkDataRelated.h"
#include "meshData/meshCommon.h"
#include <vtkTransform.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkExtractSelection.h>
#include <vtkAppendFilter.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPolyDataNormals.h>
#include <vtkTransformFilter.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkKdTree.h>
#include <vtkDataSetSurfaceFilter.h>

vtkDataRelated::vtkDataRelated() : _transform(vtkSmartPointer<vtkTransform>::New()),
								   _cellIDs(vtkSmartPointer<vtkIdTypeArray>::New())
{
	_transform->PostMultiply();
}

vtkDataRelated::vtkDataRelated(vtkDataSet* data) : _data(data), 
												   _transform(vtkSmartPointer<vtkTransform>::New()),
												   _cellIDs(vtkSmartPointer<vtkIdTypeArray>::New())
{
	_transform->PostMultiply();
}

void vtkDataRelated::SetData(vtkDataSet* data)
{
	_data = data;
}

void vtkDataRelated::SetRotateWXYZ(double angle, double x, double y, double z)
{
	_transform->RotateWXYZ(angle, x, y, z);
}

void vtkDataRelated::SetTransCellIDs(vtkIdTypeArray* cellIDs)
{
	_cellIDs = cellIDs;
}

void vtkDataRelated::Translate(double x, double y, double z)
{
	_transform->Translate(x, y, z);
}

void vtkDataRelated::Scale(double x, double y, double z)
{
	_transform->Scale(x, y, z);
}

vtkDataSet* vtkDataRelated::StartTransform()
{
	vtkSmartPointer<vtkDataObject> tobecopy = nullptr;
	if (_cellIDs->GetNumberOfValues() == 0)
		tobecopy = _data;
	else
	{
		auto selectionNode = vtkSmartPointer<vtkSelectionNode>::New();
		selectionNode->SetFieldType(vtkSelectionNode::CELL);
		selectionNode->SetContentType(vtkSelectionNode::INDICES);
		selectionNode->SetSelectionList(_cellIDs);

		auto selection = vtkSmartPointer<vtkSelection>::New();
		selection->AddNode(selectionNode);

		auto extractionSelection = vtkExtractSelection::New();
		extractionSelection->SetInputData(0, _data);
		extractionSelection->SetInputData(1, selection);
		extractionSelection->Update();

		tobecopy = extractionSelection->GetOutput();
	}
	
	auto transformFilter = vtkSmartPointer<vtkTransformFilter>::New();
	transformFilter->SetInputData(tobecopy);
	transformFilter->SetTransform(_transform);
	transformFilter->Update();

	auto appendFliter = vtkAppendFilter::New();
//	appendFliter->AddInputData(_data);
	appendFliter->AddInputData(transformFilter->GetOutput());
	appendFliter->Update();
	return appendFliter->GetOutput();
}

vtkPolyData* vtkDataRelated::GetVTKDataNormals(vtkPolyData* data, int normalsType)
{
	auto normals = vtkSmartPointer<vtkPolyDataNormals>::New();
	normals->SetInputData(data);
	normals->SetAutoOrientNormals(1);
	normals->SetSplitting(0);
	switch (normalsType)
	{
	case 0:
		normals->ComputePointNormalsOn();
		normals->ComputeCellNormalsOff();
		break;
	case 1:
		normals->ComputePointNormalsOn();
		normals->ComputeCellNormalsOn();
		break;
	}
	normals->Update();
	vtkPolyData* newData = vtkPolyData::New();
	newData->DeepCopy(normals->GetOutput());
	return newData;
}

std::set<vtkIdType> vtkDataRelated::GetCoplanarPointId(vtkDataSet* data, vtkIdType seedId, int minAngle)
{
	vtkSmartPointer<vtkPolyData> polyData = GetSurfaceData(data);
	vtkIdType pSeedId = ConvertToPolyDataId(seedId, data, polyData);
	std::set<vtkIdType> coplanarPointIds;
	if (pSeedId == -1)	return coplanarPointIds;
	coplanarPointIds = GetNeighborPointID(polyData, pSeedId, minAngle);
	return ConvertToDataSetId(data, polyData, coplanarPointIds);
	/*vtkSmartPointer<vtkPolyData> polyData = GetSurfaceData(data);
	vtkIdType pSeedId = ConvertToPolyDataId(seedId, data, polyData);
	std::set<vtkIdType> coplanarPointIds;
	if (pSeedId == -1)	return coplanarPointIds;

	std::set<VTKIDPAIR> seedNeibors = GetNeighborPointID(polyData, pSeedId);
	vtkSmartPointer<vtkDataArray> pointNormals = GetVTKDataNormals(polyData, 0)->GetPointData()->GetNormals();
	if (pointNormals->GetNumberOfTuples() == 0)    return coplanarPointIds;
	coplanarPointIds.insert(pSeedId);

	VTKIDPAIR inverse, self{ pSeedId, pSeedId };
	seedNeibors.erase(self);

	std::set<VTKIDPAIR> newSeedNeibors;
	while (seedNeibors.size() != 0)
	{
		std::set<VTKIDPAIR> temp;
		for (VTKIDPAIR seedNeibor : seedNeibors)
		{
			if (coplanarPointIds.count(seedNeibor.second) == 1)    continue;
			if (GetNormalAngle(pointNormals, seedNeibor) > minAngle)	continue;
			coplanarPointIds.insert(seedNeibor.second);
			newSeedNeibors = GetNeighborPointID(polyData, seedNeibor.second);

			for (VTKIDPAIR newSeedNeibor : newSeedNeibors)
			{
				if (coplanarPointIds.count(newSeedNeibor.second) == 1)	continue;
				temp.insert(newSeedNeibor);
				inverse.first = newSeedNeibor.second;
				inverse.second = newSeedNeibor.first;
				if (temp.count(inverse) == 0)	continue;
				temp.erase(inverse);
			}
		}
		seedNeibors = temp;
	}

	return ConvertToDataSetId(data, polyData, coplanarPointIds);*/
}

std::set<vtkIdType> vtkDataRelated::GetCoplanarCellId(vtkDataSet* data, vtkIdType seedId, int minAngle)
{
	std::set<vtkIdType> coplanarCellIds;
	int nCell = data->GetNumberOfCells();
	for (int i = 0; i < nCell; i++)
	{
		vtkCell* cell = data->GetCell(i);
		if (!cell)	continue;
		int dim = VTKCellTypeToDim(cell->GetCellType());
		if (dim > 2 || dim < 0)    return coplanarCellIds;
	}

	vtkSmartPointer<vtkPolyData> polyData = GetSurfaceData(data);
	std::set<VTKIDPAIR> seedNeibors = GetNeighborCellID(polyData, seedId);
	vtkSmartPointer<vtkDataArray> cellNormals = GetVTKDataNormals(polyData, 1)->GetCellData()->GetNormals();
	if (cellNormals->GetNumberOfTuples() == 0)    return coplanarCellIds;
	coplanarCellIds.insert(seedId);

	VTKIDPAIR inverse, self{ seedId, seedId };
	seedNeibors.erase(self);

	std::set<VTKIDPAIR> newSeedNeibors;
	while (seedNeibors.size() != 0)
	{
		std::set<VTKIDPAIR> temp;
		for (VTKIDPAIR seedNeibor : seedNeibors)
		{
			if (coplanarCellIds.count(seedNeibor.second) == 1)    continue;
			if (GetNormalAngle(cellNormals, seedNeibor) > minAngle)	continue;
			coplanarCellIds.insert(seedNeibor.second);
			newSeedNeibors = GetNeighborCellID(polyData, seedNeibor.second);

			for (VTKIDPAIR newSeedNeibor : newSeedNeibors)
			{
				if (coplanarCellIds.count(newSeedNeibor.second) == 1)	continue;
				temp.insert(newSeedNeibor);
				inverse.first = newSeedNeibor.second;
				inverse.second = newSeedNeibor.first;
				if (temp.count(inverse) == 0)	continue;
				temp.erase(inverse);
			}
		}
		seedNeibors = temp;
	}
	return coplanarCellIds;
}

std::set<VTKIDPAIR> vtkDataRelated::GetNeighborCellID(vtkPolyData* data, int cellId)
{
	std::set<VTKIDPAIR> neighbors;
	auto ptIds = vtkSmartPointer<vtkIdList>::New();
	data->GetCellPoints(cellId, ptIds);
	for (vtkIdType i = 0; i < ptIds->GetNumberOfIds(); i++)
	{
		auto idList = vtkSmartPointer<vtkIdList>::New();
		idList->InsertNextId(ptIds->GetId(i));
		auto neighborCellIds = vtkSmartPointer<vtkIdList>::New();
		data->GetCellNeighbors(cellId, idList, neighborCellIds);
		for (vtkIdType j = 0; j < neighborCellIds->GetNumberOfIds(); j++)
		{
			VTKIDPAIR seedneighbor(cellId, neighborCellIds->GetId(j));
			neighbors.insert(seedneighbor);
		}
	}
	return neighbors;
}

std::set<vtkIdType> vtkDataRelated::GetNeighborPointID(vtkPolyData* data, int ptId, int minAngle)
{
	auto cellIds = vtkSmartPointer<vtkIdList>::New();
	data->GetPointCells(ptId, cellIds);
	std::set<vtkIdType> pointIds;
	if (cellIds->GetNumberOfIds() == 0)
		return pointIds;
	std::set<vtkIdType> nCellIds = GetCoplanarCellId(data, cellIds->GetId(1), minAngle);
	vtkCell* aCell = NULL;
	int nPoint = -1, j = 0;
	for (vtkIdType cellId : nCellIds)
	{
		aCell = data->GetCell(cellId);
		nPoint = aCell->GetNumberOfPoints();
		for (int i = 0; i < nPoint; i++)
			pointIds.insert(aCell->GetPointId(i));
	}
	return pointIds;
	/*std::set<VTKIDPAIR> neighbors;
	auto cellIds = vtkSmartPointer<vtkIdList>::New();
	data->GetPointCells(ptId, cellIds);
	for (vtkIdType i = 0; i < cellIds->GetNumberOfIds(); i++)
	{
		auto ptIds = vtkSmartPointer<vtkIdList>::New();
		data->GetCellPoints(cellIds->GetId(i), ptIds);
		for (vtkIdType j = 0; j < ptIds->GetNumberOfIds(); j++)
		{
			VTKIDPAIR seedneighbor(ptId, ptIds->GetId(j));
			neighbors.insert(seedneighbor);
		}
	}
	return neighbors;*/
}

double vtkDataRelated::GetNormalAngle(vtkDataArray* pointNormals, const VTKIDPAIR& ids)
{
	double firstNormal[3]{}, secondNormal[3]{};
	pointNormals->GetTuple(ids.first, firstNormal);
	pointNormals->GetTuple(ids.second, secondNormal);
	double numerator = firstNormal[0] * secondNormal[0] + firstNormal[1] * secondNormal[1] + firstNormal[2] * secondNormal[2];
	double denominator1 = sqrt((firstNormal[0] * firstNormal[0]) + (firstNormal[1] * firstNormal[1]) + (firstNormal[2] * firstNormal[2]));
	double denominator2 = sqrt((secondNormal[0] * secondNormal[0]) + (secondNormal[1] * secondNormal[1]) + (secondNormal[2] * secondNormal[2]));
	return acos(numerator / (denominator1 * denominator2)) * 180 / 3.14159;
}

vtkPolyData* vtkDataRelated::GetSurfaceData(vtkDataSet* data)
{
	vtkDataSetSurfaceFilter* filter = vtkDataSetSurfaceFilter::New();
	filter->SetInputData(data);
	filter->Update();
	return filter->GetOutput();
}

vtkIdType vtkDataRelated::ConvertToPolyDataId(vtkIdType seedId, vtkDataSet* data, vtkPolyData* polyData)
{
	auto polytree = vtkSmartPointer<vtkKdTree>::New();
	polytree->BuildLocatorFromPoints(polyData->GetPoints());
	double seedXYZ[3]{};
	data->GetPoint(seedId, seedXYZ);
	return polytree->FindPoint(seedXYZ);
}

std::set<vtkIdType> vtkDataRelated::ConvertToDataSetId(vtkDataSet* data, vtkPolyData* polyData, const std::set<vtkIdType>& coplanarPointIds)
{
	double xyz[3]{};
	auto points = vtkSmartPointer<vtkPoints>::New();
	for (int i = 0; i < data->GetNumberOfPoints(); i++)
	{
		data->GetPoint(i, xyz);
		points->InsertNextPoint(xyz);
	}

	std::set<vtkIdType> originalIds;
	if (points->GetNumberOfPoints() < coplanarPointIds.size())
		return originalIds;

	auto pointTree = vtkSmartPointer<vtkKdTree>::New();
	pointTree->BuildLocatorFromPoints(points);
	
	for (vtkIdType coplanarPointId : coplanarPointIds)
	{
		polyData->GetPoint(coplanarPointId, xyz);
		vtkIdType originalId = pointTree->FindPoint(xyz);
		if (originalId == -1)	continue;
		originalIds.insert(originalId);
	}
	return originalIds;
}