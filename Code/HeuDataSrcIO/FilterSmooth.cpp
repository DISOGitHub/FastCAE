#include "FilterSmooth.h"
#include "vtkReflectionFilter.h"

FilterSmooth::FilterSmooth(const QString& dataFile, PipelineObject * parent)
:mDataFile(dataFile)
{
	SetObjParent(parent);
	//smoothFilter = NULL;
	selectActor = NULL;
	mBaseGrid = NULL;
	polyData = vtkSmartPointer<vtkPolyData>::New();
	if (GetObjParent() != NULL)
	{
		mBaseGrid = GetObjParent()->mBaseGrid;
		mPipeLineObjProp = GetObjParent()->mPipeLineObjProp;
		init_subObject_count();
		GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_smooth++;
	}
	
	QString tep_filter_str;
	mPipeLineObjProp.smoothFilter_propData.filterName = "Smooth" + tep_filter_str.setNum(GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_smooth, 10);
	mPipeLineObjProp.smoothFilter_propData.smooth_coef = 5;
	mPipeLineObjProp.smoothFilter_propData.flag_cellToPoint = true;
	mPipeLineObjProp.smoothFilter_propData.flag_meshSmooth = false;
	mPipeLineObjProp.pipelineObj_base_propData.fileName = "SmoothFilter";
	mPipeLineObjProp.pipelineObj_base_propData.file_xh = 0;
	mPipeLineObjProp.pipelineObj_base_propData.fileName = GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.fileName;
	SetObjectType(dSmooth_DataSource);
	GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_total_child++;
	smoothFilter = new Smooth_VtkSetData;
}


FilterSmooth::~FilterSmooth()
{
	if (GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_total_child>0)
		GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_total_child--;
}

QString FilterSmooth::GetName() const
{
	QString tep_str;
	return mPipeLineObjProp.smoothFilter_propData.filterName;
}

void FilterSmooth::func_generate_actors()
{
	if (mPipeLineObjProp.smoothFilter_propData.flag_meshSmooth)
	{
		if (smoothFilter == NULL)
		{
			smoothFilter = new Smooth_VtkSetData;
		}
		smoothFilter->Smooth_SmoothSetData(GetObjParent()->mBaseGrid, polyData, mPipeLineObjProp.smoothFilter_propData.smooth_coef);
		mBaseGrid = static_cast<vtkDataSet*> (polyData);
		int num = polyData->GetPointData()->GetNumberOfArrays();
		for (int i = 0; i < num; i++)
		{
			mBaseGrid->GetPointData()->AddArray(polyData->GetPointData()->GetArray(polyData->GetPointData()->GetArrayName(i)));
		}
		num = polyData->GetCellData()->GetNumberOfArrays();
		for (int i = 0; i < num; i++)
		{
			mBaseGrid->GetCellData()->AddArray(polyData->GetCellData()->GetArray(polyData->GetCellData()->GetArrayName(i)));
		}
	}
	else if (mPipeLineObjProp.smoothFilter_propData.flag_cellToPoint)
	{
		if ((GetObjParent()->mBaseGrid->GetCellData() != NULL) && (GetObjParent()->mBaseGrid->GetCellData()->GetNumberOfComponents() > 0))
		{
			if (mBaseGrid != NULL)
				mBaseGrid->RemoveAllObservers();
			vtkCellDataToPointData*	cellToPtData = vtkCellDataToPointData::New();
			cellToPtData->SetInputData(GetObjParent()->mBaseGrid);
			cellToPtData->Update();
			mBaseGrid = cellToPtData->GetOutput();
		}
	}
	else
		mBaseGrid->DeepCopy(GetObjParent()->mBaseGrid);
	if ((mBaseGrid == NULL) || (mBaseGrid->GetPointData() == NULL))
		return;
	m_glphMapper->SetInputData(mBaseGrid);
	mPipelineDataSet = mBaseGrid;
	if (selectActor == NULL)
	{
		selectActor = vtkSmartPointer<vtkActor>::New();
		selectActor->SetBackfaceProperty(mBackFaceProp);
	}
	selectActor->SetMapper(m_glphMapper);
}

