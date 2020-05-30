#include "FilterVector.h"
#include "vtkDataObject.h"

FilterVector::FilterVector(const QString& dataFile, PipelineObject * parent)
	:mDataFile(dataFile)
{
	SetObjParent(parent);
	mArrowSource=NULL;
	mConeSource = NULL;
	mMask = NULL;
	glyphFilter = NULL;
	selectActor = NULL;
	glyph_map = NULL;
	surfacefliter = NULL;
	normalGenerator = NULL;
	pointData = NULL;
	mConeSource = vtkSmartPointer<vtkConeSource>::New();
	mArrowSource = vtkSmartPointer<vtkArrowSource>::New();
	/*vtkUnstructuredGrid* tep_grid = vtkUnstructuredGrid::New();
	tep_grid->DeepCopy(GetObjParent()->mBaseGrid);
	mBaseGrid = tep_grid;*/
	mBaseGrid = GetObjParent()->mBaseGrid;
	mPipeLineObjProp = GetObjParent()->mPipeLineObjProp;
	init_subObject_count();
	GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_vector++;
	
	QString tep_filter_str;
	mPipeLineObjProp.vectorFilter_propData.glyphVector_colorXYZ = "X";
	mPipeLineObjProp.vectorFilter_propData.filterName = "Vector" + tep_filter_str.setNum(GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_vector, 10);
	mPipeLineObjProp.vectorFilter_propData.glyph_data.arrow_data.tipRes = 6;
	mPipeLineObjProp.vectorFilter_propData.glyph_data.arrow_data.tipRad = 0.1;
	mPipeLineObjProp.vectorFilter_propData.glyph_data.arrow_data.tipLen = 0.35;
	mPipeLineObjProp.vectorFilter_propData.glyph_data.arrow_data.shaftRes = 6;
	mPipeLineObjProp.vectorFilter_propData.glyph_data.arrow_data.shaftRad = 0.03;
	mPipeLineObjProp.vectorFilter_propData.glyph_data.type = 0;
	mPipeLineObjProp.vectorFilter_propData.scalar_list = GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.scalarList;
	mPipeLineObjProp.vectorFilter_propData.vector_list = GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.vectorList;
	if (GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.scalarList.count()>0)
		mPipeLineObjProp.vectorFilter_propData.scalarColName = GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.scalarList.at(0);
	else
		mPipeLineObjProp.vectorFilter_propData.scalarColName = "None";
	mPipeLineObjProp.vectorFilter_propData.vectorColName = "None";
	mPipeLineObjProp.pipelineObj_base_propData.fileName = "VectorFilter";
	mPipeLineObjProp.pipelineObj_base_propData.file_xh = 0;
	mPipeLineObjProp.vectorFilter_propData.mask = 1;
	//mPipeLineObjProp.vectorFilter_propData.numPoints = 0;
	mPipeLineObjProp.vectorFilter_propData.numPoints = mBaseGrid->GetNumberOfPoints();
	if (mPipeLineObjProp.vectorFilter_propData.numPoints >mBaseGrid->GetNumberOfCells())
		mPipeLineObjProp.vectorFilter_propData.numPoints = mBaseGrid->GetNumberOfCells();
	mPipeLineObjProp.pipelineObj_base_propData.fileName = GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.fileName;
	SetObjectType(dVector_DataSource);
	GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_total_child++;
}

FilterVector::~FilterVector()
{
	if (GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_total_child>0)
		GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_total_child--;
	if (glyphVector_arr != NULL)
	{
		glyphVector_arr->Delete();
		glyphVector_arr = NULL;
	}
	if (mBaseGrid != NULL)
	{
		mBaseGrid->Delete();
		mBaseGrid = NULL;
	}
}

QString FilterVector::GetName() const
{
	QString tep_str;
	return mPipeLineObjProp.vectorFilter_propData.filterName;
}

void FilterVector::ChangeColumnColor()
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
	else if (colName == "GlyphVector")
	{
		selectActor->GetMapper()->ScalarVisibilityOff();
		func_initGlyphVectorArr(mBaseGrid);
		if (glyphVector_arr == NULL)
			return;
		if (glyphVector_arr->GetNumberOfTuples() < 1)
			return;
		mPipelineDataSet->GetPointData()->SetScalars(glyphVector_arr);
		vtkSmartPointer<vtkColorTransferFunction> cur_lookupTable = GetObjParent()->getLookupTable(GetObjParent()->mBaseGridBak, mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle);

		glyph_map->SetInputConnection(glyphFilter->GetOutputPort());
		if (cur_lookupTable != NULL)
		{
			glyph_map->SetLookupTable(cur_lookupTable);
			glyph_map->SetScalarRange(cur_lookupTable->GetRange());
		}
		glyph_map->ScalarVisibilityOn();
		return;
	}
	
	char * columnstr_color;
	QByteArray ba2 = mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle.toLatin1();
	columnstr_color = ba2.data();
	
	vtkSmartPointer<vtkFloatArray>  cur_color_arr = NULL;
	if (mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle.indexOf("_cell000") >= 0)
	{
		int tep_len = (int)strlen(columnstr_color);
		columnstr_color[tep_len - 8] = '\0';
		cur_color_arr = static_cast<vtkFloatArray*>(mBaseGrid->GetCellData()->GetArray(columnstr_color));
	}
	else
	{
		if (mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle != "vtkBlockColors")
		{
			cur_color_arr = static_cast<vtkFloatArray*>(mBaseGrid->GetPointData()->GetArray(columnstr_color));
		}
		//cur_color_arr = static_cast<vtkFloatArray*>(mBaseGrid->GetPointData()->GetArray(columnstr_color));
	}
	mPipelineDataSet->GetPointData()->SetScalars(cur_color_arr);

	vtkSmartPointer<vtkColorTransferFunction> cur_lookupTable = GetObjParent()->getLookupTable(GetObjParent()->mBaseGridBak, mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle);
	
	glyph_map->SetInputConnection(glyphFilter->GetOutputPort());
	if (cur_lookupTable != NULL)
	{
		glyph_map->SetLookupTable(cur_lookupTable);
		glyph_map->SetScalarRange(cur_lookupTable->GetRange());
	}
	glyph_map->ScalarVisibilityOn();
}

