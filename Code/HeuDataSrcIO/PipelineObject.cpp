#include "PipelineObject.h"
#include "qdebug.h"
#include "vtkPolyData.h"
#include "vtkRectilinearGrid.h"
#include "vtkDataSet.h"
#include "qtextcodec.h"
#include "QtXml/qdom.h"
#include "vtkTextProperty.h"
#include "vtkSelectionNode.h"
#include "vtkSelection.h"
#include "vtkExtractSelection.h"
#include "vtkColorSeries.h"
#include "vtkNamedColors.h"
#include "vtkColor.h"
#include "qfileinfo.h"
#include "vtkTransform.h"
#include "vtkTextProperty.h"
#include "qdir.h"
#include "vtkBorderRepresentation.h"
#include "vtkReflectionFilter.h"

PipelineObject::PipelineObject(PipelineObject* parent)
	: QObject(parent),mName("Unknown"), mParent(parent)
{
	flag_exist_actors = false;
	componentXYZ_arr = NULL;
	model_ptr = NULL;
	mBaseGrid = NULL;
	mBaseGridBak = NULL;
	mBlockDataSet = NULL;
	glyphVector_arr = NULL;
	scalarBar_positionMap.clear();
	idTypeBlockDataSet.clear();
	dataSetMap.clear();
	dataSetNameMap.clear();
	numberOfCgnsTimes = 0;
	flag_update_mBaseGrid = false;
	mGridType = dDefault;
	m_glphMapper = vtkSmartPointer<vtkDataSetMapper>::New();
	if (mBackFaceProp == NULL)
		mBackFaceProp = vtkSmartPointer<vtkProperty>::New();
	mObjectType = dNumeric_DataSource;
	for (int i = 0; i < 6; i++)
		model_move_para[i] = 0;
	axesActor = NULL;
	mPipelineDataSet = NULL;
	numPoints_SeaBottom = 0;
	m_reflectionPara.flag_refleciton = false;
	m_reflectionPara.type = ReflectionPlane::USE_X_MIN;
	mPipeLineObjProp.pipelineObj_base_propData.pre_componentXYZ = "";
}

vtkDataSet* PipelineObject::func_merge_blockDataSet(vtkMultiBlockDataSet* tep_blockDataSet)
{
	vtkDataSet* tep_merge_dataSet = NULL;
	if (tep_blockDataSet == NULL)
		return NULL;
	bool flag_combine=false;
	int num_blocks = tep_blockDataSet->GetNumberOfBlocks();
	if (num_blocks <= 1)
	{
		return NULL;
	}
	QString tep_key, tepstr;
	bool tep_val;
	vtkAppendFilter* appendFilter = vtkAppendFilter::New();
	for (int i = 0; i < num_blocks; i++)
	{
		tep_key = QString("zone%1").arg(i, 2, 10, QChar('0'));
		if (!mPipeLineObjProp.pipelineObj_base_propData.block_showMap.contains(tep_key))
			continue;
		tep_val = mPipeLineObjProp.pipelineObj_base_propData.block_showMap[tep_key];
		if (!tep_val)
			continue;
		vtkDataSet* tep_dataset = vtkDataSet::SafeDownCast(tep_blockDataSet->GetBlock(i));
		if (tep_dataset == NULL)
			continue;
		if (!tep_dataset->GetPointData()->HasArray("vtkBlockColors"))
		{
			int tep_pnum = tep_dataset->GetNumberOfPoints();
			vtkSmartPointer<vtkFloatArray> floatArray = vtkSmartPointer<vtkFloatArray>::New();
			floatArray->SetNumberOfValues(tep_pnum);
			floatArray->SetNumberOfComponents(1);
			floatArray->SetName("vtkBlockColors");
			for (vtkIdType j = 0; j < tep_pnum; j++)
			{
				floatArray->SetValue(j, i);
			}
			tep_dataset->GetPointData()->AddArray(floatArray);
		}
		appendFilter->AddInputData(tep_dataset);
		flag_combine = true;
	}
	if (flag_combine)
	{
		appendFilter->Update();
		set_GridType(dUNSTRUCTURED_GRID);
		vtkUnstructuredGrid* tep_grid = vtkUnstructuredGrid::New();
		tep_grid->DeepCopy(appendFilter->GetOutput());
		tep_merge_dataSet = tep_grid;
	}
	appendFilter->Delete();
	appendFilter = NULL;
	return tep_merge_dataSet;
}

vtkDataSet* PipelineObject::func_merge_idTypeBlockDataSet(QMap<QString, vtkSmartPointer<vtkIdTypeArray>> tep_idTypeArrayMap)
{
	if (tep_idTypeArrayMap.count() <= 1)
		return NULL;
	vtkDataSet* tep_merge_dataSet = NULL;
	int num_blocks = tep_idTypeArrayMap.count();
	if (num_blocks < 1)
		return NULL;
	bool flag_combine=false;
	vtkAppendFilter* appendFilter = vtkAppendFilter::New();
	QMap<QString, bool>::iterator it;
	int tep_count = -1;
	for (it = mPipeLineObjProp.pipelineObj_base_propData.block_showMap.begin(); it != mPipeLineObjProp.pipelineObj_base_propData.block_showMap.end(); it++)
	{
		QString zoneId = it.key();
		bool flag_show = it.value();
		tep_count++;
		if (flag_show)
		{
			if (tep_idTypeArrayMap.contains(zoneId))
			{
				vtkSmartPointer<vtkIdTypeArray> ids1 = tep_idTypeArrayMap[zoneId]; 
				vtkDataSet* tep_grid = getExtractUnstructuredGrid(zoneId, ids1);
				if (tep_grid != NULL)
				{
					if (!tep_grid->GetPointData()->HasArray("vtkBlockColors"))
					{
						int tep_pnum = tep_grid->GetNumberOfPoints();
						vtkSmartPointer<vtkFloatArray> floatArray = vtkSmartPointer<vtkFloatArray>::New();
						floatArray->SetNumberOfValues(tep_pnum);
						floatArray->SetNumberOfComponents(1);
						floatArray->SetName("vtkBlockColors");
						for (vtkIdType j = 0; j < tep_pnum; j++)
						{
							floatArray->SetValue(j, tep_count);  
						}
						tep_grid->GetPointData()->AddArray(floatArray);
					}
					appendFilter->AddInputData(tep_grid);
					flag_combine = true;
					tep_grid->Delete();
					tep_grid = NULL;
				}				
			}
		}
	}
	if (flag_combine)
	{
		appendFilter->Update();
		vtkUnstructuredGrid* tep_grid = vtkUnstructuredGrid::New();
		tep_grid->DeepCopy(appendFilter->GetOutput());
		tep_merge_dataSet = tep_grid;

		set_GridType(dUNSTRUCTURED_GRID);
	}
	else
		tep_merge_dataSet = NULL;
	appendFilter->Delete();
	appendFilter = NULL;
	return tep_merge_dataSet;
}

bool PipelineObject::func_update_mBaseGrid()
{ 
	if (!flag_update_mBaseGrid)
		return false;
	if (mBaseGrid == NULL)
	{
		flag_update_mBaseGrid = false;
		return false;
	}
	if ((mBlockDataSet == NULL) && (idTypeBlockDataSet.count() <= 1))
	{
		flag_update_mBaseGrid = false;
		return false;
	}
	if (!mPipeLineObjProp.pipelineObj_base_propData.flag_grid_blocks)
	{
		if (mBaseGrid != NULL)
			mBaseGrid->Delete();
		/*vtkUnstructuredGrid* tep_grid = vtkUnstructuredGrid::New();
		tep_grid->DeepCopy(mBaseGridBak);
		mBaseGrid = tep_grid;*/
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
		return true;
	}
	
	if ((mBlockDataSet != NULL) && (mBlockDataSet->GetNumberOfBlocks() >= 1))
	{
		vtkDataSet* tep_dataset1 = func_merge_blockDataSet(mBlockDataSet);
		if (tep_dataset1 != NULL)
		{
			if (mBaseGrid != NULL)
				mBaseGrid->Delete();
			mBaseGrid = tep_dataset1;
			flag_update_mBaseGrid = false;
			/*if (!mBaseGridBak->GetPointData()->HasArray("vtkBlockColors") && mBaseGrid->GetPointData()->HasArray("vtkBlockColors"))
				mBaseGridBak->GetPointData()->AddArray(mBaseGrid->GetPointData()->GetArray("vtkBlockColors"));*/
			
			return true;
		}
	}
	if (idTypeBlockDataSet.count() > 0)
	{
		vtkDataSet* tep_dataset2 = func_merge_idTypeBlockDataSet(idTypeBlockDataSet);
		if (tep_dataset2 != NULL)
		{
			if (mBaseGrid != NULL)
				mBaseGrid->Delete();
			mBaseGrid = tep_dataset2;
			flag_update_mBaseGrid = false;
			return true;
		}
	}
	flag_update_mBaseGrid = false;
	return false;
}

void PipelineObject::init_subObject_count()
{
	mPipeLineObjProp.pipelineObj_base_propData.count_total_child = 0;
	mPipeLineObjProp.pipelineObj_base_propData.count_clip = 0;
	mPipeLineObjProp.pipelineObj_base_propData.count_slice = 0;
	mPipeLineObjProp.pipelineObj_base_propData.count_contour = 0;
	mPipeLineObjProp.pipelineObj_base_propData.count_vector = 0;
	mPipeLineObjProp.pipelineObj_base_propData.count_reflection = 0;
	mPipeLineObjProp.pipelineObj_base_propData.count_calculator = 0;
	mPipeLineObjProp.pipelineObj_base_propData.count_smooth = 0;
	mPipeLineObjProp.pipelineObj_base_propData.count_streamLine = 0;
}

