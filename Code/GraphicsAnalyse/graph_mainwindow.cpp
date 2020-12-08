#include "graph_mainwindow.h"
#include "ui_graph_mainwindow.h"
#include "ScriptHandler.h"
#include "aboutdialog.h"
#include "myInteractorStyle.h"
#include "solutiondataio.h"
#include "qstringlist.h"
#include "savescriptdialog.h"
#include <QResizeEvent>
graph_MainWindow::graph_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::graph_MainWindow)
{
    ui->setupUi(this);
	dos_scriptFileName = "";
	flag_commandLine = false;
	VRProcess = NULL;
	g_ani_objList.clear();
	//flag_script_running = false;
	//script_dataObj = NULL;
	aniSetting_data.flag_save = false;
	aniSetting_data.frame_rate = 15.0;
	aniSetting_data.frame_num = 0;
	aniSetting_data.frame_step = 1;
	aniSetting_data.frame_start_xh = 0;
	aniSetting_data.frame_end_xh = 0;
	aniSetting_data.image_wid = 600;
	aniSetting_data.image_hei = 400;
	aniSetting_data.total_frame_num = 0;
	aniSetting_data.avi_filename = "";
	aniSetting_data.flag_saveImage = false;
	aniSetting_data.saveImage_format = "png";
	aniSetting_data.saveImage_path = "";
	aniSetting_data.flag_savePov = false;
	aniSetting_data.savePov_path = "";
	aniSetting_data.file_interval = 1;

	cur_ani_time = QTime::currentTime();
	count_obj_id = 0;
	cur_PipelineObj = NULL;
    flag_newFile=false;
    pipelineObj_plotWidget=NULL;
    pipelineObj_propWidget=NULL;
    pipelineObj_treeWidget=NULL;
    console_cmdWidget=NULL;
	pickCurve_Widget = NULL;
	scalarBar_dockWidget = NULL;
    //column_dataWidget=NULL;
    flag_DockWin=false;
    setStyleSheet("QMainWindow::title{background-color: qlineargradient(spread:reflect, x1:0, y1:0, x2:0, y2:1, stop:0 #eaedf6, stop:1 #a6c9e2);}");
    setWindowTitle(QStringLiteral("GraphicsAnalysis 1.0"));
	this->setWindowIcon(QIcon(":/images/app.png"));
	
    creatActions();
    mdiArea=new QMdiArea;
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setWindowFlags(Qt::FramelessWindowHint);
    QLinearGradient linear(mdiArea->geometry().left(), mdiArea->geometry().top(), mdiArea->geometry().left(), mdiArea->geometry().bottom());
    linear.setColorAt(0, QColor(254,254,255));
    linear.setColorAt(1, QColor(237,244,250));
    linear.setSpread(QGradient::PadSpread);
    QBrush brush(linear);
    mdiArea->setBackground(brush);
    mdiArea->setViewMode(QMdiArea::TabbedView);
    mdiArea->setTabPosition(QTabWidget::North);
    mdiArea->setTabsClosable(false);
    mdiArea->setTabsMovable(false);
    mdiArea->setTabShape(QTabWidget::Triangular);
    mdiArea->setStyleSheet("QTabWidget::title{border-radius: 6px; background-image: url(:/images/project_title.png);}");
    //mdiArea->setStyleSheet("QMdiArea::TabbedView:title{background-color:qlineargradient(spread:reflect, x1:0, y1:0, x2:0, y2:1, stop:0 #eaedf6, stop:1 #0009e2);}");
    setCentralWidget(mdiArea);
	newFile();
	initTranslate();
	exitSoftWareScript_para.flag_exit = false;
	exitSoftWareScript_para.ms = 50;
	flag_state = 0;
}

void graph_MainWindow::initTranslate()
{
	_translator = new Translator;
	actionEnglish->setChecked(true);
	actionChinese->setChecked(false);
	if (actionEnglish->isChecked())
		ChangeLanguage("English");
	else
		ChangeLanguage("Chinese");
}

graph_MainWindow::~graph_MainWindow()
{
    mdiArea->closeAllSubWindows();
    flag_DockWin=false;
    if(pipelineObj_treeWidget!=NULL)
    {
		delete pipelineObj_treeWidget;
		pipelineObj_treeWidget = NULL;
    }
    if(pipelineObj_propWidget!=NULL)
    {
		delete pipelineObj_propWidget;
		pipelineObj_propWidget = NULL;
    }
    /*if(column_dataWidget!=NULL)
    {
        delete column_dataWidget;
        column_dataWidget=NULL;
    }*/
    if(pipelineObj_plotWidget!=NULL)
    {
		delete pipelineObj_plotWidget;
		pipelineObj_plotWidget = NULL;
    }
    if(console_cmdWidget!=NULL)
    {
        delete console_cmdWidget;
        console_cmdWidget=NULL;
    }
	if (pickCurve_Widget != NULL)
	{
		delete pickCurve_Widget;
		pickCurve_Widget = NULL;
	}
	if (scalarBar_dockWidget != NULL)
	{
		delete scalarBar_dockWidget;
		scalarBar_dockWidget = NULL;
	}
	if (_translator != nullptr)
	{
		delete _translator;
		_translator = nullptr;
	}
    delete ui;
}

Translator* graph_MainWindow::GetTranslator()
{
	return _translator;
}

void graph_MainWindow::ChangeLanguage(QString lang)
{
	if (nullptr != ui)
	{
		if (lang == "English")
		{
			actionChinese->setChecked(false);
			actionEnglish->setChecked(true);
			_translator->toEnglish();
		}
		else if (lang == "Chinese")
		{
			actionChinese->setChecked(true);
			actionEnglish->setChecked(false);
			_translator->toChinese();
		}		
		reTranslate();
	}
}

void graph_MainWindow::reTranslate()
{
	ui->retranslateUi(this);
	fileMenu->setTitle(tr("File"));
	viewMenu->setTitle(tr("View"));
	filterMenu->setTitle(tr("Filter"));
	toolMenu->setTitle(tr("Tool"));
	helpMenu->setTitle(tr("Help"));
	VRMenu->setTitle(tr("VR"));

	openAct->setText(tr("open"));
	openAct->setStatusTip(tr("Open an existing file."));
	runScriptAct->setText(tr("Run script"));
	runScriptAct->setStatusTip(tr("Run script file."));
	editScriptAct->setText(tr("Edit script"));
	editScriptAct->setStatusTip(tr("Generate script file."));
	saveImageAct->setText(tr("saveImage"));
	saveImageAct->setStatusTip(tr("save image."));
	saveAniAct->setText(tr("saveAnimation..."));
	saveAniAct->setStatusTip(tr("save avi animation."));
	exitAct->setText(tr("exit"));
	exitAct->setStatusTip(tr("Exit this software."));
	contourAct->setText(tr("contour"));
	contourAct->setStatusTip(tr("contour filter."));
	clipAct->setText(tr("clip"));
	clipAct->setStatusTip(tr("clipFilter"));
	sliceAct->setText(tr("slice"));
	sliceAct->setStatusTip(tr("sliceFilter"));
	glyphAct->setText(tr("glyph"));
	glyphAct->setStatusTip(tr("glyphFilter"));
	reflectionAct->setText(tr("reflection"));
	reflectionAct->setStatusTip(tr("reflectionFilter"));
	reflectionAct->setText(tr("reflection"));
	reflectionAct->setStatusTip(tr("reflectionFilter"));
	calculateAct->setText(tr("calculator"));
	calculateAct->setStatusTip(tr("calculator filter."));
	streamLineAct->setText(tr("streamLine"));
	streamLineAct->setStatusTip(tr("streamLineFilter"));
	plotViewMenu->setTitle(tr("viewDirection"));
	viewInteractionAct->setText(tr("viewInteraction"));
	viewInteractionAct->setStatusTip(tr("view interaction enable or disable"));
	resetViewAct->setText(tr("reset"));
	resetViewAct->setStatusTip(tr("reset view"));
	XPlusAct->setText(tr("+X"));
	XPlusAct->setStatusTip(tr("set view direction to +X"));
	XMinusAct->setText(tr("-X"));
	XMinusAct->setStatusTip(tr("set view direction to -X"));
	YPlusAct->setText(tr("+Y"));
	YPlusAct->setStatusTip(tr("set view direction to +Y"));
	YMinusAct->setText(tr("-Y"));
	YMinusAct->setStatusTip(tr("set view direction to -Y"));
	ZPlusAct->setText(tr("+Z"));
	ZPlusAct->setStatusTip(tr("set view direction to +Z"));
	ZMinusAct->setText(tr("-Z"));
	ZMinusAct->setStatusTip(tr("set view direction to -Z"));
	cameraUndoAct->setText(tr("camera undo"));
	cameraUndoAct->setStatusTip(tr("camera undo"));
	cameraRedoAct->setText(tr("camera redo"));
	cameraRedoAct->setStatusTip(tr("camera redo"));
	rubberZoomAct->setText(tr("rubberZoom"));
	rubberZoomAct->setStatusTip(tr("rubberBand Zoom"));
	actorMoveAct->setText(tr("move actor"));
	actorMoveAct->setStatusTip(tr("move actor"));
	actorRotateAct->setText(tr("rotate actor"));
	actorRotateAct->setStatusTip(tr("rotate actor"));
	actorRotateXAct->setText(tr("rotateX actor"));
	actorRotateXAct->setStatusTip(tr("rotateX actor"));
	actorRotateYAct->setText(tr("rotateY actor"));
	actorRotateYAct->setStatusTip(tr("rotateY actor"));
	actorRotateZAct->setText(tr("rotateZ actor"));
	actorRotateZAct->setStatusTip(tr("rotateZ actor"));
	langueMenu->setTitle(tr("language"));
	actionChinese->setText(tr("chinese"));
	actionEnglish->setText(tr("english"));
	menuAct->setText(tr("menu"));
	menuAct->setStatusTip(tr("show menu bar."));
	statusAct->setText(tr("status"));
	statusAct->setStatusTip(tr("show status bar."));
	toolAct->setText(tr("tool"));
	toolAct->setStatusTip(tr("show tool bar."));
	helpAct->setText(tr("help"));
	helpAct->setStatusTip(tr("help"));
	aboutAct->setText(tr("about"));
	aboutAct->setStatusTip(tr("about"));
	VRAct->setText(tr("VR run"));
	VRAct->setStatusTip(tr("VR run"));
	pickPointAct->setText(tr("pick point"));
	pickPointAct->setStatusTip(tr("pick point"));
	pickCellAct->setText(tr("pick cell"));
	pickCellAct->setStatusTip(tr("pick cell"));
	pickAreaCellAct->setText(tr("pick area cells"));
	pickAreaCellAct->setStatusTip(tr("pick area cells"));
	pickAreaPointAct->setText(tr("pick area points"));
	pickAreaPointAct->setStatusTip(tr("pick area points"));
	pickClearAct->setText(tr("pick clear"));
	pickClearAct->setStatusTip(tr("pick clear"));
	pickCurvePointAct->setText(tr("pick curve point"));
	pickCurvePointAct->setStatusTip(tr("pick curve point"));
	pickCurveCellAct->setText( tr("pick curve cell"));
	pickCurveCellAct->setStatusTip(tr("pick curve cell"));
	objProp_presentCmb->setItemText(0, tr("Points"));
	objProp_presentCmb->setItemText(1,tr("Surface"));
	objProp_presentCmb->setItemText(2,tr("Surface With Edges"));
	objProp_presentCmb->setItemText(3,tr("Wireframe"));
	objProp_scalarBarAct->setText(tr("ScalarBar"));
	objProp_scalarBarAct->setStatusTip(tr("pipelineObject ScalarBar"));
	objProp_solidColorAct->setText(tr("editColor"));
	objProp_solidColorAct->setStatusTip(tr("pipelineObject editColor"));
	cgnsAniTimesAct->setText(tr("CgnsAniTimes"));
	cgnsAniTimesAct->setStatusTip(tr("Cgns pipelineObject animate times render!"));
	exportAct->setText(tr("Export"));
	exportAct->setStatusTip(tr("export a solution file."));
	firstAct->setText(tr("first frame"));
	firstAct->setStatusTip(tr("first frame"));
	lastAct->setText(tr("last frame"));
	lastAct->setStatusTip(tr("last frame"));
	runAct->setText(tr("run"));
	runAct->setStatusTip(tr("run"));
	nextAct->setText(tr("next frame"));
	nextAct->setStatusTip(tr("next frame"));
	endAct->setText(tr("end frame"));
	endAct->setStatusTip(tr("end frame"));
	lab1->setText(tr("frame:"));
	lab2->setText(tr("File Interval:"));	
	flag_saveAvi_label->setText(tr("animate: "));
	flag_saveAvi_checkBox->setText(tr("save avi"));

	pipelineObj_propWidget->reTranslate();
	pipelineObj_treeWidget->reTranslate();
	pipelineObj_plotWidget->reTranslate();
	console_cmdWidget->reTranslate();
	if (pickCurve_Widget != NULL)
		pickCurve_Widget->reTranslate();
	if (scalarBar_dockWidget != NULL)
		scalarBar_dockWidget->reTranslate();
}

void graph_MainWindow::slot_objProp_scalarBarAct()
{
	if (cur_PipelineObj == NULL)
		return;
	pipelineObj_propWidget->get_propWidget()->toolBar_objProp_scalarBarAct();
}

void graph_MainWindow::slot_objProp_exportAct()
{
	if (cur_PipelineObj != NULL)
	{
		QString tep_filename;
		tep_filename = QFileDialog::getSaveFileName(this, tr("Export solution"), "solution", tr("solution(*.sol)"));
		if (tep_filename == "")
			return;
		pipelineObj_plotWidget->func_exportFile(cur_PipelineObj, tep_filename);
		
	}
}

void graph_MainWindow::slot_objProp_solidColorAct()
{
	if (cur_PipelineObj == NULL)
		return;
	pipelineObj_propWidget->get_propWidget()->toolBar_objProp_solidColorAct();
}

void graph_MainWindow::slot_objProp_presentCmb_currentIndexChanged(int tep_index)
{
	if (tep_index == -1)
		return;
	if (cur_PipelineObj == NULL)
		return;
	pipelineObj_propWidget->get_propWidget()->toolBar_objProp_presentCmb_currentIndexChanged(tep_index);
}

void graph_MainWindow::slot_objProp_columnCmb_currentIndexChanged(int tep_index)
{
	if (tep_index == -1)
		return;
	if (cur_PipelineObj == NULL)
		return;
	if (cur_PipelineObj->numberOfCgnsTimes > 1)
	{
		if ((objProp_columnCmb->currentText() == "SolidColor") || (objProp_columnCmb->currentText() == "vtkBlockColors"))
		{
			if (cgnsAniTimesAct->isEnabled())
			{
				cgnsAniTimesAct->setEnabled(false);
			}
		}
		else
		{
			if (!cgnsAniTimesAct->isEnabled())
			{
				cgnsAniTimesAct->setEnabled(true);
			}
		}
	}

	if (objProp_columnCmb->currentText() == tr("GlyphVector"))
	{
		objProp_GlyphVector->setEnabled(true);
		//objProp_GlyphVector->setCurrentText(cur_PipelineObj->mPipeLineObjProp.vectorFilter_propData.glyphVector_colorXYZ);
		//slot_objProp_GlyphVector_currentIndexChanged(objProp_GlyphVector->currentIndex());
	}
	pipelineObj_propWidget->get_propWidget()->toolBar_objProp_columnCmb_currentIndexChanged(tep_index);
}

void graph_MainWindow::slot_objProp_GlyphVector_currentIndexChanged(int tep_index)
{
	if (tep_index == -1)
		return;
	if (cur_PipelineObj == NULL)
		return;
	pipelineObj_propWidget->get_propWidget()->toolBar_objProp_GlyphVector_currentIndexChanged(tep_index);
}

void graph_MainWindow::slot_update_toolBar_objProp_column(int tep_index)
{
	objProp_columnCmb->setEnabled(false);
	if ((objProp_columnCmb->count() > tep_index) && (tep_index >= 0))
		objProp_columnCmb->setCurrentIndex(tep_index);
	////////////////////////////////////
	if (cur_PipelineObj == NULL)
		return;
	if (cur_PipelineObj->mBaseGrid == NULL)
		return;
	bool flag_xyz = false;
	PipelineObject* tep_obj;
	if (cur_PipelineObj->GetObjectType() == dNumeric_DataSource)
		tep_obj = cur_PipelineObj;
	else
		tep_obj = cur_PipelineObj->GetObjParent();
	if (objProp_columnCmb->itemData(tep_index).toString().indexOf("val_on_cell.png") >= 0)
	{
		flag_xyz = cur_PipelineObj->func_isVector(objProp_columnCmb->currentText().trimmed(), 1, tep_obj->mBaseGrid);
	}
	else if (cur_PipelineObj->mBaseGrid->GetPointData()->HasArray(objProp_columnCmb->currentText().toStdString().data()))
	{
		flag_xyz = cur_PipelineObj->func_isVector(objProp_columnCmb->currentText().trimmed(), 0, tep_obj->mBaseGrid);
	}
	else if (objProp_columnCmb->currentText()=="GlyphVector")
	{
		flag_xyz = true;
	}
	if (flag_xyz)
	{
		objProp_GlyphVector->setEnabled(false);
		objProp_GlyphVector->setCurrentText(cur_PipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle_XYZ);
		objProp_GlyphVector->setEnabled(true);
	}
	else
	{
		objProp_GlyphVector->setEnabled(false);
	}
	///////////////////////////////////////
	objProp_columnCmb->setEnabled(true);
	if (scalarBar_dockWidget != NULL)
	{
		scalarBar_dockWidget->get_scalarBarEditForm()->updateForm(cur_PipelineObj);
	}
}
void graph_MainWindow::slot_update_toolBar_objProp_column_GlyphVector(bool flag)
{
	if (cur_PipelineObj == NULL)
		return;
	objProp_columnCmb->blockSignals(true);
	if (flag)
	{
		int index = objProp_columnCmb->findText("GlyphVector");
		if (index < 0)
		{
			QIcon icon3 = QIcon::fromTheme("val_on_point.png", QIcon(":/images/val_on_point.png"));
			objProp_columnCmb->addItem(icon3, "GlyphVector", "val_on_point.png");
		}
		//ui->vectorColorXYZ_comboBox->setVisible(true);
	}
	else
	{
		int index = objProp_columnCmb->findText("GlyphVector");
		if (index >= 0)
		{
			objProp_columnCmb->removeItem(index);
		}
	}
	objProp_columnCmb->blockSignals(false);
}
void graph_MainWindow::slot_update_toolBar_objProp_column_Calculator()
{
	if (cur_PipelineObj == NULL)
		return;
	objProp_columnCmb->blockSignals(true);
	objProp_columnCmb->clear();
	QString tep_str;
	QStringList listColumn = cur_PipelineObj->mPipeLineObjProp.pipelineObj_base_propData.m_colorList;
	for (int i = 0; i < listColumn.count(); i++)
	{
		tep_str = listColumn.at(i);
		if (tep_str.indexOf("_cell000") >= 0)
		{
			QIcon icon1 = QIcon::fromTheme("val_on_cell.png", QIcon(":/images/val_on_cell.png"));
			objProp_columnCmb->addItem(icon1, tep_str.left(tep_str.length() - 8), "val_on_cell.png");
		}
		else
		{
			if (tep_str == "SolidColor")
			{
				QIcon icon3 = QIcon::fromTheme("solidcolor.png", QIcon(":/images/solidcolor.png"));
				objProp_columnCmb->addItem(icon3, tep_str, "solidcolor.png");
			}
			else if (tep_str == "vtkBlockColors")
			{
				//continue;// add for debug zone
				QIcon icon4 = QIcon::fromTheme("blockColors.png", QIcon(":/images/blockColors.png"));
				objProp_columnCmb->addItem(icon4, "vtkBlockColors", "blockColors.png");
			}
			else
			{
				QIcon icon2 = QIcon::fromTheme("val_on_point.png", QIcon(":/images/val_on_point.png"));
				objProp_columnCmb->addItem(icon2, tep_str, "val_on_point.png");
			}
		}
	}
	objProp_columnCmb->blockSignals(false);
}
void graph_MainWindow::slot_update_toolBar_objProp_column_GlyphVectorXYZ(int index)
{
	if (cur_PipelineObj == NULL)
		return;
	if ((index < 0) || (index>2))
		return;
	objProp_GlyphVector->setEnabled(false);
	objProp_GlyphVector->setCurrentIndex(index);
	objProp_GlyphVector->setEnabled(true);
}
void graph_MainWindow::slot_update_toolBar_objProp_present(int tep_index)
{
	objProp_presentCmb->setEnabled(false);
	if ((objProp_presentCmb->count() > tep_index) && (tep_index >= 0))
		objProp_presentCmb->setCurrentIndex(tep_index);
	objProp_presentCmb->setEnabled(true);
}

void graph_MainWindow::slot_update_pipelineObj_toolBar(PipelineObject *tep_pipeObj)
{
	QString tep_str;
	cur_PipelineObj = tep_pipeObj;
	objProp_presentCmb->setCurrentIndex(tep_pipeObj->mPipeLineObjProp.pipelineObj_base_propData.m_enum_representationtype);
	objProp_columnCmb->blockSignals(true);
	objProp_columnCmb->clear();
	objProp_columnCmb->setInsertPolicy(QComboBox::InsertAtBottom);	
	QIcon icon0 = QIcon::fromTheme("solidcolor.png", QIcon(":/images/solidcolor.png"));
	objProp_columnCmb->addItem(icon0, "SolidColor", "solidcolor.png");
	QStringList listColumn = tep_pipeObj->mPipeLineObjProp.pipelineObj_base_propData.m_colorList;
	for (int i = 0; i < listColumn.count(); i++)
	{
		tep_str = listColumn.at(i);
		if (tep_str.indexOf("_cell000") >= 0)
		{
			QIcon icon1 = QIcon::fromTheme("val_on_cell.png", QIcon(":/images/val_on_cell.png"));
			objProp_columnCmb->addItem(icon1, tep_str.left(tep_str.length() - 8), "val_on_cell.png");
		}
		else
		{
			if (tep_str == "vtkBlockColors")
			{
				//continue;// add for debug zone
				QIcon icon4 = QIcon::fromTheme("blockColors.png", QIcon(":/images/blockColors.png"));
				objProp_columnCmb->addItem(icon4, "vtkBlockColors", "blockColors.png");
			}
			else
			{
				QIcon icon2 = QIcon::fromTheme("val_on_point.png", QIcon(":/images/val_on_point.png"));
				objProp_columnCmb->addItem(icon2, tep_str, "val_on_point.png");
			}
		}
	}
	
	objProp_columnCmb->blockSignals(false);
	QString tep_column = tep_pipeObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle;
	if (tep_column.indexOf("_cell000") >= 0)
	{
		tep_column = tep_column.left(tep_column.length() - 8);
	}
	objProp_columnCmb->setCurrentText(tep_column);
	objProp_columnCmb->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	objProp_presentCmb->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	if (tep_pipeObj->GetObjectType() == dVector_DataSource)
	{
		if (tep_pipeObj->mPipeLineObjProp.vectorFilter_propData.vectorColName != "None")
		{
			QIcon icon2 = QIcon::fromTheme("val_on_point.png", QIcon(":/images/val_on_point.png"));
			objProp_columnCmb->addItem(icon2, "GlyphVector", "val_on_point.png");
		}
	}
}

PipelineObjProp* graph_MainWindow::GetPipelineObjProp(PipelineObject* pplnObj)
{
	return &pplnObj->mPipeLineObjProp;
}