void FilterVector::generate_actors()
{
	if (!MakeGlyphs1())
		return;
	if (!flag_exist_actors)
	{
		mVecActors.clear();
		mVecActors.push_back(selectActor);
		
		flag_exist_actors = true;
	}
	ChangeColumnColor();
	UpdatePropertyValueData();
	ShowActor();
}

bool FilterVector::MakeGlyphs1()
{
	if (mBaseGrid == NULL)
		return false;
	char * columnstr_scalar;
	char * columnstr_vector;
	//char * columnstr_color;
	QByteArray ba = mPipeLineObjProp.vectorFilter_propData.scalarColName.toLatin1();
	columnstr_scalar = ba.data();
	QByteArray ba1 = mPipeLineObjProp.vectorFilter_propData.vectorColName.toLatin1();
	columnstr_vector = ba1.data();
	
	int flag_vector_pos = -1;//1 point  0 cell
	int flag_scalar_pos = -1;
	vtkSmartPointer<vtkFloatArray> vector_arr = NULL;
	vtkSmartPointer<vtkFloatArray> scalar_arr = NULL;
	//vtkSmartPointer<vtkFloatArray> cur_color_arr = NULL;
	if (strcmp(columnstr_vector, "None") != 0)
	{
		if (mPipeLineObjProp.vectorFilter_propData.vectorColName.indexOf("_cell000") >= 0)
		{
			int tep_len = (int)strlen(columnstr_vector);
			columnstr_vector[tep_len -8] = '\0';
			vector_arr = static_cast<vtkFloatArray*>(mBaseGrid->GetCellData()->GetArray(columnstr_vector));
			if (vector_arr != NULL)
			{
				flag_vector_pos = 0;
			}
		}
		else
		{
			vector_arr = static_cast<vtkFloatArray*>(mBaseGrid->GetPointData()->GetArray(columnstr_vector));
			if (vector_arr != NULL)
			{
				flag_vector_pos = 1;
			}
		}
	}
	else
	{
		if (strcmp(columnstr_scalar, "None") != 0)
		{
			if (mPipeLineObjProp.vectorFilter_propData.scalarColName.indexOf("_cell000") >= 0)
			{
				int tep_len = (int)strlen(columnstr_scalar);
				columnstr_scalar[tep_len - 8] = '\0';
				scalar_arr = static_cast<vtkFloatArray*>(mBaseGrid->GetCellData()->GetArray(columnstr_scalar));
				if (scalar_arr != NULL)
				{
					flag_scalar_pos = 0;
				}
			}
			else
			{
				scalar_arr = static_cast<vtkFloatArray*>(mBaseGrid->GetPointData()->GetArray(columnstr_scalar));
				if (scalar_arr != NULL)
				{
					flag_scalar_pos = 1;
				}
			}
		}
	}
	if ((flag_vector_pos == -1) && (flag_scalar_pos == -1))
		return false;
	
	vtkSmartPointer<vtkCellCenters> cellCentersFilter = vtkSmartPointer<vtkCellCenters>::New();
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	int real_numPoints;
	if (flag_vector_pos != -1)
	{
		if (flag_vector_pos == 0)
		{
			cellCentersFilter->SetInputData(mBaseGrid);
			cellCentersFilter->VertexCellsOn();
			cellCentersFilter->Update();
			const int cellnum = cellCentersFilter->GetOutput()->GetNumberOfPoints();
			real_numPoints = cellnum;
			for (vtkIdType i = 0; i < cellnum; i++)
			{
				double p[3];
				cellCentersFilter->GetOutput()->GetPoint(i, p);
				points->InsertNextPoint(p);
			}
		}
		else
		{
			int pointnum = mBaseGrid->GetNumberOfPoints();
			real_numPoints = pointnum;
			for (vtkIdType i = 0; i < pointnum; i++)
			{
				double *p;
				p = mBaseGrid->GetPoint(i);
				points->InsertNextPoint(p);
			}
		}
	}
	else if (flag_scalar_pos != -1)
	{
		if (flag_scalar_pos == 0)
		{
			cellCentersFilter->SetInputData(mBaseGrid);
			cellCentersFilter->VertexCellsOn();
			cellCentersFilter->Update();
			const int cellnum = cellCentersFilter->GetOutput()->GetNumberOfPoints();
			real_numPoints = cellnum;
			for (vtkIdType i = 0; i < cellnum; i++)
			{
				double p[3];
				cellCentersFilter->GetOutput()->GetPoint(i, p);
				points->InsertNextPoint(p);
			}
		}
		else
		{
			int pointnum = mBaseGrid->GetNumberOfPoints();
			real_numPoints = pointnum;
			for (vtkIdType i = 0; i < pointnum; i++)
			{
				double *p;
				p = mBaseGrid->GetPoint(i);
				points->InsertNextPoint(p);
			}
		}
	}
	QString tep_str1;
	if (pointData==NULL)
		pointData = vtkSmartPointer<vtkPolyData>::New();
	pointData->SetPoints(points);
	mPipeLineObjProp.vectorFilter_propData.glyph_data.scaleFactor = pointData->GetLength()*0.06;
	if (vector_arr != NULL)
	{
		pointData->GetPointData()->SetNormals(vector_arr);
	}		
	
	if (mMask == NULL)
	{
		mMask = vtkSmartPointer<vtkMaskPoints>::New();		
	}
	mMask->SetInputData(pointData);
	if (!mPipeLineObjProp.vectorFilter_propData.numPoints)
	{
		if (real_numPoints > 20000)
			mPipeLineObjProp.vectorFilter_propData.numPoints = 20000;
		else
			mPipeLineObjProp.vectorFilter_propData.numPoints = real_numPoints;
	}
	mPipeLineObjProp.vectorFilter_propData.mask = real_numPoints / mPipeLineObjProp.vectorFilter_propData.numPoints;
	mMask->SetOnRatio(mPipeLineObjProp.vectorFilter_propData.mask);
	mMask->Update();

	if (mPipeLineObjProp.vectorFilter_propData.glyph_data.type == 1)//kGlyph_Cone
	{
		mConeSource->SetResolution(6);
		mConeSource->SetRadius(0.4);
		mConeSource->Update();
	}
	else if (mPipeLineObjProp.vectorFilter_propData.glyph_data.type == 0)//kGlyph_Arrow
	{
		mArrowSource->SetTipResolution(mPipeLineObjProp.vectorFilter_propData.glyph_data.arrow_data.tipRes);
		mArrowSource->SetTipRadius(mPipeLineObjProp.vectorFilter_propData.glyph_data.arrow_data.tipRad);
		mArrowSource->SetTipLength(mPipeLineObjProp.vectorFilter_propData.glyph_data.arrow_data.tipLen);
		mArrowSource->SetShaftResolution(mPipeLineObjProp.vectorFilter_propData.glyph_data.arrow_data.shaftRes);
		mArrowSource->SetShaftRadius(mPipeLineObjProp.vectorFilter_propData.glyph_data.arrow_data.shaftRad);
		mArrowSource->Update();
	}

	glyphFilter = vtkSmartPointer<vtkGlyph3D>::New();
	if (mPipeLineObjProp.vectorFilter_propData.glyph_data.type == 1)//kGlyph_Cone
	{
		glyphFilter->SetSourceConnection(mConeSource->GetOutputPort());
	}
	else if (mPipeLineObjProp.vectorFilter_propData.glyph_data.type == 0)//kGlyph_Arrow
	{
		glyphFilter->SetSourceConnection(mArrowSource->GetOutputPort());
	}
	glyphFilter->SetScaleModeToDataScalingOff();
	glyphFilter->SetInputData(mMask->GetOutput());
	if (vector_arr!=NULL)
		glyphFilter->SetVectorModeToUseNormal();	

	glyphFilter->SetScaleFactor(mPipeLineObjProp.vectorFilter_propData.glyph_data.scaleFactor); //glyphFilter->SetScaleFactor(346);//0.2
	glyphFilter->ScalingOn();
	glyphFilter->OrientOn();
	glyphFilter->Update();

	//vtkSmartPointer<vtkColorTransferFunction> cur_lookupTable = GetObjParent()->getLookupTable(GetObjParent()->mBaseGridBak, mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle);
	
	if (glyph_map == NULL)
		glyph_map = vtkSmartPointer<vtkPolyDataMapper>::New();
	glyph_map->SetInputConnection(glyphFilter->GetOutputPort());
	mPipelineDataSet = (vtkDataSet*)mMask->GetOutput();//mPipelineDataSet = (vtkDataSet*)glyphFilter->GetOutputPort();
	
	if (selectActor == NULL)
	{
		selectActor = vtkSmartPointer<vtkActor>::New();
		selectActor->SetBackfaceProperty(mBackFaceProp);
	}
	selectActor->SetMapper(glyph_map);
	return true;
}