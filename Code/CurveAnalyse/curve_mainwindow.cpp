#include "curve_mainwindow.h"
#include "ui_curve_mainwindow.h"
#include "CurveScriptHandler.h"
QMutex g_aniThread_mutex;
curve_aniPoint_struct aniPoint_para;
curve_MainWindow::curve_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::curve_MainWindow)
{
    ui->setupUi(this);
    curve_plotWidget=NULL;
    curve_treeWidget=NULL;
    column_dataWidget=NULL;
    flag_DockWin=false;
    setStyleSheet("QMainWindow::title{background-color: qlineargradient(spread:reflect, x1:0, y1:0, x2:0, y2:1, stop:0 #eaedf6, stop:1 #a6c9e2);}");
    setWindowTitle(tr("Numerical tank 2D module"));
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
	slot_newFile();
	initTranslate();
}

curve_MainWindow::~curve_MainWindow()
{
    mdiArea->closeAllSubWindows();
    flag_DockWin=false;
    if(curve_treeWidget!=NULL)
    {
        delete curve_treeWidget;
        curve_treeWidget=NULL;
    }
    if(column_dataWidget!=NULL)
    {
        delete column_dataWidget;
        column_dataWidget=NULL;
    }
    if(curve_plotWidget!=NULL)
    {
        delete curve_plotWidget;
        curve_plotWidget=NULL;
    }
	if (_translator != nullptr)
	{
		delete _translator;
		_translator = nullptr;
	}
    delete ui;
}

void curve_MainWindow::initTranslate()
{
	_translator = new Translator;
	actionEnglish->setChecked(true);
	actionChinese->setChecked(false);
	if (actionEnglish->isChecked())
		ChangeLanguage("English");
	else
		ChangeLanguage("Chinese");
}

Translator* curve_MainWindow::GetTranslator()
{
	return _translator;
}

void curve_MainWindow::ChangeLanguage(QString lang)
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

