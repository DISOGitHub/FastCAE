#include "FilterStreamLine.h"
#include "vtkDataObject.h"
#include "vtkRungeKutta2.h"
#include "vtkRungeKutta4.h"
#include "vtkRungeKutta45.h"
#include "vtkPlaneSource.h"
#include "vtkTubeFilter.h"
#include "vtkReflectionFilter.h"
#include "qdebug.h"
FilterStreamLine::FilterStreamLine(const QString& dataFile, PipelineObject * parent)
	:mDataFile(dataFile)
{
	SetObjParent(parent);
	active_colName = "";
	stream_grid = NULL;
	streamTracer = NULL;
	selectActor = NULL;
	glyph_map = NULL;
	pointData = NULL;
	mBaseGrid = GetObjParent()->mBaseGrid;
	mPipeLineObjProp = GetObjParent()->mPipeLineObjProp;
	init_subObject_count();
	GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_streamLine++;
	QString tep_filter_str;
	mPipeLineObjProp.streamLineFilter_propData.filterName = "StreamLine" + tep_filter_str.setNum(GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_streamLine, 10);
	mPipeLineObjProp.streamLineFilter_propData.integration_data.direction = IntegrationDirection::BOTH;
	mPipeLineObjProp.streamLineFilter_propData.integration_data.type = IntegratorType::Runge_Kutta_4;
	mPipeLineObjProp.streamLineFilter_propData.integration_data.stepUnit = IntegrationStepUnit::Cell_Length;
	mPipeLineObjProp.streamLineFilter_propData.integration_data.initStepLen = 0.2;
	mPipeLineObjProp.streamLineFilter_propData.integration_data.miniStepLen = 0.01;
	mPipeLineObjProp.streamLineFilter_propData.integration_data.maxiStepLen = 0.5;
	mPipeLineObjProp.streamLineFilter_propData.stream_data.maxiSteps = 2000;
	mPipeLineObjProp.streamLineFilter_propData.stream_data.maxiStreamLen = mBaseGrid->GetBounds()[1] - mBaseGrid->GetBounds()[0];
	mPipeLineObjProp.streamLineFilter_propData.stream_data.old_maxiStreamLen = mPipeLineObjProp.streamLineFilter_propData.stream_data.maxiStreamLen;
	mPipeLineObjProp.streamLineFilter_propData.stream_data.terminalSpeed= 1e-12;
	mPipeLineObjProp.streamLineFilter_propData.stream_data.maxiError = 1e-6;
	mBaseGrid->GetCenter(center_point);
	mPipeLineObjProp.streamLineFilter_propData.seeds_data.mPoint.x = center_point[0];
	mPipeLineObjProp.streamLineFilter_propData.seeds_data.mPoint.y = center_point[1];
	mPipeLineObjProp.streamLineFilter_propData.seeds_data.mPoint.z = center_point[2];
	mPipeLineObjProp.streamLineFilter_propData.seeds_data.type = SpeedType::Point_Source;
	mPipeLineObjProp.streamLineFilter_propData.seeds_data.num_points = 100; 
	mPipeLineObjProp.streamLineFilter_propData.seeds_data.radius = 0.5*(mBaseGrid->GetBounds()[5] - mBaseGrid->GetBounds()[4]);
	mPipeLineObjProp.streamLineFilter_propData.vorticity = true;
	mPipeLineObjProp.streamLineFilter_propData.surface_streamLines = false;
	mPipeLineObjProp.streamLineFilter_propData.interpolator_type = InterpolatorType::PointLocator;
	mPipeLineObjProp.streamLineFilter_propData.vector_list = GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.vectorList;
	if (mPipeLineObjProp.streamLineFilter_propData.vector_list.count()>0)
		mPipeLineObjProp.streamLineFilter_propData.vectorColName = mPipeLineObjProp.streamLineFilter_propData.vector_list.at(0);
	mPipeLineObjProp.pipelineObj_base_propData.fileName = "StreamFilter";
	mPipeLineObjProp.pipelineObj_base_propData.file_xh = 0;
	mPipeLineObjProp.pipelineObj_base_propData.fileName = GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.fileName;
	SetObjectType(dStreamLine_DataSource);
	GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_total_child++;
	stream_grid = mBaseGrid;
}
void FilterStreamLine::initReflectionPara()
{
	if (m_reflectionPara.flag_refleciton)
	{
		vtkReflectionFilter* reflectionFilter = vtkReflectionFilter::New();
		reflectionFilter->SetInputData(mBaseGrid);
		reflectionFilter->SetPlane(m_reflectionPara.type);
		reflectionFilter->CopyInputOn();
		reflectionFilter->Update();
		stream_grid = (vtkDataSet*)reflectionFilter->GetOutput();
	}
	else
	{
		stream_grid = mBaseGrid;
	}
	stream_grid->GetCenter(center_point);
	mPipeLineObjProp.streamLineFilter_propData.seeds_data.mPoint.x = center_point[0];
	mPipeLineObjProp.streamLineFilter_propData.seeds_data.mPoint.y = center_point[1];
	mPipeLineObjProp.streamLineFilter_propData.seeds_data.mPoint.z = center_point[2];
	mPipeLineObjProp.streamLineFilter_propData.seeds_data.radius = 0.5*(stream_grid->GetBounds()[5] - stream_grid->GetBounds()[4]);
}
FilterStreamLine::~FilterStreamLine()
{
	if (GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_total_child>0)
		GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_total_child--;
}