void graph_MainWindow::AddPipelineObject(PipelineObject* pipeObj)
{
	count_obj_id++;
	pipeObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id = count_obj_id;
	mProjectDB.AddPipelineObject(pipeObj);
	g_ani_objList.append(pipeObj);
	////add file interval
	pipelineObj_treeWidget->AddPipelineObject(pipeObj);
	pipelineObj_treeWidget->DoSelectPipelineObject(pipeObj); 
	if (pipeObj->GetObjectType() == dSlice_DataSource)
	{
		pipelineObj_plotWidget->slot_init_sliceFilter_plotForm((FilterSlice*)pipeObj);
		connect(((FilterSlice*)pipeObj)->get_callback(), SIGNAL(sig_update_planeProp_widget(double*, double*)), (sliceFilterPropForm*)(pipelineObj_propWidget->get_propWidget()->get_cur_addWidget()), SLOT(slot_update_planeProp_widget(double*, double*)));
		
	}
	else if (pipeObj->GetObjectType() == dClip_DataSource)
	{
		pipelineObj_plotWidget->slot_init_clipFilter_plotForm((FilterClip*)pipeObj);
		connect(((FilterClip*)pipeObj)->get_callback(), SIGNAL(sig_update_planeProp_widget(double*, double*)), (sliceFilterPropForm*)(pipelineObj_propWidget->get_propWidget()->get_cur_addWidget()), SLOT(slot_update_planeProp_widget(double*, double*)));
		
	}
	
	//pipelineObj_propWidget->propWidget->updateObjPropForm(pipeObj);
	if (pipeObj->GetObjectType() == dNumeric_DataSource)
	{
		if (pipeObj->mPipeLineObjProp.pipelineObj_base_propData.fileList.count() > pipelineObj_plotWidget->max_ani_frames)
		{
			pipelineObj_plotWidget->max_ani_frames = pipeObj->mPipeLineObjProp.pipelineObj_base_propData.fileList.count();

			if (pipelineObj_plotWidget->max_ani_frames > 1)
			{
				QString tepstr;
				tepstr.setNum(pipelineObj_plotWidget->max_ani_frames, 10);
				cur_frame_num->setText(" of " + tepstr);
				cur_frame_num->setVisible(true);
				cur_frame_spinBox->setMaximum(pipelineObj_plotWidget->max_ani_frames - 1);
				aniSetting_data.total_frame_num = pipelineObj_plotWidget->max_ani_frames;
				aniSetting_data.frame_end_xh = aniSetting_data.total_frame_num - 1;
			}
		}
	}
	if (mProjectDB.GetPipelineObjects().count() > 0)
	{
		editScriptAct->setEnabled(true);
		saveAniAct->setEnabled(true);
		saveImageAct->setEnabled(true);

		//objPropToolBar->setEnabled(true);
		objProp_scalarBarAct->setEnabled(true);
		objProp_solidColorAct->setEnabled(true);
		objProp_columnCmb->setEnabled(true);
		objProp_presentCmb->setEnabled(true);
		exportAct->setEnabled(true);

		rubberZoomAct->setEnabled(true);
		resetViewAct->setEnabled(true);
		viewInteractionAct->setEnabled(true);
		XPlusAct->setEnabled(true);
		YPlusAct->setEnabled(true);
		ZPlusAct->setEnabled(true);
		XMinusAct->setEnabled(true);
		YMinusAct->setEnabled(true);
		ZMinusAct->setEnabled(true);

		//mousePickToolBar->setEnabled(true);
		pickCellAct->setEnabled(true);
		pickPointAct->setEnabled(true);
		pickAreaCellAct->setEnabled(true);
		pickAreaPointAct->setEnabled(true);
		pickClearAct->setEnabled(true);

		//cameraRedoAct->setEnabled(true);
		//cameraUndoAct->setEnabled(true);
		actorMoveAct->setEnabled(true);
		actorRotateAct->setEnabled(true);
		actorRotateXAct->setEnabled(true);
		actorRotateYAct->setEnabled(true);
		actorRotateZAct->setEnabled(true);
	}
	emit sig_script_user_custom_add_pipelineObject(pipeObj);
	//python////////////////////////////////////////
	if (pipeObj->GetObjectType() == dNumeric_DataSource)
	{
		QString fileNamePy;
		if (pipeObj->mPipeLineObjProp.pipelineObj_base_propData.fileList.count() > 1)
		{
			QString namePy;
			QString suffixPy;
			int pos = pipeObj->mPipeLineObjProp.pipelineObj_base_propData.fileList.at(0).lastIndexOf(".");
			if (pos >= 0)
			{
				suffixPy = pipeObj->mPipeLineObjProp.pipelineObj_base_propData.fileList.at(0).right(pipeObj->mPipeLineObjProp.pipelineObj_base_propData.fileList.at(0).length() - pos - 1);
				namePy = pipeObj->mPipeLineObjProp.pipelineObj_base_propData.fileList.at(0).left(pos);
			}
			fileNamePy = QString("%1*.%2").arg(namePy).arg(suffixPy);
		}
		else
		{
			fileNamePy = pipeObj->mPipeLineObjProp.pipelineObj_base_propData.fileList.at(0);
		}
		emit sig_python_openFile(fileNamePy);
	}
	else if (pipeObj->GetObjectType() == dClip_DataSource)
	{
		emit sig_python_FilterClip(pipeObj->GetObjParent());
	}
	else if (pipeObj->GetObjectType() == dSlice_DataSource)
	{
		emit sig_python_FilterSlice(pipeObj->GetObjParent());
	}
	else if (pipeObj->GetObjectType() == dContour_DataSource)
	{
		emit sig_python_FilterContour(pipeObj->GetObjParent());
	}
	else if (pipeObj->GetObjectType() == dVector_DataSource)
	{
		emit sig_python_FilterVector(pipeObj->GetObjParent());
	}
	else if (pipeObj->GetObjectType() == dReflection_DataSource)
	{
		emit sig_python_FilterReflection(pipeObj->GetObjParent());
	}
	else if (pipeObj->GetObjectType() == dCalculator_DataSource)
	{
		emit sig_python_FilterCalculator(pipeObj->GetObjParent());
	}
	else if (pipeObj->GetObjectType() == dSmooth_DataSource)
	{
		emit sig_python_FilterSmooth(pipeObj->GetObjParent());
	}
	else if (pipeObj->GetObjectType() == dStreamLine_DataSource)
	{
		emit sig_python_FilterStreamLine(pipeObj->GetObjParent());
	}
	//python////////////////////////////////////////
}

void graph_MainWindow::slot_delPipelineObj(PipelineObject* pipeObj)
{
	if (pipeObj == NULL)
		return;
	if (pipeObj->GetObjectType() == dClip_DataSource)
	{
		((FilterClip*)pipeObj)->close_planeWidget();//((FilterClip*)pipeObj)->planeWidget->Off();
	}
	else if (pipeObj->GetObjectType() == dSlice_DataSource)
	{
		((FilterSlice*)pipeObj)->close_planeWidget();//((FilterSlice*)pipeObj)->planeWidget->Off();
	}
	int tep_script_pipelineObj_id = pipeObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id;

	pipelineObj_plotWidget->del_pipelineObj_pickPlot(pipeObj);
	/*if ((pipeObj->mPipeLineObjProp.pick_propData.flag_pick != -1) && (pipeObj->mPipeLineObjProp.pick_propData.pick_actor!=NULL))
	{
		pipelineObj_plotWidget->del_pickActor(pipeObj->mPipeLineObjProp.pick_propData.pick_actor);
	}*/

	pipelineObj_plotWidget->slot_delPipelineObj(pipeObj);//pipelineObj_plotWidget->del_obj_scalarBarWidget(pipeObj);
	pipelineObj_treeWidget->RmvPipelineObject(pipeObj);	
	//pipelineObj_propWidget->get_propWidget()->slot_update_buildIn_propForm();
	mProjectDB.RmvPipelineObject(pipeObj);
	g_ani_objList.removeOne(pipeObj);//20180511 add
	int tep_max_frames = 0;
	int tep_num;
	QString tepstr;
	for (int i = 0; i < mProjectDB.GetPipelineObjects().count(); i++)
	{
		if (mProjectDB.GetPipelineObjects()[i]->GetObjectType() == dNumeric_DataSource)
		{
			tep_num = mProjectDB.GetPipelineObjects()[i]->mPipeLineObjProp.pipelineObj_base_propData.fileList.count();
			if (tep_num>tep_max_frames)
				tep_max_frames = tep_num;
			//glo_ani_objList[i]->mPipeLineObjProp.pipelineObj_base_propData.file_xh = 0;
		}
	}
	if (pipelineObj_plotWidget->max_ani_frames > tep_max_frames)
	{
		pipelineObj_plotWidget->max_ani_frames = tep_max_frames;
		pipelineObj_plotWidget->set_cur_ani_frame_xh(0);//pipelineObj_plotWidget->cur_ani_frame_xh = 0;
		tepstr.setNum(tep_max_frames, 10);
		cur_frame_num->setText(" of " + tepstr);

		if (pipelineObj_plotWidget->max_ani_frames>0)
			cur_frame_spinBox->setMaximum(pipelineObj_plotWidget->max_ani_frames - 1);
		else
			cur_frame_spinBox->setMaximum(0);
		aniSetting_data.total_frame_num = pipelineObj_plotWidget->max_ani_frames;
		if (aniSetting_data.total_frame_num > 0)
			aniSetting_data.frame_end_xh = aniSetting_data.total_frame_num - 1;
		else
			aniSetting_data.frame_end_xh = 0;
	}
	if (scalarBar_dockWidget != NULL)
	{
		scalarBar_dockWidget->get_scalarBarEditForm()->clearAll();
	}
	g_ani_objList.clear();
	g_ani_objList = mProjectDB.GetPipelineObjects();
	if (mProjectDB.GetPipelineObjects().count() < 1)
	{
		editScriptAct->setEnabled(false);
		saveAniAct->setEnabled(false);
		saveImageAct->setEnabled(false);

		//objPropToolBar->setEnabled(false);
		objProp_scalarBarAct->setEnabled(false);
		objProp_solidColorAct->setEnabled(false);
		objProp_columnCmb->setEnabled(false);
		objProp_presentCmb->setEnabled(false);
		objProp_GlyphVector->setEnabled(false);
		exportAct->setEnabled(false);

		rubberZoomAct->setEnabled(false);
		resetViewAct->setEnabled(false);
		viewInteractionAct->setEnabled(false);
		XPlusAct->setEnabled(false);
		YPlusAct->setEnabled(false);
		ZPlusAct->setEnabled(false);
		XMinusAct->setEnabled(false);
		YMinusAct->setEnabled(false);
		ZMinusAct->setEnabled(false);

		//mousePickToolBar->setEnabled(false);
		pickCellAct->setEnabled(false);
		pickPointAct->setEnabled(false);
		pickAreaCellAct->setEnabled(false);
		pickAreaPointAct->setEnabled(false);
		pickClearAct->setEnabled(false);

		//cameraRedoAct->setEnabled(false);
		//cameraUndoAct->setEnabled(false);
		actorMoveAct->setEnabled(false);
		actorRotateAct->setEnabled(false);
		actorRotateXAct->setEnabled(false);
		actorRotateYAct->setEnabled(false);
		actorRotateZAct->setEnabled(false);
	}
	cur_PipelineObj = NULL;
	emit sig_script_user_custom_del_pipelineObject(tep_script_pipelineObj_id);
}

bool graph_MainWindow::slot_LoadDataSource(QString tep_file, QStringList fileList, int tep_file_xh)
{
	NumericSrcObject* cur_NumericSrcObj = new NumericSrcObject(fileList,tep_file_xh);
	QFileInfo tep_info(fileList.at(tep_file_xh));
	if (tep_info.suffix() == "obj")
	{
		QString obj_name, mtl_name;
		obj_name = fileList.at(tep_file_xh);
		mtl_name = obj_name;
		mtl_name.replace(".obj", ".mtl");
		QFile fileobj(obj_name);
		QFile filemtl(mtl_name);
		if ((!fileobj.exists()) || (!filemtl.exists()))
		{
			delete cur_NumericSrcObj;
			cur_NumericSrcObj = NULL;
			return false;
		}
		else
		{
			if (pipelineObj_plotWidget->load_objFile(cur_NumericSrcObj, obj_name))
			{
				cur_NumericSrcObj->mPipeLineObjProp.pipelineObj_base_propData.fileName = QFileInfo(tep_file).fileName();
				cur_NumericSrcObj->mPipeLineObjProp.pipelineObj_base_propData.fileList.clear();
				cur_NumericSrcObj->mPipeLineObjProp.pipelineObj_base_propData.fileList.append(tep_file);
				cur_NumericSrcObj->mPipeLineObjProp.pipelineObj_base_propData.fileListBak = cur_NumericSrcObj->mPipeLineObjProp.pipelineObj_base_propData.fileList;
				cur_PipelineObj = cur_NumericSrcObj;
				AddPipelineObject(cur_PipelineObj);
				return true;
			}
			else
			{
				delete cur_NumericSrcObj;
				cur_NumericSrcObj = NULL;
				return false;
			}
		}
	}
	else
	{
		if (cur_NumericSrcObj->readFile(fileList.at(tep_file_xh)))
		{
			cur_PipelineObj = (PipelineObject*)cur_NumericSrcObj;
			cur_NumericSrcObj->func_init_displacement();
			cur_NumericSrcObj->generate_scalarBar_lookupTableMap(cur_NumericSrcObj->mBaseGrid);
			AddPipelineObject(cur_PipelineObj);
			//add for multi zone with pipelineobj
			if (cur_PipelineObj->dataSetMap.count() > 1)
			{
				addNewPipelineObjectForMulBlock(cur_NumericSrcObj);
			}
			//add for multi zone with pipelineobj
			return true;
		}
		else
		{
			delete cur_NumericSrcObj;
			cur_NumericSrcObj = NULL;
			return false;
		}
	}	
}

void graph_MainWindow::addNewPipelineObjectForMulBlock(NumericSrcObject* tep_obj)
{
	QMap<vtkDataSet*, QMap<QString, vtkSmartPointer<vtkIdTypeArray>>>::iterator it;
	vtkDataSet* tep_dataSet;
	for (it = tep_obj->dataSetMap.begin(); it != tep_obj->dataSetMap.end(); it++)
	{
		if (it == tep_obj->dataSetMap.begin())
			continue;
		tep_dataSet = it.key();
		if (tep_dataSet == NULL)
			continue;
		
		QStringList teplist = tep_obj->GetAllDataFiles();
		NumericSrcObject*	tep_NumericSrcObj = new NumericSrcObject(teplist, 0);
		tep_NumericSrcObj->mPipeLineObjProp.pipelineObj_base_propData.fileName = tep_obj->mPipeLineObjProp.pipelineObj_base_propData.fileName;
		tep_NumericSrcObj->mPipeLineObjProp.pipelineObj_base_propData.file_xh = tep_obj->mPipeLineObjProp.pipelineObj_base_propData.file_xh;
		tep_NumericSrcObj->mPipeLineObjProp.pipelineObj_base_propData.fileList = teplist;
		tep_NumericSrcObj->mPipeLineObjProp.pipelineObj_base_propData.block_showMap = tep_obj->mPipeLineObjProp.pipelineObj_base_propData.block_showMap;
		tep_NumericSrcObj->mBaseGridBak = tep_dataSet;
		tep_NumericSrcObj->numberOfCgnsTimes = tep_obj->numberOfCgnsTimes;
		//tep_NumericSrcObj->numberOfCgnsObjsPtr = tep_obj->numberOfCgnsObjsPtr;
		tep_obj->dataSetMap = tep_obj->dataSetMap;
		tep_NumericSrcObj->dataSetNameMap = tep_obj->dataSetNameMap;
		if (tep_obj->dataSetNameMap.contains(tep_dataSet))
		{
			tep_NumericSrcObj->SetName(tep_NumericSrcObj->mPipeLineObjProp.pipelineObj_base_propData.fileName + QString(" (%1)").arg(tep_NumericSrcObj->dataSetNameMap[tep_dataSet]));
		}
		tep_NumericSrcObj->idTypeBlockDataSet = tep_obj->dataSetMap[tep_dataSet];
		tep_NumericSrcObj->set_GridType(dUNSTRUCTURED_GRID);
		tep_NumericSrcObj->scalar_vector_val = tep_NumericSrcObj->ReadVarsList(tep_dataSet);
		
		tep_NumericSrcObj->setColorList();	
		vtkUnstructuredGrid* tep_grid = vtkUnstructuredGrid::New();
		tep_grid->DeepCopy(tep_NumericSrcObj->mBaseGridBak);
		if (tep_NumericSrcObj->mBaseGrid != NULL)
			tep_NumericSrcObj->mBaseGrid->Delete();
		tep_NumericSrcObj->mBaseGrid = tep_grid;
		tep_NumericSrcObj->flag_update_mBaseGrid = true;
		tep_NumericSrcObj->func_update_mBaseGrid();
		tep_NumericSrcObj->generate_scalarBar_lookupTableMap(tep_NumericSrcObj->mBaseGrid);
		AddPipelineObject(tep_NumericSrcObj);
	}
}

void graph_MainWindow::slot_refreshView_fromPropWidget()
{
	emit sig_refreshView_fromMainWin(mProjectDB.GetPipelineObjects());
}

void graph_MainWindow::slot_saveImage()
{
	func_saveImage(true, -1, -1, "invalid");
}

void graph_MainWindow::func_saveImage(bool flag_dlg, int wid, int hei, QString fileName)
{
	if (flag_dlg)
	{
		saveImageDialog* dlg = new saveImageDialog;
		dlg->setImageSize(QSize(pipelineObj_plotWidget->width(), pipelineObj_plotWidget->height()));
		if (dlg->exec() == QDialog::Accepted)
		{
			
			pipelineObj_plotWidget->set_saveImage_prop_data(dlg->get_saveImage_prop_data());
			QString tep_name = runAct->data().toString().trimmed();
			if (tep_name == "aniRun")
				pipelineObj_plotWidget->slot_saveImage();
		}
		delete dlg;
		dlg = NULL;
	}
	else
	{
		saveImage_prop tep_data;
		tep_data.image_wid = wid;
		tep_data.image_hei = hei;
		tep_data.flag_saveImage = true;
		tep_data.saveImage_fileName = fileName;

		tep_data.flag_savePov = false;
		tep_data.savePov_fileName = "";
		pipelineObj_plotWidget->set_saveImage_prop_data(tep_data);
		pipelineObj_plotWidget->slot_saveImage();
	}
	qDebug("Image is saved!\n");
}

void graph_MainWindow::saveAniSetting()
{
	saveAniDialog *dlg = new saveAniDialog(this);
	aniSetting_data.image_wid = pipelineObj_plotWidget->frameSize().width();
	aniSetting_data.image_hei = pipelineObj_plotWidget->frameSize().height();
	aniSetting_data.frame_start_xh = cur_frame_spinBox->value();
	aniSetting_data.frame_end_xh = pipelineObj_plotWidget->max_ani_frames - 1;
	aniSetting_data.file_interval = ani_fileInterval_spinBox->value();
	if (aniSetting_data.frame_start_xh == aniSetting_data.frame_end_xh)
		aniSetting_data.frame_start_xh = 0;
	dlg->updateDialog(&aniSetting_data);
	dlg->exec();
	delete dlg;
	dlg = NULL;
	if (aniSetting_data.flag_save && (aniSetting_data.frame_num>1))
		slot_runAni_fromAniSetting();
}