PipelineObject::PipelineObject(QString& name, PipelineObject* parent)
	: mName(name), mParent(parent)
{

}

void PipelineObject::ShowActor()
{
	for (int i = 0; i < mVecActors.count(); i++)
	{
		if (i == 1)
		{
			if (!mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors)
			{
				mVecActors[i]->SetVisibility(0);
			}
			continue;
		}
		mVecActors[i]->SetVisibility(mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors);
	}
}

PipelineObject* PipelineObject::GetObjParent()
{
	return mParent;
}

void PipelineObject::SetObjectType(Def_PipelineObjType type)
{
	mObjectType = type;
}
Def_PipelineObjType PipelineObject::GetObjectType()
{
	return mObjectType;
}

PipelineObject::~PipelineObject()
{
	/*if (componentXYZ_arr != NULL)
	{
		componentXYZ_arr->Delete();
		componentXYZ_arr = NULL;
	}*/
}

void PipelineObject::SetPosition()
{
	for (int i = 0; i < mVecActors.count(); i++)
	{
		mVecActors[i]->SetPosition(mPipeLineObjProp.pipelineObj_base_propData.m_struct_transforming.x, mPipeLineObjProp.pipelineObj_base_propData.m_struct_transforming.y, mPipeLineObjProp.pipelineObj_base_propData.m_struct_transforming.z);
	}
}

QByteArray PipelineObject::QStringToChar(QString sourceStr)
{
	QTextCodec *codec = QTextCodec::codecForName("GB18030");
	QByteArray ba = codec->fromUnicode(sourceStr);
	return ba;
}

void PipelineObject::SetRenderMode()
{
	QFileInfo tep_fin(mPipeLineObjProp.pipelineObj_base_propData.fileName);
	for (int i = 0; i < mVecActors.count(); i++)
	{
		auto p = mVecActors[i]->GetProperty();
		p->EdgeVisibilityOff();
		p->SetRepresentationToSurface();
		switch (mPipeLineObjProp.pipelineObj_base_propData.m_enum_representationtype)
		{
		case ERepresentationType::kkPoints:
			p->SetRepresentationToPoints();
			break;
		case ERepresentationType::kkSurface:
			p->SetRepresentationToSurface();
			break;
		case ERepresentationType::kkSurfaceWithEdges:
			p->SetRepresentationToSurface();
			p->SetEdgeColor(mPipeLineObjProp.pipelineObj_base_propData.mEdgeColor.r / 255.0, mPipeLineObjProp.pipelineObj_base_propData.mEdgeColor.g / 255.0, mPipeLineObjProp.pipelineObj_base_propData.mEdgeColor.b / 255.0);
			p->EdgeVisibilityOn();
			break;
		case ERepresentationType::kWIreframe:
			p->SetRepresentationToWireframe();
			//p->SetEdgeColor(mPipeLineObjProp.pipelineObj_base_propData.mEdgeColor.r, mPipeLineObjProp.pipelineObj_base_propData.mEdgeColor.g, mPipeLineObjProp.pipelineObj_base_propData.mEdgeColor.b);
			break;
		default:
			break;
		}
	}
}

//不透明度，范围0.0~1.0，0.0时完全透明，1.0时完全不透明，需要注意现代3D技术存在透明排序问题，vtk无法解决，因此遇到一些奇怪的透明现象可能无解
void PipelineObject::SetOpacity()
{
	for (int i = 0; i < mVecActors.count(); i++)
	{
		auto p =mVecActors[i]->GetProperty();
		auto pb = mVecActors[i]->GetBackfaceProperty();
		p->SetOpacity(mPipeLineObjProp.pipelineObj_base_propData.mOpacity);
		pb->SetOpacity(mPipeLineObjProp.pipelineObj_base_propData.mOpacity);
	}
}

void PipelineObject::SetLightingAmbientColor()
{
	double r, g, b;
	r = mPipeLineObjProp.pipelineObj_base_propData.mAmbientColor.r / 255.0;
	g = mPipeLineObjProp.pipelineObj_base_propData.mAmbientColor.g / 255.0;
	b = mPipeLineObjProp.pipelineObj_base_propData.mAmbientColor.b / 255.0;
	for (int i = 0; i < mVecActors.count(); i++)
	{
		mVecActors[i]->GetProperty()->SetAmbientColor(r, g, b);
		mVecActors[i]->GetBackfaceProperty()->SetAmbientColor(r, g,b);
	}
}

//光照环境光颜色
void PipelineObject::SetLightingAmbient()
{
	for (int i = 0; i < mVecActors.count(); i++)
	{
		mVecActors[i]->GetProperty()->SetAmbient(mPipeLineObjProp.pipelineObj_base_propData.mAmbient);
		mVecActors[i]->GetBackfaceProperty()->SetAmbient(mPipeLineObjProp.pipelineObj_base_propData.mAmbient);
	}
}

//光照漫反射颜色
void PipelineObject::SetLightingDiffuseColor()
{
	for (int i = 0; i < mVecActors.count(); i++)
	{
		mVecActors[i]->GetProperty()->SetDiffuse(mPipeLineObjProp.pipelineObj_base_propData.mDiffuse);
		mVecActors[i]->GetBackfaceProperty()->SetDiffuse(mPipeLineObjProp.pipelineObj_base_propData.mDiffuse);
	}
}

//光照高光（亮斑）光洁度，虽然英文是power，但是实际上这个参数应该是phong光照模型中的光洁度，范围0.0~128.0，注意值很低，比如小于5.0时效果会很奇怪
void PipelineObject::SetLightingSpecularPower()
{
	for (int i = 0; i < mVecActors.count(); i++)
	{
		mVecActors[i]->GetProperty()->SetSpecularPower(mPipeLineObjProp.pipelineObj_base_propData.mSpecularPower);
		mVecActors[i]->GetBackfaceProperty()->SetSpecularPower(mPipeLineObjProp.pipelineObj_base_propData.mSpecularPower);
	}
}

void PipelineObject::SetLightingSpecularCorlor()
{
	double r, g, b;
	r = mPipeLineObjProp.pipelineObj_base_propData.mSpecularColor.r / 255.0;
	g = mPipeLineObjProp.pipelineObj_base_propData.mSpecularColor.g / 255.0;
	b = mPipeLineObjProp.pipelineObj_base_propData.mSpecularColor.b / 255.0;
	for (int i = 0; i < mVecActors.count(); i++)
	{
		mVecActors[i]->GetProperty()->SetSpecularColor(r,g, b);
		mVecActors[i]->GetBackfaceProperty()->SetSpecularColor(r, g, b);
	}
}

void PipelineObject::SetSolidCorlor()
{
	if (mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle == mdefaultColumnName)
	{
		double r, g, b;
		r = mPipeLineObjProp.pipelineObj_base_propData.mSolidColor.r / 255.0;
		g = mPipeLineObjProp.pipelineObj_base_propData.mSolidColor.g / 255.0;
		b = mPipeLineObjProp.pipelineObj_base_propData.mSolidColor.b / 255.0;
		for (int i = 0; i < mVecActors.count(); i++)
		{
			mVecActors[i]->GetProperty()->SetColor(r, g,b );
			mVecActors[i]->GetBackfaceProperty()->SetColor(r, g, b);
		}
	}	
}

void PipelineObject::SetEdgeCorlor()
{
	double r, g, b;
	r = mPipeLineObjProp.pipelineObj_base_propData.mEdgeColor.r / 255.0;
	g = mPipeLineObjProp.pipelineObj_base_propData.mEdgeColor.g / 255.0;
	b = mPipeLineObjProp.pipelineObj_base_propData.mEdgeColor.b / 255.0;
	for (int i = 0; i < mVecActors.count(); i++)
	{
		mVecActors[i]->GetProperty()->SetEdgeColor(r, g, b);
		mVecActors[i]->GetBackfaceProperty()->SetEdgeColor(r, g, b);
	}
}

void PipelineObject::SetLightingInterpolation()
{
	for (int i = 0; i < mVecActors.count(); i++)
	{
		if (mPipeLineObjProp.pipelineObj_base_propData.m_enum_interpolationtype == EInterpolationType::kGouraud)
			mVecActors[i]->GetProperty()->SetInterpolationToGouraud();
		else
			mVecActors[i]->GetProperty()->SetInterpolationToFlat();
	}
}

void PipelineObject::SetScale()
{
	for (int i = 0; i < mVecActors.count(); i++)
	{
		mVecActors[i]->SetScale(mPipeLineObjProp.pipelineObj_base_propData.m_struct_scale.x, mPipeLineObjProp.pipelineObj_base_propData.m_struct_scale.y, mPipeLineObjProp.pipelineObj_base_propData.m_struct_scale.z);
	}
}

void PipelineObject::SetTranslate()
{
	for (int i = 0; i < mVecActors.count(); i++)
	{
		mVecActors[i]->SetPosition(mPipeLineObjProp.pipelineObj_base_propData.m_struct_transforming.x, mPipeLineObjProp.pipelineObj_base_propData.m_struct_transforming.y, mPipeLineObjProp.pipelineObj_base_propData.m_struct_transforming.z);
	}
}

