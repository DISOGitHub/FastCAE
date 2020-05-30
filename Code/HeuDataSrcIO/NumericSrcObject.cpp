#include "NumericSrcObject.h"
#include "qfileinfo.h"
#include <QtWidgets/QLabel>
#include "qdebug.h"
#include "qtextcodec.h"
#include "string.h"
#include <vtkInformation.h>
#include <vtkInformationIdTypeKey.h>
#include "vtkColorTransferFunction.h"
#include <vtkPiecewiseFunction.h>
#include "qmath.h"
#include "vtkSphereSource.h"
#include "vtkQuad.h"
#include "qmath.h"
#include <vtkPolyVertex.h>
#include <vtkCell.h>
NumericSrcObject::NumericSrcObject(QStringList dataFileList,int fileXh)
	: mDataFileList(dataFileList), mFileXh(fileXh)
{
	refrence_point_for_objID = -1;
	mDataFile = dataFileList.at(fileXh);
	mFileXh = fileXh;
	mDataFileList= dataFileList;
	readdata_io = NULL;
	flag_generate_scalarBarWidget = false;
	SetObjectType(dNumeric_DataSource);
	scalarBar_widgetMap.clear();
	QFileInfo finf(mDataFile);
	this->SetName(finf.fileName());
	//add for Mooring Line
	pointData=NULL;
	mArrowSource = NULL;
	mConeSource = NULL;
	glyphFilter = NULL;
	glyph_map = NULL;
	mCylinderSource = NULL;
	mooringLine_normals = NULL;
	mBlockDataSet = NULL;
}

NumericSrcObject::~NumericSrcObject()
{
	QMap<QString, vtkColorTransferFunction*>::iterator it;
	vtkColorTransferFunction* tep_corTransFer = NULL;
	for (it = scalarBar_lookupTableMap.begin(); it != scalarBar_lookupTableMap.end(); it++)
	{
		tep_corTransFer = it.value();
		if (tep_corTransFer != NULL)
		{
			tep_corTransFer->Delete();
			tep_corTransFer = NULL;
		}
	}
	scalarBar_lookupTableMap.clear();

	QMap<QString, vtkPiecewiseFunction*>::iterator ito;
	vtkPiecewiseFunction* tep_opa = NULL;
	for (ito = scalarBar_lookupOpacityMap.begin(); ito != scalarBar_lookupOpacityMap.end(); ito++)
	{
		tep_opa = ito.value();
		if (tep_opa != NULL)
		{
			tep_opa->Delete();
			tep_opa = NULL;
		}
	}
	scalarBar_lookupOpacityMap.clear();
	
	if (readdata_io != NULL)
	{
		delete readdata_io;
		readdata_io = NULL;
	}
	dataSetMap.clear();
	dataSetNameMap.clear();
}