void graph_MainWindow::creatActions()
{
    fileMenu = menuBar()->addMenu(tr("File"));
    viewMenu = menuBar()->addMenu(tr("View"));
	filterMenu = menuBar()->addMenu(tr("Filter"));
    toolMenu = menuBar()->addMenu(tr("Tool"));
	VRMenu = menuBar()->addMenu(tr("VR"));
    helpMenu = menuBar()->addMenu(tr("Help"));

    const QIcon c_openIcon = QIcon::fromTheme("open", QIcon(":/images/open.png"));
	openAct = new QAction(c_openIcon, tr("open"), this);
    connect(openAct, &QAction::triggered, this, &graph_MainWindow::openFile);
    openAct->setShortcut(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file."));
    fileMenu->addAction(openAct);
	fileMenu->addSeparator();

	const QIcon c_scriptStartIcon = QIcon::fromTheme("scriptStart", QIcon(":/images/script_start.png"));
	runScriptAct = new QAction(c_scriptStartIcon, tr("Run script"), this);
	connect(runScriptAct, &QAction::triggered, this, &graph_MainWindow::runScript);
	runScriptAct->setStatusTip(tr("Run script file."));
	fileMenu->addAction(runScriptAct);
	const QIcon c_scriptEditIcon = QIcon::fromTheme("scriptEdit", QIcon(":/images/script_edit.png"));
	editScriptAct = new QAction(c_scriptEditIcon, tr("Edit script"), this);
	connect(editScriptAct, &QAction::triggered, this, &graph_MainWindow::editScript);
	editScriptAct->setStatusTip(tr("Generate script file."));
	fileMenu->addAction(editScriptAct);
	fileMenu->addSeparator();

	const QIcon c_saveImageIcon = QIcon::fromTheme("saveImage", QIcon(":/images/saveimage.png"));
	saveImageAct = new QAction(c_saveImageIcon, tr("saveImage"), this);
	connect(saveImageAct, &QAction::triggered, this, &graph_MainWindow::slot_saveImage);
	saveImageAct->setStatusTip(tr("save image."));
	//saveAniAct->setEnabled(false);
	fileMenu->addAction(saveImageAct);

	const QIcon c_saveAniIcon = QIcon::fromTheme("saveAnimation...", QIcon(":/images/saveAnimation.png"));
	saveAniAct = new QAction(c_saveAniIcon, tr("saveAnimation..."), this);
	connect(saveAniAct, &QAction::triggered, this, &graph_MainWindow::saveAniSetting);
	saveAniAct->setStatusTip(tr("save avi animation."));
	saveAniAct->setEnabled(false);
	fileMenu->addAction(saveAniAct);
	fileMenu->addSeparator();

	const QIcon c_exitIcon = QIcon::fromTheme("exie", QIcon(":/images/exit.png"));
	exitAct = new QAction(c_exitIcon, tr("exit"), this);
	connect(exitAct, SIGNAL(triggered()), this, SLOT(exitSoftware()));
	exitAct->setShortcut(QKeySequence::Quit);
	exitAct->setStatusTip(tr("Exit this software."));
	fileMenu->addAction(exitAct);

	const QIcon c_contourIcon = QIcon::fromTheme("contour", QIcon(":/images/isosurf.png"));
	contourAct = new QAction(c_contourIcon, tr("contour"), this);
	connect(contourAct, &QAction::triggered, this, &graph_MainWindow::slot_contourFilter);
	contourAct->setStatusTip(tr("contour filter."));
	filterMenu->addAction(contourAct);

	const QIcon c_clipIcon = QIcon::fromTheme("clip", QIcon(":/images/clip.png"));
	clipAct = new QAction(c_clipIcon, tr("clip"), this);
	connect(clipAct, &QAction::triggered, this, &graph_MainWindow::slot_clipFilter);
	clipAct->setStatusTip(tr("clipFilter"));
	filterMenu->addAction(clipAct);

	const QIcon c_sliceIcon = QIcon::fromTheme("slice", QIcon(":/images/slice.png"));
	sliceAct = new QAction(c_sliceIcon, tr("slice"), this);
	connect(sliceAct, &QAction::triggered, this, &graph_MainWindow::slot_sliceFilter);
	sliceAct->setStatusTip(tr("sliceFilter"));
	filterMenu->addAction(sliceAct);

	const QIcon c_glyphIcon = QIcon::fromTheme("glyph", QIcon(":/images/glyph.png"));
	glyphAct = new QAction(c_glyphIcon, tr("glyph"), this);
	connect(glyphAct, &QAction::triggered, this, &graph_MainWindow::slot_glyphFilter);
	glyphAct->setStatusTip(tr("glyphFilter"));
	filterMenu->addAction(glyphAct);

	const QIcon c_reflectionIcon = QIcon::fromTheme("reflection", QIcon(":/images/Reflection.png"));
	reflectionAct = new QAction(c_reflectionIcon, tr("reflection"), this);
	connect(reflectionAct, &QAction::triggered, this, &graph_MainWindow::slot_reflectionFilter);
	reflectionAct->setStatusTip(tr("reflectionFilter"));
	filterMenu->addAction(reflectionAct);

	const QIcon c_smoothIcon = QIcon::fromTheme("smooth", QIcon(":/images/smooth.png"));
	smoothAct = new QAction(c_smoothIcon, tr("smooth"), this);
	connect(smoothAct, &QAction::triggered, this, &graph_MainWindow::slot_smoothFilter);
	smoothAct->setStatusTip(tr("smooth filter."));
	filterMenu->addAction(smoothAct);

	const QIcon c_streamLineIcon = QIcon::fromTheme("streamLine", QIcon(":/images/streamline.png"));
	streamLineAct = new QAction(c_streamLineIcon, tr("streamLine"), this);
	connect(streamLineAct, &QAction::triggered, this, &graph_MainWindow::slot_streamLineFilter);
	streamLineAct->setStatusTip(tr("streamLineFilter"));
	filterMenu->addAction(streamLineAct);

	const QIcon c_calculateIcon = QIcon::fromTheme("calculate", QIcon(":/images/calculator.png"));
	calculateAct = new QAction(c_calculateIcon, tr("calculator"), this);
	connect(calculateAct, &QAction::triggered, this, &graph_MainWindow::slot_calculatorFilter);
	calculateAct->setStatusTip(tr("calculatorFilter"));
	filterMenu->addAction(calculateAct);
	//view
	const QIcon c_plotViewIcon = QIcon::fromTheme("view-direction", QIcon(":/images/XYZ.png"));
	plotViewMenu = new QMenu(tr("viewDirection"));
	plotViewMenu->setIcon(c_plotViewIcon);

	const QIcon c_viewInteractionIcon = QIcon::fromTheme("viewInteraction", QIcon(":/images/enableViewInteraction.png"));
	viewInteractionAct = new QAction(c_viewInteractionIcon, tr("viewInteraction"), this);
	viewInteractionAct->setStatusTip(tr("view interaction enable or disable"));
	viewInteractionAct->setDisabled(false);
	viewInteractionAct->setCheckable(true);
	plotViewMenu->addAction(viewInteractionAct);

	const QIcon c_resetViewIcon = QIcon::fromTheme("resetView", QIcon(":/images/reset.png"));
	resetViewAct = new QAction(c_resetViewIcon, tr("reset"), this);
	resetViewAct->setStatusTip(tr("reset view"));
	resetViewAct->setDisabled(true);
	plotViewMenu->addAction(resetViewAct);

	const QIcon c_XPlusIcon = QIcon::fromTheme("view-direction", QIcon(":/images/XPlus.png"));
	XPlusAct = new QAction(c_XPlusIcon, tr("+X"), this);
	XPlusAct->setStatusTip(tr("set view direction to +X"));
	XPlusAct->setDisabled(true);
	plotViewMenu->addAction(XPlusAct);
	const QIcon c_XMinusIcon = QIcon::fromTheme("view-direction", QIcon(":/images/XMinus.png"));
	XMinusAct = new QAction(c_XMinusIcon, tr("-X"), this);
	XMinusAct->setStatusTip(tr("set view direction to -X"));
	XMinusAct->setDisabled(true);
	plotViewMenu->addAction(XMinusAct);

	const QIcon c_YPlusIcon = QIcon::fromTheme("view-direction", QIcon(":/images/YPlus.png"));
	YPlusAct = new QAction(c_YPlusIcon, tr("+Y"), this);
	YPlusAct->setStatusTip(tr("set view direction to +Y"));
	YPlusAct->setDisabled(true);
	plotViewMenu->addAction(YPlusAct);
	const QIcon c_YMinusIcon = QIcon::fromTheme("view-direction", QIcon(":/images/YMinus.png"));
	YMinusAct = new QAction(c_YMinusIcon, tr("-Y"), this);
	YMinusAct->setStatusTip(tr("set view direction to -Y"));
	YMinusAct->setDisabled(true);
	plotViewMenu->addAction(YMinusAct);

	const QIcon c_ZPlusIcon = QIcon::fromTheme("view-direction", QIcon(":/images/ZPlus.png"));
	ZPlusAct = new QAction(c_ZPlusIcon, tr("+Z"), this);
	ZPlusAct->setStatusTip(tr("set view direction to +Z"));
	ZPlusAct->setDisabled(true);
	plotViewMenu->addAction(ZPlusAct);
	const QIcon c_ZMinusIcon = QIcon::fromTheme("view-direction", QIcon(":/images/ZMinus.png"));
	ZMinusAct = new QAction(c_ZMinusIcon, tr("-Z"), this);
	ZMinusAct->setStatusTip(tr("set view direction to -Z"));
	ZMinusAct->setDisabled(true);
	plotViewMenu->addAction(ZMinusAct);

	plotViewMenu->addSeparator();
	const QIcon c_cameraUndoIcon = QIcon::fromTheme("cameraUndo", QIcon(":/images/cameraUndo.png"));
	cameraUndoAct = new QAction(c_cameraUndoIcon, tr("camera undo"), this);
	cameraUndoAct->setStatusTip(tr("camera undo"));
	cameraUndoAct->setDisabled(true);
	plotViewMenu->addAction(cameraUndoAct);
	const QIcon c_cameraRedoIcon = QIcon::fromTheme("cameraRedo", QIcon(":/images/cameraRedo.png"));
	cameraRedoAct = new QAction(c_cameraRedoIcon, tr("camera redo"), this);
	cameraRedoAct->setStatusTip(tr("camera redo"));
	cameraRedoAct->setDisabled(true);
	plotViewMenu->addAction(cameraRedoAct);

	plotViewMenu->addSeparator();
	const QIcon c_rubberZoomIcon = QIcon::fromTheme("rubberZoom", QIcon(":/images/rubberZoom.png"));
	rubberZoomAct = new QAction(c_rubberZoomIcon, tr("rubberZoom"), this);
	rubberZoomAct->setStatusTip(tr("rubberBand Zoom"));
	rubberZoomAct->setDisabled(true);
	plotViewMenu->addAction(rubberZoomAct);
	//viewMenu->addMenu(menuView);
	//plotViewMenu->addSeparator();
	const QIcon c_actorMoveIcon = QIcon::fromTheme("actorMove", QIcon(":/images/actorMove.png"));
	actorMoveAct = new QAction(c_actorMoveIcon, tr("move actor"), this);
	actorMoveAct->setStatusTip(tr("move actor"));
	actorMoveAct->setDisabled(true);
	plotViewMenu->addAction(actorMoveAct);
	const QIcon c_actorRotateIcon = QIcon::fromTheme("actorRotate", QIcon(":/images/actorRotate.png"));
	actorRotateAct = new QAction(c_actorRotateIcon, tr("rotate actor"), this);
	actorRotateAct->setStatusTip(tr("rotate actor"));
	actorRotateAct->setDisabled(true);
	plotViewMenu->addAction(actorRotateAct);

	const QIcon c_actorRotateXIcon = QIcon::fromTheme("actorRotateX", QIcon(":/images/actorRotateX.png"));
	actorRotateXAct = new QAction(c_actorRotateXIcon, tr("rotateX actor"), this);
	actorRotateXAct->setStatusTip(tr("rotateX actor"));
	actorRotateXAct->setDisabled(true);
	plotViewMenu->addAction(actorRotateXAct);

	const QIcon c_actorRotateYIcon = QIcon::fromTheme("actorRotateY", QIcon(":/images/actorRotateY.png"));
	actorRotateYAct = new QAction(c_actorRotateYIcon, tr("rotateY actor"), this);
	actorRotateYAct->setStatusTip(tr("rotateY actor"));
	actorRotateYAct->setDisabled(true);
	plotViewMenu->addAction(actorRotateYAct);

	const QIcon c_actorRotateZIcon = QIcon::fromTheme("actorRotateZ", QIcon(":/images/actorRotateZ.png"));
	actorRotateZAct = new QAction(c_actorRotateZIcon, tr("rotateZ actor"), this);
	actorRotateZAct->setStatusTip(tr("rotateZ actor"));
	actorRotateZAct->setDisabled(true);
	plotViewMenu->addAction(actorRotateZAct);

	const QIcon c_langueIcon = QIcon::fromTheme("document-open", QIcon(":/images/langue.png"));
    langueMenu = new QMenu(tr("language"));
    //langueMenu->setIcon(langueIcon);
	const QIcon c_chineseIcon = QIcon::fromTheme("chinese_language", QIcon(":/images/chinese_language.png"));
	actionChinese = new QAction(c_chineseIcon, tr("chinese"), this);
	actionChinese->setCheckable(true);
	actionChinese->setChecked(false);
	//actionChinese->setEnabled(false);
	langueMenu->addAction(actionChinese);
	const QIcon c_englishIcon = QIcon::fromTheme("english_language", QIcon(":/images/english_language.png"));
	actionEnglish = new QAction(c_englishIcon, tr("english"), this);
	actionEnglish->setCheckable(true);
	actionEnglish->setChecked(true);
	//actionEnglish->setEnabled(false);
	langueMenu->addAction(actionEnglish);
    toolMenu->addMenu(langueMenu);


    menuAct = new QAction(tr("menu"), this);
    menuAct->setStatusTip(tr("show menu bar."));
    menuAct->setCheckable(true);
	menuAct->setChecked(true);
	viewMenu->addAction(menuAct);
	connect(menuAct, SIGNAL(triggered()), this, SLOT(slot_showMenuBar()));

    statusAct = new QAction(tr("status"), this);
    statusAct->setStatusTip(tr("show status bar."));
    statusAct->setCheckable(true);
	statusAct->setChecked(true);
    viewMenu->addAction(statusAct);
	connect(statusAct, SIGNAL(triggered()), this, SLOT(slot_showStatusBar()));
    
	toolAct = new QAction(tr("tool"), this);
    toolAct->setStatusTip(tr("show tool bar."));
    toolAct->setCheckable(true);
	toolAct->setChecked(true);
	viewMenu->addAction(toolAct);
	viewMenu->addSeparator();
	//viewMenu->addMenu(plotViewMenu);
	connect(toolAct, SIGNAL(triggered()), this, SLOT(slot_showToolBar()));
	connect(resetViewAct, SIGNAL(triggered()), this, SLOT(slot_resetView()));
	connect(viewInteractionAct, SIGNAL(triggered(bool)), this, SLOT(slot_viewInteraction(bool)));
	connect(rubberZoomAct, SIGNAL(triggered()), this, SLOT(slot_view_rubberZoom()));
	connect(XPlusAct, SIGNAL(triggered()), this, SLOT(slot_view_XPlus()));
	connect(YPlusAct, SIGNAL(triggered()), this, SLOT(slot_view_YPlus()));
	connect(ZPlusAct, SIGNAL(triggered()), this, SLOT(slot_view_ZPlus()));
	connect(XMinusAct, SIGNAL(triggered()), this, SLOT(slot_view_XMinus()));
	connect(YMinusAct, SIGNAL(triggered()), this, SLOT(slot_view_YMinus()));
	connect(ZMinusAct, SIGNAL(triggered()), this, SLOT(slot_view_ZMinus()));
	connect(cameraUndoAct, SIGNAL(triggered()), this, SLOT(slot_cameraUndo()));
	connect(cameraRedoAct, SIGNAL(triggered()), this, SLOT(slot_cameraRedo()));
	connect(actorMoveAct, SIGNAL(triggered()), this, SLOT(slot_actorMove()));
	connect(actorRotateAct, SIGNAL(triggered()), this, SLOT(slot_actorRotate()));
	connect(actorRotateXAct, SIGNAL(triggered()), this, SLOT(slot_actorRotateX()));
	connect(actorRotateYAct, SIGNAL(triggered()), this, SLOT(slot_actorRotateY()));
	connect(actorRotateZAct, SIGNAL(triggered()), this, SLOT(slot_actorRotateZ()));

	const QIcon c_aboutIcon = QIcon::fromTheme("help", QIcon(":/images/about.png"));
	const QIcon c_helpIcon = QIcon::fromTheme("help", QIcon(":/images/help.png"));
    helpAct = new QAction(c_helpIcon,tr("help"), this);
    helpAct->setStatusTip(tr("help"));
    connect(helpAct, SIGNAL(triggered()), this, SLOT(slot_showHelpPDF()));
    helpMenu->addAction(helpAct);
	aboutAct = new QAction(c_aboutIcon, tr("about"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(slot_showAboutDialog()));
    aboutAct->setStatusTip(tr("about"));
    helpMenu->addAction(aboutAct);

	const QIcon c_VRIcon = QIcon::fromTheme("VR", QIcon(":/images/VRrun.png"));
	VRAct = new QAction(c_VRIcon, tr("VR run"), this);
	VRAct->setData("VRrun");
	connect(VRAct, SIGNAL(triggered()), this, SLOT(slot_showVR()));
	VRAct->setStatusTip(tr("VR run"));
	VRMenu->addAction(VRAct);

	connect(actionEnglish, SIGNAL(triggered()), this, SLOT(slot_setEnglish()));
	connect(actionChinese, SIGNAL(triggered()), this, SLOT(slot_setChinese()));

	const QIcon c_mousePointIcon = QIcon::fromTheme("pick point", QIcon(":/images/mouse_point.png"));
	pickPointAct = new QAction(c_mousePointIcon, tr("pick point"), this);
	pickPointAct->setCheckable(true);
	pickPointAct->setStatusTip(tr("pick point"));
	pickPointAct->setDisabled(true);
	const QIcon c_mouseCellIcon = QIcon::fromTheme("pick cell", QIcon(":/images/mouse_cell.png"));
	pickCellAct = new QAction(c_mouseCellIcon, tr("pick cell"), this);
	pickCellAct->setCheckable(true);
	pickCellAct->setStatusTip(tr("pick cell"));
	pickCellAct->setDisabled(true);
	const QIcon c_mouseAreaCellIcon = QIcon::fromTheme("pick area cells", QIcon(":/images/mouse_areaCell.png"));
	pickAreaCellAct = new QAction(c_mouseAreaCellIcon, tr("pick area cells"), this);
	pickAreaCellAct->setCheckable(true);
	pickAreaCellAct->setStatusTip(tr("pick area cells"));
	pickAreaCellAct->setDisabled(true);
	const QIcon c_mouseAreaPointIcon = QIcon::fromTheme("pick area points", QIcon(":/images/mouse_areaPoint.png"));
	pickAreaPointAct = new QAction(c_mouseAreaPointIcon, tr("pick area points"), this);
	pickAreaPointAct->setCheckable(true);
	pickAreaPointAct->setStatusTip(tr("pick area points"));
	pickAreaPointAct->setDisabled(true);
	const QIcon c_mousePickClearIcon = QIcon::fromTheme("pick clear", QIcon(":/images/mouse_pickClear.png"));
	pickClearAct = new QAction(c_mousePickClearIcon, tr("pick clear"), this);
	pickClearAct->setStatusTip(tr("pick clear"));
	pickClearAct->setDisabled(true);
	const QIcon c_mouseCurvePointIcon = QIcon::fromTheme("pick curve point", QIcon(":/images/mouse_pickCurvePoint.png"));
	pickCurvePointAct = new QAction(c_mouseCurvePointIcon, tr("pick curve point"), this);
	pickCurvePointAct->setCheckable(true);
	pickCurvePointAct->setStatusTip(tr("pick curve point"));
	pickCurvePointAct->setDisabled(true);
	const QIcon c_mouseCurveCellIcon = QIcon::fromTheme("pick curve cell", QIcon(":/images/mouse_pickCurveCell.png"));
	pickCurveCellAct = new QAction(c_mouseCurveCellIcon, tr("pick curve cell"), this);
	pickCurveCellAct->setCheckable(true);
	pickCurveCellAct->setStatusTip(tr("pick curve cell"));
	pickCurveCellAct->setDisabled(true);
	connect(pickPointAct, SIGNAL(triggered(bool)), this, SLOT(slot_pickPointAct(bool)));
	connect(pickCellAct, SIGNAL(triggered(bool)), this, SLOT(slot_pickCellAct(bool)));
	connect(pickAreaCellAct, SIGNAL(triggered(bool)), this, SLOT(slot_pickAreaCellAct(bool)));
	connect(pickAreaPointAct, SIGNAL(triggered(bool)), this, SLOT(slot_pickAreaPointAct(bool)));
	connect(pickCurvePointAct, SIGNAL(triggered(bool)), this, SLOT(slot_pickCurvePointAct(bool)));
	connect(pickCurveCellAct, SIGNAL(triggered(bool)), this, SLOT(slot_pickCurveCellAct(bool)));
	connect(pickClearAct, SIGNAL(triggered()), this, SLOT(slot_pickClearAct()));

	fileToolBar = addToolBar(tr("&FileBar"));
    fileToolBar->addAction(openAct);
	fileToolBar->addSeparator();
	fileToolBar->addAction(editScriptAct);
	fileToolBar->addAction(runScriptAct);
	//fileToolBar->addSeparator();
	save_pngAviToolBar = addToolBar(tr("&savePngAviBar"));
	save_pngAviToolBar->addAction(saveImageAct);
	save_pngAviToolBar->addAction(saveAniAct);
	//fileToolBar->addSeparator();
    
	filterToolBar = addToolBar(tr("&FilterBar"));
	filterToolBar->addAction(contourAct);
	filterToolBar->addAction(clipAct);
	filterToolBar->addAction(sliceAct);
	filterToolBar->addAction(glyphAct);
	filterToolBar->addAction(reflectionAct);
	filterToolBar->addAction(smoothAct);
	filterToolBar->addAction(streamLineAct);
	filterToolBar->addAction(calculateAct);
	//filterToolBar->addSeparator();
	
	
	objProp_columnCmb=new QComboBox;
	objProp_columnCmb->clear();
	objProp_columnCmb->addItem("SolidColor");
	objProp_GlyphVector = new QComboBox;
	objProp_GlyphVector->clear();
	objProp_GlyphVector->addItem("X");
	objProp_GlyphVector->addItem("Y");
	objProp_GlyphVector->addItem("Z");
	objProp_presentCmb = new QComboBox;
	objProp_presentCmb->clear();
	objProp_presentCmb->addItem(QIcon::fromTheme("rep_point.png", QIcon(":/images/rep_point.png")), tr("Points"));
	objProp_presentCmb->addItem(QIcon::fromTheme("rep_surface.png", QIcon(":/images/rep_surface.png")), tr("Surface"));
	objProp_presentCmb->addItem(QIcon::fromTheme("rep_surfaceWithEdge.png", QIcon(":/images/rep_surfaceWithEdge.png")), tr("Surface With Edges"));
	objProp_presentCmb->addItem(QIcon::fromTheme("rep_wireFrame.png", QIcon(":/images/rep_wireFrame.png")), tr("Wireframe"));
	connect(objProp_presentCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_objProp_presentCmb_currentIndexChanged(int)));
	connect(objProp_columnCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_objProp_columnCmb_currentIndexChanged(int)));
	connect(objProp_GlyphVector, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_objProp_GlyphVector_currentIndexChanged(int)));
	
	const QIcon c_scalarBarIcon = QIcon::fromTheme("scalarBar", QIcon(":/images/ScalarBar.png"));
	objProp_scalarBarAct = new QAction(c_scalarBarIcon, tr("ScalarBar"), this);
	objProp_scalarBarAct->setStatusTip(tr("pipelineObject ScalarBar"));
	connect(objProp_scalarBarAct, SIGNAL(triggered()), this, SLOT(slot_objProp_scalarBarAct()));
	const QIcon c_solidColorIcon = QIcon::fromTheme("solidColor", QIcon(":/images/editColor.png"));
	objProp_solidColorAct = new QAction(c_solidColorIcon, tr("editColor"), this);
	objProp_solidColorAct->setStatusTip(tr("pipelineObject editColor"));
	connect(objProp_solidColorAct, SIGNAL(triggered()), this, SLOT(slot_objProp_solidColorAct()));
	//cgns animate times
	const QIcon c_cgnsTimesIcon = QIcon::fromTheme("cgnsAniTimes", QIcon(":/images/cgnsAniTimesStart.png"));
	cgnsAniTimesAct = new QAction(c_cgnsTimesIcon, tr("CgnsAniTimes"), this);
	cgnsAniTimesAct->setStatusTip(tr("Cgns pipelineObject animate times render!"));
	cgnsAniTimesAct->setData("cgnsAniTimesStart");
	connect(cgnsAniTimesAct, SIGNAL(triggered()), this, SLOT(slot_cgnsAniTimesAct()));
	//cgns
	const QIcon c_exportIcon = QIcon::fromTheme("export", QIcon(":/images/export.png"));
	exportAct = new QAction(c_exportIcon, tr("Export"), this);
	exportAct->setStatusTip(tr("export a solution file"));
	exportAct->setData("cgnsAniTimesStart");
	connect(exportAct, SIGNAL(triggered()), this, SLOT(slot_objProp_exportAct()));
	/*objPropToolBar = addToolBar(tr("&objPropBar"));
	objPropToolBar->addAction(objProp_scalarBarAct);
	objPropToolBar->addAction(objProp_solidColorAct);
	objPropToolBar->addWidget(objProp_columnCmb);
	objPropToolBar->addWidget(new QLabel(" "));
	objPropToolBar->addWidget(objProp_presentCmb);
	objPropToolBar->addWidget(new QLabel("  "));
	objPropToolBar->addAction(cgnsAniTimesAct);*/

	mousePickToolBar = addToolBar(tr("&mousePickBar"));
	mousePickToolBar->addAction(pickPointAct);
	mousePickToolBar->addAction(pickCellAct);
	mousePickToolBar->addAction(pickAreaPointAct);
	mousePickToolBar->addAction(pickAreaCellAct);
	mousePickToolBar->addAction(pickCurvePointAct);
	mousePickToolBar->addAction(pickCurveCellAct);
	mousePickToolBar->addAction(pickClearAct);

	const QIcon c_firstIcon = QIcon::fromTheme("first", QIcon(":/images/first.png"));
	firstAct = new QAction(c_firstIcon, tr("first frame"), this);
	firstAct->setStatusTip(tr("first frame"));
	connect(firstAct, SIGNAL(triggered()), this, SLOT(slot_animateFirst()));
	const QIcon c_lastIcon = QIcon::fromTheme("last", QIcon(":/images/last.png"));
	lastAct = new QAction(c_lastIcon, tr("last frame"), this);
	lastAct->setStatusTip(tr("last frame"));
	connect(lastAct, SIGNAL(triggered()), this, SLOT(slot_animateLast()));
	const QIcon c_runIcon = QIcon::fromTheme("run", QIcon(":/images/run.png"));
	runAct = new QAction(c_runIcon, tr("run"), this);
	runAct->setData("aniRun");
	connect(runAct, SIGNAL(triggered()), this, SLOT(slot_animate()));
	runAct->setStatusTip(tr("run"));
	const QIcon c_nextIcon = QIcon::fromTheme("next", QIcon(":/images/next.png"));
	nextAct = new QAction(c_nextIcon, tr("next frame"), this);
	nextAct->setStatusTip(tr("next frame"));
	connect(nextAct, SIGNAL(triggered()), this, SLOT(slot_animateNext()));
	const QIcon c_endIcon = QIcon::fromTheme("end", QIcon(":/images/end.png"));
	endAct = new QAction(c_endIcon, tr("end frame"), this);
	endAct->setStatusTip(tr("end frame"));
	connect(endAct, SIGNAL(triggered()), this, SLOT(slot_animateEnd()));
	AniToolBar = addToolBar(tr("&AnimateBar"));
	AniToolBar->addAction(firstAct);
	AniToolBar->addAction(lastAct);
	AniToolBar->addAction(runAct);
	AniToolBar->addAction(nextAct);
	AniToolBar->addAction(endAct);
	AniToolBar->addSeparator();
	lab1 = new QLabel(tr("frame:"));
	cur_frame_lineEdit = new QLineEdit;
	cur_frame_lineEdit->setReadOnly(true);
	cur_frame_lineEdit->setText("0");
	//connect(cur_frame_lineEdit, SIGNAL(textChanged(QString)), this, SLOT(slot_changeCurFrame_lineEdit(QString)));
	//QIntValidator *validator=new QIntValidator(0,20000,this);
	//cur_frame_lineEdit->setValidator(validator);
	cur_frame_spinBox = new QSpinBox;
	cur_frame_spinBox->setSingleStep(1);
	cur_frame_spinBox->setDisplayIntegerBase(10);
	cur_frame_spinBox->setCorrectionMode(QAbstractSpinBox::CorrectToPreviousValue);
	cur_frame_spinBox->setValue(0);
	cur_frame_spinBox->setMinimum(0);
	cur_frame_spinBox->setMaximum(0);
	connect(cur_frame_spinBox, SIGNAL(valueChanged(int)), this, SLOT(slot_changeCurFrame_spinBox(int)), Qt::QueuedConnection);
	QLabel *lab_blank = new QLabel(tr(" "));
	cur_frame_num = new QLabel(tr("  "));
	/////////////////////////////////////////////////////////////////////////////
	//add file interval
	QLabel *lab_blank2 = new QLabel(tr(" "));
	lab2 = new QLabel(tr("File Interval:"));
	ani_fileInterval_spinBox = new QSpinBox;
	//ani_fileInterval_spinBox->setReadOnly(true);
	//ani_fileInterval_spinBox->setFocusPolicy(Qt::NoFocus);
	ani_fileInterval_spinBox->setSingleStep(1);
	ani_fileInterval_spinBox->setDisplayIntegerBase(10);
	ani_fileInterval_spinBox->setCorrectionMode(QAbstractSpinBox::CorrectToPreviousValue);
	ani_fileInterval_spinBox->setValue(1);
	ani_fileInterval_spinBox->setMinimum(1);
	ani_fileInterval_spinBox->setMaximum(1000);
	connect(ani_fileInterval_spinBox, SIGNAL(valueChanged(int)), this, SLOT(slot_ani_fileInterval_spinBox(int)), Qt::QueuedConnection);
	/////////////////////////////////////////////////////////////////////////////
	AniToolBar->addWidget(lab1);
	AniToolBar->addWidget(cur_frame_lineEdit);
	AniToolBar->addWidget(lab_blank);
	AniToolBar->addWidget(cur_frame_spinBox);
	AniToolBar->addWidget(cur_frame_num);
	AniToolBar->addSeparator();
	AniToolBar->addWidget(lab_blank2);
	AniToolBar->addWidget(lab2);
	AniToolBar->addWidget(ani_fileInterval_spinBox);
	//lab_of->setVisible(false);
	cur_frame_num->setVisible(false);

    flag_saveAvi_label=new QLabel(tr("animate: "));
    flag_saveAvi_checkBox=new QCheckBox;
    flag_saveAvi_checkBox->setText(tr("save avi"));

	VRToolBar = addToolBar(tr("&VRBar"));
	VRToolBar->addAction(VRAct);
	exitToolBar = addToolBar(tr("&exitBar"));
    exitToolBar->addAction(exitAct);

	this->addToolBarBreak(Qt::TopToolBarArea);
	objPropToolBar = addToolBar(tr("&objPropBar"));
	objPropToolBar->addAction(objProp_scalarBarAct);
	objPropToolBar->addAction(objProp_solidColorAct);
	objPropToolBar->addWidget(objProp_columnCmb);
	objPropToolBar->addWidget(new QLabel(" "));
	objPropToolBar->addWidget(objProp_GlyphVector);
	objProp_GlyphVector->setEnabled(false);
	objPropToolBar->addWidget(new QLabel(" "));
	objPropToolBar->addWidget(objProp_presentCmb);
	objPropToolBar->addWidget(new QLabel("  "));
	objPropToolBar->addSeparator();
	objPropToolBar->addAction(cgnsAniTimesAct);
	objPropToolBar->addSeparator();
	objPropToolBar->addAction(exportAct);

	viewDirectionToolBar = addToolBar(tr("&viewDirectionBar"));
	viewDirectionToolBar->addAction(viewInteractionAct);
	viewDirectionToolBar->addSeparator();
	viewDirectionToolBar->addAction(resetViewAct);
	viewDirectionToolBar->addAction(XPlusAct);
	viewDirectionToolBar->addAction(XMinusAct);
	viewDirectionToolBar->addAction(YPlusAct);
	viewDirectionToolBar->addAction(YMinusAct);
	viewDirectionToolBar->addAction(ZPlusAct);
	viewDirectionToolBar->addAction(ZMinusAct);
	viewDirectionToolBar->addSeparator();
	viewDirectionToolBar->addAction(cameraUndoAct);
	viewDirectionToolBar->addAction(cameraRedoAct);
	viewDirectionToolBar->addSeparator();
	actorToolBar = addToolBar(tr("&actorBar"));
	actorToolBar->addAction(rubberZoomAct);
	actorToolBar->addAction(actorMoveAct);
	actorToolBar->addAction(actorRotateAct);
	actorToolBar->addAction(actorRotateXAct);
	actorToolBar->addAction(actorRotateYAct);
	actorToolBar->addAction(actorRotateZAct);

	contourAct->setEnabled(false);
	clipAct->setEnabled(false);
	sliceAct->setEnabled(false);
	glyphAct->setEnabled(false);
	reflectionAct->setEnabled(false);
	smoothAct->setEnabled(false);
	streamLineAct->setEnabled(false);
	calculateAct->setEnabled(false);

	firstAct->setEnabled(false);
	lastAct->setEnabled(false);
	runAct->setEnabled(false);
	nextAct->setEnabled(false);
	endAct->setEnabled(false);

	saveImageAct->setEnabled(false);
	saveAniAct->setEnabled(false);
	editScriptAct->setEnabled(false);

	cameraUndoAct->setEnabled(false);
	cameraRedoAct->setEnabled(false);
	resetViewAct->setEnabled(false);
	rubberZoomAct->setEnabled(false);
	actorMoveAct->setEnabled(false);
	actorRotateAct->setEnabled(false);
	actorRotateXAct->setEnabled(false);
	actorRotateYAct->setEnabled(false);
	actorRotateZAct->setEnabled(false);

	XPlusAct->setEnabled(false);
	YPlusAct->setEnabled(false);
	ZPlusAct->setEnabled(false);
	XMinusAct->setEnabled(false);
	YMinusAct->setEnabled(false);
	ZMinusAct->setEnabled(false);
	//objPropToolBar->setEnabled(false);
	objProp_scalarBarAct->setEnabled(false);
	objProp_solidColorAct->setEnabled(false);
	objProp_columnCmb->setEnabled(false);
	objProp_presentCmb->setEnabled(false);
	exportAct->setEnabled(false);

	//mousePickToolBar->setEnabled(false);
	pickCellAct->setEnabled(false);
	pickPointAct->setEnabled(false);
	pickAreaCellAct->setEnabled(false);
	pickAreaPointAct->setEnabled(false);
	pickCurvePointAct->setEnabled(false);
	pickCurveCellAct->setEnabled(false);
	pickClearAct->setEnabled(false);

	/*objProp_scalarBarAct->setEnabled(false);
	objProp_solidColorAct->setEnabled(false);*/
	//objPropToolBar->setEnabled(false);
	cgnsAniTimesAct->setEnabled(false);
	//VRAct->setEnabled(false);
}