void PipelineObject::SetOrigin()
{
	for (int i = 0; i < mVecActors.count(); i++)
	{
		mVecActors[i]->SetOrigin(mPipeLineObjProp.pipelineObj_base_propData.m_struct_origin.x, mPipeLineObjProp.pipelineObj_base_propData.m_struct_origin.y, mPipeLineObjProp.pipelineObj_base_propData.m_struct_origin.z);
	}
}

void PipelineObject::SetOrientation()
{
	for (int i = 0; i < mVecActors.count(); i++)
	{
		mVecActors[i]->SetOrientation(mPipeLineObjProp.pipelineObj_base_propData.m_struct_orientation.x, mPipeLineObjProp.pipelineObj_base_propData.m_struct_orientation.y, mPipeLineObjProp.pipelineObj_base_propData.m_struct_orientation.z);
	}
}

void PipelineObject::SetPointSize()
{
	for (int i = 0; i < mVecActors.count(); i++)
	{
		mVecActors[i]->GetProperty()->SetPointSize(mPipeLineObjProp.pipelineObj_base_propData.mPointSize);
	}
}

void PipelineObject::SetEdgeColor()
{
	double r, g, b;
	r = mPipeLineObjProp.pipelineObj_base_propData.m_struct_edgecolor.r / 255.0;
	g = mPipeLineObjProp.pipelineObj_base_propData.m_struct_edgecolor.g / 255.0;
	b = mPipeLineObjProp.pipelineObj_base_propData.m_struct_edgecolor.b / 255.0;
	for (int i = 0; i < mVecActors.count(); i++)
	{
		mVecActors[i]->GetProperty()->SetEdgeColor(r, g, b);
		mVecActors[i]->GetBackfaceProperty()->SetEdgeColor(r,g,b);
	}
}

//光照高光（亮斑）颜色系数（coefficient）
void PipelineObject::SetLightingSpecular()
{
	for (int i = 0; i < mVecActors.count(); i++)
	{
		mVecActors[i]->GetProperty()->SetSpecular(mPipeLineObjProp.pipelineObj_base_propData.mSpecular);
		mVecActors[i]->GetBackfaceProperty()->SetSpecular(mPipeLineObjProp.pipelineObj_base_propData.mSpecular);
	}
}

void PipelineObject::SetLightingDiffuse()
{
	for (int i = 0; i < mVecActors.count(); i++)
	{
		mVecActors[i]->GetProperty()->SetDiffuse(mPipeLineObjProp.pipelineObj_base_propData.mDiffuse);
		mVecActors[i]->GetBackfaceProperty()->SetDiffuse(mPipeLineObjProp.pipelineObj_base_propData.mDiffuse);
	}
}

void PipelineObject::SetLineWidth()
{
	for (int i = 0; i < mVecActors.count(); i++)
	{
		mVecActors[i]->GetProperty()->SetLineWidth(mPipeLineObjProp.pipelineObj_base_propData.mLineWidth);
		mVecActors[i]->GetBackfaceProperty()->SetLineWidth(mPipeLineObjProp.pipelineObj_base_propData.mLineWidth);
	}
}

void PipelineObject::UpdatePropertyValueData()
{
	SetOpacity();
	SetLightingDiffuse();
	SetLightingDiffuseColor();
	SetLightingAmbient();
	SetLineWidth();
	SetPointSize();
	SetLightingSpecular();
	SetLightingSpecularPower();
	SetLightingSpecularCorlor();
	SetLightingInterpolation();
	SetEdgeColor();
	SetOrientation();
	//SetOrigin();
	SetScale();
	SetRenderMode();
	SetPosition();
	SetMooringSeaBottom();
}
void PipelineObject::SetMooringSeaBottom()
{
	if (mVecActors.count() < 2)
		return;
	if (mPipeLineObjProp.mooringLine_propData.flag_seaBottom)
	{
		double r, g, b;
		r = mPipeLineObjProp.mooringLine_propData.seaBottomColor.r / 255.0;
		g = mPipeLineObjProp.mooringLine_propData.seaBottomColor.g / 255.0;
		b = mPipeLineObjProp.mooringLine_propData.seaBottomColor.b / 255.0;
		mVecActors[1]->GetProperty()->SetColor(r, g, b);
		mVecActors[1]->GetBackfaceProperty()->SetColor(r, g, b);
		mVecActors[1]->SetVisibility(1);
	}
	else
	{
		mVecActors[1]->SetVisibility(0);
	}
}
void PipelineObject::SetObjParent(PipelineObject* parent)
{
	mParent = parent;
}

void PipelineObject::generate_actors()
{

}

vtkColorTransferFunction* PipelineObject::getLookupTable(vtkDataSet* tep_mBaseGrid,QString colName)
{
	if (tep_mBaseGrid == NULL)
		return NULL;
	vtkColorTransferFunction* tep_vtklookuptable = NULL;
	if (scalarBar_lookupTableMap.contains(colName))
	{
		tep_vtklookuptable = scalarBar_lookupTableMap[colName];
	}
	return tep_vtklookuptable;
}

vtkPiecewiseFunction* PipelineObject::getLookupOpacity(vtkDataSet* tep_mBaseGrid, QString colName)
{
	if (tep_mBaseGrid == NULL)
		return NULL;
	vtkPiecewiseFunction* tep_vtklookupOpacity = NULL;
	if (scalarBar_lookupOpacityMap.contains(colName))
		tep_vtklookupOpacity = scalarBar_lookupOpacityMap[colName];
	return tep_vtklookupOpacity;
}

bool PipelineObject::get_column_range(vtkDataSet* tep_mBaseGrid, QString colName,double *tep_range)
{
	if (colName.indexOf("_cell000") >= 0)
	{
		QString tep = colName.left(colName.length() - 8);
		vtkFloatArray* tep_arr=static_cast<vtkFloatArray*>(tep_mBaseGrid->GetCellData()->GetArray(tep.toStdString().data()));
		if (tep_arr != NULL)
		{
			tep_arr->GetRange(tep_range);
			return true;
		}
		else
			return false;
	}
	else
	{
		vtkFloatArray* tep_arr=NULL;
		if (colName == "GlyphVector")
		{
			if ((glyphVector_arr != NULL) && (glyphVector_arr->GetNumberOfTuples() > 1))
			{
				glyphVector_arr->GetRange(tep_range);
				return true;
			}
			else
				return false;
		}
		else
			tep_arr = static_cast<vtkFloatArray*>(tep_mBaseGrid->GetPointData()->GetArray(colName.toStdString().data()));
		if (tep_arr != NULL)
		{
			tep_arr->GetRange(tep_range);
			return true;
		}
		else
			return false;
	}
}


void PipelineObject::generate_scalarBar_lookupTableMap(vtkDataSet* tep_mBaseGrid)
{
	QMap<QString, vtkColorTransferFunction*>::iterator it;
	for (it = scalarBar_lookupTableMap.begin(); it != scalarBar_lookupTableMap.end(); it++)
	{
		vtkColorTransferFunction* tep000 = it.value();
		tep000->Delete();
		tep000 = NULL;
	}
	scalarBar_lookupTableMap.clear();
	scalarBar_lookupOpacityMap.clear();
	int num1 = tep_mBaseGrid->GetPointData()->GetNumberOfArrays();
	for (int i = 0; i < num1; i++)
	{
		const char* tep = tep_mBaseGrid->GetPointData()->GetArrayName(i);
		vtkFloatArray *tep_arr = static_cast<vtkFloatArray*> (tep_mBaseGrid->GetPointData()->GetArray(tep));
		if (tep_arr != NULL)
		{
			if (strcmp(tep, "vtkBlockColors"))
			{
				vtkColorTransferFunction* tep_vtklookuptable = vtkColorTransferFunction::New();
				//tep_vtklookuptable->SetColorSpaceToDiverging();
				double range[2]; tep_arr->GetRange(range);
				if (range[0] == range[1])
					range[1] = range[0] + 0.00001;
				tep_vtklookuptable->SetColorSpaceToHSV();
				tep_vtklookuptable->HSVWrapOff();
				tep_vtklookuptable->AddRGBPoint(range[0], 0.0, 0.0, 1.0);
				tep_vtklookuptable->AddRGBPoint(0.5*(range[0] + range[1]), 0.0, 1.0, 0.0);
				tep_vtklookuptable->AddRGBPoint(range[1], 1.0, 0.0, 0.0);
				scalarBar_lookupTableMap.insert(QString(tep), tep_vtklookuptable);

				vtkPiecewiseFunction* tep_opacityFunction = vtkPiecewiseFunction::New();
				tep_opacityFunction->AddPoint(range[0], 0.0);
				tep_opacityFunction->AddPoint(range[1], 1.0);
				scalarBar_lookupOpacityMap.insert(QString(tep), tep_opacityFunction);

				if ((tep_arr->GetNumberOfComponents() == 3) && (mPipeLineObjProp.pipelineObj_base_propData.pre_componentXYZ == ""))
				{
					mPipeLineObjProp.pipelineObj_base_propData.pre_componentXYZ = "X_"+QString(tep);
				}
			}
			else
			{
				//add for multi zone
				vtkColorTransferFunction* tep_lut = makeBlocksLUT();
				if (tep_lut != NULL)
					scalarBar_lookupTableMap.insert("vtkBlockColors", (vtkColorTransferFunction*)tep_lut);
				//add for multi zone
			}
		}		
	}
	int num2 = tep_mBaseGrid->GetCellData()->GetNumberOfArrays();
	for (int i = 0; i < num2; i++)
	{
		const char* tep = tep_mBaseGrid->GetCellData()->GetArrayName(i);
		vtkFloatArray *tep_arr = static_cast<vtkFloatArray*> (tep_mBaseGrid->GetCellData()->GetArray(tep));
		if (tep_arr != NULL)
		{
			vtkColorTransferFunction* tep_vtklookuptable = vtkColorTransferFunction::New();
			double range[2]; tep_arr->GetRange(range);
			if (range[1] == range[0])
				range[1] = range[0] + 0.00001;
			//tep_vtklookuptable->SetColorSpaceToHSV();
			tep_vtklookuptable->HSVWrapOff();//tep_vtklookuptable->HSVWrapOn();
			tep_vtklookuptable->AddRGBPoint(range[0], 0.0, 0.0, 1.0);
			tep_vtklookuptable->AddRGBPoint(0.5*(range[0] + range[1]), 0.0, 1.0, 0.0);
			tep_vtklookuptable->AddRGBPoint(range[1], 1.0, 0.0, 0.0);
			scalarBar_lookupTableMap.insert(QString(tep) + "_cell000", tep_vtklookuptable);

			vtkPiecewiseFunction* tep_opacityFunction = vtkPiecewiseFunction::New();
			tep_opacityFunction->AddPoint(range[0], 0.0);
			tep_opacityFunction->AddPoint(range[1], 1.0);
			scalarBar_lookupOpacityMap.insert(QString(tep) + "_cell000", tep_opacityFunction);

			if ((tep_arr->GetNumberOfComponents() == 3) && (mPipeLineObjProp.pipelineObj_base_propData.pre_componentXYZ == ""))
			{
				mPipeLineObjProp.pipelineObj_base_propData.pre_componentXYZ = "X_" + QString(tep) + "_cell000";
			}
		}
	}
	//func_axesVal_lookupTableMap();
}