QString FilterStreamLine::GetName() const
{
	QString tep_str;
	return mPipeLineObjProp.streamLineFilter_propData.filterName;
}

void FilterStreamLine::ChangeColumnColor()
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
	
	char * columnstr_color;
	QByteArray ba2 = mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle.toLatin1();
	columnstr_color = ba2.data();
	
	vtkSmartPointer<vtkFloatArray>  cur_color_arr = NULL;
	if (mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle.indexOf("_cell000") >= 0)
	{
		int tep_len = (int)strlen(columnstr_color);
		columnstr_color[tep_len - 8] = '\0';
		cur_color_arr = static_cast<vtkFloatArray*>(mPipelineDataSet->GetCellData()->GetArray(columnstr_color));
		//add for vector//////////////////////////
		if((cur_color_arr !=NULL)&& (cur_color_arr->GetNumberOfComponents() == 3))
		{
			int xb=0;
			if (mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle_XYZ == "X")
				xb = 0;
			else if (mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle_XYZ == "Y")
				xb = 1;
			else if (mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle_XYZ == "Z")
				xb = 2;
			
			selectActor->GetMapper()->GetInput()->GetCellData()->SetActiveScalars(columnstr_color);
			selectActor->GetMapper()->ColorByArrayComponent(columnstr_color, xb);
			selectActor->GetMapper()->SetScalarModeToUseCellFieldData();
		}
		else
			//add for vector//////////////////////////
		{
			mPipelineDataSet->GetCellData()->SetActiveScalars(columnstr_color);
			selectActor->GetMapper()->SelectColorArray(columnstr_color);
			selectActor->GetMapper()->SetScalarModeToUseCellData();
		}
	}
	else
	{
		cur_color_arr = static_cast<vtkFloatArray*>(mPipelineDataSet->GetPointData()->GetArray(columnstr_color));
		//add for vector//////////////////////////
		if ((cur_color_arr != NULL) && (cur_color_arr->GetNumberOfComponents() == 3))
		{
			int xb = 0;
			if (mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle_XYZ == "X")
				xb = 0;
			else if (mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle_XYZ == "Y")
				xb = 1;
			else if (mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle_XYZ == "Z")
				xb = 2;
			
			selectActor->GetMapper()->GetInput()->GetPointData()->SetActiveScalars(columnstr_color);
			selectActor->GetMapper()->ColorByArrayComponent(columnstr_color, xb);
			selectActor->GetMapper()->SetScalarModeToUsePointFieldData();
		}
		else
			//add for vector//////////////////////////
		{
			//mPipelineDataSet->GetPointData()->SetActiveScalars(columnstr_color);
			selectActor->GetMapper()->GetInput()->GetPointData()->SetActiveScalars(columnstr_color);
			selectActor->GetMapper()->SelectColorArray(columnstr_color);
			selectActor->GetMapper()->SetScalarModeToUsePointData();
		}
	}
	//streamTracer->SetInputData(stream_grid);
	//streamTracer->Update();
	vtkSmartPointer<vtkColorTransferFunction> cur_lookupTable = GetObjParent()->getLookupTable(GetObjParent()->mBaseGridBak, mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle);
	//glyph_map->SetInputConnection(streamTracer->GetOutputPort());
	if (cur_lookupTable != NULL)
	{
		glyph_map->SetLookupTable(cur_lookupTable);
		glyph_map->SetScalarRange(cur_lookupTable->GetRange());
	}
	glyph_map->ScalarVisibilityOn();
}