void NumericSrcObject::delDataSet()
{
	if (mBlockDataSet != NULL)
	{
		mBlockDataSet->Delete();
		mBlockDataSet = NULL;
	}
	if (mBaseGrid != NULL)
	{
		mBaseGrid->Delete();
		mBaseGrid = NULL;
	}
	if (mBaseGridBak != NULL)
	{
		mBaseGridBak->Delete();
		mBaseGridBak = NULL;
	}
	if (model_ptr != NULL)
	{
		model_ptr->Delete();
		model_ptr = NULL;
	}
}
void NumericSrcObject::setParticlesRadiusRange(vtkDataSet* tep_dataSet)
{
	int num=tep_dataSet->GetNumberOfPoints();
	if (num < 2)
		return;
	double p1[3]; double p2[3];
	tep_dataSet->GetPoint(0,p1);
	tep_dataSet->GetPoint(1,p2);
	double val = qSqrt(qPow((p1[0] - p2[0]), 2) + qPow((p1[1] - p2[1]), 2) + qPow((p1[2] - p2[2]), 2));
	mPipeLineObjProp.particles_propData.max_radius = val;
	mPipeLineObjProp.particles_propData.min_radius = val*0.01;
	mPipeLineObjProp.particles_propData.radius = val*0.5;
}
bool NumericSrcObject::readFile(QString tep_filename)
{
	bool ret = false;
	QString errmsg; 
	readdata_io = new ReadDataIO;
	if (readdata_io->LoadDataSource(tep_filename)) 
	{
		QString msg = QString("Loading %1 done.").arg(tep_filename);
		QStringList tep_list1 = mDataFileList.at(0).split("/");//QStringList tep_list1 = tep_filename.split("/"); //
		int tep_count = tep_list1.count();
		if (tep_count > 0)
		{
			mPipeLineObjProp.pipelineObj_base_propData.fileName = tep_list1.at(tep_count - 1);
			
		}
		mPipeLineObjProp.pipelineObj_base_propData.file_xh = mFileXh;
		mPipeLineObjProp.pipelineObj_base_propData.fileList = mDataFileList;
		mPipeLineObjProp.pipelineObj_base_propData.fileListBak = mDataFileList;
		mBaseGridBak = readdata_io->getDataSet();//mBaseGrid =readdata_io->getDataSet();
		setParticlesRadiusRange(mBaseGridBak);
		func_update_xyzColorColumn(mBaseGridBak);
		//add for model
		if ((mBaseGridBak != NULL) && (mDataFileList.count()>=1))
		{
			mBaseGridBak->GetCenter(origin_center);
			mPipeLineObjProp.pipelineObj_base_propData.m_struct_origin.x = origin_center[0];
			mPipeLineObjProp.pipelineObj_base_propData.m_struct_origin.y = origin_center[1];
			mPipeLineObjProp.pipelineObj_base_propData.m_struct_origin.z = origin_center[2];
			origin_center[2] = mBaseGridBak->GetBounds()[5];
			if (mPipeLineObjProp.pipelineObj_base_propData.file_xh == 0)
			{
				origin_center0[0] = origin_center[0]; origin_center0[1] = origin_center[1]; 
				origin_center0[2] = origin_center[2];
				mPipeLineObjProp.aniDisplace_propData.m_origin.x = origin_center[0];
				mPipeLineObjProp.aniDisplace_propData.m_origin.y = origin_center[1];
				mPipeLineObjProp.aniDisplace_propData.m_origin.z = origin_center[2];
				double *tep_point;
				for (vtkIdType i = 0; i < mBaseGridBak->GetNumberOfPoints(); i++)
				{
					tep_point = mBaseGridBak->GetPoint(i);
					if ((fabs(tep_point[1] - origin_center[1]) <= 0.0001) && (fabs(tep_point[2] - origin_center[2]) <= 0.0001))
					{
						refrence_point_for_objID = i;
						refrence_point_for_obj[0] = tep_point[0];
						refrence_point_for_obj[1] = tep_point[1];
						refrence_point_for_obj[2] = tep_point[2];
						break;
					}
				}
			}

		}
		//add for model
		mBlockDataSet = readdata_io->getBlockDataSet();
		dataSetMap = readdata_io->getDataSetMap();
		dataSetNameMap = readdata_io->getDataSetNameMap();
		numberOfCgnsTimes = readdata_io->getNumberOfCgnsTimes();
		if (dataSetMap.count() >= 1)
		{
			mBaseGridBak=dataSetMap.firstKey();
			idTypeBlockDataSet = dataSetMap[mBaseGridBak]; 
			if ((dataSetNameMap.contains(mBaseGridBak)) && (dataSetMap.count() >1))
			{
				SetName(mPipeLineObjProp.pipelineObj_base_propData.fileName + QString(" (%1)").arg(dataSetNameMap[mBaseGridBak]));
			}
		}
		set_GridType(readdata_io->getGridType());
		scalar_vector_val = ReadVarsList(mBaseGridBak);
		setColorList();
		if (get_GridType() == dRECTILINEAR_GRID)
		{
			vtkRectilinearGrid* tep_grid = vtkRectilinearGrid::New();
			tep_grid->DeepCopy(mBaseGridBak);
			mBaseGrid = tep_grid;
		}
		else if (get_GridType() == dSTRUCTURED_GRID)
		{
			vtkStructuredGrid* tep_grid = vtkStructuredGrid::New();
			tep_grid->DeepCopy(mBaseGridBak);
			mBaseGrid = tep_grid;
		}
		else if (get_GridType() == dPolyData)
		{
			vtkPolyData* tep_grid = vtkPolyData::New();
			tep_grid->DeepCopy(mBaseGridBak);
			mBaseGrid = tep_grid;
		}
		else
		{
			vtkUnstructuredGrid* tep_grid = vtkUnstructuredGrid::New();
			tep_grid->DeepCopy(mBaseGridBak);
			mBaseGrid = tep_grid;
		}
		flag_update_mBaseGrid = true;
		func_update_mBaseGrid();		
		ret = true;
	}
	else
	{
		delete readdata_io;
		readdata_io = NULL;
	}
	mPipelineDataSet = mBaseGrid;
	return ret;	
}