void PipelineObject::func_update_componentXYX_lookupTableMap(QString key)
{
	QString cur_componentXYZ = mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle_XYZ+
		"_"+mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle;
	if (mPipeLineObjProp.pipelineObj_base_propData.pre_componentXYZ == cur_componentXYZ)
		return;
	QString tepStr = mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle;
	vtkFloatArray* tep_arr = NULL;
	if (mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle.indexOf("_cell000") >= 0)
	{
		tepStr = tepStr.left(tepStr.length() - 8);
		tep_arr = (vtkFloatArray*)mBaseGrid->GetCellData()->GetArray(tepStr.toStdString().data());
	}
	else
	{
		tep_arr = (vtkFloatArray*)mBaseGrid->GetPointData()->GetArray(tepStr.toStdString().data());
	}
	if (tep_arr == NULL)
		return;
	vtkFloatArray* com_arr = func_getColorXYZComponent(tep_arr, mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle_XYZ);
	double range[2]; com_arr->GetRange(range);
	if (range[0] == range[1])
		range[1] = range[0] + 0.00001;
	vtkColorTransferFunction* tep_vtklookuptable=NULL;
	vtkPiecewiseFunction* tep_opacityFunction = NULL;
	if (GetObjectType() == dNumeric_DataSource)
	{
		if (scalarBar_lookupTableMap.contains(mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle))
		{
			tep_vtklookuptable = scalarBar_lookupTableMap[mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle];
		}
		if (scalarBar_lookupTableMap.contains(mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle))
		{
			tep_opacityFunction = scalarBar_lookupOpacityMap[mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle];
		}
	}
	else
	{ 
		if (GetObjParent()->scalarBar_lookupTableMap.contains(mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle))
		{
			tep_vtklookuptable = GetObjParent()->scalarBar_lookupTableMap[mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle];
		}
		if (GetObjParent()->scalarBar_lookupTableMap.contains(mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle))
		{
			tep_opacityFunction = GetObjParent()->scalarBar_lookupOpacityMap[mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle];
		}
	}
	if (tep_vtklookuptable!=NULL)
	{
		tep_vtklookuptable->RemoveAllPoints();
		tep_vtklookuptable->AddRGBPoint(range[0], 0.0, 0.0, 1.0);
		tep_vtklookuptable->AddRGBPoint(0.5*(range[0] + range[1]), 0.0, 1.0, 0.0);
		tep_vtklookuptable->AddRGBPoint(range[1], 1.0, 0.0, 0.0);
	}
	if (tep_opacityFunction!=NULL)
	{
		tep_opacityFunction->RemoveAllPoints();
		tep_opacityFunction->AddPoint(range[0], 0.0);
		tep_opacityFunction->AddPoint(range[1], 1.0);
	}
	mPipeLineObjProp.pipelineObj_base_propData.pre_componentXYZ = mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle_XYZ + "_"
		+ mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle;
}

bool PipelineObject::getcolorColumnRange(QString colName,double mRange[2])
{
	char * columnname;
	bool flag_onPoint = true;
	QString tepstr;
	mRange[0] = 0.0;
	mRange[1] = 0.0;
	if (colName == "SolidColor")
		return false;
	tepstr = colName;
	if (tepstr.indexOf("_cell000") >= 0)
	{
		tepstr = tepstr.left(tepstr.length() - 8);
		flag_onPoint = false;
	}
	QByteArray ba = QStringToChar(tepstr);
	columnname = ba.data();

	if (flag_onPoint)
	{
		if (mParent == 0)
		{
			if (mBaseGrid->GetPointData()->HasArray(columnname))
			{
				mBaseGrid->GetPointData()->GetArray(columnname)->GetRange(mRange);
				return true;
			}
			else
				return false;
		}
		else
		{
			PipelineObject *tempParent = this->GetObjParent();
			vtkSmartPointer<vtkPointData> vtkpd = tempParent->mBaseGrid->GetPointData();
			if (vtkpd->HasArray(columnname))
			{
				vtkpd->GetScalars(columnname)->GetRange(mRange);
				return true;
			}
			else
				return false;
		}
	}
	else
	{
		if (mParent == 0)
		{
			if (mBaseGrid->GetCellData()->HasArray(columnname))
			{
				mBaseGrid->GetCellData()->GetScalars(columnname)->GetRange(mRange);
				return true;
			}
			else
				return false;
		}
		else
		{
			if (GetObjParent()->mBaseGrid->GetCellData()->HasArray(columnname))
			{
				GetObjParent()->mBaseGrid->GetCellData()->GetScalars(columnname)->GetRange(mRange);
				return true;
			}
			else
				return false;
		}
	}
	return false;
}

bool PipelineObject::get_prop_val(vtkIdType tep_id, bool flag_point,double *val)
{
	if (flag_point)
	{
		if (tep_id >= mPipelineDataSet->GetNumberOfPoints())
			return false;
		QByteArray ba = QStringToChar(mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle);
		char* column_name = ba.data();
		if (mPipelineDataSet->GetPointData()->HasArray(column_name))
		{
			vtkDataArray* tep_arr = mPipelineDataSet->GetPointData()->GetArray(column_name);
			if (tep_arr == NULL)
				return false;
			int num_components = tep_arr->GetNumberOfComponents();
			if ((num_components == 1) || (num_components == 3))
			{
				if (num_components == 1)
					*val = mPipelineDataSet->GetPointData()->GetArray(column_name)->GetTuple1(tep_id);
				else if (num_components == 3)
				{
					int xb = 0;
					if (mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle_XYZ == "X")
						xb = 0;
					else if (mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle_XYZ == "Y")
						xb = 1;
					else if (mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle_XYZ == "Z")
						xb = 2;
					*val = mPipelineDataSet->GetPointData()->GetArray(column_name)->GetTuple3(tep_id)[xb];
				}
				//qDebug() << "tep_id="<<tep_id<<"           val=" << *val<<endl;
				return true;
			}
			else
				return false;
		}
		return false;
	}
	else
	{
		if (tep_id >= mPipelineDataSet->GetNumberOfCells())
			return false;
		if (mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle.indexOf("_cell000") < 0)
			return false;
		QString str = mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle;
		str = str.left(str.length() - 8);
		QByteArray ba = QStringToChar(str);
		char* column_name = ba.data();
		if (mPipelineDataSet->GetCellData()->HasArray(column_name))
		{
			vtkDataArray* tep_arr = mPipelineDataSet->GetCellData()->GetArray(column_name);
			if (tep_arr == NULL)
				return false;
			int num_components = tep_arr->GetNumberOfComponents();
			if ((num_components == 1) || (num_components == 3))
			{
				if (num_components == 1)
					*val = mPipelineDataSet->GetCellData()->GetArray(column_name)->GetTuple1(tep_id);
				else if (num_components == 3)
				{
					int xb = 0;
					if (mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle_XYZ == "X")
						xb = 0;
					else if (mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle_XYZ == "Y")
						xb = 1;
					else if (mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle_XYZ == "Z")
						xb = 2;
					*val = mPipelineDataSet->GetCellData()->GetArray(column_name)->GetTuple3(tep_id)[xb];
				}
				//qDebug() << "tep_id=" << tep_id << "           val=" << *val << endl;
				return true;
			}
			else
				return false;
		}
		return false;
	}
}


