#include "FilterReflection.h"
#include "vtkReflectionFilter.h"

FilterReflection::FilterReflection(const QString& dataFile, PipelineObject * parent)
:mDataFile(dataFile)
{
	SetObjParent(parent);
	reflectionFilter = NULL;
	refMap = NULL;
	selectActor = NULL;
	if (GetObjParent() != NULL)
	{
		mBaseGrid = GetObjParent()->mBaseGrid;
		mPipeLineObjProp = GetObjParent()->mPipeLineObjProp;
		init_subObject_count();
		GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_reflection++;
	}
	
	QString tep_filter_str;
	mPipeLineObjProp.reflectionFilter_propData.filterName = "Reflection" + tep_filter_str.setNum(GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_reflection, 10);
	mPipeLineObjProp.reflectionFilter_propData.reflection_axis = USE_X_MIN;
	mPipeLineObjProp.reflectionFilter_propData.flag_copyInput = false;
	mPipeLineObjProp.pipelineObj_base_propData.fileName = "ReflectionFilter";
	mPipeLineObjProp.pipelineObj_base_propData.file_xh = 0;
	mPipeLineObjProp.pipelineObj_base_propData.fileName = GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.fileName;
	SetObjectType(dReflection_DataSource);
	GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_total_child++;
}


FilterReflection::~FilterReflection()
{
	if (GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_total_child>0)
		GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_total_child--;
}

QString FilterReflection::GetName() const
{
	QString tep_str;
	return mPipeLineObjProp.reflectionFilter_propData.filterName;
}

void FilterReflection::func_generate_actors()
{
	if (mBaseGrid == NULL)
		return;
	if (reflectionFilter == NULL)
	{
		reflectionFilter = vtkSmartPointer<vtkReflectionFilter>::New();
	}
	reflectionFilter->SetInputData(mBaseGrid);
	reflectionFilter->SetPlane(mPipeLineObjProp.reflectionFilter_propData.reflection_axis);
	if (mPipeLineObjProp.reflectionFilter_propData.flag_copyInput)
		reflectionFilter->CopyInputOn();
	else
		reflectionFilter->CopyInputOff();
	reflectionFilter->Update();
	if (refMap == NULL)
	{
		refMap = vtkSmartPointer<vtkDataSetMapper>::New();
	}
	refMap->SetInputConnection(reflectionFilter->GetOutputPort());
	mPipelineDataSet = (vtkDataSet*)reflectionFilter->GetOutput();
	if (selectActor == NULL)
	{
		selectActor = vtkSmartPointer<vtkActor>::New();
		selectActor->SetBackfaceProperty(mBackFaceProp);
	}
	selectActor->SetMapper(refMap);
}

void FilterReflection::generate_actors()
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
