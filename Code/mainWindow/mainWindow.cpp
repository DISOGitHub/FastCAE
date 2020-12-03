#include <QMdiSubWindow>
#include <QDebug>
#include <QList>
#include <QTreeWidgetItem>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QSignalMapper>
#include <QLabel>
#include "mainWindow.h"
#include "signalHandler.h"
#include "translator.h"
#include "SubWindowManager.h"
#include "ui_mainWindow.h"
#include "settings/busAPI.h"
#include "settings/GraphOption.h"
#include "moduleBase/ModuleType.h"
#include "moduleBase/ModuleBase.h"
#include "MainWidgets/ControlPanel.h"
#include "MainWidgets/messageWindow.h"
#include "MainWidgets/ProcessWindow.h"
#include "MainWidgets/projectSolveDialog.h"
#include "MainWidgets/DialogCreateSet.h"
#include "MainWidgets/DialogCreateGeoComponent.h"
#include "MainWidgets/DialogVTKTransform.h"
#include "MainWidgets/preWindow.h"
#include "GeometryWidgets/dialogSketchPlane.h"
#include "meshData/meshSingleton.h"
#include "meshData/meshKernal.h"
#include "geometry/geometryData.h"
#include "DialogAbout.h"
#include <QDesktopServices>
#include <QMessageBox>
#include <QApplication>
#include "IO/IOConfig.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/GlobalConfig.h"
#include "ConfigOptions/GeometryConfig.h"
#include "ConfigOptions/MeshConfig.h"
#include "ConfigOptions/PostConfig.h"
#include "ConfigOptions/ProjectTreeConfig.h"
#include "ModelData/modelDataBaseExtend.h"
#include "ModelData/modelDataSingleton.h"
#include "PluginManager/PluginManager.h"
#include "python/PyAgent.h"
#include "MainWindowPy.h"
#include "CustomizerHelper.h"
#include "SolverControl/DialogSolverManager.h"
#include "Gmsh/GmshModule.h"
#include "moduleBase/ThreadTaskManager.h"
#include "MainWidgets/DialogFilterMesh.h"

namespace GUI
{
	MainWindow::MainWindow() 
		:_ui(new Ui::MainWindow)
	{
		_ui->setupUi(this);
		this->setContextMenuPolicy(Qt::NoContextMenu);

		QString lang = Setting::BusAPI::instance()->getLanguage();

		registerMoudel();
		_signalHandler = new SignalHandler(this);
		_translator = new Translator();
		_subWindowManager = new SubWindowManager(this, _ui->mdiArea, _signalHandler,_controlPanel);
		connectSignals();
//		registerMoudel();

		_ui->mdiArea->setViewMode(QMdiArea::TabbedView);
		_ui->mdiArea->setTabPosition(QTabWidget::North);
		
		this->ChangeLanguage(lang);
		QString workingdir = Setting::BusAPI::instance()->getWorkingDir();
		if (workingdir.isEmpty()) setWorkingDir();

		Setting::BusAPI::instance()->setMainWindow(this);
		Setting::GraphOption* gp = Setting::BusAPI::instance()->getGraphOption();
		_ui->actionDisplayPoint->setChecked(gp->isShowGeoPoint());
		_ui->actionDisplayCurve->setChecked(gp->isShowGeoEdge());
		_ui->actionDisplayFace->setChecked(gp->isShowGeoSurface());
		this->setGeometryDisplay();
		updateRecentMenu();
		setCurrentFile("");
		
		this->startSketch(false);

		QString slogo = ConfigOption::ConfigOption::getInstance()->getGlobalConfig()->getLogo();
		this->setIcon(QApplication::applicationDirPath() + "/../ConfigFiles/icon/" + slogo);

		_signalHandler->updateActionsStates();
		MainWindowPy::init(this,_signalHandler);
		
		Gmsh::GmshModule::getInstance(this);
		
		this->showGraphRange(0, 0);

		Py::PythonAagent::getInstance()->initialize(this);
		Plugins::PluginManager::getInstance()->loadPlugs(this);

		_customizerHelper = new CustomizerHelper(this, _ui);
		_customizerHelper->registerInterface();

		_subWindowManager->openStartPage();
		isLoadRecordScripFile();
	}

	MainWindow::~MainWindow()
	{
		Py::PythonAagent::getInstance()->finalize();
		Plugins::PluginManager::getInstance()->releasePlugs();
		Gmsh::GmshModule::getInstance(this)->finalize();
	
		ModuleBase::ThreadTaskManager::getInstance()->clearThreadTaskList();

		if (_signalHandler != nullptr)             delete _signalHandler;
		if (_controlPanel != nullptr)              delete _controlPanel;
		if (_messageWindow != nullptr)             delete _messageWindow;
		if (_processWindow != nullptr)             delete _processWindow;
		if (_ui != nullptr)                        delete _ui;
		if (_translator != nullptr)                delete _translator;
		if (_viewSignalMapper != nullptr)          delete _viewSignalMapper;
		if (_selectSignalMapper != nullptr)        delete _selectSignalMapper;
		if (_displayModeSignalMapper != nullptr)   delete _displayModeSignalMapper;
		if (_recentFileMapper != nullptr)          delete _recentFileMapper;
		if (_recentMenu != nullptr)                delete _recentMenu;
		if (_graphRange != nullptr)                delete _graphRange;
		if (_customizerHelper != nullptr)          delete _customizerHelper;

	}

