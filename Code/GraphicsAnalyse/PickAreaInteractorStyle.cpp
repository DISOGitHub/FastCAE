#include "PickAreaInteractorStyle.h"
#include "qmap.h"
#include "vtkPlane.h"
#include "vtkKdTreePointLocator.h"
#include "vtkExtractDataSets.h"
#define VTKISRBP_ORIENT 0
#define VTKISRBP_SELECT 1

void PickAreaInteractorStyle::restoreInteractor()
{
	while (GetInteractor()->GetRenderWindow()->CheckInRenderStatus());
	if (default_mouse_style != NULL)
	{
		GetInteractor()->SetInteractorStyle(default_mouse_style);
	}
}

void PickAreaInteractorStyle::generate_actor()
{
	vtkPlanes* tep_frustum = areaPicker->GetFrustum();//pick_Data->frustum = static_cast<vtkAreaPicker*>(this->GetInteractor()->GetPicker())->GetFrustum();	

	vtkSmartPointer<vtkExtractGeometry> extractGeometry = vtkSmartPointer<vtkExtractGeometry>::New();
	extractGeometry->SetImplicitFunction(tep_frustum);//extractGeometry->SetImplicitFunction(pick_Data.frustum);//
#if VTK_MAJOR_VERSION <= 5
	extractGeometry->SetInput(dataSet);
#else
	extractGeometry->SetInputData(dataSet);
#endif

	extractGeometry->Update();
	//std::cout << "Extracted cells" << extractGeometry->GetOutput()->GetNumberOfCells() <<  std::endl;
	//std::cout << "Extracted points" << extractGeometry->GetOutput()->GetNumberOfPoints() <<  std::endl;
	vtkSmartPointer<vtkDataSetMapper> SelectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
	SelectedMapper->SetInputConnection(extractGeometry->GetOutputPort());
	SelectedMapper->ScalarVisibilityOff();
	if (pick_Data.pick_actor == NULL)
	{
		vtkSmartPointer<vtkProperty> mBackFaceProp = vtkSmartPointer<vtkProperty>::New();
		pick_Data.pick_actor = vtkActor::New();
		pick_Data.pick_actor->SetBackfaceProperty(mBackFaceProp);
		pick_Data.pick_actor->GetProperty()->SetColor(1.0, 1.0, 1.0);
		pick_Data.pick_actor->GetBackfaceProperty()->SetColor(1.0, 1.0, 1.0);
	}
	pick_Data.pick_actor->SetMapper(SelectedMapper);

	if (pick_Data.flag_pickMode == 3)
	{
		//pick_Data.pick_actor->GetProperty()->EdgeVisibilityOn();
		pick_Data.pick_actor->GetProperty()->SetRepresentationToWireframe();
		pick_Data.pick_actor->GetProperty()->SetLineWidth(3);
		pick_Data.pick_actor->GetProperty()->SetEdgeColor(1.0, 1.0, 1.0);
		pick_Data.pick_actor->GetProperty()->SetPointSize(1);
	}
	else if (pick_Data.flag_pickMode == 2)
	{
		pick_Data.pick_actor->GetProperty()->SetRepresentationToPoints();
		pick_Data.pick_actor->GetProperty()->SetPointSize(4);
	}
	else
	{
		restoreInteractor();
		return;
	}
	GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(pick_Data.pick_actor);
	GetInteractor()->GetRenderWindow()->Render();
	QMap<vtkIdType, QString> cell_output_map;
	cell_output_map.clear();
	pick_Data.pick_areaCell->Resize(0);
	pick_Data.pick_areaPoint->Resize(0);
	QString outstr_point, outstr_cell, tepstr, tepstr2;
	vtkIdType num0 = extractGeometry->GetOutput()->GetNumberOfPoints();
	vtkSmartPointer<vtkKdTreePointLocator> kDTree = vtkSmartPointer<vtkKdTreePointLocator>::New();
	kDTree->SetDataSet(extractGeometry->GetOutput());//dataset
	kDTree->BuildLocator();
	for (vtkIdType i = 0; i < num0; i++)
	{
		double coor[3];
		extractGeometry->GetOutput()->GetPoint(i, coor);
		if (pick_Data.flag_pickMode == 2)
		{
			outstr_point += QString("Point Id : %1 (").arg(i);
			outstr_point += tepstr.setNum(coor[0]); outstr_point += ",";
			outstr_point += tepstr.setNum(coor[1]); outstr_point += ",";
			outstr_point += tepstr.setNum(coor[2]); outstr_point += ")\n";
		}
		vtkIdType iD = kDTree->FindClosestPoint(coor);
		pick_Data.pick_areaPoint->InsertNextId(iD);
	}
	outstr_cell = update_selectedCell(pick_Data.pick_areaPoint, cell_output_map);
	if (consoleForm != NULL)
	{
		if (pick_Data.flag_pickMode == 2)
			consoleForm->slot_addInfoToConsole(outstr_point);
		else if (pick_Data.flag_pickMode == 3)
			consoleForm->slot_addInfoToConsole(outstr_cell);
	}
	emit sig_updatePickData(pick_Data);
	//std::cout << "save cells" << pick_Data.pick_areaCell->GetNumberOfIds() << std::endl;
	//std::cout << "save points" <<pick_Data.pick_areaPoint->GetNumberOfIds() << std::endl;
}