void NumericSrcObject::ChangeColumnColor()
{
	if ((QFileInfo(mPipeLineObjProp.pipelineObj_base_propData.fileName).suffix() != "moor") ||
		((QFileInfo(mPipeLineObjProp.pipelineObj_base_propData.fileName).suffix() == "moor") && (mPipeLineObjProp.mooringLine_propData.glyph_data.type ==-1)))
	{
		func_ChangeColumnColor();
	}
	else
	{
		ChangeColumnColor_MooringLine();
	}
}

bool NumericSrcObject::func_replace_vtkFloatArray(vtkSmartPointer<vtkFloatArray> src_data, vtkFloatArray* des_data)
{
	if ((src_data == NULL) || (des_data==NULL))
		return false;
	if (src_data->GetNumberOfTuples() != des_data->GetNumberOfTuples())
		return false;
	if (src_data->GetNumberOfComponents() != des_data->GetNumberOfComponents())
		return false;
	
	if (src_data->GetNumberOfComponents() == 1)
	{
		for (vtkIdType i = 0; i < src_data->GetNumberOfTuples(); i++)
		{
			des_data->SetTuple1(i, src_data->GetTuple1(i));
		}
	}
	else if (src_data->GetNumberOfComponents() == 3)
	{
		double *tep_val;
		for (vtkIdType i = 0; i < src_data->GetNumberOfTuples(); i++)
		{
			tep_val = src_data->GetTuple3(i);
			des_data->SetTuple3(i, tep_val[0], tep_val[1], tep_val[2]);
		}
	}
	else
		return false;
	return true;
}

void NumericSrcObject::ChangeColumnColorWithArray(vtkSmartPointer<vtkFloatArray> tep_data)
{
	if (mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle.indexOf("_cell000") >= 0)
	{
		QString tep = mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle;
		tep = tep.left(tep.length() - 8);
		
		if (mBaseGridBak->GetNumberOfCells() == tep_data->GetNumberOfTuples())
		{
			bool flag = func_replace_vtkFloatArray(tep_data, (vtkFloatArray*)mBaseGridBak->GetCellData()->GetArray(tep.toStdString().data()));
			if (flag)
			{
				mBaseGridBak->GetCellData()->SetActiveScalars(tep.toStdString().data());
				flag_update_mBaseGrid = true;
				if (func_update_mBaseGrid())
				{
					m_glphMapper->RemoveAllInputs();
					m_glphMapper->SetInputData(mBaseGrid);
					vtkColorTransferFunction* tep_lookupTable = getLookupTable(mBaseGridBak, mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle);
					if (tep_lookupTable != NULL)
					{
						m_glphMapper->SetLookupTable(tep_lookupTable);
						m_glphMapper->SetScalarRange(tep_data->GetRange());
						
					}
				}
			}
		}
		else
			return;
	}
	else
	{
		QString tep = mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle;
		if (selectActor->GetMapper()->GetInput()->GetNumberOfPoints() == tep_data->GetNumberOfTuples())
		{
			bool flag = func_replace_vtkFloatArray(tep_data, (vtkFloatArray*)selectActor->GetMapper()->GetInput()->GetPointData()->GetArray(tep.toStdString().data()));
			if (flag)
			{
				selectActor->GetMapper()->GetInput()->GetPointData()->SetActiveScalars(tep.toStdString().data());
				flag_update_mBaseGrid = true;
				if (func_update_mBaseGrid())
				{
					m_glphMapper->RemoveAllInputs();
					m_glphMapper->SetInputData(mBaseGrid);
					vtkColorTransferFunction* tep_lookupTable = getLookupTable(mBaseGridBak, tep);
					if (tep_lookupTable != NULL)
					{
						m_glphMapper->SetLookupTable(tep_lookupTable);
						m_glphMapper->SetScalarRange(tep_data->GetRange());
					}
				}
			}
		}
		else
			return;
	}
}

QVector<ScalarColor_Data> NumericSrcObject::init_LookUpTable_Value(vtkFloatArray* tep_array)
{
	QVector<ScalarColor_Data> tep_list; tep_list.clear();
	ScalarColor_Data tep_scalarColor_data;	
	tep_scalarColor_data.val = tep_array->GetRange()[0];
	tep_scalarColor_data.r = 1.0;
	tep_scalarColor_data.g = 0.0;
	tep_scalarColor_data.b = 0.0;
	tep_list.append(tep_scalarColor_data);
	tep_scalarColor_data.val = 0.5*(tep_array->GetRange()[0] + tep_array->GetRange()[1]);
	tep_scalarColor_data.r = 0.0;
	tep_scalarColor_data.g = 1.0;
	tep_scalarColor_data.b = 0.0;
	tep_list.append(tep_scalarColor_data);
	tep_scalarColor_data.val = tep_array->GetRange()[1];
	tep_scalarColor_data.r = 0.0;
	tep_scalarColor_data.g = 0.0;
	tep_scalarColor_data.b = 1.0;
	tep_list.append(tep_scalarColor_data);
	return tep_list;
}