	void MainWindow::connectSignals()
	{
		connect(this, SIGNAL(closeMainWindow()), this, SLOT(closeWindow()));
		connect(this, SIGNAL(sendInfoToStatesBar(QString)), this, SLOT(setStatusBarInfo(QString)));
//		connect(this, SIGNAL(importMeshByNamesSig(QString)), this, SLOT(importMesh(QString)));
		connect(this, SIGNAL(importMeshDataSetSig(vtkDataSet*)), this, SLOT(importMeshDataset(vtkDataSet*)));
		connect(this, SIGNAL(printMessageSig(int, QString)), this, SLOT(printMessage(int, QString)));

		connect(_ui->actionNew, SIGNAL(triggered()), this, SLOT(on_actionNew()));
		connect(_ui->actionOpen, SIGNAL(triggered()), this, SLOT(on_actionOpen()));
		connect(_ui->actionSave, SIGNAL(triggered()), this, SLOT(on_actionSave()));
		connect(_ui->actionSaveAs, SIGNAL(triggered()), this, SLOT(on_actionSaveAs()));
		connect(_ui->actionWorkingDir, SIGNAL(triggered()), this, SLOT(setWorkingDir()));
		connect(_ui->actionImportMesh, SIGNAL(triggered()), this, SLOT(on_importMesh()));
		connect(_ui->actionExportMesh, SIGNAL(triggered()), this, SLOT(on_exportMesh()));
		connect(_ui->actionImportGeometry, SIGNAL(triggered()), this, SLOT(on_importGeometry()));
		connect(_ui->actionExportGeometry, SIGNAL(triggered()), this, SLOT(on_exportGeometry()));
		connect(_ui->actionClose, SIGNAL(triggered()), this, SLOT(closeWindow()));
		connect(_ui->actionSolver_Manager, SIGNAL(triggered()), this, SLOT(on_solverManager()));
		connect(_ui->actionSolve, SIGNAL(triggered()), this, SLOT(on_solve()));
		connect(_ui->actionSolve_Options, SIGNAL(triggered()), this, SLOT(on_solveOption()));
		connect(_ui->actionGraph_Options, SIGNAL(triggered()), this, SLOT(on_graphOption()));
		connect(_ui->actionAbout, SIGNAL(triggered()), this, SLOT(on_about()));
		connect(_ui->actionUser_Manual, SIGNAL(triggered()), this, SLOT(on_userManual()));
		connect(_ui->actionCreate_Set, SIGNAL(triggered()), this, SLOT(on_CreateSet()));
		connect(_ui->actionCreateGeoComponent, SIGNAL(triggered()), this, SLOT(on_CreateGeoComponent()));
		connect(_ui->actionSave_Script, SIGNAL(triggered()), this, SLOT(on_SaveScript()));
		connect(_ui->actionExecute_Script, SIGNAL(triggered()), this, SLOT(on_ExecuateScript()));
		connect(_ui->actionPluginManager, SIGNAL(triggered()), Plugins::PluginManager::getInstance(), SLOT(manage()));
		connect(_ui->actionFilterMesh, SIGNAL(triggered()), this, SLOT(on_FilterMesh()));
		connect(_ui->actionVTKTranslation, SIGNAL(triggered()), this, SLOT(on_VTKTranslation()));

		//设置视角
		_viewSignalMapper = new QSignalMapper(this);
		connect(_ui->actionFitView, SIGNAL(triggered()), _viewSignalMapper, SLOT(map()));
		connect(_ui->actionViewXPlus, SIGNAL(triggered()), _viewSignalMapper, SLOT(map()));
		connect(_ui->actionViewXMinus, SIGNAL(triggered()), _viewSignalMapper, SLOT(map()));
		connect(_ui->actionViewYPlus, SIGNAL(triggered()), _viewSignalMapper, SLOT(map()));
		connect(_ui->actionViewYMinus, SIGNAL(triggered()), _viewSignalMapper, SLOT(map()));
		connect(_ui->actionViewZPlus, SIGNAL(triggered()), _viewSignalMapper, SLOT(map()));
		connect(_ui->actionViewZMinus, SIGNAL(triggered()), _viewSignalMapper, SLOT(map()));
		_viewSignalMapper->setMapping(_ui->actionFitView, QString("fit"));
		_viewSignalMapper->setMapping(_ui->actionViewXPlus, QString("XPlus"));
		_viewSignalMapper->setMapping(_ui->actionViewXMinus, QString("XMinus"));
		_viewSignalMapper->setMapping(_ui->actionViewYPlus, QString("YPlus"));
		_viewSignalMapper->setMapping(_ui->actionViewYMinus, QString("YMinus"));
		_viewSignalMapper->setMapping(_ui->actionViewZPlus, QString("ZPlus"));
		_viewSignalMapper->setMapping(_ui->actionViewZMinus, QString("ZMinus"));
		connect(_viewSignalMapper, SIGNAL(mapped(QString)), _subWindowManager, SLOT(setView(QString)));
		//设置mesh选择模式
		_selectSignalMapper = new QSignalMapper(this);
		connect(_ui->actionSelectOff, SIGNAL(triggered()), _selectSignalMapper, SLOT(map()));
		//connect(_ui->actionSelectGeoBody, SIGNAL(triggered()), _selectSignalMapper, SLOT(map()));
		connect(_ui->actionSelectMeshNode, SIGNAL(triggered()), _selectSignalMapper, SLOT(map()));
		connect(_ui->actionSelectMeshCell, SIGNAL(triggered()), _selectSignalMapper, SLOT(map()));
		connect(_ui->actionBoxMeshNode, SIGNAL(triggered()), _selectSignalMapper, SLOT(map()));
		connect(_ui->actionBoxMeshCell, SIGNAL(triggered()), _selectSignalMapper, SLOT(map()));
		_selectSignalMapper->setMapping(_ui->actionSelectOff, (int)ModuleBase::SelectModel::None);
		//_selectSignalMapper->setMapping(_ui->actionSelectGeoBody, (int)ModuleBase::SelectModel::GeometryBody);
		_selectSignalMapper->setMapping(_ui->actionSelectMeshNode, (int)ModuleBase::SelectModel::MeshNode);
		_selectSignalMapper->setMapping(_ui->actionSelectMeshCell, (int)ModuleBase::SelectModel::MeshCell);
		_selectSignalMapper->setMapping(_ui->actionBoxMeshNode, (int)ModuleBase::SelectModel::BoxMeshNode);
		_selectSignalMapper->setMapping(_ui->actionBoxMeshCell, (int)ModuleBase::SelectModel::BoxMeshCell);
		connect(_selectSignalMapper, SIGNAL(mapped(int)), this, SIGNAL(selectModelChangedSig(int)));
		connect(_selectSignalMapper, SIGNAL(mapped(int)), this, SLOT(selectModelChanged(int)));
		//网格显示模式
		_displayModeSignalMapper = new QSignalMapper(this);
		connect(_ui->actionDisplayNode, SIGNAL(triggered()), _displayModeSignalMapper, SLOT(map()));
		connect(_ui->actionDisplayWireFrame, SIGNAL(triggered()), _displayModeSignalMapper, SLOT(map()));
		connect(_ui->actionDisplaySurface, SIGNAL(triggered()), _displayModeSignalMapper, SLOT(map()));
		connect(_ui->actionDisplaySurfaceEdge, SIGNAL(triggered()), _displayModeSignalMapper, SLOT(map()));
		_displayModeSignalMapper->setMapping(_ui->actionDisplayNode, QString("Node"));
		_displayModeSignalMapper->setMapping(_ui->actionDisplayWireFrame, QString("WireFrame"));
		_displayModeSignalMapper->setMapping(_ui->actionDisplaySurface, QString("Surface"));
		_displayModeSignalMapper->setMapping(_ui->actionDisplaySurfaceEdge, QString("SurfaceWithEdge"));
		connect(_displayModeSignalMapper, SIGNAL(mapped(QString)), this, SLOT(setDisplay(QString)));
		connect(_displayModeSignalMapper, SIGNAL(mapped(QString)), this, SIGNAL(displayModeChangedSig(QString)));
		//几何显示模式(点、线、面)
		connect(_ui->actionDisplayPoint, SIGNAL(triggered()), this, SLOT(setGeometryDisplay()));
		connect(_ui->actionDisplayCurve, SIGNAL(triggered()), this, SLOT(setGeometryDisplay()));
		connect(_ui->actionDisplayFace, SIGNAL(triggered()), this, SLOT(setGeometryDisplay()));
		//设置几何选取模式(点、线、面)
		_selectGeometryModeMapper = new QSignalMapper(this);
		connect(_ui->actionSelectOff, SIGNAL(triggered()), _selectGeometryModeMapper, SLOT(map()));
		connect(_ui->actionSelectPoint, SIGNAL(triggered()), _selectGeometryModeMapper, SLOT(map()));
		connect(_ui->actionSelectCurve, SIGNAL(triggered()), _selectGeometryModeMapper, SLOT(map()));
		connect(_ui->actionSelectFace, SIGNAL(triggered()), _selectGeometryModeMapper, SLOT(map()));
		connect(_ui->actionSelectGeometryBody, SIGNAL(triggered()), _selectGeometryModeMapper, SLOT(map()));
		_selectGeometryModeMapper->setMapping(_ui->actionSelectOff, (int)ModuleBase::SelectModel::None);
		_selectGeometryModeMapper->setMapping(_ui->actionSelectPoint, (int)ModuleBase::SelectModel::GeometryWinPoint);
		_selectGeometryModeMapper->setMapping(_ui->actionSelectCurve, (int)ModuleBase::SelectModel::GeometryWinCurve);
		_selectGeometryModeMapper->setMapping(_ui->actionSelectFace, (int)ModuleBase::SelectModel::GeometryWinSurface);
		_selectGeometryModeMapper->setMapping(_ui->actionSelectGeometryBody, (int)ModuleBase::SelectModel::GeometryWinBody);

		connect(_selectGeometryModeMapper, SIGNAL(mapped(int)), this, SLOT(selectGeometryModelChanged(int)));
		connect(_selectGeometryModeMapper, SIGNAL(mapped(int)), this, SIGNAL(selectModelChangedSig(int)));


		connect(_ui->actionStart_Page, SIGNAL(triggered()), _subWindowManager, SLOT(openStartPage()));
		connect(_ui->actionPre_Window, SIGNAL(triggered()), _subWindowManager, SLOT(openPreWindow()));

		connect(this, SIGNAL(importGeometrySig(QStringList)), this, SLOT(importGeometry(QStringList)));
		connect(this, SIGNAL(exportGeometrySig(QString)), this, SLOT(exportGeometry(QString)));

		connect(_ui->actionCreate_Sketch, SIGNAL(triggered()), this, SLOT(on_sketchClicked()));
		connect(this, SIGNAL(showGraphRangeSig(double, double)), this, SLOT(showGraphRange(double, double)));
		connect(this, SIGNAL(startSketchSig(bool, double*, double*)), this, SLOT(startSketch(bool)));
		connect(this, SIGNAL(updateActionsStatesSig()), this, SLOT(updateActionsStates()));
		connect(this, SIGNAL(updatePreMeshActorSig()), this, SLOT(updatePreMeshActor()));		
		connect(this, SIGNAL(updatePreGeometryActorSig()), this, SLOT(updatePreGeometryActor()));
	}
							 