vtkUnstructuredGrid* PipelineObject::getExtractUnstructuredGrid(QString zoneName, vtkSmartPointer<vtkIdTypeArray> ids)
{
	//if (zoneName == "fluid")
	//	return NULL;
	vtkUnstructuredGrid* selected = NULL;
	if (ids == NULL)
		return selected;
	if (ids->GetNumberOfTuples() < 1)
		return selected;
	vtkSmartPointer<vtkSelectionNode> selectionNode = vtkSmartPointer<vtkSelectionNode>::New(); 
	QString tep_name = ids->GetName();//tep_name += "_cell000"; 
	if (tep_name.indexOf("_cell000") >= 0)
		selectionNode->SetFieldType(vtkSelectionNode::CELL);
	else
		selectionNode->SetFieldType(vtkSelectionNode::POINT);
	selectionNode->SetContentType(vtkSelectionNode::INDICES);
	selectionNode->SetSelectionList(ids);
	//selectionNode->GetProperties()->Set(vtkSelectionNode::INVERSE(), 1);

	vtkSmartPointer<vtkSelection> selection = vtkSmartPointer<vtkSelection>::New();
	selection->AddNode(selectionNode);

	vtkSmartPointer<vtkExtractSelection> extractSelection = vtkSmartPointer<vtkExtractSelection>::New();
	extractSelection->SetInputData(0, mBaseGridBak);
	extractSelection->SetInputData(1, selection);
	extractSelection->Update(); 

	selected = vtkUnstructuredGrid::New();
	selected->ShallowCopy(extractSelection->GetOutput());
	return  selected;
}

Scalar_Vector_NameListData PipelineObject::ReadVarsList(vtkDataSet* tep_dataSet)
{
	Scalar_Vector_NameListData tep_scalar_vector_list;
	if (tep_dataSet == nullptr) return tep_scalar_vector_list;
	tep_scalar_vector_list.scalarList_Point.clear();
	tep_scalar_vector_list.scalarList_Cell.clear();
	tep_scalar_vector_list.vectorList_Point.clear();
	tep_scalar_vector_list.vectorList_Cell.clear();
	vtkSmartPointer<vtkPointData> pd = NULL;
	QString columnName;
	pd = tep_dataSet->GetPointData();
	int tep_num1, tep_num2;
	if (pd)
	{
		tep_num1 = pd->GetNumberOfArrays();
		for (int i = 0; i < tep_num1; i++)
		{
			columnName = pd->GetArrayName(i);
			vtkFloatArray* dataarray = static_cast<vtkFloatArray *>(pd->GetArray(columnName.toStdString().data()));
			if (dataarray == NULL)
				continue;
			tep_num2 = dataarray->GetNumberOfComponents();
			//int aa1 = dataarray->GetNumberOfTuples();
			if (tep_num2 == 1)
			{
				tep_scalar_vector_list.scalarList_Point.append(columnName);
			}
			else if (tep_num2 == 3)
			{
				tep_scalar_vector_list.vectorList_Point.append(columnName);
			}
		}
	}
	vtkSmartPointer<vtkCellData> cd = NULL;
	cd = tep_dataSet->GetCellData();
	if (cd)
	{
		tep_num1 = cd->GetNumberOfArrays();
		for (int i = 0; i < tep_num1; i++)
		{
			columnName = cd->GetArrayName(i);
			vtkFloatArray* dataarray = static_cast<vtkFloatArray *>(cd->GetArray(columnName.toStdString().data())); 
			if (dataarray == NULL)
				continue;
			tep_num2 = dataarray->GetNumberOfComponents();
			//int aa1 = dataarray->GetNumberOfTuples();
			if (tep_num2 == 1)
			{
				tep_scalar_vector_list.scalarList_Cell.append(columnName);
			}
			else if (tep_num2 == 3)
			{
				tep_scalar_vector_list.vectorList_Cell.append(columnName);
			}
		}
	}
	return tep_scalar_vector_list;
}

vtkColorTransferFunction* PipelineObject::makeBlocksLUT()
{
	double lut_range[2];
	QList<QString> zoneID_list = mPipeLineObjProp.pipelineObj_base_propData.block_showMap.keys();
	vtkPointData* tep_pd = mBaseGrid->GetPointData();
	if (tep_pd == NULL)
		return NULL;
	if (!tep_pd->HasArray("vtkBlockColors"))
		return NULL;
	vtkFloatArray *tep_arr = (vtkFloatArray *)tep_pd->GetArray("vtkBlockColors");
	if (tep_arr == NULL)
		return NULL;
	tep_arr->GetRange(lut_range);

	QList<QString> colorName_List; colorName_List.clear();
	colorName_List.append("Blue");
	colorName_List.append("Cyan");
	colorName_List.append("Magenta");
	colorName_List.append("Yellow");
	colorName_List.append("White");
	colorName_List.append("Pink");
	colorName_List.append("Green");
	colorName_List.append("Black");
	colorName_List.append("Mint");
	colorName_List.append("Peacock");
	colorName_List.append("DarkBlue");
	colorName_List.append("Gray");
	colorName_List.append("Lavender");
	colorName_List.append("Wheat");
	colorName_List.append("Tomato");
	vtkSmartPointer<vtkNamedColors> nc = vtkSmartPointer<vtkNamedColors>::New();
	vtkColorTransferFunction *lut = vtkColorTransferFunction::New();
	int tep_div = zoneID_list.count() / colorName_List.count();
	int tep_mod = zoneID_list.count() % colorName_List.count();
	for (int i = 0; i < tep_div; i++)
	{
		for (int j = 0; j < colorName_List.count(); j++)
		{
			lut->AddRGBPoint(i*colorName_List.count() + j, nc->GetColor3d(colorName_List.at(j).toStdString()).GetRed(), nc->GetColor3d(colorName_List.at(j).toStdString()).GetGreen(), nc->GetColor3d(colorName_List.at(j).toStdString()).GetBlue());
		}
	}
	for (int i = 0; i < tep_mod; i++)
	{
		lut->AddRGBPoint(tep_div*colorName_List.count() + i, nc->GetColor3d(colorName_List.at(i).toStdString()).GetRed(), nc->GetColor3d(colorName_List.at(i).toStdString()).GetGreen(), nc->GetColor3d(colorName_List.at(i).toStdString()).GetBlue());
	}
	lut->IndexedLookupOn();
	for (vtkIdType i = 0; i < zoneID_list.count(); i++)
	{
		lut->SetAnnotation(i, zoneID_list.at(i).toStdString());
	}
	return lut;
}
vtkFloatArray* PipelineObject::func_getColorXYZComponent(vtkFloatArray* tep_arr, QString component)
{
	if (tep_arr->GetNumberOfComponents() != 3)
		return NULL;
	int xb = 0;
	if (component == "X")
		xb = 0;
	else if (component == "Y")
		xb = 1;
	else if (component == "Z")
		xb = 2;
	if (componentXYZ_arr == NULL)
	{
		componentXYZ_arr = vtkFloatArray::New();
		componentXYZ_arr->SetName("Component_XYZ");
		//componentXYZ_arr->SetNumberOfTuples(0);
		componentXYZ_arr->SetNumberOfComponents(1);
	}
	//else
		//componentXYZ_arr->RemoveAllObservers();
	componentXYZ_arr->SetNumberOfTuples(tep_arr->GetNumberOfTuples());
	for (vtkIdType i = 0; i < tep_arr->GetNumberOfTuples(); i++)
	{
		componentXYZ_arr->SetTuple1(i, tep_arr->GetTuple3(i)[xb]);
		//componentXYZ_arr->InsertNextTuple1(tep_arr->GetTuple3(i)[xb]);
	}
	return componentXYZ_arr;
}
void PipelineObject::func_ChangeColumnColor()
{
	if ((selectActor == NULL) || (mPipelineDataSet == NULL))
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
	
	vtkSmartPointer<vtkColorTransferFunction> tep_lookupTable;
	if(GetObjectType()==dNumeric_DataSource)
		tep_lookupTable= getLookupTable(mBaseGridBak, colName);
	else
	{
		if ((GetObjectType() == dCalculator_DataSource)&&(colName==mPipeLineObjProp.calculatorFilter_propData.resultName))
		{
			tep_lookupTable = GetObjParent()->getLookupTable(GetObjParent()->mBaseGridBak, "CalculatorResult");
		}
		else
			tep_lookupTable = GetObjParent()->getLookupTable(GetObjParent()->mBaseGridBak, colName);
	}
	if (tep_lookupTable != NULL)
	{
		if (selectActor != NULL)
		{
			selectActor->GetMapper()->SetLookupTable(tep_lookupTable);
			selectActor->GetMapper()->SetScalarRange(tep_lookupTable->GetRange());
		}
	}

	if (colName.indexOf("_cell000") >= 0)
	{
		colName = colName.left(colName.length() - 8);
		if (mPipelineDataSet->GetCellData()->HasArray(colName.toStdString().data()))
		{
			//add for vector//////////////////////////
			vtkFloatArray* tep_arr = (vtkFloatArray*)mPipelineDataSet->GetCellData()->GetArray(colName.toStdString().data());
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
				
				selectActor->GetMapper()->GetInput()->GetCellData()->SetActiveScalars(colName.toStdString().data());
				selectActor->GetMapper()->ColorByArrayComponent(colName.toStdString().data(),xb);
				selectActor->GetMapper()->SetScalarModeToUseCellFieldData();
			}
			else
				//add for vector//////////////////////////
			{
				selectActor->GetMapper()->GetInput()->GetCellData()->SetActiveScalars(colName.toStdString().data());
				selectActor->GetMapper()->SelectColorArray(colName.toStdString().data());
				selectActor->GetMapper()->SetScalarModeToUseCellData();
			}
		}
	}
	else
	{
		if (mPipelineDataSet->GetPointData()->HasArray(colName.toStdString().data()))
		{
			//add for vector//////////////////////////
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
				selectActor->GetMapper()->GetInput()->GetCellData()->SetActiveScalars(colName.toStdString().data());
				selectActor->GetMapper()->ColorByArrayComponent(colName.toStdString().data(), xb);
				selectActor->GetMapper()->SetScalarModeToUsePointFieldData();
			}
			else
				//add for vector//////////////////////////
			{
				selectActor->GetMapper()->GetInput()->GetPointData()->SetActiveScalars(colName.toStdString().data());
				selectActor->GetMapper()->SelectColorArray(colName.toStdString().data());
				selectActor->GetMapper()->SetScalarModeToUsePointData();
			}
		}
	}
	selectActor->GetMapper()->ScalarVisibilityOn();
}

