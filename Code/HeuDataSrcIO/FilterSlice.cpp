#include "FilterSlice.h"
#include "qdebug.h"

FilterSlice::FilterSlice(const QString& dataFile, PipelineObject * parent)
:mDataFile(dataFile)
{
	SetObjParent(parent);
	scalarBar_widgetMap.clear();
	plane_interactor = NULL;
	planeWidget = NULL;
	cutter = NULL;
	selectActor = NULL;
	outline = NULL;
	outlineActor = NULL;
	outlineMapper = NULL;
	sliceRep = NULL;
	plane = NULL;
	myCallback = NULL;
	slinceoutlineMapper = NULL;
	SetNormal(1, 0, 0);
	if (GetObjParent() != NULL)
	{
		mBaseGrid = GetObjParent()->mBaseGrid;
		mPipeLineObjProp = GetObjParent()->mPipeLineObjProp;
		init_subObject_count();
		GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_slice++;
	}
	mPipeLineObjProp.plane_propData.flag_plane = true;
	mBaseGrid->GetCenter(origin_center);
	QString tep_filter_str;
	mPipeLineObjProp.plane_propData.filterName = "Slice" + tep_filter_str.setNum(GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_slice, 10);
	mPipeLineObjProp.plane_propData.m_plane_origin.x = origin_center[0];
	mPipeLineObjProp.plane_propData.m_plane_origin.y = origin_center[1];
	mPipeLineObjProp.plane_propData.m_plane_origin.z = origin_center[2];
	mPipeLineObjProp.plane_propData.m_plane_normal.x = 1;
	mPipeLineObjProp.plane_propData.m_plane_normal.y = 0;
	mPipeLineObjProp.plane_propData.m_plane_normal.z = 0;
	mPipeLineObjProp.pipelineObj_base_propData.fileName = "SliceFilter";
	mPipeLineObjProp.pipelineObj_base_propData.file_xh = 0;
	mPipeLineObjProp.pipelineObj_base_propData.fileName = GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.fileName;
	SetObjectType(dSlice_DataSource);
	GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_total_child++;
}

FilterSlice::~FilterSlice()
{
	if (GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_total_child>0)
		GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.count_total_child--;
}

QString FilterSlice::GetName() const
{
	QString tep_str;
	return mPipeLineObjProp.plane_propData.filterName;
}