	void MainWindow::registerMoudel()
	{
		///添加ProjectWindow
		_controlPanel = new MainWidget::ControlPanel(this);
		_controlPanel->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
		addDockWidget(Qt::LeftDockWidgetArea, _controlPanel);
		this->setCorner(Qt::Corner::BottomLeftCorner, Qt::LeftDockWidgetArea);
		///添加MessageWindow
		_messageWindow = new MainWidget::MessageWindow(this);
		_messageWindow->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
		addDockWidget(Qt::BottomDockWidgetArea, _messageWindow);
		///添加ProcessWindow
		_processWindow = new MainWidget::ProcessWindow(this);
		_processWindow->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
		addDockWidget(Qt::BottomDockWidgetArea, _processWindow);			
	}

	Ui::MainWindow* MainWindow::getUi()
	{
		return _ui;
	}

	Translator* MainWindow::GetTranslator()
	{
		return _translator;
	}

	void MainWindow::setStatusBarInfo(QString info)
	{
		_ui->statusbar->showMessage(info,10000);
	}

	void MainWindow::setMD5(const QString& md5)
	{
		_MD5 = md5;
	}

	void MainWindow::setCurrentFile(const QString& file)
	{
		if (!file.isEmpty())
		{
			QFileInfo F(file);
			if (!F.exists()) return;
			_currentFile = F.absoluteFilePath();
		}
		  
		QString lang = Setting::BusAPI::instance()->getLanguage();
		QString title;
		ConfigOption::GlobalConfig* g = ConfigOption::ConfigOption::getInstance()->getGlobalConfig();

		title = g->getSoftName();
		if (lang.toLower() == "chinese")
			title = g->getChineseName();
		if (title.isEmpty())
			title = "FastCAE";
		if (!_currentFile.isEmpty())
			title = QString("%1-[%2]").arg(title).arg(_currentFile);

		this->setWindowTitle(title);
	}