void PipelineObject::ChangeColumnColor()
{
	func_ChangeColumnColor();
}

void PipelineObject::update_fileList_byFileInterval(int val)
{
	QStringList tep_list = mPipeLineObjProp.pipelineObj_base_propData.fileListBak;
	int tep_num = tep_list.count();
	if ( tep_num> 1)
	{
		int div = (tep_num - 1) / val;
		mPipeLineObjProp.pipelineObj_base_propData.fileList.clear();
		mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.clear();
		for (int k = 0; k <= div; k++)
		{
			mPipeLineObjProp.pipelineObj_base_propData.fileList.append(tep_list.at(k*val));
		}
	}
	else
	{
		mPipeLineObjProp.pipelineObj_base_propData.fileList = mPipeLineObjProp.pipelineObj_base_propData.fileListBak;
	}
}
void PipelineObject::generate_axesActor()
{
	if (mPipeLineObjProp.pipelineObj_base_propData.flag_cubeAxes)
	{
		if (axesActor == NULL)
		{
			axesActor = vtkSmartPointer<vtkCubeAxesActor>::New();
			axesActor->SetXTitle("X");
			axesActor->SetYTitle("Y");
			axesActor->SetZTitle("Z");
			axesActor->GetTitleTextProperty(0)->SetColor(1.0, 1.0, 1.0);//设置x轴的标题的颜色为
			axesActor->GetLabelTextProperty(0)->SetColor(1.0, 1.0, 1.0);//设置x轴的标签label颜色
			axesActor->GetTitleTextProperty(1)->SetColor(1.0, 1.0, 1.0);
			axesActor->GetLabelTextProperty(1)->SetColor(1.0, 1.0, 1.0);
			axesActor->GetTitleTextProperty(2)->SetColor(1.0, 1.0, 1.0);
			axesActor->GetLabelTextProperty(2)->SetColor(1.0, 1.0, 1.0);
			//axesActor->XAxisMinorTickVisibilityOff();//小标尺的打开与关闭
			//axesActor->YAxisMinorTickVisibilityOff();
			//axesActor->ZAxisMinorTickVisibilityOff();
			//axesActor->SetUseAxisOrigin(1);//里面的参数可以设为0和1，设置为1则可以固定显示的坐标轴的起点始终固定，那么坐标轴就不会随着物体的移动而移动。
			//axesActor->SetFlyModeToStaticEdges();//调整坐标轴显示固定在一个轴上
			//axesActor->SetScreenSize(25);//默认大小是10个像素

			/*画包围盒
			vtkSmartPointer<vtkOutlineFilter> outlineData = vtkSmartPointer<vtkOutlineFilter>::New();//画出包围盒
			outlineData->SetInputData(inputImage);

			vtkSmartPointer<vtkPolyDataMapper> mapOutline = vtkSmartPointer<vtkPolyDataMapper>::New();
			mapOutline->SetInputConnection(outlineData->GetOutputPort());

			vtkSmartPointer<vtkActor> outline = vtkSmartPointer<vtkActor>::New();
			outline->SetMapper(mapOutline);
			outline->GetProperty()->SetColor(1.0, 1.0, 1.0);//设置包围盒的边界的颜色为黑
			aRenderer->AddActor(outline);*/
		}
		axesActor->SetBounds(mBaseGrid->GetBounds());
	}
	/*else
	{

	}*/
}
void PipelineObject::func_update_displacement(int cur_frame_xh)
{
	if ((!mPipeLineObjProp.aniDisplace_propData.flag_AniDisplaceFile) || (!mPipeLineObjProp.aniDisplace_propData.flag_exeDisplace))
		return;
	double tep_cofficient = mPipeLineObjProp.aniDisplace_propData.zoom_factor;
	if (mPipeLineObjProp.aniDisplace_propData.flag_inputGravity)
	{
		if (model_ptr != NULL)
		{
			model_ptr->SetPosition(0,0,0);//4.219, 19.228, 5.614
			model_ptr->SetOrigin(mPipeLineObjProp.aniDisplace_propData.m_origin.x, mPipeLineObjProp.aniDisplace_propData.m_origin.y, mPipeLineObjProp.aniDisplace_propData.m_origin.z);//  + 0.259			
			model_ptr->RotateY((mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Pitch_Gyb - mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Pitch_Gyb)*57.32);
			model_ptr->RotateX((mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Roll_Gxb - mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Roll_Gxb)*57.32);
			model_ptr->RotateZ((mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Yaw_Gzb - mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Yaw_Gzb)*57.32);
			vtkSmartPointer<vtkTransform> tep_transform2 = vtkSmartPointer<vtkTransform>::New();
			tep_transform2->Translate(mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Surge_GX*tep_cofficient,
				mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Sway_GY*tep_cofficient,
				mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Heave_GZ*tep_cofficient);
			model_ptr->SetUserTransform(tep_transform2);
			qDebug() << cur_frame_xh << "angle:  " << (mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Roll_Gxb - mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Roll_Gxb)*57.32
				<< (mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Pitch_Gyb - mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Pitch_Gyb)*57.32
				<< (mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Yaw_Gzb - mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Yaw_Gzb)*57.32 << endl;
		}
		else
		{
			for (int i = 0; i < mVecActors.count(); i++)
			{
				/*mVecActors[i]->SetPosition(mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Surge_GX*tep_cofficient,
					mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Sway_GY*tep_cofficient,
					mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Heave_GZ*tep_cofficient);*///4.219,19.228,5.614
				mVecActors[i]->SetPosition(0, 0, 0); //mVecActors[i]->SetPosition(4.219, 19.228, 5.614);
				mVecActors[i]->SetOrigin(mPipeLineObjProp.aniDisplace_propData.m_origin.x, mPipeLineObjProp.aniDisplace_propData.m_origin.y, mPipeLineObjProp.aniDisplace_propData.m_origin.z);//  + 0.259			
				mVecActors[i]->RotateY((mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Pitch_Gyb - mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Pitch_Gyb)*57.32);
			    mVecActors[i]->RotateX((mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Roll_Gxb - mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Roll_Gxb)*57.32);
				mVecActors[i]->RotateZ((mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Yaw_Gzb - mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Yaw_Gzb)*57.32);
				vtkSmartPointer<vtkTransform> tep_transform2 = vtkSmartPointer<vtkTransform>::New();
				tep_transform2->Translate(mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Surge_GX*tep_cofficient,
					mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Sway_GY*tep_cofficient,
					mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Heave_GZ*tep_cofficient);
				mVecActors[i]->SetUserTransform(tep_transform2);
				/*if (i==0)
				qDebug() << cur_frame_xh << ":  " << mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Surge_GX
							<< mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Sway_GY << mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Heave_GZ << endl;*/
			}
		}
	}
	else
	{
		for (int i = 0; i < mVecActors.count(); i++)
		{

			mVecActors[i]->SetPosition(mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Surge_GX*tep_cofficient, mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Sway_GY*tep_cofficient, mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Heave_GZ*tep_cofficient);
			mVecActors[i]->SetOrigin(mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Surge_GX*tep_cofficient, mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Sway_GY*tep_cofficient, mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Heave_GZ*tep_cofficient);//  + 0.259
			mVecActors[i]->RotateX(mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Roll_Gxb*tep_cofficient);
			mVecActors[i]->RotateY(mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Pitch_Gyb*tep_cofficient);
			mVecActors[i]->RotateZ(mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Yaw_Gzb*tep_cofficient);
		}
	}
	//for (int i = 0; i < mVecActors.count(); i++)
	//{
	//	if (mPipeLineObjProp.aniDisplace_propData.flag_inputGravity)
	//	{
	//		mVecActors[i]->SetPosition(0,0,0);//4.219,19.228,5.614
	//		mVecActors[i]->SetOrigin(mPipeLineObjProp.aniDisplace_propData.m_origin.x, mPipeLineObjProp.aniDisplace_propData.m_origin.y, mPipeLineObjProp.aniDisplace_propData.m_origin.z);//  + 0.259			
	//		mVecActors[i]->RotateY((mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Pitch_Gyb - mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Pitch_Gyb)*57.32);
	//		mVecActors[i]->RotateX((mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Roll_Gxb - mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Roll_Gxb)*57.32);
	//		mVecActors[i]->RotateZ((mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Yaw_Gzb - mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Yaw_Gzb)*57.32);
	//		vtkSmartPointer<vtkTransform> tep_transform2 = vtkSmartPointer<vtkTransform>::New();
	//		tep_transform2->Translate(mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Surge_GX*tep_cofficient,
	//			mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Sway_GY*tep_cofficient,
	//			mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Heave_GZ*tep_cofficient);
	//		mVecActors[i]->SetUserTransform(tep_transform2);
	//		//mVecActors[i]->AddPosition(mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Surge_GX*tep_cofficient,
	//		//	mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Sway_GY*tep_cofficient,
	//		//	mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Heave_GZ*tep_cofficient);
	//	}
	//	else
	//	{
	//		mVecActors[i]->SetPosition(mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Surge_GX*tep_cofficient, mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Sway_GY*tep_cofficient, mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Heave_GZ*tep_cofficient);
	//		mVecActors[i]->SetOrigin(mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Surge_GX*tep_cofficient, mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Sway_GY*tep_cofficient, mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Heave_GZ*tep_cofficient);//  + 0.259
	//		mVecActors[i]->RotateX(mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Roll_Gxb*tep_cofficient);
	//		mVecActors[i]->RotateY(mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Pitch_Gyb*tep_cofficient);
	//		mVecActors[i]->RotateZ(mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Yaw_Gzb*tep_cofficient);
	//	}
	//}
	mPipeLineObjProp.aniDisplace_propData.m_pre_displace.time = mPipeLineObjProp.aniDisplace_propData.m_cur_displace.time;
	mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Roll_Gxb = mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Roll_Gxb;
	mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Pitch_Gyb = mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Pitch_Gyb;
	mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Yaw_Gzb = mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Yaw_Gzb;
	mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Surge_GX = mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Surge_GX;
	mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Sway_GY = mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Sway_GY;
	mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Heave_GZ = mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Heave_GZ;
	mPipeLineObjProp.aniDisplace_propData.flag_exeDisplace = false;
}
void PipelineObject::func_init_displacement()
{
	if (mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.count() < 1)
		return;
	mPipeLineObjProp.aniDisplace_propData.m_pre_displace.time = mPipeLineObjProp.aniDisplace_propData.m_cur_displace.time;
	mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Roll_Gxb = mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Roll_Gxb;
	mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Pitch_Gyb = mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Pitch_Gyb;
	mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Yaw_Gzb = mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Yaw_Gzb;
	mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Surge_GX = mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Surge_GX;
	mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Sway_GY = mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Sway_GY;
	mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Heave_GZ = mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Heave_GZ;
	mPipeLineObjProp.aniDisplace_propData.m_cur_displace.time = mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(0).time;
	mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Roll_Gxb = mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(0).Roll_Gxb;
	mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Pitch_Gyb = mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(0).Pitch_Gyb;
	mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Yaw_Gzb = mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(0).Yaw_Gzb;
	mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Surge_GX = mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(0).Surge_GX;
	mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Sway_GY = mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(0).Sway_GY;
	mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Heave_GZ = mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(0).Heave_GZ;
}
void PipelineObject::func_update_xyzColorColumn(vtkDataSet* tep_dataSet)
{
	if (tep_dataSet == NULL)
		return;
	int tep_num_points = tep_dataSet->GetNumberOfPoints();
	if (tep_num_points < 1)
		return;
	if (tep_arrX == NULL)
	{
		tep_arrX = vtkSmartPointer<vtkFloatArray>::New();
		tep_arrX->SetName("Axes_X");
		tep_arrX->SetNumberOfTuples(0);
		tep_arrX->SetNumberOfComponents(1);
	}
	else
		tep_arrX->RemoveAllObservers();
	for (vtkIdType i = 0; i < tep_num_points; i++)
	{
		tep_arrX->InsertNextTuple1((tep_dataSet->GetPoint(i))[0]);
	}
	if (tep_arrY == NULL)
	{
		tep_arrY = vtkSmartPointer<vtkFloatArray>::New();
		tep_arrY->SetName("Axes_Y");
		tep_arrY->SetNumberOfTuples(0);
		tep_arrY->SetNumberOfComponents(1);
	}
	else
		tep_arrY->RemoveAllObservers();
	for (vtkIdType i = 0; i < tep_num_points; i++)
	{
		tep_arrY->InsertNextTuple1((tep_dataSet->GetPoint(i))[1]);
	}
	if (tep_arrZ == NULL)
	{
		tep_arrZ = vtkSmartPointer<vtkFloatArray>::New();
		tep_arrZ->SetName("Axes_Z");
		tep_arrZ->SetNumberOfTuples(0);
		tep_arrZ->SetNumberOfComponents(1);
	}
	else
		tep_arrZ->RemoveAllObservers();
	for (vtkIdType i = 0; i < tep_num_points; i++)
	{
		tep_arrZ->InsertNextTuple1((tep_dataSet->GetPoint(i))[2]);
	}
	if (!tep_dataSet->GetPointData()->HasArray("Axes_X"))
		tep_dataSet->GetPointData()->AddArray(tep_arrX);
	if (!tep_dataSet->GetPointData()->HasArray("Axes_Y"))
		tep_dataSet->GetPointData()->AddArray(tep_arrY);
	if (!tep_dataSet->GetPointData()->HasArray("Axes_Z"))
		tep_dataSet->GetPointData()->AddArray(tep_arrZ);
}