void graph_MainWindow::slot_pickPointAct(bool checked)
{
	slot_pickClearAct();
	pickPointAct->setChecked(checked);
	if (checked)
	{
		pipelineObj_plotWidget->clear_pickPlot();
		pickCellAct->setChecked(false);
		pickAreaCellAct->setChecked(false);
		pickAreaPointAct->setChecked(false);
		pickCurvePointAct->setChecked(false);
		pickCurveCellAct->setChecked(false);
		pipelineObj_plotWidget->set_pickPointMode();
	}
}

void graph_MainWindow::slot_pickCellAct(bool checked)
{
	pickCellAct->setChecked(checked);
	if (checked)
	{
		pipelineObj_plotWidget->clear_pickPlot();
		pickPointAct->setChecked(false);
		pickAreaCellAct->setChecked(false);
		pickAreaPointAct->setChecked(false);
		pickCurvePointAct->setChecked(false);
		pickCurveCellAct->setChecked(false);
		pipelineObj_plotWidget->set_pickCellMode();
	}
	else
		slot_pickClearAct();

}

void graph_MainWindow::slot_pickAreaCellAct(bool checked)
{
	pickAreaCellAct->setChecked(checked);
	if (checked)
	{
		pipelineObj_plotWidget->clear_pickPlot();
		pickCellAct->setChecked(false);
		pickPointAct->setChecked(false);
		pickAreaPointAct->setChecked(false);
		pickCurvePointAct->setChecked(false);
		pickCurveCellAct->setChecked(false);
		pipelineObj_plotWidget->set_pickAreaMode(3,viewInteractionAct->isChecked());
		//QKeyEvent key_r(QEvent::KeyPress, Qt::Key_R, Qt::NoModifier);
		//QCoreApplication::sendEvent(pipelineObj_plotWidget->get_vtkWidget(), &key_r);
	}
	else
	{
		//slot_pickClearAct();
		pickCellAct->setChecked(false);
		pickPointAct->setChecked(false);
		pickAreaCellAct->setChecked(false);
		pickAreaPointAct->setChecked(false);
		pickCurvePointAct->setChecked(false);
		pickCurveCellAct->setChecked(false);
		pipelineObj_plotWidget->slot_restoreInteractor();
	}
}

void graph_MainWindow::slot_pickAreaPointAct(bool checked)
{
	pickAreaPointAct->setChecked(checked);
	if (checked)
	{
		pipelineObj_plotWidget->clear_pickPlot();
		pickCellAct->setChecked(false);
		pickPointAct->setChecked(false);
		pickAreaCellAct->setChecked(false);
		pickCurvePointAct->setChecked(false);
		pickCurveCellAct->setChecked(false);
		pipelineObj_plotWidget->set_pickAreaMode(2, viewInteractionAct->isChecked());
		//QKeyEvent key_r(QEvent::KeyPress, Qt::Key_R, Qt::NoModifier);
		//QCoreApplication::sendEvent(pipelineObj_plotWidget->get_vtkWidget(), &key_r);
	}
	else
	{
		//slot_pickClearAct();
		pickCellAct->setChecked(false);
		pickPointAct->setChecked(false);
		pickAreaCellAct->setChecked(false);
		pickAreaPointAct->setChecked(false);
		pickCurvePointAct->setChecked(false);
		pickCurveCellAct->setChecked(false);
		pipelineObj_plotWidget->slot_restoreInteractor();
	}
}

void graph_MainWindow::slot_pickClearAct()
{
	pipelineObj_plotWidget->clear_pickPlot();
	pickCellAct->setChecked(false);
	pickPointAct->setChecked(false);
	pickAreaCellAct->setChecked(false);
	pickAreaPointAct->setChecked(false);
	pickCurvePointAct->setChecked(false);
	pickCurveCellAct->setChecked(false);
}

void graph_MainWindow::slot_pickCurvePointAct(bool checked)
{
	pickCurvePointAct->setChecked(checked);
	if (checked)
	{
		pipelineObj_plotWidget->clear_pickPlot();
		pickCellAct->setChecked(false);
		pickPointAct->setChecked(false);
		pickAreaCellAct->setChecked(false);
		pickAreaPointAct->setChecked(false);
		pickCurveCellAct->setChecked(false);
		if (pickCurve_Widget == NULL)
		{
			pickCurve_Widget = new pickCurveDockWidget;
			pickCurve_Widget->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable);
			addDockWidget(Qt::BottomDockWidgetArea, pickCurve_Widget);
			viewMenu->addAction(pickCurve_Widget->toggleViewAction());
			connect(pipelineObj_plotWidget, SIGNAL(sig_draw_3d_curve(QString, QVector<double>)), pickCurve_Widget, SLOT(slot_draw_3d_curve(QString, QVector<double>)));
			connect(pipelineObj_plotWidget, SIGNAL(sig_update_3d_curve(int,int, double, double)), pickCurve_Widget, SLOT(slot_update_3d_curve(int,int, double, double)));
			emit sig_script_creatPickCurveWidget(pickCurve_Widget);
		}
		//pickCurve_Widget->setVisible(true);
		pipelineObj_plotWidget->set_pickCurvePointMode();
	}
	else
		slot_pickClearAct();
}

void graph_MainWindow::slot_pickCurveCellAct(bool checked)
{
	pickCurveCellAct->setChecked(checked);
	if (checked)
	{
		pipelineObj_plotWidget->clear_pickPlot();
		pickCellAct->setChecked(false);
		pickPointAct->setChecked(false);
		pickAreaCellAct->setChecked(false);
		pickAreaPointAct->setChecked(false);
		pickCurvePointAct->setChecked(false);
		if (pickCurve_Widget == NULL)
		{
			pickCurve_Widget = new pickCurveDockWidget;
			pickCurve_Widget->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable);
			addDockWidget(Qt::BottomDockWidgetArea, pickCurve_Widget);
			viewMenu->addAction(pickCurve_Widget->toggleViewAction());
			connect(pipelineObj_plotWidget, SIGNAL(sig_draw_3d_curve(QString, QVector<double>)), pickCurve_Widget, SLOT(slot_draw_3d_curve(QString, QVector<double>)));
			connect(pipelineObj_plotWidget, SIGNAL(sig_update_3d_curve(int,int, double, double)), pickCurve_Widget, SLOT(slot_update_3d_curve(int,int, double, double)));
			emit sig_script_creatPickCurveWidget(pickCurve_Widget);
		}
		//pickCurve_Widget->setVisible(true);
		pipelineObj_plotWidget->set_pickCurveCellMode();
	}
	else
		slot_pickClearAct();
}

void graph_MainWindow::slot_showToolBar()
{
	if (!toolAct->isChecked())
	{
		fileToolBar->setVisible(false);
		filterToolBar->setVisible(false);
		AniToolBar->setVisible(false);
		VRToolBar->setVisible(false);
		exitToolBar->setVisible(false);
	}
	else
	{
		fileToolBar->setVisible(true);
		filterToolBar->setVisible(true);
		AniToolBar->setVisible(true);
		VRToolBar->setVisible(true);
		exitToolBar->setVisible(true);
	}
}

void graph_MainWindow::slot_showMenuBar()
{
	if (menuAct->isChecked())
		menuBar()->show();
	else
		menuBar()->hide();
}

void graph_MainWindow::slot_showStatusBar()
{
	if (statusAct->isChecked())
		statusBar()->show();
	else
		statusBar()->hide();
}

void graph_MainWindow::slot_showHelpPDF()
{
    //QString runPath = QCoreApplication::applicationDirPath();
	QString runPath = QDir::currentPath();
    QString helpName = "help3D.pdf";
    QString helpPath = QString("%1/%2").arg(runPath).arg(helpName);
    QFile bfilePath(helpPath);
    if (!bfilePath.exists())
        return;
    QString filePath = "file:///" + helpPath;   //打开文件夹用file:///,打开网页用http://
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}

void graph_MainWindow::slot_showAboutDialog()
{
    //QMessageBox::information(NULL, tr("About"), tr("version : 1.0"));
	aboutDialog *dlg = new aboutDialog;
	dlg->exec();
}
void graph_MainWindow::slot_showVR()
{
	if (pipelineObj_plotWidget->max_ani_frames < 1)
		return;
	QString tep_name = VRAct->data().toString().trimmed();
	if (tep_name == "VRrun")
	{
		VRAct->setIcon(QIcon(":/images/VRstop.png"));
		VRAct->setData("VRstop");
		g_ani_objList = mProjectDB.GetPipelineObjects();
		saveAniAct->setEnabled(false);
		runAct->setEnabled(false);
		firstAct->setEnabled(false);
		endAct->setEnabled(false);
		nextAct->setEnabled(false);
		lastAct->setEnabled(false);
		cur_frame_spinBox->setEnabled(false);
		openAct->setEnabled(false);
		ani_fileInterval_spinBox->setEnabled(false);
		cgnsAniTimesAct->setEnabled(false);
		ani_update_all_form(true);
		QString tep_xmlFileName = QDir::currentPath();
		tep_xmlFileName += "/VRPara.xml";
		get_pipelineObj_treeWidget()->writeVRXml(tep_xmlFileName);
		QStringList arguments_list;
		arguments_list.clear();
		arguments_list.append("-"+tep_xmlFileName);

		if (VRProcess != NULL)
		{
			VRProcess->close();
			delete VRProcess;
			VRProcess = NULL;
		}
		QString VRExeFileName = QDir::currentPath();
		VRExeFileName += "/";
		VRExeFileName += "VR.exe";
		QFile file(VRExeFileName);
		if (!file.exists())
			return;
		VRProcess = new QProcess;
		VRProcess->setProcessChannelMode(QProcess::MergedChannels);
		//connect(VRProcess, SIGNAL(readyRead()), glomdiarea->parentWidget(), SLOT(outlog()));
		//connect(VRProcess, SIGNAL(finished(int, QProcess::ExitStatus)), glomdiarea->parentWidget(), SLOT(outlog_finish(int, QProcess::ExitStatus)));
		VRProcess->start(VRExeFileName, arguments_list);
		VRProcess->closeWriteChannel();
	}
	else if (tep_name == "VRstop")
	{
		VRAct->setIcon(QIcon(":/images/VRrun.png"));
		VRAct->setData("VRrun");
		runAct->setEnabled(true);
		saveAniAct->setEnabled(true);
		firstAct->setEnabled(true);
		endAct->setEnabled(true);
		nextAct->setEnabled(true);
		lastAct->setEnabled(true);
		cur_frame_spinBox->setEnabled(true);
		ani_fileInterval_spinBox->setEnabled(true);
		openAct->setEnabled(true);
		ani_update_all_form(false);
		
		if (VRProcess != NULL)
		{
			VRProcess->close();
			delete VRProcess;
			VRProcess = NULL;
		}
	}
}
void graph_MainWindow::slot_setEnglish()
{
	ChangeLanguage("English");
}

void graph_MainWindow::slot_setChinese()
{
	ChangeLanguage("Chinese");
}

void graph_MainWindow::do_contourFilter(PipelineObject *pipelineobj)
{
	FilterContour *contour_pipelineObj = new FilterContour("Contour", pipelineobj);
	pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors = false;
	
	if (pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible)
	{
		pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
		contour_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = true;
		if (pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.contains(pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle))
			pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert(pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle, true);
	}
	else
	{
		contour_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
	}
	contour_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors = true;
	AddPipelineObject(contour_pipelineObj);
	cur_PipelineObj = contour_pipelineObj;
}

void graph_MainWindow::slot_contourFilter()
{
	PipelineObject *pipelineobj = slot_adjust_addFilter();
	if (pipelineobj != NULL)
		do_contourFilter(pipelineobj);
}

void graph_MainWindow::slot_clipFilter()
{
	PipelineObject *pipelineobj = slot_adjust_addFilter();
	if (pipelineobj != NULL)
		do_clipFilter(pipelineobj);
}

void graph_MainWindow::slot_reflectionFilter()
{
	PipelineObject *pipelineobj = pipelineObj_treeWidget->GetActivePipelineObject();
	if (pipelineobj == NULL)
	{
		QMessageBox box(QMessageBox::Warning, QStringLiteral("warning"), QStringLiteral("This pipeline object is null,you can not add filter !"));
		box.setStandardButtons(QMessageBox::Yes);
		box.setButtonText(QMessageBox::Yes, QStringLiteral("ok"));
		box.exec();
		return;
	}
	/*if (pipelineobj->GetObjectType() != dNumeric_DataSource)
	{
		QMessageBox box(QMessageBox::Warning, QStringLiteral("warning"), QStringLiteral("This current pipeline object is not dataSource object！"));
		box.setStandardButtons(QMessageBox::Yes);
		box.setButtonText(QMessageBox::Yes, QStringLiteral("ok"));
		box.exec();
		return;
	}*/
	do_reflectionFilter(pipelineobj);
}

void graph_MainWindow::do_calculatorFilter(PipelineObject *pipelineobj)
{
	FilterCalculator *calculator_pipelineObj = new FilterCalculator("Calculator", pipelineobj);
	//pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors = false;

	if (pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible)
	{
		pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
		calculator_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = true;
		if (pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle == pipelineobj->mPipeLineObjProp.calculatorFilter_propData.resultName)
		{
			if (pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.contains("CalculatorResult"))
				pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert("CalculatorResult", true);
		}
		else
		{
			if (pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.contains(pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle))
				pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert(pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle, true);
		}
	}
	else
	{
		calculator_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
	}
	calculator_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors = true;
	//clip_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
	AddPipelineObject(calculator_pipelineObj);
	cur_PipelineObj = calculator_pipelineObj;
}
void graph_MainWindow::slot_calculatorFilter()
{
	PipelineObject *pipelineobj = pipelineObj_treeWidget->GetActivePipelineObject();
	if (pipelineobj == NULL)
	{
		QMessageBox box(QMessageBox::Warning, QStringLiteral("warning"), QStringLiteral("This pipeline object is null,you can not add filter !"));
		box.setStandardButtons(QMessageBox::Yes);
		box.setButtonText(QMessageBox::Yes, QStringLiteral("ok"));
		box.exec();
		return;
	}
	do_calculatorFilter(pipelineobj);
}

void graph_MainWindow::do_clipFilter(PipelineObject *pipelineobj)
{
	FilterClip *clip_pipelineObj = new FilterClip("Clip", pipelineobj);
	pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors = false;
	
	if (pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible)
	{
		pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
		clip_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = true;
		if (pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.contains(pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle))
			pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert(pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle, true);
	}
	else
	{
		clip_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
	}
	clip_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors = true;
	//clip_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
	AddPipelineObject(clip_pipelineObj);
	cur_PipelineObj = clip_pipelineObj;
}

void graph_MainWindow::do_reflectionFilter(PipelineObject *pipelineobj)
{
	FilterReflection *reflection_pipelineObj = new FilterReflection("Reflection", pipelineobj);
	//pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors = false;

	if (pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible)
	{
		pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
		reflection_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = true;
		if (pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.contains(pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle))
			pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert(pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle, true);
	}
	else
	{
		reflection_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
	}
	reflection_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors = true;
	//clip_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
	AddPipelineObject(reflection_pipelineObj);
	cur_PipelineObj = reflection_pipelineObj;
}

void graph_MainWindow::slot_sliceFilter()
{
	PipelineObject *pipelineobj = slot_adjust_addFilter();
	if (pipelineobj != NULL)
		do_sliceFilter(pipelineobj);
}

void graph_MainWindow::do_sliceFilter(PipelineObject *pipelineobj)
{
	FilterSlice *slice_pipelineObj = new FilterSlice("Slice", pipelineobj);
	pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors = false;
	
	if (pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible)
	{
		pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
		slice_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = true;
		if (pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.contains(pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle))
			pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert(pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle, true);
	}
	else
	{
		slice_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
	}
	slice_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors = true;
	//slice_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
	AddPipelineObject(slice_pipelineObj);
	cur_PipelineObj = slice_pipelineObj;
}

void graph_MainWindow::do_glyphFilter(PipelineObject *pipelineobj)
{
	FilterVector *vector_pipelineObj = new FilterVector("Vector", pipelineobj);
	vector_pipelineObj->mBaseGrid = ((NumericSrcObject*)pipelineobj)->mBaseGrid;
	vector_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors = true;
	vector_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
	AddPipelineObject(vector_pipelineObj);
	cur_PipelineObj = vector_pipelineObj;
}

void graph_MainWindow::slot_glyphFilter()
{
	PipelineObject *pipelineobj = slot_adjust_addFilter();
	if (pipelineobj != NULL)
		do_glyphFilter(pipelineobj);
}

void graph_MainWindow::do_streamLineFilter(PipelineObject *pipelineobj)
{
	FilterStreamLine *streamLine_pipelineObj = new FilterStreamLine("StreamLine", pipelineobj);
	pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors = false;
	if (pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible)
	{
		pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
		streamLine_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = true;
		if (pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.contains(pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle))
			pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert(pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle, true);
	}
	else
	{
		streamLine_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
	}
	streamLine_pipelineObj->mBaseGrid = ((NumericSrcObject*)pipelineobj)->mBaseGrid;
	streamLine_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors = true;
	streamLine_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
	AddPipelineObject(streamLine_pipelineObj);
	cur_PipelineObj = streamLine_pipelineObj;
}

void graph_MainWindow::slot_streamLineFilter()
{
	PipelineObject *pipelineobj = slot_adjust_addFilter();
	if (pipelineobj != NULL)
		do_streamLineFilter(pipelineobj);
}

void graph_MainWindow::do_smoothFilter(PipelineObject *pipelineobj)
{
	FilterSmooth *smooth_pipelineObj = new FilterSmooth("Smooth", pipelineobj);
	pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors = false;

	if (pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible)
	{
		pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
		smooth_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = true;
		if (pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.contains(pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle))
			pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert(pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle, true);
	}
	else
	{
		smooth_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
	}
	smooth_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors = true;
	//clip_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
	AddPipelineObject(smooth_pipelineObj);
	cur_PipelineObj = smooth_pipelineObj;
}

void graph_MainWindow::slot_smoothFilter()
{
	PipelineObject *pipelineobj = slot_adjust_addFilter();
	if (pipelineobj!=NULL)
		do_smoothFilter(pipelineobj);
}

PipelineObject* graph_MainWindow::slot_adjust_addFilter()
{
	PipelineObject *pipelineobj = pipelineObj_treeWidget->GetActivePipelineObject();
	if (pipelineobj == NULL)
	{
		QMessageBox box(QMessageBox::Warning, QStringLiteral("warning"), QStringLiteral("This pipeline object is null,you can not add filter!"));
		box.setStandardButtons(QMessageBox::Yes);
		box.setButtonText(QMessageBox::Yes, QStringLiteral("ok"));
		box.exec();
		return NULL;
	}
	if (pipelineobj->GetObjectType() != dNumeric_DataSource)
	{
		QMessageBox box(QMessageBox::Warning, QStringLiteral("warning"), QStringLiteral("This current pipeline object is not dataSource object!"));
		box.setStandardButtons(QMessageBox::Yes);
		box.setButtonText(QMessageBox::Yes, QStringLiteral("ok"));
		box.exec();
		return NULL;
	}
	return pipelineobj;
}

void graph_MainWindow::slot_update_filterToolBar(bool flag_selected, PipelineObject* tepObj)
{
	if (!flag_selected)
	{
		contourAct->setEnabled(false);
		clipAct->setEnabled(false);
		reflectionAct->setEnabled(false);
		sliceAct->setEnabled(false);
		glyphAct->setEnabled(false);
		smoothAct->setEnabled(false);
		streamLineAct->setEnabled(false);
		calculateAct->setEnabled(false);
		//objPropToolBar->setEnabled(false);
		objProp_scalarBarAct->setEnabled(false);
		objProp_solidColorAct->setEnabled(false);
		objProp_columnCmb->setEnabled(false);
		objProp_presentCmb->setEnabled(false);
		exportAct->setEnabled(false);
		cgnsAniTimesAct->setEnabled(false);
	}
	else if (tepObj != NULL)
	{
		if ((tepObj->GetObjectType() == dNumeric_DataSource) && tepObj->flag_exist_actors &&(QFileInfo(tepObj->mPipeLineObjProp.pipelineObj_base_propData.fileList.at(0)).suffix()!="moor"))
		{
			clipAct->setEnabled(true);
			sliceAct->setEnabled(true);
			glyphAct->setEnabled(true);
			reflectionAct->setEnabled(true);
			contourAct->setEnabled(true);
			/*if (tepObj->get_GridType() == dRECTILINEAR_GRID)
				contourAct->setEnabled(true);
			else
				contourAct->setEnabled(false);*/
			smoothAct->setEnabled(true);
			streamLineAct->setEnabled(true);
			calculateAct->setEnabled(true);
			if (tepObj->numberOfCgnsTimes>1)
				cgnsAniTimesAct->setEnabled(true);
		}
		else
		{
			contourAct->setEnabled(false);
			clipAct->setEnabled(false);
			sliceAct->setEnabled(false);
			glyphAct->setEnabled(false);
			reflectionAct->setEnabled(false);
			smoothAct->setEnabled(false);
			streamLineAct->setEnabled(false);
			calculateAct->setEnabled(false);
			cgnsAniTimesAct->setEnabled(false);
		}
		//objPropToolBar->setEnabled(true);
		objProp_scalarBarAct->setEnabled(true);
		objProp_solidColorAct->setEnabled(true);
		objProp_columnCmb->setEnabled(true);
		objProp_presentCmb->setEnabled(true);
		objProp_scalarBarAct->setEnabled(true);
		
	}
	cur_PipelineObj = tepObj;
}