QVector<ScalarOpacity_Data> NumericSrcObject::init_LookUpTable_Opacity(vtkFloatArray* tep_array)
{
	QVector<ScalarOpacity_Data> tep_list; tep_list.clear();
	ScalarOpacity_Data tep_scalarOpacity_data;
	tep_scalarOpacity_data.val = tep_array->GetRange()[0];
	tep_scalarOpacity_data.opacity = 0.0;
	tep_list.append(tep_scalarOpacity_data);
	tep_scalarOpacity_data.val = tep_array->GetRange()[1];
	tep_scalarOpacity_data.opacity = 1.0;
	tep_list.append(tep_scalarOpacity_data);
	return tep_list;
}

void NumericSrcObject::ChangeColumnColor_MooringLine()
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
	int num_mooringLine = readdata_io->get_num_mooringline();
	int num_points_oneLine = mBaseGrid->GetNumberOfPoints() / num_mooringLine;
	char * columnstr_color;
	QByteArray ba2 = mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle.toLatin1();
	columnstr_color = ba2.data();
	vtkSmartPointer<vtkFloatArray>  read_color_arr = NULL;
	read_color_arr = static_cast<vtkFloatArray*>(mBaseGrid->GetPointData()->GetArray(columnstr_color));
	if (read_color_arr == NULL)
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
	vtkSmartPointer<vtkFloatArray>  cur_color_arr = vtkSmartPointer<vtkFloatArray>::New();
	for (vtkIdType i = 0; i < read_color_arr->GetNumberOfTuples(); i++)
	{
		double tep_val=read_color_arr->GetTuple1(i);
		if (i && (!((i + 1) % num_points_oneLine)))
			continue;
		cur_color_arr->InsertNextTuple1(tep_val);
	}
	pointData->GetPointData()->SetScalars(cur_color_arr);
	pointData->GetPointData()->SetNormals(mooringLine_normals);
	glyphFilter->SetInputData(pointData);
	glyphFilter->Update();

	vtkColorTransferFunction* m_vtklookuptable = getLookupTable(mBaseGridBak, mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle);
	//vtkPiecewiseFunction* m_vtklookupOpacity = getLookupOpacity(mBaseGridBak, mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle);
	
	glyph_map->SetInputConnection(glyphFilter->GetOutputPort());
	if (m_vtklookuptable != NULL)
	{
		glyph_map->SetLookupTable(m_vtklookuptable);
		glyph_map->SetScalarRange(m_vtklookuptable->GetRange());
	}
	glyph_map->ScalarVisibilityOn();
}