void FilterSlice::plane_visible(bool flag)
{
	if (flag)
	{
		if (outlineActor != NULL)
		{
			outlineActor->VisibilityOn();
		}
		if (planeWidget != NULL)
		{
			planeWidget->Off();
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

void FilterSlice::get_slice_pos_data(double tep_val[6])
{
	plane->GetOrigin(&tep_val[0]);
	plane->GetNormal(&tep_val[3]);
}

void FilterSlice::func_init_plane()
{
	if (plane == NULL)
		plane = vtkPlane::New();

	//add for combine animation
	plane->SetOrigin(mPipeLineObjProp.plane_propData.m_plane_origin.x, mPipeLineObjProp.plane_propData.m_plane_origin.y, mPipeLineObjProp.plane_propData.m_plane_origin.z);//sliceRep->SetOrigin(origin_center);
	plane->SetNormal(mPipeLineObjProp.plane_propData.m_plane_normal.x, mPipeLineObjProp.plane_propData.m_plane_normal.y, mPipeLineObjProp.plane_propData.m_plane_normal.z); //sliceRep->SetNormal(normal);
	//add for combine animation
	if (!flag_exist_actors)
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
	if (sliceRep == NULL)
	{
		sliceRep = vtkSmartPointer<vtkImplicitPlaneRepresentation>::New();
		sliceRep->SetPlaceFactor(1.0);//±ÈÀýÏµÊý
		sliceRep->SetOutlineTranslation(0);
		sliceRep->GetPlaneProperty()->SetOpacity(0.4);
	}
	sliceRep->SetOrigin(mPipeLineObjProp.plane_propData.m_plane_origin.x, mPipeLineObjProp.plane_propData.m_plane_origin.y, mPipeLineObjProp.plane_propData.m_plane_origin.z);//sliceRep->SetOrigin(origin_center);
	sliceRep->SetNormal(mPipeLineObjProp.plane_propData.m_plane_normal.x, mPipeLineObjProp.plane_propData.m_plane_normal.y, mPipeLineObjProp.plane_propData.m_plane_normal.z); //sliceRep->SetNormal(normal);
	sliceRep->PlaceWidget(mBaseGrid->GetBounds());
	sliceRep->SetPlane(plane);//sliceRep->GetPlane(plane);
	sliceRep->UpdatePlacement();
	
	if (planeWidget == NULL)
	{
		planeWidget = vtkSmartPointer<vtkImplicitPlaneWidget2>::New();
		//planeWidget->SetRepresentation(sliceRep);
		planeWidget->SetInteractor(plane_interactor);
		planeWidget->EnabledOn();
	}
	planeWidget->SetRepresentation(sliceRep);
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
		myCallback->Actor = selectActor;
		planeWidget->AddObserver(vtkCommand::InteractionEvent, myCallback);
	}
}

void FilterSlice::func_generate_actors()
{
	if (cutter == NULL)
	{
		cutter = vtkSmartPointer<vtkCutter>::New();
		
	}
	cutter->SetCutFunction(plane);
	cutter->SetInputData(mBaseGrid);
	if (slinceoutlineMapper == NULL)
	{
		slinceoutlineMapper = vtkSmartPointer<vtkDataSetMapper>::New();
	}
	cutter->Update();
	vtkSmartPointer<vtkUnstructuredGrid> tep_dataset = vtkSmartPointer<vtkUnstructuredGrid>::New();
	tep_dataset->DeepCopy(cutter->GetOutput());
	slinceoutlineMapper->SetInputData(tep_dataset);
	mPipelineDataSet = (vtkDataSet*)tep_dataset;
	//func_update_xyzColorColumn();
	if (selectActor == NULL)
	{
		selectActor = vtkSmartPointer<vtkActor>::New();
		selectActor->SetBackfaceProperty(mBackFaceProp);
	}
	selectActor->SetMapper(slinceoutlineMapper);
}

void FilterSlice::init_plane()
{
	func_init_plane();
}

void FilterSlice::close_planeWidget()
{
	planeWidget->Off();
}

void FilterSlice::generate_actors()
{
	init_plane();
	func_generate_actors();
	/*slot_changeOrigins();
	slot_changeNormals();*/
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

void FilterSlice::slot_changeOrigins()
{
	if (plane != NULL)
		plane->SetOrigin(mPipeLineObjProp.plane_propData.m_plane_origin.x, mPipeLineObjProp.plane_propData.m_plane_origin.y, mPipeLineObjProp.plane_propData.m_plane_origin.z);
	if (sliceRep != NULL)
		sliceRep->SetOrigin(mPipeLineObjProp.plane_propData.m_plane_origin.x, mPipeLineObjProp.plane_propData.m_plane_origin.y, mPipeLineObjProp.plane_propData.m_plane_origin.z);
}

void FilterSlice::slot_changeNormals()
{
	if (plane != NULL)
		plane->SetNormal(mPipeLineObjProp.plane_propData.m_plane_normal.x, mPipeLineObjProp.plane_propData.m_plane_normal.y, mPipeLineObjProp.plane_propData.m_plane_normal.z);
	if (sliceRep != NULL)
		sliceRep->SetNormal(mPipeLineObjProp.plane_propData.m_plane_normal.x, mPipeLineObjProp.plane_propData.m_plane_normal.y, mPipeLineObjProp.plane_propData.m_plane_normal.z);
}
void FilterSlice::slot_changeCameraNormals()
{
	if (sliceRep!=NULL)
		sliceRep->SetNormalToCamera();
	if (plane != NULL)
		plane->SetNormal(sliceRep->GetNormal());
}

void FilterSlice::SetCenter(double x, double y, double z)
{
	origin_center[0] = x;
	origin_center[1] = y;
	origin_center[2] = z;
}

void FilterSlice::SetNormal(double x, double y, double z)
{
	normal[0] = x;
	normal[1] = y;
	normal[2] = z;
}