void graph_MainWindow::slot_update_aniToolBar(int num)
{
	if (num ==0)
	{
		firstAct->setEnabled(false);
		lastAct->setEnabled(false);
		runAct->setEnabled(false);
		nextAct->setEnabled(false);
		endAct->setEnabled(false);
		saveAniAct->setEnabled(false);
		pickCurvePointAct->setEnabled(false);
		pickCurveCellAct->setEnabled(false);
		//VRAct->setEnabled(false);
	}
	else
	{
		firstAct->setEnabled(true);
		lastAct->setEnabled(true);
		runAct->setEnabled(true);
		nextAct->setEnabled(true);
		endAct->setEnabled(true);
		saveAniAct->setEnabled(true);
		pickCurvePointAct->setEnabled(true);
		pickCurveCellAct->setEnabled(true);
		//VRAct->setEnabled(true);
	}
}

void graph_MainWindow::newFile()
{
    creatDocWindows();
    if(pipelineObj_plotWidget==NULL)
    {
		pipelineObj_plotWidget = new PipelineObjPlotForm(this,AniToolBar);		
		//pipelineObj_plotWidget->setAniToolBarPtr(AniToolBar);
		//pipelineObj_plotWidget->updateForm();
		pipelineObj_plotWidget->aniSetting_data = &aniSetting_data;
		pipelineObj_plotWidget->m_renderViewPara = &(pipelineObj_propWidget->get_propWidget()->cur_renderview_global_prop_data);
		//pipelineObj_plotWidget->slot_change_global_view_prop();
		mdiArea->addSubWindow(pipelineObj_plotWidget);
		pipelineObj_plotWidget->showMaximized();//
		connect(this, SIGNAL(sig_refreshView_fromMainWin(QList<PipelineObject*>)), pipelineObj_plotWidget, SLOT(slot_refreshView_fromMainWin(QList<PipelineObject*>)));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_refreshView_fromPropWidget()), this, SLOT(slot_refreshView_fromPropWidget()));
		//connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_change_global_view_prop(renderview_global_prop)), pipelineObj_plotWidget, SLOT(slot_change_global_view_prop(renderview_global_prop)));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_change_global_view_prop()), pipelineObj_plotWidget, SLOT(slot_change_global_view_prop()));

		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_opacity(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_opacity(PipelineObject*)));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_color(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_color(PipelineObject*)));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_translate(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_translate(PipelineObject*)));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_origin(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_origin(PipelineObject*)));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_scale(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_scale(PipelineObject*)));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_orientation(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_orientation(PipelineObject*)));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_representation(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_representation(PipelineObject*)));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_axes_visible(bool)), pipelineObj_plotWidget, SLOT(slot_update_axes_visible(bool)));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_remark_visible(bool)), pipelineObj_plotWidget, SLOT(slot_update_remark(bool)));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_remark_fontColor()), pipelineObj_plotWidget, SLOT(slot_update_remark_fontColor()));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_remark_text()), pipelineObj_plotWidget, SLOT(slot_update_remark_text()));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_specular(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_specular(PipelineObject*)));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_specularPower(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_specularPower(PipelineObject*)));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_specularColor(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_specularColor(PipelineObject*)));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_solidColor(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_solidColor(PipelineObject*)));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_load_scalarBarEditForm(PipelineObject*)), this, SLOT(slot_load_scalarBarEditForm(PipelineObject*)));/////////
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_edgeColor(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_edgeColor(PipelineObject*)));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_diffuse(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_diffuse(PipelineObject*)));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_ambient(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_ambient(PipelineObject*)));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_interpolation(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_interpolation(PipelineObject*)));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_camera_parallel(bool)), pipelineObj_plotWidget, SLOT(slot_update_camera_parallel(bool)));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_cubeAxes(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_cubeAxes(PipelineObject*)));

		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_glyphVector(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_glyphVector(PipelineObject*)));

		//connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_scalarBar(PipelineObject*)), pipelineObj_treeWidget, SLOT(slot_update_scalarBar(PipelineObject*)));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_scalarBar(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_scalarBar(PipelineObject*)));
		connect(pipelineObj_treeWidget, SIGNAL(sig_update_scalarBar(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_scalarBar(PipelineObject*)));
		
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_lineWidth(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_lineWidth(PipelineObject*)));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_pointSize(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_pointSize(PipelineObject*)));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_filterToolBar(bool, PipelineObject *)), this, SLOT(slot_update_filterToolBar(bool, PipelineObject *)));

		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_planeOrigin(PipelineObject *)), pipelineObj_plotWidget, SLOT(slot_update_planeOrigin(PipelineObject *)));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_planeNormal(PipelineObject *)), pipelineObj_plotWidget, SLOT(slot_update_planeNormal(PipelineObject *)));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_planeCameraNormal(PipelineObject *)), pipelineObj_plotWidget, SLOT(slot_update_planeCameraNormal(PipelineObject *)));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_planeVis(PipelineObject *)), pipelineObj_plotWidget, SLOT(slot_update_planeVis(PipelineObject *)));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_toolBar_objProp_column(int)), this, SLOT(slot_update_toolBar_objProp_column(int)));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_toolBar_objProp_present(int)), this, SLOT(slot_update_toolBar_objProp_present(int)));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_toolBar_objProp_column_GlyphVector(bool)), this, SLOT(slot_update_toolBar_objProp_column_GlyphVector(bool)));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_toolBar_objProp_column_GlyphVectorXYZ(int)), this, SLOT(slot_update_toolBar_objProp_column_GlyphVectorXYZ(int)));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_setFlagRender(bool)), pipelineObj_propWidget->get_propWidget(), SLOT(slot_setFlagRender(bool)));
		connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_toolBar_objProp_column_calculator()), this, SLOT(slot_update_toolBar_objProp_column_calculator()));
		//
		connect(pipelineObj_plotWidget, SIGNAL(sig_script_update_currentObj(PipelineObject*)), pipelineObj_treeWidget, SLOT(DoSelectPipelineObject(PipelineObject *)));
		//20171122 add for contourFilter
		//connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_contourMin(PipelineObject *)), pipelineObj_plotWidget, SLOT(slot_update_contourMin(PipelineObject *)));
		//connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_contourMax(PipelineObject *)), pipelineObj_plotWidget, SLOT(slot_update_contourMax(PipelineObject *)));
		//connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_contourColName(PipelineObject *)), pipelineObj_plotWidget, SLOT(slot_update_contourColName(PipelineObject *)));
		//connect(pipelineObj_propWidget->get_propWidget(), SIGNAL(sig_update_contourColValue(PipelineObject *)), pipelineObj_plotWidget, SLOT(slot_update_contourColValue(PipelineObject *)));
		//20171122 add for contourFilter

		connect(pipelineObj_treeWidget, SIGNAL(sig_update_aniToolBar(int)), this, SLOT(slot_update_aniToolBar(int)));
		connect(pipelineObj_treeWidget, SIGNAL(sig_update_filterToolBar(bool, PipelineObject *)), this, SLOT(slot_update_filterToolBar(bool, PipelineObject *)));
		connect(pipelineObj_treeWidget, SIGNAL(sig_update_buildIn_propForm()), pipelineObj_propWidget->get_propWidget(), SLOT(slot_update_buildIn_propForm()));
		connect(pipelineObj_treeWidget, SIGNAL(sig_update_pipelineObj_propForm(PipelineObject*)), pipelineObj_propWidget->get_propWidget(), SLOT(slot_update_pipelineObj_propForm(PipelineObject*)));
		connect(pipelineObj_treeWidget, SIGNAL(sig_update_pipelineObj_toolBar(PipelineObject*)), this, SLOT(slot_update_pipelineObj_toolBar(PipelineObject*)));
		connect(pipelineObj_treeWidget, SIGNAL(sig_update_actor_visible_plotForm(PipelineObject*,bool)), pipelineObj_plotWidget, SLOT(slot_update_actor_visible_plotForm(PipelineObject*,bool)));
		//connect(pipelineObj_treeWidget, SIGNAL(sig_update_cgns_pipelineObj(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_cgns_pipelineObj(PipelineObject*)));
		connect(pipelineObj_plotWidget, SIGNAL(sig_script_refresh_pickData(struct pick_pipelineObj_propData)), pipelineObj_treeWidget, SLOT(slot_script_refresh_pickData(struct pick_pipelineObj_propData)));
		
		//connect(this, SIGNAL(sig_runAni()), pipelineObj_plotWidget, SLOT(slot_runAni()));
		//connect(this, SIGNAL(sig_stopAni()), pipelineObj_plotWidget, SLOT(slot_stopAni()));
		connect(pipelineObj_treeWidget, SIGNAL(sig_update_clear_plotAni()), pipelineObj_plotWidget, SLOT(slot_update_clear_plotAni()));
		connect(this, SIGNAL(sig_saveToPng()), pipelineObj_plotWidget, SLOT(slot_saveToPng()));
		connect(pipelineObj_plotWidget, SIGNAL(sig_finish_aviFile(QString)), this, SIGNAL(sig_finish_aviFile(QString)));//finish aviFile signal
		connect(pipelineObj_plotWidget, SIGNAL(sig_finish_aniThread(bool)), this, SLOT(slot_finish_aniThread(bool)));
		connect(pipelineObj_plotWidget, SIGNAL(sig_treeItem_disable(PipelineObject*)), pipelineObj_treeWidget, SLOT(slot_treeItem_disable(PipelineObject*)));
		connect(pipelineObj_plotWidget, SIGNAL(sig_update_countAniFrame(int,int)), this, SLOT(slot_update_countAniFrame(int,int)));
		connect(pipelineObj_plotWidget, SIGNAL(sig_script_user_custom_refresh_pipelineTree()), this, SLOT(slot_script_user_custom_refresh_pipelineTree()));//script add for user custom tree
		//void sig_addInfoToConsole(QString infoStr);
		connect(pipelineObj_plotWidget, SIGNAL(sig_addInfoToConsole(QString)), console_cmdWidget, SLOT(slot_addInfoToConsole(QString)));
		connect(this, SIGNAL(sig_addInfoToConsole(QString)), console_cmdWidget, SLOT(slot_addInfoToConsole(QString)));
		connect(pipelineObj_plotWidget, SIGNAL(sig_pickCellAct_status(bool)), this, SLOT(slot_pickCellAct_status(bool)));
		connect(pipelineObj_plotWidget, SIGNAL(sig_pickPointAct_status(bool)), this, SLOT(slot_pickPointAct_status(bool)));
		connect(pipelineObj_plotWidget, SIGNAL(sig_pickAreaCellAct_status(bool)), this, SLOT(slot_pickAreaCellAct_status(bool)));
		connect(pipelineObj_plotWidget, SIGNAL(sig_pickAreaPointAct_status(bool)), this, SLOT(slot_pickAreaPointAct_status(bool)));
		connect(pipelineObj_plotWidget, SIGNAL(sig_pickCurvePointAct_status(bool)), this, SLOT(slot_pickCurvePointAct_status(bool)));
		connect(pipelineObj_plotWidget, SIGNAL(sig_pickCurveCellAct_status(bool)), this, SLOT(slot_pickCurveCellAct_status(bool)));

		connect(pipelineObj_plotWidget, SIGNAL(sig_cgns_animnate_times(QString)), console_cmdWidget, SLOT(slot_update_console_cmd_form(QString)));
		connect(pipelineObj_plotWidget, SIGNAL(sig_cgnsAniTimesIcon_status(bool)), this, SLOT(slot_cgnsAniTimesIcon_status(bool)));

		connect(pipelineObj_plotWidget, SIGNAL(sig_update_UndoReDoButton(int)), this, SLOT(slot_update_UndoReDoButton(int)));
		connect(pipelineObj_plotWidget, SIGNAL(sig_exitSoftWare()), this, SLOT(script_exitSoftWare()));

		//add for script
		connect(this, SIGNAL(sig_script_saveImage()), pipelineObj_plotWidget, SLOT(slot_saveImage()));
    }
	QTreeWidgetItem *tep_item = pipelineObj_treeWidget->get_pipelineObjTree()->topLevelItem(0);
	pipelineObj_treeWidget->get_pipelineObjTree()->setCurrentItem(tep_item->child(0));
    initWindow();
	pipelineObj_plotWidget->set_consoleWidget(console_cmdWidget);
    flag_newFile=true;
	viewInteractionAct->setChecked(true);
	_signalHandler = new SignalHandlerPy(this);
}

void graph_MainWindow::slot_changeCurFrame_spinBox(int tep_val)
{
	cur_frame_spinBox->setEnabled(false);
	QString tep_name = runAct->data().toString().trimmed();
	if (tep_name == "aniStop")
	{
		return;
	}
	if ((tep_val < 0) || (tep_val >= pipelineObj_plotWidget->max_ani_frames))
		return;
	if (g_ani_objList.count() == 0)
		g_ani_objList = mProjectDB.GetPipelineObjects();
	QString tepstr;
	cur_frame_lineEdit->setText(tepstr.setNum(tep_val,10));
	pipelineObj_plotWidget->slot_changeCurFrame(tep_val);
	cur_frame_spinBox->setEnabled(true);
}

void graph_MainWindow::slot_ani_fileInterval_spinBox(int val)
{
	ani_fileInterval_spinBox->setEnabled(false);
	int num=mProjectDB.GetPipelineObjects().count();
	QStringList tep_list;
	int tep_max = 0;
	for (int i = 0; i < num; i++)
	{
		mProjectDB.GetPipelineObjects().at(i)->update_fileList_byFileInterval(val);
		if (mProjectDB.GetPipelineObjects().at(i)->mPipeLineObjProp.pipelineObj_base_propData.fileList.count()>tep_max)
			tep_max = mProjectDB.GetPipelineObjects().at(i)->mPipeLineObjProp.pipelineObj_base_propData.fileList.count();
	}
	pipelineObj_plotWidget->max_ani_frames = tep_max;
	pipelineObj_plotWidget->set_cur_ani_frame_xh(0);
	///////////////////////////////////////////////////
	QString tepstr;
	tepstr.setNum(pipelineObj_plotWidget->max_ani_frames, 10);
	cur_frame_num->setText(" of " + tepstr);
	cur_frame_num->setVisible(true);
	cur_frame_spinBox->setEnabled(false);
	if (pipelineObj_plotWidget->max_ani_frames>0)
		cur_frame_spinBox->setMaximum(pipelineObj_plotWidget->max_ani_frames - 1);
	else
		cur_frame_spinBox->setMaximum(0);		
	cur_frame_spinBox->setValue(0);
	cur_frame_spinBox->setEnabled(true);
	aniSetting_data.total_frame_num = pipelineObj_plotWidget->max_ani_frames;
	if (aniSetting_data.total_frame_num > 0)
		aniSetting_data.frame_end_xh = aniSetting_data.total_frame_num - 1;
	else
		aniSetting_data.frame_end_xh = 0;
	aniSetting_data.frame_start_xh = 0;
	////////////////////////////////////////////////////////
	ani_fileInterval_spinBox->setEnabled(true);
}

void graph_MainWindow::slot_ani_combine_model_comboBox(int)
{

}

void graph_MainWindow::slot_finish_aniThread(bool flag_runing)
{
	runAct->setIcon(QIcon(":/images/run.png"));
	runAct->setData("aniRun");
	saveAniAct->setEnabled(true);
	firstAct->setEnabled(true);
	endAct->setEnabled(true);
	nextAct->setEnabled(true);
	lastAct->setEnabled(true);
	cur_frame_spinBox->setEnabled(true);
	ani_fileInterval_spinBox->setEnabled(true);
	openAct->setEnabled(true);
	ani_update_all_form(false);
}

void graph_MainWindow::DisplayMessage(const QString& title, const QString& message, EMsgType type, bool onDlg)
{
	console_cmdWidget->DisplayMessage(title, message, type, onDlg);
}

void graph_MainWindow::initWindow()
{
	pipelineObj_treeWidget->initTree();
    //console_cmdWidget->slot_update_clear_console_command();
}

void graph_MainWindow::openFile()
{
	pipelineObj_treeWidget->slot_addFile();
}

void graph_MainWindow::editScript()
{	
	saveScriptDialog* dlg = new saveScriptDialog;
	dlg->saveScript_prop_data.console_hei = get_console_cmdWidget()->height();
	dlg->saveScript_prop_data.console_wid = get_console_cmdWidget()->width();
	dlg->saveScript_prop_data.tree_hei = get_pipelineObj_treeWidget()->height();
	dlg->saveScript_prop_data.tree_wid = get_pipelineObj_treeWidget()->width();
	dlg->saveScript_prop_data.prop_hei = get_pipelineObj_propWidget()->height();
	dlg->saveScript_prop_data.prop_wid = get_pipelineObj_propWidget()->width();
	dlg->saveScript_prop_data.plot_hei = get_pipelineObj_plotWidget()->height();
	dlg->saveScript_prop_data.plot_wid = get_pipelineObj_plotWidget()->width();
	dlg->saveScript_prop_data.mainwindow_hei = height();
	dlg->saveScript_prop_data.mainwindow_wid = width();
	dlg->setImageSize(QSize(dlg->saveScript_prop_data.plot_wid, dlg->saveScript_prop_data.plot_hei));
	if (dlg->exec() == QDialog::Accepted)
	{		
		saveScript_prop saveScript_prop_data=dlg->get_saveScript_prop_data();
		QString script_filename = saveScript_prop_data.saveScript_fileName;
		//QString tep_filename = QFileDialog::getSaveFileName(this, tr("Save Script"), "tep_Script", "images(*.scr)");
		if (script_filename == "")
			return;
		pipelineObj_plotWidget->func_saveRemarkPosition();
		pipelineObj_treeWidget->script_set_cameraPara(pipelineObj_plotWidget->getCameraPara());//pipelineObj_treeWidget->script_cameraPara = pipelineObj_plotWidget->getCameraPara();
		pipelineObj_treeWidget->script_set_renderViewPara(pipelineObj_propWidget->get_propWidget()->get_renderView_prop());//pipelineObj_treeWidget->script_renderViewPara = pipelineObj_propWidget->propWidget->get_renderView_prop();
		pipelineObj_treeWidget->set_viewInteraction(viewInteractionAct->isChecked());
		pipelineObj_treeWidget->saveScript_prop_data.flag_saveAvi = saveScript_prop_data.flag_saveAvi;
		pipelineObj_treeWidget->saveScript_prop_data.flag_saveImage = saveScript_prop_data.flag_saveImage;
		pipelineObj_treeWidget->saveScript_prop_data.image_hei = saveScript_prop_data.image_hei;
		pipelineObj_treeWidget->saveScript_prop_data.image_wid = saveScript_prop_data.image_wid;
		if (saveScript_prop_data.flag_saveImage)
			pipelineObj_treeWidget->saveScript_prop_data.saveImage_fileName = saveScript_prop_data.saveImage_fileName;
		else
			pipelineObj_treeWidget->saveScript_prop_data.saveImage_fileName = "";
		if (saveScript_prop_data.flag_saveAvi)
			pipelineObj_treeWidget->saveScript_prop_data.saveAvi_fileName = saveScript_prop_data.saveAvi_fileName;
		else
			pipelineObj_treeWidget->saveScript_prop_data.saveAvi_fileName = "";
		pipelineObj_treeWidget->saveScript_prop_data.plot_wid = saveScript_prop_data.plot_wid;
		pipelineObj_treeWidget->saveScript_prop_data.plot_hei = saveScript_prop_data.plot_hei;
		pipelineObj_treeWidget->saveScript_prop_data.tree_wid = saveScript_prop_data.tree_wid;
		pipelineObj_treeWidget->saveScript_prop_data.tree_hei = saveScript_prop_data.tree_hei;
		pipelineObj_treeWidget->saveScript_prop_data.prop_wid = saveScript_prop_data.prop_wid;
		pipelineObj_treeWidget->saveScript_prop_data.prop_hei = saveScript_prop_data.prop_hei;
		pipelineObj_treeWidget->saveScript_prop_data.console_wid = saveScript_prop_data.console_wid;
		pipelineObj_treeWidget->saveScript_prop_data.console_hei = saveScript_prop_data.console_hei;
		pipelineObj_treeWidget->saveScript_prop_data.mainwindow_wid = saveScript_prop_data.mainwindow_wid;
		pipelineObj_treeWidget->saveScript_prop_data.mainwindow_hei = saveScript_prop_data.mainwindow_hei;
		pipelineObj_treeWidget->saveScript_prop_data.flag_plotSize = saveScript_prop_data.flag_plotSize;
		pipelineObj_treeWidget->saveScript_prop_data.flag_autoExit = saveScript_prop_data.flag_autoExit;
		pipelineObj_treeWidget->saveScript_prop_data.timeMS_autoExit = saveScript_prop_data.timeMS_autoExit;
		pipelineObj_treeWidget->saveScript_prop_data.flag_adapt = saveScript_prop_data.flag_adapt;
		pipelineObj_treeWidget->editScript(script_filename);
	}
	delete dlg;
	dlg = NULL;
}

void graph_MainWindow::runScript()
{
	QString title = tr("Run Script");
	QString filter = tr("Script(*.scr)");
	QString path = QFileDialog::getOpenFileName(this, title, "", filter);
	if (path.isEmpty()) 
		return;
	RunScript(path);
}

void graph_MainWindow::RunScript(QString fileName)
{
	ScriptHandler handler(this, fileName);
	connect(&handler, SIGNAL(sig_script_clearAllPipelineObjs(int)), this, SLOT(slot_script_clearAllPipelineObjs(int)));
	connect(&handler, SIGNAL(sig_script_setPlotSize(int*)), this, SLOT(slot_script_setPlotSize(int*)));
	get_pipelineObj_plotWidget()->setScriptRunning(true);
	if (flag_commandLine)
		handler.flag_commandLine = true;
	else
		handler.flag_commandLine = false;
	//flag_script_running = true;
	handler.Handle();
	//flag_script_running = false;
	
}
void graph_MainWindow::slot_script_setPlotSize(int* val)
{
	script_setPlotSize(val);
}
void graph_MainWindow::exitSoftware()
{
    close();
}

void graph_MainWindow::creatDocWindows()
{
    if(!flag_DockWin)
    {
        setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
        setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

        setDockNestingEnabled(true);

		pipelineObj_treeWidget = new PipelineObjTreeDockWidget(this);
		pipelineObj_treeWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
		addDockWidget(Qt::LeftDockWidgetArea, pipelineObj_treeWidget);
		viewMenu->addAction(pipelineObj_treeWidget->toggleViewAction());

        console_cmdWidget = new consoleCmdDockWidget(this);
        addDockWidget(Qt::BottomDockWidgetArea, console_cmdWidget);
        viewMenu->addAction(console_cmdWidget->toggleViewAction());

		pipelineObj_propWidget = new PipelineObjPropDockWidget(this);
		pipelineObj_propWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
		addDockWidget(Qt::LeftDockWidgetArea, pipelineObj_propWidget);
		viewMenu->addAction(pipelineObj_propWidget->toggleViewAction());
		
		connect(pipelineObj_treeWidget, SIGNAL(sig_update_console_cmd_form(QString)), console_cmdWidget, SLOT(slot_update_console_cmd_form(QString)));
		connect(pipelineObj_treeWidget, SIGNAL(sig_update_clear_console_command()), console_cmdWidget, SLOT(slot_update_clear_console_command()));
		connect(pipelineObj_treeWidget, SIGNAL(sig_delPipelineObj(PipelineObject*)), this, SLOT(slot_delPipelineObj(PipelineObject*)));
		//connect(pipelineObj_treeWidget, SIGNAL(sig_update_cgns_pipelineObj(PipelineObject* pipeObj)), this, SLOT(slot_update_cgns_pipelineObj(PipelineObject* pipeObj)));
		connect(pipelineObj_treeWidget, SIGNAL(sig_LoadDataSource(QString, QStringList,int)), this, SLOT(slot_LoadDataSource(QString, QStringList,int)));
		
        flag_DockWin=true;
    }
    else
    {
		pipelineObj_treeWidget->show();
        pipelineObj_propWidget->show();
        console_cmdWidget->show();

		pipelineObj_treeWidget->initTree();
    }
}

void graph_MainWindow::slot_runAni_fromAniSetting()
{
	if (pipelineObj_plotWidget->max_ani_frames < 2)
		return;
	pipelineObj_plotWidget->set_cur_ani_frame_xh(aniSetting_data.frame_start_xh);//20190429 add
	QString tep_name = runAct->data().toString().trimmed();
	if (tep_name == "aniStop")
		return;
	slot_animate();
}


