#include "PipelineObjPlotForm.h"
#include "ui_PipelineObjPlotForm.h"
#include "NumericSrcObject.h"
#include <QKeyEvent>
#include "PickAreaInteractorStyle.h"
#include "qmessagebox.h"
#include "vtkCoordinate.h"
#include "myInteractorStyle.h"
#include "qmath.h"
#include <vtkVersionMacros.h>

#ifdef Q_OS_WIN
#include "vtkAVIWriter.h"
#endif

vtkStandardNewMacro(PickAreaInteractorStyle);
vtkStandardNewMacro(rubberZoomInteractorStyle);
vtkStandardNewMacro(actorInteractorStyle);
vtkStandardNewMacro(actorMoveInteractorStyle);
vtkStandardNewMacro(MyInteractorStyle);
//vtkStandardNewMacro(disvtkPOVExporter);

PipelineObjPlotForm::PipelineObjPlotForm(QWidget *parent,QToolBar* toolBar) :
    QWidget(parent), ui(new Ui::PipelineObjPlotForm)
{
    ui->setupUi(this);
#if(VTK_MAJOR_VERSION < 9)
	renderWindow = ui->qvtkWidget->GetRenderWindow();
#else
	renderWindow = ui->qvtkWidget->renderWindow();
#endif



	flag_selfAdaptation = false;
	flag_render = false;
	flag_script_running = false;
	exportThread = NULL;
	//QString dispStr("D:/Vtk_Data_File/vtk-liao20170508/FSPL4-20171129/Case2 20170405/OutExl/Disp.Dat");
	//loadDispFile.Load(dispStr,"FLCTA");
	setWindowFlags(Qt::FramelessWindowHint);
	setContentsMargins(0, 0, 0, 0);
	flag_cur_viewInteraction = true;
	rubberZoom_style = NULL;
	actorROperate_style = NULL;
	actorMOperate_style = NULL;
	areaPick_style = NULL;
	consoleWidget_ptr = NULL;
	flag_refresh_forAni = false;
	init_saveImage_prop_data();
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
	mFontPath = QDir::currentPath();
	mFontPath += "/font/simhei.ttf";
	savePng_filename = "";
	flag_start_aniSave = false;
    avi_writer = NULL;
	aniSetting_data = NULL;
	max_ani_frames = 0;
	cur_ani_frame_xh = 0;
	cur_ani_dis_xh = 0;
	scalarBar_max[0] = 0.05;
	scalarBar_max[1]=0.45;
	scalarBar_min[0] = 0.05;
	scalarBar_min[1] = 0.45;
	scalarBar_list.clear();
	cur_camera = NULL;
	m_orientationMarkerWidget = NULL;
	m_orientationAxesActor = NULL;
	m_remarkWidget = NULL;
	m_remarkActor = NULL;
	m_remarkPresentation = NULL;
	count_scalarBar = 0;
	ani_thread = NULL;
	cgns_thread = NULL;
	vtk_mousePicker_connector = NULL;
    renderer = vtkSmartPointer<vtkRenderer>::New();

	renderWindow->AddRenderer(renderer);
	renderer->GlobalWarningDisplayOff();
//	renderWindow->Render();

	if (m_orientationAxesActor == NULL)
	{
		m_orientationAxesActor = vtkAxesActor::New();
	}
	m_orientationAxesActor->SetPosition(0, 0, 0);
	if (m_orientationMarkerWidget == NULL)
	{
		m_orientationMarkerWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
	}
	
	renderWindowInteractor = renderWindow->GetInteractor();
// 	renderWindowInteractor->SetRenderWindow(renderWindow);
// 	renderWindowInteractor->Initialize();
	mouse_style = MyInteractorStyle::New();//mouse_style = vtkInteractorStyleTrackballCamera::New();
	mouse_style->SetDefaultRenderer(renderer);
	renderWindowInteractor->SetInteractorStyle(mouse_style);
	connect(mouse_style, SIGNAL(sig_update_UndoReDoButton(int)), this, SLOT(slot_update_UndoReDoButton(int)));

	m_orientationMarkerWidget->SetDefaultRenderer(renderer);
	m_orientationMarkerWidget->SetOrientationMarker(m_orientationAxesActor);
	m_orientationMarkerWidget->SetInteractor(renderWindow->GetInteractor());
	m_orientationMarkerWidget->SetEnabled(1);
	m_orientationMarkerWidget->SetInteractive(0);
	m_orientationMarkerWidget->SetViewport(0.0, 0.0, 0.2, 0.2);
	m_orientationMarkerWidget->On();
	//renderWindowInteractor->Start();
	//renderWindow->Render();
	//add for pick
	pick_Data.flag_pickMode = -1;
	//pick_Data.frustum = NULL;
	pick_Data.pick_actor = NULL;
	pick_Data.pick_id = -1;
	pick_Data.pick_pipelineObj = NULL;
	pick_Data.pick_areaCell=NULL;
	pick_Data.pick_areaPoint=NULL;
	pick_Data.flag_draw_curve = false;
	pick_Data.pick_curve_data.clear();
	int tep_objId = -1;
	pick_Data.pickArea_objId =&tep_objId;
	QSpacerItem* horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	QHBoxLayout* hlay = new QHBoxLayout;
	hlay->addWidget(toolBar);
	hlay->addItem(horizontalSpacer);
	ui->verticalLayout_2->addLayout(hlay);
	for (int i = 0; i < 9;i++)
		pov_propData.disAndang[i] = 0;
	for (int i = 0; i < 3;i++)
		pov_propData.mOrigin[i] = 0;
	for (int i = 0; i < 3; i++)
		pov_propData.pos[i] = 0;
	connect(this, SIGNAL(sig_saveImage()), this, SLOT(slot_saveImage()));
	connect(this, SIGNAL(sig_saveAnimate()), this, SLOT(slot_saveAnimate()));
	
	renderer->SetGradientBackground(false);
	renderer->SetBackground(0.278, 0.533, 0.478);
 

	renderWindow->Render();
	renderWindowInteractor->Start();
}

void PipelineObjPlotForm::set_consoleWidget(consoleCmdDockWidget *tep_wid)
{
	consoleWidget_ptr = tep_wid;
}
void PipelineObjPlotForm::slot_update_remark(bool flag)
{
	if (m_remarkActor == NULL)
	{
		m_remarkActor = vtkSmartPointer<vtkTextActor>::New();
		m_remarkActor->GetTextProperty()->SetFontFamily(VTK_FONT_FILE);
		m_remarkActor->GetTextProperty()->SetFontFile(mFontPath.toLocal8Bit().data());
	}
	m_remarkActor->GetTextProperty()->SetFontSize(m_renderViewPara->remark_para.fontSize);
	m_remarkActor->GetTextProperty()->SetColor(m_renderViewPara->remark_para.fontColor.red() / 255.0,
		m_renderViewPara->remark_para.fontColor.green() / 255.0,
		m_renderViewPara->remark_para.fontColor.blue() / 255.0);
	m_remarkActor->SetInput(m_renderViewPara->remark_para.text.toStdString().c_str());

	if (m_remarkPresentation == NULL)
	{
		m_remarkPresentation = vtkSmartPointer<vtkTextRepresentation>::New();
		m_remarkPresentation->GetPositionCoordinate()->SetValue(m_renderViewPara->remark_para.position1[0], m_renderViewPara->remark_para.position1[1]);
		m_remarkPresentation->GetPosition2Coordinate()->SetValue(m_renderViewPara->remark_para.position2[0], m_renderViewPara->remark_para.position2[1]);
	}
	else if (getScriptRunning())
	{
		m_remarkPresentation->GetPositionCoordinate()->SetValue(m_renderViewPara->remark_para.position1[0], m_renderViewPara->remark_para.position1[1]);
		m_remarkPresentation->GetPosition2Coordinate()->SetValue(m_renderViewPara->remark_para.position2[0], m_renderViewPara->remark_para.position2[1]);
	}

	if (m_remarkWidget == NULL)
	{
		m_remarkWidget = vtkSmartPointer<vtkTextWidget>::New();
		m_remarkWidget->SetRepresentation(m_remarkPresentation);

		m_remarkWidget->SetInteractor(renderWindow->GetInteractor());
		m_remarkWidget->SetTextActor(m_remarkActor);
		m_remarkWidget->SelectableOff();
	}
	if (m_renderViewPara->remark_para.flag_remark)
		m_remarkWidget->On();
	else
		m_remarkWidget->Off();
	if (flag_render)
		renderWindow->Render();
}
void PipelineObjPlotForm::func_saveRemarkPosition()
{
	if (m_remarkWidget == NULL)
		return;
	vtkTextRepresentation* tep_pos = (vtkTextRepresentation*)m_remarkWidget->GetRepresentation();
	m_renderViewPara->remark_para.position1[0] = tep_pos->GetPositionCoordinate()->GetValue()[0];
	m_renderViewPara->remark_para.position1[1] = tep_pos->GetPositionCoordinate()->GetValue()[1];
	m_renderViewPara->remark_para.position2[0] = tep_pos->GetPosition2Coordinate()->GetValue()[0];
	m_renderViewPara->remark_para.position2[1] = tep_pos->GetPosition2Coordinate()->GetValue()[1];
}
void PipelineObjPlotForm::slot_update_remark_fontColor()
{
	m_remarkActor->GetTextProperty()->SetColor(m_renderViewPara->remark_para.fontColor.red() / 255.0,
		m_renderViewPara->remark_para.fontColor.green() / 255.0,
		m_renderViewPara->remark_para.fontColor.blue() / 255.0);
	if (flag_render)
		renderWindow->Render();
}

void PipelineObjPlotForm::slot_update_remark_text()
{
	m_remarkActor->SetInput(m_renderViewPara->remark_para.text.toStdString().c_str());
	if (flag_render)
		renderWindow->Render();
}

void PipelineObjPlotForm::slot_change_global_view_prop()
{
    if(m_renderViewPara->back_color_type==0)
    {
		renderer->GradientBackgroundOff();
		renderer->SetBackground(m_renderViewPara->back_color1.red() / 255.0, m_renderViewPara->back_color1.green() / 255.0, m_renderViewPara->back_color1.blue() / 255.0);
    }
	else if (m_renderViewPara->back_color_type == 1)
    {		
		renderer->SetBackground(m_renderViewPara->back_color1.red() / 255.0, m_renderViewPara->back_color1.green() / 255.0, m_renderViewPara->back_color1.blue() / 255.0);
		renderer->SetBackground2(m_renderViewPara->back_color2.red() / 255.0, m_renderViewPara->back_color2.green() / 255.0, m_renderViewPara->back_color2.blue() / 255.0);
		renderer->GradientBackgroundOn();
    }
	if (m_renderViewPara->flag_axesVisible)
		m_orientationMarkerWidget->On();
	else
		m_orientationMarkerWidget->Off();
	if (m_renderViewPara->flag_camera_parallel)
		renderWindow ->GetRenderers()->GetFirstRenderer()->GetActiveCamera()->ParallelProjectionOn();
	else
		renderWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera()->ParallelProjectionOff();
	slot_update_remark(m_renderViewPara->remark_para.flag_remark);
	renderWindow->Render();
}

PipelineObjPlotForm::~PipelineObjPlotForm()
{
    delete ui;
	if (rubberZoom_style != NULL)
	{
		rubberZoom_style->Delete();
		rubberZoom_style = NULL;
	}
	if (actorMOperate_style != NULL)
	{
		actorMOperate_style->Delete();
		actorMOperate_style = NULL;
	}
	if (actorROperate_style != NULL)
	{
		actorROperate_style->Delete();
		actorROperate_style = NULL;
	}
	if (areaPick_style != NULL)
	{
		areaPick_style->Delete();
		areaPick_style = NULL;
	}
}

void PipelineObjPlotForm::set_saveImage_filename(QString tep_name)
{
	savePng_filename = tep_name;
}

void PipelineObjPlotForm::set_cur_ani_frame_xh(int val)
{
	cur_ani_frame_xh = val;
}

void PipelineObjPlotForm::slot_refreshView_fromMainWin(QList<PipelineObject*> mPipelineObjs)
{
	slot_refreshView(mPipelineObjs);
}
void PipelineObjPlotForm::func_set_disActors(QList<PipelineObject*> mPipelineObjs)
{
	pov_propData.mVecActors.clear();
	for (int i = 0; i < mPipelineObjs.count(); i++)
	{
		/*if (mPipelineObjs[i]->mPipeLineObjProp.aniDisplace_propData.flag_AniDisplaceFile
		&& mPipelineObjs[i]->mPipeLineObjProp.aniDisplace_propData.flag_exeDisplace)*/
		if (QFileInfo(mPipelineObjs[i]->mPipeLineObjProp.pipelineObj_base_propData.fileName).suffix() == "obj")
		{
			for (int j = 0; j < mPipelineObjs[i]->mVecActors.count(); j++)
			{
				pov_propData.mVecActors.append(mPipelineObjs[i]->mVecActors.at(j));
				mPipelineObjs[i]->mVecActors.at(j)->SetPosition(pov_propData.pos);
			}
		}
	}
}
void PipelineObjPlotForm::AddActors(PipelineObject* pipeObject)
{
	if (pipeObject->GetObjectType() == dNumeric_DataSource)
	{
		bool flag = pipeObject->func_update_mBaseGrid();
		if (!pipeObject->flag_exist_actors)
		{
			pipeObject->generate_actors();
			if (pipeObject->model_ptr != NULL)
			{
				renderer->AddViewProp(pipeObject->model_ptr);
			}
			else
			{
				for (int i = 0; i < pipeObject->mVecActors.count(); i++)
				{
					renderer->AddViewProp(pipeObject->mVecActors[i]);
				}
				generate_scalarBarWidget((NumericSrcObject*)pipeObject);
			}
		}
		else
		{
			pipeObject->generate_actors(); 
		}
		if (flag)//add for multi zone
		{
			for (int i = 0; i < cur_mPipelineObjs.count(); i++)
			{
				if (cur_mPipelineObjs.at(i)->GetObjParent() == pipeObject)
				{
					cur_mPipelineObjs.at(i)->mBaseGrid = pipeObject->mBaseGrid;
				}
			}
		}
	}
	else if (pipeObject->GetObjectType() == dSlice_DataSource)
	{
		func_addActors<FilterSlice>(pipeObject);
	}	
	else if (pipeObject->GetObjectType() == dClip_DataSource)
	{
		func_addActors<FilterClip>(pipeObject);
	}
	else if (pipeObject->GetObjectType() == dContour_DataSource)
	{
		func_addActors<FilterContour>(pipeObject);
	}
	else if (pipeObject->GetObjectType() == dVector_DataSource)
	{
		FilterVector *tep_pipelineObj = (FilterVector*)pipeObject;
		if (!pipeObject->flag_exist_actors)
		{
			int flag_scalar_onPoint = -1;
			int flag_vector_onPoint = -1;
			if (pipeObject->mPipeLineObjProp.vectorFilter_propData.scalarColName.indexOf("_cell000") >= 0)
				flag_scalar_onPoint = 0;
			else if (pipeObject->mPipeLineObjProp.vectorFilter_propData.scalarColName != "None")
				flag_scalar_onPoint = 1;
			if (pipeObject->mPipeLineObjProp.vectorFilter_propData.vectorColName.indexOf("_cell000") >= 0)
				flag_vector_onPoint = 0;
			else if (pipeObject->mPipeLineObjProp.vectorFilter_propData.vectorColName != "None")
				flag_vector_onPoint = 1;
			if ((flag_vector_onPoint == -1) && (flag_scalar_onPoint == -1))
				return;
			tep_pipelineObj->generate_actors();
			for (int i = 0; i < tep_pipelineObj->mVecActors.count(); i++)
			{
				renderer->AddViewProp(tep_pipelineObj->mVecActors[i]);
			}
		}
		else
		{
			tep_pipelineObj->generate_actors();
		}
	}
	else if (pipeObject->GetObjectType() == dReflection_DataSource)
	{
		func_addActors<FilterReflection>(pipeObject);
	}
	else if (pipeObject->GetObjectType() == dCalculator_DataSource)
	{
		func_addActors<FilterCalculator>(pipeObject);
	}
	else if (pipeObject->GetObjectType() == dSmooth_DataSource)
	{
		func_addActors<FilterSmooth>(pipeObject);
	}
	else if (pipeObject->GetObjectType() == dStreamLine_DataSource)
	{
		func_addActors<FilterStreamLine>(pipeObject);
	}
	
	if (pipeObject->mPipeLineObjProp.aniDisplace_propData.flag_AniDisplaceFile)
	{
		pov_propData.mOrigin[0] = pipeObject->mPipeLineObjProp.aniDisplace_propData.m_origin.x;
		pov_propData.mOrigin[1] = pipeObject->mPipeLineObjProp.aniDisplace_propData.m_origin.y;
		pov_propData.mOrigin[2] = pipeObject->mPipeLineObjProp.aniDisplace_propData.m_origin.z;
		if (pipeObject->mPipeLineObjProp.aniDisplace_propData.flag_inputGravity)
		{
			pov_propData.disAndang[0] = pipeObject->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Surge_GX;
			pov_propData.disAndang[1] = pipeObject->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Sway_GY;
			pov_propData.disAndang[2] = pipeObject->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Heave_GZ;
			/*pov_propData.disAndang[4] = (pipeObject->mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Pitch_Gyb - pipeObject->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Pitch_Gyb)*57.32;
			pov_propData.disAndang[3] = (pipeObject->mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Roll_Gxb - pipeObject->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Roll_Gxb)*57.32;
			pov_propData.disAndang[5] = (pipeObject->mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Yaw_Gzb - pipeObject->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Yaw_Gzb)*57.32;*/		
			pov_propData.disAndang[4] = (-pipeObject->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Pitch_Gyb)*57.32;
			pov_propData.disAndang[3] = (-pipeObject->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Roll_Gxb)*57.32;
			pov_propData.disAndang[5] = (-pipeObject->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Yaw_Gzb)*57.32; 
			pov_propData.disAndang[6] = pipeObject->mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Surge_GX;
			pov_propData.disAndang[7] = pipeObject->mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Sway_GY;
			pov_propData.disAndang[8] = pipeObject->mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Heave_GZ;
		}
		else
		{
			pov_propData.disAndang[0] = pipeObject->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Surge_GX;
			pov_propData.disAndang[1] = pipeObject->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Sway_GY;
			pov_propData.disAndang[2] = pipeObject->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Heave_GZ;
			pov_propData.disAndang[3] = pipeObject->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Roll_Gxb;
			pov_propData.disAndang[4] = pipeObject->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Pitch_Gyb;
			pov_propData.disAndang[5] = pipeObject->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Yaw_Gzb;
			pov_propData.disAndang[6] = pipeObject->mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Surge_GX;
			pov_propData.disAndang[7] = pipeObject->mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Sway_GY;
			pov_propData.disAndang[8] = pipeObject->mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Heave_GZ;
		}
	}
	pipeObject->func_update_displacement(cur_ani_frame_xh);
}