vtkSmartPointer<vtkLookupTable> PipelineObject::MakeLUTFromCTF(vtkColorTransferFunction* colorTransferFunction,	vtkPiecewiseFunction* opacityFunction, int tableSize)
{
	vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
	lut->SetNumberOfTableValues(tableSize);
	lut->SetTableRange(colorTransferFunction->GetRange());
	double tep_range[2]; colorTransferFunction->GetRange(tep_range);
	double tep_step = (tep_range[1] - tep_range[0]) / (tableSize - 1);
	for (int i = 0; i < tableSize; i++)
	{
		double rgb[3]; double tep_opacity;
		colorTransferFunction->GetColor(tep_range[0] + i*tep_step, rgb);
		tep_opacity = opacityFunction->GetValue(tep_range[0] + i*tep_step);
		lut->SetTableValue(i, rgb[0], rgb[1], rgb[2], tep_opacity);
	}
	lut->Build();
	return lut;
}
vtkFloatArray* PipelineObject::getColumnData(QString colName)
{
	vtkFloatArray* arr = NULL;
	if (mBaseGrid == NULL)
		return arr;
	if (colName.indexOf(colName) >= 0)
	{
		vtkCellData* cd = mBaseGrid->GetCellData();
		if (cd == NULL)
			return arr;
		QString tepName = colName.left(colName.length() - 8);
		if (cd->HasArray(tepName.toStdString().data()))
			arr =(vtkFloatArray*) cd->GetArray(tepName.toStdString().data());
		return arr;
	}
	else
	{
		vtkPointData* pd = mBaseGrid->GetPointData();
		if (pd == NULL)
			return arr;
		if (pd->HasArray(colName.toStdString().data()))
			arr = (vtkFloatArray*)pd->GetArray(colName.toStdString().data());
		return arr;
	}
}
void PipelineObject::func_ReInitLookupTable(QString tep_colName,QString val)
{
	QStringList valList = val.split(",");
	if (valList.count() % 4)
		return;
	bool ok;
	vtkColorTransferFunction* tep_colorTransfer;
	QString command_line = "";
	if (GetObjectType() != dNumeric_DataSource)
	{
		PipelineObject* tep_pipeObj = GetObjParent();
		if ((GetObjectType() == dCalculator_DataSource)
			&& (tep_colName == mPipeLineObjProp.calculatorFilter_propData.resultName))
		{
			if (tep_pipeObj->scalarBar_lookupTableMap.contains("CalculatorResult"))
			{
				tep_colorTransfer = tep_pipeObj->scalarBar_lookupTableMap["CalculatorResult"];
			}
			else
			{
					return;
			}
		}
		else
		{
			if (tep_pipeObj->scalarBar_lookupTableMap.contains(tep_colName))
			{
				tep_colorTransfer = tep_pipeObj->scalarBar_lookupTableMap[tep_colName];
			}
			else
			{	
					return;
			}
		}
	}
	else
	{
		if (scalarBar_lookupTableMap.contains(tep_colName))
		{
			tep_colorTransfer = scalarBar_lookupTableMap[tep_colName];
		}
		else
			return;
	}
	
	tep_colorTransfer->RemoveAllPoints();
	int num1 = valList.count() / 4;
	for (int i = 0; i < num1; i++)
	{
		tep_colorTransfer->AddRGBPoint(valList.at(i * 4).toDouble(&ok), valList.at(4 * i + 1).toDouble(&ok), valList.at(4 * i + 2).toDouble(&ok), valList.at(4 * i + 3).toDouble(&ok));
	}
	tep_colorTransfer->Build();
	int flag = 0;
	QString tepStr;
	if (tep_colName.indexOf("_cell000") >= 0)
	{
		flag = 1;
		tepStr = tep_colName.left(tep_colName.length() - 8);
	}
	if (func_isVector(tepStr, flag, mBaseGrid))
	{
		mPipeLineObjProp.pipelineObj_base_propData.pre_componentXYZ = mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle_XYZ+"_"+tep_colName;
	}
}
void PipelineObject::func_ReInitLookupOpacity(QString tep_colName, QString val)
{
	QStringList valList = val.split(",");
	if (valList.count() % 2)
		return;
	bool ok;
	vtkPiecewiseFunction* tep_opacity;
	QString command_line = "";
	if (GetObjectType() != dNumeric_DataSource)
	{
		PipelineObject* tep_pipeObj = GetObjParent();
		if ((GetObjectType() == dCalculator_DataSource)
			&& (tep_colName == mPipeLineObjProp.calculatorFilter_propData.resultName))
		{
			if (tep_pipeObj->scalarBar_lookupOpacityMap.contains("CalculatorResult"))
			{
				tep_opacity = tep_pipeObj->scalarBar_lookupOpacityMap["CalculatorResult"];
			}
			else
				return;
		}
		else
		{
			if (tep_pipeObj->scalarBar_lookupOpacityMap.contains(tep_colName))
			{
				tep_opacity = tep_pipeObj->scalarBar_lookupOpacityMap[tep_colName];
			}
			else
				return;
		}
	}
	else
	{
		if (scalarBar_lookupOpacityMap.contains(tep_colName))
		{
			tep_opacity = scalarBar_lookupOpacityMap[tep_colName];
		}
		else
			return;
	}
	tep_opacity->RemoveAllPoints();
	int num1 = valList.count() /2;
	for (int i = 0; i < num1; i++)
	{
		tep_opacity->AddPoint(valList.at(2 * i).toDouble(&ok), valList.at(2 * i + 1).toDouble(&ok));
	}
}
void PipelineObject::func_initGlyphVectorArr(vtkDataSet* tep_dataSet)
{
	if ((mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle == mPipeLineObjProp.vectorFilter_propData.m_preGlyphVectorPara.colName)
		&& (mPipeLineObjProp.vectorFilter_propData.vectorColName == mPipeLineObjProp.vectorFilter_propData.m_preGlyphVectorPara.vecName)
		&& (mPipeLineObjProp.vectorFilter_propData.glyphVector_colorXYZ == mPipeLineObjProp.vectorFilter_propData.m_preGlyphVectorPara.XYZName))
		return;
	if (tep_dataSet == NULL)
		return;
	char * columnstr_vector;
	QByteArray ba1 = mPipeLineObjProp.vectorFilter_propData.vectorColName.toLatin1();
	columnstr_vector = ba1.data();
	vtkFloatArray* vector_arr = NULL;
	if (strcmp(columnstr_vector, "None") != 0)
	{
		if (mPipeLineObjProp.vectorFilter_propData.vectorColName.indexOf("_cell000") >= 0)
		{
			int tep_len = (int)strlen(columnstr_vector);
			columnstr_vector[tep_len - 8] = '\0';
			vector_arr = static_cast<vtkFloatArray*>(tep_dataSet->GetCellData()->GetArray(columnstr_vector));
		}
		else
		{
			vector_arr = static_cast<vtkFloatArray*>(tep_dataSet->GetPointData()->GetArray(columnstr_vector));
		}
		if (vector_arr == NULL)
		{
			return;
		}
	}
	if (glyphVector_arr == NULL)
	{
		glyphVector_arr = vtkFloatArray::New();
		glyphVector_arr->SetNumberOfTuples(0);
		glyphVector_arr->SetNumberOfComponents(1);
	}
	int tep_num = vector_arr->GetNumberOfTuples();
	glyphVector_arr->SetNumberOfTuples(tep_num);
	int index = 0;
	if (mPipeLineObjProp.vectorFilter_propData.glyphVector_colorXYZ == "X")
	{
		index = 0;
		glyphVector_arr->SetName("GlyphVector_X");
	}
	else if (mPipeLineObjProp.vectorFilter_propData.glyphVector_colorXYZ == "Y")
	{
		index = 1;
		glyphVector_arr->SetName("GlyphVector_Y");
	}
	else if (mPipeLineObjProp.vectorFilter_propData.glyphVector_colorXYZ == "Z")
	{
		index = 2;
		glyphVector_arr->SetName("GlyphVector_Z");
	}
	
	for (vtkIdType i = 0; i <tep_num; i++)
	{
		glyphVector_arr->SetTuple1(i, vector_arr->GetTuple3(i)[index]);
		//glyphVector_arr->InsertNextTuple1(vector_arr->GetTuple3(i)[index]);
	}
	
	func_insertGlyphVector_lookupTableMap(glyphVector_arr);
}
//20181203 add glyphVector
void PipelineObject::func_insertGlyphVector_lookupTableMap(vtkFloatArray* tep_arr)
{
	//int tep_num = tep_arr->GetNumberOfTuples();
	double range[2];
	vtkColorTransferFunction* tep_vtklookuptableX = vtkColorTransferFunction::New();
	tep_arr->GetRange(range);
	if (range[1] == range[0])
		range[1] = range[0] + 0.00001;
	tep_vtklookuptableX->HSVWrapOff();
	tep_vtklookuptableX->AddRGBPoint(range[0], 0.0, 0.0, 1.0);
	tep_vtklookuptableX->AddRGBPoint(0.5*(range[0] + range[1]), 0.0, 1.0, 0.0);
	tep_vtklookuptableX->AddRGBPoint(range[1], 1.0, 0.0, 0.0);
	GetObjParent()->scalarBar_lookupTableMap.insert("GlyphVector", tep_vtklookuptableX);
	vtkPiecewiseFunction* tep_opacityFunctionX = vtkPiecewiseFunction::New();
	tep_opacityFunctionX->AddPoint(range[0], 0.0);
	tep_opacityFunctionX->AddPoint(range[1], 1.0);
	GetObjParent()->scalarBar_lookupOpacityMap.insert("GlyphVector", tep_opacityFunctionX);
	//GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap.insert("GlyphVector", false);
	if (!GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.contains("GlyphVector"))
	{
		scalarBarTitle_PropData tep_titlePara;
		tep_titlePara.title = tep_arr->GetName();
		tep_titlePara.size = scalarBar_FontSize;
		tep_titlePara.num_lable = scalarBar_NumLables;
		GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.insert("GlyphVector", tep_titlePara);
	}
	mPipeLineObjProp.vectorFilter_propData.m_preGlyphVectorPara.colName = mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle;
	mPipeLineObjProp.vectorFilter_propData.m_preGlyphVectorPara.vecName = mPipeLineObjProp.vectorFilter_propData.vectorColName;
	mPipeLineObjProp.vectorFilter_propData.m_preGlyphVectorPara.XYZName = mPipeLineObjProp.vectorFilter_propData.glyphVector_colorXYZ;
	
}
//20181203 add glyphVector