void graph_MainWindow::slot_animate()
{
	LoadActionFile* ptr = new LoadActionFile;
	int tep_num_maxDisplace = ptr->func_get_max_num_displace(g_ani_objList);
	delete ptr;
	ptr = NULL;
	pipelineObj_plotWidget->func_set_num_maxDisplace(tep_num_maxDisplace);
	if ((pipelineObj_plotWidget->max_ani_frames < 2) && (tep_num_maxDisplace<2))
		return;
	QString tep_name = runAct->data().toString().trimmed();
	if (tep_name == "aniRun")
	{
		if ((pipelineObj_plotWidget->max_ani_frames >1) || (tep_num_maxDisplace>1))
		{
			pipelineObj_treeWidget->projectTreeItemClicked(pipelineObj_treeWidget->get_pipelineObjTree()->topLevelItem(0), 0);
			pipelineObj_plotWidget->slot_runAni();
		}
		runAct->setIcon(QIcon(":/images/stop.png"));
		runAct->setData("aniStop");
		g_ani_objList = mProjectDB.GetPipelineObjects();
		saveAniAct->setEnabled(false);
		firstAct->setEnabled(false);
		endAct->setEnabled(false);
		nextAct->setEnabled(false);
		lastAct->setEnabled(false);
		cur_frame_spinBox->setEnabled(false);
		openAct->setEnabled(false);
		ani_fileInterval_spinBox->setEnabled(false);
		cgnsAniTimesAct->setEnabled(false);
		ani_update_all_form(true);
		
		if (cur_PipelineObj != NULL)
		{
			cur_PipelineObj = NULL;
		}
	}
	else if (tep_name == "aniStop")
	{
		pipelineObj_plotWidget->slot_stopAni();
		runAct->setIcon(QIcon(":/images/run.png"));
		runAct->setData("aniRun");
		saveAniAct->setEnabled(true);
		firstAct->setEnabled(true);
		endAct->setEnabled(true);
		nextAct->setEnabled(true);
		lastAct->setEnabled(true);
		cur_frame_spinBox->setEnabled(true);
		ani_fileInterval_spinBox->setEnabled(true);
		openAct->setEnabled(true);
		ani_update_all_form(false);
		slot_update_pipelineObjs();
	}
}

void graph_MainWindow::slot_animateFirst()
{
	if (pipelineObj_plotWidget->max_ani_frames > 1)
	{
		cur_frame_spinBox->setValue(0);
		cur_frame_lineEdit->setText("0");
	}
	else
	{
		LoadActionFile* ptr = new LoadActionFile;
		int tep_max = ptr->func_get_max_num_displace(g_ani_objList);
		delete ptr;
		ptr = NULL;
		if (tep_max > 0)
		{
			int tep_num =0;
			if (func_aniToolBar(tep_num))
					pipelineObj_plotWidget->slot_changeCurFrame(tep_num);
		}
	}
}

void graph_MainWindow::slot_animateLast()
{
	if (pipelineObj_plotWidget->max_ani_frames > 1)
	{
		int tep_val = cur_frame_spinBox->value();
		if (tep_val > cur_frame_spinBox->minimum())
		{
			tep_val--;
			cur_frame_spinBox->setValue(tep_val);
			QString tepstr;
			tepstr.setNum(tep_val, 10);
			cur_frame_lineEdit->setText(tepstr);
		}
	}
	else
	{
		LoadActionFile* ptr = new LoadActionFile;
		int tep_max = ptr->func_get_max_num_displace(g_ani_objList);
		delete ptr;
		ptr = NULL;
		if (tep_max > 0)
		{
			int tep_num = pipelineObj_plotWidget->get_cur_ani_dis_xh(); tep_num--;
			if ((tep_num < tep_max) && (tep_num >= 0))
			{
				if (func_aniToolBar(tep_num))
					pipelineObj_plotWidget->slot_changeCurFrame(tep_num);
			}
		}
	}
}

void graph_MainWindow::slot_animateEnd()
{
	if (pipelineObj_plotWidget->max_ani_frames > 1)
	{
		cur_frame_spinBox->setValue(pipelineObj_plotWidget->max_ani_frames - 1);
		QString tepstr;
		tepstr.setNum(pipelineObj_plotWidget->max_ani_frames - 1, 10);
		cur_frame_lineEdit->setText(tepstr);
		//pipelineObj_plotWidget->slot_changeCurFrame(pipelineObj_plotWidget->max_ani_frames - 1);
	}
	else
	{
		LoadActionFile* ptr = new LoadActionFile;
		int tep_max = ptr->func_get_max_num_displace(g_ani_objList);
		delete ptr;
		ptr = NULL;
		if (tep_max > 0)
		{
			int tep_num=tep_max-1;
			if (func_aniToolBar(tep_num))
				pipelineObj_plotWidget->slot_changeCurFrame(tep_num);
		}
	}
}

void graph_MainWindow::slot_animateNext()
{
	if (pipelineObj_plotWidget->max_ani_frames > 1)
	{
		int tep_val = cur_frame_spinBox->value();
		if (tep_val < cur_frame_spinBox->maximum())
		{
			tep_val++;
			cur_frame_spinBox->setValue(tep_val);
			QString tepstr;
			tepstr.setNum(tep_val, 10);
			cur_frame_lineEdit->setText(tepstr);
			//pipelineObj_plotWidget->slot_changeCurFrame(tep_val);
		}
	}
	else
	{
		LoadActionFile* ptr = new LoadActionFile;
		int tep_max = ptr->func_get_max_num_displace(g_ani_objList);
		delete ptr;
		ptr = NULL;
		if (tep_max > 0)
		{
			int tep_num = pipelineObj_plotWidget->get_cur_ani_dis_xh(); tep_num++;
			if ((tep_num < tep_max) && (tep_num >= 0))
			{
				if (func_aniToolBar(tep_num))
					pipelineObj_plotWidget->slot_changeCurFrame(tep_num);
			}
		}
	}
}

void graph_MainWindow::ani_update_all_form(bool flag_start)
{
	pipelineObj_propWidget->get_propWidget()->setEnabled(!flag_start);
	pipelineObj_treeWidget->setEnabled(!flag_start);
	contourAct->setEnabled(!flag_start);
	clipAct->setEnabled(!flag_start);
	sliceAct->setEnabled(!flag_start);
	glyphAct->setEnabled(!flag_start);
	reflectionAct->setEnabled(!flag_start);
	smoothAct->setEnabled(!flag_start);
	streamLineAct->setEnabled(!flag_start);
	calculateAct->setEnabled(!flag_start);
	openAct->setEnabled(!flag_start);
	//objPropToolBar->setEnabled(!flag_start);
	objProp_scalarBarAct->setEnabled(!flag_start);
	objProp_solidColorAct->setEnabled(!flag_start);
	objProp_columnCmb->setEnabled(!flag_start);
	if (flag_start)
		objProp_GlyphVector->setEnabled(false);
	else
	{
		if (cur_PipelineObj != NULL)
		{
			if (cur_PipelineObj->GetObjectType() == dVector_DataSource)
			{
				if (cur_PipelineObj->mPipeLineObjProp.vectorFilter_propData.vectorColName != "None")
				{
					objProp_GlyphVector->setEnabled(true);
				}
				else
					objProp_GlyphVector->setEnabled(false);
			}
			else
				objProp_GlyphVector->setEnabled(false);
		}
		else
			objProp_GlyphVector->setEnabled(false);
	}
	objProp_presentCmb->setEnabled(!flag_start);
	exportAct->setEnabled(!flag_start);
}

void graph_MainWindow::slot_update_pipelineObjs()//QList<PipelineObject*> tep_pipelineObjs
{
	//mProjectDB.update_pipelineObjs(glo_ani_objList);
	pipelineObj_plotWidget->slot_finish_saveAvi();
	pipelineObj_treeWidget->setEnabled(true);
	pipelineObj_propWidget->get_propWidget()->setEnabled(true);
	pipelineObj_treeWidget->update_pipelineObjs(g_ani_objList);
	/*runAct->setIcon(QIcon(":/images/run.png"));
	runAct->setData("aniRun");
	saveAniAct->setEnabled(true);
	firstAct->setEnabled(true);
	lastAct->setEnabled(true);
	nextAct->setEnabled(true);
	endAct->setEnabled(true);
	cur_frame_spinBox->setEnabled(true);
	openAct->setEnabled(true);*/
	/*pickCellAct->setEnabled(true);
	pickPointAct->setEnabled(true);
	pickAreaCellAct->setEnabled(true);
	pickAreaPointAct->setEnabled(true);
	pickCurvePointAct->setEnabled(true);
	pickCurveCellAct->setEnabled(true);
	pickClearAct->setEnabled(true);*/
}

void graph_MainWindow::slot_update_countAniFrame(int tep_cur_frame_xh, int tep_max_num_frame)
{
	QString tep_str;
	cur_frame_lineEdit->setText(tep_str.setNum(tep_cur_frame_xh));
	cur_frame_spinBox->setValue(tep_cur_frame_xh);
	//qDebug() << "************* cur frame xh =    " << tep_cur_frame_xh << endl;
	emit sig_script_countAniFrame(tep_cur_frame_xh);
}

void graph_MainWindow::slot_resetView()
{
	pipelineObj_plotWidget->resetView();
	emit sig_python_resetView();
}

void graph_MainWindow::slot_viewInteraction(bool checked)
{
	if (checked)
		viewInteractionAct->setIcon(QIcon(":/images/enableViewInteraction.png"));
	else
		viewInteractionAct->setIcon(QIcon(":/images/disableViewInteraction.png"));
	pipelineObj_plotWidget->func_viewInteraction(checked);
}

void graph_MainWindow::slot_view_rubberZoom()
{
	pipelineObj_plotWidget->func_set_rubberZoom(viewInteractionAct->isChecked());
	pickCellAct->setChecked(false);
	pickPointAct->setChecked(false);
	pickAreaPointAct->setChecked(false);
	pickAreaCellAct->setChecked(false);
	pickCurvePointAct->setChecked(false);
	pickCurveCellAct->setChecked(false);
}

void graph_MainWindow::slot_actorMove()
{
	pipelineObj_plotWidget->func_set_actorOperate(kActor_Move,viewInteractionAct->isChecked());
	pickCellAct->setChecked(false);
	pickPointAct->setChecked(false);
	pickAreaPointAct->setChecked(false);
	pickAreaCellAct->setChecked(false);
	pickCurvePointAct->setChecked(false);
	pickCurveCellAct->setChecked(false);
	
}

void graph_MainWindow::slot_cameraUndo()
{
	pipelineObj_plotWidget->slot_cameraUndo();
}

void graph_MainWindow::slot_cameraRedo()
{
	pipelineObj_plotWidget->slot_cameraRedo();
}

void graph_MainWindow::slot_actorRotate()
{
	pipelineObj_plotWidget->func_set_actorOperate(kActor_Rotate, viewInteractionAct->isChecked());
	pickCellAct->setChecked(false);
	pickPointAct->setChecked(false);
	pickAreaPointAct->setChecked(false);
	pickAreaCellAct->setChecked(false);
	pickCurvePointAct->setChecked(false);
	pickCurveCellAct->setChecked(false);
	
}

void graph_MainWindow::slot_actorRotateX()
{
	pipelineObj_plotWidget->func_set_actorOperate(kActor_RotateX, viewInteractionAct->isChecked());
	pickCellAct->setChecked(false);
	pickPointAct->setChecked(false);
	pickAreaPointAct->setChecked(false);
	pickAreaCellAct->setChecked(false);
	pickCurvePointAct->setChecked(false);
	pickCurveCellAct->setChecked(false);
	
}

void graph_MainWindow::slot_actorRotateZ()
{
	pipelineObj_plotWidget->func_set_actorOperate(kActor_RotateZ, viewInteractionAct->isChecked());
	pickCellAct->setChecked(false);
	pickPointAct->setChecked(false);
	pickAreaPointAct->setChecked(false);
	pickAreaCellAct->setChecked(false);
	pickCurvePointAct->setChecked(false);
	pickCurveCellAct->setChecked(false);
	
}

void graph_MainWindow::slot_actorRotateY()
{
	pipelineObj_plotWidget->func_set_actorOperate(kActor_RotateY, viewInteractionAct->isChecked());
	pickCellAct->setChecked(false);
	pickPointAct->setChecked(false);
	pickAreaPointAct->setChecked(false);
	pickAreaCellAct->setChecked(false);
	pickCurvePointAct->setChecked(false);
	pickCurveCellAct->setChecked(false);
	
}

void graph_MainWindow::slot_view_XPlus()
{
	pipelineObj_plotWidget->setViewDirection("XPlus");
	emit sig_python_view_XPlus();
}

void graph_MainWindow::slot_view_YPlus()
{
	pipelineObj_plotWidget->setViewDirection("YPlus");
	emit sig_python_view_YPlus();
}

void graph_MainWindow::slot_view_ZPlus()
{
	pipelineObj_plotWidget->setViewDirection("ZPlus");
	emit sig_python_view_ZPlus();
}

void graph_MainWindow::slot_view_XMinus()
{
	pipelineObj_plotWidget->setViewDirection("XMinus");
	emit sig_python_view_XMinus();
}

void graph_MainWindow::slot_view_YMinus()
{
	pipelineObj_plotWidget->setViewDirection("YMinus");
	emit sig_python_view_YMinus();
}

void graph_MainWindow::slot_view_ZMinus()
{
	pipelineObj_plotWidget->setViewDirection("ZMinus");
	emit sig_python_view_ZMinus();
}

bool  graph_MainWindow::script_openFile(QString args)
{
	QStringList filelist;
	if (!args.contains("*"))
	{
		filelist.append(args);
	}
	else
	{
		const int np = args.lastIndexOf("/");
		const int nd = args.lastIndexOf(".");
		const int ns = args.indexOf("*");
		QString name = args.left(ns).remove(args.left(np + 1));
		//char tep_val = name.right(1).toLocal8Bit().data()[0];
		while ((name.right(1) >= "0") && (name.right(1) <= "9"))
			name = name.remove(name.length()-1,1);
		QString path = args.left(np);
		QString suffix = args.right(args.size() - nd);
		QStringList fillter; fillter << "*" + suffix;

		QDir dir(path);
		dir.setNameFilters(fillter);
		QList<QFileInfo> info = dir.entryInfoList(fillter);
		const int n = info.size();
		for (int i = 0; i < n; ++i)
		{
			QString m = info.at(i).fileName();
			if (m.startsWith(name))
			{
				QString tp = info.at(i).filePath();
				filelist.append(tp);
			}

		}
		if (filelist.count() > 0)
		{
			QFile tep_file(filelist.at(0));
			if (!tep_file.exists())
				return false;
		}
		else
			return false;
	}

	if (slot_LoadDataSource(filelist.at(0), filelist, 0))
	{
		//script_dataObj = cur_PipelineObj;
		return true;
	}
	else
		return false;
}

void graph_MainWindow::script_updatePipelineObjeDataSet(PipelineObject* tep_pipeObj, QString tep_filename)
{
	g_ani_objList = mProjectDB.GetPipelineObjects();
	pipelineObj_plotWidget->updatePipelineObjeDataSet(tep_pipeObj, tep_filename);
}

void graph_MainWindow::script_applyClicked()
{
	pipelineObj_plotWidget->slot_refreshView(mProjectDB.GetPipelineObjects());
	QThread::msleep(100);
}
vtkDataSet* graph_MainWindow::script_get_pipelineObj_dataSet(PipelineObject* tep_piplnObj)
{
	return tep_piplnObj->get_pipelineObj_dataSet();
}
//void MainWindow::script_FilterClip(PipelineObject* tep_piplnObj, sliceFilter_pipelineObj_propData tep_plane_propData, bool flag_defaultPlaneProp)
void graph_MainWindow::script_FilterClip(PipelineObject* tep_piplnObj)
{
	if (tep_piplnObj == NULL)
		return;
	if (tep_piplnObj->GetObjectType() != dNumeric_DataSource)
		return;
	do_clipFilter(tep_piplnObj);
	//if (!flag_defaultPlaneProp)
	//	cur_PipelineObj->mPipeLineObjProp.plane_propData = tep_plane_propData;
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors = false;
}

//void MainWindow::script_FilterSlice(PipelineObject* tep_piplnObj, sliceFilter_pipelineObj_propData tep_plane_propData, bool flag_defaultPlaneProp)
void graph_MainWindow::script_FilterSlice(PipelineObject* tep_piplnObj)
{
	if (tep_piplnObj == NULL)
		return;
	if (tep_piplnObj->GetObjectType() != dNumeric_DataSource)
		return;
	do_sliceFilter(tep_piplnObj);
	//if (!flag_defaultPlaneProp)
	//	cur_PipelineObj->mPipeLineObjProp.plane_propData = tep_plane_propData;
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors = false;
}

//void MainWindow::script_FilterContour(PipelineObject* tep_piplnObj, contourFilter_pipelineObj_propData tep_contour_propData, bool flag_defaultContourProp)
void graph_MainWindow::script_FilterContour(PipelineObject* tep_piplnObj)
{
	if (tep_piplnObj == NULL)
		return;
	if (tep_piplnObj->GetObjectType() != dNumeric_DataSource)
		return;
	do_contourFilter(tep_piplnObj);
	//if (!flag_defaultContourProp)
	//	cur_PipelineObj->mPipeLineObjProp.contourFilter_propData= tep_contour_propData;
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors = false;
}

//void MainWindow::script_FilterVector(PipelineObject* tep_piplnObj, vectorFilter_pipelineObj_propData tep_vector_propData, bool flag_defaultVectorProp)
void graph_MainWindow::script_FilterVector(PipelineObject* tep_piplnObj)
{
	if (tep_piplnObj == NULL)
		return;
	if (tep_piplnObj->GetObjectType() != dNumeric_DataSource)
		return;
	do_glyphFilter(tep_piplnObj);
	//if (!flag_defaultVectorProp)
	//	cur_PipelineObj->mPipeLineObjProp.vectorFilter_propData = tep_vector_propData;
}

void graph_MainWindow::script_FilterStreamLine(PipelineObject* tep_piplnObj)
{
	if (tep_piplnObj == NULL)
		return;
	if (tep_piplnObj->GetObjectType() != dNumeric_DataSource)
		return;
	do_streamLineFilter(tep_piplnObj);
	//if (!flag_defaultVectorProp)
	//	cur_PipelineObj->mPipeLineObjProp.vectorFilter_propData = tep_vector_propData;
}

void graph_MainWindow::script_FilterReflection(PipelineObject* tep_piplnObj)
{
	if (tep_piplnObj == NULL)
		return;
	if (tep_piplnObj->GetObjectType() != dNumeric_DataSource)
		return;
	do_reflectionFilter(tep_piplnObj);
}

void graph_MainWindow::script_FilterCalculator(PipelineObject* tep_piplnObj)
{
	if (tep_piplnObj == NULL)
		return;
	if (tep_piplnObj->GetObjectType() != dNumeric_DataSource)
		return;
	do_calculatorFilter(tep_piplnObj);
}

void graph_MainWindow::script_FilterSmooth(PipelineObject* tep_piplnObj)
{
	if (tep_piplnObj == NULL)
		return;
	if (tep_piplnObj->GetObjectType() != dNumeric_DataSource)
		return;
	do_smoothFilter(tep_piplnObj);
}

void graph_MainWindow::script_Properties_Opacity(PipelineObject* tep_piplnObj, double val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mOpacity = val;
	if (!get_pipelineObj_plotWidget()->getScriptRunning())//flag_script_running
		pipelineObj_plotWidget->slot_update_opacity(tep_piplnObj);
}

void graph_MainWindow::script_Properties_colorColumn(PipelineObject* tep_piplnObj, QString args)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle = args;
	if (!get_pipelineObj_plotWidget()->getScriptRunning())
		pipelineObj_plotWidget->slot_update_color(tep_piplnObj);
	else if ((tep_piplnObj->GetObjectType()==dVector_DataSource)&&(args=="GlyphVector"))
		tep_piplnObj->func_initGlyphVectorArr(tep_piplnObj->mBaseGrid);
}
void graph_MainWindow::script_Properties_colorColumn_component(PipelineObject* tep_piplnObj, QString args)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle_XYZ = args;
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.pre_componentXYZ = tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle_XYZ
		+ "_" + tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle;
	if (!get_pipelineObj_plotWidget()->getScriptRunning())
		pipelineObj_plotWidget->slot_update_color(tep_piplnObj);
}
void graph_MainWindow::script_Properties_lineWidth(PipelineObject* tep_piplnObj, int val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mLineWidth = val;
	if (!get_pipelineObj_plotWidget()->getScriptRunning())
		pipelineObj_plotWidget->slot_update_lineWidth(tep_piplnObj);
}

void graph_MainWindow::script_Properties_pointSize(PipelineObject* tep_piplnObj, int val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mPointSize = val;
	if (!get_pipelineObj_plotWidget()->getScriptRunning())
		pipelineObj_plotWidget->slot_update_pointSize(tep_piplnObj);
}

void graph_MainWindow::script_Properties_translate(PipelineObject* tep_piplnObj, double val[3])
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_transforming.x = val[0];
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_transforming.y = val[1];
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_transforming.z = val[2];
	if (!get_pipelineObj_plotWidget()->getScriptRunning())
		pipelineObj_plotWidget->slot_update_translate(tep_piplnObj);
}

void graph_MainWindow::script_Properties_origin(PipelineObject* tep_piplnObj, double val[3])
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_origin.x = val[0];
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_origin.y = val[1];
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_origin.z = val[2];
	if (!get_pipelineObj_plotWidget()->getScriptRunning())
		pipelineObj_plotWidget->slot_update_origin(tep_piplnObj);
}

void graph_MainWindow::script_Properties_scale(PipelineObject* tep_piplnObj, double val[3])
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_scale.x = val[0];
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_scale.y = val[1];
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_scale.z = val[2];
	if (!get_pipelineObj_plotWidget()->getScriptRunning())
		pipelineObj_plotWidget->slot_update_scale(tep_piplnObj);
}

void graph_MainWindow::script_Properties_orientation(PipelineObject* tep_piplnObj, double val[3])
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_orientation.x = val[0];
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_orientation.y = val[1];
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_orientation.z = val[2];
	if (!get_pipelineObj_plotWidget()->getScriptRunning())
		pipelineObj_plotWidget->slot_update_orientation(tep_piplnObj);
}

void graph_MainWindow::script_Properties_representation(PipelineObject* tep_piplnObj, ERepresentationType val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_enum_representationtype = val;
	if (!get_pipelineObj_plotWidget()->getScriptRunning())
		pipelineObj_plotWidget->slot_update_representation(tep_piplnObj);
}

void graph_MainWindow::script_Properties_specular(PipelineObject* tep_piplnObj, double val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mSpecular = val;
	if (!get_pipelineObj_plotWidget()->getScriptRunning())
		pipelineObj_plotWidget->slot_update_specular(tep_piplnObj);
}

void graph_MainWindow::script_Properties_diffuse(PipelineObject* tep_piplnObj, double val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mDiffuse = val;
	if (!get_pipelineObj_plotWidget()->getScriptRunning())
		pipelineObj_plotWidget->slot_update_diffuse(tep_piplnObj);
}

void graph_MainWindow::script_Properties_ambient(PipelineObject* tep_piplnObj, double val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mAmbient = val;
	if (!get_pipelineObj_plotWidget()->getScriptRunning())
		pipelineObj_plotWidget->slot_update_ambient(tep_piplnObj);
}

void graph_MainWindow::script_Properties_specularPower(PipelineObject* tep_piplnObj, int val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mSpecularPower = val;
	if (!get_pipelineObj_plotWidget()->getScriptRunning())
		pipelineObj_plotWidget->slot_update_specularPower(tep_piplnObj);
}

void graph_MainWindow::script_Properties_specularColor(PipelineObject* tep_piplnObj, int val[3])
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mSpecularColor.r = val[0];
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mSpecularColor.g = val[1];
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mSpecularColor.b = val[2];
	if (!get_pipelineObj_plotWidget()->getScriptRunning())
		pipelineObj_plotWidget->slot_update_specularColor(tep_piplnObj);
}

void graph_MainWindow::script_Properties_solidColor(PipelineObject* tep_piplnObj, int val[3])
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mSolidColor.r = val[0];
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mSolidColor.g = val[1];
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mSolidColor.b = val[2];
	if (!get_pipelineObj_plotWidget()->getScriptRunning())
		pipelineObj_plotWidget->slot_update_solidColor(tep_piplnObj);
}

void graph_MainWindow::script_Properties_edgeColor(PipelineObject* tep_piplnObj, int val[3])
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mEdgeColor.r = val[0];
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mEdgeColor.g = val[1];
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mEdgeColor.b = val[2];
	if (!get_pipelineObj_plotWidget()->getScriptRunning())
		pipelineObj_plotWidget->slot_update_edgeColor(tep_piplnObj);
}

void graph_MainWindow::script_Properties_interpolation(PipelineObject* tep_piplnObj, EInterpolationType val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_enum_interpolationtype = val;
	if (!get_pipelineObj_plotWidget()->getScriptRunning())
		pipelineObj_plotWidget->slot_update_interpolation(tep_piplnObj);
}