void NumericSrcObject::generate_actors()
{
	if (model_ptr != NULL)
	{
		flag_exist_actors = true;
		return;
	}
	if (mBaseGrid == NULL)
		return;
	if ((QFileInfo(mPipeLineObjProp.pipelineObj_base_propData.fileName).suffix() != "moor")||
		((QFileInfo(mPipeLineObjProp.pipelineObj_base_propData.fileName).suffix() == "moor")&&(mPipeLineObjProp.mooringLine_propData.glyph_data.type==-1)))
	{
		if (mBaseGrid->GetNumberOfCells() == 0)
		{
			MakeParticles();
		}
		else
		{
			if (m_glphMapper == NULL)
				m_glphMapper = vtkSmartPointer<vtkDataSetMapper>::New();
			m_glphMapper->SetInputData(mBaseGrid);
			if (selectActor == NULL)
			{
				selectActor = vtkSmartPointer<vtkActor>::New();
				selectActor->SetBackfaceProperty(mBackFaceProp);
			}
			selectActor->SetMapper(m_glphMapper);
			mPipeLineObjProp.pipelineObj_base_propData.mPointCount = mBaseGrid->GetNumberOfPoints();
			mPipeLineObjProp.pipelineObj_base_propData.mCellCount = mBaseGrid->GetNumberOfCells();
		}
		
		if (!flag_exist_actors)
		{
			mVecActors.clear();
			mVecActors.push_back(selectActor);
			flag_exist_actors = true;
			if (QFileInfo(mPipeLineObjProp.pipelineObj_base_propData.fileName).suffix() == "moor")
				addSeaBottom(0.69, 1.0, 1.0);
		}
	}
	else
	{
		if (!MakeGlyphs())
			return;
	}
	mPipelineDataSet = mBaseGrid;
	//func_update_xyzColorColumn();
	ChangeColumnColor();
	UpdatePropertyValueData();
	ShowActor();
}
void NumericSrcObject::addSeaBottom(double r,double g,double b)
{
	if (QFileInfo(mPipeLineObjProp.pipelineObj_base_propData.fileName).suffix() == "moor")
	{
		numPoints_SeaBottom = readdata_io->get_num_mooringline();
		if (numPoints_SeaBottom > 0)
		{
			int tep_points = mBaseGrid->GetNumberOfPoints() / numPoints_SeaBottom;
			double max_x, min_x, max_y, min_y, z;
			max_x=mBaseGrid->GetPoint(tep_points-1)[0];
			max_y=mBaseGrid->GetPoint(tep_points - 1)[1];
			min_x = mBaseGrid->GetPoint(tep_points - 1)[0];
			min_y = mBaseGrid->GetPoint(tep_points - 1)[1];
			z = mBaseGrid->GetPoint(tep_points - 1)[2];
			double tep_x, tep_y;
			for (int i = 2; i <= numPoints_SeaBottom; i++)
			{
				tep_x = mBaseGrid->GetPoint(tep_points*i-1)[0];
				tep_y = mBaseGrid->GetPoint(tep_points*i-1)[1];
				if (max_x < tep_x)
					max_x = tep_x;
				if (max_y < tep_y)
					max_y = tep_y;
				if (tep_x < min_x)
					min_x = tep_x;
				if (tep_y < min_y)
					min_y = tep_y;
			}
			vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
			points->InsertNextPoint(max_x,max_y,z);
			points->InsertNextPoint(min_x, max_y, z);
			points->InsertNextPoint(min_x, min_y, z);
			points->InsertNextPoint(max_x, min_y, z);
			vtkSmartPointer<vtkCellArray> quads = vtkSmartPointer<vtkCellArray>::New();
			vtkSmartPointer<vtkQuad> quad = vtkSmartPointer<vtkQuad>::New();
			quad->GetPointIds()->SetId(0, 0);
			quad->GetPointIds()->SetId(1, 1);
			quad->GetPointIds()->SetId(2, 2);
			quad->GetPointIds()->SetId(3, 3);
			quad->GetPointIds()->SetId(2, 2);
			quads->InsertNextCell(quad);
			vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
			polydata->SetPoints(points);
			polydata->SetPolys(quads);
			vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
			mapper->SetInputData(polydata);
			vtkSmartPointer<vtkActor> actor_bottom = vtkSmartPointer<vtkActor>::New();
			actor_bottom->SetBackfaceProperty(mBackFaceProp);
			actor_bottom->SetMapper(mapper);
			actor_bottom->GetProperty()->SetColor(r, g, b);
			actor_bottom->GetBackfaceProperty()->SetColor(r, g, b);
			mVecActors.push_back(actor_bottom);
		}
	}
}
void NumericSrcObject::setColorList()
{
	mPipeLineObjProp.pipelineObj_base_propData.m_colorList.clear();
	mPipeLineObjProp.pipelineObj_base_propData.scalarList.clear();
	mPipeLineObjProp.pipelineObj_base_propData.vectorList.clear();
	for (int i = 0; i < scalar_vector_val.scalarList_Point.count(); i++)
	{
		mPipeLineObjProp.pipelineObj_base_propData.m_colorList.append(scalar_vector_val.scalarList_Point.at(i));
		mPipeLineObjProp.pipelineObj_base_propData.scalarList.append(scalar_vector_val.scalarList_Point.at(i));
	}
	for (int i = 0; i < scalar_vector_val.vectorList_Point.count(); i++)
	{
		mPipeLineObjProp.pipelineObj_base_propData.m_colorList.append(scalar_vector_val.vectorList_Point.at(i));
		mPipeLineObjProp.pipelineObj_base_propData.vectorList.append(scalar_vector_val.vectorList_Point.at(i));
	}
	for (int i = 0; i < scalar_vector_val.scalarList_Cell.count(); i++)
	{
		mPipeLineObjProp.pipelineObj_base_propData.m_colorList.append(scalar_vector_val.scalarList_Cell.at(i) + "_cell000");
		mPipeLineObjProp.pipelineObj_base_propData.scalarList.append(scalar_vector_val.scalarList_Cell.at(i) + "_cell000");
	}
	for (int i = 0; i < scalar_vector_val.vectorList_Cell.count(); i++)
	{
		mPipeLineObjProp.pipelineObj_base_propData.m_colorList.append(scalar_vector_val.vectorList_Cell.at(i) + "_cell000");
		mPipeLineObjProp.pipelineObj_base_propData.vectorList.append(scalar_vector_val.vectorList_Cell.at(i) + "_cell000");
	}
	
	mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.clear();
	mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap.clear();
	mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.clear();
	QString tepStr;
	for (int i = 0; i < mPipeLineObjProp.pipelineObj_base_propData.m_colorList.count(); i++)
	{
		mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert(mPipeLineObjProp.pipelineObj_base_propData.m_colorList.at(i), false);
		mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap.insert(mPipeLineObjProp.pipelineObj_base_propData.m_colorList.at(i), false);
		tepStr = mPipeLineObjProp.pipelineObj_base_propData.m_colorList.at(i);
		if (tepStr.contains("_cell000"))
			tepStr = tepStr.left(tepStr.length() - 8);
		scalarBarTitle_PropData tep_titlePara;
		tep_titlePara.title = tepStr;
		tep_titlePara.size = scalarBar_FontSize;
		tep_titlePara.num_lable = scalarBar_NumLables;
		mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.insert(mPipeLineObjProp.pipelineObj_base_propData.m_colorList.at(i),
			tep_titlePara);
	}

	if (mPipeLineObjProp.pipelineObj_base_propData.m_colorList.count()>0)
		mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle = mPipeLineObjProp.pipelineObj_base_propData.m_colorList.at(0);
	else
		mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle = "SolidColor";
	//add multi zone for tecplot
	mPipeLineObjProp.pipelineObj_base_propData.block_showMap.clear();
	if (mBlockDataSet != NULL)
	{
		int num_blocks = mBlockDataSet->GetNumberOfBlocks();
		if (num_blocks > 1)
		{
			QString tepstr;
			for (int i = 0; i < num_blocks; i++)
			{
				mPipeLineObjProp.pipelineObj_base_propData.block_showMap.insert(QString("zone%1").arg(i,2,10,QChar('0')), true);
			}
			mPipeLineObjProp.pipelineObj_base_propData.m_colorList.append("vtkBlockColors");
			mPipeLineObjProp.pipelineObj_base_propData.scalarList.append("vtkBlockColors");
			mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert("vtkBlockColors", false);
			mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap.insert("vtkBlockColors", false);
			scalarBarTitle_PropData tep_titlePara;
			tep_titlePara.title = "vtkBlockColors";
			tep_titlePara.size = scalarBar_FontSize;
			tep_titlePara.num_lable = scalarBar_NumLables;
			mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.insert("vtkBlockColors", tep_titlePara);
			mPipeLineObjProp.pipelineObj_base_propData.flag_grid_blocks = 1;
		}
	}
	else if (idTypeBlockDataSet.count()>1)
	{
		QMap<QString, vtkSmartPointer<vtkIdTypeArray>>::iterator it;
		for (it = idTypeBlockDataSet.begin(); it != idTypeBlockDataSet.end(); it++)
		{
			mPipeLineObjProp.pipelineObj_base_propData.block_showMap.insert(it.key(), true);
		}
		mPipeLineObjProp.pipelineObj_base_propData.m_colorList.append("vtkBlockColors");
		mPipeLineObjProp.pipelineObj_base_propData.scalarList.append("vtkBlockColors");
		mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert("vtkBlockColors", false);
		mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap.insert("vtkBlockColors", false);
		scalarBarTitle_PropData tep_titlePara;
		tep_titlePara.title = "vtkBlockColors";
		tep_titlePara.size = scalarBar_FontSize;
		tep_titlePara.num_lable = scalarBar_NumLables;
		mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.insert("vtkBlockColors", tep_titlePara);
		mPipeLineObjProp.pipelineObj_base_propData.flag_grid_blocks = 1;
	}

}