void PipelineObjPlotForm::slot_init_sliceFilter_plotForm(FilterSlice *pipeObj)
{
	pipeObj->set_plane_interactor(renderWindow->GetInteractor());
	pipeObj->init_plane();
	//if (flag_render)
	renderWindow->Render();
}

void PipelineObjPlotForm::slot_init_clipFilter_plotForm(FilterClip *pipeObj)
{
	pipeObj->set_plane_interactor(renderWindow->GetInteractor());
	pipeObj->init_plane();
	//if (flag_render)
		renderWindow->Render();
}

void PipelineObjPlotForm::slot_refreshViewAni(int tep_cur_frame_xh)
{
	flag_refresh_forAni = true;
	slot_refreshView(g_ani_objList);
	LoadActionFile* ptr = new LoadActionFile;
	int tep_max_dis_num=ptr->func_get_max_num_displace(g_ani_objList);
	if (max_ani_frames > 1)
	{
		cur_ani_frame_xh = tep_cur_frame_xh;
		if (tep_cur_frame_xh < tep_max_dis_num)
			cur_ani_dis_xh = tep_cur_frame_xh;
	}
	else
	{
		if (tep_cur_frame_xh < tep_max_dis_num)
			cur_ani_dis_xh = tep_cur_frame_xh;
	}
	emit sig_update_countAniFrame(cur_ani_frame_xh, max_ani_frames);
	/*if ((cur_ani_frame_xh+1)<max_ani_frames)
		cur_ani_frame_xh++;*/
	g_aniThread_mutex.unlock();
}

void PipelineObjPlotForm::init_saveImage_prop_data()
{
	/*saveImage_prop_data.image_wid =ui->qvtkWidget->width();
	saveImage_prop_data.image_hei = ui->qvtkWidget->height();*/
	saveImage_prop_data.image_wid = renderWindow->GetScreenSize()[0];
	saveImage_prop_data.image_hei = renderWindow->GetScreenSize()[1];
	saveImage_prop_data.flag_saveImage = false;
	saveImage_prop_data.saveImage_fileName = "";
	saveImage_prop_data.flag_savePov = false;
	saveImage_prop_data.savePov_fileName = "";
}

void PipelineObjPlotForm::set_saveImage_prop_data(saveImage_prop tep_prop)
{
	saveImage_prop_data = tep_prop;
	if (tep_prop.flag_savePov)
		func_set_disActors(g_ani_objList);
}

void PipelineObjPlotForm::slot_saveImage()
{
	while (renderWindow->CheckInRenderStatus());
	QThread::msleep(100);
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
	QFileInfo tep_info(saveImage_prop_data.saveImage_fileName);
	if (saveImage_prop_data.saveImage_fileName != "")
	{
		QString tep_path=tep_info.absolutePath();
		QDir tep_dir(tep_path);
		if (!tep_dir.exists())
			tep_dir.mkpath(tep_path);
	}
	if (saveImage_prop_data.flag_saveImage && (saveImage_prop_data.saveImage_fileName != ""))
	{
		vtkSmartPointer<vtkWindowToImageFilter> filter = vtkSmartPointer<vtkWindowToImageFilter>::New();
		filter->SetInput(renderWindow);
		filter->Modified();
		if (tep_info.suffix() == "png")
		{
			vtkSmartPointer<vtkPNGWriter> pngWriter = vtkSmartPointer<vtkPNGWriter>::New();
			pngWriter->SetFileName(saveImage_prop_data.saveImage_fileName.toLocal8Bit().data());
			if ((saveImage_prop_data.image_wid > 0) && (saveImage_prop_data.image_hei > 0))
			{
				vtkSmartPointer<vtkImageResize> resize = vtkSmartPointer<vtkImageResize>::New();
				resize->SetInputConnection(filter->GetOutputPort());
				//resize->SetOutputDimensions(saveImage_prop_data.image_wid, saveImage_prop_data.image_hei, -1);
				resize->Update();
				pngWriter->SetInputConnection(resize->GetOutputPort());
			}
			else
				pngWriter->SetInputConnection(filter->GetOutputPort());
			pngWriter->Write();
		}
		else if (tep_info.suffix() == "jpg")
		{
			vtkSmartPointer<vtkJPEGWriter> jpegWriter = vtkSmartPointer<vtkJPEGWriter>::New();
			jpegWriter->SetFileName(saveImage_prop_data.saveImage_fileName.toLocal8Bit().data());
			if ((saveImage_prop_data.image_wid > 0) && (saveImage_prop_data.image_hei > 0))
			{
				vtkSmartPointer<vtkImageResize> resize = vtkSmartPointer<vtkImageResize>::New();
				resize->SetInputConnection(filter->GetOutputPort());
				//resize->SetOutputDimensions(saveImage_prop_data.image_wid, saveImage_prop_data.image_hei, -1);
				resize->Update();
				jpegWriter->SetInputConnection(resize->GetOutputPort());
			}
			else
				jpegWriter->SetInputConnection(filter->GetOutputPort());
			jpegWriter->Write();
		}
		else if (tep_info.suffix() == "bmp")
		{
			vtkSmartPointer<vtkBMPWriter> bmpWriter = vtkSmartPointer<vtkBMPWriter>::New();
			bmpWriter->SetFileName(saveImage_prop_data.saveImage_fileName.toLocal8Bit().data());
			if ((saveImage_prop_data.image_wid > 0) && (saveImage_prop_data.image_hei > 0))
			{
				vtkSmartPointer<vtkImageResize> resize = vtkSmartPointer<vtkImageResize>::New();
				resize->SetInputConnection(filter->GetOutputPort());
				//resize->SetOutputDimensions(saveImage_prop_data.image_wid, saveImage_prop_data.image_hei, -1);
				resize->Update();
				bmpWriter->SetInputConnection(resize->GetOutputPort());
			}
			else
				bmpWriter->SetInputConnection(filter->GetOutputPort());
			bmpWriter->Write();
		}
		else if (tep_info.suffix() == "eps")
		{
			vtkSmartPointer<vtkImageResize> resize = vtkSmartPointer<vtkImageResize>::New();
			resize->SetInputConnection(filter->GetOutputPort());
			//resize->SetOutputDimensions(saveImage_prop_data.image_wid, saveImage_prop_data.image_hei, -1);
			resize->Update();
			vtkSmartPointer<vtkGL2PSExporter> exp = vtkSmartPointer<vtkGL2PSExporter>::New();
			exp->SetRenderWindow(renderWindow);
			exp->SetFileFormatToEPS();
			exp->SetSortToSimple();
			exp->DrawBackgroundOn();
			exp->Write3DPropsAsRasterImageOff();
			exp->SetTextAsPath(true);
			exp->SetFilePrefix(saveImage_prop_data.saveImage_fileName.toLocal8Bit().data());
			exp->SetCompress(0);
			exp->Write();
		}		
		saveImage_prop_data.flag_saveImage = false;
		saveImage_prop_data.saveImage_fileName = "";
		QThread::usleep(100);
	}
	if (saveImage_prop_data.flag_savePov && (saveImage_prop_data.savePov_fileName != ""))
	{
		vtkSmartPointer<disvtkPOVExporter> povexp =vtkSmartPointer<disvtkPOVExporter>::New();
		povexp->SetRenderWindow(renderWindow);
		povexp->set_origin(pov_propData.mOrigin);
		povexp->set_disPosition(pov_propData.pos);
		povexp->set_disAndangle(pov_propData.disAndang);
		povexp->set_disActors(pov_propData.mVecActors);
		/*vtkActorCollection *tep_collection = renderer->GetActors();
		if (tep_collection != NULL)
		{
			int tep_num = tep_collection->GetNumberOfItems();
			for (int i = 0; i < tep_num; i++)
			{
				vtkActor* tep_actor=tep_collection->GetNextActor();
				if (tep_actor != NULL)
					tep_actor->Modified();
			}
		}*/
		povexp->SetFileName(saveImage_prop_data.savePov_fileName.toLocal8Bit().data());
		povexp->Update();
		povexp->Write();
		saveImage_prop_data.flag_savePov = false;
		saveImage_prop_data.savePov_fileName = "";
		QThread::usleep(100);
	}
}

void PipelineObjPlotForm::slot_delPipelineObj(PipelineObject* pipeObj)
{
	if (pipeObj == NULL)
		return;
	if (pipeObj->flag_exist_actors)
	{
		QString tep_colorColumn, tep;
		if (pipeObj->GetObjectType() == dNumeric_DataSource)
		{
			QMap<QString, vtkScalarBarWidget*>::iterator it;
			for (it = pipeObj->scalarBar_widgetMap.begin(); it != pipeObj->scalarBar_widgetMap.end(); it++)
			{
				it.value()->Off();
				it.value()->Delete();
			}
		}
		else
		{
			if (pipeObj->GetObjectType() == dCalculator_DataSource)
			{
				if (pipeObj->GetObjParent()->scalarBar_widgetMap.contains("CalculatorResult"))
				{
					pipeObj->GetObjParent()->scalarBar_widgetMap["CalculatorResult"]->Off();
					pipeObj->GetObjParent()->scalarBar_widgetMap["CalculatorResult"]->Delete();
				}
				if (pipeObj->GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.contains("CalculatorResult"))
				{
					pipeObj->GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.remove("CalculatorResult");
				}
				if (pipeObj->GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.contains("CalculatorResult"))
				{
					pipeObj->GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.remove("CalculatorResult");
				}
				if (pipeObj->GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap.contains("CalculatorResult"))
				{
					pipeObj->GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap.remove("CalculatorResult");
				}
				if (pipeObj->GetObjParent()->scalarBar_positionMap.contains("CalculatorResult"))
				{
					pipeObj->GetObjParent()->scalarBar_positionMap.remove("CalculatorResult");
				}
			}
			else
			{
				if (pipeObj->GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap[pipeObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle] && pipeObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible)
				{
					pipeObj->GetObjParent()->scalarBar_widgetMap[pipeObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle]->Off();
					pipeObj->GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert(pipeObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle, false);
				}
			}
			pipeObj->GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors = true;
			if (pipeObj->GetObjParent()->flag_exist_actors)
			{
				pipeObj->GetObjParent()->ShowActor();
				if (pipeObj->GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap[pipeObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle] && pipeObj->GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible)
					pipeObj->GetObjParent()->scalarBar_widgetMap[pipeObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle]->On();
			}
		}
		for (int i = 0; i < pipeObj->mVecActors.count(); i++)
		{
			renderer->RemoveViewProp(pipeObj->mVecActors[i]);
		}
		if (pipeObj->model_ptr != NULL)
		{
			renderer->RemoveViewProp(pipeObj->model_ptr);
		}
		if (pipeObj->get_axesActor() != NULL)
		{
			renderer->RemoveViewProp(pipeObj->get_axesActor());
		}
	}
	if (flag_render)
		renderWindow->Render();
}