void graph_MainWindow::script_Properties_Flag_scalarBar(PipelineObject* tep_piplnObj, bool val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = val;
	PipelineObject *tep_obj;
	QString tep_colName;
	if (tep_piplnObj->GetObjectType() == dNumeric_DataSource)
	{
		tep_obj = tep_piplnObj;
		//tep_colName = tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle;
	}
	else if ((tep_piplnObj->GetObjectType() == dClip_DataSource) || (tep_piplnObj->GetObjectType() == dSlice_DataSource)
		|| (tep_piplnObj->GetObjectType() == dContour_DataSource) || (tep_piplnObj->GetObjectType() == dVector_DataSource)
		|| (tep_piplnObj->GetObjectType() == dReflection_DataSource) || (tep_piplnObj->GetObjectType() == dSmooth_DataSource)
		|| (tep_piplnObj->GetObjectType() == dCalculator_DataSource) || (tep_piplnObj->GetObjectType() == dStreamLine_DataSource))
	{
		tep_obj = tep_piplnObj->GetObjParent();
	}
	else
		return;
	tep_colName = tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle;
	if ((tep_piplnObj->GetObjectType() == dCalculator_DataSource)
		&& (tep_colName == tep_piplnObj->mPipeLineObjProp.calculatorFilter_propData.resultName))
	{
		if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.contains("CalculatorResult"))
		{
			tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert("CalculatorResult", val);
			pipelineObj_plotWidget->generate_scalarBarWidget_calculatorResult((NumericSrcObject*)tep_obj, tep_piplnObj->mPipeLineObjProp.calculatorFilter_propData.resultName);
		}
	}
	else
	{
		if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.contains(tep_colName))
		{
			tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert(tep_colName, val);
		}
	}
	
	/* if (val)
	 {
		 QMap<QString, bool>::iterator it;
		 for (it = tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.begin(); it != tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.end(); ++it)
		 {
			 if (it.value() && (it.key() != tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle))
			 {
				 tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert(it.key(),false);
			 }
		 }
	 }
	if (tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.contains(tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle))
	{
		tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert(tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle, val);
	}*/
}
void graph_MainWindow::script_Properties_EnableOpacityMap(PipelineObject* tep_piplnObj, bool val)
{
	if (tep_piplnObj == NULL)
		return;
	PipelineObject* tep_obj = NULL;
	if (tep_piplnObj->GetObjectType() == dNumeric_DataSource)
		tep_obj = tep_piplnObj;
	else
		tep_obj = tep_piplnObj->GetObjParent();
	QString tep_colName = tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle;
	if ((tep_piplnObj->GetObjectType() == dCalculator_DataSource) 
		&& (tep_colName==tep_piplnObj->mPipeLineObjProp.calculatorFilter_propData.resultName))
	{
		if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap.contains("CalculatorResult"))
			tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap.insert("CalculatorResult", val);
	}
	else
	{
		if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap.contains(tep_colName))
			tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap.insert(tep_colName, val);
	}
}
void graph_MainWindow::script_Properties_LookupTableMap(PipelineObject* tep_piplnObj, QString val)
{
	tep_piplnObj->func_ReInitLookupTable(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle, val);
}
void graph_MainWindow::script_Properties_LookupOpacityMap(PipelineObject* tep_piplnObj, QString val)
{
	tep_piplnObj->func_ReInitLookupOpacity(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle, val);
}
void graph_MainWindow::script_Properties_visible(PipelineObject* tep_piplnObj, bool val)
{
	if (tep_piplnObj == NULL)
		return;
	bool flag_change = false;
	if (tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors != val)
	{
		flag_change = true;
		tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors = val;
	}
	if (!get_pipelineObj_plotWidget()->getScriptRunning())
		pipelineObj_plotWidget->slot_update_actor_visible_plotForm(tep_piplnObj, flag_change);

}

void graph_MainWindow::script_Properties_show_scalarBars(PipelineObject* tep_piplnObj)
{
	if (tep_piplnObj == NULL)
		return;
	if (tep_piplnObj != NULL)
		pipelineObj_plotWidget->slot_update_scalarBar(tep_piplnObj);
}
void graph_MainWindow::script_Properties_show_cubeAxes(PipelineObject* tep_piplnObj)
{
	if (tep_piplnObj == NULL)
		return;
	if (tep_piplnObj != NULL)
		pipelineObj_plotWidget->slot_update_cubeAxes(tep_piplnObj);
}
void graph_MainWindow::script_Properties_DataSource_DisplayGridOrZone(PipelineObject* tep_piplnObj, int val)
{
	tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.flag_grid_blocks = val;
}

void graph_MainWindow::script_Properties_dataSourceZone(PipelineObject* tep_piplnObj, QString val)
{
	int tep_count = 0;
	QStringList tep_list = val.split(",");
	QMap<QString, bool>::iterator it;
	for (it = tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.block_showMap.begin(); it!= tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.block_showMap.end(); it++)
	{
		tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.block_showMap.insert(it.key(), false);
	}
	for (int i = 0; i < tep_list.count(); i++)
	{
		if (tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.block_showMap.contains(tep_list.at(i)))
		{
			tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.block_showMap.insert(tep_list.at(i), true);
			tep_count++;
		}
	}
	if (tep_count > 0)
	{
		tep_piplnObj->flag_update_mBaseGrid = true;
	}
}
vtkFloatArray* graph_MainWindow::script_Properties_getColumnData(PipelineObject* tep_piplnObj, QString col)
{
	vtkFloatArray* arr = NULL;
	arr = tep_piplnObj->getColumnData(col);
	return arr;
}
void graph_MainWindow::script_Properties_planeOrigin(PipelineObject* tep_piplnObj, double val[3])
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.plane_propData.m_plane_origin.x = val[0];
	tep_piplnObj->mPipeLineObjProp.plane_propData.m_plane_origin.y = val[1];
	tep_piplnObj->mPipeLineObjProp.plane_propData.m_plane_origin.z = val[2];
	if (!get_pipelineObj_plotWidget()->getScriptRunning())
		pipelineObj_plotWidget->slot_update_planeOrigin(tep_piplnObj);
}

void graph_MainWindow::script_Properties_planeNormal(PipelineObject* tep_piplnObj, double val[3])
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.plane_propData.m_plane_normal.x = val[0];
	tep_piplnObj->mPipeLineObjProp.plane_propData.m_plane_normal.y = val[1];
	tep_piplnObj->mPipeLineObjProp.plane_propData.m_plane_normal.z = val[2];
	if (!get_pipelineObj_plotWidget()->getScriptRunning())
		pipelineObj_plotWidget->slot_update_planeNormal(tep_piplnObj);
}

void graph_MainWindow::script_Properties_planeVisible(PipelineObject* tep_piplnObj, bool val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.plane_propData.flag_plane = val;
	if (!get_pipelineObj_plotWidget()->getScriptRunning())
		pipelineObj_plotWidget->slot_update_planeVis(tep_piplnObj);
}
void graph_MainWindow::script_Properties_insideOut(PipelineObject* tep_piplnObj, bool val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.plane_propData.flag_insideOut = val;
	
}
void graph_MainWindow::script_Properties_vector_scalar(PipelineObject* tep_piplnObj, QString val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.vectorFilter_propData.scalarColName = val;
}
void graph_MainWindow::script_Properties_vector_GlyphVector(PipelineObject* tep_piplnObj, QString val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.vectorFilter_propData.glyphVector_colorXYZ= val;
}
void graph_MainWindow::script_Properties_vector_normal(PipelineObject* tep_piplnObj, QString val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.vectorFilter_propData.vectorColName = val;
}

void graph_MainWindow::script_Properties_vector_numPoints(PipelineObject* tep_piplnObj, int val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.vectorFilter_propData.numPoints = val;
}

void graph_MainWindow::script_Properties_vector_glyph_type(PipelineObject* tep_piplnObj, int val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.vectorFilter_propData.glyph_data.type = val;
}

void graph_MainWindow::script_Properties_vector_glyph_tipRes(PipelineObject* tep_piplnObj, int val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.vectorFilter_propData.glyph_data.arrow_data.tipRes = val;
}

void graph_MainWindow::script_Properties_vector_glyph_tipRad(PipelineObject* tep_piplnObj, double val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.vectorFilter_propData.glyph_data.arrow_data.tipRad = val;
}

void graph_MainWindow::script_Properties_vector_glyph_tipLen(PipelineObject* tep_piplnObj, double val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.vectorFilter_propData.glyph_data.arrow_data.tipLen = val;
}

void graph_MainWindow::script_Properties_vector_glyph_shaftRes(PipelineObject* tep_piplnObj, int val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.vectorFilter_propData.glyph_data.arrow_data.shaftRes = val;
}

void graph_MainWindow::script_Properties_vector_glyph_shaftRad(PipelineObject* tep_piplnObj, double val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.vectorFilter_propData.glyph_data.arrow_data.shaftRad = val;
}

///////////////////////////
void graph_MainWindow::script_Properties_streamline_vector(PipelineObject* tep_piplnObj, QString val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.streamLineFilter_propData.vectorColName = val;
}

void graph_MainWindow::script_Properties_streamline_integration_direction(PipelineObject* tep_piplnObj, IntegrationDirection val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.streamLineFilter_propData.integration_data.direction = val;
}

void graph_MainWindow::script_Properties_streamline_integration_type(PipelineObject* tep_piplnObj, IntegratorType val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.streamLineFilter_propData.integration_data.type = val;
}

void graph_MainWindow::script_Properties_streamline_integration_stepUnit(PipelineObject* tep_piplnObj, IntegrationStepUnit val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.streamLineFilter_propData.integration_data.stepUnit = val;
}

void graph_MainWindow::script_Properties_streamline_integration_initStepLen(PipelineObject* tep_piplnObj, double val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.streamLineFilter_propData.integration_data.initStepLen = val;
}

void graph_MainWindow::script_Properties_streamline_integration_miniStepLen(PipelineObject* tep_piplnObj, double val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.streamLineFilter_propData.integration_data.miniStepLen = val;
}
void graph_MainWindow::script_Properties_streamline_integration_maxiStepLen(PipelineObject* tep_piplnObj, double val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.streamLineFilter_propData.integration_data.maxiStepLen = val;
}

void graph_MainWindow::script_Properties_streamline_stream_maxiSteps(PipelineObject* tep_piplnObj, int val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.streamLineFilter_propData.stream_data.maxiSteps = val;
}

void graph_MainWindow::script_Properties_streamline_stream_maxiStreamLen(PipelineObject* tep_piplnObj, double val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.streamLineFilter_propData.stream_data.maxiStreamLen = val;
}

void graph_MainWindow::script_Properties_streamline_stream_terminalSpeed(PipelineObject* tep_piplnObj, double val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.streamLineFilter_propData.stream_data.terminalSpeed = val;
}

void graph_MainWindow::script_Properties_streamline_stream_maxiError(PipelineObject* tep_piplnObj, double val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.streamLineFilter_propData.stream_data.maxiError = val;
}

void graph_MainWindow::script_Properties_streamline_seeds_type(PipelineObject* tep_piplnObj, SpeedType val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.vectorFilter_propData.glyph_data.arrow_data.shaftRes = val;
}

void graph_MainWindow::script_Properties_streamline_seeds_mPoint(PipelineObject* tep_piplnObj, double val[3])
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.streamLineFilter_propData.seeds_data.mPoint.x = val[0];
	tep_piplnObj->mPipeLineObjProp.streamLineFilter_propData.seeds_data.mPoint.y = val[1];
	tep_piplnObj->mPipeLineObjProp.streamLineFilter_propData.seeds_data.mPoint.z = val[2];
}

void graph_MainWindow::script_Properties_streamline_seeds_num_points(PipelineObject* tep_piplnObj, int val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.streamLineFilter_propData.seeds_data.num_points = val;
}

void graph_MainWindow::script_Properties_streamline_seeds_radius(PipelineObject* tep_piplnObj, double val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.streamLineFilter_propData.seeds_data.radius = val;
}

void graph_MainWindow::script_Properties_streamline_vorticity(PipelineObject* tep_piplnObj, bool val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.streamLineFilter_propData.vorticity = val;
}

void graph_MainWindow::script_Properties_streamline_interpolatorType(PipelineObject* tep_piplnObj, InterpolatorType val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.streamLineFilter_propData.interpolator_type = val;
}

void graph_MainWindow::script_Properties_streamline_surface_streamLines(PipelineObject* tep_piplnObj, bool val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.streamLineFilter_propData.surface_streamLines = val;
}

///////////////////////////////
void graph_MainWindow::script_Properties_contourColumn(PipelineObject* tep_piplnObj, QString val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.contourFilter_propData.contour_colName = val;
}

void graph_MainWindow::script_Properties_contourValue(PipelineObject* tep_piplnObj, double val)
{
	if (tep_piplnObj == NULL)
		return;
	contour_PropData tep_data;
	for (int i = 0; i < tep_piplnObj->mPipeLineObjProp.contourFilter_propData.contour_dataList.count(); i++)
	{
		if (tep_piplnObj->mPipeLineObjProp.contourFilter_propData.contour_dataList.at(i).colName == tep_piplnObj->mPipeLineObjProp.contourFilter_propData.contour_colName)
		{	
			tep_data = tep_piplnObj->mPipeLineObjProp.contourFilter_propData.contour_dataList.at(i);
			tep_data.val = val;
			tep_piplnObj->mPipeLineObjProp.contourFilter_propData.contour_dataList.replace(i, tep_data);
			break;
		}
	}
	
}
void graph_MainWindow::script_Properties_contour_reflection(PipelineObject* tep_piplnObj, bool val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->m_reflectionPara.flag_refleciton = val;
}
void graph_MainWindow::script_Properties_contour_reflectionAxes(PipelineObject* tep_piplnObj, ReflectionPlane val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->m_reflectionPara.type = val;
}
void graph_MainWindow::script_Properties_reflectionAxes(PipelineObject* tep_piplnObj, ReflectionPlane val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.reflectionFilter_propData.reflection_axis = val;
}
void graph_MainWindow::script_Properties_calculatorFormula(PipelineObject* tep_piplnObj, QString val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.calculatorFilter_propData.formula = val;
}
void graph_MainWindow::script_Properties_calculatorResultName(PipelineObject* tep_piplnObj, QString val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.calculatorFilter_propData.resultName = val;
	((FilterCalculator*)tep_piplnObj)->setCalculator();
	tep_piplnObj->mPipeLineObjProp.calculatorFilter_propData.pre_formula = tep_piplnObj->mPipeLineObjProp.calculatorFilter_propData.formula;
}
void graph_MainWindow::script_Properties_reflectionCopyInput(PipelineObject* tep_piplnObj, bool val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.reflectionFilter_propData.flag_copyInput = val;
}
void graph_MainWindow::script_Properties_smooth(PipelineObject* tep_piplnObj, int type, double coef)//0cellToPoint,1mesh
{
	if (tep_piplnObj == NULL)
		return;
	if (type == 0)
	{
		tep_piplnObj->mPipeLineObjProp.smoothFilter_propData.flag_cellToPoint = true;
		tep_piplnObj->mPipeLineObjProp.smoothFilter_propData.flag_meshSmooth = false;
	}
	else
	{
		tep_piplnObj->mPipeLineObjProp.smoothFilter_propData.flag_cellToPoint = false;
		tep_piplnObj->mPipeLineObjProp.smoothFilter_propData.flag_meshSmooth = true;
		tep_piplnObj->mPipeLineObjProp.smoothFilter_propData.smooth_coef = coef;
	}
}
//properties aniDisplace
void graph_MainWindow::script_Properties_aniDisplace_fileName(PipelineObject* tep_piplnObj, QString val)
{
	if (tep_piplnObj == NULL)
		return;
	QFile tep_file(val);
	if (tep_file.exists())
	{
		tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.flag_AniDisplaceFile = true;
		tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.path_AniDisplaceFile = val;
		//////////////////////////////
		

		tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.clear();
		LoadActionFile* ptr = new LoadActionFile;
		QFileInfo tep_info(val);
		if (tep_info.suffix() == "dis")
		{
			if (ptr->Load(val, "Sway_GY"))
			{
				tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList = ptr->displaceList;
				tep_piplnObj->func_init_displacement();
				tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.cur_pos = 0;
				tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.start_pos = 0;
				tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.interval = 1;
				tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.dis_xh = 0;
				tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.zoom_factor = 1.0;
				tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.flag_inputGravity = false;
				//tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.m_origin.x = 0;
				//tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.m_origin.y = 0;
				//tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.m_origin.z = 0;
			}
			else
			{
				delete ptr;
				return;
			}
		}
		else if (tep_info.suffix() == "csv")
		{
			if (ptr->LoadCSV(val))
			{
				tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList = ptr->displaceList;
				tep_piplnObj->func_init_displacement();
				tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.cur_pos = 0;
				tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.start_pos = 0;
				tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.interval = 1;
				tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.dis_xh = 0;
				tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.zoom_factor = 1.0;
				tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.flag_inputGravity = false;
				tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.m_origin.x = 0;
				tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.m_origin.y = 0;
				tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.m_origin.z = 0;
			}
			else
			{
				delete ptr;
				return;
			}
		}
		delete ptr;
		tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.time = tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(0).time;
		tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Roll_Gxb = tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(0).Roll_Gxb;
		tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Pitch_Gyb = tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(0).Pitch_Gyb;
		tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Yaw_Gzb = tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(0).Yaw_Gzb;
		tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Surge_GX = tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(0).Surge_GX;
		tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Sway_GY = tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(0).Sway_GY;
		tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Heave_GZ = tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(0).Heave_GZ;
		tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.flag_exeDisplace = true;
	}
	else
	{
		tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.flag_AniDisplaceFile = false;
		tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.path_AniDisplaceFile = "";
	}
}
void graph_MainWindow::script_Properties_aniDisplace_startPosition(PipelineObject* tep_piplnObj, int val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.start_pos = val;
}
void graph_MainWindow::script_Properties_aniDisplace_interval(PipelineObject* tep_piplnObj, int val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.interval = val;
}
void graph_MainWindow::script_Properties_aniDisplace_zoomFactor(PipelineObject* tep_piplnObj, double val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.zoom_factor = val;
}
void graph_MainWindow::script_Properties_aniDisplace_gravityCenter(PipelineObject* tep_piplnObj, double val[3])
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.m_origin.x= val[0];
	tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.m_origin.y = val[1];
	tep_piplnObj->mPipeLineObjProp.aniDisplace_propData.m_origin.z = val[2];
}

//properties aniDisplace
//mooringLine properties
void graph_MainWindow::script_Properties_mooringLine_type(PipelineObject* tep_piplnObj, int val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.mooringLine_propData.glyph_data.type=val;
}

void graph_MainWindow::script_Properties_mooringLine_tipRes(PipelineObject* tep_piplnObj, int val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.mooringLine_propData.glyph_data.arrow_data.tipRes = val;
}

void graph_MainWindow::script_Properties_mooringLine_tipRad(PipelineObject* tep_piplnObj, double val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.mooringLine_propData.glyph_data.arrow_data.tipRad = val;
}

void graph_MainWindow::script_Properties_mooringLine_tipLen(PipelineObject* tep_piplnObj, double val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.mooringLine_propData.glyph_data.arrow_data.tipLen = val;
}

void graph_MainWindow::script_Properties_mooringLine_shaftRes(PipelineObject* tep_piplnObj, int val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.mooringLine_propData.glyph_data.arrow_data.shaftRes = val;
}

void graph_MainWindow::script_Properties_mooringLine_shaftRad(PipelineObject* tep_piplnObj, double val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.mooringLine_propData.glyph_data.arrow_data.shaftRad = val;
}
void graph_MainWindow::script_Properties_mooringLine_displaySeaBottom(PipelineObject* tep_piplnObj, bool val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.mooringLine_propData.flag_seaBottom = val;
}
void graph_MainWindow::script_Properties_mooringLine_colorSeaBottom(PipelineObject* tep_piplnObj, int val[3])
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->mPipeLineObjProp.mooringLine_propData.seaBottomColor.r = val[0];
	tep_piplnObj->mPipeLineObjProp.mooringLine_propData.seaBottomColor.g = val[1];
	tep_piplnObj->mPipeLineObjProp.mooringLine_propData.seaBottomColor.b = val[2];
}
void graph_MainWindow::script_showPlotOnly()
{
	fileToolBar->setVisible(false);
	filterToolBar->setVisible(false);
	AniToolBar->setVisible(false);
	VRToolBar->setVisible(false);
	exitToolBar->setVisible(false);

	menuBar()->hide();
	statusBar()->hide();

	pipelineObj_treeWidget->hide();
	pipelineObj_propWidget->hide();
	console_cmdWidget->hide();
	
}

Camera_Para graph_MainWindow::script_getCameraPara()
{
	return pipelineObj_plotWidget->getCameraPara();
}

void graph_MainWindow::script_Camera_Position(double pos[3])
{
	pipelineObj_plotWidget->setCameraPara_Position(pos);	
}

void graph_MainWindow::script_Camera_FocalPoint(double focalPoint[3])
{
	pipelineObj_plotWidget->setCameraPara_FocalPoint(focalPoint);
}

void graph_MainWindow::script_Camera_ClippingRange(double clippingRange[2])
{
	pipelineObj_plotWidget->setCameraPara_ClippingRange(clippingRange);
}

void graph_MainWindow::script_Camera_ViewUp(double viewup[3])
{
	pipelineObj_plotWidget->setCameraPara_ViewUp(viewup);
}

void graph_MainWindow::script_Camera_ViewAngle(double angle)
{
	pipelineObj_plotWidget->setCameraPara_ViewAngle(angle);
}
void graph_MainWindow::script_Camera_ParalellScale(double scale)
{
	pipelineObj_plotWidget->setCameraPara_ParalellScale(scale);
}

void graph_MainWindow::script_Camera_Zoom(double zoom)
{
	pipelineObj_plotWidget->setCameraPara_Zoom(zoom);
}
void graph_MainWindow::script_Camera_Reset()
{
	pipelineObj_plotWidget->setCameraPara_Reset();
}
void graph_MainWindow::script_Camera_SelfAdaptation(bool flag)
{
	pipelineObj_plotWidget->setCameraPara_SelfAdaptation(flag);
}
//properties view
void graph_MainWindow::script_Properties_view_backgroundType(int val)
{
	pipelineObj_propWidget->get_propWidget()->set_renderView_prop_backgroundType(val);
}

void graph_MainWindow::script_Properties_view_backgroundColor(QColor color_val)
{
	pipelineObj_propWidget->get_propWidget()->set_renderView_prop_backgroundColor(color_val);
}

void graph_MainWindow::script_Properties_view_background2Color(QColor color_val)
{
	pipelineObj_propWidget->get_propWidget()->set_renderView_prop_background2Color(color_val);
}

void graph_MainWindow::script_Properties_view_axesVisible(bool val)
{
	pipelineObj_propWidget->get_propWidget()->set_renderView_prop_axesVisible(val);
}
void graph_MainWindow::script_Properties_view_remarkVisible(bool val)
{
	pipelineObj_propWidget->get_propWidget()->set_renderView_prop_remarkVisible(val);
}
void graph_MainWindow::script_Properties_view_remarkFontColor(QColor color_val)
{
	pipelineObj_propWidget->get_propWidget()->set_renderView_prop_remarkFontColor(color_val);
}
void graph_MainWindow::script_Properties_view_remarkText(QString text)
{
	pipelineObj_propWidget->get_propWidget()->set_renderView_prop_remarkText(text);
}
void graph_MainWindow::script_Properties_view_remarkPosition1(double *pos)
{
	pipelineObj_propWidget->get_propWidget()->set_renderView_prop_remarkPosition1(pos);
}
void graph_MainWindow::script_Properties_view_remarkPosition2(double *pos)
{
	pipelineObj_propWidget->get_propWidget()->set_renderView_prop_remarkPosition2(pos);
}
void graph_MainWindow::script_Properties_view_cameraParallel(bool val)
{
	pipelineObj_propWidget->get_propWidget()->set_renderView_prop_cameraParallel(val);
	pipelineObj_plotWidget->slot_update_camera_parallel(val);
}

void graph_MainWindow::script_Properties_view_interaction(bool val)
{
	pipelineObj_treeWidget->set_viewInteraction(val);
}

void graph_MainWindow::script_Properties_renderView()
{
	pipelineObj_propWidget->get_propWidget()->script_update_global_view_prop();
	pipelineObj_plotWidget->slot_change_global_view_prop();
}

void graph_MainWindow::script_Save_Animate(QString aviFileName)
{
	aniSetting_data.flag_save = true;
	aniSetting_data.frame_num = aniSetting_data.total_frame_num;
	aniSetting_data.avi_filename = aviFileName;
}

QList<PipelineObject*> graph_MainWindow::script_get_pipelineObject_list()
{
	return mProjectDB.GetPipelineObjects();
}

QToolBar* graph_MainWindow::script_get_filterToolBar()
{
	return filterToolBar;
}

QToolBar* graph_MainWindow::script_get_AniToolBar()
{
	return AniToolBar;
}

QToolBar* graph_MainWindow::script_get_viewDirectionToolBar()
{
	return viewDirectionToolBar;
}

QToolBar* graph_MainWindow::script_get_actorToolBar()
{
	return actorToolBar;
}

QToolBar* graph_MainWindow::script_get_savePngAviToolBar()
{
	return save_pngAviToolBar;
}

void graph_MainWindow::script_delPipelineObj(PipelineObject* pipeObj)
{
	if (pipeObj == NULL)
		return;
	if (pipeObj->mPipeLineObjProp.pipelineObj_base_propData.count_total_child==0)
		slot_delPipelineObj(pipeObj);
	else
	{
		QMessageBox box(QMessageBox::Warning, QStringLiteral("warning"), QStringLiteral("This pipeline object has children,you can not delete it !"));
		box.setStandardButtons(QMessageBox::Yes);
		box.setButtonText(QMessageBox::Yes, QStringLiteral("ok"));
		box.exec();
		return;
	}
}

void graph_MainWindow::script_update_pipelineObj_propForm(PipelineObject* pipeObj)
{
	if (pipeObj == NULL)
		return;
	get_pipelineObj_treeWidget()->DoSelectPipelineObject(pipeObj);
	get_pipelineObj_plotWidget()->slot_update_actor_visible_plotForm(pipeObj,true);
	slot_update_filterToolBar(true, pipeObj);
}