void FilterStreamLine::generate_actors()
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

bool FilterStreamLine::MakeGlyphs1()
{
	if (mBaseGrid == NULL)
		return false;
	if (stream_grid == NULL)
	{
		return false;
	}
	if (mPipeLineObjProp.streamLineFilter_propData.vectorColName == NULL)
		return false;
	char * columnstr_vector;
	QByteArray ba1 = mPipeLineObjProp.streamLineFilter_propData.vectorColName.toLatin1();
	columnstr_vector = ba1.data();
	int flag_vector_pos = -1;//1 point  0 cell
	vtkSmartPointer<vtkFloatArray> vector_arr = NULL;
	if (strcmp(columnstr_vector, "None") != 0)
	{
		/*if (get_GridType() == dSTRUCTURED_GRID)
		{
		vtkStructuredGrid* tep_grid0 = vtkStructuredGrid::New();
		stream_grid = tep_grid0;
		stream_grid->DeepCopy(mBaseGrid);
		}
		else if (get_GridType() == dRECTILINEAR_GRID)
		{
		vtkRectilinearGrid* tep_grid0 = vtkRectilinearGrid::New();
		stream_grid = tep_grid0;
		stream_grid->DeepCopy(mBaseGrid);
		}
		else if (get_GridType() == dPolyData)
		{
		vtkPolyData* tep_grid0 = vtkPolyData::New();
		stream_grid = tep_grid0;
		stream_grid->DeepCopy(mBaseGrid);
		}
		else //if (get_GridType() == dUNSTRUCTURED_GRID)
		{
		vtkUnstructuredGrid* tep_grid0 = vtkUnstructuredGrid::New();
		stream_grid = tep_grid0;
		stream_grid->DeepCopy(mBaseGrid);
		}*/
		/*if (m_reflectionPara.flag_refleciton)
		{
			vtkReflectionFilter* reflectionFilter = vtkReflectionFilter::New();
			reflectionFilter->SetInputData(mBaseGrid);
			reflectionFilter->SetPlane(m_reflectionPara.type);
			reflectionFilter->CopyInputOn();
			reflectionFilter->Update();
			stream_grid = (vtkPolyData*)reflectionFilter->GetOutput();
		}
		else
		{
			stream_grid = mBaseGrid;
		}*/
		/*vtkUnstructuredGrid* tep_grid0 = vtkUnstructuredGrid::New();
		stream_grid = tep_grid0;
		stream_grid->DeepCopy(mBaseGrid);*/
		if (mPipeLineObjProp.streamLineFilter_propData.vectorColName.indexOf("_cell000") >= 0)
		{
			int tep_len = (int)strlen(columnstr_vector);
			columnstr_vector[tep_len - 8] = '\0';
			vector_arr = static_cast<vtkFloatArray*>(stream_grid->GetCellData()->GetArray(columnstr_vector));
			if (vector_arr != NULL)
			{
				flag_vector_pos = 0;
				stream_grid->GetCellData()->SetActiveVectors(columnstr_vector);
			}
		}
		else
		{
			vector_arr = static_cast<vtkFloatArray*>(stream_grid->GetPointData()->GetArray(columnstr_vector));
			if (vector_arr != NULL)
			{
				flag_vector_pos = 1;
				stream_grid->GetPointData()->SetActiveVectors(columnstr_vector);
			}
		}
		if (flag_vector_pos == -1)
			return false;
		active_colName = mPipeLineObjProp.streamLineFilter_propData.vectorColName;
	}
	else
		return false;
	/*vtkSmartPointer<vtkPlaneSource> seeds =	vtkSmartPointer<vtkPlaneSource>::New();//vtkLineSource
	seeds->SetXResolution(4);
	seeds->SetYResolution(4);
	seeds->SetOrigin(2, -2, 26);
	seeds->SetPoint1(2, 2, 26);
	seeds->SetPoint2(2, -2, 32);*/

	if (mPipeLineObjProp.streamLineFilter_propData.seeds_data.type == SpeedType::Point_Source)
	{
		if (seeds == NULL)
			seeds = vtkSmartPointer<vtkPointSource>::New();
		seeds->SetRadius(mPipeLineObjProp.streamLineFilter_propData.seeds_data.radius);
		seeds->SetCenter(mPipeLineObjProp.streamLineFilter_propData.seeds_data.mPoint.x, mPipeLineObjProp.streamLineFilter_propData.seeds_data.mPoint.y, mPipeLineObjProp.streamLineFilter_propData.seeds_data.mPoint.z);
		seeds->SetNumberOfPoints(mPipeLineObjProp.streamLineFilter_propData.seeds_data.num_points);
	}

	streamTracer = vtkSmartPointer<vtkStreamTracer>::New();
	streamTracer->SetInputData(stream_grid);
	streamTracer->SetSourceConnection(seeds->GetOutputPort());
	if (mPipeLineObjProp.streamLineFilter_propData.interpolator_type == InterpolatorType::PointLocator)
		streamTracer->SetInterpolatorTypeToDataSetPointLocator();
	else
		streamTracer->SetInterpolatorTypeToCellLocator();
	streamTracer->SetSurfaceStreamlines(mPipeLineObjProp.streamLineFilter_propData.surface_streamLines);
	if (mPipeLineObjProp.streamLineFilter_propData.integration_data.direction == IntegrationDirection::FORWARD)
	{
		streamTracer->SetIntegrationDirectionToForward();//积分方向
	}
	else if (mPipeLineObjProp.streamLineFilter_propData.integration_data.direction == IntegrationDirection::BACKWARD)
	{
		streamTracer->SetIntegrationDirectionToBackward();
	}
	else if (mPipeLineObjProp.streamLineFilter_propData.integration_data.direction == IntegrationDirection::BOTH)
	{
		streamTracer->SetIntegrationDirectionToBoth();
	}
	if (mPipeLineObjProp.streamLineFilter_propData.integration_data.type == IntegratorType::Runge_Kutta_2)
	{
		streamTracer->SetIntegratorTypeToRungeKutta2();
	}
	else if (mPipeLineObjProp.streamLineFilter_propData.integration_data.type == IntegratorType::Runge_Kutta_4)
	{
		streamTracer->SetIntegratorTypeToRungeKutta4();
	}
	else if (mPipeLineObjProp.streamLineFilter_propData.integration_data.type == IntegratorType::Runge_Kutta_4_5)
	{
		streamTracer->SetIntegratorTypeToRungeKutta45();
	}
	streamTracer->SetMaximumPropagation(500);
	streamTracer->SetInitialIntegrationStep(mPipeLineObjProp.streamLineFilter_propData.integration_data.initStepLen);
	streamTracer->SetMaximumIntegrationStep(mPipeLineObjProp.streamLineFilter_propData.integration_data.maxiStepLen);
	streamTracer->SetMinimumIntegrationStep(mPipeLineObjProp.streamLineFilter_propData.integration_data.miniStepLen);
	streamTracer->SetIntegrationStepUnit(mPipeLineObjProp.streamLineFilter_propData.integration_data.stepUnit);
	streamTracer->SetMaximumNumberOfSteps(mPipeLineObjProp.streamLineFilter_propData.stream_data.maxiSteps);
	streamTracer->SetMaximumError(mPipeLineObjProp.streamLineFilter_propData.stream_data.maxiError);
	streamTracer->SetTerminalSpeed(mPipeLineObjProp.streamLineFilter_propData.stream_data.terminalSpeed);
	streamTracer->SetComputeVorticity(mPipeLineObjProp.streamLineFilter_propData.vorticity);
	streamTracer->Update();
	/*vtkSmartPointer<vtkTubeFilter> streamTube = vtkSmartPointer<vtkTubeFilter>::New();
	streamTube->SetInputConnection(streamerLine->GetOutputPort());
	streamTube->SetNumberOfSides(50);
	streamTube->SetRadius(0.02);
	streamTube->SetUseDefaultNormal(0);
	streamTube->Update();*/
	if (glyph_map == NULL)
		glyph_map = vtkSmartPointer<vtkDataSetMapper>::New();
	if (m_reflectionPara.flag_refleciton)
	{
		vtkReflectionFilter* reflectionFilter = vtkReflectionFilter::New();
		reflectionFilter->SetInputData(streamTracer->GetOutput());
		reflectionFilter->SetPlane(m_reflectionPara.type);
		reflectionFilter->CopyInputOn();
		reflectionFilter->Update();
		glyph_map->SetInputConnection(reflectionFilter->GetOutputPort());
		mPipelineDataSet = (vtkDataSet*)reflectionFilter->GetOutput();
	}
	else
	{
		glyph_map->SetInputConnection(streamTracer->GetOutputPort());
		mPipelineDataSet = (vtkDataSet*)streamTracer->GetOutput();
	}
	/*qDebug() << "point_min:   " << tep_polydata->GetBounds()[0] << "   ," << tep_polydata->GetBounds()[2] << "   ," << tep_polydata->GetBounds()[4] << "\n";
	qDebug() << "point_max:   " << tep_polydata->GetBounds()[1] << "   ," << tep_polydata->GetBounds()[3] << "   ," << tep_polydata->GetBounds()[5] << "\n";
	qDebug() << "point_num:   " << tep_polydata->GetNumberOfPoints()<<"\n";
	qDebug() << "cell_num:   " << tep_polydata->GetNumberOfCells() << "\n";*/
	/*for (int i = 0; i < mPipelineDataSet->GetNumberOfPoints(); i++)
	{
		QString str = QString("point %1 : %2  , %3  , %4\n").arg(i).arg(mPipelineDataSet->GetPoint(i)[0]).arg(mPipelineDataSet->GetPoint(i)[1]).arg(mPipelineDataSet->GetPoint(i)[2]);
		qDebug() << str;
	}
	for (int i = 0; i <mPipelineDataSet->GetNumberOfCells(); i++)
	{
		vtkIdList* tep_list = mPipelineDataSet->GetCell(i)->GetPointIds();
		QString tepSTR,ttt;
		for (int j = 0; j < tep_list->GetNumberOfIds(); j++)
		{
			tepSTR += ttt.setNum((int)tep_list->GetId(j), 10); tepSTR += "  ,";
		}
		tepSTR += "\n";
		qDebug()<<"cell_ID: "<<i<<"  :  "<<tepSTR;
	}*/
	//func_update_xyzColorColumn();
	if (selectActor == NULL)
	{
		selectActor = vtkSmartPointer<vtkActor>::New();
		selectActor->SetBackfaceProperty(mBackFaceProp);
	}
	selectActor->SetMapper(glyph_map);
	return true;
}