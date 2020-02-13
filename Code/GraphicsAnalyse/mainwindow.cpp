#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ScriptHandler.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	aniSetting_data.flag_save = false;
	aniSetting_data.frame_rate = 15.0;
	aniSetting_data.frame_num = 0;
	aniSetting_data.frame_step = 1;
	aniSetting_data.frame_start_xh = 0;
	aniSetting_data.frame_end_xh = 0;
	aniSetting_data.image_wid = 600;
	aniSetting_data.image_hei = 400;
	aniSetting_data.total_frame_num = 0;

	cur_ani_time = QTime::currentTime();
	count_obj_id = 0;
	cur_PipelineObj = NULL;
	pre_translator =NULL;
    //translator = new QTranslator;
    //translator->load("./QianFan_Curve.qm");
    //qApp->installTranslator(translator);
    cur_ini_filename="";
    flag_newFile=false;
    pipelineObj_plotWidget=NULL;
    pipelineObj_propWidget=NULL;
    pipelineObj_treeWidget=NULL;
    console_cmdWidget=NULL;
    //column_dataWidget=NULL;
    flag_DockWin=false;
    setStyleSheet("QMainWindow::title{background-color: qlineargradient(spread:reflect, x1:0, y1:0, x2:0, y2:1, stop:0 #eaedf6, stop:1 #a6c9e2);}");
    setWindowTitle(QStringLiteral("GraphicsAnalysis 1.0"));
	this->setWindowIcon(QIcon(":/images/app.png"));
	
    //setWindowTitle(QObject::tr("plot module"));
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
    //////////////20160817 add
    mdiArea->setViewMode(QMdiArea::TabbedView);
    mdiArea->setTabPosition(QTabWidget::North);
    mdiArea->setTabsClosable(false);
    mdiArea->setTabsMovable(false);
    mdiArea->setTabShape(QTabWidget::Triangular);
    mdiArea->setStyleSheet("QTabWidget::title{border-radius: 6px; background-image: url(:/images/project_title.png);}");
    //mdiArea->setStyleSheet("QMdiArea::TabbedView:title{background-color:qlineargradient(spread:reflect, x1:0, y1:0, x2:0, y2:1, stop:0 #eaedf6, stop:1 #0009e2);}");
    setCentralWidget(mdiArea);
	newFile();
}