void PickAreaInteractorStyle::OnLeftButtonDown()
{
	this->CurrentMode = VTKISRBP_SELECT;
	vtkInteractorStyleRubberBandPick::OnLeftButtonDown();
}

void PickAreaInteractorStyle::OnLeftButtonUp()
{
	vtkInteractorStyleRubberBandPick::OnLeftButtonUp();
	if (areaPicker == NULL)
	{
		//emit sig_restoreInteractor();
		return;
	}		
	vtkActor *tep_actor = areaPicker->GetActor();
	if (tep_actor == NULL)
	{
		//emit sig_restoreInteractor();
		return;
	}
	
	dataSet = tep_actor->GetMapper()->GetInput();
	if (dataSet == NULL)
	{
		//emit sig_restoreInteractor();
		return;
	}
	for (int i = 0; i < cur_mPipelineObjs.count(); i++)
	{
		for (int j = 0; j < cur_mPipelineObjs[i]->mVecActors.count(); j++)
		{
			if (tep_actor == cur_mPipelineObjs[i]->mVecActors[j])
			{
				pick_Data.pick_pipelineObj = cur_mPipelineObjs[i];
				pick_Data.objId = &(cur_mPipelineObjs[i]->mPipeLineObjProp.pipelineObj_base_propData.obj_id);
				generate_actor();
				//emit sig_restoreInteractor();
				return;
			}
		}
	}
	//emit sig_restoreInteractor();
}

void PickAreaInteractorStyle::SetPipelineObjectList(QList<PipelineObject*> mPipelineObjs)
{
	cur_mPipelineObjs.clear();
	cur_mPipelineObjs.append(mPipelineObjs);
}

void PickAreaInteractorStyle::SetPickData(struct pick_pipelineObj_propData tep_pick_Data)
{ 
	//pick_Data = &tep_pick_Data;
	pick_Data.pick_actor = tep_pick_Data.pick_actor;
	pick_Data.pick_id = tep_pick_Data.pick_id;
	pick_Data.flag_pickMode = tep_pick_Data.flag_pickMode;
	pick_Data.pick_pipelineObj = tep_pick_Data.pick_pipelineObj;
	pick_Data.pick_areaCell = tep_pick_Data.pick_areaCell;
	pick_Data.pick_areaPoint = tep_pick_Data.pick_areaPoint;
	pick_Data.objId = tep_pick_Data.pickArea_objId;
}

bool PickAreaInteractorStyle::findIdInIdList(vtkIdType id,vtkIdList* pointId_list)
{
	int num = pointId_list->GetNumberOfIds(); 
	vtkIdType i;
	for (i = 0; i < num; i++)
	{
		if (id == pointId_list->GetId(i))
			break;
	}
	if (i < num)
		return true;
	else
		return false;
}

QString PickAreaInteractorStyle::update_selectedCell(vtkIdList* pointId_list, QMap<vtkIdType, QString> cell_output_map)
{
	QString outstr,tepstr;
	for (int i = 0; i < pointId_list->GetNumberOfIds(); i++)
	{
		vtkIdList *tep_cell_list = vtkIdList::New();
		vtkIdType point_Id = pointId_list->GetId(i);
		dataSet->GetPointCells(point_Id, tep_cell_list);
		vtkIdType tep_num_cells = tep_cell_list->GetNumberOfIds();
		for (vtkIdType j = 0; j < tep_num_cells; j++)
		{
			vtkIdType cur_cell_id = tep_cell_list->GetId(j);
			if (cell_output_map.contains(cur_cell_id))
				continue;
			vtkCell* tep_cell = dataSet->GetCell(cur_cell_id);
			vtkIdList *tep_idList = tep_cell->GetPointIds();
			vtkIdType k;
			for (k = 0; k < tep_idList->GetNumberOfIds(); k++)
			{
				vtkIdType cur_point_id = tep_idList->GetId(k);
				if (!findIdInIdList(cur_point_id, pointId_list))
					break;
			}
			if (k == tep_idList->GetNumberOfIds())
			{
				outstr = QString("Cell Id : %1 (").arg(cur_cell_id);
				for (vtkIdType m = 0; m < tep_idList->GetNumberOfIds(); m++)
				{
					if (m != (tep_idList->GetNumberOfIds() - 1))
					{
						outstr += tepstr.setNum(tep_idList->GetId(m)); outstr += ",";
					}
					else
					{
						outstr += tepstr.setNum(tep_idList->GetId(m)); outstr += ")\n";
					}
				}
				cell_output_map.insert(cur_cell_id, outstr);
			}
		}
		tep_cell_list->Delete();
		tep_cell_list = NULL;
	}
	QMap<vtkIdType, QString>::iterator it;
	QString outstr2;
	for (it = cell_output_map.begin(); it != cell_output_map.end(); it++)
	{
		outstr2 += it.value();
		pick_Data.pick_areaCell->InsertNextId(it.key());
	}
	return outstr2;
}