void graph_MainWindow::script_projectTreeItemClicked(PipelineObject* tep_pipelineObj)
{
	pipelineObj_treeWidget->script_projectTreeItemClicked(tep_pipelineObj);
}

void graph_MainWindow::script_saveImage(bool flag_dlg, int wid, int hei, QString fileName)//void func_saveImage(bool flag_dlg,int wid,int hei,QString fileName);
{
	saveImage_prop tep_data;
	tep_data.image_wid = wid;
	tep_data.image_hei = hei;
	tep_data.flag_saveImage = true;
	tep_data.saveImage_fileName = fileName;
	tep_data.flag_savePov = false;
	tep_data.savePov_fileName = "";
	get_pipelineObj_plotWidget()->set_saveImage_prop_data(tep_data);

	emit sig_script_saveImage();

	//func_saveImage(flag_dlg,wid,hei,fileName);
}
void graph_MainWindow::script_exitSoftWare()
{
	if (exitSoftWareScript_para.flag_exit)
	{
		exitSoftware();
		emit sig_python_exitSoftWare();
		exitSoftWareScript_para.flag_exit = false;
	}
}
void graph_MainWindow::script_setExitSoftWare(int ms)
{
	exitSoftWareScript_para.flag_exit = true;
	exitSoftWareScript_para.ms = ms;
}
void graph_MainWindow::script_setPlotSize(int* val)
{	
	setDockSize(pipelineObj_treeWidget, val[2], val[3]);
	setDockSize(pipelineObj_propWidget, val[4], val[5]);
	setDockSize(console_cmdWidget, val[6], val[7]);	
	script_MainWinWid = val[8]; //script_MainWinWid = val[4] + val[6];
	script_MainWinHei = val[9];
	if (flag_state == 0)
	{
		setGeometry(0, 0, script_MainWinWid, script_MainWinHei);////resize(val[4] + val[6], val[9]);	
		flag_state = 1;
	}
	else
	{
		setGeometry(0, 0, script_MainWinWid+1, script_MainWinHei);////resize(val[4] + val[6], val[9]);	
		flag_state = 0;
	}
	QSettings settings("Software Inc.", "Icon Editor");
	settings.beginGroup("mainWindow");
	settings.setValue("geometry", saveGeometry());
	settings.setValue("state", saveState());
	settings.endGroup();
	QTimer::singleShot(1, this, SLOT(script_ReturnToOldSize()));
	emit sig_python_setPlotSize(val);
}
void graph_MainWindow::script_resetView()
{
	pipelineObj_plotWidget->resetView();
}

void graph_MainWindow::script_setViewDirection(QString val)
{
	pipelineObj_plotWidget->setViewDirection(val);
}
void graph_MainWindow::script_setViewValueDirection(int x1, int x2, int x3, int y1, int y2, int y3, int z1, int z2, int z3)
{
	pipelineObj_plotWidget->setViewValueDirection(x1,x2,x3,y1,y2,y3,z1,z2,z3);
}
bool graph_MainWindow::script_get_pipelineObj_visible_status(PipelineObject* tep_piplnObj)
{
	if (tep_piplnObj == NULL)
		return false;
	else
		return tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors;
}

void graph_MainWindow::slot_script_user_custom_refresh_pipelineTree()
{
	emit sig_script_user_custom_refresh_pipelineTree();
}

int graph_MainWindow::script_get_pipelineObj_ID(PipelineObject* tep_piplnObj)
{
	int tep_id = -1;
	if (tep_piplnObj != NULL)
	{
		tep_id = tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id;
	}
	return tep_id;
}

void graph_MainWindow::script_draw_PickPoint(PipelineObject * tep_pipelineObj, vtkIdType pick_Id)
{
	if ((tep_pipelineObj == NULL) || (pick_Id < 0))
		return;
	pipelineObj_plotWidget->set_pickData_point_cell(tep_pipelineObj, 0, pick_Id);
}

void graph_MainWindow::script_draw_PickCell(PipelineObject * tep_pipelineObj, vtkIdType pick_Id)
{
	if ((tep_pipelineObj == NULL) || (pick_Id < 0))
		return;
	pipelineObj_plotWidget->set_pickData_point_cell(tep_pipelineObj, 1, pick_Id);
}

void graph_MainWindow::script_draw_PickPointCurve(PipelineObject * tep_pipelineObj, vtkIdType pick_Id)
{
	if ((tep_pipelineObj == NULL) || (pick_Id < 0))
		return;
	slot_pickCurvePointAct(true);
	pipelineObj_plotWidget->set_pickData_curve(tep_pipelineObj, 0, pick_Id);
}

void graph_MainWindow::script_draw_PickCellCurve(PipelineObject * tep_pipelineObj, vtkIdType pick_Id)
{
	if ((tep_pipelineObj == NULL) || (pick_Id < 0))
		return;
	slot_pickCurveCellAct(true);
	pipelineObj_plotWidget->set_pickData_curve(tep_pipelineObj, 1, pick_Id);
}

void graph_MainWindow::script_draw_PickArea(PipelineObject * tep_pipelineObj, int flag_pickMode, vtkIdList* pick_plist, vtkIdList* pick_clist)
{
	if ((tep_pipelineObj == NULL) || (pick_plist == NULL) || (pick_clist == NULL))
		return;
	if ((flag_pickMode != 2) && (flag_pickMode != 3))
		return;
	pipelineObj_plotWidget->set_pickData_area(tep_pipelineObj,flag_pickMode,pick_plist, pick_clist);
}

void graph_MainWindow::slot_pickCellAct_status(bool status)
{
	pickCellAct->setChecked(status);
}

void graph_MainWindow::slot_pickPointAct_status(bool status)
{
	pickPointAct->setChecked(status);
}

void graph_MainWindow::slot_pickAreaCellAct_status(bool status)
{
	pickAreaCellAct->setChecked(status);
}

void graph_MainWindow::slot_pickAreaPointAct_status(bool status)
{
	pickAreaPointAct->setChecked(status);
}

void graph_MainWindow::slot_pickCurvePointAct_status(bool status)
{
	pickCurvePointAct->setChecked(status);
}

void graph_MainWindow::slot_pickCurveCellAct_status(bool status)
{
	pickCurveCellAct->setChecked(status);
}

bool graph_MainWindow::script_start_animate()
{
	if (pipelineObj_plotWidget->max_ani_frames < 2)
		return false;
	QString tep_name = runAct->data().toString().trimmed();
	if (tep_name == "aniStop")
		return false;
	else if (tep_name == "aniRun")
	{
		runAct->setIcon(QIcon(":/images/stop.png"));
		runAct->setData("aniStop");
		g_ani_objList = mProjectDB.GetPipelineObjects();
		saveAniAct->setEnabled(false);
		firstAct->setEnabled(false);
		endAct->setEnabled(false);
		nextAct->setEnabled(false);
		lastAct->setEnabled(false);

		cgnsAniTimesAct->setEnabled(false);

		cur_frame_spinBox->setEnabled(false);
		ani_fileInterval_spinBox->setEnabled(false);
		openAct->setEnabled(false);
		if (pipelineObj_plotWidget->max_ani_frames > 1)
		{
			pipelineObj_treeWidget->projectTreeItemClicked(pipelineObj_treeWidget->get_pipelineObjTree()->topLevelItem(0), 0);
			pipelineObj_plotWidget->slot_runAni();
		}
		ani_update_all_form(true);
		//pipelineObj_plotWidget->clear_pickCurveData();//20180226
		return true;
	}
	else
		return false;
}

bool graph_MainWindow::script_stop_animate()
{
	if (pipelineObj_plotWidget->max_ani_frames < 2)
		return true;
	QString tep_name = runAct->data().toString().trimmed();
	if (tep_name == "aniRun")
		return true;
	else if (tep_name == "aniStop")
	{
		pipelineObj_plotWidget->slot_stopAni();
		slot_update_pipelineObjs();
		runAct->setIcon(QIcon(":/images/run.png"));
		runAct->setData("aniRun");
		saveAniAct->setEnabled(true);
		firstAct->setEnabled(true);
		endAct->setEnabled(true);
		nextAct->setEnabled(true);
		lastAct->setEnabled(true);
		cur_frame_spinBox->setEnabled(true);
		ani_fileInterval_spinBox->setEnabled(true);
		openAct->setEnabled(true);
		ani_update_all_form(false);
		return true;
	}
	else
		return false;

}
void graph_MainWindow::script_Properties_scalarBarTitle(PipelineObject* tep_piplnObj, QString colName, QString m_title)
{
	PipelineObject* tep_obj = NULL;
	if (tep_piplnObj->GetObjectType() == dNumeric_DataSource)
		tep_obj = tep_piplnObj;
	else
		tep_obj = tep_piplnObj->GetObjParent();
	scalarBarTitle_PropData tep_titlePara;
	if ((tep_piplnObj->GetObjectType() == dCalculator_DataSource)
		&& (tep_piplnObj->mPipeLineObjProp.calculatorFilter_propData.resultName==colName))
	{
		if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.contains("CalculatorResult"))
			tep_titlePara = tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap["CalculatorResult"];
		else
		{
			tep_titlePara.size = scalarBar_FontSize;
			tep_titlePara.num_lable = scalarBar_NumLables;
		}
		tep_titlePara.title = m_title;

		tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.insert("CalculatorResult", tep_titlePara);
	}
	else
	{
		if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.contains(colName))
			tep_titlePara = tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap[colName];
		else
		{
			tep_titlePara.size = scalarBar_FontSize;
			tep_titlePara.num_lable = scalarBar_NumLables;
		}
		tep_titlePara.title = m_title;

		tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.insert(colName, tep_titlePara);
		/*if (tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle == "GlyphVector")
			tep_piplnObj->mPipeLineObjProp.vectorFilter_propData.m_preGlyphVectorPara.title = m_title;*/
	}
}
void graph_MainWindow::script_Properties_scalarBarFontSize(PipelineObject* tep_piplnObj, QString colName, int m_fontSize)
{
	PipelineObject* tep_obj = NULL;
	if (tep_piplnObj->GetObjectType() == dNumeric_DataSource)
		tep_obj = tep_piplnObj;
	else
		tep_obj = tep_piplnObj->GetObjParent();
	scalarBarTitle_PropData tep_titlePara;
	if ((tep_piplnObj->GetObjectType() == dCalculator_DataSource)
		&& (tep_piplnObj->mPipeLineObjProp.calculatorFilter_propData.resultName == colName))
	{
		if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.contains("CalculatorResult"))
			tep_titlePara = tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap["CalculatorResult"];
		else
		{
			tep_titlePara.title = colName;
			tep_titlePara.num_lable = scalarBar_NumLables;
		}
		tep_titlePara.size = m_fontSize;

		tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.insert("CalculatorResult", tep_titlePara);
	}
	else
	{
		if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.contains(colName))
			tep_titlePara = tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap[colName];
		else
		{
			tep_titlePara.title = colName;
			tep_titlePara.num_lable = scalarBar_NumLables;
		}
		tep_titlePara.size = m_fontSize;

		tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.insert(colName, tep_titlePara);
		/*if (tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle == "GlyphVector")
		tep_piplnObj->mPipeLineObjProp.vectorFilter_propData.m_preGlyphVectorPara.fontSize = m_fontSize;*/
	}
}
void graph_MainWindow::script_Properties_scalarBarNumLables(PipelineObject* tep_piplnObj, QString colName,int m_numLables)
{
	PipelineObject* tep_obj = NULL;
	if (tep_piplnObj->GetObjectType() == dNumeric_DataSource)
		tep_obj = tep_piplnObj;
	else
		tep_obj = tep_piplnObj->GetObjParent();
	scalarBarTitle_PropData tep_titlePara;
	if ((tep_piplnObj->GetObjectType() == dCalculator_DataSource)
		&& (tep_piplnObj->mPipeLineObjProp.calculatorFilter_propData.resultName == colName))
	{
		if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.contains("CalculatorResult"))
			tep_titlePara = tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap["CalculatorResult"];
		else
		{
			tep_titlePara.size = scalarBar_FontSize;
			tep_titlePara.title = colName;
		}
		tep_titlePara.num_lable = m_numLables;

		tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.insert("CalculatorResult", tep_titlePara);
		/*if (tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle == "GlyphVector")
			tep_piplnObj->mPipeLineObjProp.vectorFilter_propData.m_preGlyphVectorPara.numLables = m_numLables;*/
	}
	else
	{
		if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.contains(colName))
			tep_titlePara = tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap[colName];
		else
		{
			tep_titlePara.size = scalarBar_FontSize;
			tep_titlePara.title = colName;
		}
		tep_titlePara.num_lable = m_numLables;

		tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.insert(colName, tep_titlePara);
	}
}
void graph_MainWindow::slot_update_scalarBar_title(PipelineObject* tep_pipeObj, QString colName, QString m_title)
{
	emit sig_python_scalarBar_title(tep_pipeObj, colName, m_title);
}
void graph_MainWindow::slot_update_scalarBar_fontSize(PipelineObject* tep_pipeObj, QString colName, int m_fontSize)
{
	emit sig_python_scalarBar_fontSize(tep_pipeObj, colName, m_fontSize);
}
void graph_MainWindow::slot_update_scalarBar_numLables(PipelineObject* tep_pipeObj, QString colName, int m_numLables)
{
	emit sig_python_scalarBar_numLables(tep_pipeObj, colName, m_numLables);
}
void graph_MainWindow::slot_load_scalarBarEditForm(PipelineObject* tep_pipeObj)
{
	if (cur_PipelineObj->mVecActors.count() < 1)
		return;
	if (scalarBar_dockWidget == NULL)
	{
		scalarBar_dockWidget = new scalarBarEditDockWidget;
		scalarBar_dockWidget->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable);
		addDockWidget(Qt::RightDockWidgetArea, scalarBar_dockWidget);
		viewMenu->addAction(scalarBar_dockWidget->toggleViewAction());
		connect(scalarBar_dockWidget->get_scalarBarEditForm(), SIGNAL(sig_update_plot_pipelineObj(PipelineObject*, vtkColorTransferFunction*, vtkPiecewiseFunction*)), pipelineObj_plotWidget, SLOT(slot_update_plot_pipelineObj(PipelineObject*, vtkColorTransferFunction*, vtkPiecewiseFunction*)));
		connect(scalarBar_dockWidget->get_scalarBarEditForm(), SIGNAL(sig_volume_scalarBarEdit(PipelineObject*, vtkSmartPointer<vtkLookupTable>)), pipelineObj_plotWidget, SLOT(slot_volume_scalarBarEdit(PipelineObject*, vtkSmartPointer<vtkLookupTable>)));
		connect(scalarBar_dockWidget->get_scalarBarEditForm(), SIGNAL(sig_update_scalarBar_title(PipelineObject*, QString, QString)),this, SLOT(slot_update_scalarBar_title(PipelineObject*, QString, QString)));
		connect(scalarBar_dockWidget->get_scalarBarEditForm(), SIGNAL(sig_update_scalarBar_fontSize(PipelineObject*, QString, int)), this, SLOT(slot_update_scalarBar_fontSize(PipelineObject*, QString, int)));
		connect(scalarBar_dockWidget->get_scalarBarEditForm(), SIGNAL(sig_update_scalarBar_numLables(PipelineObject*, QString, int)), this, SLOT(slot_update_scalarBar_numLables(PipelineObject*, QString, int)));
		connect(scalarBar_dockWidget->get_scalarBarEditForm(), SIGNAL(sig_update_scalarBar(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_scalarBar(PipelineObject*)));

		emit sig_script_creatScalarBarEditWidget(scalarBar_dockWidget);
		if (pipelineObj_propWidget!=NULL)
			pipelineObj_propWidget->get_propWidget()->set_scalarBar_Form(scalarBar_dockWidget);
	}
	scalarBar_dockWidget->get_scalarBarEditForm()->updateForm(tep_pipeObj);
	if (!scalarBar_dockWidget->isVisible())
	{
		scalarBar_dockWidget->setVisible(true);
		//emit sig_script_creatScalarBarEditWidget(scalarBar_dockWidget);
	}
}

void graph_MainWindow::script_clearAllPipelineObjs()
{
	QList<PipelineObject*> objList = mProjectDB.GetPipelineObjects();
	for (int i = 0; i < objList.size(); ++i)
	{
		PipelineObject* p = objList.at(i);
		if (p == NULL)
			continue;
		if (p->mPipeLineObjProp.pipelineObj_base_propData.count_total_child == 0)
			slot_delPipelineObj(p);
	}
	if (pickCellAct->isChecked())
		slot_pickCellAct(false);
	if (pickPointAct->isChecked())
		slot_pickPointAct(false);
	if (pickAreaCellAct->isChecked())
		slot_pickAreaCellAct(false);
	if (pickAreaPointAct->isChecked())
		slot_pickAreaPointAct(false);
	if (pickCurveCellAct->isChecked())
		slot_pickCurveCellAct(false);
	if (pickCurvePointAct->isChecked())
		slot_pickCurvePointAct(false);
}
void graph_MainWindow::slot_script_clearAllPipelineObjs(int delayTime)
{
	/*if (delayTime>0)
	QThread::msleep(delayTime);
	script_clearAllPipelineObjs();*/
	QTimer *timer = new QTimer(this);
	QTimer::singleShot(delayTime, this, SLOT(script_clearAllPipelineObjs()));
	//connect(timer, SIGNAL(timeout()), this, SLOT(script_clearAllPipelineObjs()));
	timer->start(delayTime);

}
bool graph_MainWindow::script_getScalarRange(PipelineObject* tep_obj, double val[2])
{
	return tep_obj->getcolorColumnRange(tep_obj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle, val);
}

void graph_MainWindow::script_rescaleScalarRange(PipelineObject* tep_obj,double min, double max)
{
	if ((scalarBar_dockWidget == NULL) || (tep_obj==NULL))
		return;
	double val[2];
	val[0] = min; val[1] = max;
	scalarBar_dockWidget->get_scalarBarEditForm()->updateForm(tep_obj);
	scalarBar_dockWidget->get_scalarBarEditForm()->slot_update_scalarRange(val);	
}

void graph_MainWindow::slot_cgnsAniTimesAct()
{
	if (cur_PipelineObj == NULL)
		return;
	if (cur_PipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle == "vtkBlockColors")
		return;
	if (cur_PipelineObj->numberOfCgnsTimes< 2)
		return;
	QString tep_name = cgnsAniTimesAct->data().toString().trimmed();
	if (tep_name == "cgnsAniTimesStart")
	{
		ani_update_all_form(true);
		pipelineObj_plotWidget->slot_update_cgns_pipelineObj(cur_PipelineObj);
		cgnsAniTimesAct->setIcon(QIcon(":/images/cgnsAniTimesStop.png"));
		cgnsAniTimesAct->setData("cgnsAniTimesStop");
		AniToolBar->setEnabled(false);
	}
	else if (tep_name == "cgnsAniTimesStop")
	{
		pipelineObj_plotWidget->slot_stop_update_cgns_pipelineObj();
		cgnsAniTimesAct->setIcon(QIcon(":/images/cgnsAniTimesStart.png"));
		cgnsAniTimesAct->setData("cgnsAniTimesStart");
		ani_update_all_form(false);
		AniToolBar->setEnabled(true);
	}
}

void graph_MainWindow::slot_cgnsAniTimesIcon_status(bool)
{
	cgnsAniTimesAct->setIcon(QIcon(":/images/cgnsAniTimesStart.png"));
	cgnsAniTimesAct->setData("cgnsAniTimesStart");
	ani_update_all_form(false);
	AniToolBar->setEnabled(true);
}

void graph_MainWindow::slot_update_UndoReDoButton(int)
{
	MyInteractorStyle* tep_style=pipelineObj_plotWidget->get_mouse_style();
	if (tep_style == NULL)
		return;
	if (tep_style->get_cameraPara_list().count() < 2)
	{
		cameraUndoAct->setEnabled(false);
		cameraRedoAct->setEnabled(false);
	}
	else
	{
		int tep_pos = tep_style->get_cur_pos();
		int tep_num = tep_style->get_cameraPara_list().count();
		if (tep_pos + 1 >= tep_num)
		{
			cameraUndoAct->setEnabled(false);
			cameraRedoAct->setEnabled(true);
		}
		else if (tep_pos <= 0)
		{
			cameraUndoAct->setEnabled(true);
			cameraRedoAct->setEnabled(false);
		}
		else
		{
			cameraUndoAct->setEnabled(true);
			cameraRedoAct->setEnabled(true);
		}
	}
}
void graph_MainWindow::script_ChangeLanguage(QString lang)
{
	if (lang == "English")
		slot_setEnglish();
	else if (lang == "Chinese")
		slot_setChinese();
}
Ui::graph_MainWindow* graph_MainWindow::getUi()
{
	return ui;
}
void graph_MainWindow::resizeEvent(QResizeEvent * event)
{	
	QMainWindow::resizeEvent(event);
	if ((dos_scriptFileName != "") && (isMaximized()))
	{
		RunScript(dos_scriptFileName);
		dos_scriptFileName = "";
	}
}
void graph_MainWindow::SetDosScriptFile(QString fileName)
{
	dos_scriptFileName = fileName;
}
bool graph_MainWindow::func_aniToolBar(int tep_cur_frame_xh)
{
	bool flag = false;
	LoadActionFile* ptr = new LoadActionFile;
	int tep_num_maxDisplace = ptr->func_get_max_num_displace(g_ani_objList);
	delete ptr;
	ptr = NULL;
	if ((pipelineObj_plotWidget->max_ani_frames < 2) && (tep_num_maxDisplace>1)&&(tep_cur_frame_xh<tep_num_maxDisplace))
	{
		for (int j = 0; j < g_ani_objList.count(); j++)
		{
			if (g_ani_objList[j]->GetObjectType() == dNumeric_DataSource)
			{
				if (g_ani_objList[j]->mPipeLineObjProp.pipelineObj_base_propData.fileList.count()==1)
				{
					LoadActionFile* ptr = new LoadActionFile;
					ptr->func_setPipeObjDisplaceData(g_ani_objList[j], tep_cur_frame_xh, g_ani_objList);
					delete ptr;
					ptr = NULL;
				}				
			}
		}
		pipelineObj_plotWidget->set_cur_ani_dis_xh(tep_cur_frame_xh);
		flag = true;
	}
	return flag;
}
void graph_MainWindow::script_Properties_scalarBarPosition(PipelineObject* tep_piplnObj, QString colName,int tep_orietation,double pos[4])
{
	if (tep_piplnObj == NULL)
		return;
	if (get_pipelineObj_plotWidget()->getScriptRunning())
	{
		ScalarBarPosition_Para tep_posPara;
		tep_posPara.orietation = tep_orietation;		
		for (int i = 0; i < 4; i++)
			tep_posPara.position[i] = pos[i];
		tep_piplnObj->scalarBar_positionMap.insert(colName, tep_posPara);
	}
}
void graph_MainWindow::script_Properties_streamline_reflection(PipelineObject* tep_piplnObj, bool val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->m_reflectionPara.flag_refleciton = val;
}
void graph_MainWindow::script_Properties_streamline_reflectionAxes(PipelineObject* tep_piplnObj, ReflectionPlane val)
{
	if (tep_piplnObj == NULL)
		return;
	tep_piplnObj->m_reflectionPara.type = val;
}
PipelineObject* graph_MainWindow::script_getPipelineObj_byID(int id)
{
	PipelineObject* tep_obj = NULL;
	QList<PipelineObject*> tep_list=mProjectDB.GetPipelineObjects();
	for (int i = 0; i < tep_list.count(); i++)
	{
		if (tep_list.at(i)->get_objID() == id)
		{
			return tep_list.at(i);
		}
	}
	return tep_obj;
}
void graph_MainWindow::setDockSize(QDockWidget* dock, int setWidth, int setHeight)
{
	if (setWidth >= 0)
	{
		if (dock->width() < setWidth)
			dock->setMinimumWidth(setWidth);
		else 
			dock->setMaximumWidth(setWidth);
	}
	if (setHeight >= 0)
	{
		if (dock->height() < setHeight)
			dock->setMinimumHeight(setHeight);
		else 
			dock->setMaximumHeight(setHeight);
	}
}
void graph_MainWindow::script_ReturnToOldSize()
{
	QSize oldMaxSize = QSize(16777215, 16777215);
	QSize oldMinSize = QSize(1, 1);
	//setGeometry(0, 0, script_MainWinWid, script_MainWinHei);
	pipelineObj_propWidget->setMinimumSize(oldMinSize);
	pipelineObj_propWidget->setMaximumSize(oldMaxSize);
	pipelineObj_treeWidget->setMinimumSize(oldMinSize);
	pipelineObj_treeWidget->setMaximumSize(oldMaxSize);
	console_cmdWidget->setMinimumSize(oldMinSize);
	console_cmdWidget->setMaximumSize(oldMaxSize);	
	QSettings settings("Software Inc.", "Icon Editor");
	settings.beginGroup("mainWindow");	
	restoreState(settings.value("state").toByteArray());
	restoreGeometry(settings.value("geometry").toByteArray());
	settings.endGroup();
}