MainWindow::~MainWindow()
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
    delete ui;
}
PipelineObjProp* MainWindow::GetPipelineObjProp(PipelineObject* pplnObj)
{
	return &pplnObj->mPipeLineObjProp;
}
void MainWindow::AddPipelineObject(PipelineObject* pipeObj)
{
	count_obj_id++;
	pipeObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id = count_obj_id;
	mProjectDB.AddPipelineObject(pipeObj);
	pipelineObj_treeWidget->AddPipelineObject(pipeObj);
	pipelineObj_treeWidget->DoSelectPipelineObject(pipeObj); //构建树
	if (pipeObj->GetObjectType() == dSlice_DataSource)
	{
		pipelineObj_plotWidget->slot_init_sliceFilter_plotForm((FilterSlice*)pipeObj);
		connect(((FilterSlice*)pipeObj)->myCallback, SIGNAL(sig_update_sliceProp_widget(double*, double*)), (sliceFilterPropForm*)(pipelineObj_propWidget->propWidget->cur_addWidget), SLOT(slot_update_sliceProp_widget(double*, double*)));
		
	}
	else if (pipeObj->GetObjectType() == dClip_DataSource)
	{
		pipelineObj_plotWidget->slot_init_clipFilter_plotForm((FilterClip*)pipeObj);
		connect(((FilterClip*)pipeObj)->myCallback, SIGNAL(sig_update_sliceProp_widget(double*, double*)), (sliceFilterPropForm*)(pipelineObj_propWidget->propWidget->cur_addWidget), SLOT(slot_update_sliceProp_widget(double*, double*)));
		
	}
	else if (pipeObj->GetObjectType() == dContour_DataSource)
	{
		//pipelineObj_plotWidget->slot_init_contourFilter_plotForm((FilterContour*)pipeObj);
	}
	//pipelineObj_propWidget->propWidget->updateObjPropForm(pipeObj);
	if (pipeObj->mPipeLineObjProp.pipelineObj_base_propData.fileList.count() > pipelineObj_plotWidget->max_ani_frames)
	{
		pipelineObj_plotWidget->max_ani_frames = pipeObj->mPipeLineObjProp.pipelineObj_base_propData.fileList.count();
		
		if (pipelineObj_plotWidget->max_ani_frames > 1)
		{
			QString tepstr; 
			tepstr.setNum(pipelineObj_plotWidget->max_ani_frames, 10);
			cur_frame_num->setText(" of "+tepstr);
			cur_frame_num->setVisible(true);
			cur_frame_spinBox->setMaximum(pipelineObj_plotWidget->max_ani_frames - 1);
			aniSetting_data.total_frame_num = pipelineObj_plotWidget->max_ani_frames;
		}
	}
}
void MainWindow::slot_delPipelineObj(PipelineObject* pipeObj)
{
	if (pipeObj->GetObjectType() == dClip_DataSource)
	{
		((FilterClip*)pipeObj)->planeWidget->Off();
	}
	else if (pipeObj->GetObjectType() == dSlice_DataSource)
	{
		((FilterSlice*)pipeObj)->planeWidget->Off();
	}
	pipelineObj_treeWidget->RmvPipelineObject(pipeObj);
	pipelineObj_plotWidget->del_obj_scalarBarWidget(pipeObj);
	pipelineObj_propWidget->propWidget->slot_update_buildIn_propForm();
	mProjectDB.RmvPipelineObject(pipeObj);
	pipelineObj_plotWidget->slot_refreshView(mProjectDB.GetPipelineObjects());//emit sig_refreshView_fromMainWin(mProjectDB.GetPipelineObjects());

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
		pipelineObj_plotWidget->cur_ani_frame_xh = 0;
		tepstr.setNum(tep_max_frames, 10);
		cur_frame_num->setText(" of "+tepstr);
	}	
}
void MainWindow::slot_LoadDataSource(QString tep_file, QStringList fileList,int tep_file_xh)
{
	NumericSrcObject* cur_NumericSrcObj = new NumericSrcObject(fileList,tep_file_xh);
	if (cur_NumericSrcObj->readFile(fileList.at(tep_file_xh)))
	{
		cur_PipelineObj = (PipelineObject*)cur_NumericSrcObj;
		AddPipelineObject(cur_PipelineObj);
	}
	else
	{
		delete cur_NumericSrcObj;
		cur_NumericSrcObj = NULL;
	}
}
void MainWindow::slot_refreshView_fromPropWidget()
{
	emit sig_refreshView_fromMainWin(mProjectDB.GetPipelineObjects());
}
void MainWindow::saveAniSetting()
{
	saveAniDialog *dlg = new saveAniDialog;
	connect(dlg, SIGNAL(sig_runAni_fromAniSetting(saveAni_prop)), this, SLOT(slot_runAni_fromAniSetting(saveAni_prop)));
	dlg->updateDialog(aniSetting_data);
	dlg->exec();
	delete dlg;
	dlg = NULL;
}
void MainWindow::creatActions()
{
    fileMenu = menuBar()->addMenu(QObject::tr("File"));
    viewMenu = menuBar()->addMenu(QObject::tr("View"));
	filterMenu = menuBar()->addMenu(QObject::tr("Filter"));
    toolMenu = menuBar()->addMenu(QObject::tr("Tool"));
    helpMenu = menuBar()->addMenu(QObject::tr("Help"));

    const QIcon openIcon = QIcon::fromTheme("open", QIcon(":/images/open.png"));
    openAct = new QAction(openIcon, QObject::tr("open"), this);
    connect(openAct, &QAction::triggered, this, &MainWindow::openFile);
    openAct->setShortcut(QKeySequence::Open);
    openAct->setStatusTip(QObject::tr("Open an existing file."));
    fileMenu->addAction(openAct);
	fileMenu->addSeparator();
	runScriptAct = new QAction(tr("Run script"), this);
	connect(runScriptAct, &QAction::triggered, this, &MainWindow::runScript);
	fileMenu->addAction(runScriptAct);

	const QIcon saveAniIcon = QIcon::fromTheme("saveAnimation...", QIcon(":/images/saveAnimation.png"));
	saveAniAct = new QAction(saveAniIcon, QObject::tr("saveAnimation..."), this);
	connect(saveAniAct, &QAction::triggered, this, &MainWindow::saveAniSetting);
	saveAniAct->setStatusTip(QObject::tr("save avi animation."));
	fileMenu->addAction(saveAniAct);
	fileMenu->addSeparator();

	const QIcon exitIcon = QIcon::fromTheme("exie", QIcon(":/images/exit.png"));
	exitAct = new QAction(exitIcon, QObject::tr("exit"), this);
	connect(exitAct, SIGNAL(triggered()), this, SLOT(exitSoftware()));
	exitAct->setShortcut(QKeySequence::Quit);
	exitAct->setStatusTip(QObject::tr("Exit this software."));
	fileMenu->addAction(exitAct);

    const QIcon contourIcon = QIcon::fromTheme("contour", QIcon(":/images/isosurf.png"));
	contourAct = new QAction(contourIcon, QObject::tr("contour"), this);
	connect(contourAct, &QAction::triggered, this, &MainWindow::slot_contourFilter);
	contourAct->setStatusTip(QStringLiteral("contour filter."));
	filterMenu->addAction(contourAct);

    const QIcon clipIcon = QIcon::fromTheme("clip", QIcon(":/images/clip.png"));
	clipAct = new QAction(clipIcon, QObject::tr("clip"), this);
	connect(clipAct, &QAction::triggered, this, &MainWindow::slot_clipFilter);
	clipAct->setStatusTip(QObject::tr("clipFilter"));
	filterMenu->addAction(clipAct);

	const QIcon sliceIcon = QIcon::fromTheme("slice", QIcon(":/images/slice.png"));
	sliceAct = new QAction(sliceIcon, QObject::tr("slice"), this);
	connect(sliceAct, &QAction::triggered, this, &MainWindow::slot_sliceFilter);
	sliceAct->setStatusTip(QObject::tr("sliceFilter"));
	filterMenu->addAction(sliceAct);

	const QIcon glyphIcon = QIcon::fromTheme("glyph", QIcon(":/images/glyph.png"));
	glyphAct = new QAction(glyphIcon, QObject::tr("glyph"), this);
	connect(glyphAct, &QAction::triggered, this, &MainWindow::slot_glyphFilter);
	glyphAct->setStatusTip(QObject::tr("glyphFilter"));
	filterMenu->addAction(glyphAct);

    const QIcon langueIcon = QIcon::fromTheme("document-open", QIcon(":/images/langue.png"));
    QMenu *langueMenu = new QMenu(QObject::tr("language"));
    langueMenu->setIcon(langueIcon);
	language_cn = new QAction(QObject::tr("chinese"), this);
    language_cn->setCheckable(true);
    language_cn->setChecked(true);
	language_cn->setEnabled(false);
    langueMenu->addAction(language_cn);
	language_en = new QAction(QObject::tr("english"), this);
    language_en->setCheckable(true);
    language_en->setChecked(false);
    language_en->setEnabled(false);
    langueMenu->addAction(language_en);
    toolMenu->addMenu(langueMenu);


    menuAct = new QAction(QObject::tr("menu"), this);
    menuAct->setStatusTip(QObject::tr("show menu bar."));
    menuAct->setCheckable(true);
	menuAct->setChecked(true);
	viewMenu->addAction(menuAct);
	connect(menuAct, SIGNAL(triggered()), this, SLOT(slot_showMenuBar()));

    statusAct = new QAction(QObject::tr("status"), this);
    statusAct->setStatusTip(QObject::tr("show status bar."));
    statusAct->setCheckable(true);
	statusAct->setChecked(true);
    viewMenu->addAction(statusAct);
	connect(statusAct, SIGNAL(triggered()), this, SLOT(slot_showStatusBar()));
    
	toolAct = new QAction(QObject::tr("tool"), this);
    toolAct->setStatusTip(QObject::tr("show tool bar."));
    toolAct->setCheckable(true);
	toolAct->setChecked(true);
	viewMenu->addAction(toolAct);
	connect(toolAct, SIGNAL(triggered()), this, SLOT(slot_showToolBar()));

    const QIcon helpIcon = QIcon::fromTheme("help", QIcon(":/images/help.png"));
    QAction *helpAct = new QAction(QObject::tr("help"), this);
    helpAct->setStatusTip(QObject::tr("help"));
    connect(helpAct, SIGNAL(triggered()), this, SLOT(slot_showHelpPDF()));
    helpMenu->addAction(helpAct);
    QAction *aboutAct = new QAction(helpIcon,QObject::tr("about"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(slot_showAboutDialog()));
    aboutAct->setStatusTip(QObject::tr("about"));
    helpMenu->addAction(aboutAct);

    connect(language_en, SIGNAL(triggered()), this, SLOT(slot_setEnglish()));
    connect(language_cn, SIGNAL(triggered()), this, SLOT(slot_setChinese()));



	fileToolBar = addToolBar(QObject::tr("&FileBar"));
    //fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
	//fileToolBar->addSeparator();
    
	filterToolBar = addToolBar(QObject::tr("&FilterBar"));
	filterToolBar->addAction(contourAct);
	filterToolBar->addAction(clipAct);
	filterToolBar->addAction(sliceAct);
	filterToolBar->addAction(glyphAct);
	//filterToolBar->addSeparator();
	

	const QIcon firstIcon = QIcon::fromTheme("first", QIcon(":/images/first.png"));
	firstAct = new QAction(firstIcon, QObject::tr("first frame"), this);
	firstAct->setStatusTip(QObject::tr("first frame"));
	connect(firstAct, SIGNAL(triggered()), this, SLOT(slot_animateFirst()));
	const QIcon lastIcon = QIcon::fromTheme("last", QIcon(":/images/last.png"));
	lastAct = new QAction(lastIcon, QObject::tr("last frame"), this);
	lastAct->setStatusTip(QObject::tr("last frame"));
	connect(lastAct, SIGNAL(triggered()), this, SLOT(slot_animateLast()));
	const QIcon runIcon = QIcon::fromTheme("run", QIcon(":/images/run.png"));
	runAct = new QAction(runIcon, QObject::tr("run"), this);
	runAct->setData("aniRun");
	connect(runAct, SIGNAL(triggered()), this, SLOT(slot_animate()));
	runAct->setStatusTip(QObject::tr("run"));
	const QIcon nextIcon = QIcon::fromTheme("next", QIcon(":/images/next.png"));
	nextAct = new QAction(nextIcon, QObject::tr("next frame"), this);
	nextAct->setStatusTip(QObject::tr("next frame"));
	connect(nextAct, SIGNAL(triggered()), this, SLOT(slot_animateNext()));
	const QIcon endIcon = QIcon::fromTheme("end", QIcon(":/images/end.png"));
	endAct = new QAction(endIcon, QObject::tr("end frame"), this);
	endAct->setStatusTip(QObject::tr("end frame"));
	connect(endAct, SIGNAL(triggered()), this, SLOT(slot_animateEnd()));
	AniToolBar = addToolBar(QObject::tr("&AnimateBar"));
	AniToolBar->addAction(firstAct);
	AniToolBar->addAction(lastAct);
	AniToolBar->addAction(runAct);
	AniToolBar->addAction(nextAct);
	AniToolBar->addAction(endAct);
	AniToolBar->addSeparator();
	QLabel *lab1 = new QLabel(tr("frame:"));
	//lab2=new QLabel(QStringLiteral("of"));
	//lab3=new QLabel(QStringLiteral("  "));
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
	AniToolBar->addWidget(lab1);
	AniToolBar->addWidget(cur_frame_lineEdit);
	AniToolBar->addWidget(lab_blank);
	AniToolBar->addWidget(cur_frame_spinBox);
	AniToolBar->addWidget(cur_frame_num);
	//lab_of->setVisible(false);
	cur_frame_num->setVisible(false);

    flag_saveAvi_label=new QLabel(QObject::tr("animate: "));
    flag_saveAvi_checkBox=new QCheckBox;
    flag_saveAvi_checkBox->setText(QObject::tr("save avi"));

	exitToolBar = addToolBar(QObject::tr("&exitBar"));
    exitToolBar->addAction(exitAct);

	contourAct->setEnabled(false);
	clipAct->setEnabled(false);
	sliceAct->setEnabled(false);
	glyphAct->setEnabled(false);
	firstAct->setEnabled(false);
	lastAct->setEnabled(false);
	runAct->setEnabled(false);
	nextAct->setEnabled(false);
	endAct->setEnabled(false);
}
void MainWindow::slot_showToolBar()
{
	if (!toolAct->isChecked())
	{
		fileToolBar->setVisible(false);
		filterToolBar->setVisible(false);
		AniToolBar->setVisible(false);
		exitToolBar->setVisible(false);
	}
	else
	{
		fileToolBar->setVisible(true);
		filterToolBar->setVisible(true);
		AniToolBar->setVisible(true);
		exitToolBar->setVisible(true);
	}
}
void MainWindow::slot_showMenuBar()
{
	if (menuAct->isChecked())
		menuBar()->show();
	else
		menuBar()->hide();
}
void MainWindow::slot_showStatusBar()
{
	if (statusAct->isChecked())
		statusBar()->show();
	else
		statusBar()->hide();
}
void MainWindow::slot_showHelpPDF()
{
    //QString runPath = QCoreApplication::applicationDirPath();
	QString runPath = QDir::currentPath();
    QString helpName = "help.pdf";
    QString helpPath = QString("%1/%2").arg(runPath).arg(helpName);
    QFile bfilePath(helpPath);
    if (!bfilePath.exists())
        return;
    QString filePath = "file:///" + helpPath;   //打开文件夹用file:///,打开网页用http://
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}

void MainWindow::slot_showAboutDialog()
{
    QMessageBox::information(NULL, QObject::tr("About"), QObject::tr("version : 1.0"));
}
void MainWindow::slot_setEnglish()
{
    language_cn->setChecked(false);
    language_en->setChecked(true);
    if(pre_translator!=NULL)
    {
        qApp->removeTranslator(pre_translator);
        delete pre_translator;
        pre_translator=NULL;
    }
	QTranslator *translator = new QTranslator;
    translator->load("./QianFan_Curve_en.qm");
    qApp->installTranslator(translator);
    pre_translator=translator;
	ui->retranslateUi(this);
}
void MainWindow::slot_setChinese()
{
    language_en->setChecked(false);
    language_cn->setChecked(true);
    if(pre_translator!=NULL)
    {
        qApp->removeTranslator(pre_translator);
        delete pre_translator;
        pre_translator=NULL;
    }
	QTranslator *translator = new QTranslator;
    translator->load("./QianFan_Curve_cn.qm");
    qApp->installTranslator(translator);
    pre_translator=translator;
	ui->retranslateUi(this);
}
void MainWindow::slot_contourFilter()
{
	PipelineObject *pipelineobj = pipelineObj_treeWidget->GetActivePipelineObject();
	if (pipelineobj == NULL)
	{
		QMessageBox box(QMessageBox::Warning, QStringLiteral("warning"), QStringLiteral("thie pioeline object has children,you can not delete it！"));
		box.setStandardButtons(QMessageBox::Yes);
		box.setButtonText(QMessageBox::Yes, QStringLiteral("ok"));
		box.exec();
		return;
	}
	FilterContour *contour_pipelineObj = new FilterContour("Contour", pipelineobj);
	pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors = false;
	pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
	if (pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.contains(pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle))
	{
		pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert(pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle, false);
	}
	contour_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors = true;
	contour_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
	AddPipelineObject(contour_pipelineObj);
}
void MainWindow::slot_clipFilter()
{
	PipelineObject *pipelineobj = pipelineObj_treeWidget->GetActivePipelineObject();
	if (pipelineobj == NULL)
	{
		QMessageBox box(QMessageBox::Warning, QStringLiteral("warning"), QStringLiteral("thie pioeline object has children,you can not delete it！"));
		box.setStandardButtons(QMessageBox::Yes);
		box.setButtonText(QMessageBox::Yes, QStringLiteral("ok"));
		box.exec();
		return;
	}
	FilterClip *clip_pipelineObj = new FilterClip("Clip", pipelineobj);
	pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors = false;
	pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
	if (pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.contains(pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle))
	{
		pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert(pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle, false);
	}
	clip_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors = true;
	clip_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
	AddPipelineObject(clip_pipelineObj);
}
void MainWindow::slot_sliceFilter()
{
	PipelineObject *pipelineobj = pipelineObj_treeWidget->GetActivePipelineObject();
	if (pipelineobj == NULL)
	{
		QMessageBox box(QMessageBox::Warning, QStringLiteral("warning"), QStringLiteral("thie pioeline object has children,you can not delete it！"));
		box.setStandardButtons(QMessageBox::Yes);
		box.setButtonText(QMessageBox::Yes, QStringLiteral("ok"));
		box.exec();
		return;
	}
	FilterSlice *slice_pipelineObj = new FilterSlice("Slice", pipelineobj);
	pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors = false;
	pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
	if (pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.contains(pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle))
	{
		pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert(pipelineobj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle, false);
	}
	slice_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors = true;
	slice_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
	AddPipelineObject(slice_pipelineObj);
}
void MainWindow::slot_glyphFilter()
{
	PipelineObject *pipelineobj = pipelineObj_treeWidget->GetActivePipelineObject();
	if (pipelineobj == NULL)
	{
		QMessageBox box(QMessageBox::Warning, QStringLiteral("warning"), QStringLiteral("thie pioeline object has children,you can not delete it！"));
		box.setStandardButtons(QMessageBox::Yes);
		box.setButtonText(QMessageBox::Yes, QStringLiteral("ok"));
		box.exec();
		return;
	}
	FilterVector *vector_pipelineObj = new FilterVector("Vector", pipelineobj);
	vector_pipelineObj->mBaseGrid = ((NumericSrcObject*)pipelineobj)->mBaseGrid;
	vector_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors = true;
	vector_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
	AddPipelineObject(vector_pipelineObj);
}
void MainWindow::slot_update_filterToolBar(bool flag_selected, PipelineObject* tepObj)
{
	if (!flag_selected)
	{
		contourAct->setEnabled(false);
		clipAct->setEnabled(false);
		sliceAct->setEnabled(false);
		glyphAct->setEnabled(false);
	}
	else if (tepObj != NULL)
	{
		if ((tepObj->GetObjectType() == dNumeric_DataSource) && tepObj->flag_exist_actors)
		{
			clipAct->setEnabled(true);
			sliceAct->setEnabled(true);
			glyphAct->setEnabled(true);
			if (tepObj->GetGridType() == dRECTILINEAR_GRID)
				contourAct->setEnabled(true);
			else
				contourAct->setEnabled(false);
		}
		else
		{
			contourAct->setEnabled(false);
			clipAct->setEnabled(false);
			sliceAct->setEnabled(false);
			glyphAct->setEnabled(false);
		}
	}
}
void MainWindow::slot_update_aniToolBar(int num)
{
	if (num == 0)
	{
		firstAct->setEnabled(false);
		lastAct->setEnabled(false);
		runAct->setEnabled(false);
		nextAct->setEnabled(false);
		endAct->setEnabled(false);
	}
	else
	{
		firstAct->setEnabled(true);
		lastAct->setEnabled(true);
		runAct->setEnabled(true);
		nextAct->setEnabled(true);
		endAct->setEnabled(true);
	}
}
void MainWindow::newFile()
{
    creatDocWindows();
    if(pipelineObj_plotWidget==NULL)
    {
		pipelineObj_plotWidget = new PipelineObjPlotForm;
		pipelineObj_plotWidget->aniSetting_data = &aniSetting_data;
		mdiArea->addSubWindow(pipelineObj_plotWidget);
		pipelineObj_plotWidget->showMaximized();//
		connect(this, SIGNAL(sig_refreshView_fromMainWin(QList<PipelineObject*>)), pipelineObj_plotWidget, SLOT(slot_refreshView_fromMainWin(QList<PipelineObject*>)));
		connect(pipelineObj_propWidget->propWidget, SIGNAL(sig_refreshView_fromPropWidget()), this, SLOT(slot_refreshView_fromPropWidget()));
		connect(pipelineObj_propWidget->propWidget, SIGNAL(sig_change_global_view_prop(renderview_global_prop)), pipelineObj_plotWidget, SLOT(slot_change_global_view_prop(renderview_global_prop)));
		connect(pipelineObj_propWidget->propWidget, SIGNAL(sig_update_opacity(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_opacity(PipelineObject*)));
		connect(pipelineObj_propWidget->propWidget, SIGNAL(sig_update_color(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_color(PipelineObject*)));	
		connect(pipelineObj_propWidget->propWidget, SIGNAL(sig_update_translate(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_translate(PipelineObject*)));
		connect(pipelineObj_propWidget->propWidget, SIGNAL(sig_update_origin(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_origin(PipelineObject*)));
		connect(pipelineObj_propWidget->propWidget, SIGNAL(sig_update_scale(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_scale(PipelineObject*)));
		connect(pipelineObj_propWidget->propWidget, SIGNAL(sig_update_orientation(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_orientation(PipelineObject*)));
		connect(pipelineObj_propWidget->propWidget, SIGNAL(sig_update_representation(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_representation(PipelineObject*)));
		connect(pipelineObj_propWidget->propWidget, SIGNAL(sig_update_axes_visible(bool)), pipelineObj_plotWidget, SLOT(slot_update_axes_visible(bool)));		
		connect(pipelineObj_propWidget->propWidget, SIGNAL(sig_update_specular(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_specular(PipelineObject*)));
		connect(pipelineObj_propWidget->propWidget, SIGNAL(sig_update_specularPower(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_specularPower(PipelineObject*)));
		connect(pipelineObj_propWidget->propWidget, SIGNAL(sig_update_specularColor(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_specularColor(PipelineObject*)));
		connect(pipelineObj_propWidget->propWidget, SIGNAL(sig_update_solidColor(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_solidColor(PipelineObject*)));
		connect(pipelineObj_propWidget->propWidget, SIGNAL(sig_update_diffuse(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_diffuse(PipelineObject*)));
		connect(pipelineObj_propWidget->propWidget, SIGNAL(sig_update_ambient(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_ambient(PipelineObject*)));
		connect(pipelineObj_propWidget->propWidget, SIGNAL(sig_update_interpolation(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_interpolation(PipelineObject*)));

		connect(pipelineObj_propWidget->propWidget, SIGNAL(sig_update_scalarBar(PipelineObject*)), pipelineObj_treeWidget, SLOT(slot_update_scalarBar(PipelineObject*)));
		connect(pipelineObj_treeWidget, SIGNAL(sig_update_scalarBar(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_scalarBar(PipelineObject*)));
		
		connect(pipelineObj_propWidget->propWidget, SIGNAL(sig_update_lineWidth(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_lineWidth(PipelineObject*)));
		connect(pipelineObj_propWidget->propWidget, SIGNAL(sig_update_pointSize(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_pointSize(PipelineObject*)));
		connect(pipelineObj_propWidget->propWidget, SIGNAL(sig_update_filterToolBar(bool, PipelineObject *)), this, SLOT(slot_update_filterToolBar(bool, PipelineObject *)));

		connect(pipelineObj_propWidget->propWidget, SIGNAL(sig_update_planeOrigin(PipelineObject *)), pipelineObj_plotWidget, SLOT(slot_update_planeOrigin(PipelineObject *)));
		connect(pipelineObj_propWidget->propWidget, SIGNAL(sig_update_planeNormal(PipelineObject *)), pipelineObj_plotWidget, SLOT(slot_update_planeNormal(PipelineObject *)));
		connect(pipelineObj_propWidget->propWidget, SIGNAL(sig_update_planeCameraNormal(PipelineObject *)), pipelineObj_plotWidget, SLOT(slot_update_planeCameraNormal(PipelineObject *)));
		connect(pipelineObj_propWidget->propWidget, SIGNAL(sig_update_planeVis(PipelineObject *)), pipelineObj_plotWidget, SLOT(slot_update_planeVis(PipelineObject *)));
		
		connect(pipelineObj_treeWidget, SIGNAL(sig_update_aniToolBar(int)), this, SLOT(slot_update_aniToolBar(int)));
		connect(pipelineObj_treeWidget, SIGNAL(sig_update_filterToolBar(bool, PipelineObject *)), this, SLOT(slot_update_filterToolBar(bool, PipelineObject *)));
		connect(pipelineObj_treeWidget, SIGNAL(sig_update_buildIn_propForm()), pipelineObj_propWidget->propWidget, SLOT(slot_update_buildIn_propForm()));
		connect(pipelineObj_treeWidget, SIGNAL(sig_update_pipelineObj_propForm(PipelineObject*)), pipelineObj_propWidget->propWidget, SLOT(slot_update_pipelineObj_propForm(PipelineObject*)));
		connect(pipelineObj_treeWidget, SIGNAL(sig_update_actor_visible_plotForm(PipelineObject*)), pipelineObj_plotWidget, SLOT(slot_update_actor_visible_plotForm(PipelineObject*)));
		
		connect(pipelineObj_treeWidget, SIGNAL(sig_del_curve_plot(int)), pipelineObj_plotWidget, SLOT(slot_del_curve_plot(int)));
		connect(pipelineObj_treeWidget, SIGNAL(sig_add_curve_plot()), pipelineObj_plotWidget, SLOT(slot_init_curve_plot()));
        connect(this, SIGNAL(sig_runAni()), pipelineObj_plotWidget, SLOT(slot_runAni()));
		connect(this, SIGNAL(sig_stopAni()), pipelineObj_plotWidget, SLOT(slot_stopAni()));
		connect(pipelineObj_treeWidget, SIGNAL(sig_update_clear_plotAni()), pipelineObj_plotWidget, SLOT(slot_update_clear_plotAni()));
		connect(this, SIGNAL(sig_saveToPng()), pipelineObj_plotWidget, SLOT(slot_saveToPng()));
		
		connect(pipelineObj_plotWidget, SIGNAL(sig_treeItem_disable(PipelineObject*)), pipelineObj_treeWidget, SLOT(slot_treeItem_disable(PipelineObject*)));
		connect(pipelineObj_plotWidget, SIGNAL(sig_update_runtool_runing(bool)), this, SLOT(slot_update_runtool_runing(bool)));
		connect(pipelineObj_plotWidget, SIGNAL(sig_update_pipelineObjs()), this, SLOT(slot_update_pipelineObjs()));
		connect(pipelineObj_plotWidget, SIGNAL(sig_update_countAniFrame(int,int)), this, SLOT(slot_update_countAniFrame(int,int)));
		
    }
	QTreeWidgetItem *tep_item = pipelineObj_treeWidget->pipelineObjTree->topLevelItem(0);
	pipelineObj_treeWidget->pipelineObjTree->setCurrentItem(tep_item->child(0));
    initWindow();
    flag_newFile=true;
}
void MainWindow::slot_changeCurFrame_spinBox(int tep_val)
{
	cur_frame_spinBox->setEnabled(false);
	QString tep_name = runAct->data().toString().trimmed();
	if (tep_name == "aniStop")
	{
		return;
	}
	QString tepstr;
	cur_frame_lineEdit->setText(tepstr.setNum(tep_val,10));
	pipelineObj_plotWidget->slot_changeCurFrame(tep_val);
	cur_frame_spinBox->setEnabled(true);
}
void MainWindow::slot_update_runtool_runing(bool flag_runing)
{
	if (flag_runing)
	{
		runAct->setIcon(QIcon(":/images/stop.png"));
		runAct->setData("aniStop");
		firstAct->setEnabled(false);
		endAct->setEnabled(false);
		nextAct->setEnabled(false);
		lastAct->setEnabled(false);
		cur_frame_spinBox->setEnabled(false);
		ani_update_all_form(true);
	}
	else 
	{
		runAct->setIcon(QIcon(":/images/run.png"));
		runAct->setData("aniRun");
		firstAct->setEnabled(true);
		endAct->setEnabled(true);
		nextAct->setEnabled(true);
		lastAct->setEnabled(true);
		cur_frame_spinBox->setEnabled(true);
	}
}
void MainWindow::DisplayMessage(const QString& title, const QString& message, EMsgType type, bool onDlg)
{
	console_cmdWidget->DisplayMessage(title, message, type, onDlg);
}
void MainWindow::initWindow()
{
	pipelineObj_treeWidget->initTree();
    console_cmdWidget->slot_update_clear_console_command();
}

void MainWindow::openFile()
{
	pipelineObj_treeWidget->slot_addFile();
}
void MainWindow::runScript()
{
	QString title = tr("Run Script");
	QString filter = tr("Script(*.scr)");
	QString path = QFileDialog::getOpenFileName(this, title, "", filter);
	if (path.isEmpty()) return;
	RunScript(path);
// 	qDebug() << path;
//	QString path = "F:/post_graph/03_code/TestProgram/new.scr";
// 	ScriptHandler handler(this, path);
// 	handler.Handle();
//	QStringList ks;
//	ks << "F:/testfile/FLBSM0000.vtk" << "F:/testfile/FLBSM0001.vtk" << "F:/testfile/FLBSM0002.vtk";
//	emit pipelineObj_treeWidget->sig_LoadDataSource(ks.at(0), ks, 0);
}
void MainWindow::RunScript(QString filename)
{
	ScriptHandler handler(this, filename);
	handler.Handle();
}
void MainWindow::exitSoftware()
{
    close();
}
void MainWindow::creatDocWindows()
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
		connect(this, SIGNAL(sig_update_curveTree()), pipelineObj_treeWidget, SLOT(slot_update_curveTree()));
		connect(this, SIGNAL(sig_update_tree_curve_info(int)), pipelineObj_treeWidget, SLOT(slot_update_tree_curve_info(int)));
		connect(pipelineObj_treeWidget, SIGNAL(sig_update_clear_console_command()), console_cmdWidget, SLOT(slot_update_clear_console_command()));
		connect(pipelineObj_treeWidget, SIGNAL(sig_delPipelineObj(PipelineObject*)), this, SLOT(slot_delPipelineObj(PipelineObject*)));
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
void MainWindow::slot_runAni_fromAniSetting(saveAni_prop)
{
	if (pipelineObj_plotWidget->max_ani_frames < 2)
		return;
	QString tep_name = runAct->data().toString().trimmed();
	if (tep_name == "aniStop")
		return;
	slot_animate();
}
void MainWindow::slot_animate()
{
	if (pipelineObj_plotWidget->max_ani_frames < 2)
		return;
	QString tep_name = runAct->data().toString().trimmed();
	if (tep_name == "aniRun")
	{
		runAct->setIcon(QIcon(":/images/stop.png"));
		runAct->setData("aniStop");
		glo_ani_objList = mProjectDB.GetPipelineObjects();
		firstAct->setEnabled(false);
		endAct->setEnabled(false);
		nextAct->setEnabled(false);
		lastAct->setEnabled(false);
		cur_frame_spinBox->setEnabled(false);
		if (pipelineObj_plotWidget->max_ani_frames > 1)
		{
			pipelineObj_treeWidget->projectTreeItemClicked(pipelineObj_treeWidget->pipelineObjTree->topLevelItem(0), 0);
			emit sig_runAni();
		}
			
	}
	else if (tep_name == "aniStop")
	{
		runAct->setIcon(QIcon(":/images/run.png"));
		runAct->setData("aniRun");
		firstAct->setEnabled(true);
		endAct->setEnabled(true);
		nextAct->setEnabled(true);
		lastAct->setEnabled(true);
		cur_frame_spinBox->setEnabled(true);
		emit sig_stopAni();
	}
}
void MainWindow::slot_animateFirst()
{
	cur_frame_spinBox->setValue(0);
	cur_frame_lineEdit->setText("0");
	//pipelineObj_plotWidget->slot_changeCurFrame(0);
}
void MainWindow::slot_animateLast()
{
	int tep_val = cur_frame_spinBox->value();
	if (tep_val > cur_frame_spinBox->minimum())
	{
		tep_val--;
		cur_frame_spinBox->setValue(tep_val);
		QString tepstr;
		tepstr.setNum(tep_val, 10);
		cur_frame_lineEdit->setText(tepstr);
		//pipelineObj_plotWidget->slot_changeCurFrame(tep_val);
	}
}
void MainWindow::slot_animateEnd()
{
	cur_frame_spinBox->setValue(pipelineObj_plotWidget->max_ani_frames - 1);
	QString tepstr;
	tepstr.setNum(pipelineObj_plotWidget->max_ani_frames - 1, 10);
	cur_frame_lineEdit->setText(tepstr);
	//pipelineObj_plotWidget->slot_changeCurFrame(pipelineObj_plotWidget->max_ani_frames - 1);
}
void MainWindow::slot_animateNext()
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
void MainWindow::ani_update_all_form(bool flag_start)
{
	pipelineObj_propWidget->propWidget->setEnabled(!flag_start);
	pipelineObj_treeWidget->setEnabled(!flag_start);
	contourAct->setEnabled(false);
	clipAct->setEnabled(false);
	sliceAct->setEnabled(false);
	glyphAct->setEnabled(false);
	openAct->setEnabled(false);
	
}
void MainWindow::slot_update_pipelineObjs()//QList<PipelineObject*> tep_pipelineObjs
{
	mProjectDB.update_pipelineObjs(glo_ani_objList);
	pipelineObj_treeWidget->setEnabled(true);
	pipelineObj_propWidget->propWidget->setEnabled(true);
	pipelineObj_treeWidget->update_pipelineObjs(glo_ani_objList);
	runAct->setIcon(QIcon(":/images/run.png"));
	runAct->setData("aniRun");
	firstAct->setEnabled(true);
	lastAct->setEnabled(true);
	nextAct->setEnabled(true);
	endAct->setEnabled(true);
	cur_frame_spinBox->setEnabled(true);
	openAct->setEnabled(true);
}
void MainWindow::slot_update_countAniFrame(int tep_cur_frame_xh,int tep_max_num_frame)
{
	QString tep_str;
	cur_frame_lineEdit->setText(tep_str.setNum(tep_cur_frame_xh));
	cur_frame_spinBox->setValue(tep_cur_frame_xh);
}

