#include "FilterCalculator.h"

FilterCalculator::FilterCalculator(const QString& dataFile, PipelineObject * parent)
:mDataFile(dataFile)
{
	SetObjParent(parent);
	calMap = NULL;
	selectActor = NULL;
	if (GetObjParent() != NULL)
	{
		mBaseGrid = GetObjParent()->mBaseGrid;
		mPipeLineObjProp = GetObjParent()->mPipeLineObjProp;
		init_subObject_count();
		GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_calculator++;
	}
	
	QString tep_filter_str;
	mPipeLineObjProp.calculatorFilter_propData.filterName = "Calculator" + tep_filter_str.setNum(GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_calculator, 10);
	mPipeLineObjProp.calculatorFilter_propData.formula = "";
	mPipeLineObjProp.calculatorFilter_propData.pre_formula = "";
	mPipeLineObjProp.calculatorFilter_propData.resultName = "result";
	mPipeLineObjProp.pipelineObj_base_propData.fileName = "CalculatorFilter";
	mPipeLineObjProp.pipelineObj_base_propData.file_xh = 0;
	mPipeLineObjProp.pipelineObj_base_propData.fileName = GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.fileName;
	SetObjectType(dCalculator_DataSource);
	GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_total_child++;
	arrCalculator = vtkSmartPointer<vtkArrayCalculator>::New();
	for (int i = 0; i < mPipeLineObjProp.pipelineObj_base_propData.scalarList.count(); i++)
	{
		QString tep_str = mPipeLineObjProp.pipelineObj_base_propData.scalarList.at(i);
		if (tep_str.indexOf("_cell000")< 0)
		{
			arrCalculator->AddScalarArrayName(tep_str.toStdString().data());
		}
	}
	for (int i = 0; i < mPipeLineObjProp.pipelineObj_base_propData.vectorList.count(); i++)
	{
		QString tep_str = mPipeLineObjProp.pipelineObj_base_propData.vectorList.at(i);
		if (tep_str.indexOf("_cell000")< 0)
		{
			arrCalculator->AddVectorArrayName(tep_str.toStdString().data());
		}
	}
	resultArr = vtkSmartPointer<vtkFloatArray>::New();
	
}


FilterCalculator::~FilterCalculator()
{
	if (GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_total_child>0)
		GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_total_child--;
}

QString FilterCalculator::GetName() const
{
	QString tep_str;
	return mPipeLineObjProp.calculatorFilter_propData.filterName;
}

void FilterCalculator::func_generate_actors()
{
	if (mBaseGrid == NULL)
		return;
	if (calMap == NULL)
	{
		calMap = vtkSmartPointer<vtkDataSetMapper>::New();
	}
	setCalculator();
	calMap->SetInputData(vtkDataSet::SafeDownCast(arrCalculator->GetOutput()));
	mPipelineDataSet = vtkDataSet::SafeDownCast(arrCalculator->GetOutput());
	if (selectActor == NULL)
	{
		selectActor = vtkSmartPointer<vtkActor>::New();
		selectActor->SetBackfaceProperty(mBackFaceProp);
	}
	selectActor->SetMapper(calMap);
}

void FilterCalculator::generate_actors()
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
void FilterCalculator::setCalculator()
{
	arrCalculator->SetInputData(mBaseGrid);
	arrCalculator->SetAttributeModeToUsePointData();
	arrCalculator->SetFunction(mPipeLineObjProp.calculatorFilter_propData.formula.toStdString().data());
	arrCalculator->SetResultArrayName(mPipeLineObjProp.calculatorFilter_propData.resultName.toStdString().data());
	arrCalculator->Update();

	vtkDataSet* data = vtkDataSet::SafeDownCast(arrCalculator->GetOutput());
	vtkFloatArray* tep_arr = (vtkFloatArray*)data->GetPointData()->GetArray(mPipeLineObjProp.calculatorFilter_propData.resultName.toStdString().data());
	if (tep_arr != NULL)
	{
		if (tep_arr->GetNumberOfTuples() == mBaseGrid->GetNumberOfPoints())
		{
			mPipeLineObjProp.pipelineObj_base_propData.m_colorList = GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.m_colorList;
			mPipeLineObjProp.pipelineObj_base_propData.m_colorList << tep_arr->GetName();
			if (mPipeLineObjProp.calculatorFilter_propData.formula != mPipeLineObjProp.calculatorFilter_propData.pre_formula)
			{
				func_insertCalculatorResult_lookupTableMap(tep_arr);
				mPipeLineObjProp.calculatorFilter_propData.pre_formula = mPipeLineObjProp.calculatorFilter_propData.formula;
			}
			if (mPipeLineObjProp.pipelineObj_base_propData.m_colorList.indexOf(mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle, 0) < 0)
			{
				mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle = mPipeLineObjProp.calculatorFilter_propData.resultName;
				if (GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.contains("CalculatorResult"))
				{
					GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap["CalculatorResult"].title = mPipeLineObjProp.calculatorFilter_propData.resultName;
				}
			}
		}
	}

}

