#include "FilterClip.h"
#include "vtkUnstructuredGrid.h"
#include "qdebug.h"

FilterClip::FilterClip(const QString& dataFile, PipelineObject * parent)
:mDataFile(dataFile)
{
	SetObjParent(parent);
	scalarBar_widgetMap.clear();
	mclipDataSet = NULL;
	m_glphMapper = NULL;
	plane_interactor = NULL;
	planeWidget = NULL;
	selectActor = NULL;
	outline = NULL;
	outlineActor = NULL;
	outlineMapper = NULL;
	clipRep = NULL;
	plane = NULL;
	myCallback = NULL;
	SetNormal(1, 0, 0);
	if (GetObjParent() != NULL)
	{
		mBaseGrid = GetObjParent()->mBaseGrid;
		mPipeLineObjProp = GetObjParent()->mPipeLineObjProp;
		init_subObject_count();
		GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_clip++;
	}
	mBaseGrid->GetCenter(origin_center);
	QString tep_filter_str;
	mPipeLineObjProp.plane_propData.filterName = "Clip" + tep_filter_str.setNum(GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_clip, 10);
	mPipeLineObjProp.plane_propData.m_plane_origin.x = origin_center[0];
	mPipeLineObjProp.plane_propData.m_plane_origin.y = origin_center[1];
	mPipeLineObjProp.plane_propData.m_plane_origin.z = origin_center[2];
	mPipeLineObjProp.plane_propData.m_plane_normal.x = 1;
	mPipeLineObjProp.plane_propData.m_plane_normal.y = 0;
	mPipeLineObjProp.plane_propData.m_plane_normal.z = 0;
	mPipeLineObjProp.plane_propData.flag_plane = true;
	mPipeLineObjProp.plane_propData.flag_insideOut = true;
	mPipeLineObjProp.pipelineObj_base_propData.fileName = "ClipFilter";
	mPipeLineObjProp.pipelineObj_base_propData.fileName = GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.fileName;
	mPipeLineObjProp.pipelineObj_base_propData.file_xh = 0;
	SetObjectType(dClip_DataSource);
	GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_total_child++;
}


FilterClip::~FilterClip()
{
	if (GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_total_child>0)
		GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_total_child--;
}

QString FilterClip::GetName() const
{
	QString tep_str;
	return mPipeLineObjProp.plane_propData.filterName;
}

void FilterClip::plane_visible(bool flag)
{
	if (flag)
	{
		if (outlineActor != NULL)
		{
			outlineActor->VisibilityOn();
		}
		if (planeWidget != NULL)
		{
			planeWidget->On();
		}
	}
	else
	{
		if (outlineActor != NULL)
		{
			outlineActor->VisibilityOff();
		}
		if (planeWidget != NULL)
		{
			planeWidget->Off();
		}
	}
}

void FilterClip::get_clip_pos_data(double tep_val[6])
{
	plane->GetOrigin(&tep_val[0]);
	plane->GetNormal(&tep_val[3]);
}