	void MainWindow::updatePreMeshActor()
	{
		_subWindowManager->updatePreMeshActor();
	}

	void MainWindow::updatePreGeometryActor()
	{
		_subWindowManager->updatePreGeometryActor();
	}

	void MainWindow::closeEvent(QCloseEvent *event)
	{
		QString md5 = _signalHandler->getMD5();
		if (md5 != _MD5)
		{
			int nRet = QMessageBox::warning(this, tr("Save"), tr("Do you want to save before exit ?"), tr("Yes"),tr("No"),tr("Cancel"));
			
			switch (nRet)
			{
			case 2:
				event->ignore(); return;
			case 0:
				this->on_actionSave();
			case 1:
				//clean data
			default:
				break;
			}
			event->accept();
		}
//		if (!_designModel)
		_subWindowManager->closeAllSubWindow();
		Setting::BusAPI::instance()->writeINI();
	}

	void MainWindow::closeWindow()
	{
		close();
	}
	
	void MainWindow::ChangeLanguage(QString lang)
	{
		Setting::BusAPI::instance()->setLanguage(lang);
		if (nullptr == _ui) return;
		if (lang == "English")
		{
			_ui->actionChinese->setChecked(false);
			_ui->actionEnglish->setChecked(true);
			_translator->toEnglish();
		}
		else if (lang == "Chinese")
		{
			_ui->actionChinese->setChecked(true);
			_ui->actionEnglish->setChecked(false);
			_translator->toChinese();
		}

		Plugins::PluginManager::getInstance()->reTranslate(lang);
		_ui->retranslateUi(this);
		if (_recentMenu  != nullptr)
			_recentMenu->setTitle(tr("Recent"));

		_controlPanel->reTranslate();
		_messageWindow->reTranslate();
		_subWindowManager->reTranslate();
		if (_processWindow != nullptr) _processWindow->reTranslate();

		ConfigOption::GlobalConfig* g = ConfigOption::ConfigOption::getInstance()->getGlobalConfig();
		QString title = "FastCAE";
		if (lang.toLower() == "chinese")
		{
			QString f = g->getChineseName();
			if (!f.isEmpty()) title = f;
		}
		else
		{
			QString f = g->getSoftName();
			if (!f.isEmpty()) title = f;
		}
		if (!_currentFile.isEmpty())
			title = QString("%1-[%2]").arg(title).arg(_currentFile);

		this->setWindowTitle(title);

		
	}