void PipelineObjPlotForm::slot_refreshView(QList<PipelineObject*> mPipelineObjs)
{
	cur_mPipelineObjs.clear();
	cur_mPipelineObjs.append(mPipelineObjs);
	if (!flag_refresh_forAni)
	{
		if (cur_camera == NULL)
		{
			vtkActorCollection* actor_list = renderer->GetActors();
			if (actor_list != NULL)
			{
				if (actor_list->GetNumberOfItems()>0)
					cur_camera = renderer->GetActiveCamera();
			}
		}		
		
		for (int i = 0; i < mPipelineObjs.count(); i++)
		{
			PipelineObject* pipeObj = mPipelineObjs.at(i);
			if (pipeObj->GetObjectType() == dVector_DataSource)
			{
				int flag_scalar_onPoint = -1;
				int flag_vector_onPoint = -1;
				if (pipeObj->mPipeLineObjProp.vectorFilter_propData.scalarColName.indexOf("_cell000") >= 0)
					flag_scalar_onPoint = 0;
				else if (pipeObj->mPipeLineObjProp.vectorFilter_propData.scalarColName != "None")
					flag_scalar_onPoint = 1;
				if (pipeObj->mPipeLineObjProp.vectorFilter_propData.vectorColName.indexOf("_cell000") >= 0)
					flag_vector_onPoint = 0;
				else if (pipeObj->mPipeLineObjProp.vectorFilter_propData.vectorColName != "None")
					flag_vector_onPoint = 1;
				if ((flag_scalar_onPoint == -1) && (flag_vector_onPoint == -1))
					continue;
			}
			else if (pipeObj->GetObjectType() == dStreamLine_DataSource)
			{
				if (pipeObj->mPipeLineObjProp.streamLineFilter_propData.vectorColName == "None")
					continue;
			}
			
			AddActors(pipeObj);
			emit sig_treeItem_disable(pipeObj);
			if (pipeObj->GetObjectType() == dNumeric_DataSource)
				slot_update_scalarBar(pipeObj);
			if (pipeObj->GetObjectType() == dSlice_DataSource)
			{
				((FilterSlice*)pipeObj)->plane_visible(pipeObj->mPipeLineObjProp.plane_propData.flag_plane);
			}
			else if (pipeObj->GetObjectType() == dClip_DataSource)
			{
				((FilterClip*)pipeObj)->plane_visible(pipeObj->mPipeLineObjProp.plane_propData.flag_plane);
			}
			else if (pipeObj->GetObjectType() == dCalculator_DataSource)
			{
				if (!(NumericSrcObject*)(pipeObj->GetObjParent())->scalarBar_widgetMap.contains("CalculatorResult"))
					generate_scalarBarWidget_calculatorResult((NumericSrcObject*)(pipeObj->GetObjParent()), pipeObj->mPipeLineObjProp.calculatorFilter_propData.resultName);
				//((FilterClip*)pipeObj)->plane_visible(pipeObj->mPipeLineObjProp.plane_propData.flag_plane);
			}
			slot_update_cubeAxes(pipeObj);
			/*if (getScriptRunning() && pipeObj->mPipeLineObjProp.pipelineObj_base_propData.flag_opacityMap)
				pipeObj->func_opacityMap();*/
		}
		if (getScriptRunning())
			emit sig_script_update_currentObj(mPipelineObjs.last());
		if (cur_camera != NULL)
			renderer->SetActiveCamera(cur_camera);
		else
		{
			if (!getScriptRunning())
				renderer->ResetCamera();
			else
				setScriptRunning(false);
		}
	}
	else
	{
		for (int i = 0; i < mPipelineObjs.count(); i++)
		{
			PipelineObject* pipeObj = mPipelineObjs.at(i);
			//pipeObj->generate_actors();
			AddActors(pipeObj);
			if (pipeObj->GetObjectType() == dNumeric_DataSource)//&&(pipeObj->mPipeLineObjProp.pipelineObj_base_propData.file_xh==0)
			{
				slot_update_scalarBar(pipeObj);
				if (pipeObj->mPipeLineObjProp.pipelineObj_base_propData.fileList.count()>1)
					func_combine_obj_model(pipeObj);
			}
			else if (pipeObj->GetObjectType() == dSlice_DataSource)
			{
				((FilterSlice*)pipeObj)->plane_visible(pipeObj->mPipeLineObjProp.plane_propData.flag_plane);
			}
			else if (pipeObj->GetObjectType() == dClip_DataSource)
			{
				((FilterClip*)pipeObj)->plane_visible(pipeObj->mPipeLineObjProp.plane_propData.flag_plane);
			}
			else if (pipeObj->GetObjectType() == dCalculator_DataSource)
			{
				if (!(NumericSrcObject*)(pipeObj->GetObjParent())->scalarBar_widgetMap.contains("CalculatorResult"))
					generate_scalarBarWidget_calculatorResult((NumericSrcObject*)(pipeObj->GetObjParent()), pipeObj->mPipeLineObjProp.calculatorFilter_propData.resultName);
				//((FilterClip*)pipeObj)->plane_visible(pipeObj->mPipeLineObjProp.plane_propData.flag_plane);
			}
			slot_update_cubeAxes(pipeObj);
		}
	}
	slot_refreshPicker();
	if (flag_refresh_forAni)
		flag_refresh_forAni = false;
	if (flag_selfAdaptation)
	{
		renderWindow->GetRenderers()->GetFirstRenderer()->ResetCamera();
		flag_selfAdaptation = false;
	}
	renderWindow->Render();
	while (renderWindow->CheckInRenderStatus());
	//script add for user refresh custom tree
	sig_script_user_custom_refresh_pipelineTree();
	//script add for user refresh custom tree
	func_update_scalarBarPosition(mPipelineObjs);
	//emit sig_saveImage();//slot_saveImage();
	//emit sig_saveAnimate();//slot_saveAnimate();
	QTimer::singleShot(500, this, SLOT(slot_ImageAndAnimate()));
	//QThread::msleep(100);
	//slot_ImageAndAnimate();
	flag_render = true;
}
void PipelineObjPlotForm::slot_ImageAndAnimate()
{
	slot_saveImage();slot_saveAnimate();
	emit sig_exitSoftWare();
}
void PipelineObjPlotForm::func_combine_obj_model(PipelineObject* tep_pipelineObj)
{
	if (tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.flag_comModel)
	{
		if (tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.comModel_fileName != "")
		{
			bool flag = false;
			NumericSrcObject* tep_numericObj;
			for (int i = 0; i < g_ani_objList.count(); i++)
			{
				if (g_ani_objList.at(i)->mPipeLineObjProp.pipelineObj_base_propData.fileName == tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.comModel_fileName)
				{
					flag = true;
					tep_numericObj = (NumericSrcObject*)g_ani_objList.at(i);
					break;
				}
			}
			/*if (((NumericSrcObject*)tep_pipelineObj)->refrence_point_for_objID < 0)
				return;*/
			if (flag)
			{
				vtkAssembly* tep_model = tep_numericObj->model_ptr;
				if (tep_model != NULL)
				{
					//tep_model->SetOrigin(((NumericSrcObject*)tep_pipelineObj)->origin_center0[0], ((NumericSrcObject*)tep_pipelineObj)->origin_center0[1], ((NumericSrcObject*)tep_pipelineObj)->origin_center0[2]);
					double subX, subY, subZ, angX, angY, angZ;
					subX = ((NumericSrcObject*)tep_pipelineObj)->origin_center[0] - ((NumericSrcObject*)tep_pipelineObj)->origin_center0[0];
					subY = ((NumericSrcObject*)tep_pipelineObj)->origin_center[1] - ((NumericSrcObject*)tep_pipelineObj)->origin_center0[1];
					subZ = ((NumericSrcObject*)tep_pipelineObj)->origin_center[2] - ((NumericSrcObject*)tep_pipelineObj)->origin_center0[2];
					double *tep_refPoint1;
					tep_refPoint1 = ((NumericSrcObject*)tep_pipelineObj)->mBaseGridBak->GetPoint(((NumericSrcObject*)tep_pipelineObj)->refrence_point_for_objID);
					//double ref_radius = ((NumericSrcObject*)tep_pipelineObj)->refrence_point_for_obj[0] - ((NumericSrcObject*)tep_pipelineObj)->origin_center0[0];
					double ref_radius = (tep_refPoint1[2] - ((NumericSrcObject*)tep_pipelineObj)->origin_center[2])*(tep_refPoint1[2] - ((NumericSrcObject*)tep_pipelineObj)->origin_center[2]) +
						(tep_refPoint1[1] - ((NumericSrcObject*)tep_pipelineObj)->origin_center[1])*(tep_refPoint1[1] - ((NumericSrcObject*)tep_pipelineObj)->origin_center[1]) +
						(tep_refPoint1[0] - ((NumericSrcObject*)tep_pipelineObj)->origin_center[0])*(tep_refPoint1[0] - ((NumericSrcObject*)tep_pipelineObj)->origin_center[0]);
					ref_radius = sqrtf(ref_radius);
					//angY = qAcos((tep_refPoint1[1] - ((NumericSrcObject*)tep_pipelineObj)->origin_center[1]) / fabs(ref_radius));
					angX = 0;
					angY = qAsin((tep_refPoint1[2] - ((NumericSrcObject*)tep_pipelineObj)->origin_center[2]) / fabs(ref_radius));
					angZ = qAcos((tep_refPoint1[0] - ((NumericSrcObject*)tep_pipelineObj)->origin_center[0]) / fabs(ref_radius*cos(angY)));
					if ((tep_refPoint1[1] - ((NumericSrcObject*)tep_pipelineObj)->origin_center[1]) < 0)
						angZ *= (-1.0);
					//if (tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.file_xh == 0)
					//{
					//	tep_model->GetPosition(tep_numericObj->objModel_pos);
					//}
					//if (loadDispFile.frameList.size() > tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.file_xh)
					//{
					//	//subX = loadDispFile.frameList.at(tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.file_xh).Surge_GX;
					//	//subY = loadDispFile.frameList.at(tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.file_xh).Sway_GY;
					//	//subZ = loadDispFile.frameList.at(tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.file_xh).Heave_GZ;
					//	angX = loadDispFile.frameList.at(tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.file_xh).Roll_Gxb;
					//	angY = loadDispFile.frameList.at(tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.file_xh).Pitch_Gyb;
					//	angZ = loadDispFile.frameList.at(tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.file_xh).Yaw_Gzb;
					//}
					vtkSmartPointer<vtkTransform> tep_transForm = vtkSmartPointer<vtkTransform>::New();
					tep_transForm->Identity();
					tep_transForm->PostMultiply();
					tep_transForm->GetOrientation();
					tep_transForm->Translate(-((NumericSrcObject*)tep_pipelineObj)->origin_center0[0], -((NumericSrcObject*)tep_pipelineObj)->origin_center0[1], -((NumericSrcObject*)tep_pipelineObj)->origin_center0[2]);
					tep_transForm->RotateY(angY*57.32);
					tep_transForm->RotateZ(angZ*57.32);
					//tep_transForm->RotateZ(angZ*57.32);
					tep_model->SetUserTransform(tep_transForm);
					tep_transForm->Translate(((NumericSrcObject*)tep_pipelineObj)->origin_center[0], ((NumericSrcObject*)tep_pipelineObj)->origin_center[1], ((NumericSrcObject*)tep_pipelineObj)->origin_center[2]);
					//tep_model->AddPosition(-tep_pipelineObj->model_move_para[0], -tep_pipelineObj->model_move_para[1], -tep_pipelineObj->model_move_para[2]);					
					/*double *tep_refPoint1;
					tep_refPoint1 = ((NumericSrcObject*)tep_pipelineObj)->mBaseGridBak->GetPoint(((NumericSrcObject*)tep_pipelineObj)->refrence_point_for_objID);
					double tep_refPoint2[3];
					tep_refPoint2[0] = ((NumericSrcObject*)tep_pipelineObj)->refrence_point_for_obj[0] + subX;
					tep_refPoint2[1] = ((NumericSrcObject*)tep_pipelineObj)->refrence_point_for_obj[1] + subY;
					tep_refPoint2[2] = ((NumericSrcObject*)tep_pipelineObj)->refrence_point_for_obj[2] + subZ;
					double angX, angY, angZ;
					double ref_radius = ((NumericSrcObject*)tep_pipelineObj)->refrence_point_for_obj[0] - ((NumericSrcObject*)tep_pipelineObj)->origin_center0[0];
					angX = qAcos((tep_refPoint1[2] - ((NumericSrcObject*)tep_pipelineObj)->origin_center[2]) / fabs(ref_radius));
					angY = qAcos((tep_refPoint1[1] - ((NumericSrcObject*)tep_pipelineObj)->origin_center[1]) / fabs(ref_radius));
					angZ = qAcos((tep_refPoint1[0] - ((NumericSrcObject*)tep_pipelineObj)->origin_center[0]) / fabs(ref_radius));*/
					
					//tep_model->SetPosition(tep_numericObj->objModel_pos);
					//tep_model->RotateY(90);
					/*vtkSmartPointer<vtkTransform> tep_transForm2 = vtkSmartPointer<vtkTransform>::New();
					tep_transForm2->Translate(((NumericSrcObject*)tep_pipelineObj)->origin_center0[0], ((NumericSrcObject*)tep_pipelineObj)->origin_center0[1], ((NumericSrcObject*)tep_pipelineObj)->origin_center0[2]);
					tep_model->SetUserTransform(tep_transForm2);*/
					//tep_model->RotateY((-angY + tep_pipelineObj->model_move_para[4])*57.32+30);
					//tep_model->RotateX((-angX + tep_pipelineObj->model_move_para[3])*57.32);
					//tep_model->RotateZ((-angZ + tep_pipelineObj->model_move_para[5])*57.32);
					//tep_model->SetPosition(tep_numericObj->objModel_pos);
					//vtkSmartPointer<vtkTransform> tep_transForm2 = vtkSmartPointer<vtkTransform>::New();
					//tep_transForm2->Translate(subX, subY, subZ);
					//tep_model->SetUserTransform(tep_transForm2);
					tep_pipelineObj->model_move_para[0] = subX;
					tep_pipelineObj->model_move_para[1] = subY;
					tep_pipelineObj->model_move_para[2] = subZ;
					tep_pipelineObj->model_move_para[3] = angX;
					tep_pipelineObj->model_move_para[4] = angY;
					tep_pipelineObj->model_move_para[5] = angZ;
					/*vtkSmartPointer<vtkTransform> tep_transForm = vtkSmartPointer<vtkTransform>::New();
					tep_transForm->Translate(subX - tep_pipelineObj->model_move_para[0], subY - tep_pipelineObj->model_move_para[1], subZ - tep_pipelineObj->model_move_para[2]);
					double *tep_refPoint1;
					tep_refPoint1 = ((NumericSrcObject*)tep_pipelineObj)->mBaseGridBak->GetPoint(((NumericSrcObject*)tep_pipelineObj)->refrence_point_for_objID);
					double tep_refPoint2[3];
					tep_refPoint2[0] = ((NumericSrcObject*)tep_pipelineObj)->refrence_point_for_obj[0] + subX;
					tep_refPoint2[1] = ((NumericSrcObject*)tep_pipelineObj)->refrence_point_for_obj[1] + subY;
					tep_refPoint2[2] = ((NumericSrcObject*)tep_pipelineObj)->refrence_point_for_obj[2] + subZ;
					double angX, angY, angZ;
					angX = qAsin((tep_refPoint1[0] - tep_refPoint2[0]) / (((NumericSrcObject*)tep_pipelineObj)->refrence_point_for_obj[0] - ((NumericSrcObject*)tep_pipelineObj)->origin_center0[0]));
					angY = qAsin((tep_refPoint1[1] - tep_refPoint2[1]) / (((NumericSrcObject*)tep_pipelineObj)->refrence_point_for_obj[0] - ((NumericSrcObject*)tep_pipelineObj)->origin_center0[0]));
					angZ = qAsin((tep_refPoint1[2] - tep_refPoint2[2]) / (((NumericSrcObject*)tep_pipelineObj)->refrence_point_for_obj[0] - ((NumericSrcObject*)tep_pipelineObj)->origin_center0[0]));
					tep_transForm->RotateX(angX*57.32 - tep_pipelineObj->model_move_para[3]);
					tep_transForm->RotateY(angY*57.32 - tep_pipelineObj->model_move_para[4]);
					tep_transForm->RotateZ(angZ*57.32 - tep_pipelineObj->model_move_para[5]);
					tep_model->SetUserTransform(tep_transForm);
					tep_pipelineObj->model_move_para[0] = subX;
					tep_pipelineObj->model_move_para[1] = subY;
					tep_pipelineObj->model_move_para[2] = subZ;
					tep_pipelineObj->model_move_para[3] = angX;
					tep_pipelineObj->model_move_para[4] = angY;
					tep_pipelineObj->model_move_para[5] = angZ;*/
					qDebug() << "model axis:  " << subX << ",   " << subY << ",   " << subZ << ",   " << angX << ",   " << angY << ",   " << angZ << endl;
				}
			}
		}
	}
}

void PipelineObjPlotForm::slot_saveAnimate()
{
	//保存动画
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
	if (aniSetting_data->flag_save)
	{
		if ((cur_ani_frame_xh >= aniSetting_data->frame_start_xh) && (!((cur_ani_frame_xh - aniSetting_data->frame_start_xh) % aniSetting_data->frame_step)))
		{
#ifdef Q_OS_WIN
			if (!flag_start_aniSave)
			{
				QString avi_name;
				avi_name = aniSetting_data->avi_filename;
				/*avi_name = QDir::currentPath(); avi_name += "/";
				avi_name += "avi/";
				QDir tep_dir(avi_name);
				if (!tep_dir.exists())
				{
					QDir dir1;
					dir1.mkdir(avi_name);
				}
				QDateTime tep_dt = QDateTime::currentDateTime();
				avi_name += tep_dt.toString("yyyyMMdd-hhmmss");
				avi_name += ".avi";*/
				if (avi_writer==NULL)
					avi_writer = vtkAVIWriter::New();
				QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
				avi_writer->SetFileName(avi_name.toLocal8Bit().data());
				avi_writer->SetRate(aniSetting_data->frame_rate);
				vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
				windowToImageFilter->SetInput(renderWindow);
				windowToImageFilter->Modified();
				vtkSmartPointer<vtkImageResize> resize = vtkSmartPointer<vtkImageResize>::New();
				resize->SetInputConnection(windowToImageFilter->GetOutputPort());
				//resize->SetOutputDimensions(aniSetting_data->image_wid, aniSetting_data->image_hei, -1);
				resize->Update();
				avi_writer->SetInputConnection(resize->GetOutputPort());
				avi_writer->Start();
				avi_writer->Write();
				flag_start_aniSave = true;
			}
			else
			{
				vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
				windowToImageFilter->SetInput(renderWindow);
				windowToImageFilter->Modified();
				vtkSmartPointer<vtkImageResize> resize = vtkSmartPointer<vtkImageResize>::New();
				resize->SetInputConnection(windowToImageFilter->GetOutputPort());
				//resize->SetOutputDimensions(aniSetting_data->image_wid, aniSetting_data->image_hei, -1);
				resize->Update();
				avi_writer->SetInputConnection(resize->GetOutputPort());
				avi_writer->Write();

				if (cur_ani_frame_xh == (aniSetting_data->frame_start_xh + (aniSetting_data->frame_num - 1)*aniSetting_data->frame_step))
				{
					slot_finish_saveAvi();
				}
			}
#endif

		}
		QString tep_saveFileName; tep_saveFileName = tep_saveFileName.setNum(cur_ani_frame_xh,10);//QString tep_saveFileName = QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss");
		if (aniSetting_data->flag_saveImage)
			savePicutreForAvi(tep_saveFileName);
		if (aniSetting_data->flag_savePov)
			savePovForAvi(tep_saveFileName);
	}
}

void PipelineObjPlotForm::savePicutreForAvi(QString img_name)
{	
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
	QDir dir1(aniSetting_data->saveImage_path);
	if (!dir1.exists())
		return;
	QString tep_name = aniSetting_data->saveImage_path; tep_name += "/";
	tep_name += img_name;
	tep_name += ".";
	tep_name += aniSetting_data->saveImage_format;

	vtkSmartPointer<vtkWindowToImageFilter> filter = vtkSmartPointer<vtkWindowToImageFilter>::New();
	filter->SetInput(renderWindow);
	filter->Modified();
	if (aniSetting_data->saveImage_format == "png")
	{
		vtkSmartPointer<vtkPNGWriter> pngWriter =vtkSmartPointer<vtkPNGWriter>::New();
		vtkSmartPointer<vtkImageResize> resize =vtkSmartPointer<vtkImageResize>::New();
		resize->SetInputConnection(filter->GetOutputPort());
		//resize->SetOutputDimensions(aniSetting_data->image_wid,aniSetting_data->image_hei, -1);
		resize->Update();
		pngWriter->SetFileName(tep_name.toLocal8Bit().data());
		pngWriter->SetInputConnection(resize->GetOutputPort());
		pngWriter->Write();
		
	}
	else if (aniSetting_data->saveImage_format == "jpg")
	{
		vtkSmartPointer<vtkJPEGWriter> jpegWriter =vtkSmartPointer<vtkJPEGWriter>::New();
		vtkSmartPointer<vtkImageResize> resize =vtkSmartPointer<vtkImageResize>::New();
		resize->SetInputConnection(filter->GetOutputPort());
		//resize->SetOutputDimensions(aniSetting_data->image_wid, aniSetting_data->image_hei, -1);
		resize->Update();

		jpegWriter->SetFileName(tep_name.toLocal8Bit().data());
		jpegWriter->SetInputConnection(resize->GetOutputPort());
		jpegWriter->Write();
	}
	else if (aniSetting_data->saveImage_format =="bmp")
	{
		vtkSmartPointer<vtkBMPWriter> bmpWriter =vtkSmartPointer<vtkBMPWriter>::New();
		vtkSmartPointer<vtkImageResize> resize =vtkSmartPointer<vtkImageResize>::New();
		resize->SetInputConnection(filter->GetOutputPort());
		//resize->SetOutputDimensions(aniSetting_data->image_wid, aniSetting_data->image_hei, -1);
		resize->Update();
		bmpWriter->SetFileName(tep_name.toLocal8Bit().data());
		bmpWriter->SetInputConnection(resize->GetOutputPort());
		bmpWriter->Write();
	}
	else if (aniSetting_data->saveImage_format == "eps")
	{
		vtkSmartPointer<vtkImageResize> resize =vtkSmartPointer<vtkImageResize>::New();
		resize->SetInputConnection(filter->GetOutputPort());
		//resize->SetOutputDimensions(aniSetting_data->image_wid, aniSetting_data->image_hei, -1);
		resize->Update();
		vtkSmartPointer<vtkGL2PSExporter> exp =vtkSmartPointer<vtkGL2PSExporter>::New();
		exp->SetRenderWindow(renderWindow);
		exp->SetFileFormatToEPS();
		exp->SetSortToSimple();
		exp->DrawBackgroundOn();
		exp->Write3DPropsAsRasterImageOff();
		exp->SetTextAsPath(true);
		exp->SetFilePrefix(tep_name.toLocal8Bit().data());
		exp->SetCompress(0);
		exp->Write();
	}
}

void PipelineObjPlotForm::savePovForAvi(QString pov_name)
{
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
	QDir dir1(aniSetting_data->savePov_path);
	if (!dir1.exists())
		return;
	QString tep_name = aniSetting_data->savePov_path; tep_name += "/";
	tep_name += pov_name;
	tep_name += ".pov";
	vtkSmartPointer<vtkWindowToImageFilter> filter = vtkSmartPointer<vtkWindowToImageFilter>::New();
	filter->SetInput(renderWindow);
	filter->Modified();
	vtkSmartPointer<vtkImageResize> resize = vtkSmartPointer<vtkImageResize>::New();
	resize->SetInputConnection(filter->GetOutputPort());
	resize->Update();
	vtkSmartPointer<disvtkPOVExporter> povexp =
		vtkSmartPointer<disvtkPOVExporter>::New();
	povexp->SetFileName(tep_name.toLocal8Bit().data());
	povexp->SetRenderWindow(renderWindow);
	povexp->set_origin(pov_propData.mOrigin);
	povexp->set_disPosition(pov_propData.pos);
	povexp->set_disAndangle(pov_propData.disAndang);
	povexp->set_disActors(pov_propData.mVecActors);
	/*vtkActorCollection *tep_collection= renderer->GetActors();
	if (tep_collection != NULL)
	{
		int tep_num = tep_collection->GetNumberOfItems();
		for (int i = 0; i < tep_num; i++)
		{
			vtkActor* tep_actor=tep_collection->GetNextActor();
			if (tep_actor!=NULL)
				tep_actor->Modified();
		}
	}*/
	povexp->Update();
	povexp->Write();
}

void PipelineObjPlotForm::slot_update_opacity(PipelineObject * tep_pipelineObj)
{
    tep_pipelineObj->SetOpacity();
	renderWindow->Render();
}