void curve_MainWindow::creatActions()
{
    fileMenu = menuBar()->addMenu(tr("File"));
    viewMenu = menuBar()->addMenu(tr("View"));
	zoomMenu = menuBar()->addMenu(tr("Zoom"));
    toolMenu = menuBar()->addMenu(tr("Tool"));
    helpMenu = menuBar()->addMenu(tr("Help"));

	const QIcon c_openIcon = QIcon::fromTheme("open", QIcon(":/images/open.png"));
	openAct = new QAction(c_openIcon, tr("open"), this);
	connect(openAct, &QAction::triggered, this, &curve_MainWindow::openFile);
    openAct->setShortcut(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file."));
    fileMenu->addAction(openAct);
	fileMenu->addSeparator();
	const QIcon c_savePngIcon = QIcon::fromTheme(QStringLiteral("save image"), QIcon(":/images/saveimage.png"));
	savePngAct = new QAction(c_savePngIcon, tr("save image"), this);
	connect(savePngAct, &QAction::triggered, this, &curve_MainWindow::slot_savePngFile);
	savePngAct->setStatusTip(QStringLiteral("save as image."));
	fileMenu->addAction(savePngAct);
	fileMenu->addSeparator();

	const QIcon c_scriptStartIcon = QIcon::fromTheme("scriptStart", QIcon(":/images/script_start.png"));
	runScriptAct = new QAction(c_scriptStartIcon, tr("Run script"), this);
	connect(runScriptAct, &QAction::triggered, this, &curve_MainWindow::slot_runScript);
	runScriptAct->setStatusTip(tr("Run script file."));
	fileMenu->addAction(runScriptAct);
	const QIcon c_scriptEditIcon = QIcon::fromTheme("scriptEdit", QIcon(":/images/script_edit.png"));
	editScriptAct = new QAction(c_scriptEditIcon, tr("Edit script"), this);
	connect(editScriptAct, &QAction::triggered, this, &curve_MainWindow::slot_editScript);
	editScriptAct->setStatusTip(tr("Generate script file."));
	fileMenu->addAction(editScriptAct);
	fileMenu->addSeparator();

	const QIcon c_exitIcon = QIcon::fromTheme("exie", QIcon(":/images/exit.png"));
	exitAct = new QAction(c_exitIcon, tr("exit"), this);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(slot_exitSoftware()));
    exitAct->setShortcut(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit this software."));
    fileMenu->addAction(exitAct);
	const QIcon c_langueIcon = QIcon::fromTheme("document-open", QIcon(":/images/langue.png"));
	langueMenu = new QMenu(tr("language"));
	//langueMenu->setIcon(langueIcon);
	const QIcon chineseIcon = QIcon::fromTheme("chinese_language", QIcon(":/images/chinese_language.png"));
	actionChinese = new QAction(chineseIcon, tr("chinese"), this);
	actionChinese->setCheckable(true);
	actionChinese->setChecked(false);
	langueMenu->addAction(actionChinese);
	const QIcon englishIcon = QIcon::fromTheme("english_language", QIcon(":/images/english_language.png"));
	actionEnglish = new QAction(englishIcon, tr("english"), this);
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
//	toolAct->setVisible(false);
	viewMenu->addAction(toolAct);
	viewMenu->addSeparator();
	connect(toolAct, SIGNAL(triggered()), this, SLOT(slot_showToolBar()));

	const QIcon c_helpIcon = QIcon::fromTheme("help", QIcon(":/images/help.png"));
	helpAct = new QAction(c_helpIcon, tr("help"), this);
    helpAct->setStatusTip(tr("help"));
    connect(helpAct, SIGNAL(triggered()), this, SLOT(slot_showHelpPDF()));
    helpMenu->addAction(helpAct);
	const QIcon c_aboutIcon = QIcon::fromTheme("about", QIcon(":/images/about.png"));
	aboutAct = new QAction(c_aboutIcon, tr("about"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(slot_showAboutDialog()));
    aboutAct->setStatusTip(tr("about"));
    helpMenu->addAction(aboutAct);

	connect(actionEnglish, SIGNAL(triggered()), this, SLOT(slot_setEnglish()));
	connect(actionChinese, SIGNAL(triggered()), this, SLOT(slot_setChinese()));

	const QIcon c_runIcon = QIcon::fromTheme("run", QIcon(":/images/run.png"));
	aniPointAct = new QAction(c_runIcon, tr("point animate"), this);
	aniPointAct->setData("aniRun");
	connect(aniPointAct, SIGNAL(triggered()), this, SLOT(slot_animate()));
	aniPointAct->setStatusTip(tr("point animate"));

	const QIcon c_delIcon = QIcon::fromTheme("run", QIcon(":/images/delAniPoint.png"));
	delAniPointAct = new QAction(c_delIcon, tr("clear animate points"), this);
	connect(delAniPointAct, SIGNAL(triggered()), this, SLOT(slot_delAniPointAct()));
	delAniPointAct->setStatusTip(tr("clear all animate points"));

	const QIcon c_zoomXIcon = QIcon::fromTheme("zoomX", QIcon(":/images/zoomX.png"));
	zoomXAct = new QAction(c_zoomXIcon, tr("XAxis zoom"), this);
	zoomXAct->setStatusTip(tr("X axis zoom"));
	zoomXAct->setEnabled(false);
	zoomXAct->setCheckable(true);
	zoomXAct->setChecked(false);
	zoomMenu->addAction(zoomXAct);
	connect(zoomXAct, SIGNAL(triggered(bool)), this, SLOT(slot_zoomXAct(bool)));
	const QIcon c_zoomYIcon = QIcon::fromTheme("zoomY", QIcon(":/images/zoomY.png"));
	zoomYAct = new QAction(c_zoomYIcon, tr("YAxis zoom"), this);
	zoomYAct->setStatusTip(tr("Y axis zoom"));
	zoomYAct->setEnabled(false);
	zoomYAct->setCheckable(true);
	zoomYAct->setChecked(false);
	zoomMenu->addAction(zoomYAct);
	connect(zoomYAct, SIGNAL(triggered(bool)), this, SLOT(slot_zoomYAct(bool)));
	//const QIcon c_zoomXYIcon = QIcon::fromTheme("zoomXY", QIcon(":/images/zoomXY.png"));
	//zoomXYAct = new QAction(c_zoomXIcon, tr("XYAxis zoom"), this);
	//zoomXYAct->setStatusTip(tr("XY axis zoom"));
	////zoomXYAct->setDisabled(true);
	//zoomXYAct->setCheckable(true);
	//zoomXYAct->setChecked(true);
	//zoomMenu->addAction(zoomXYAct);
	//connect(zoomXYAct, SIGNAL(triggered(bool)), this, SLOT(slot_zoomXYAct(bool)));

	fileToolBar = addToolBar(tr("&FileBar"));
	fileToolBar->addAction(openAct);
	fileToolBar->addAction(savePngAct);
	fileToolBar->addSeparator();
	fileToolBar->addAction(editScriptAct);
	fileToolBar->addAction(runScriptAct);
	aniPointBar = addToolBar(tr("&aniBar"));
	aniPointBar->addAction(aniPointAct);
	aniPointBar->addAction(delAniPointAct);
	zoomBar = addToolBar(tr("&zoomBar"));
	zoomBar->addAction(zoomXAct);
	zoomBar->addAction(zoomYAct);
	//zoomBar->addAction(zoomXYAct);
	helpToolBar = addToolBar(tr("&helpBar"));
	helpToolBar->addAction(helpAct);
	helpToolBar->addAction(aboutAct);
	exitToolBar = addToolBar(tr("&exitBar"));
	exitToolBar->addAction(exitAct);

    /*QToolBar *viewToolBar = addToolBar(tr("&ViewBar"));
    viewToolBar->addAction(menuAct);
    viewToolBar->addAction(toolAct);
    viewToolBar->addAction(statusAct);*/
}

void curve_MainWindow::slot_showMenuBar()
{
	if (menuAct->isChecked())
		menuBar()->show();
	else
		menuBar()->hide();
}

void curve_MainWindow::slot_showStatusBar()
{
	if (statusAct->isChecked())
		statusBar()->show();
	else
		statusBar()->hide();
}

void curve_MainWindow::slot_showToolBar()
{
	if (!toolAct->isChecked())
	{
		fileToolBar->setVisible(false);
		exitToolBar->setVisible(false);
		helpToolBar->setVisible(false);
	}
	else
	{
		fileToolBar->setVisible(true);
		exitToolBar->setVisible(true);
		helpToolBar->setVisible(true);
	}
}

void curve_MainWindow::slot_showHelpPDF()
{
    //QString runPath = QCoreApplication::applicationDirPath();
	QString runPath = QDir::currentPath();
    QString helpName = "help2D.pdf";
    QString helpPath = QString("%1/%2").arg(runPath).arg(helpName);
    QFile bfilePath(helpPath);
    if (!bfilePath.exists())
        return;
    QString filePath = "file:///" + helpPath;   //打开文件夹用file:///,打开网页用http://
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}

void curve_MainWindow::slot_showAboutDialog()
{
    //QMessageBox::information(NULL, tr("About"), tr("version : 1.0"));
	curve_aboutDialog *dlg = new curve_aboutDialog;
	dlg->exec();
	delete dlg;
	dlg = NULL;
	
}

void curve_MainWindow::slot_setEnglish()
{
	ChangeLanguage("English");
}

void curve_MainWindow::slot_setChinese()
{
	ChangeLanguage("Chinese");
}

void curve_MainWindow::slot_savePngFile()
{
	curve_plotWidget->func_saveImage(true, -1, -1, "");
}

void curve_MainWindow::slot_newFile()
{
	initPlotData();
    creatDocWindows();
    if(curve_plotWidget==NULL)
    {
        curve_plotWidget=new CurvePlotForm;
		curve_plotWidget->curve_data_ptr = &curve_data;
        mdiArea->addSubWindow(curve_plotWidget);
        curve_plotWidget->showMaximized();
        connect(curve_treeWidget,SIGNAL(sig_del_curve_plot(int)),curve_plotWidget,SLOT(slot_del_curve_plot(int)));
		connect(curve_treeWidget, SIGNAL(sig_add_curve_plot(int, int)), curve_plotWidget, SLOT(slot_add_curve_plot(int, int)));
		connect(curve_treeWidget, SIGNAL(sig_del_curve_plot(int, int)), curve_plotWidget, SLOT(slot_del_curve_plot(int, int)));
		connect(curve_plotWidget, SIGNAL(sig_update_tree_curve_name(int, int, QString)), curve_treeWidget, SLOT(slot_update_tree_curve_name(int, int, QString)));
		connect(curve_plotWidget, SIGNAL(sig_finish_aniThread(bool)), this, SLOT(slot_finish_aniThread(bool)));
		connect(curve_plotWidget, SIGNAL(sig_updateZoomBar(bool)), this, SLOT(slot_updateZoomBar(bool)));
        
    }
    QTreeWidgetItem *tep_item=curve_treeWidget->curveTree->topLevelItem(0);
    curve_treeWidget->curveTree->setCurrentItem(tep_item->child(0));
	initWindow(); 
}

void curve_MainWindow::initWindow()
{
	if (column_dataWidget->get_column_dataForm()->curve_data_ptr == NULL)
		column_dataWidget->get_column_dataForm()->curve_data_ptr = &curve_data;
    if(curve_plotWidget->curve_data_ptr==NULL)
        curve_plotWidget->curve_data_ptr=&curve_data;
	if (curve_treeWidget->curve_data_ptr == NULL)
		curve_treeWidget->curve_data_ptr = &curve_data;
    curve_plotWidget->initPlot();
    curve_treeWidget->initTree();
	column_dataWidget->get_column_dataForm()->slot_update_clear_data();
}

void curve_MainWindow::openFile()
{
	curve_treeWidget->slot_addFile();
}

void curve_MainWindow::slot_exitSoftware()
{
	if (curve_plotWidget != NULL)
	{
		delete curve_plotWidget;
		curve_plotWidget = NULL;
	}
	if (column_dataWidget != NULL)
	{
		delete column_dataWidget;
		column_dataWidget = NULL;
	}
	if (curve_treeWidget != NULL)
	{
		delete curve_treeWidget;
		curve_treeWidget = NULL;
	}
    close();
}

void curve_MainWindow::creatDocWindows()
{
    if(!flag_DockWin)
    {
        setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
        setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

        setDockNestingEnabled(true);

        curve_treeWidget = new CurveTreeDockWidget(this);
        curve_treeWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        addDockWidget(Qt::LeftDockWidgetArea, curve_treeWidget);
        viewMenu->addAction(curve_treeWidget->toggleViewAction());
        curve_treeWidget->curve_data_ptr=&curve_data;

        column_dataWidget = new ColumnDataDockWidget(this);
        addDockWidget(Qt::BottomDockWidgetArea, column_dataWidget);
        viewMenu->addAction(column_dataWidget->toggleViewAction());
        column_dataWidget->get_column_dataForm()->curve_data_ptr=&curve_data;
		connect(curve_treeWidget, SIGNAL(sig_console_file_data_val_show(int)), column_dataWidget, SLOT(slot_console_file_data_val_show(int)));
		connect(curve_treeWidget, SIGNAL(sig_console_file_data_val_show_from_curveData(int, QVector<int>)), column_dataWidget, SLOT(slot_console_file_data_val_show_from_curveData(int, QVector<int>)));//
		connect(column_dataWidget->get_column_dataForm(), SIGNAL(sig_add_curve(int, int)), curve_treeWidget, SLOT(slot_add_curve(int, int)));
		connect(column_dataWidget->get_column_dataForm(), SIGNAL(sig_del_curve(int, int)), curve_treeWidget, SLOT(slot_del_curve(int, int)));
        connect(this,SIGNAL(sig_update_curveTree()),curve_treeWidget,SLOT(slot_update_curveTree()));
		connect(curve_treeWidget, SIGNAL(sig_update_clear_console_data()), column_dataWidget->get_column_dataForm(), SLOT(slot_update_clear_data()));//
        flag_DockWin=true;
    }
    else
    {
        curve_treeWidget->show();
        column_dataWidget->show();
        curve_treeWidget->initTree();
    }
    //curve_treeWidget->projectTreeItemClicked(curve_treeWidget->curve_project_item,0);
}

void curve_MainWindow::initPlotData()
{
	curve_data.glo_curve_data.file_num = 0;
	curve_data.glo_curve_data.backColor = QColor(255, 255, 255);
	curve_data.glo_curve_data.flag_legend = plotProp_legendPostion::none;
	curve_data.glo_curve_data.flag_grid = true;
	curve_data.glo_curve_data.curve_fileNameList.clear();
	curve_data.glo_curve_data.num_axis = 1;
	for (int i = 0; i < MAX_AXIS_NUM; i++)
	{
		curve_data.glo_curve_data.glo_xAxis_prop_data[i].axis_name = QString("X%1").arg(i + 1);
		curve_data.glo_curve_data.glo_xAxis_prop_data[i].axis_dataSource = plotProp_dataSource::Index;
		curve_data.glo_curve_data.glo_xAxis_prop_data[i].axis_fileColumn = -1;
		curve_data.glo_curve_data.glo_xAxis_prop_data[i].axis_fileIndex = -1;
		curve_data.glo_curve_data.glo_xAxis_prop_data[i].axis_font.font_type = plotProp_fontType::Arial;
		curve_data.glo_curve_data.glo_xAxis_prop_data[i].axis_font.font_size = 8;
		curve_data.glo_curve_data.glo_xAxis_prop_data[i].axis_font.font_color = QColor(0, 0, 0);
		curve_data.glo_curve_data.glo_xAxis_prop_data[i].axis_range[0] = 0;
		curve_data.glo_curve_data.glo_xAxis_prop_data[i].axis_range[1] = 0;

		curve_data.glo_curve_data.glo_yAxis_prop_data[i].axis_name = QString("Y%1").arg(i + 1);
		curve_data.glo_curve_data.glo_yAxis_prop_data[i].axis_font.font_type = plotProp_fontType::Arial;
		curve_data.glo_curve_data.glo_yAxis_prop_data[i].axis_font.font_size = 8;
		curve_data.glo_curve_data.glo_yAxis_prop_data[i].axis_font.font_color = QColor(0, 0, 0);
	}
	//QTextCodec *codec = QTextCodec::codecForName("gbk");
	//curve_data.glo_curve_data.title = codec->toUnicode(QString(tr("curve_title")).toStdString().data());
	curve_data.glo_curve_data.title = tr("curve_title");
	curve_data.glo_curve_data.title_font.font_type = plotProp_fontType::Arial;
    curve_data.glo_curve_data.title_font.font_size=8;
	curve_data.glo_curve_data.title_font.font_color = QColor(0, 0, 0);
    
    curve_data.curve_file_list.clear();
	curve_data.curveList.clear();
}

void curve_MainWindow::RunScript(QString fileName)
{
	CurveScriptHandler handler(this, fileName);
	flag_script_running = true;
	handler.Handle();
	flag_script_running = false;
}

void curve_MainWindow::slot_runScript()
{
	QString title = tr("Run Script");
	QString filter = tr("Script(*.curve_scr)");
	QString path = QFileDialog::getOpenFileName(this, title, "", filter);
	if (path.isEmpty()) 
		return;
	RunScript(path);
}

void curve_MainWindow::slot_editScript()
{
	QString tep_filename = QFileDialog::getSaveFileName(this, tr("Save Script"), "tep_Script", "Script(*.curve_scr)");
	if (tep_filename == "")
		return;
	editScript(tep_filename);
}

void curve_MainWindow::editScript_start(QString fileName)
{
	QFile tep_file1(fileName);
	if (tep_file1.open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		QTextStream stream(&tep_file1);
		stream << "#The script file is build " + QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss") << endl;
		tep_file1.close();
	}
}

void curve_MainWindow::editScript_end(QString fileName)
{
	QFile tep_file1(fileName);
	if (tep_file1.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		QTextStream stream(&tep_file1);
		stream << "Replot()" << endl;
		stream << "#The script file is over! " << endl;
		tep_file1.close();
	}
}

void curve_MainWindow::editScript(QString tep_filename)
{
	QString command_line;
	if (tep_filename.isNull())
		return;
	editScript_start(tep_filename);
	editScript_GlobalProp(tep_filename);
	editScript_Curve(tep_filename);
	editScript_end(tep_filename);
}

void curve_MainWindow::editScript_GlobalProp(QString fileName)
{
	QFile tep_file1(fileName);
	if (tep_file1.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		QTextStream stream(&tep_file1);
		QString command_line;
		command_line = QString("# Plot Global Property Setting");
		stream << command_line << endl;
		command_line = QString("Plot_title(%1)").arg(curve_data.glo_curve_data.title);
		stream << command_line << endl;
		QColor tep_color = curve_data.glo_curve_data.title_font.font_color;
		command_line = QString("Plot_titleFontColor(%1,%2,%3)").arg(tep_color.red()).arg(tep_color.green()).arg(tep_color.blue());
		stream << command_line << endl;
		command_line = QString("Plot_titleFontSize(%1)").arg(curve_data.glo_curve_data.title_font.font_size);
		stream << command_line << endl;
		command_line = QString("Plot_titleFontType(%1)").arg(curve_data.glo_curve_data.title_font.font_type);
		stream << command_line << endl;
		tep_color = curve_data.glo_curve_data.backColor;
		command_line = QString("Plot_backgroundColor(%1,%2,%3)").arg(tep_color.red()).arg(tep_color.green()).arg(tep_color.blue());
		stream << command_line << endl;
		if (curve_data.glo_curve_data.flag_grid)
			command_line = QString("Plot_grid(%1)").arg(1);
		else 
			command_line = QString("Plot_grid(%1)").arg(0);
		stream << command_line << endl;
		if (curve_data.glo_curve_data.flag_legend)
			command_line = QString("Plot_legend(%1)").arg(1);
		else
			command_line = QString("Plot_legend(%1)").arg(0);
		stream << command_line << endl;
		command_line = QString("Plot_numAxis(%1)").arg(curve_data.glo_curve_data.num_axis);
		stream << command_line << endl;
		QString xAxis_str="X";
		QString yAxis_str = "Y";
		for (int i = 0; i < curve_data.glo_curve_data.num_axis; i++)
		{
			if (i == 1)
			{
				xAxis_str += "2";
				yAxis_str += "2";
			}
			command_line = QString("Plot_axisName(%1,%2)").arg(xAxis_str).arg(curve_data.glo_curve_data.glo_xAxis_prop_data[i].axis_name);
			stream << command_line << endl;
			tep_color = curve_data.glo_curve_data.glo_xAxis_prop_data[i].axis_font.font_color;
			command_line = QString("Plot_axisFontColor(%1,%2,%3,%4)").arg(xAxis_str).arg(tep_color.red()).arg(tep_color.green()).arg(tep_color.blue());
			stream << command_line << endl;
			command_line = QString("Plot_axisFontSize(%1,%2)").arg(xAxis_str).arg(curve_data.glo_curve_data.glo_xAxis_prop_data[i].axis_font.font_size);
			stream << command_line << endl;
			command_line = QString("Plot_axisFontType(%1,%2)").arg(xAxis_str).arg(curve_data.glo_curve_data.glo_xAxis_prop_data[i].axis_font.font_type);
			stream << command_line << endl;
			command_line = QString("Plot_axisDataSource(%1,%2)").arg(xAxis_str).arg(curve_data.glo_curve_data.glo_xAxis_prop_data[i].axis_dataSource);
			stream << command_line << endl;
			if (curve_data.glo_curve_data.glo_xAxis_prop_data[i].axis_dataSource == plotProp_dataSource::File)
			{
				QString tep_filename = curve_data.curve_file_list.at(curve_data.glo_curve_data.glo_xAxis_prop_data[i].axis_fileIndex).file_prop.Axis_filename;
				command_line = QString("Plot_axisFileName(%1,%2)").arg(xAxis_str).arg(tep_filename);
				stream << command_line << endl;
				command_line = QString("Plot_axisColumnIndex(%1,%2)").arg(xAxis_str).arg(curve_data.glo_curve_data.glo_xAxis_prop_data[i].axis_fileColumn);
				stream << command_line << endl;
			}
			command_line = QString("Plot_axisrange(%1,%2,%3)").arg(xAxis_str).arg(curve_data.glo_curve_data.glo_xAxis_prop_data[i].axis_range[0], curve_data.glo_curve_data.glo_xAxis_prop_data[i].axis_range[1]);
			stream << command_line << endl;
			
			command_line = QString("Plot_axisName(%1,%2)").arg(yAxis_str).arg(curve_data.glo_curve_data.glo_yAxis_prop_data[i].axis_name);
			stream << command_line << endl;
			tep_color = curve_data.glo_curve_data.glo_yAxis_prop_data[i].axis_font.font_color;
			command_line = QString("Plot_axisFontColor(%1,%2,%3,%4)").arg(yAxis_str).arg(tep_color.red()).arg(tep_color.green()).arg(tep_color.blue());
			stream << command_line << endl;
			command_line = QString("Plot_axisFontSize(%1,%2)").arg(yAxis_str).arg(curve_data.glo_curve_data.glo_yAxis_prop_data[i].axis_font.font_size);
			stream << command_line << endl;
			command_line = QString("Plot_axisFontType(%1,%2)").arg(yAxis_str).arg(curve_data.glo_curve_data.glo_yAxis_prop_data[i].axis_font.font_type);
			stream << command_line << endl;
			command_line = QString("Plot_axisRange(%1,%2,%3)").arg(yAxis_str).arg(curve_data.glo_curve_data.glo_yAxis_prop_data[i].axis_range[0], curve_data.glo_curve_data.glo_yAxis_prop_data[i].axis_range[1]);
			stream << command_line << endl;
		}
		tep_file1.close();
	}
}

void curve_MainWindow::editScript_Curve(QString fileName)
{
	QFile tep_file1(fileName);
	if (tep_file1.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		QColor tep_color;
		QTextStream stream(&tep_file1);
		QString command_line;
		int num_files = curve_data.curve_file_list.count();
		command_line = QString("# Curve Setting");
		stream << command_line << endl;
		for (int i = 0; i < num_files; i++)
		{
			curve_file_data tep_file_data = curve_data.curve_file_list.at(i);
			command_line = QString("openFile(%1)").arg(tep_file_data.file_prop.Axis_filename);
			stream << command_line << endl;
			int num_lines = tep_file_data.line_data.count();
			for (int j = 0; j < num_lines; j++)
			{
				command_line = QString("# Curve Property Setting");
				stream << command_line << endl;
				curve_line_data tep_line_data=tep_file_data.line_data.at(j);
				command_line = QString("addCurve(%1,%2)").arg(tep_file_data.file_prop.Axis_filename).arg(tep_line_data.yAxis_column);
				stream << command_line << endl;
				//curve_prop
				tep_color = tep_line_data.curve_color;
				command_line = QString("Curve_color(%1,%2,%3,%4,%5)").arg(tep_file_data.file_prop.Axis_filename).arg(tep_line_data.yAxis_column).arg(tep_color.red()).arg(tep_color.green()).arg(tep_color.blue());
				stream << command_line << endl;		
				command_line = QString("Curve_lineType(%1,%2,%3)").arg(tep_file_data.file_prop.Axis_filename).arg(tep_line_data.yAxis_column).arg(tep_line_data.curve_lineType);
				stream << command_line << endl;
				command_line = QString("Curve_lineWidth(%1,%2,%3)").arg(tep_file_data.file_prop.Axis_filename).arg(tep_line_data.yAxis_column).arg(tep_line_data.curve_lineWidth);
				stream << command_line << endl;
				command_line = QString("Curve_showPoint(%1,%2,%3)").arg(tep_file_data.file_prop.Axis_filename).arg(tep_line_data.yAxis_column).arg(tep_line_data.flag_showPoint);
				stream << command_line << endl;
				command_line = QString("Curve_name(%1,%2,%3)").arg(tep_file_data.file_prop.Axis_filename).arg(tep_line_data.yAxis_column).arg(tep_line_data.yAxis_name);
				stream << command_line << endl;
				command_line = QString("Curve_axisIndex(%1,%2,%3)").arg(tep_file_data.file_prop.Axis_filename).arg(tep_line_data.yAxis_column).arg(tep_line_data.axis_index);
				stream << command_line << endl;	

				tep_color = tep_line_data.aniPoint_color;
				command_line = QString("AniPoint_color(%1,%2,%3,%4,%5)").arg(tep_file_data.file_prop.Axis_filename).arg(tep_line_data.yAxis_column).arg(tep_color.red()).arg(tep_color.green()).arg(tep_color.blue());
				stream << command_line << endl;
				command_line = QString("AniPoint_type(%1,%2,%3)").arg(tep_file_data.file_prop.Axis_filename).arg(tep_line_data.yAxis_column).arg(tep_line_data.aniPoint_type);
				stream << command_line << endl;
			}
		}
		tep_file1.close();
	}
}

bool  curve_MainWindow::script_openFile(QString args)
{
	if (get_curve_treeWidget()->readDataFile(args))
	{
		return true;
	}
	else
		return false;
}

void curve_MainWindow::script_addCurve(QString tep_filename, int tep_column_index)
{
	int num_files = curve_data.curve_file_list.count();
	int tep_file_index = -1;
	for (int i = 0; i < num_files; i++)
	{
		if (curve_data.curve_file_list.at(i).file_prop.Axis_filename.trimmed() == tep_filename.trimmed())
		{
			tep_file_index = i;
			break;
		}
	}
	if (tep_file_index == -1)
		return;
	if (tep_column_index >= curve_data.curve_file_list.at(tep_file_index).file_prop.num_column)
		return;
	QString tep_curve_id = QString("curve_%1_%2").arg(tep_file_index).arg(tep_column_index);
	if (curve_data.curveList.contains(tep_curve_id))
		return;
	curve_treeWidget->func_projectTreeItemClicked(tep_file_index);
	column_dataWidget->get_column_dataForm()->set_tableWidget_checked(tep_column_index, Qt::Checked);
	column_dataWidget->get_column_dataForm()->func_tableWidget_clicked(tep_file_index, tep_column_index);
}

QStringList curve_MainWindow::script_getFileColumnNameList(QString tep_filename)
{
	int num_files = curve_data.curve_file_list.count();
	int tep_file_index = -1;
	for (int i = 0; i < num_files; i++)
	{
		if (curve_data.curve_file_list.at(i).file_prop.Axis_filename.trimmed() == tep_filename.trimmed())
		{
			tep_file_index = i;
			break;
		}
	}
	QStringList tep_list;
	tep_list.clear();
	if (tep_file_index == -1)
		return tep_list;
	return curve_data.curve_file_list.at(tep_file_index).file_prop.columns_name_list;
}

curve_file_data curve_MainWindow::func_create_file_fromY(QVector<double> data_y, QString tep_filename)
{
	curve_file_data tep_file_data;
	tep_file_data.line_data.clear();
	tep_file_data.file_prop.Axis_filename = tep_filename;
	tep_file_data.file_prop.columns_name_list.clear();
	tep_file_data.file_prop.columns_name_list << "data_y";
	tep_file_data.file_prop.num_column = 1;
	if (data_y.count() < 1)
		return tep_file_data;
	tep_file_data.file_prop.num_point = data_y.count();
	tep_file_data.file_prop.column_val_data[0].resize(tep_file_data.file_prop.num_point);
	for (int i = 0; i < tep_file_data.file_prop.num_point; i++)
	{
		tep_file_data.file_prop.column_val_data[0][i] = data_y.at(i);
		if (i == 0)
		{
			tep_file_data.file_prop.val_min[0] = data_y.at(i);
			tep_file_data.file_prop.val_max[0] = data_y.at(i);
		}
		else
		{
			if (data_y.at(i)<tep_file_data.file_prop.val_min[0])
				tep_file_data.file_prop.val_min[0] = data_y.at(i);
			if (data_y.at(i)>tep_file_data.file_prop.val_max[0])
				tep_file_data.file_prop.val_max[0] = data_y.at(i);
		}
	}
	return tep_file_data;
}

void curve_MainWindow::script_addCurve(QVector<double> data_y, QString tep_filename)
{
	curve_file_data tep_file_data = func_create_file_fromY(data_y, tep_filename);
	int flag_pos = -1;
	for (int i = 0; i < curve_data.curve_file_list.count(); i++)
	{
		if (curve_data.curve_file_list.at(i).file_prop.Axis_filename.trimmed() == tep_filename.trimmed())
		{
			flag_pos = i;
			break;
		}
	}
	if (flag_pos >= 0)
		curve_data.curve_file_list.replace(flag_pos, tep_file_data);
	else
	{
		curve_data.curve_file_list.append(tep_file_data);
		curve_data.glo_curve_data.file_num++;
	}
	//add curve
	int num_files = curve_data.curve_file_list.count();
	int tep_file_index;
	if (flag_pos >= 0)
		tep_file_index = flag_pos;
	else
		tep_file_index = num_files - 1;
	curve_data.glo_curve_data.glo_xAxis_prop_data[0].axis_dataSource = plotProp_dataSource::Index;
	curve_data.glo_curve_data.glo_xAxis_prop_data[0].axis_fileIndex = -1;
	curve_data.glo_curve_data.glo_xAxis_prop_data[0].axis_fileColumn = -1;
	curve_treeWidget->addFile_from_array(tep_filename);
	curve_treeWidget->func_projectTreeItemClicked(tep_file_index);
	column_dataWidget->get_column_dataForm()->set_tableWidget_checked(0, Qt::Checked);
	column_dataWidget->get_column_dataForm()->func_tableWidget_clicked(tep_file_index, 0);
}

curve_file_data curve_MainWindow::func_create_file_fromXY(QVector<double> data_x, QVector<double> data_y, QString tep_filename)
{
	curve_file_data tep_file_data;
	tep_file_data.line_data.clear();
	tep_file_data.file_prop.Axis_filename = tep_filename;
	tep_file_data.file_prop.columns_name_list.clear();
	tep_file_data.file_prop.columns_name_list << "data_x" << "data_y";
	tep_file_data.file_prop.num_column = 2;
	if (data_x.count() >= data_y.count())
		tep_file_data.file_prop.num_point = data_y.count();
	else
		tep_file_data.file_prop.num_point = data_x.count();
	//	if (tep_file_data.file_prop.num_point<2)
	//		return;
	tep_file_data.file_prop.column_val_data[0].resize(tep_file_data.file_prop.num_point);
	tep_file_data.file_prop.column_val_data[1].resize(tep_file_data.file_prop.num_point);
	for (int i = 0; i < tep_file_data.file_prop.num_point; i++)
	{
		tep_file_data.file_prop.column_val_data[0][i] = data_x.at(i);
		tep_file_data.file_prop.column_val_data[1][i] = data_y.at(i);
		if (i == 0)
		{
			tep_file_data.file_prop.val_min[0] = data_x.at(i);
			tep_file_data.file_prop.val_max[0] = data_x.at(i);
			tep_file_data.file_prop.val_min[1] = data_y.at(i);
			tep_file_data.file_prop.val_max[1] = data_y.at(i);
		}
		else
		{
			if (data_x.at(i)<tep_file_data.file_prop.val_min[0])
				tep_file_data.file_prop.val_min[0] = data_x.at(i);
			if (data_x.at(i)>tep_file_data.file_prop.val_max[0])
				tep_file_data.file_prop.val_max[0] = data_x.at(i);
			if (data_y.at(i)<tep_file_data.file_prop.val_min[1])
				tep_file_data.file_prop.val_min[1] = data_y.at(i);
			if (data_y.at(i)>tep_file_data.file_prop.val_max[1])
				tep_file_data.file_prop.val_max[1] = data_y.at(i);
		}
	}
	return tep_file_data;
}

void curve_MainWindow::script_addCurve(QVector<double> data_x, QVector<double> data_y,QString tep_filename)
{
	curve_file_data tep_file_data = func_create_file_fromXY(data_x,data_y,tep_filename);
	int flag_pos =-1;
	for (int i = 0; i < curve_data.curve_file_list.count(); i++)
	{
		if (curve_data.curve_file_list.at(i).file_prop.Axis_filename.trimmed() == tep_filename.trimmed())
		{
			flag_pos = i;
			break;
		}
	}
	if (flag_pos>=0)
		curve_data.curve_file_list.replace(flag_pos,tep_file_data);
	else
	{
		curve_data.curve_file_list.append(tep_file_data);
		curve_data.glo_curve_data.file_num++;
	}
	//add curve
	int num_files = curve_data.curve_file_list.count();
	int tep_file_index;
	if (flag_pos >= 0)
		tep_file_index = flag_pos;
	else
		tep_file_index= num_files - 1;
	curve_data.glo_curve_data.glo_xAxis_prop_data[0].axis_dataSource = plotProp_dataSource::File;
	curve_data.glo_curve_data.glo_xAxis_prop_data[0].axis_fileIndex = tep_file_index;
	curve_data.glo_curve_data.glo_xAxis_prop_data[0].axis_fileColumn = 0;
	curve_treeWidget->addFile_from_array(tep_filename);
	curve_treeWidget->func_projectTreeItemClicked(tep_file_index);
	column_dataWidget->get_column_dataForm()->set_tableWidget_checked(1, Qt::Checked);
	column_dataWidget->get_column_dataForm()->func_tableWidget_clicked(tep_file_index,1);
}

//void curve_MainWindow::script_update_curve_data(QString tep_curve_id, double val_x, double val_y)
void curve_MainWindow::script_updateCurveData(int tep_file_index,int tep_column_index, double val_x, double val_y)
{
	curve_plotWidget->slot_updateCurveData(tep_file_index,tep_column_index, val_x, val_y);
}

void curve_MainWindow::script_updateCurveData(QVector<double> data_x, QVector<double> data_y, QString tep_filename)
{
	curve_plotWidget->slot_updateCurveData(data_x, data_y,tep_filename);
}

void curve_MainWindow::script_updateCurveData(QVector<double> data_y, QString tep_filename)
{
	curve_plotWidget->slot_updateCurveData(data_y, tep_filename);
}

void curve_MainWindow::script_delCurve(QString tep_filename, int tep_column_index)
{
	int num_files = curve_data.curve_file_list.count();
	int tep_file_index = -1;
	for (int i = 0; i < num_files; i++)
	{
		if (curve_data.curve_file_list.at(i).file_prop.Axis_filename.trimmed() == tep_filename.trimmed())
		{
			tep_file_index = i;
			break;
		}
	}
	if (tep_file_index == -1)
		return;
	if (tep_column_index >= curve_data.curve_file_list.at(tep_file_index).file_prop.num_column)
		return;
	curve_treeWidget->func_projectTreeItemClicked(tep_file_index);
	column_dataWidget->get_column_dataForm()->set_tableWidget_checked(tep_column_index, Qt::Unchecked);
	column_dataWidget->get_column_dataForm()->func_tableWidget_clicked(tep_file_index, tep_column_index);
}

void curve_MainWindow::script_delAllCurves()
{
	curve_treeWidget->slot_delAllFiles();
	curve_plotWidget->func_clear_plot();
	/*int num_files = curve_data.curve_file_list.count();
	int tep_file_index = -1;
	int tep_column_index = -1;
	for (int i = 0; i < num_files; i++)
	{
		curve_file_data tep_data = curve_data.curve_file_list.at(i);
		int num_lines = tep_data.line_data.count();
		for (int j = 0; j < num_lines; j++)
		{
			tep_column_index = tep_data.line_data.at(num_lines-j-1).yAxis_column;
			script_delCurve(tep_data.file_prop.Axis_filename, tep_column_index);
		}
	}
	curve_treeWidget->slot_delAllFiles();*/
}

void curve_MainWindow::script_Plot_title(QString name)
{
	curve_data.glo_curve_data.title = name;
}

void curve_MainWindow::script_Plot_titleFontColor(QColor color)
{
	curve_data.glo_curve_data.title_font.font_color = color;
}

void curve_MainWindow::script_Plot_titleFontSize(int fontSize)
{
	curve_data.glo_curve_data.title_font.font_size = fontSize;
}

void curve_MainWindow::script_Plot_titleFontType(plotProp_fontType fontType)
{
	curve_data.glo_curve_data.title_font.font_type = fontType;
}

void curve_MainWindow::script_Plot_backgroundColor(QColor color)
{
	curve_data.glo_curve_data.backColor = color;
}

void curve_MainWindow::script_Plot_grid(bool flag_grid)
{
	curve_data.glo_curve_data.flag_grid = flag_grid;
}

void curve_MainWindow::script_Plot_legend(plotProp_legendPostion flag_legend)
{
	curve_data.glo_curve_data.flag_legend = flag_legend;
}

void curve_MainWindow::script_Plot_numAxis(int num_axis)
{
	curve_data.glo_curve_data.num_axis = num_axis;
	//curve_plotWidget->slot_setAxisNum(num_axis);
}

void curve_MainWindow::script_Plot_axisName(QString axis, QString name)
{
	global_plot_axis_prop* axisPropPtr=NULL;
	if (axis.toLower() == "x")
	{
		axisPropPtr = &(curve_data.glo_curve_data.glo_xAxis_prop_data[0]);
	}
	else if (axis.toLower() == "x2")
	{
		axisPropPtr = &(curve_data.glo_curve_data.glo_xAxis_prop_data[1]);
	}
	else if (axis.toLower() == "y")
	{
		axisPropPtr = &(curve_data.glo_curve_data.glo_yAxis_prop_data[0]);
	}
	else if (axis.toLower() == "y2")
	{
		axisPropPtr = &(curve_data.glo_curve_data.glo_yAxis_prop_data[1]);
	}
	axisPropPtr->axis_name = name;
	//curve_plotWidget->slot_setAxisName(axis);
}

void curve_MainWindow::script_Plot_axisFontColor(QString axis, QColor color)
{
	global_plot_axis_prop* axisPropPtr = NULL;
	if (axis.toLower() == "x")
	{
		axisPropPtr = &(curve_data.glo_curve_data.glo_xAxis_prop_data[0]);
	}
	else if (axis.toLower() == "x2")
	{
		axisPropPtr = &(curve_data.glo_curve_data.glo_xAxis_prop_data[1]);
	}
	else if (axis.toLower() == "y")
	{
		axisPropPtr = &(curve_data.glo_curve_data.glo_yAxis_prop_data[0]);
	}
	else if (axis.toLower() == "y2")
	{
		axisPropPtr = &(curve_data.glo_curve_data.glo_yAxis_prop_data[1]);
	}
	axisPropPtr->axis_font.font_color = color;
	//curve_plotWidget->slot_axis_font(axis);
}

void curve_MainWindow::script_Plot_axisFontSize(QString axis, int fontSize)
{
	global_plot_axis_prop* axisPropPtr = NULL;
	if (axis.toLower() == "x")
	{
		axisPropPtr = &(curve_data.glo_curve_data.glo_xAxis_prop_data[0]);
	}
	else if (axis.toLower() == "x2")
	{
		axisPropPtr = &(curve_data.glo_curve_data.glo_xAxis_prop_data[1]);
	}
	else if (axis.toLower() == "y")
	{
		axisPropPtr = &(curve_data.glo_curve_data.glo_yAxis_prop_data[0]);
	}
	else if (axis.toLower() == "y2")
	{
		axisPropPtr = &(curve_data.glo_curve_data.glo_yAxis_prop_data[1]);
	}
	axisPropPtr->axis_font.font_size = fontSize;
	//curve_plotWidget->slot_axis_font(axis);
}

void curve_MainWindow::script_Plot_axisFontType(QString axis, plotProp_fontType fontType)
{
	global_plot_axis_prop* axisPropPtr = NULL;
	if (axis.toLower() == "x")
	{
		axisPropPtr = &(curve_data.glo_curve_data.glo_xAxis_prop_data[0]);
	}
	else if (axis.toLower() == "x2")
	{
		axisPropPtr = &(curve_data.glo_curve_data.glo_xAxis_prop_data[1]);
	}
	else if (axis.toLower() == "y")
	{
		axisPropPtr = &(curve_data.glo_curve_data.glo_yAxis_prop_data[0]);
	}
	else if (axis.toLower() == "y2")
	{
		axisPropPtr = &(curve_data.glo_curve_data.glo_yAxis_prop_data[1]);
	}
	axisPropPtr->axis_font.font_type= fontType;
	//curve_plotWidget->slot_axis_font(axis);
}

void curve_MainWindow::script_Plot_axisDataSource(QString axis, plotProp_dataSource dataSource)
{
	if (axis.toLower() == "x")
		curve_data.glo_curve_data.glo_xAxis_prop_data[0].axis_dataSource = dataSource;
	else if (axis.toLower() == "x2")
		curve_data.glo_curve_data.glo_xAxis_prop_data[1].axis_dataSource = dataSource;
}

void curve_MainWindow::script_Plot_axisFileName(QString axis, QString tep_filename)
{
	int num_files = curve_data.curve_file_list.count();
	int index = -1;
	for (int i = 0; i < num_files; i++)
	{
		if (curve_data.curve_file_list.at(i).file_prop.Axis_filename == tep_filename)
		{
			index = i;
			break;
		}
	}
	if (axis.toLower() == "x")
		curve_data.glo_curve_data.glo_xAxis_prop_data[0].axis_fileIndex = index;
	else if (axis.toLower() == "x2")
		curve_data.glo_curve_data.glo_xAxis_prop_data[1].axis_fileIndex = index;
}

void curve_MainWindow::script_Plot_axisFileColumn(QString axis, int tep_column_index)
{
	if (axis.toLower() == "x")
		curve_data.glo_curve_data.glo_xAxis_prop_data[0].axis_fileColumn = tep_column_index;
	else if (axis.toLower() == "x2")
		curve_data.glo_curve_data.glo_xAxis_prop_data[1].axis_fileColumn = tep_column_index;
}

void curve_MainWindow::script_Plot_axisrange(QString axis, double range[2])
{
	if (axis.trimmed().toLower() == "x")
	{
		curve_data.glo_curve_data.glo_xAxis_prop_data[0].axis_range[0] = range[0];
		curve_data.glo_curve_data.glo_xAxis_prop_data[0].axis_range[1] = range[1];
	}
	else if (axis.trimmed().toLower() == "x2")
	{
		curve_data.glo_curve_data.glo_xAxis_prop_data[1].axis_range[0] = range[0];
		curve_data.glo_curve_data.glo_xAxis_prop_data[1].axis_range[1] = range[1];
	}
	else if (axis.trimmed().toLower() == "y")
	{
		curve_data.glo_curve_data.glo_yAxis_prop_data[0].axis_range[0] = range[0];
		curve_data.glo_curve_data.glo_yAxis_prop_data[0].axis_range[1] = range[1];
	}
	else if (axis.trimmed().toLower() == "y2")
	{
		curve_data.glo_curve_data.glo_yAxis_prop_data[1].axis_range[0] = range[0];
		curve_data.glo_curve_data.glo_yAxis_prop_data[1].axis_range[1] = range[1];
	}
	get_curve_plotWidget()->slot_setAxisRange(axis);
}

void curve_MainWindow::script_Curve_color(QString tep_fileName, int tep_column_index, QColor color)
{
	if (tep_column_index < 0)
		return;
	int num_files = curve_data.curve_file_list.count();
	int tep_file_index = -1;
	for (int i = 0; i < num_files; i++)
	{
		if (curve_data.curve_file_list.at(i).file_prop.Axis_filename.trimmed() == tep_fileName.trimmed())
		{
			tep_file_index = i;
			break;
		}
	}
	if (tep_file_index == -1)
		return;
	if (tep_column_index >= curve_data.curve_file_list.at(tep_file_index).file_prop.num_column)
		return;
	curve_file_data tep_file_data = curve_data.curve_file_list.at(tep_file_index);
	int num_lines = tep_file_data.line_data.count();
	for (int i = 0; i < num_lines; i++)
	{
		if (tep_file_data.line_data.at(i).yAxis_column == tep_column_index)
		{
			curve_line_data tep_line_data = tep_file_data.line_data.at(i);
			tep_line_data.curve_color = color;
			tep_file_data.line_data.replace(i, tep_line_data);
			curve_data.curve_file_list.replace(tep_file_index,tep_file_data);
			break;
		}
	}
}
curve_line_data curve_MainWindow::script_get_CurveProp(QString tep_fileName, int tep_column_index)
{
	curve_line_data tep_line_data;
	tep_line_data.axis_index = -1;
	if (tep_column_index < 0)
		return tep_line_data;
	int num_files = curve_data.curve_file_list.count();
	int tep_file_index = -1;
	for (int i = 0; i < num_files; i++)
	{
		if (curve_data.curve_file_list.at(i).file_prop.Axis_filename.trimmed() == tep_fileName.trimmed())
		{
			tep_file_index = i;
			break;
		}
	}
	if (tep_file_index == -1)
		return tep_line_data;
	if (tep_column_index >= curve_data.curve_file_list.at(tep_file_index).file_prop.num_column)
		return tep_line_data;
	curve_file_data tep_file_data = curve_data.curve_file_list.at(tep_file_index);
	int num_lines = tep_file_data.line_data.count();
	for (int i = 0; i < num_lines; i++)
	{
		if (tep_file_data.line_data.at(i).yAxis_column == tep_column_index)
		{
			tep_line_data = tep_file_data.line_data.at(i);
			break;
		}
	}
	return tep_line_data;
}
void curve_MainWindow::script_Curve_lineType(QString tep_fileName, int tep_column_index, Qt::PenStyle lineType)
{
	if (tep_column_index < 0)
		return;
	int num_files = curve_data.curve_file_list.count();
	int tep_file_index = -1;
	for (int i = 0; i < num_files; i++)
	{
		if (curve_data.curve_file_list.at(i).file_prop.Axis_filename.trimmed() == tep_fileName.trimmed())
		{
			tep_file_index = i;
			break;
		}
	}
	if (tep_file_index == -1)
		return;
	if (tep_column_index >= curve_data.curve_file_list.at(tep_file_index).file_prop.num_column)
		return;
	curve_file_data tep_file_data = curve_data.curve_file_list.at(tep_file_index);
	int num_lines = tep_file_data.line_data.count();
	for (int i = 0; i < num_lines; i++)
	{
		if (tep_file_data.line_data.at(i).yAxis_column == tep_column_index)
		{
			curve_line_data tep_line_data = tep_file_data.line_data.at(i);
			tep_line_data.curve_lineType = lineType;
			tep_file_data.line_data.replace(i, tep_line_data);
			curve_data.curve_file_list.replace(tep_file_index, tep_file_data);
			break;
		}
	}
}

void curve_MainWindow::script_Curve_lineWidth(QString tep_fileName, int tep_column_index, int lineWidth)
{
	if (tep_column_index < 0)
		return;
	int num_files = curve_data.curve_file_list.count();
	int tep_file_index = -1;
	for (int i = 0; i < num_files; i++)
	{
		if (curve_data.curve_file_list.at(i).file_prop.Axis_filename.trimmed() == tep_fileName.trimmed())
		{
			tep_file_index = i;
			break;
		}
	}
	if (tep_file_index == -1)
		return;
	if (tep_column_index >= curve_data.curve_file_list.at(tep_file_index).file_prop.num_column)
		return;
	curve_file_data tep_file_data = curve_data.curve_file_list.at(tep_file_index);
	int num_lines = tep_file_data.line_data.count();
	for (int i = 0; i < num_lines; i++)
	{
		if (tep_file_data.line_data.at(i).yAxis_column == tep_column_index)
		{
			curve_line_data tep_line_data = tep_file_data.line_data.at(i);
			tep_line_data.curve_lineWidth = lineWidth;
			tep_file_data.line_data.replace(i, tep_line_data);
			curve_data.curve_file_list.replace(tep_file_index, tep_file_data);
			break;
		}
	}
}

void curve_MainWindow::script_Curve_showPoint(QString tep_fileName, int tep_column_index, bool flag_point)
{
	if (tep_column_index < 0)
		return;
	int num_files = curve_data.curve_file_list.count();
	int tep_file_index = -1;
	for (int i = 0; i < num_files; i++)
	{
		if (curve_data.curve_file_list.at(i).file_prop.Axis_filename.trimmed() == tep_fileName.trimmed())
		{
			tep_file_index = i;
			break;
		}
	}
	if (tep_file_index == -1)
		return;
	if (tep_column_index >= curve_data.curve_file_list.at(tep_file_index).file_prop.num_column)
		return;
	curve_file_data tep_file_data = curve_data.curve_file_list.at(tep_file_index);
	int num_lines = tep_file_data.line_data.count();
	for (int i = 0; i < num_lines; i++)
	{
		if (tep_file_data.line_data.at(i).yAxis_column == tep_column_index)
		{
			curve_line_data tep_line_data = tep_file_data.line_data.at(i);
			tep_line_data.flag_showPoint = flag_point;
			tep_file_data.line_data.replace(i, tep_line_data);
			curve_data.curve_file_list.replace(tep_file_index, tep_file_data);
			break;
		}
	}
}

void curve_MainWindow::script_Curve_name(QString tep_fileName, int tep_column_index, QString name)
{
	if (tep_column_index < 0)
		return;
	int num_files = curve_data.curve_file_list.count();
	int tep_file_index = -1;
	for (int i = 0; i < num_files; i++)
	{
		if (curve_data.curve_file_list.at(i).file_prop.Axis_filename.trimmed() == tep_fileName.trimmed())
		{
			tep_file_index = i;
			break;
		}
	}
	if (tep_file_index == -1)
		return;
	if (tep_column_index >= curve_data.curve_file_list.at(tep_file_index).file_prop.num_column)
		return;
	curve_file_data tep_file_data = curve_data.curve_file_list.at(tep_file_index);
	int num_lines = tep_file_data.line_data.count();
	for (int i = 0; i < num_lines; i++)
	{
		if (tep_file_data.line_data.at(i).yAxis_column == tep_column_index)
		{
			curve_line_data tep_line_data = tep_file_data.line_data.at(i);
			tep_line_data.yAxis_name = name;
			tep_file_data.line_data.replace(i, tep_line_data);
			curve_data.curve_file_list.replace(tep_file_index, tep_file_data);
			break;
		}
	}
}

//void curve_MainWindow::script_UpdateCurvePlot(QString tep_fileName, int tep_column_index)
//{
//
//}

void curve_MainWindow::script_Curve_axisIndex(QString tep_fileName, int tep_column_index, int index)
{
	if (tep_column_index < 0)
		return;
	int num_files = curve_data.curve_file_list.count();
	int tep_file_index = -1;
	for (int i = 0; i < num_files; i++)
	{
		if (curve_data.curve_file_list.at(i).file_prop.Axis_filename.trimmed() == tep_fileName.trimmed())
		{
			tep_file_index = i;
			break;
		}
	}
	if (tep_file_index == -1)
		return;
	if (tep_column_index >= curve_data.curve_file_list.at(tep_file_index).file_prop.num_column)
		return;
	curve_file_data tep_file_data = curve_data.curve_file_list.at(tep_file_index);
	int num_lines = tep_file_data.line_data.count();
	for (int i = 0; i < num_lines; i++)
	{
		if (tep_file_data.line_data.at(i).yAxis_column == tep_column_index)
		{
			curve_line_data tep_line_data = tep_file_data.line_data.at(i);
			tep_line_data.axis_index = index;
			tep_file_data.line_data.replace(i, tep_line_data);
			curve_data.curve_file_list.replace(tep_file_index, tep_file_data);
			break;
		}
	}
}

void curve_MainWindow::script_update_replot()
{
	curve_plotWidget->slot_replot();
}

void curve_MainWindow::script_refreshCurveForm()
{
	curve_plotWidget->func_refreshPlotForm();
}

QToolBar* curve_MainWindow::script_get_fileToolBar()
{
	return fileToolBar;
}

QToolBar* curve_MainWindow::script_get_helpToolBar()
{
	return helpToolBar;
}

void curve_MainWindow::script_saveImage(bool flag_dlg, int wid, int hei, QString fileName)
{
	curve_plotWidget->func_saveImage(flag_dlg, wid, hei, fileName);
}

void curve_MainWindow::reTranslate()
{
	ui->retranslateUi(this);

	fileMenu->setTitle(tr("File"));
	viewMenu->setTitle(tr("View"));
	zoomMenu->setTitle(tr("Zoom"));
	toolMenu->setTitle(tr("Tool"));
	helpMenu->setTitle(tr("Help"));

	openAct->setText( tr("open"));
	openAct->setStatusTip(tr("Open an existing file."));
	savePngAct->setText(tr("save image"));
	savePngAct->setStatusTip(QStringLiteral("save as image."));
	runScriptAct->setText(tr("Run script"));
	runScriptAct->setStatusTip(tr("Run script file."));
	editScriptAct->setText(tr("Edit script"));
	editScriptAct->setStatusTip(tr("Generate script file."));
	exitAct->setText(tr("exit"));
	exitAct->setStatusTip(tr("Exit this software."));
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

	zoomXAct->setText(tr("XAxis zoom"));
	zoomXAct->setStatusTip(tr("X axis zoom"));
	zoomYAct->setText(tr("YAxis zoom"));
	zoomYAct->setStatusTip(tr("Y axis zoom"));
	/*zoomXYAct->setText(tr("XYAxis zoom"));
	zoomXYAct->setStatusTip(tr("XY axis zoom"));*/

	aniPointAct->setText(tr("point animate"));
	aniPointAct->setStatusTip(tr("point animate"));
	delAniPointAct->setText(tr("clear animate points"));
	delAniPointAct->setStatusTip(tr("celar all animate points"));

	curve_treeWidget->reTranslate();
	curve_plotWidget->reTranslate();
	column_dataWidget->reTranslate();
}
void curve_MainWindow::slot_animate()
{
	if (curve_data.curveList.count()<1)
		return;
	QString tep_name = aniPointAct->data().toString().trimmed();
	if (tep_name == "aniRun")
	{
		aniPointAct->setIcon(QIcon(":/images/stop.png"));
		aniPointAct->setData("aniStop");		
		ani_update_all_form(true);
		emit sig_CAE_curve_animate(1);
		get_curve_plotWidget()->slot_runAni();

	}
	else if (tep_name == "aniStop")
	{
		get_curve_plotWidget()->slot_stopAni();
		aniPointAct->setIcon(QIcon(":/images/run.png"));
		aniPointAct->setData("aniRun");	
		emit sig_CAE_curve_animate(0);
		ani_update_all_form(false);		
	}
}
void curve_MainWindow::slot_delAniPointAct()
{
	get_curve_plotWidget()->delAniPointCurves();
	
}
void curve_MainWindow::slot_finish_aniThread(bool flag_runing)
{
	aniPointAct->setIcon(QIcon(":/images/run.png"));
	aniPointAct->setData("aniRun");
	ani_update_all_form(false);
}
void curve_MainWindow::ani_update_all_form(bool flag_start)
{
	curve_treeWidget->setEnabled(!flag_start);
	column_dataWidget->setEnabled(!flag_start);
	openAct->setEnabled(!flag_start);
	savePngAct->setEnabled(!flag_start);
	runScriptAct->setEnabled(!flag_start);
	editScriptAct->setEnabled(!flag_start);
	actionChinese->setEnabled(!flag_start);
	actionEnglish->setEnabled(!flag_start);
	delAniPointAct->setEnabled(!flag_start);
}
bool curve_MainWindow::script_start_animate()
{
	if (curve_data.curveList.count()<1)
		return false;
	QString tep_name = aniPointAct->data().toString().trimmed();
	if (tep_name == "aniStop")
		return false;
	else if (tep_name == "aniRun")
	{
		aniPointAct->setIcon(QIcon(":/images/stop.png"));
		aniPointAct->setData("aniStop");
		ani_update_all_form(true);
		emit sig_CAE_curve_animate(1);
		get_curve_plotWidget()->slot_runAni();
		return true;
	}
	else
		return false;
}

bool curve_MainWindow::script_stop_animate()
{
	if (curve_data.curveList.count()<1)
		return true;
	QString tep_name = aniPointAct->data().toString().trimmed();
	if (tep_name == "aniRun")
		return true;
	else if (tep_name == "aniStop")
	{
		get_curve_plotWidget()->slot_stopAni();
		aniPointAct->setIcon(QIcon(":/images/run.png"));
		aniPointAct->setData("aniRun");
		emit sig_CAE_curve_animate(0);
		ani_update_all_form(false);
		return true;
	}
	else
		return false;
}
void curve_MainWindow::script_AniPoint_color(QString tep_fileName, int tep_column_index, QColor color)
{
	if (tep_column_index < 0)
		return;
	int num_files = curve_data.curve_file_list.count();
	int tep_file_index = -1;
	for (int i = 0; i < num_files; i++)
	{
		if (curve_data.curve_file_list.at(i).file_prop.Axis_filename.trimmed() == tep_fileName.trimmed())
		{
			tep_file_index = i;
			break;
		}
	}
	if (tep_file_index == -1)
		return;
	if (tep_column_index >= curve_data.curve_file_list.at(tep_file_index).file_prop.num_column)
		return;
	curve_file_data tep_file_data = curve_data.curve_file_list.at(tep_file_index);
	int num_lines = tep_file_data.line_data.count();
	for (int i = 0; i < num_lines; i++)
	{
		if (tep_file_data.line_data.at(i).yAxis_column == tep_column_index)
		{
			curve_line_data tep_line_data = tep_file_data.line_data.at(i);
			tep_line_data.aniPoint_color = color;
			tep_file_data.line_data.replace(i, tep_line_data);
			curve_data.curve_file_list.replace(tep_file_index, tep_file_data);
			break;
		}
	}
}
void curve_MainWindow::script_AniPoint_type(QString tep_fileName, int tep_column_index, aniPointType type)
{
	if (tep_column_index < 0)
		return;
	int num_files = curve_data.curve_file_list.count();
	int tep_file_index = -1;
	for (int i = 0; i < num_files; i++)
	{
		if (curve_data.curve_file_list.at(i).file_prop.Axis_filename.trimmed() == tep_fileName.trimmed())
		{
			tep_file_index = i;
			break;
		}
	}
	if (tep_file_index == -1)
		return;
	if (tep_column_index >= curve_data.curve_file_list.at(tep_file_index).file_prop.num_column)
		return;
	curve_file_data tep_file_data = curve_data.curve_file_list.at(tep_file_index);
	int num_lines = tep_file_data.line_data.count();
	for (int i = 0; i < num_lines; i++)
	{
		if (tep_file_data.line_data.at(i).yAxis_column == tep_column_index)
		{
			curve_line_data tep_line_data = tep_file_data.line_data.at(i);
			tep_line_data.aniPoint_type = type;
			tep_file_data.line_data.replace(i, tep_line_data);
			curve_data.curve_file_list.replace(tep_file_index, tep_file_data);
			break;
		}
	}
}
void curve_MainWindow::script_AniPoint_del()
{
	slot_delAniPointAct();
}
void curve_MainWindow::slot_zoomXAct(bool val)
{
	if (val)
	{
		get_curve_plotWidget()->func_zoomAct("x");
		if (zoomYAct->isChecked())
		{
			zoomYAct->setChecked(false);
		}
	}
	else
		get_curve_plotWidget()->func_zoomAct("xy");
}
void curve_MainWindow::slot_zoomYAct(bool val)
{
	if (val)
	{
		get_curve_plotWidget()->func_zoomAct("y");
		if (zoomXAct->isChecked())
		{
			zoomXAct->setChecked(false);
		}
	}
	else
		get_curve_plotWidget()->func_zoomAct("xy");
}
void curve_MainWindow::slot_updateZoomBar(bool val)
{
	zoomXAct->setEnabled(val);
	zoomYAct->setEnabled(val);
}