	void MainWindow::on_actionNew()
	{
//		if (!_designModel)
		_signalHandler->on_actionNew();
//		emit updateActionStatesSig();
	}
	void MainWindow::on_actionOpen()
	{
/*		emit sig_action_open();*/
		QString md5 = _signalHandler->getMD5();
		if (md5 != _MD5)
		{
			QMessageBox::StandardButton but;
			but = QMessageBox::warning(this, tr("Warning"), tr("Do you want to save current data ?"), QMessageBox::Yes | QMessageBox::No);
			if (but == QMessageBox::Yes)
				on_actionSave();
		}
//		_signalHandler->clearData();
		QString dir = Setting::BusAPI::instance()->getWorkingDir();

		QString fillter = tr("Project file(*.diso);;Project file(*.xml)");
		QString title = tr("Open a project");
		QString filePath = QFileDialog::getOpenFileName(this, title, dir, fillter);
		
		if (filePath.isEmpty()) return;

//		_signalHandler->clearData();
//		_signalHandler->openProjectFile(filePath);

		QString pycode = QString("MainWindow.openProjectFile(\"%1\")").arg(filePath);
		qDebug() << pycode;
		Py::PythonAagent::getInstance()->submit(pycode);
	}

	void MainWindow::on_actionSave()
	{
		if (!_currentFile.isEmpty())
		{
			QString pycode = QString("MainWindow.saveProjectFile(\"%1\")").arg(_currentFile);
			Py::PythonAagent::getInstance()->submit(pycode);
		}
		else
			on_actionSaveAs();
		
	}

	void MainWindow::on_actionSaveAs()
	{
		QString fillter = tr("Project file(*.diso);;Project file(*.xml)");
		QString title = tr("Save project");
		QString dir = Setting::BusAPI::instance()->getWorkingDir();
		QString filePath = QFileDialog::getSaveFileName(this, title, dir, fillter);
		
		bool empty = Geometry::GeometryData::getInstance()->isEmpty();
		if (filePath.right(3).toLower() == "xml" && !empty )
		{
			QMessageBox::StandardButton bt = QMessageBox::warning(this, tr("Warning"), tr("Geometry will be lost! still continue?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			if (bt == QMessageBox::No)
				return;

		}
		if (!filePath.isEmpty())
		{
			QString pycode = QString("MainWindow.saveProjectFile(\"%1\")").arg(filePath);
			Py::PythonAagent::getInstance()->submit(pycode);
		}
		
	}

	void MainWindow::on_importMesh()
	{
		QStringList list = IO::IOConfigure::getMeshImporters();
		if (list.isEmpty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("The MeshPlugin is not installed !"));
			return;
		}

		qSort(list.begin(), list.end());
		QString suffixes = list.join(";;");
		QString senderName = sender()->objectName();
		int modelID = -1;
		if (senderName.contains("Only INP_"))
		{
			suffixes = list.at(0);
			modelID = senderName.right(1).toInt();
		}

		QString workDir = Setting::BusAPI::instance()->getWorkingDir();
		QFileDialog dlg(this, tr("Import Mesh"), workDir, suffixes);
		dlg.setAcceptMode(QFileDialog::AcceptOpen);
		dlg.setFileMode(QFileDialog::ExistingFile);		
		if (dlg.exec() != QFileDialog::Accepted)	return;

		QString aSuffix = dlg.selectedNameFilter();
		QString fileName = dlg.selectedFiles().join(",");
		if (fileName.isEmpty())	return;
		QString pyCode = QString("MainWindow.importMesh(\"%1\",\"%2\",%3)").arg(fileName).arg(aSuffix).arg(modelID);
		Py::PythonAagent::getInstance()->submit(pyCode);
	}

	void MainWindow::on_exportGeometry()
	{
		QString dir = Setting::BusAPI::instance()->getWorkingDir();

		ConfigOption::GeometryConfig* geoconfig = ConfigOption::ConfigOption::getInstance()->getGeometryConfig();
		QString conSuffix = geoconfig->getExportSuffix().toLower();
		QStringList sl = conSuffix.split(";");
		
		conSuffix.clear();
		for (QString s : sl)
		{
			conSuffix += " *." + s;
		}
		conSuffix = QString("Geometry Files(%1)").arg(conSuffix);
		QString regSuffix{};

		QString title = tr("Export Geometry");
		QString filename = QFileDialog::getSaveFileName(this, title, dir, conSuffix + regSuffix/* + ";;All Files(*.*)"*/);
		if (filename.isEmpty()) return;

		QString pycode = QString("MainWindow.exportGeometry(\"%1\")").arg(filename);
		Py::PythonAagent::getInstance()->submit(pycode);
	}

	void MainWindow::on_exportMesh()
	{	
		QStringList list = IO::IOConfigure::getMeshImporters();
		if (list.isEmpty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("The MeshPlugin is not installed !"));
			return;
		}

		if (MeshData::MeshData::getInstance()->getKernalCount() == 0)
		{
			QMessageBox::warning(this, tr("Warning"), tr("No one has any grid!"));
			return;
		}

		qSort(list.begin(), list.end());
		QString suffixes = list.join(";;");
		QString senderName = sender()->objectName();
		int modelID = -1;
		if (senderName.contains("Only INP_"))
		{
			suffixes = list.at(0);
			modelID = senderName.right(1).toInt();
		}
		QString workDir = Setting::BusAPI::instance()->getWorkingDir();
		QFileDialog dlg(this, tr("Export Mesh"), workDir, suffixes);
		dlg.setAcceptMode(QFileDialog::AcceptSave);
		if (dlg.exec() != QFileDialog::FileName)    return;

		QString aSuffix = dlg.selectedNameFilter();
		QString aFileName = dlg.selectedFiles().join(",");
		if (aFileName.isEmpty())	return;
		QString pyCode = QString("MainWindow.exportMesh(\"%1\",\"%2\",%3)").arg(aFileName).arg(aSuffix).arg(modelID);
		Py::PythonAagent::getInstance()->submit(pyCode);
	}

// 	void MainWindow::importMesh(QString fileName ,QString suffix, int modelId)
// 	{
// 		_signalHandler->importMesh(fileName, suffix, modelId);
// 	}