void PipelineObjPlotForm::slot_update_color(PipelineObject * tep_pipelineObj)
{
	if (tep_pipelineObj->GetObjectType() == dNumeric_DataSource)
	{
		NumericSrcObject* tep_obj = dynamic_cast<NumericSrcObject*> (tep_pipelineObj);
		tep_obj->ChangeColumnColor();
		if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible)
			slot_update_scalarBar(tep_obj);
		else
		{
			if (flag_render)
				renderWindow->Render();
		}
	}
	else if (tep_pipelineObj->GetObjectType() == dSlice_DataSource)
	{
		FilterSlice* tep_obj = dynamic_cast<FilterSlice*> (tep_pipelineObj);
		tep_obj->ChangeColumnColor();
		if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible)
			slot_update_scalarBar(tep_obj->GetObjParent());
		else
		{
			if (flag_render)
				renderWindow->Render();
		}
	}
	else if (tep_pipelineObj->GetObjectType() == dClip_DataSource)
	{
		FilterClip* tep_obj = dynamic_cast<FilterClip*> (tep_pipelineObj);
		tep_obj->ChangeColumnColor();
		if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible)
			slot_update_scalarBar(tep_obj->GetObjParent());
		else
		{
			if (flag_render)
				renderWindow->Render();
		}
	}
	else if (tep_pipelineObj->GetObjectType() == dContour_DataSource)
	{
		FilterContour* tep_obj = dynamic_cast<FilterContour*> (tep_pipelineObj);
		tep_obj->ChangeColumnColor();
		if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible)
			slot_update_scalarBar(tep_obj->GetObjParent());
		else
		{
			if (flag_render)
				renderWindow->Render();
		}
	}
	else if (tep_pipelineObj->GetObjectType() == dVector_DataSource)
	{
		FilterVector* tep_obj = dynamic_cast<FilterVector*> (tep_pipelineObj);
		tep_obj->ChangeColumnColor();
		if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible)
			slot_update_scalarBar(tep_obj->GetObjParent());
		else
		{
			if (flag_render)
				renderWindow->Render();
		}
	}
	else if (tep_pipelineObj->GetObjectType() == dReflection_DataSource)
	{
		FilterReflection* tep_obj = dynamic_cast<FilterReflection*> (tep_pipelineObj);
		tep_obj->ChangeColumnColor();
		if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible)
			slot_update_scalarBar(tep_obj->GetObjParent());
		else
		{
			if (flag_render)
				renderWindow->Render();
		}
	}
	else if (tep_pipelineObj->GetObjectType() == dCalculator_DataSource)
	{
		FilterCalculator* tep_obj = dynamic_cast<FilterCalculator*> (tep_pipelineObj);
		tep_obj->ChangeColumnColor();
		if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible)
			slot_update_scalarBar(tep_obj->GetObjParent());
		else
		{
			if (flag_render)
				renderWindow->Render();
		}
	}
	else if (tep_pipelineObj->GetObjectType() == dSmooth_DataSource)
	{
		FilterSmooth* tep_obj = dynamic_cast<FilterSmooth*> (tep_pipelineObj);
		tep_obj->ChangeColumnColor();
		if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible)
			slot_update_scalarBar(tep_obj->GetObjParent());
		else
		{
			if (flag_render)
				renderWindow->Render();
		}
	}
	else if (tep_pipelineObj->GetObjectType() == dStreamLine_DataSource)
	{
		FilterStreamLine* tep_obj = dynamic_cast<FilterStreamLine*> (tep_pipelineObj);
		tep_obj->ChangeColumnColor();
		if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible)
			slot_update_scalarBar(tep_obj->GetObjParent());
		else
		{
			if (flag_render)
				renderWindow->Render();
		}
	}
}

void PipelineObjPlotForm::slot_update_lineWidth(PipelineObject * tep_pipelineObj)
{
	tep_pipelineObj->SetLineWidth();
	renderWindow->Render();
}

void PipelineObjPlotForm::slot_update_pointSize(PipelineObject * tep_pipelineObj)
{
	tep_pipelineObj->SetPointSize(); 
	renderWindow->Render();
}

void PipelineObjPlotForm::slot_update_translate(PipelineObject * tep_pipelineObj)
{
	tep_pipelineObj->SetTranslate();
	renderWindow->Render();
}

void PipelineObjPlotForm::slot_update_origin(PipelineObject * tep_pipelineObj)
{
	tep_pipelineObj->SetOrigin();
	renderWindow->Render();
}

void PipelineObjPlotForm::slot_update_scale(PipelineObject * tep_pipelineObj)
{
	tep_pipelineObj->SetScale();
	renderWindow->Render();
}

void PipelineObjPlotForm::slot_update_orientation(PipelineObject * tep_pipelineObj)
{
	tep_pipelineObj->SetOrientation();
	renderWindow->Render();
}

void PipelineObjPlotForm::slot_update_representation(PipelineObject * tep_pipelineObj)
{
	tep_pipelineObj->SetRenderMode();
	if (flag_render)
		renderWindow->Render();
}

void PipelineObjPlotForm::slot_update_axes_visible(bool checked)
{
	if (checked)
		m_orientationMarkerWidget->On();	
	else
		m_orientationMarkerWidget->Off();
	if (flag_render)
		renderWindow->Render();
}
void PipelineObjPlotForm::slot_update_camera_parallel(bool checked)
{
	if (checked)
		renderWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera()->ParallelProjectionOn();
	else
		renderWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera()->ParallelProjectionOff();
	if (flag_render)
		renderWindow->Render();
}

void PipelineObjPlotForm::slot_update_specular(PipelineObject *tep_pipelineObj)
{
	tep_pipelineObj->SetLightingSpecular();
	renderWindow->Render();
}

void PipelineObjPlotForm::slot_update_diffuse(PipelineObject *tep_pipelineObj)
{
	tep_pipelineObj->SetLightingDiffuse();
	renderWindow->Render();
}

void PipelineObjPlotForm::slot_update_ambient(PipelineObject *tep_pipelineObj)
{
	tep_pipelineObj->SetLightingAmbient();
	renderWindow->Render();
}

void PipelineObjPlotForm::slot_update_specularPower(PipelineObject *tep_pipelineObj)
{
	tep_pipelineObj->SetLightingSpecularPower();
	renderWindow->Render();
}

void PipelineObjPlotForm::slot_update_specularColor(PipelineObject *tep_pipelineObj)
{
	tep_pipelineObj->SetLightingSpecularCorlor();
	if (flag_render)
		renderWindow->Render();
}

void PipelineObjPlotForm::slot_update_solidColor(PipelineObject *tep_pipelineObj)
{
	tep_pipelineObj->SetSolidCorlor();
	if (flag_render)
		renderWindow->Render();
}

void PipelineObjPlotForm::slot_update_edgeColor(PipelineObject *tep_pipelineObj)
{
	tep_pipelineObj->SetEdgeCorlor();
	if (flag_render)
		renderWindow->Render();
}

void PipelineObjPlotForm::slot_update_interpolation(PipelineObject *tep_pipelineObj)
{
	tep_pipelineObj->SetLightingInterpolation();
	if (flag_render)
		renderWindow->Render();
}

void PipelineObjPlotForm::slot_update_actor_visible_plotForm(PipelineObject* pipeObj,bool flag_change)
{
	if (pipeObj->GetObjectType() == dNumeric_DataSource)
	{
		if (!pipeObj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors)
		{
			if (pipeObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible && (pipeObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle != "SolidColor"))
			{
				pipeObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
				pipeObj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert(pipeObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle, false);
			}
		}
		else
		{
			if (pipeObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible && (pipeObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle != "SolidColor"))
			{
				pipeObj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert(pipeObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle, true);
			}
		}
		slot_update_scalarBar(pipeObj);
		
	}
	else
	{
		if (!pipeObj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors)
		{
			if (pipeObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible && 
				(pipeObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle != "SolidColor"))
			{
				if (!pipeObj->GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible
					|| (pipeObj->GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle != pipeObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle))
				{
					pipeObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
					pipeObj->GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert(pipeObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle, false);
				}
			}
		}
		else
		{
			if (pipeObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible && (pipeObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle != "SolidColor"))
			{
				pipeObj->GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert(pipeObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle, true);
			}
		}
		slot_update_scalarBar(pipeObj->GetObjParent());
	}		
	pipeObj->ShowActor();
	slot_update_cubeAxes(pipeObj);
	if (pipeObj->GetObjectType() == dSlice_DataSource) 
	{
		((FilterSlice*)pipeObj)->plane_visible(pipeObj->mPipeLineObjProp.plane_propData.flag_plane);
	}
	else if (pipeObj->GetObjectType() == dClip_DataSource)
	{
		((FilterClip*)pipeObj)->plane_visible(pipeObj->mPipeLineObjProp.plane_propData.flag_plane);
	}
	if (flag_change)
		renderWindow->Render();
	flag_render = true;
}

void PipelineObjPlotForm::slot_update_planeOrigin(PipelineObject *tep_pipelineObj)
{
	if (tep_pipelineObj->GetObjectType() == dSlice_DataSource)
		((FilterSlice*)tep_pipelineObj)->slot_changeOrigins();
	else if (tep_pipelineObj->GetObjectType() == dClip_DataSource)
		((FilterClip*)tep_pipelineObj)->slot_changeOrigins();	
	if (flag_render)
		renderWindow->Render();
}

void PipelineObjPlotForm::slot_update_planeNormal(PipelineObject *tep_pipelineObj)
{
	if (tep_pipelineObj->GetObjectType()==dSlice_DataSource)
		((FilterSlice*)tep_pipelineObj)->slot_changeNormals();
	else if (tep_pipelineObj->GetObjectType() == dClip_DataSource)
		((FilterClip*)tep_pipelineObj)->slot_changeNormals();
	if (flag_render)
		renderWindow->Render();
}

void PipelineObjPlotForm::slot_update_planeVis(PipelineObject *tep_pipelineObj)
{
	if (tep_pipelineObj->GetObjectType()==dSlice_DataSource)
		((FilterSlice*)tep_pipelineObj)->plane_visible(tep_pipelineObj->mPipeLineObjProp.plane_propData.flag_plane);
	else if (tep_pipelineObj->GetObjectType() == dClip_DataSource)
		((FilterClip*)tep_pipelineObj)->plane_visible(tep_pipelineObj->mPipeLineObjProp.plane_propData.flag_plane);
	if (flag_render)
		renderWindow->Render();
}

void PipelineObjPlotForm::slot_update_planeCameraNormal(PipelineObject *tep_pipelineObj)
{
	if (tep_pipelineObj->GetObjectType() == dSlice_DataSource)
		((FilterSlice*)tep_pipelineObj)->slot_changeCameraNormals();
	else if (tep_pipelineObj->GetObjectType() == dClip_DataSource)
		((FilterClip*)tep_pipelineObj)->slot_changeCameraNormals();
	if (flag_render)
		renderWindow->Render();
}

void PipelineObjPlotForm::slot_finish_aniThread(bool flag)
{
	emit sig_finish_aniThread(flag);
}
bool PipelineObjPlotForm::func_displaceAniFinish()
{
	bool flag = true;
	int new_pos;
	for (int i = 0; i < g_ani_objList.count(); i++)
	{
		PipelineObject* tep_pipelineObj = g_ani_objList.at(i);
		if (tep_pipelineObj->GetObjectType() == dNumeric_DataSource)
		{
			if (tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.flag_AniDisplaceFile)
			{
				new_pos = tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.cur_pos + tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.interval+1;
				if (tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.count() <= new_pos)
				{
					flag = false;
					return flag;
				}
			}
		}
	}
	return flag;
}
void PipelineObjPlotForm::func_resetDisAni(QList<PipelineObject*> tep_mPipelineObjs)
{
	//int num = 0;
	QString tep_filename;
	QString tep_str;
	for (int j = 0; j < tep_mPipelineObjs.count(); j++)
	{
		if (tep_mPipelineObjs[j]->GetObjectType() == dNumeric_DataSource)
		{
			if (tep_mPipelineObjs[j]->mPipeLineObjProp.aniDisplace_propData.flag_AniDisplaceFile)
			{
				tep_mPipelineObjs[j]->mPipeLineObjProp.aniDisplace_propData.cur_pos = tep_mPipelineObjs[j]->mPipeLineObjProp.aniDisplace_propData.start_pos;
				for (int k = 0; k < tep_mPipelineObjs.count(); k++)
				{
					if ((tep_mPipelineObjs[k]->GetObjectType() == dSlice_DataSource) || (tep_mPipelineObjs[k]->GetObjectType() == dClip_DataSource)
						|| (tep_mPipelineObjs[k]->GetObjectType() == dContour_DataSource) || (tep_mPipelineObjs[k]->GetObjectType() == dVector_DataSource)
						|| (tep_mPipelineObjs[k]->GetObjectType() == dReflection_DataSource) || (tep_mPipelineObjs[k]->GetObjectType() == dSmooth_DataSource)
						|| (tep_mPipelineObjs[k]->GetObjectType() == dCalculator_DataSource) || (tep_mPipelineObjs[k]->GetObjectType() == dStreamLine_DataSource))
					{
						if ((tep_mPipelineObjs[k] != NULL) && (tep_mPipelineObjs[k]->GetObjParent() == tep_mPipelineObjs[j]))
						{
							tep_mPipelineObjs[k]->mPipeLineObjProp.aniDisplace_propData.cur_pos = tep_mPipelineObjs[j]->mPipeLineObjProp.aniDisplace_propData.start_pos;
						}
					}
				}
			}
		}
	}
}
void PipelineObjPlotForm::slot_runAni()
{
	if ((cur_ani_frame_xh + 1) == max_ani_frames)
	{
		cur_ani_frame_xh = 0;
		func_resetDisAni(g_ani_objList);
	}
	else if (max_ani_frames < 2)
	{
		if (func_displaceAniFinish())
		{
			cur_ani_frame_xh = 0;
			func_resetDisAni(g_ani_objList);
		}
	}
	if ((max_ani_frames > 1)||(num_maxDisplace>1))
	{
		func_set_disActors(g_ani_objList);
		if (ani_thread == NULL)
		{
			ani_thread = new vtkAniThread;
			connect(ani_thread, SIGNAL(sig_finish_aniThread(bool)), this, SLOT(slot_finish_aniThread(bool)));
			connect(ani_thread, SIGNAL(sig_refreshViewAni(int)), this, SLOT(slot_refreshViewAni(int)));
			connect(ani_thread, SIGNAL(sig_offPlaneWidget(PipelineObject*)), this, SLOT(slot_offPlaneWidget(PipelineObject*)));
		}
		else
		{
			if (ani_thread->isRunning())
			{
				ani_thread->stop();
			}
		}
		ani_thread->initThread(g_ani_objList, max_ani_frames, cur_ani_frame_xh);
		ani_thread->start();
		
	}
	//emit sig_update_runtool_runing(true);
}

void PipelineObjPlotForm::slot_update_cgns_plot(PipelineObject* pipeObj,int cgns_times)
{
	if ((pipeObj == NULL) || (pipeObj->blockValMap.count() < 1))
	{
		return;
	}
	vtkSmartPointer<vtkFloatArray> tep_arr = NULL;
	QString tepStr = pipeObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle;
	if (tepStr.indexOf("_cell000")>=0)
		tepStr = tepStr.left(tepStr.length() - 8);
	if (pipeObj->blockValMap.contains(tepStr))
		tep_arr = pipeObj->blockValMap[tepStr];
	if (tep_arr == NULL)
		return;
	((NumericSrcObject*)pipeObj)->ChangeColumnColorWithArray(tep_arr);
	
	//refresh child pipelineObj
	for (int i = 0; i < cur_mPipelineObjs.count(); i++)
	{
		if (cur_mPipelineObjs.at(i)->GetObjParent() == pipeObj)
		{
			cur_mPipelineObjs.at(i)->mBaseGrid = pipeObj->mBaseGrid;
			cur_mPipelineObjs.at(i)->generate_actors();
		}
	}
	
	if (flag_render)
		renderWindow->Render();
	tepStr = QString("%1 cgns render times---%2").arg(pipeObj->GetName()).arg(cgns_times);
	emit sig_cgns_animnate_times(tepStr);
	while (renderWindow->CheckInRenderStatus());
	g_aniThread_mutex.unlock();
}

void PipelineObjPlotForm::slot_update_cgns_pipelineObj(PipelineObject* pipeObj)
{
	if (pipeObj == NULL)
		return;
	if (((NumericSrcObject*)pipeObj)->readdata_io == NULL)
		return;
	if ((ani_thread!=NULL)&&(ani_thread->isRunning()))
	{
		QMessageBox box(QMessageBox::Warning, QStringLiteral("warning"), QStringLiteral("Please stop the current animation !"));
		box.setStandardButtons(QMessageBox::Yes);
		box.setButtonText(QMessageBox::Yes, QStringLiteral("ok"));
		box.exec();
		return;
	}
	if (cgns_thread == NULL)
	{
		cgns_thread = new updateCgnsThread;
		connect(cgns_thread, SIGNAL(sig_update_cgns_plot(PipelineObject*,int)), this, SLOT(slot_update_cgns_plot(PipelineObject*,int)));
		connect(cgns_thread, SIGNAL(sig_finish_cgnsThread(PipelineObject*)), this, SLOT(slot_finish_cgnsThread(PipelineObject*)));
		
		/*connect(ani_thread, SIGNAL(sig_refreshViewAni()), this, SLOT(slot_refreshViewAni()));
		connect(ani_thread, SIGNAL(sig_offPlaneWidget(PipelineObject*)), this, SLOT(slot_offPlaneWidget(PipelineObject*)));*/
	}
	else
	{
		slot_stop_update_cgns_pipelineObj();
	}
	cgns_thread->initThread(pipeObj, pipeObj->numberOfCgnsTimes,0);
	cgns_thread->start();
	QString tepStr = pipeObj->GetName(); tepStr += "  start cgns times render!";
	emit sig_cgns_animnate_times(tepStr);
}