bool NumericSrcObject::MakeGlyphs()
{
	if (mBaseGrid == NULL)
		return false;
	int num_mooringLine=readdata_io->get_num_mooringline();
	int num_points_oneLine=mBaseGrid->GetNumberOfPoints()/num_mooringLine;
	if (num_points_oneLine < 2)
		return false;
	
	if (mooringLine_normals==NULL)
		mooringLine_normals = vtkSmartPointer<vtkDoubleArray>::New();
	mooringLine_normals->SetNumberOfComponents(3);
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	int pointnum = mBaseGrid->GetNumberOfPoints();
	double *p;
	for (vtkIdType i = 0; i < pointnum; i++)
	{
		if (i && (!((i+1) % num_points_oneLine)))
			continue;
		p = mBaseGrid->GetPoint(i);
		points->InsertNextPoint(p);
	}
	for (vtkIdType i = 0; i < pointnum - 1; i++)
	{
		if (i && (!((i+1) % num_points_oneLine)))
			continue;
		double p1[3]; double p2[3];
		mBaseGrid->GetPoint(i + 1, p2);
		mBaseGrid->GetPoint(i, p1);
		mooringLine_normals->InsertNextTuple3(p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2]);
	}
	
	QString tep_str1;
	if (pointData==NULL)
		pointData = vtkSmartPointer<vtkPolyData>::New();
	pointData->SetPoints(points);
	
	if (mooringLine_normals != NULL)
		pointData->GetPointData()->SetNormals(mooringLine_normals);
	if (glyphFilter==NULL)
		glyphFilter = vtkSmartPointer<vtkGlyph3D>::New();
	if (mPipeLineObjProp.mooringLine_propData.glyph_data.type == 0)//kGlyph_Arrow
	{
		if (mArrowSource==NULL)
			mArrowSource = vtkSmartPointer<vtkArrowSource>::New();
		mArrowSource->SetTipResolution(mPipeLineObjProp.mooringLine_propData.glyph_data.arrow_data.tipRes);
		mArrowSource->SetTipRadius(mPipeLineObjProp.mooringLine_propData.glyph_data.arrow_data.tipRad);
		mArrowSource->SetTipLength(mPipeLineObjProp.mooringLine_propData.glyph_data.arrow_data.tipLen);
		mArrowSource->SetShaftResolution(mPipeLineObjProp.mooringLine_propData.glyph_data.arrow_data.shaftRes);
		mArrowSource->SetShaftRadius(mPipeLineObjProp.mooringLine_propData.glyph_data.arrow_data.shaftRad);
		mArrowSource->Update();
		glyphFilter->SetSourceConnection(mArrowSource->GetOutputPort());
	}
	else if (mPipeLineObjProp.mooringLine_propData.glyph_data.type == 1)//kGlyph_Cone
	{
		if (mConeSource==NULL)
			mConeSource = vtkSmartPointer<vtkConeSource>::New();
		mConeSource->SetResolution(mPipeLineObjProp.mooringLine_propData.glyph_data.cone_data.Res);
		mConeSource->SetRadius(mPipeLineObjProp.mooringLine_propData.glyph_data.cone_data.Rad);
		mConeSource->SetHeight(mPipeLineObjProp.mooringLine_propData.glyph_data.cone_data.Hei);
		mConeSource->Update();
		glyphFilter->SetSourceConnection(mConeSource->GetOutputPort());
	}
	else if (mPipeLineObjProp.mooringLine_propData.glyph_data.type == 2)//kGlyph_cylinder
	{
		if (mCylinderSource==NULL)
			mCylinderSource = vtkSmartPointer<vtkCylinderSource>::New();
		mCylinderSource->SetResolution(mPipeLineObjProp.mooringLine_propData.glyph_data.cylinder_data.Res);
		mCylinderSource->SetRadius(mPipeLineObjProp.mooringLine_propData.glyph_data.cylinder_data.Rad);
		mCylinderSource->SetHeight(mPipeLineObjProp.mooringLine_propData.glyph_data.cylinder_data.Hei);
		mCylinderSource->SetCenter(mPipeLineObjProp.mooringLine_propData.glyph_data.cylinder_data.glyph_center.x,
			mPipeLineObjProp.mooringLine_propData.glyph_data.cylinder_data.glyph_center.y,
			mPipeLineObjProp.mooringLine_propData.glyph_data.cylinder_data.glyph_center.z);
		mCylinderSource->Update();
		glyphFilter->SetSourceConnection(mCylinderSource->GetOutputPort());
	}
		
	//glyphFilter->SetScaleModeToDataScalingOff();
	glyphFilter->SetInputData(pointData);
	glyphFilter->SetVectorModeToUseNormal();
	glyphFilter->SetScaleFactor(mPipeLineObjProp.mooringLine_propData.glyph_data.scaleFactor);//ÉèÖÃÉìËõ±ÈÀý
	glyphFilter->ScalingOn();
	glyphFilter->OrientOn();
	glyphFilter->Update();
	if (glyph_map==NULL)
		glyph_map = vtkSmartPointer<vtkPolyDataMapper>::New();
	glyph_map->SetInputConnection(glyphFilter->GetOutputPort());
	
	if (selectActor == NULL)
	{
		selectActor = vtkSmartPointer<vtkActor>::New();
		selectActor->SetBackfaceProperty(mBackFaceProp);
	}
	selectActor->SetMapper(glyph_map);
	mPipeLineObjProp.pipelineObj_base_propData.mPointCount = pointData->GetNumberOfPoints();
	mPipeLineObjProp.pipelineObj_base_propData.mCellCount = pointData->GetNumberOfCells();
	mVecActors.clear();
	mVecActors.push_back(selectActor);
	flag_exist_actors = true;
	if (QFileInfo(mPipeLineObjProp.pipelineObj_base_propData.fileName).suffix() == "moor")
	{
		if(mPipeLineObjProp.mooringLine_propData.flag_seaBottom)
			addSeaBottom(mPipeLineObjProp.mooringLine_propData.seaBottomColor.r, mPipeLineObjProp.mooringLine_propData.seaBottomColor.g, 
			mPipeLineObjProp.mooringLine_propData.seaBottomColor.b);
	}
	return true;
}
bool NumericSrcObject::MakeParticles()
{
	//vtkActor* ttt;
	if (mBaseGrid == NULL)
		return false;
	if (mPipeLineObjProp.particles_propData.type == 0)
	{
		if (glyph_map == NULL)
			glyph_map = vtkSmartPointer<vtkPolyDataMapper>::New();
		vtkPolyData* tep_polyData = static_cast<vtkPolyData*>(mBaseGrid);
		vtkPolyVertex * polyvertex = vtkPolyVertex::New();
		int tep_num = mBaseGrid->GetNumberOfPoints();
		polyvertex->GetPointIds()->SetNumberOfIds(tep_num);
		for (int i = 0; i<tep_num; i++)
		{
			polyvertex->GetPointIds()->SetId(i, i);
		}
		tep_polyData->InsertNextCell(polyvertex->GetCellType(),	polyvertex->GetPointIds());
		glyph_map->SetInputData(tep_polyData);
		if (selectActor == NULL)
		{
			selectActor = vtkSmartPointer<vtkActor>::New();
			selectActor->SetBackfaceProperty(mBackFaceProp);
		}
		selectActor->SetMapper(glyph_map);
	}
	else
	{
		if (glyphFilter == NULL)
			glyphFilter = vtkSmartPointer<vtkGlyph3D>::New();
		vtkSphereSource *sphereSource = vtkSphereSource::New();
		sphereSource->SetRadius(mPipeLineObjProp.particles_propData.radius);
		//sphereSource->SetCenter(25, 0, 0);
		sphereSource->SetThetaResolution(mPipeLineObjProp.particles_propData.resolution);
		sphereSource->SetPhiResolution(mPipeLineObjProp.particles_propData.resolution);
		sphereSource->Update();
		glyphFilter->SetSourceConnection(sphereSource->GetOutputPort());

		//glyphFilter->SetScaleModeToDataScalingOff();
		vtkPolyData* tep_polyData = static_cast<vtkPolyData*>(mBaseGrid);
		glyphFilter->SetInputData(tep_polyData);
		//glyphFilter->SetScaleFactor(mPipeLineObjProp.mooringLine_propData.glyph_data.scaleFactor);//ÉèÖÃÉìËõ±ÈÀý
		//glyphFilter->ScalingOn();
		glyphFilter->Update();
		if (glyph_map == NULL)
			glyph_map = vtkSmartPointer<vtkPolyDataMapper>::New();
		glyph_map->SetInputConnection(glyphFilter->GetOutputPort());

		if (selectActor == NULL)
		{
			selectActor = vtkSmartPointer<vtkActor>::New();
			selectActor->SetBackfaceProperty(mBackFaceProp);
		}
		selectActor->SetMapper(glyph_map);
	}
	mPipeLineObjProp.pipelineObj_base_propData.mPointCount = mBaseGrid->GetNumberOfPoints();
	mPipeLineObjProp.pipelineObj_base_propData.mCellCount = mBaseGrid->GetNumberOfCells();
	mVecActors.clear();
	mVecActors.push_back(selectActor);
	flag_exist_actors = true;
	return true;
}