	void MainWindow::importMeshDataset(vtkDataSet* dataset)
	{
		if (dataset == nullptr) return;
		MeshData::MeshKernal* k = new MeshData::MeshKernal;
		k->setMeshData(dataset);
		MeshData::MeshData::getInstance()->appendMeshKernal(k);
		_subWindowManager->openPreWindow();
//		_subWindowManager->updatePreMeshActor();
		emit updateMeshTreeSig();
		emit updateActionStatesSig();
	}

	void MainWindow::on_importGeometry()
	{
		QString dir = Setting::BusAPI::instance()->getWorkingDir();
		ConfigOption::GeometryConfig* geoconfig = ConfigOption::ConfigOption::getInstance()->getGeometryConfig();
		QString conSuffix = geoconfig->getImportSuffix().toLower();
		QStringList sl = conSuffix.split(";");
		conSuffix.clear();
		for (QString s : sl)
		{
			conSuffix += " *." + s;
		}
		conSuffix = QString("Geometry Files(%1)").arg(conSuffix);
		QString regSuffix{};

		QString title = tr("Import Geometry");
		QStringList filenames = QFileDialog::getOpenFileNames(this, title, dir, conSuffix + regSuffix /*+ ";;All Files(*.*)"*/);
		if (filenames.isEmpty()) return;

		QString files = filenames.join(",");
		QString pycode = QString("MainWindow.importGeometry(\"%1\")").arg(files);
		Py::PythonAagent::getInstance()->submit(pycode);		
	}

	void MainWindow::importGeometry(QStringList filenames)
	{
		_subWindowManager->openPreWindow();
		if (!_signalHandler->importGeometry(filenames)) return;
	}

	void MainWindow::exportGeometry(QString f)
	{
		if (!_signalHandler->exportGeometry(f)) return;
		Py::PythonAagent::getInstance()->unLock();
	}

	void MainWindow::on_solidMesh()
	{
		_signalHandler->generateSolidMesh();
	}

	void MainWindow::on_surfaceMesh()
	{
		_signalHandler->generateSurfaceMesh();
	}

	void MainWindow::on_genMesh()
	{
	}

	void MainWindow::setWorkingDir()
	{
		Setting::BusAPI::instance()->setWorkingDir();
	}

	void MainWindow::on_solverManager()
	{
		SolverControl::SolverManagerDialog dlg(this);
		dlg.exec();
	}

	void MainWindow::on_solve()
	{
		bool showDlg = true;
		MainWidget::ProjcctSolveDialog dlg(this, showDlg);
		if (showDlg)
			dlg.exec();
	}

	void MainWindow::solveProject(int id)
	{
		bool showDlg = true;
		MainWidget::ProjcctSolveDialog dlg(this, showDlg, id);
		if (showDlg)
			dlg.exec();
	}

	void MainWindow::on_solveOption()
	{
//		Setting::BusAPI::instance()->setSolverOptions();
	}

	void MainWindow::on_graphOption()
	{
		Setting::BusAPI::instance()->setGraphOptions();
	}

	void MainWindow::selectModelChanged(int model)
	{
		_ui->actionSelectOff->setChecked(false);
		//_ui->actionSelectGeoBody->setChecked(false);
		_ui->actionSelectMeshNode->setChecked(false);
		_ui->actionSelectMeshCell->setChecked(false);
		_ui->actionBoxMeshNode->setChecked(false);
		_ui->actionBoxMeshCell->setChecked(false);

		switch (model)
		{
		case (int)ModuleBase::SelectModel::None:
			_ui->actionSelectOff->setChecked(true); break;
		case (int)ModuleBase::SelectModel::MeshNode:
			_ui->actionSelectMeshNode->setChecked(true); break;
		case (int)ModuleBase::SelectModel::MeshCell:
			_ui->actionSelectMeshCell->setChecked(true); break;
		case (int)ModuleBase::SelectModel::BoxMeshNode:
			_ui->actionBoxMeshNode->setChecked(true); break;
		case (int)ModuleBase::SelectModel::BoxMeshCell:
			_ui->actionBoxMeshCell->setChecked(true); break;

	  
		default: break;
		}
	}

	SubWindowManager* MainWindow::getSubWindowManager()
	{
		return _subWindowManager;
	}

	void MainWindow::on_about()
	{
		AboutDialog dlg;
		dlg.exec();
	}

	void MainWindow::on_userManual()
	{
		QString file = ConfigOption::ConfigOption::getInstance()->getGlobalConfig()->GetUserManual();
		QString userManulFile = QApplication::applicationDirPath() + "/../Doc/" + file;

		if (file.isEmpty())
		{
			QDesktopServices::openUrl(QUrl("http://www.fastcae.com/document.html"));
			return;
		}

		QFile f(userManulFile);
		if (!f.exists() )
		{
			ModuleBase::Message msg;
			msg.type = ModuleBase::Warning_Message;
			msg.message = tr("Please make sure \" %1 \" file exist!").arg(userManulFile);
			emit printMessageToMessageWindow(msg);
		}
		else
		{
			if (!QDesktopServices::openUrl(QUrl::fromLocalFile(userManulFile)))
			{
				QString mess = QString(tr("%1 not exist !")).arg(file);
				QMessageBox::warning(this, QString(tr("Warning")), mess);
			}
				
		}
	}