void PipelineObjPlotForm::slot_finish_cgnsThread(PipelineObject* pipeObj)
{
	if (pipeObj == NULL)
		return;
	QString tepStr = pipeObj->GetName(); tepStr += "  cgns stop animate times!";
	emit sig_cgns_animnate_times(tepStr);
	emit sig_cgnsAniTimesIcon_status(true);
}
void PipelineObjPlotForm::slot_changeCurFrame(int tep_frame_xh)
{
	LoadActionFile* ptr = new LoadActionFile;
	int tep_max_dis_num = ptr->func_get_max_num_displace(g_ani_objList);
	delete ptr;
	ptr = NULL;
	if (max_ani_frames > 1)
	{
		cur_ani_frame_xh = tep_frame_xh;
		if (tep_frame_xh < tep_max_dis_num)
			cur_ani_dis_xh = tep_frame_xh;
	}
	else
	{
		if (tep_frame_xh < tep_max_dis_num)
			cur_ani_dis_xh = tep_frame_xh;
	}
	QString tep_filename;
	QString tep_str;
	QList<PipelineObject*> source_mPipelineObjs;
	//QList<PipelineObject*> filter_mPipelineObjs;
	source_mPipelineObjs.clear();
	for (int j = 0; j < g_ani_objList.count(); j++)
	{
		if (g_ani_objList[j]->GetObjectType() == dNumeric_DataSource)
		{
			if ((g_ani_objList[j]->mPipeLineObjProp.pipelineObj_base_propData.fileList.count() > tep_frame_xh) &&
				(g_ani_objList[j]->mPipeLineObjProp.pipelineObj_base_propData.fileListBak.count()>1))
			{
				tep_filename = g_ani_objList[j]->mPipeLineObjProp.pipelineObj_base_propData.fileList.at(tep_frame_xh);
				//if (cur_NumericSrcObj == NULL)
				NumericSrcObject*	cur_NumericSrcObj = new NumericSrcObject(g_ani_objList[j]->mPipeLineObjProp.pipelineObj_base_propData.fileList, tep_frame_xh);
				if (cur_NumericSrcObj->readFile(tep_filename))
				{
					///////////////////////////////////////////
					bool flag_generate_lookupTable = false;
					if (g_ani_objList[j]->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle != "SolidColor")
					{
						vtkColorTransferFunction* tep_colorTransferFunc = g_ani_objList[j]->getLookupTable(g_ani_objList[j]->mBaseGridBak, g_ani_objList[j]->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle);
						double tep_range[2]; tep_colorTransferFunc->GetRange(tep_range);
						if (tep_range[0]+0.00001 == tep_range[1])
							flag_generate_lookupTable = true;
						/*else
						{
							double tep_range[2];
							if (cur_NumericSrcObj->get_column_range(cur_NumericSrcObj->mBaseGrid, glo_ani_objList[j]->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle, tep_range))
							{
								glo_ani_objList[j]->getLookupTable(glo_ani_objList[j]->mBaseGrid, glo_ani_objList[j]->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle)->AdjustRange(tep_range);
								glo_ani_objList[j]->getLookupOpacity(glo_ani_objList[j]->mBaseGrid, glo_ani_objList[j]->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle)->AdjustRange(tep_range);
							}
						}*/ //20180227
					}
					//add for multi zone
					cur_NumericSrcObj->mPipeLineObjProp.pipelineObj_base_propData.block_showMap = g_ani_objList[j]->mPipeLineObjProp.pipelineObj_base_propData.block_showMap;
					cur_NumericSrcObj->flag_update_mBaseGrid = true;					
					cur_NumericSrcObj->func_update_mBaseGrid();
					//add for multi zone
					if (g_ani_objList[j]->mBaseGrid != NULL)
					{
						g_ani_objList[j]->mBaseGrid->Delete();
					}
					if (g_ani_objList[j]->mBaseGridBak != NULL)
					{
						g_ani_objList[j]->mBaseGridBak->Delete();
					}
					g_ani_objList[j]->mBaseGrid = cur_NumericSrcObj->mBaseGrid;
					g_ani_objList[j]->mBaseGridBak = cur_NumericSrcObj->mBaseGridBak;
					if (flag_generate_lookupTable)
						g_ani_objList[j]->generate_scalarBar_lookupTableMap(g_ani_objList[j]->mBaseGrid);
					QStringList tep_list1 = g_ani_objList[j]->mPipeLineObjProp.pipelineObj_base_propData.fileList.at(0).split("/"); //QStringList tep_list1 = tep_filename.split("/");//
					int tep_count = tep_list1.count();
					if (tep_count > 0)
					{
						g_ani_objList[j]->mPipeLineObjProp.pipelineObj_base_propData.fileName = tep_list1.at(tep_count - 1);
						g_ani_objList[j]->mPipeLineObjProp.pipelineObj_base_propData.file_xh = tep_frame_xh;
					}
				}
				delete cur_NumericSrcObj;
				cur_NumericSrcObj = NULL;
			}
			source_mPipelineObjs.append(g_ani_objList[j]);
			LoadActionFile* ptr = new LoadActionFile;
			ptr->func_setPipeObjDisplaceData(g_ani_objList[j], tep_frame_xh, g_ani_objList);
			delete ptr;
			ptr = NULL;
		}
	}
	//filter_mPipelineObjs.clear();
	for (int j = 0; j < g_ani_objList.count(); j++)
	{
		if ((g_ani_objList[j]->GetObjectType() == dSlice_DataSource) || (g_ani_objList[j]->GetObjectType() == dClip_DataSource)
			|| (g_ani_objList[j]->GetObjectType() == dContour_DataSource) || (g_ani_objList[j]->GetObjectType() == dVector_DataSource)
			|| (g_ani_objList[j]->GetObjectType() == dReflection_DataSource) || (g_ani_objList[j]->GetObjectType() == dSmooth_DataSource)
			|| (g_ani_objList[j]->GetObjectType() == dCalculator_DataSource) || (g_ani_objList[j]->GetObjectType() == dStreamLine_DataSource))
		{
			if (g_ani_objList[j]->mPipeLineObjProp.pipelineObj_base_propData.fileList.count() > tep_frame_xh)
			{
				tep_filename = g_ani_objList[j]->mPipeLineObjProp.pipelineObj_base_propData.fileName;
				PipelineObject *tep_pipelineObj = findParentPipelineObj(tep_filename, source_mPipelineObjs);
				if (tep_pipelineObj != NULL)
				{
					g_ani_objList[j]->mBaseGrid = tep_pipelineObj->mBaseGrid;

				}			
			}
		}
	}
	source_mPipelineObjs.clear();
	flag_refresh_forAni = true;
	slot_refreshView(g_ani_objList);
}

PipelineObject* PipelineObjPlotForm::findParentPipelineObj(QString tep_filename, QList<PipelineObject*> tep_mPipelineObjs)
{
	for (int i = 0; i < tep_mPipelineObjs.count(); i++)
	{
		if (tep_filename == tep_mPipelineObjs[i]->mPipeLineObjProp.pipelineObj_base_propData.fileName)
		{
			return tep_mPipelineObjs[i];
		}

	}
	return NULL;
}

void PipelineObjPlotForm::slot_stopAni()
{
	if (ani_thread != NULL)
	{
		if (ani_thread->isRunning())
		{
			ani_thread->stop();
		}
	}
	//emit sig_update_runtool_runing(false);
}

void PipelineObjPlotForm::slot_stop_update_cgns_pipelineObj()
{
	if (cgns_thread != NULL)
	{
		if (cgns_thread->isRunning())
		{
			cgns_thread->stop();
			cgns_thread->wait();
			PipelineObject* tepObj = cgns_thread->cur_NumericSrcObj;
			if (tepObj != NULL)
			{
				QString tepStr = tepObj->GetName(); tepStr += "  stop cgns times render!";
				emit sig_cgns_animnate_times(tepStr);
			}
		}
	}
}

void PipelineObjPlotForm::slot_finish_saveAvi()
{
	if (flag_start_aniSave)
	{
#ifdef Q_OS_WIN
		if (avi_writer != NULL)
		{
			avi_writer->End();
			avi_writer->Delete();
			avi_writer = NULL;
			emit sig_finish_aviFile(aniSetting_data->avi_filename);
		}
		flag_start_aniSave = false;
		aniSetting_data->flag_save = false;
#endif
		
	}
}

void PipelineObjPlotForm::generate_scalarBarWidget(NumericSrcObject *tep_obj)
{
	if (!tep_obj->get_flag_generate_scalarBarWidget())
	{
		QString tep_colorColumn, tep;
		vtkSmartPointer<vtkTextProperty> propLable = vtkSmartPointer<vtkTextProperty>::New();
		propLable->SetFontFamily(VTK_FONT_FILE);
		propLable->SetFontFile(mFontPath.toLocal8Bit().data());
		propLable->SetBold(0);
		propLable->SetItalic(0);
		propLable->SetShadow(0);
		propLable->SetJustification(VTK_TEXT_LEFT);
		propLable->SetColor(0, 0, 0);
		propLable->SetFontSize(scalarBar_FontSize);
		for (int i = 0; i < tep_obj->mPipeLineObjProp.pipelineObj_base_propData.m_colorList.count(); i++)
		{
			tep_colorColumn = tep_obj->mPipeLineObjProp.pipelineObj_base_propData.m_colorList.at(i);
			vtkSmartPointer<vtkColorTransferFunction> tep_lookupTable = tep_obj->getLookupTable(tep_obj->mBaseGridBak, tep_colorColumn);
			if (tep_lookupTable == NULL)
				continue;
			vtkScalarBarWidget* tep_scalarBarWidget = vtkScalarBarWidget::New();
			if (tep_colorColumn.indexOf("_cell000") >= 0)
				tep = tep_colorColumn.left(tep_colorColumn.length() - 8);
			else
				tep = tep_colorColumn;
			tep_scalarBarWidget->GetScalarBarActor()->SetTitleTextProperty(propLable);
			tep_scalarBarWidget->GetScalarBarActor()->SetLabelTextProperty(propLable);
			tep_scalarBarWidget->GetScalarBarActor()->SetTitle(tep.toStdString().c_str());
			tep_scalarBarWidget->GetScalarBarActor()->SetVerticalTitleSeparation(1);
			tep_scalarBarWidget->GetScalarBarActor()->SetUnconstrainedFontSize(true);
			tep_scalarBarWidget->GetScalarBarActor()->SetBarRatio(0.1);
			tep_scalarBarWidget->GetBorderRepresentation()->SetPosition(0.05, 0.05);
			tep_scalarBarWidget->GetBorderRepresentation()->SetPosition2(0.08, 0.45);
			tep_scalarBarWidget->GetScalarBarActor()->SetLookupTable(tep_lookupTable);
			tep_scalarBarWidget->SetInteractor(renderWindow->GetInteractor());
			tep_scalarBarWidget->Off();
			tep_obj->scalarBar_widgetMap.insert(tep_colorColumn, tep_scalarBarWidget);
			//tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert(tep_colorColumn, false);
			//tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap.insert(tep_colorColumn, false);
		}
		generate_scalarBarWidget_GlyphVector(tep_obj);
		
		tep_obj->set_flag_generate_scalarBarWidget(true);//tep_obj->flag_generate_scalarBarWidget = true;
	}
}

void PipelineObjPlotForm::generate_scalarBarWidget_GlyphVector(NumericSrcObject *tep_obj)
{
	if (tep_obj->scalarBar_widgetMap.contains("GlyphVector"))
		return;
	vtkSmartPointer<vtkTextProperty> propLable = vtkSmartPointer<vtkTextProperty>::New();
	propLable->SetFontFamily(VTK_FONT_FILE);
	propLable->SetFontFile(mFontPath.toLocal8Bit().data());
	propLable->SetBold(0);
	propLable->SetItalic(0);
	propLable->SetShadow(0);
	propLable->SetJustification(VTK_TEXT_LEFT);
	propLable->SetColor(0, 0, 0);
	propLable->SetFontSize(scalarBar_FontSize);
	vtkScalarBarWidget* tep_scalarBarWidget = vtkScalarBarWidget::New();
	tep_scalarBarWidget->GetScalarBarActor()->SetTitleTextProperty(propLable);
	tep_scalarBarWidget->GetScalarBarActor()->SetLabelTextProperty(propLable);
	//tep_scalarBarWidget->GetScalarBarActor()->SetTitle(tep.toStdString().c_str());
	tep_scalarBarWidget->GetScalarBarActor()->SetVerticalTitleSeparation(1);
	tep_scalarBarWidget->GetScalarBarActor()->SetUnconstrainedFontSize(true);
	tep_scalarBarWidget->GetScalarBarActor()->SetBarRatio(0.1);
	tep_scalarBarWidget->GetBorderRepresentation()->SetPosition(0.05, 0.05);
	tep_scalarBarWidget->GetBorderRepresentation()->SetPosition2(0.08, 0.45);
	//tep_scalarBarWidget->GetScalarBarActor()->SetLookupTable(tep_lookupTable);
	tep_scalarBarWidget->SetInteractor(renderWindow->GetInteractor());
	tep_scalarBarWidget->Off();
	tep_obj->scalarBar_widgetMap.insert("GlyphVector", tep_scalarBarWidget);
	if (!tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.contains("GlyphVector"))
		tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert("GlyphVector", false);
	if (!tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.contains("GlyphVector"))
	{
		scalarBarTitle_PropData tep_titlePara;
		tep_titlePara.size = scalarBar_FontSize;
		tep_titlePara.num_lable = scalarBar_NumLables;
		tep_titlePara.title = "GlyphVector";
		tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.insert("GlyphVector", tep_titlePara);
	}	
	if (!tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap.contains("GlyphVector"))
		tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap.insert("GlyphVector", false);
}
void PipelineObjPlotForm::generate_scalarBarWidget_calculatorResult(NumericSrcObject *tep_obj, QString calResultName)
{
	if (tep_obj->scalarBar_widgetMap.contains("CalculatorResult"))
		return;
	vtkSmartPointer<vtkTextProperty> propLable = vtkSmartPointer<vtkTextProperty>::New();
	propLable->SetFontFamily(VTK_FONT_FILE);
	propLable->SetFontFile(mFontPath.toLocal8Bit().data());
	propLable->SetBold(0);
	propLable->SetItalic(0);
	propLable->SetShadow(0);
	propLable->SetJustification(VTK_TEXT_LEFT);
	propLable->SetColor(0, 0, 0);
	propLable->SetFontSize(scalarBar_FontSize);
	vtkScalarBarWidget* tep_scalarBarWidget = vtkScalarBarWidget::New();
	tep_scalarBarWidget->GetScalarBarActor()->SetTitleTextProperty(propLable);
	tep_scalarBarWidget->GetScalarBarActor()->SetLabelTextProperty(propLable);
	//tep_scalarBarWidget->GetScalarBarActor()->SetTitle(tep.toStdString().c_str());
	tep_scalarBarWidget->GetScalarBarActor()->SetVerticalTitleSeparation(1);
	tep_scalarBarWidget->GetScalarBarActor()->SetUnconstrainedFontSize(true);
	tep_scalarBarWidget->GetScalarBarActor()->SetBarRatio(0.1);
	tep_scalarBarWidget->GetBorderRepresentation()->SetPosition(0.05, 0.05);
	tep_scalarBarWidget->GetBorderRepresentation()->SetPosition2(0.08, 0.45);
	//tep_scalarBarWidget->GetScalarBarActor()->SetLookupTable(tep_lookupTable);
	tep_scalarBarWidget->SetInteractor(renderWindow->GetInteractor());
	tep_scalarBarWidget->Off();
	tep_obj->scalarBar_widgetMap.insert("CalculatorResult", tep_scalarBarWidget);
	if (!tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.contains("CalculatorResult"))
		tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert("CalculatorResult", false);
	if (!tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.contains("CalculatorResult"))
	{
		scalarBarTitle_PropData tep_titlePara;
		tep_titlePara.size = scalarBar_FontSize;
		tep_titlePara.num_lable = scalarBar_NumLables;
		tep_titlePara.title = calResultName;
		tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.insert("CalculatorResult", tep_titlePara);
	}
	if (!tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap.contains("CalculatorResult"))
		tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap.insert("CalculatorResult", false);
}
void PipelineObjPlotForm::slot_update_scalarBar(PipelineObject *tep_pipelineObj)
{	
	if (tep_pipelineObj == NULL)
		return;
	if (tep_pipelineObj->GetObjectType() != dNumeric_DataSource)
		return;
	QString tep_colorColumn;
	QMap<QString, bool>::iterator it;
	for (it = tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.begin(); it != tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.end(); it++)
	{
		if (tep_pipelineObj->scalarBar_widgetMap.contains(it.key()))
		{
			vtkScalarBarWidget* tep_scalarWidget = tep_pipelineObj->scalarBar_widgetMap[it.key()];
			if (tep_scalarWidget == NULL)
				return;
			if (it.value())
			{
					//动画颜色条范围变化
					//QString tep_colorColumn = it.key();
					//double mRange[2];
					//tep_pipelineObj->getcolorColumnRange(tep_colorColumn, mRange);
					//vtkColorTransferFunction* tep_lookupTable = tep_pipelineObj->getLookupTable(tep_pipelineObj->mBaseGrid, tep_colorColumn);
					////tep_lookupTable->AdjustRange(mRange);
					//if (tep_lookupTable != NULL)
					//{
					//	tep_scalarWidget->GetScalarBarActor()->SetLookupTable(tep_lookupTable);//tep_scalarWidget->GetScalarBarActor()->GetLookupTable()->SetRange(mRange);
					//	//动画颜色条范围变化
					//	tep_scalarWidget->On();
					//}
				vtkColorTransferFunction* tep_lookupTable = NULL;
				if (tep_pipelineObj->scalarBar_lookupTableMap.contains(it.key()))
				{
					tep_lookupTable = tep_pipelineObj->scalarBar_lookupTableMap[it.key()];
					tep_scalarWidget->GetScalarBarActor()->SetLookupTable(tep_lookupTable);
				}
				else
					continue;
				vtkPiecewiseFunction* tep_opacityFunction = NULL;
				if (tep_pipelineObj->scalarBar_lookupOpacityMap.contains(it.key()))
				{
					tep_opacityFunction= tep_pipelineObj->scalarBar_lookupOpacityMap[it.key()];
				}
				else
					continue;
				QString tep_title;
				if (tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.contains(it.key()))
				{
					scalarBarTitle_PropData tep_titlePara = tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap[it.key()];
					tep_title = tep_titlePara.title;
					tep_scalarWidget->GetScalarBarActor()->SetTitle(tep_title.toStdString().data());
					tep_scalarWidget->GetScalarBarActor()->SetNumberOfLabels(tep_titlePara.num_lable);
					tep_scalarWidget->GetScalarBarActor()->GetTitleTextProperty()->SetFontSize(tep_titlePara.size);
				}
				if (tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap.contains(it.key()))
				{
					if (tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap[it.key()])
					{
						vtkSmartPointer<vtkLookupTable> new_lookupTable = tep_pipelineObj->MakeLUTFromCTF(tep_lookupTable, tep_opacityFunction, 256);
						//if (tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible && (tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap[it.key()]))
						tep_pipelineObj->mVecActors[0]->GetMapper()->SetScalarRange(new_lookupTable->GetRange());
						tep_pipelineObj->mVecActors[0]->GetMapper()->SetLookupTable(new_lookupTable);
						tep_pipelineObj->mVecActors[0]->GetMapper()->ScalarVisibilityOn();
						/*else
						{
							for (int kk = 0; kk < cur_mPipelineObjs.count(); kk++)
							{
								PipelineObject* tep_obj = cur_mPipelineObjs.at(kk);
								if (tep_obj == NULL)
									continue;
								if (tep_obj->GetObjParent() != tep_pipelineObj)
									continue;
								if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible && (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle == it.key()))
								{
									if (tep_obj->mVecActors.count()>0)
									{
										tep_obj->mVecActors[0]->GetMapper()->SetLookupTable(new_lookupTable);
										break;
									}
								}
							}
						}*/
						tep_scalarWidget->GetScalarBarActor()->SetLookupTable(new_lookupTable);
						tep_scalarWidget->GetScalarBarActor()->UseOpacityOn();
					}
					else
					{
						tep_scalarWidget->GetScalarBarActor()->UseOpacityOff();
						tep_scalarWidget->GetScalarBarActor()->SetLookupTable(tep_lookupTable);
					}
				}
				tep_scalarWidget->On();
			}
			else
				tep_scalarWidget->Off();
		}
	}
	if (flag_render)
		renderWindow->Render();
}
void PipelineObjPlotForm::del_obj_scalarBarWidget(PipelineObject *pipeObj)
{
	QString tep_colorColumn, tep;
	if (pipeObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible && (pipeObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle != "SolidColor"))
	{
		if (pipeObj->GetObjectType() == dNumeric_DataSource)
		{
			QMap<QString, vtkScalarBarWidget*>::iterator it;
			for (it = pipeObj->scalarBar_widgetMap.begin(); it != pipeObj->scalarBar_widgetMap.end(); it++)
			{
				it.value()->Off();
				it.value()->Delete();
			}
		}
		else
		{
			if (pipeObj->GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap[pipeObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle] && pipeObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible)
			{
				pipeObj->GetObjParent()->scalarBar_widgetMap[pipeObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle]->Off();
				pipeObj->GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert(pipeObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle, false);

			}
			pipeObj->GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors = true;
			pipeObj->GetObjParent()->ShowActor();
		}
	}
	
	if (flag_render)
		renderWindow->Render();
}