void FilterSmooth::generate_actors()
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
void FilterSmooth::ChangeColumnColor()
{
	if (selectActor == NULL)
		return;

	QString colName = mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle;
	if (colName == "SolidColor")
	{
		selectActor->GetMapper()->ScalarVisibilityOff();
		double r, g, b;
		r = double(mPipeLineObjProp.pipelineObj_base_propData.mSolidColor.r) / 255.0f;
		g = double(mPipeLineObjProp.pipelineObj_base_propData.mSolidColor.g) / 255.0f;
		b = double(mPipeLineObjProp.pipelineObj_base_propData.mSolidColor.b) / 255.0f;
		selectActor->GetProperty()->SetColor(r, g, b);
		selectActor->GetBackfaceProperty()->SetColor(r, g, b);
		return;
	}
	if (colName.indexOf("_cell000") >= 0)
	{
		colName = colName.left(colName.length() - 8);
	}
	if (mPipelineDataSet->GetPointData()->HasArray(colName.toStdString().data()))
	{
		vtkFloatArray* tep_arr = (vtkFloatArray*)mPipelineDataSet->GetPointData()->GetArray(colName.toStdString().data());
		if (tep_arr == NULL)
			return;
		if (tep_arr->GetNumberOfComponents() == 3)
		{
			int xb = 0;
			if (mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle_XYZ == "X")
				xb = 0;
			else if (mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle_XYZ == "Y")
				xb = 1;
			else if (mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle_XYZ == "Z")
				xb = 2;
			//mPipelineDataSet->GetPointData()->SetScalars(tep_arrXYZ);
			mPipelineDataSet->GetPointData()->SetActiveScalars(colName.toStdString().data());
			m_glphMapper->ColorByArrayComponent(colName.toStdString().data(), xb);
			m_glphMapper->SetScalarModeToUsePointFieldData();
		}
		else
		{
			mPipelineDataSet->GetPointData()->SetActiveScalars(colName.toStdString().data());
			m_glphMapper->SelectColorArray(colName.toStdString().data());
			m_glphMapper->SetScalarModeToUsePointData();
		}
	}
	vtkSmartPointer<vtkColorTransferFunction> cur_lookupTable = GetObjParent()->getLookupTable(GetObjParent()->mBaseGridBak, mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle);
	//selectActor->GetMapper()->SetInputConnection(glyphFilter->GetOutputPort());
	if (cur_lookupTable != NULL)
	{
		m_glphMapper->SetLookupTable(cur_lookupTable);
		m_glphMapper->SetScalarRange(cur_lookupTable->GetRange());
	}
	m_glphMapper->ScalarVisibilityOn();
	/*if (mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle.indexOf("_cell000") >= 0)
	{
		int tep_len = (int)strlen(columnstr_color);
		columnstr_color[tep_len - 8] = '\0';
		cur_color_arr = static_cast<vtkFloatArray*>(mBaseGrid->GetCellData()->GetArray(columnstr_color));
	}
	else
	{
		cur_color_arr = static_cast<vtkFloatArray*>(mBaseGrid->GetPointData()->GetArray(columnstr_color));
	}
	mPipelineDataSet->GetPointData()->SetScalars(cur_color_arr);

	vtkSmartPointer<vtkColorTransferFunction> cur_lookupTable = GetObjParent()->getLookupTable(GetObjParent()->mBaseGridBak, mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle);

	selectActor->GetMapper()->SetInputConnection(glyphFilter->GetOutputPort());
	if (cur_lookupTable != NULL)
	{
		glyph_map->SetLookupTable(cur_lookupTable);
		glyph_map->SetScalarRange(cur_lookupTable->GetRange());
	}
	glyph_map->ScalarVisibilityOn();*/
}