	void MainWindow::setDisplay(QString m)
	{
		_ui->actionDisplayNode->setChecked(false);
		_ui->actionDisplayWireFrame->setChecked(false);
		_ui->actionDisplaySurface->setChecked(false);
		_ui->actionDisplaySurfaceEdge->setChecked(false);

		if (m.toLower() == "node") _ui->actionDisplayNode->setChecked(true);
		else if (m.toLower() == "wireframe") _ui->actionDisplayWireFrame->setChecked(true);
		else if (m.toLower() == "surface") _ui->actionDisplaySurface->setChecked(true);
		else if (m.toLower() == "surfacewithedge") _ui->actionDisplaySurfaceEdge->setChecked(true);
	}

	void MainWindow::selectGeometryModelChanged(int m)
	{
		bool active{ true };
		_ui->actionSelectOff->setChecked(false);
		_ui->actionSelectPoint->setChecked(false);
		_ui->actionSelectCurve->setChecked(false);
		_ui->actionSelectFace->setChecked(false);
		_ui->actionSelectGeometryBody->setChecked(false);
		switch (m)
		{
		case (int)ModuleBase::SelectModel::None:
			_ui->actionSelectOff->setChecked(true); break;
		case (int)ModuleBase::SelectModel::GeometryPoint:
		case (int)ModuleBase::SelectModel::GeometryWinPoint:
			_ui->actionSelectPoint->setChecked(true); break;
		case (int)ModuleBase::SelectModel::GeometryCurve:
		case (int)ModuleBase::SelectModel::GeometryWinCurve:
			_ui->actionSelectCurve->setChecked(true); break;
		case (int)ModuleBase::SelectModel::GeometrySurface:
		case (int)ModuleBase::SelectModel::GeometryWinSurface:
			_ui->actionSelectFace->setChecked(true); break;
		case (int)ModuleBase::SelectModel::GeometryBody:
		case (int)ModuleBase::SelectModel::GeometryWinBody:
			_ui->actionSelectGeometryBody->setChecked(true); break;;
		default:
			break;
			
		}
//		emit selectGeoActiveSig(active);
	}

	void MainWindow::setGeometryDisplay()
	{
		
		bool checkvertex = _ui->actionDisplayPoint->isChecked();
		bool checkcurve = _ui->actionDisplayCurve->isChecked();
		bool checkface = _ui->actionDisplayFace->isChecked();

		Setting::GraphOption* gp = Setting::BusAPI::instance()->getGraphOption();
		gp->isShowGeoPoint(checkvertex);
		gp->isShowGeoEdge(checkcurve);
		gp->isShowGeoSurface(checkface);
		
		emit selectGeometryDisplay(checkvertex, checkcurve, checkface);
	}

	void MainWindow::updateRecentMenu()
	{
		if (_recentMenu == nullptr)
		{
			_recentMenu = new QMenu(tr("Recent"));
			_ui->menuFile->addMenu(_recentMenu);
		}
		_recentMenu->clear();

		if (_recentFileMapper == nullptr)
		{
			_recentFileMapper = new QSignalMapper;
			
		}
		_recentFileMapper->disconnect();
		QStringList rencentFile = Setting::BusAPI::instance()->getRencetFiles();
		for (int i = 0; i < rencentFile.size(); ++i)
		{
			QString f = rencentFile.at(i);
			QAction* action = _recentMenu->addAction(f);
			action->setStatusTip(f);
			connect(action, SIGNAL(triggered()), _recentFileMapper, SLOT(map()));
			_recentFileMapper->setMapping(action, f);
		}
		connect(_recentFileMapper, SIGNAL(mapped(QString)), this, SLOT(openRencentFile(QString)));
	}

	void MainWindow::setActionVisible(QString objname, bool enable)
	{
		QList<QAction*> acs = this->findChildren<QAction*>();
		for (QAction* a : acs)
		{
			if (a->objectName().toLower() == objname.toLower())
			{
				a->setVisible(enable);
				break;
			}
		}
	}

	void MainWindow::openRencentFile(QString file)
	{
		QFileInfo info(file);
		if (!info.exists())
		{
			QMessageBox::warning(this, tr("Warning"), tr("File \"%1\" is not exist !"));
			return;
		}

		QString md5 = _signalHandler->getMD5();
		if (md5 != _MD5)
		{
			QMessageBox::StandardButton but;
			but = QMessageBox::warning(this, tr("Warning"), tr("Do you want to save current data ?"), QMessageBox::Yes | QMessageBox::No);
			if (but == QMessageBox::Yes)
				on_actionSave();
		}
		_signalHandler->clearData(false);
		QString dir = Setting::BusAPI::instance()->getWorkingDir();

		bool ok = _signalHandler->openProjectFile(file);
		if (!ok) return;
		_subWindowManager->openPreWindow();
		_subWindowManager->updatePreActors();

		setCurrentFile(file);

		updateRecentMenu();
		emit updateActionStatesSig();
	}

	void MainWindow::keyPressEvent(QKeyEvent *e)
	{
		qDebug() << "press";

	}

	void MainWindow::keyReleaseEvent(QKeyEvent *e)
	{
		qDebug() << "release";
	}

	void MainWindow::showEvent(QShowEvent *e)
	{
		QMainWindow::showEvent(e);
		_signalHandler->showUserGuidence(true);
	}