void PipelineObjPlotForm::slot_offPlaneWidget(PipelineObject *pipeObj)
{
	if (pipeObj->GetObjectType() == dClip_DataSource)
	{
		((FilterClip*)pipeObj)->plane_visible(false);
	}
	else if (pipeObj->GetObjectType() == dSlice_DataSource)
	{
		((FilterSlice*)pipeObj)->plane_visible(false);
	}
}

Camera_Para PipelineObjPlotForm::getCameraPara()
{
	Camera_Para tep_data;
	vtkCamera* tep_camera = renderer->GetActiveCamera();
	if (tep_camera != NULL)
	{
		tep_data.position[0]=tep_camera->GetPosition()[0];
		tep_data.position[1] = tep_camera->GetPosition()[1];
		tep_data.position[2] = tep_camera->GetPosition()[2];
		tep_data.focalPoint[0]=tep_camera->GetFocalPoint()[0];
		tep_data.focalPoint[1] = tep_camera->GetFocalPoint()[1];
		tep_data.focalPoint[2] = tep_camera->GetFocalPoint()[2];
		tep_data.clippingRange[0] = tep_camera->GetClippingRange()[0];
		tep_data.clippingRange[1] = tep_camera->GetClippingRange()[1];
		tep_data.viewUp[0]=tep_camera->GetViewUp()[0];
		tep_data.viewUp[1] = tep_camera->GetViewUp()[1];
		tep_data.viewUp[2] = tep_camera->GetViewUp()[2];
		tep_data.viewAngle = tep_camera->GetViewAngle();
		tep_camera->Zoom(1.0);
		tep_data.parallel_scale = tep_camera->GetParallelScale();
	}
	return tep_data;
}

void PipelineObjPlotForm::setCameraPara_Position(double pos[3])
{
	vtkCamera *tep_camera = renderer->GetActiveCamera();
	if (tep_camera == NULL)
		return;
	tep_camera->SetPosition(pos[0], pos[1], pos[2]);
	if (flag_render)
		renderWindow->Render();
}

void PipelineObjPlotForm::setCameraPara_FocalPoint(double focalPoint[3])
{
	vtkCamera *tep_camera = renderer->GetActiveCamera();
	if (tep_camera == NULL)
		return;
	tep_camera->SetFocalPoint(focalPoint[0], focalPoint[1], focalPoint[2]);
	if (flag_render)
		renderWindow->Render();
}

void PipelineObjPlotForm::setCameraPara_ClippingRange(double clippingRange[2])
{
	vtkCamera *tep_camera = renderer->GetActiveCamera();
	if (tep_camera == NULL)
		return;
	tep_camera->SetClippingRange(clippingRange[0], clippingRange[1]);
	if (flag_render)
		renderWindow->Render();
}

void PipelineObjPlotForm::setCameraPara_ViewUp(double viewup[3])
{
	vtkCamera *tep_camera = renderer->GetActiveCamera();
	if (tep_camera == NULL)
		return;
	tep_camera->SetViewUp(viewup[0], viewup[1], viewup[2]);
	if (flag_render)
		renderWindow->Render();
}

void PipelineObjPlotForm::setCameraPara_ViewAngle(double angle)
{
	vtkCamera *tep_camera = renderer->GetActiveCamera();
	if (tep_camera == NULL)
		return;
	tep_camera->SetViewAngle(angle);
	if (flag_render)
		renderWindow->Render();
}
void PipelineObjPlotForm::setCameraPara_ParalellScale(double scale)
{
	vtkCamera *tep_camera = renderer->GetActiveCamera();
	if (tep_camera == NULL)
		return;
	tep_camera->SetParallelScale(scale);
	if (flag_render)
		renderWindow->Render();
}
void PipelineObjPlotForm::setCameraPara_Reset()
{
	vtkCamera *tep_camera = renderer->GetActiveCamera();
	if (tep_camera == NULL)
		return;
	renderer->ResetCamera();
	if (flag_render)
		renderWindow->Render();
}
void PipelineObjPlotForm::setCameraPara_Zoom(double zoom)
{
	vtkCamera *tep_camera = renderer->GetActiveCamera();
	if (tep_camera == NULL)
		return;
	tep_camera->Zoom(zoom); 
	if (flag_render)
		renderWindow->Render();
}
void PipelineObjPlotForm::setCameraPara_SelfAdaptation(bool flag)
{
	flag_selfAdaptation = flag;
}
void PipelineObjPlotForm::resetView()
{
	renderWindow->GetRenderers()->GetFirstRenderer()->ResetCamera();
	renderWindow->Render();
}
void PipelineObjPlotForm::setViewValueDirection(int x1, int x2, int x3, int y1, int y2, int y3, int z1, int z2, int z3)
{
	renderWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera()->SetViewUp(x1, x2, x3);
	renderWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera()->SetPosition(y1, y2, y3);
	renderWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera()->SetFocalPoint(z1, z2, z3);
	renderWindow->GetRenderers()->GetFirstRenderer()->ResetCamera();
	if (flag_render)
		renderWindow->Render();
}

void PipelineObjPlotForm::setViewDirection(QString val)
{
	if (val.toLower() == "xplus")
	{
		renderWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera()->SetViewUp(0, 0, 1);
		renderWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera()->SetPosition(5000, 0, 0);                               
		renderWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera()->SetFocalPoint(0, 0, 0);
		renderWindow->GetRenderers()->GetFirstRenderer()->ResetCamera();
		if (flag_render)
			renderWindow->Render();
	}
	else if (val.toLower() == "xminus")
	{
		renderWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera()->SetViewUp(0, 0, 1);
		renderWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera()->SetPosition(-5000, 0, 0);                               
		renderWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera()->SetFocalPoint(0, 0, 0);
		renderWindow->GetRenderers()->GetFirstRenderer()->ResetCamera();
		if (flag_render)
			renderWindow->Render();
	}
	else if (val.toLower() == "yplus")
	{
		renderWindow->GetRenderers()->GetFirstRenderer()->ResetCamera();
		renderWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera()->SetViewUp(0, 0, 1);
		renderWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera()->SetPosition(0, 5000, 0);                               
		renderWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera()->SetFocalPoint(0, 0, 0);
		renderWindow->GetRenderers()->GetFirstRenderer()->ResetCamera();
		if (flag_render)
			renderWindow->Render();
	}
	else if (val.toLower() == "yminus")
	{
		renderWindow->GetRenderers()->GetFirstRenderer()->ResetCamera();
		renderWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera()->SetViewUp(0, 0, 1);
		renderWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera()->SetPosition(0, -5000, 0);                               
		renderWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera()->SetFocalPoint(0, 0, 0);
		renderWindow->GetRenderers()->GetFirstRenderer()->ResetCamera();
		if (flag_render)
			renderWindow->Render();
	}
	else if (val.toLower() == "zplus")
	{
		renderWindow->GetRenderers()->GetFirstRenderer()->ResetCamera();
		renderWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera()->SetViewUp(0, 1, 0);
		renderWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera()->SetPosition(0, 0, 5000);                               
		renderWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera()->SetFocalPoint(0, 0, 0);
		renderWindow->GetRenderers()->GetFirstRenderer()->ResetCamera();
		if (flag_render)
			renderWindow->Render();
	}
	else if (val.toLower() == "zminus")
	{
		renderWindow->GetRenderers()->GetFirstRenderer()->ResetCamera();
		renderWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera()->SetViewUp(0, 1, 0);
		renderWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera()->SetPosition(0, 0, -5000);                             
		renderWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera()->SetFocalPoint(0, 0, 0);
		renderWindow->GetRenderers()->GetFirstRenderer()->ResetCamera();
		if (flag_render)
			renderWindow->Render();
	}
}

bool PipelineObjPlotForm::load_objFile(NumericSrcObject* NumericSrcObj, QString obj_fileName)
{
	int num_actors1 = renderWindow->GetRenderers()->GetFirstRenderer()->GetActors()->GetNumberOfItems();
	QString mtl_fileName = obj_fileName;mtl_fileName.replace(".obj", ".mtl");
	if (NumericSrcObj->model_ptr==NULL)
		NumericSrcObj->model_ptr = vtkAssembly::New();
	vtkSmartPointer<vtkOBJImporter> model_importer = vtkSmartPointer<vtkOBJImporter>::New();
	model_importer->SetFileName(obj_fileName.toStdString().data());
	model_importer->SetFileNameMTL(mtl_fileName.toStdString().data());
	model_importer->SetRenderWindow(renderWindow);
	model_importer->Update();
	renderWindow->Render();
	vtkSmartPointer<vtkActorCollection> cur_actor_list = renderWindow->GetRenderers()->GetFirstRenderer()->GetActors();
	cur_actor_list->InitTraversal();
	int num_actors2 = cur_actor_list->GetNumberOfItems();
	int num_actors_for_obj = num_actors2 - num_actors1;
	NumericSrcObj->mVecActors.clear();
	for (int i = 0; i<num_actors_for_obj; i++)
	{
		vtkSmartPointer<vtkActor> tep_actor = cur_actor_list->GetNextActor();
		//tep_actor->SetPosition(4.25, 19.24, 5.48);
		tep_actor->SetBackfaceProperty(NumericSrcObj->mBackFaceProp);
		NumericSrcObj->model_ptr->AddPart(tep_actor);
		NumericSrcObj->mVecActors.push_back(tep_actor);
		renderer->RemoveActor(tep_actor);
	}
	//renderer->AddActor(NumericSrcObj->model_ptr);
	//renderWindow->Render();
	//NumericSrcObj->flag_exist_actors = true;
	return true;
}

void PipelineObjPlotForm::disconnect_pick()
{
	if (pick_Data.flag_pickMode != -1)
	{
		if (pick_Data.flag_pickMode == 0)
		{
			if (vtk_mousePicker_connector != NULL)
			{
				if (!pick_Data.flag_draw_curve)
					vtk_mousePicker_connector->Disconnect(renderWindowInteractor, vtkCommand::LeftButtonPressEvent, this, SLOT(slot_pointPicker()), 0);
				else
					vtk_mousePicker_connector->Disconnect(renderWindowInteractor, vtkCommand::LeftButtonPressEvent, this, SLOT(slot_curvePointPicker()), 0);
				renderWindowInteractor->ReInitialize();
			}
		}
		else if (pick_Data.flag_pickMode == 1)
		{
			if (vtk_mousePicker_connector != NULL)
			{
				if (!pick_Data.flag_draw_curve)
					vtk_mousePicker_connector->Disconnect(renderWindowInteractor, vtkCommand::LeftButtonPressEvent, this, SLOT(slot_cellPicker()), 0);
				else
					vtk_mousePicker_connector->Disconnect(renderWindowInteractor, vtkCommand::LeftButtonPressEvent, this, SLOT(slot_curveCellPicker()), 0);
				renderWindowInteractor->ReInitialize();
			}
		}
		else if ((pick_Data.flag_pickMode == 2) || (pick_Data.flag_pickMode == 3))
		{
			renderWindowInteractor->SetInteractorStyle(mouse_style);
			renderWindowInteractor->ReInitialize();
			if (areaPick_style != NULL)
			{
				areaPick_style->Delete();
				areaPick_style = NULL;
			}
			//slot_restoreInteractor();
		}
	}
	if (flag_render)
		renderWindow->Render();
}

void PipelineObjPlotForm::set_pickPointMode()
{
	pick_Data.flag_draw_curve = false;
	pick_Data.pick_curve_data.clear();
	pick_Data.flag_pickMode = 0;
	if (vtk_mousePicker_connector == NULL)
		vtk_mousePicker_connector = vtkSmartPointer<vtkEventQtSlotConnect>::New();
	vtk_mousePicker_connector->Connect(renderWindowInteractor, vtkCommand::LeftButtonPressEvent, this, SLOT(slot_pointPicker()), 0, 1.0);
	ui->qvtkWidget->setCursor(Qt::CrossCursor);
	if (flag_render)
		renderWindow->Render();
}

void PipelineObjPlotForm::set_pickCellMode()
{
	pick_Data.flag_draw_curve = false;
	pick_Data.pick_curve_data.clear();
	pick_Data.flag_pickMode = 1;
	if (vtk_mousePicker_connector == NULL)
		vtk_mousePicker_connector = vtkSmartPointer<vtkEventQtSlotConnect>::New();
	vtk_mousePicker_connector->Connect(renderWindowInteractor, vtkCommand::LeftButtonPressEvent, this, SLOT(slot_cellPicker()), 0, 1.0);
	ui->qvtkWidget->setCursor(Qt::CrossCursor);
	if (flag_render)
		renderWindow->Render();
}

void PipelineObjPlotForm::set_pickCurvePointMode()
{
	pick_Data.flag_draw_curve = true;
	pick_Data.pick_curve_data.clear();
	pick_Data.flag_pickMode = 0;
	if (vtk_mousePicker_connector == NULL)
		vtk_mousePicker_connector = vtkSmartPointer<vtkEventQtSlotConnect>::New();
	vtk_mousePicker_connector->Connect(renderWindowInteractor, vtkCommand::LeftButtonPressEvent, this, SLOT(slot_curvePointPicker()), 0, 1.0);
	ui->qvtkWidget->setCursor(Qt::CrossCursor);
	if (flag_render)
		renderWindow->Render();
}

void PipelineObjPlotForm::set_pickCurveCellMode()
{
	pick_Data.flag_draw_curve = true;
	pick_Data.pick_curve_data.clear();
	pick_Data.flag_pickMode = 1;
	if (vtk_mousePicker_connector == NULL)
		vtk_mousePicker_connector = vtkSmartPointer<vtkEventQtSlotConnect>::New();
	vtk_mousePicker_connector->Connect(renderWindowInteractor, vtkCommand::LeftButtonPressEvent, this, SLOT(slot_curveCellPicker()), 0, 1.0);
	ui->qvtkWidget->setCursor(Qt::CrossCursor);
	if (flag_render)
		renderWindow->Render();
}

void PipelineObjPlotForm::set_pickAreaMode(int flag, bool flag_viewInteraction)
{
	if ((flag != 2) && (flag != 3))
		return;
	flag_cur_viewInteraction = flag_viewInteraction;
	pick_Data.flag_draw_curve = false;
	pick_Data.pick_curve_data.clear();
	pick_Data.flag_pickMode = flag;
	vtkSmartPointer<vtkAreaPicker> areaPicker = vtkSmartPointer<vtkAreaPicker>::New();
	renderWindowInteractor->SetPicker(areaPicker);
	if (areaPick_style != NULL)
	{
		areaPick_style->Delete();
		areaPick_style = NULL;
	}
	areaPick_style = PickAreaInteractorStyle::New();
	connect(areaPick_style, SIGNAL(sig_updatePickData(struct pick_pipelineObj_propData)), this, SLOT(slot_updatePickData(struct pick_pipelineObj_propData)));
	areaPick_style->SetAreaPicker(areaPicker);
	areaPick_style->SetDefaultMouseStyle(mouse_style);
	areaPick_style->SetPipelineObjectList(cur_mPipelineObjs);
	areaPick_style->SetInteractor(renderWindowInteractor);
	//connect(areaPick_style, SIGNAL(sig_restoreInteractor()), this, SLOT(slot_restoreInteractor()));
	pick_Data.flag_pickMode = flag;
	areaPick_style->SetConsoleForm(consoleWidget_ptr);
	if (pick_Data.pick_actor == NULL)
	{
		vtkSmartPointer<vtkProperty> mBackFaceProp = vtkSmartPointer<vtkProperty>::New();
		pick_Data.pick_actor = vtkActor::New();
		pick_Data.pick_actor->SetBackfaceProperty(mBackFaceProp);
		pick_Data.pick_actor->GetProperty()->SetColor(1.0, 1.0, 1.0);
		pick_Data.pick_actor->GetBackfaceProperty()->SetColor(1.0, 1.0, 1.0);
		renderer->AddActor(pick_Data.pick_actor);
	}
	ui->qvtkWidget->setCursor(Qt::CrossCursor);
	if (flag_render)
		renderWindow->Render();
	pick_Data.pick_pipelineObj = NULL;
	if (pick_Data.pick_areaCell == NULL)
		pick_Data.pick_areaCell = vtkIdList::New();
	if (pick_Data.pick_areaPoint == NULL)
		pick_Data.pick_areaPoint = vtkIdList::New();
	areaPick_style->SetPickData(pick_Data);
	areaPick_style->SetInteractor(renderWindowInteractor);
	renderWindowInteractor->SetInteractorStyle(areaPick_style);	
	renderWindowInteractor->ReInitialize();
	renderWindowInteractor->Start();
	//QWSServer::sendKeyEvent(int('F'), Qt::Key_F, Qt::NoModifier, true, false);
	//QKeyEvent key_r(QEvent::KeyPress,Qt::Key_R, Qt::NoModifier);
	//QCoreApplication::sendEvent(this, &key_r);
	
}
void PipelineObjPlotForm::slot_updatePickData(struct pick_pipelineObj_propData tep_pickData)
{
	pick_Data.flag_pickMode = tep_pickData.flag_pickMode;
	pick_Data.objId = tep_pickData.objId;
	pick_Data.pickArea_objId = tep_pickData.pickArea_objId;
	pick_Data.pick_actor = tep_pickData.pick_actor;
	pick_Data.pick_areaCell = tep_pickData.pick_areaCell;
	pick_Data.pick_areaPoint = tep_pickData.pick_areaPoint;
	pick_Data.pick_pipelineObj = tep_pickData.pick_pipelineObj;
}
bool PipelineObjPlotForm::func_cellPicker()
{
	bool flag= false;
	int* pos = renderWindowInteractor->GetEventPosition();
	vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
	picker->SetTolerance(0.0005);
	// Pick from this location.
	picker->Pick(pos[0], pos[1], 0, renderer);
	//double* worldPosition = picker->GetPickPosition();
	std::cout << "Cell id is: " << picker->GetCellId() << std::endl;
	vtkActor* tep_actor = picker->GetActor();
	vtkIdType tep_cellId = picker->GetCellId();
	if ((tep_actor == NULL) || (tep_cellId == -1))
	{
		//vtk_mousePicker_connector->Disconnect(renderWindowInteractor, vtkCommand::LeftButtonPressEvent, this, SLOT(slot_cellPicker()), 0);
		return false;
	}
	for (int i = 0; i < cur_mPipelineObjs.count(); i++)
	{
		if (cur_mPipelineObjs[i]->mBaseGrid->GetNumberOfCells() <= tep_cellId)
			continue;
		for (int j = 0; j < cur_mPipelineObjs[i]->mVecActors.count(); j++)
		{
			if (tep_actor == cur_mPipelineObjs[i]->mVecActors[j])
			{
				pick_Data.pick_pipelineObj = cur_mPipelineObjs[i];
				if (pick_Data.flag_draw_curve)
				{
					if (pick_Data.pick_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle == "SolidColor")
						return false;
					if (pick_Data.pick_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle.indexOf("_cell000") < 0)
						return false;
				}
				draw_PickCell(pick_Data.pick_pipelineObj, tep_cellId);
				flag = true;
				break;
			}
		}
	}
	/*for (int i = 0; i < cur_mPipelineObjs.count(); i++)
	{
	if (cur_mPipelineObjs[i]->mPipeLineObjProp.pipelineObj_base_propData.obj_id == pickActor_Id)
	{
	pick_Data.pick_pipelineObj = cur_mPipelineObjs[i];
	draw_PickCell(pick_Data.pick_pipelineObj, tep_cellId);
	break;
	}
	}*/
	//vtk_mousePicker_connector->Disconnect(renderWindowInteractor, vtkCommand::LeftButtonPressEvent, this, SLOT(slot_cellPicker()), 0);
	return flag;
}