void PipelineObject::func_insertCalculatorResult_lookupTableMap(vtkFloatArray* tep_arr)
{
	//int tep_num = tep_arr->GetNumberOfTuples();
	double range[2];
	vtkColorTransferFunction* tep_vtklookuptableX = vtkColorTransferFunction::New();
	tep_arr->GetRange(range);
	if (range[1] == range[0])
		range[1] = range[0] + 0.00001;
	tep_vtklookuptableX->HSVWrapOff();
	tep_vtklookuptableX->AddRGBPoint(range[0], 0.0, 0.0, 1.0);
	tep_vtklookuptableX->AddRGBPoint(0.5*(range[0] + range[1]), 0.0, 1.0, 0.0);
	tep_vtklookuptableX->AddRGBPoint(range[1], 1.0, 0.0, 0.0);
	GetObjParent()->scalarBar_lookupTableMap.insert("CalculatorResult", tep_vtklookuptableX);
	vtkPiecewiseFunction* tep_opacityFunctionX = vtkPiecewiseFunction::New();
	tep_opacityFunctionX->AddPoint(range[0], 0.0);
	tep_opacityFunctionX->AddPoint(range[1], 1.0);
	GetObjParent()->scalarBar_lookupOpacityMap.insert("CalculatorResult", tep_opacityFunctionX);
	scalarBarTitle_PropData tep_titlePara;
	tep_titlePara.title = tep_arr->GetName();
	tep_titlePara.size = scalarBar_FontSize;
	tep_titlePara.num_lable = scalarBar_NumLables;
	if (!GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.contains("CalculatorResult"))
		GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.insert("CalculatorResult", tep_titlePara);
	if (!GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.contains("CalculatorResult"))
		GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert("CalculatorResult", false);
	if (!GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap.contains("CalculatorResult"))
		GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap.insert("CalculatorResult", false);
	
}

vtkDataSet* PipelineObject::func_getReflectionDataset(vtkDataSet* tep_dataSet)
{
	if (!m_reflectionPara.flag_refleciton)
		return NULL;
	if (tep_dataSet == NULL)
		return NULL;
	vtkReflectionFilter* reflectionFilter = vtkReflectionFilter::New();
	reflectionFilter->SetInputData(tep_dataSet);
	reflectionFilter->SetPlane(m_reflectionPara.type);
	reflectionFilter->CopyInputOn();
	//reflectionFilter->CopyInputOff();
	reflectionFilter->Update();
	return (vtkDataSet*)reflectionFilter->GetOutput();
}
bool PipelineObject::func_isVector(QString colName, int flag_PointCell, vtkDataSet* tep_dataSet)//0point,1cell
{
	int a=tep_dataSet->GetCellData()->GetNumberOfArrays();
	int b = tep_dataSet->GetPointData()->GetNumberOfArrays();
	qDebug() << "a: " << a << "   b:   " << b<<endl;
	bool flag = false;
	vtkFloatArray* tep_arr = NULL;
	if (flag_PointCell)
		tep_arr = (vtkFloatArray*)tep_dataSet->GetCellData()->GetArray(colName.toStdString().data());
	else
		tep_arr = (vtkFloatArray*)tep_dataSet->GetPointData()->GetArray(colName.toStdString().data());
	if (tep_arr != NULL)
	{
		if (tep_arr->GetNumberOfComponents() == 3)
		{
			flag = true;
		}
	}
	return flag;
}