	bool MainWindow::isLoadRecordScripFile()
	{
		QFile file(qApp->applicationDirPath() + "/../temp/RecordScript.py");
		if (!file.exists() || file.size() == 0)
			return false;

		QMessageBox::StandardButton result =
			QMessageBox::warning(this, tr("Do you need to load?"),
			tr("The program quit with an exception before, do you want to reload the contents?"),
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

		switch (result)
		{
		case QMessageBox::Yes:
			Py::PythonAagent::getInstance()->execScript(file.fileName());
			break;
		case QMessageBox::No:
			QFile::remove(qApp->applicationDirPath() + "/../temp/RecordScript.py");
			break;
		default:
			break;
		}
		return true;
	}

	void MainWindow::on_CreateSet()
	{
		MainWidget::CreateSetDialog dlg(this,_subWindowManager->getPreWindow());
		dlg.exec();
	}
	
	void MainWindow::on_CreateGeoComponent()
	{
		MainWidget::CreateGeoComponentDialog dlg(this, _subWindowManager->getPreWindow());
		dlg.exec();
	}

	void MainWindow::clearWidgets()
	{
		_subWindowManager->closeAllSubWindow();
		_controlPanel->clearWidget();
	}

	void MainWindow::printMessage(int type, QString m)
	{
		ModuleBase::MessageType t = (ModuleBase::MessageType)type;
		ModuleBase::Message s(t, m);
		emit printMessageToMessageWindow(s);
	}

	void MainWindow::on_SaveScript()
	{
		QString dir = Setting::BusAPI::instance()->getWorkingDir();
		QString suffix = "Python(*.py)";
		QString title = tr("Save Script");
		QString filenames = QFileDialog::getSaveFileName(this, title, dir, suffix);
		if (filenames.isEmpty()) return;
		Py::PythonAagent::getInstance()->saveScript(filenames);
	}

	void MainWindow::on_ExecuateScript()
	{
		QString fillter = "Python(*.py)";
		QString title = tr("Execute Script");
		QString dir = Setting::BusAPI::instance()->getWorkingDir();
		QString filePath = QFileDialog::getOpenFileName(this, title, dir, fillter);
		if (filePath.isEmpty()) return;
		bool ok = Py::PythonAagent::getInstance()->execScript(filePath);
		if (!ok)
			QMessageBox::warning(this, tr("Warning"), QString(tr("%1 execute failed !")).arg(filePath));
	}

	MainWidget::ControlPanel* MainWindow::getControlPanel()
	{
		return _controlPanel;
	}

	void MainWindow::on_sketchClicked()
	{
		bool ischecked = _ui->actionCreate_Sketch->isChecked();
		if (ischecked)
		{
			if (!_subWindowManager->isPreWindowOpened())
				_subWindowManager->openPreWindow();

			auto dlg = new GeometryWidget::SketchPlanDialog(this, _subWindowManager->getPreWindow());
			dlg->show();
		}
		else
		{
			auto p = _subWindowManager->getPreWindow();
			emit startSketchSig(false, nullptr, nullptr);
		}
	}

	void MainWindow::startSketch(bool s)
	{
		_ui->actionCreate_Sketch->setChecked(s);

		_ui->GeomertryFeatureToolBar->setVisible(!s);
		_ui->ChamferToolBar->setVisible(!s);
		_ui->BoolToolBar->setVisible(!s);
		_ui->FeatureOpertionToolBar->setVisible(!s);

		_ui->SketchToolBar->setVisible(s);
	}

	void MainWindow::on_FilterMesh()
	{
		MainWidget::FilterMeshDialog dlg(this, _subWindowManager->getPreWindow());
		dlg.exec();
	}

	void MainWindow::on_VTKTranslation()
	{
		MainWidget::DialogVTKTransform dlg(this);
		dlg.exec();
	}

	void MainWindow::showGraphRange(double w, double h)
	{
		if (_graphRange == nullptr)
		{
			_graphRange = new QLabel(this);
			_ui->statusbar->addPermanentWidget(_graphRange);
		}
		QString text = QString(tr("Canvas %1mm * %2mm  ")).arg(w).arg(h);
		_graphRange->setText(text);
	}

	void MainWindow::updateActionsStates()
	{
		_signalHandler->updateActionsStates();
	}

	CustomizerHelper* MainWindow::getCustomizerHelper()
	{
		return _customizerHelper;
	}

	SignalHandler* MainWindow::getSignalHandler()
	{
		return _signalHandler;
	}

	MainWidget::MessageWindow* MainWindow::getMessageWindow()
	{
		return _messageWindow;
	}

	void MainWindow::setIcon(QString iconPath)
	{
		this->setWindowIcon(QIcon(iconPath));
		_subWindowManager->setIcon(iconPath);

	}

	QAction* MainWindow::getAction(QString& objName)
	{
		QList<QAction*> acs = this->findChildren<QAction*>();
		for (QAction* a : acs)
		{
			if (a->objectName().toLower() == objName.toLower())
				return  a;
		}
		return nullptr;
	}

	QToolBar* MainWindow::getToolBar(QString& objName)
	{
		QList<QToolBar*> tbs = this->findChildren<QToolBar*>();
		for (auto a : tbs)
		{
			if (a->objectName().toLower() == objName.toLower())
				return  a;
		}
		return nullptr;
	}

	QMenu* MainWindow::getMenu(QString& objName)
	{
		QList<QMenu*> mes = this->findChildren<QMenu*>();
		for (auto a : mes)
		{
			if (a->objectName().toLower() == objName.toLower())
				return  a;
		}
		return nullptr;
	}

}//end of namespace