void PipelineObjPlotForm::slot_cellPicker()
{
	func_cellPicker();
	//emit sig_pickCellAct_status(true);
}

bool PipelineObjPlotForm::func_pointPicker()
{
	bool flag = false;
	int* pos = renderWindowInteractor->GetEventPosition();
	vtkSmartPointer<vtkPointPicker> picker = vtkSmartPointer<vtkPointPicker>::New();
	picker->SetTolerance(0.005);
	picker->Pick(pos[0], pos[1], 0, renderer);
	//double* worldPosition = picker->GetPickPosition();
	//std::cout << "Point id is: " << picker->GetPointId() << std::endl;
	vtkActor* tep_actor = picker->GetActor();
	vtkIdType tep_pointId = picker->GetPointId();
	if ((tep_actor == NULL) || (tep_pointId == -1))
	{
		//vtk_mousePicker_connector->Disconnect(renderWindowInteractor, vtkCommand::LeftButtonPressEvent, this, SLOT(slot_pointPicker()), 0);
		return false;
	}
	for (int i = 0; i < cur_mPipelineObjs.count(); i++)
	{
		if (cur_mPipelineObjs[i]->mBaseGrid->GetNumberOfPoints() <= tep_pointId)
			continue;
		for (int j = 0; j < cur_mPipelineObjs[i]->mVecActors.count(); j++)
		{
			if (tep_actor == cur_mPipelineObjs[i]->mVecActors[j])
			{
				pick_Data.pick_pipelineObj = cur_mPipelineObjs[i];
				
				if (pick_Data.flag_draw_curve)
				{
					if (pick_Data.pick_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle == "SolidColor")
						return false;
					if (pick_Data.pick_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle.indexOf("_cell000") >= 0)
						return false;
				}
				
				draw_PickPoint(pick_Data.pick_pipelineObj, tep_pointId);
				flag = true;
				break;
			}
		}
	}
	//vtk_mousePicker_connector->Disconnect(renderWindowInteractor, vtkCommand::LeftButtonPressEvent, this, SLOT(slot_pointPicker()), 0);
	return flag;
}

void PipelineObjPlotForm::slot_pointPicker()
{
	func_pointPicker();
	//emit sig_pickPointAct_status(true);
}

void PipelineObjPlotForm::clear_pickCurveData()
{
	pick_Data.pick_curve_data.clear();
}

void PipelineObjPlotForm::slot_curvePointPicker()
{
	func_pointPicker();
	
}

void PipelineObjPlotForm::slot_curveCellPicker()
{
	func_cellPicker();
}

void PipelineObjPlotForm::draw_PickPoint(PipelineObject * tep_pipelineObj, vtkIdType pick_Id)
{
	if (tep_pipelineObj == NULL)
		return;
	if (tep_pipelineObj->mPipelineDataSet == NULL)
		return;
	if ((pick_Id < 0) || (pick_Id >= tep_pipelineObj->mPipelineDataSet->GetNumberOfPoints()))
		return;
	QString outstr, tepstr;
	outstr = QString("Point_ID : %1 (").arg(pick_Id);
	double* worldPosition = tep_pipelineObj->mPipelineDataSet->GetPoint(pick_Id);
	for (int i = 0; i < 3; i++)
	{
		outstr += tepstr.setNum(worldPosition[i]);
		if (i != 2)
			outstr += ",";
		else
			outstr += ")";
	}
	emit sig_addInfoToConsole(outstr);

	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	points->InsertNextPoint(worldPosition);

	vtkSmartPointer<vtkVertex> aVertex = vtkSmartPointer<vtkVertex>::New();
	aVertex->GetPointIds()->SetId(0, 0);

	vtkSmartPointer<vtkUnstructuredGrid> grid = vtkSmartPointer<vtkUnstructuredGrid>::New();
	grid->Allocate(1, 1);
	grid->InsertNextCell(aVertex->GetCellType(), aVertex->GetPointIds());
	grid->SetPoints(points);
	if (pick_Data.pick_actor == NULL)
	{
		vtkSmartPointer<vtkProperty> mBackFaceProp = vtkSmartPointer<vtkProperty>::New();
		pick_Data.pick_actor = vtkActor::New();
		pick_Data.pick_actor->SetBackfaceProperty(mBackFaceProp);
		pick_Data.pick_actor->GetProperty()->SetColor(1.0, 1.0, 1.0);
		pick_Data.pick_actor->GetBackfaceProperty()->SetColor(1.0, 1.0, 1.0);
	}
	vtkSmartPointer<vtkDataSetMapper> selectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
	selectedMapper->SetInputData(grid);
	selectedMapper->ScalarVisibilityOff();
	pick_Data.pick_actor->SetMapper(selectedMapper);
	pick_Data.pick_actor->GetProperty()->SetRepresentationToPoints();
	pick_Data.pick_actor->GetProperty()->SetPointSize(4);
	pick_Data.flag_pickMode = 0;
	pick_Data.pick_id = pick_Id;
	if (pick_Data.flag_draw_curve)
	{
		if (tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.fileList.count() > 1)//20180410 add
		{
			double val;
			pick_Data.pick_pipelineObj->get_prop_val(pick_Data.pick_id, true, &val);
			pick_Data.pick_curve_data.append(val);
			if (pick_Data.pick_curve_data.count() < 2)//if (pick_Data.pick_curve_data.count() <= 2)
				emit sig_draw_3d_curve(pick_Data.pick_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle, pick_Data.pick_curve_data);
			else
				emit sig_update_3d_curve(0, 0, pick_Data.pick_curve_data.count()*1.0, val);
		}
	}
	//pick_Data.pick_actor = selectedActor;
	renderer->AddActor(pick_Data.pick_actor);
	if (flag_render)
		renderWindow->Render();
	emit sig_script_refresh_pickData(pick_Data);
}

void PipelineObjPlotForm::draw_PickCell(PipelineObject * tep_pipelineObj, vtkIdType pick_Id)
{
	if (tep_pipelineObj == NULL)
		return;
	if ((pick_Id < 0) || (pick_Id >= tep_pipelineObj->mPipelineDataSet->GetNumberOfCells()))
		return;
	QString outstr, tepstr;
	outstr = QString("Cell_ID : %1 (").arg(pick_Id);
	vtkCell* tep_cell = tep_pipelineObj->mPipelineDataSet->GetCell(pick_Id);
	vtkIdList *tep_idList = tep_cell->GetPointIds();
	for (vtkIdType i = 0; i < tep_idList->GetNumberOfIds(); i++)
	{
		outstr += tepstr.setNum(tep_idList->GetId(i));
		if (i != (tep_idList->GetNumberOfIds() - 1))
			outstr += ",";
		else
			outstr += ")";
	}
	emit sig_addInfoToConsole(outstr);

	vtkSmartPointer<vtkIdTypeArray> ids = vtkSmartPointer<vtkIdTypeArray>::New();
	ids->SetNumberOfComponents(1);
	ids->InsertNextValue(pick_Id);

	vtkSmartPointer<vtkSelectionNode> selectionNode = vtkSmartPointer<vtkSelectionNode>::New();
	selectionNode->SetFieldType(vtkSelectionNode::CELL);
	selectionNode->SetContentType(vtkSelectionNode::INDICES);
	selectionNode->SetSelectionList(ids);

	vtkSmartPointer<vtkSelection> selection = vtkSmartPointer<vtkSelection>::New();
	selection->AddNode(selectionNode);

	vtkSmartPointer<vtkExtractSelection> extractSelection = vtkSmartPointer<vtkExtractSelection>::New();
#if VTK_MAJOR_VERSION <= 5
	extractSelection->SetInput(0, this->Data);
	extractSelection->SetInput(1, selection);
#else
	extractSelection->SetInputData(0, tep_pipelineObj->mPipelineDataSet);
	extractSelection->SetInputData(1, selection);
#endif
	extractSelection->Update();

	// In selection
	vtkSmartPointer<vtkUnstructuredGrid> selected = vtkSmartPointer<vtkUnstructuredGrid>::New();
	selected->ShallowCopy(extractSelection->GetOutput());

	std::cout << "There are " << selected->GetNumberOfPoints() << " points in the selection." << std::endl;
	std::cout << "There are " << selected->GetNumberOfCells() << " cells in the selection." << std::endl;
	vtkSmartPointer<vtkDataSetMapper> selectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
#if VTK_MAJOR_VERSION <= 5
	selectedMapper->SetInputConnection(selected->GetProducerPort());
#else
	selectedMapper->SetInputData(selected);
#endif
	selectedMapper->ScalarVisibilityOff();
	if (pick_Data.pick_actor == NULL)
	{
		vtkSmartPointer<vtkProperty> mBackFaceProp = vtkSmartPointer<vtkProperty>::New();
		pick_Data.pick_actor = vtkActor::New();
		pick_Data.pick_actor->SetBackfaceProperty(mBackFaceProp);
		pick_Data.pick_actor->GetProperty()->SetColor(1.0, 1.0, 1.0);
		pick_Data.pick_actor->GetBackfaceProperty()->SetColor(1.0, 1.0, 1.0);
		renderer->AddActor(pick_Data.pick_actor);
	}
	pick_Data.pick_actor->SetMapper(selectedMapper);
	pick_Data.pick_actor->GetProperty()->SetRepresentationToWireframe();
	pick_Data.pick_actor->GetProperty()->EdgeVisibilityOn();
	pick_Data.pick_actor->GetProperty()->SetEdgeColor(1.0, 1.0, 1.0);
	pick_Data.pick_actor->GetProperty()->SetLineWidth(3);
	pick_Data.pick_actor->GetProperty()->SetPointSize(1); 
	pick_Data.flag_pickMode = 1;
	pick_Data.pick_id = pick_Id;
	if (pick_Data.flag_draw_curve)
	{
		if (tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.fileList.count() > 1)//20180410 add
		{
			double val;
			pick_Data.pick_pipelineObj->get_prop_val(pick_Data.pick_id, false, &val);
			pick_Data.pick_curve_data.append(val);
			tepstr = pick_Data.pick_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle;
			tepstr = tepstr.left(tepstr.length() - 8);
			if (pick_Data.pick_curve_data.count() < 2)//if (pick_Data.pick_curve_data.count() <= 2)
				emit sig_draw_3d_curve(tepstr, pick_Data.pick_curve_data);
			else
				emit sig_update_3d_curve(0, 0, pick_Data.pick_curve_data.count()*1.0, val);
		}
	}
	//pick_Data.pick_actor = selectedActor;
	if (flag_render)
		renderWindow->Render();
	emit sig_script_refresh_pickData(pick_Data);
}

QWidget *PipelineObjPlotForm::get_vtkWidget()
{
	return ui->qvtkWidget;
}

void PipelineObjPlotForm::draw_PickArea(PipelineObject * tep_pipelineObj, int flag_pickMode)//2point 3cell
{
	if (tep_pipelineObj == NULL)
	{
		for (int i = 0; i < cur_mPipelineObjs.count(); i++)
		{
			if (cur_mPipelineObjs[i]->mPipeLineObjProp.pipelineObj_base_propData.obj_id == *(pick_Data.pickArea_objId))
			{
				pick_Data.pick_pipelineObj = cur_mPipelineObjs[i];
				tep_pipelineObj = cur_mPipelineObjs[i];
				break;
			}
		}
	}
	
	vtkDataSet *dataSet = tep_pipelineObj->mBaseGrid;
	if (dataSet == NULL)
		return;
	if ((flag_pickMode != 2) && (flag_pickMode != 3))
		return;
	if ((pick_Data.pick_areaCell->GetNumberOfIds()< 1) || (pick_Data.pick_areaPoint->GetNumberOfIds() < 1))
		return;

	vtkSmartPointer<vtkIdTypeArray> ids = vtkSmartPointer<vtkIdTypeArray>::New();
	ids->SetNumberOfComponents(1);
	for (vtkIdType i = 0; i < pick_Data.pick_areaCell->GetNumberOfIds(); i++)
	{
		ids->InsertNextValue(pick_Data.pick_areaCell->GetId(i));
		//std::cout << "refresh cell id:---------" << pick_Data.pick_areaCell->GetId(i) << std::endl;
	}
	vtkSmartPointer<vtkSelectionNode> selectionNode = vtkSmartPointer<vtkSelectionNode>::New();
	selectionNode->SetFieldType(vtkSelectionNode::CELL);
	selectionNode->SetContentType(vtkSelectionNode::INDICES);
	selectionNode->SetSelectionList(ids);

	vtkSmartPointer<vtkSelection> selection = vtkSmartPointer<vtkSelection>::New();
	selection->AddNode(selectionNode);

	vtkSmartPointer<vtkExtractSelection> extractSelection = vtkSmartPointer<vtkExtractSelection>::New();
	extractSelection->SetInputData(0, dataSet);
	extractSelection->SetInputData(1, selection);
	extractSelection->Update();

	// In selection
	vtkSmartPointer<vtkUnstructuredGrid> selected = vtkSmartPointer<vtkUnstructuredGrid>::New();
	selected->ShallowCopy(extractSelection->GetOutput());
	vtkSmartPointer<vtkDataSetMapper> selectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
#if VTK_MAJOR_VERSION <= 5
	selectedMapper->SetInputConnection(selected->GetProducerPort());
#else
	selectedMapper->SetInputData(selected);
#endif
	selectedMapper->ScalarVisibilityOff();
	if (pick_Data.pick_actor == NULL)
	{
		vtkSmartPointer<vtkProperty> mBackFaceProp = vtkSmartPointer<vtkProperty>::New();
		pick_Data.pick_actor = vtkActor::New();
		pick_Data.pick_actor->SetBackfaceProperty(mBackFaceProp);
		pick_Data.pick_actor->GetProperty()->SetColor(1.0, 1.0, 1.0);
		pick_Data.pick_actor->GetBackfaceProperty()->SetColor(1.0, 1.0, 1.0);
	}
	pick_Data.pick_actor->SetMapper(selectedMapper);
	if (pick_Data.flag_pickMode == 3)
	{
		//pick_Data.pick_actor->GetProperty()->EdgeVisibilityOn();
		pick_Data.pick_actor->GetProperty()->SetRepresentationToWireframe();
		pick_Data.pick_actor->GetProperty()->SetLineWidth(3);
		pick_Data.pick_actor->GetProperty()->SetEdgeColor(1.0, 1.0, 1.0);
		pick_Data.pick_actor->GetProperty()->SetPointSize(1);
	}
	else if (pick_Data.flag_pickMode == 2)
	{
		pick_Data.pick_actor->GetProperty()->SetRepresentationToPoints();
		pick_Data.pick_actor->GetProperty()->SetPointSize(4);
	}
	renderer->AddActor(pick_Data.pick_actor);
	if (flag_render)
		renderWindow->Render();
}

void PipelineObjPlotForm::del_pipelineObj_pickPlot(PipelineObject* pipeObj)
{
	if (pick_Data.pick_pipelineObj == NULL)
		return;
	if (pick_Data.pick_pipelineObj != pipeObj)
		return;
	disconnect_pick();
	if (pick_Data.pick_actor != NULL)
	{
		renderer->RemoveActor(pick_Data.pick_actor);
		pick_Data.pick_actor->Delete();
		pick_Data.pick_actor = NULL;
	}
	if (pick_Data.pick_areaCell != NULL)
		pick_Data.pick_areaCell->Resize(0);
	if (pick_Data.pick_areaPoint != NULL)
		pick_Data.pick_areaPoint->Resize(0);
	pick_Data.flag_pickMode = -1;
	pick_Data.flag_draw_curve = false;
	pick_Data.pick_curve_data.clear();
}

void PipelineObjPlotForm::slot_refreshPicker()
{
	if ((pick_Data.flag_pickMode<0) || (pick_Data.flag_pickMode>3))
		return;
	if ((pick_Data.flag_pickMode == 0) || (pick_Data.flag_pickMode == 1))
	{
		if ((pick_Data.pick_id < 0) && (pick_Data.pick_pipelineObj == NULL))
			return;
	}
	
	vtkIdType cur_pickId;
	if (pick_Data.flag_pickMode == 0)//point
	{
		cur_pickId = pick_Data.pick_id;
		draw_PickPoint(pick_Data.pick_pipelineObj, cur_pickId);
	}
	else if (pick_Data.flag_pickMode == 1)//cell
	{
		cur_pickId = pick_Data.pick_id;
		draw_PickCell(pick_Data.pick_pipelineObj, cur_pickId);
	}
	else if ((pick_Data.flag_pickMode == 2) || (pick_Data.flag_pickMode == 3))//area point,area cell
	{
		draw_PickArea(pick_Data.pick_pipelineObj, pick_Data.flag_pickMode);
	}
}

