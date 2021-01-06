#include "FilterContour.h"
#include "vtkReflectionFilter.h"
FilterContour::FilterContour(const QString& dataFile, PipelineObject * parent)
:mDataFile(dataFile)
{
	SetObjParent(parent);
	contourFilter = NULL;
	con_map = NULL;
	selectActor = NULL;
	if (GetObjParent() != NULL)
	{
		mBaseGrid = GetObjParent()->mBaseGrid;
		mPipeLineObjProp = GetObjParent()->mPipeLineObjProp;
		init_subObject_count();
		GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_contour++;
	}
	
	QString tep_filter_str;
	mPipeLineObjProp.contourFilter_propData.filterName = "Contour" + tep_filter_str.setNum(GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_contour, 10);
	mPipeLineObjProp.contourFilter_propData.contour_dataList.clear();
	vtkPointData* pData= mBaseGrid->GetPointData();
	if (pData != NULL)
	{
		int tep_num_col = pData->GetNumberOfArrays();
		float tep_max, tep_min;
		for (int i = 0; i < tep_num_col; i++)
		{
			const char* tep_colName=pData->GetArrayName(i);
			if (tep_colName == NULL)
				continue;
			vtkFloatArray* tep_colArr = (vtkFloatArray*)pData->GetArray(tep_colName);
			if (tep_colArr == NULL)
				continue;
			tep_min=tep_colArr->GetRange()[0];
			tep_max = tep_colArr->GetRange()[1];
			contour_PropData tep_contourData;
			tep_contourData.colName = QString(tep_colName);
			tep_contourData.min = tep_min;
			tep_contourData.max = tep_max;
			tep_contourData.val = 0.5*(tep_max + tep_min);
			mPipeLineObjProp.contourFilter_propData.contour_dataList.append(tep_contourData);
		}
	}
	/*mPipeLineObjProp.contourFilter_propData.min = mBaseGrid->GetScalarRange()[0];
	mPipeLineObjProp.contourFilter_propData.max = mBaseGrid->GetScalarRange()[1];
	mPipeLineObjProp.contourFilter_propData.val = 0.5*(mPipeLineObjProp.contourFilter_propData.min + mPipeLineObjProp.contourFilter_propData.max);
	
	mPipeLineObjProp.contourFilter_propData.col_list = GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.m_colorList;
	if (mPipeLineObjProp.contourFilter_propData.col_list.count() > 0)
		mPipeLineObjProp.contourFilter_propData.contour_colName = mPipeLineObjProp.contourFilter_propData.col_list.at(0);
	else
		mPipeLineObjProp.contourFilter_propData.contour_colName = "";*/
	mPipeLineObjProp.pipelineObj_base_propData.fileName = "ContourFilter";
	mPipeLineObjProp.pipelineObj_base_propData.file_xh = 0;
	mPipeLineObjProp.pipelineObj_base_propData.fileName = GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.fileName;
	SetObjectType(dContour_DataSource);
	GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_total_child++;
}


FilterContour::~FilterContour()
{
	if (GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_total_child > 0)
		GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_total_child;
}

QString FilterContour::GetName() const
{
	QString tep_str;
	return mPipeLineObjProp.contourFilter_propData.filterName;
}

void FilterContour::func_generate_actors()
{
	if (mBaseGrid == NULL)
		return;
	vtkSmartPointer<vtkCellDataToPointData> cellToPtData;
	if (mBaseGrid->GetPointData() == NULL || mBaseGrid->GetPointData()->GetScalars() == NULL)
	{
		if (mBaseGrid->GetCellData() != NULL &&
			mBaseGrid->GetCellData()->GetNumberOfComponents() == 1)
		{
			cellToPtData = vtkSmartPointer<vtkCellDataToPointData>::New();
			cellToPtData->SetInputData(mBaseGrid);
			cellToPtData->Update();
			mBaseGrid = cellToPtData->GetOutput();		
		}
	}
	QString tep_str;
	tep_str = mPipeLineObjProp.contourFilter_propData.contour_colName;
	if (mPipeLineObjProp.contourFilter_propData.contour_colName.indexOf("_cell000") >= 0)
	{
		tep_str = mPipeLineObjProp.contourFilter_propData.contour_colName.left(mPipeLineObjProp.contourFilter_propData.contour_colName.length() - 8);
		if (mBaseGrid->GetCellData()->HasArray(tep_str.toStdString().data()))
		{
			mBaseGrid->GetCellData()->SetActiveScalars(tep_str.toStdString().data());
		}

	}
	else
	{
		if (mBaseGrid->GetPointData()->HasArray(mPipeLineObjProp.contourFilter_propData.contour_colName.toStdString().data()))
		{
			mBaseGrid->GetPointData()->SetActiveScalars(mPipeLineObjProp.contourFilter_propData.contour_colName.toStdString().data());
		}
	}

	if (contourFilter == NULL)
	{
		contourFilter = vtkSmartPointer<vtkContourFilter>::New();
	}
	contourFilter->SetInputData(mBaseGrid);
	contourFilter->SetNumberOfContours(1);
	for (int i = 0; i < mPipeLineObjProp.contourFilter_propData.contour_dataList.count(); i++)
	{
		if (mPipeLineObjProp.contourFilter_propData.contour_dataList.at(i).colName == mPipeLineObjProp.contourFilter_propData.contour_colName)
		{
			contourFilter->SetValue(0, mPipeLineObjProp.contourFilter_propData.contour_dataList.at(i).val);
			contourFilter->Update();
			
			vtkDataSet* tep_dataSet1 = NULL;
			//vtkDataSet* tep_dataSet2 = NULL;
			tep_dataSet1 = contourFilter->GetOutput();
			if (m_reflectionPara.flag_refleciton && (tep_dataSet1 != NULL))
			{
				vtkReflectionFilter* reflectionFilter = vtkReflectionFilter::New();
				reflectionFilter->SetInputData(tep_dataSet1);
				reflectionFilter->SetPlane(m_reflectionPara.type);
				reflectionFilter->CopyInputOn();
				reflectionFilter->Update();
				mPipelineDataSet=(vtkDataSet*)reflectionFilter->GetOutput();
			}
			else
			{
				mPipelineDataSet = tep_dataSet1;
			}

			if (con_map == NULL)
			{
				con_map = vtkSmartPointer<vtkDataSetMapper>::New();
			}
			con_map->SetInputData(mPipelineDataSet);
			
			//func_update_xyzColorColumn();
			if (selectActor == NULL)
			{
				selectActor = vtkSmartPointer<vtkActor>::New();
				selectActor->SetBackfaceProperty(mBackFaceProp);
			}
			selectActor->SetMapper(con_map);
			break;
		}
	}
}

void FilterContour::generate_actors()
{
	func_generate_actors();
	if (!flag_exist_actors)
	{
		mVecActors.clear();
		if (selectActor != NULL)
		{
			mVecActors.push_back(selectActor);
		}
		
		flag_exist_actors = true;
	}
	ChangeColumnColor();
	UpdatePropertyValueData();
	ShowActor();
	
}