void FilterClip::func_init_plane()
{
	if (plane == NULL)
		plane = vtkPlane::New();
	plane->SetOrigin(mPipeLineObjProp.plane_propData.m_plane_origin.x, mPipeLineObjProp.plane_propData.m_plane_origin.y, mPipeLineObjProp.plane_propData.m_plane_origin.z);//clipRep->SetOrigin(origin_center);
	plane->SetNormal(mPipeLineObjProp.plane_propData.m_plane_normal.x, mPipeLineObjProp.plane_propData.m_plane_normal.y, mPipeLineObjProp.plane_propData.m_plane_normal.z);//clipRep->SetNormal(normal);
	
	if(!flag_exist_actors)
		mBaseGrid->GetCenter(origin_center);
	if (outline == NULL)
	{
		outline = vtkSmartPointer<vtkOutlineFilter>::New();
	}
	outline->SetInputData(mBaseGrid);
	if (outlineMapper == NULL)
	{
		outlineMapper = vtkSmartPointer<vtkDataSetMapper>::New();
	}
	outlineMapper->SetInputConnection(outline->GetOutputPort());
	if (outlineActor == NULL)
	{
		outlineActor = vtkSmartPointer<vtkActor>::New();
		
	}
	outlineActor->SetMapper(outlineMapper);
	if (mPipeLineObjProp.plane_propData.flag_plane)
		outlineActor->VisibilityOn();
	else
		outlineActor->VisibilityOff();

	if (mclipDataSet == NULL)
	{
		mclipDataSet = vtkSmartPointer<vtkClipDataSet>::New();
		
	}
	mclipDataSet->SetClipFunction(plane);
	mclipDataSet->SetInputData(mBaseGrid);

	if (clipRep == NULL)
	{
		clipRep = vtkSmartPointer<vtkImplicitPlaneRepresentation>::New();
		clipRep->SetPlaceFactor(1.0);
		clipRep->SetOutlineTranslation(0);
		clipRep->GetPlaneProperty()->SetOpacity(0.4);
	}
	clipRep->SetOrigin(mPipeLineObjProp.plane_propData.m_plane_origin.x, mPipeLineObjProp.plane_propData.m_plane_origin.y, mPipeLineObjProp.plane_propData.m_plane_origin.z);//clipRep->SetOrigin(origin_center);
	clipRep->SetNormal(mPipeLineObjProp.plane_propData.m_plane_normal.x, mPipeLineObjProp.plane_propData.m_plane_normal.y, mPipeLineObjProp.plane_propData.m_plane_normal.z);//clipRep->SetNormal(normal);
	clipRep->PlaceWidget(mBaseGrid->GetBounds());	
	clipRep->SetPlane(plane);//clipRep->GetPlane(plane);
	clipRep->UpdatePlacement();
	
	if (planeWidget == NULL)
	{
		planeWidget = vtkSmartPointer<vtkImplicitPlaneWidget2>::New();
		planeWidget->SetInteractor(plane_interactor);
	}
	planeWidget->SetRepresentation(clipRep);
	if (mPipeLineObjProp.plane_propData.flag_plane)
		planeWidget->On();
	else
		planeWidget->Off();
	if (myCallback == NULL)
	{
		myCallback = vtkSmartPointer<vtkTIPW2Callback>::New();
		for (int k = 0; k < 3; k++)
		{
			myCallback->origin[k] = origin_center[k];
			if (k == 0)
				myCallback->normal[k] = mPipeLineObjProp.plane_propData.m_plane_normal.x;
			else if (k == 1)
				myCallback->normal[k] = mPipeLineObjProp.plane_propData.m_plane_normal.y;
			else if (k == 2)
				myCallback->normal[k] = mPipeLineObjProp.plane_propData.m_plane_normal.z;
		}
		myCallback->Plane = plane;
		planeWidget->AddObserver(vtkCommand::InteractionEvent, myCallback);
	}
}

void FilterClip::init_plane()
{
	func_init_plane();
}

void FilterClip::close_planeWidget()
{
	planeWidget->Off();
}

void FilterClip::func_generate_actors()
{
	if (mclipDataSet == NULL)
	{
		mclipDataSet = vtkSmartPointer<vtkClipDataSet>::New();
		
	}
	mclipDataSet->SetClipFunction(plane);
	mclipDataSet->SetInputData(mBaseGrid);
	
	if (mPipeLineObjProp.plane_propData.flag_insideOut)
		mclipDataSet->InsideOutOn();
	else
		mclipDataSet->InsideOutOff();
	mclipDataSet->Update();
	
	vtkSmartPointer<vtkUnstructuredGrid> tep_dataset = vtkSmartPointer<vtkUnstructuredGrid>::New();
	tep_dataset->DeepCopy(mclipDataSet->GetOutput());
	if (m_glphMapper == NULL)
	{
		m_glphMapper = vtkSmartPointer<vtkDataSetMapper>::New();
	}
	m_glphMapper->SetInputData(tep_dataset);
	mPipelineDataSet = (vtkDataSet*)tep_dataset;
	if (selectActor == NULL)
	{
		selectActor = vtkSmartPointer<vtkActor>::New();
		selectActor->SetBackfaceProperty(mBackFaceProp);
	}
	selectActor->SetMapper(m_glphMapper);
	
}

void FilterClip::generate_actors()
{
	init_plane(); 
	func_generate_actors();
	/*slot_changeOrigins();
	slot_changeNormals();*/
	if(!flag_exist_actors)
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

void FilterClip::slot_changeOrigins()
{
	if (plane != NULL)
		plane->SetOrigin(mPipeLineObjProp.plane_propData.m_plane_origin.x, mPipeLineObjProp.plane_propData.m_plane_origin.y, mPipeLineObjProp.plane_propData.m_plane_origin.z);
	if (clipRep != NULL)
	{
		clipRep->SetOrigin(plane->GetOrigin());
	}
}

void FilterClip::slot_changeNormals()
{
	if (plane != NULL)
		plane->SetNormal(mPipeLineObjProp.plane_propData.m_plane_normal.x, mPipeLineObjProp.plane_propData.m_plane_normal.y, mPipeLineObjProp.plane_propData.m_plane_normal.z);
	if (clipRep != NULL)
	{
		clipRep->SetNormal(plane->GetNormal());
	}
}

void FilterClip::slot_changeCameraNormals()
{
	if (clipRep != NULL)
		clipRep->SetNormalToCamera();
	if (plane != NULL)
		plane->SetNormal(clipRep->GetNormal());
}

void FilterClip::SetCenter(double x, double y, double z)
{
	origin_center[0] = x;
	origin_center[1] = y;
	origin_center[2] = z;
}

void FilterClip::SetNormal(double x, double y, double z)
{
	normal[0] = x;
	normal[1] = y;
	normal[2] = z;
}