void PipelineObjPlotForm::clear_pickPlot()
{
	disconnect_pick();
	pick_Data.flag_pickMode = -1;
	pick_Data.pick_id = -1;
	pick_Data.flag_draw_curve = false;
	pick_Data.pick_curve_data.clear();
	if (pick_Data.pick_actor != NULL)
	{
		renderer->RemoveActor(pick_Data.pick_actor);
		pick_Data.pick_actor->Delete();
		pick_Data.pick_actor = NULL;
	}
	if (pick_Data.pick_areaCell!=NULL)
		pick_Data.pick_areaCell->Resize(0);
	if (pick_Data.pick_areaPoint != NULL)
		pick_Data.pick_areaPoint->Resize(0);
	pick_Data.pick_pipelineObj = NULL;	
	ui->qvtkWidget->setCursor(Qt::ArrowCursor);
	if (flag_render)
		renderWindow->Render();
}

void PipelineObjPlotForm::update_consoleWidget(QString str)
{
	emit sig_addInfoToConsole(str);
}

bool PipelineObjPlotForm::set_pickData_area(PipelineObject * tep_pipelineObj, int flag_pickMode, vtkIdList* tep_plist, vtkIdList* tep_clist)
{
	if ((flag_pickMode != 2) && (flag_pickMode != 3))
		return false;
	if (tep_pipelineObj == NULL)
		return false;
	if (tep_pipelineObj->mBaseGrid == NULL)
		return false;
	int num_points = tep_pipelineObj->mBaseGrid->GetNumberOfPoints();
	int num_cells = tep_pipelineObj->mBaseGrid->GetNumberOfCells();
	if ((num_cells < 1) || (num_points < 1))
		return false;
	if (tep_plist->GetNumberOfIds() < 1)
		return false;
	if (tep_clist->GetNumberOfIds() < 1)
		return false;
	pick_Data.pick_pipelineObj = tep_pipelineObj;
	pick_Data.flag_pickMode = flag_pickMode;
	if (pick_Data.pick_areaPoint == NULL)
		pick_Data.pick_areaPoint = vtkIdList::New();
	pick_Data.pick_areaPoint->DeepCopy(tep_plist);
	pick_Data.flag_pickMode = flag_pickMode;
	if (pick_Data.pick_areaCell == NULL)
		pick_Data.pick_areaCell = vtkIdList::New();
	pick_Data.pick_areaCell->DeepCopy(tep_clist);
	return true;
}

bool PipelineObjPlotForm::set_pickData_point_cell(PipelineObject * tep_pipelineObj, int flag_pickMode, vtkIdType pick_id)
{
	if ((flag_pickMode != 0) && (flag_pickMode != 1))
		return false;
	if (tep_pipelineObj == NULL)
		return false;
	if (tep_pipelineObj->mBaseGrid == NULL)
		return false;
	int num_points = tep_pipelineObj->mBaseGrid->GetNumberOfPoints();
	int num_cells = tep_pipelineObj->mBaseGrid->GetNumberOfCells();
	if ((num_cells < 1) || (num_points < 1))
		return false;
	pick_Data.pick_pipelineObj = tep_pipelineObj;
	pick_Data.flag_pickMode = flag_pickMode;
	pick_Data.pick_id = pick_id;
	return true;
}

bool PipelineObjPlotForm::set_pickData_curve(PipelineObject * tep_pipelineObj, int flag_pickMode, vtkIdType pick_id)
{
	if ((flag_pickMode != 0) && (flag_pickMode != 1))
		return false;
	if (tep_pipelineObj == NULL)
		return false;
	if (tep_pipelineObj->mBaseGrid == NULL)
		return false;
	int num_points = tep_pipelineObj->mBaseGrid->GetNumberOfPoints();
	int num_cells = tep_pipelineObj->mBaseGrid->GetNumberOfCells();
	if ((num_cells < 1) || (num_points < 1))
		return false;
	pick_Data.pick_pipelineObj = tep_pipelineObj;
	pick_Data.flag_pickMode = flag_pickMode;
	pick_Data.pick_id = pick_id;
	pick_Data.flag_draw_curve = true;
	return true;
}

void PipelineObjPlotForm::slot_update_plot_pipelineObj(PipelineObject*, vtkColorTransferFunction*, vtkPiecewiseFunction*)
{
	
}

void PipelineObjPlotForm::slot_volume_scalarBarEdit(PipelineObject* tep_pipeObj, vtkSmartPointer<vtkLookupTable> tep_lookupTable)
{
	//vtkScalarBarWidget* tep_scalarWidget = NULL;
	//QString tep_colName = tep_pipeObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle;
	//if (tep_pipeObj->scalarBar_widgetMap.contains(tep_colName))
	//{
	//	tep_scalarWidget = tep_pipeObj->scalarBar_widgetMap[tep_colName];
	//}
	//if (tep_scalarWidget != NULL)
	//{
	//	if (tep_lookupTable == NULL)
	//	{
	//		vtkColorTransferFunction *tep_color = tep_pipeObj->getLookupTable(tep_pipeObj->mBaseGrid, tep_colName);
	//		vtkSmartPointer<vtkScalarBarActor> tep_scalarBarActor = vtkSmartPointer<vtkScalarBarActor>::New();
	//		tep_scalarBarActor->SetLookupTable(tep_lookupTable); tep_scalarWidget->SetScalarBarActor(tep_scalarBarActor);
	//		//tep_scalarWidget->GetScalarBarActor()->SetLookupTable(tep_color);
	//	}
	//	else
	//	{
	//		vtkSmartPointer<vtkScalarBarActor> tep_scalarBarActor = vtkSmartPointer<vtkScalarBarActor>::New();
	//		tep_scalarBarActor->SetLookupTable(tep_lookupTable); tep_scalarWidget->SetScalarBarActor(tep_scalarBarActor);
	//		//tep_scalarWidget->GetScalarBarActor()->SetLookupTable(tep_lookupTable);
	//	}
	//}
	if (flag_render)
		renderWindow->Render();
}

bool PipelineObjPlotForm::updatePipelineObjeDataSet(PipelineObject* tep_pipeObj,QString tep_filename)
{
	if (tep_pipeObj == NULL)
		return false;
	bool flag = false;
	QStringList tep_list; tep_list.clear(); tep_list.append(tep_filename);
	NumericSrcObject*	tep_NumericSrcObj = new NumericSrcObject(tep_list, 0);
	if (tep_NumericSrcObj->readFile(tep_filename))
	{
		if (tep_pipeObj->mBaseGrid != NULL)
		{
			tep_pipeObj->mBaseGrid->Delete();
		}
		if (tep_pipeObj->mBaseGridBak != NULL)
		{
			tep_pipeObj->mBaseGridBak->Delete();
		}
		tep_pipeObj->mBaseGrid = tep_NumericSrcObj->mBaseGrid;
		tep_pipeObj->mBaseGridBak = tep_NumericSrcObj->mBaseGridBak;
		flag = true;
	}
	delete tep_NumericSrcObj;
	tep_NumericSrcObj = NULL;
	if (flag)
	{
		((NumericSrcObject*)tep_pipeObj)->generate_actors();
		if (flag_render)
			renderWindow->Render();
	}
	return flag;
}

void PipelineObjPlotForm::func_set_rubberZoom(bool flag_viewInteraction)
{
	flag_cur_viewInteraction = flag_viewInteraction;
	disconnect_pick();
	if (rubberZoom_style != NULL)
	{
		rubberZoom_style->Delete();
		rubberZoom_style = NULL;
	}
	rubberZoom_style = rubberZoomInteractorStyle::New();
	//rubberZoom_style->SetDefaultMouseStyle(mouse_style);
	rubberZoom_style->SetDefaultRenderer(renderer);
	rubberZoom_style->SetInteractor(renderWindowInteractor);
	renderWindowInteractor->SetInteractorStyle(rubberZoom_style);
	renderWindowInteractor->ReInitialize();		
	ui->qvtkWidget->setCursor(Qt::CrossCursor);
	connect(rubberZoom_style, SIGNAL(sig_restoreInteractor()), this, SLOT(slot_restoreInteractor()));
	if (flag_render)
		renderWindow->Render();
}

void PipelineObjPlotForm::func_set_actorOperate(ActorOperateType type, bool flag_viewInteraction)
{
	flag_cur_viewInteraction = flag_viewInteraction;
	disconnect_pick();
	if ((type == kActor_Rotate) || (type == kActor_RotateX) || (type == kActor_RotateY) || (type == kActor_RotateZ))
	{
		if (actorROperate_style != NULL)
		{
			actorROperate_style->Delete();
			actorROperate_style = NULL;
		}
		actorROperate_style = actorInteractorStyle::New();
		actorROperate_style->SetDefaultRenderer(renderer);
		actorROperate_style->set_rotateType(type);
		actorROperate_style->SetInteractor(renderWindowInteractor);
		renderWindowInteractor->SetInteractorStyle(actorROperate_style);
		renderWindowInteractor->ReInitialize();
		if (type==kActor_Rotate)
			ui->qvtkWidget->setCursor(Qt::SizeAllCursor);
		else if (type == kActor_RotateX)
			ui->qvtkWidget->setCursor(Qt::SizeHorCursor);
		else if (type == kActor_RotateY)
			ui->qvtkWidget->setCursor(Qt::SizeBDiagCursor);
		else if (type == kActor_RotateZ)
			ui->qvtkWidget->setCursor(Qt::SizeFDiagCursor);
		connect(actorROperate_style, SIGNAL(sig_restoreInteractor()), this, SLOT(slot_restoreInteractor()));
	}
	else if (type == kActor_Move)
	{
		if (actorMOperate_style != NULL)
		{
			actorMOperate_style->Delete();
			actorMOperate_style = NULL;
		}
		actorMOperate_style = actorMoveInteractorStyle::New();
		actorMOperate_style->SetDefaultRenderer(renderer);
		actorMOperate_style->SetInteractor(renderWindowInteractor);
		renderWindowInteractor->SetInteractorStyle(actorMOperate_style);
		renderWindowInteractor->ReInitialize();
		ui->qvtkWidget->setCursor(Qt::PointingHandCursor);
		connect(actorMOperate_style, SIGNAL(sig_restoreInteractor()), this, SLOT(slot_restoreInteractor()));
		connect(actorMOperate_style, SIGNAL(sig_movePosition(double*)), this, SLOT(slot_movePosition(double*)));

	}	
	if (flag_render)
		renderWindow->Render();
}
void PipelineObjPlotForm::slot_movePosition(double *pos)
{
	pov_propData.pos[0] = pos[0];
	pov_propData.pos[1] = pos[1];
	pov_propData.pos[2] = pos[2];
}
void PipelineObjPlotForm::slot_restoreInteractor()
{
	while (renderWindowInteractor->GetRenderWindow()->CheckInRenderStatus());
	ui->qvtkWidget->setCursor(Qt::ArrowCursor);
	renderWindowInteractor->SetInteractorStyle(mouse_style);
	renderWindowInteractor->ReInitialize();
	func_viewInteraction(flag_cur_viewInteraction);
	if (rubberZoom_style != NULL)
	{
		rubberZoom_style->Delete();
		rubberZoom_style = NULL;
	}
	if (actorMOperate_style != NULL)
	{
		actorMOperate_style->Delete();
		actorMOperate_style = NULL;
	}
	if (actorROperate_style != NULL)
	{
		actorROperate_style->Delete();
		actorROperate_style = NULL;
	}
	if (areaPick_style != NULL)
	{
		areaPick_style->Delete();
		areaPick_style = NULL;
	}
	if (flag_render)
		renderWindow->Render();
}

void PipelineObjPlotForm::func_viewInteraction(bool checked)
{
	if (checked)
		renderWindowInteractor->Enable();
	else
		renderWindowInteractor->Disable();
	ui->qvtkWidget->setCursor(Qt::ArrowCursor);
	if (flag_render)
		renderWindow->Render();
}

void PipelineObjPlotForm::slot_cameraDo(int pos)
{
	int num_para = mouse_style->get_cameraPara_list().count();
	if ((pos > 9) || (pos<0) || (num_para<2))
		return;
	if (pos >= num_para)
		return;
	Camera_Para tep_data = mouse_style->get_cameraPara_list().at(pos);
	vtkCamera* tep_camera = renderer->GetActiveCamera();
	if (tep_camera == NULL)
		return;
	tep_camera->SetViewUp(tep_data.viewUp[0], tep_data.viewUp[1], tep_data.viewUp[2]);
	tep_camera->SetPosition(tep_data.position[0], tep_data.position[1], tep_data.position[2]);
	tep_camera->SetFocalPoint(tep_data.focalPoint[0], tep_data.focalPoint[1], tep_data.focalPoint[2]);
	tep_camera->SetClippingRange(tep_data.clippingRange[0], tep_data.clippingRange[1]);
	tep_camera->SetViewAngle(tep_data.viewAngle);
	if (tep_data.flag_parallel)
	{
		tep_camera->ParallelProjectionOn();
		tep_camera->SetParallelScale(tep_data.parallel_scale);
	}
	else
	{
		tep_camera->ParallelProjectionOff();
	}
	if (flag_render)
		renderWindow->Render();
	mouse_style->set_cur_pos(pos);
	slot_update_UndoReDoButton(pos);
}

void PipelineObjPlotForm::slot_cameraUndo()
{
	int pos=mouse_style->get_cur_pos();
	pos++;
	slot_cameraDo(pos);
}

void PipelineObjPlotForm::slot_cameraRedo()
{
	int pos = mouse_style->get_cur_pos();
	pos--;
	slot_cameraDo(pos);
}

void PipelineObjPlotForm::slot_update_UndoReDoButton(int)
{
	if (mouse_style == NULL)
		return;
	emit sig_update_UndoReDoButton(mouse_style->get_cur_pos());
}

void PipelineObjPlotForm::reTranslate()
{
	ui->retranslateUi(this);
}
void PipelineObjPlotForm::slot_update_cubeAxes(PipelineObject *tep_pipelineObj)
{
	if (tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.flag_cubeAxes)
	{
		tep_pipelineObj->generate_axesActor();
		if (tep_pipelineObj->get_axesActor() != NULL)
		{
			renderer->AddActor(tep_pipelineObj->get_axesActor());
			tep_pipelineObj->get_axesActor()->SetCamera(renderer->GetActiveCamera());
			if (flag_render)
				renderWindow->Render();
		}
	}
	else
	{
		if (tep_pipelineObj->get_axesActor() != NULL)
		{
			renderer->RemoveActor(tep_pipelineObj->get_axesActor());
			if (flag_render)
				renderWindow->Render();
		}
	}
}
void PipelineObjPlotForm::func_exportFile(PipelineObject* tep_pipeObj, QString tep_filename)
{
	ExportFileDialog *exportFileDlg = new ExportFileDialog;
	if (exportThread == NULL)
	{
		exportThread = new ExportFileThread;
	}
	connect(exportThread, SIGNAL(sig_changeExportText(QString)), exportFileDlg, SLOT(slot_changeExportText(QString)));
	connect(exportFileDlg, SIGNAL(sig_stopFileExport()), exportThread, SLOT(stop()));
	exportFileDlg->exportThreadPtr = exportThread;
	exportThread->initThread(tep_pipeObj, tep_filename);
	exportFileDlg->DisplayMovie(tr("file exporting ......"));
	exportThread->start();
	//exportFileDlg->setModal(false);
	exportFileDlg->exec();
	delete exportFileDlg;
}
void PipelineObjPlotForm::slot_update_glyphVector(PipelineObject *tep_pipeObj)
{
	PipelineObject* tep_obj;
	if (tep_pipeObj->GetObjectType() == dNumeric_DataSource)
		tep_obj = tep_pipeObj;
	else
		tep_obj= tep_pipeObj->GetObjParent();
	if (tep_pipeObj->GetObjectType()==dNumeric_DataSource)
		((NumericSrcObject*)tep_pipeObj)->ChangeColumnColor();
	else if (tep_pipeObj->GetObjectType() == dClip_DataSource)
		((FilterClip*)tep_pipeObj)->ChangeColumnColor();
	else if (tep_pipeObj->GetObjectType() == dSlice_DataSource)
		((FilterSlice*)tep_pipeObj)->ChangeColumnColor();
	else if (tep_pipeObj->GetObjectType() == dContour_DataSource)
		((FilterContour*)tep_pipeObj)->ChangeColumnColor();
	else if (tep_pipeObj->GetObjectType() == dVector_DataSource)
		((FilterVector*)tep_pipeObj)->ChangeColumnColor();
	else if (tep_pipeObj->GetObjectType() == dReflection_DataSource)
		((FilterReflection*)tep_pipeObj)->ChangeColumnColor();
	else if (tep_pipeObj->GetObjectType() == dCalculator_DataSource)
		((FilterCalculator*)tep_pipeObj)->ChangeColumnColor();
	else if (tep_pipeObj->GetObjectType() == dSmooth_DataSource)
		((FilterSmooth*)tep_pipeObj)->ChangeColumnColor();
	else if (tep_pipeObj->GetObjectType() == dStreamLine_DataSource)
		((FilterStreamLine*)tep_pipeObj)->ChangeColumnColor();
	if (tep_pipeObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible)
		slot_update_scalarBar(tep_obj);
	else
		renderWindow->Render();
	
}
void PipelineObjPlotForm::func_update_scalarBarPosition(QList<PipelineObject*> mPipelineObjs)
{
	for (int i = 0; i < mPipelineObjs.count(); i++)
	{
		PipelineObject* tep_piplnObj = mPipelineObjs.at(i);
		if (tep_piplnObj == NULL)
			continue;
		if (tep_piplnObj->GetObjectType() != dNumeric_DataSource)
			continue;
		int tep_num = tep_piplnObj->scalarBar_positionMap.count();
		if (tep_num < 1)
			continue;
		QMap<QString, ScalarBarPosition_Para>::iterator it;
		QString tep_colName;
		for (it = tep_piplnObj->scalarBar_positionMap.begin(); it != tep_piplnObj->scalarBar_positionMap.end(); ++it)
		{
			tep_colName = it.key();
			if (tep_piplnObj->scalarBar_widgetMap.contains(tep_colName))
			{
				ScalarBarPosition_Para val = it.value();
				//tep_piplnObj->scalarBar_widgetMap[tep_colName]->GetScalarBarActor()->GetOrientation();
				if (val.orietation == 0)
					tep_piplnObj->scalarBar_widgetMap[tep_colName]->GetScalarBarActor()->SetOrientationToHorizontal();
				else
					tep_piplnObj->scalarBar_widgetMap[tep_colName]->GetScalarBarActor()->SetOrientationToVertical();
				tep_piplnObj->scalarBar_widgetMap[tep_colName]->GetBorderRepresentation()->SetPosition(val.position[0], val.position[1]);
				tep_piplnObj->scalarBar_widgetMap[tep_colName]->GetBorderRepresentation()->SetPosition2(val.position[2], val.position[3]);
			}
		}
	}	
	if (flag_render)
		renderWindow->Render();
}

