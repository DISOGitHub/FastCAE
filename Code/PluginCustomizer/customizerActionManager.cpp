#include "customizerActionManager.h"
#include "mainWindow/mainWindow.h"
#include "mainWindow/CustomizerHelper.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/GlobalConfig.h"
#include "pluginCustomizer.h"
#include <QMenuBar>
#include <QDialog>
#include <QDebug>
#include <QMessageBox>
#include "ParaBasicSetup.h"
#include "ParaImportGeometrySetup.h"
#include "DataManager.h"
#include "QFWidgetAction.h"
#include "ConfigOptions/GeometryConfig.h"
#include "ParaImportMeshSetup.h"
#include "ParaExportMeshSetup.h"
#include "ParaUserManualSetup.h"
#include "FunctionTreeSetup.h"
#include "EditorSolverManager.h"
#include "DataManager.h"
#include "Generateinstallationpackage.h"
#include <QCoreApplication>
#include "ParaManagerData.h"

namespace FastCAEDesigner
{
	class DataManager;
}

namespace Plugins
{
	CustomActionManager::CustomActionManager(CustomizerPlugin* plugin, GUI::MainWindow* m)
		: _plugin(plugin), _mainWindow(m)
	{
	}

	CustomActionManager::~CustomActionManager()
	{

	}

	//初始化运行环境
	void CustomActionManager::init()
	{
		FillCustomMenu();
		
	}

	//开始定制
	void CustomActionManager::on_startCustom_slot()
	{
		_plugin->isUsing(true);
		_startAction->setEnabled(false);
		_finishAction->setEnabled(true);
		_cancelAction->setEnabled(true);
		_shareAction->setEnabled(false);

		//xuxinwei 20200324 
		FastCAEDesigner::DataManager::getInstance()->clearIconNameList();

		_mainWindow->getCustomizerHelper()->startCustomizer();

		//Added xvdongming
		//给出正在定制的标识
		QString title = _mainWindow->windowTitle();
		_mainWindow->setWindowTitle(title + " ---- " + tr("Customizing"));

		_actionList.clear();
		_menuAndtoolbarList.clear();
		//	_menuAndtoolbarList.clear();

		FastCAEDesigner::DataManager::getInstance()->ReadInfoFromServerToLocal();

		FillFileMenu();
		CreateTopMenu(tr("Views"), false);
		FillGeometryMenu();
		FillMeshMenu();
		FillSolveMenu();
		//CreateTopMenu("Solve");
		CreateTopMenu(tr("Post"), false);
		CreateTopMenu(tr("Windows"), false);
		CreateTopMenu(tr("Setup"), false);
		CreateTopMenu(tr("Plugin"), false);
		FillHelpMenu();

		FillFileToolBar();					//建立文件工具栏
		FillSolveToolBar();					//建立求解工具栏
		FillViewerToolBar();				//建立视图工具栏
		FillSelectToolBar();				//建立选择工具栏
		FillSplitMeshToolBar();				//建立网格剖分工具栏
		FillViewToolBar();					//建立查看网格几何工具栏
		FillCheckCreatToolBar();			//建立网格检测创建组件工具栏
		FillURDoToolBar();					//建立撤销恢复工具栏
		_mainWindow->addToolBarBreak();
		FillCreateSketchToolBar();			//建立草绘工具栏
		FillFeatureModingToolBar();			//建立几何建模工具栏
		FillFeatureOperations1ToolBar();	//建立几何处理工具栏1
		FillFeatureOperations2ToolBar();	//建立几何处理工具栏2
		FillFeatureOperations3ToolBar();	//建立几何处理工具栏3

		//Added xvdongming
		initMenuAndToolBar();	//初始化菜单工具栏

		//libaojun
		OnParaFunctionParaSetup();
	}

	//卸载插件
	void CustomActionManager::release()
	{
		_menu->removeAction(_startAction);
		delete _startAction;
		_menu->removeAction(_finishAction);
		delete _finishAction;
		_menu->removeAction(_shareAction);
		delete _shareAction;

		delete _menu;
		_menu = nullptr;
	}

	//完成定制
	void CustomActionManager::on_finishCustom_slot()
	{
		FastCAEDesigner::DataManager::getInstance()->SetPhysicsList(_functionTreeSetup->GetPhysicsList());
		FastCAEDesigner::DataManager::getInstance()->SetMaterialList(_functionTreeSetup->GetMaterialList());
		FastCAEDesigner::DataManager::getInstance()->WriteInfoToServerPath();

		if (_editorSolver != nullptr)
		{
			_editorSolver->writeConfig();

			QString exePath = qApp->applicationDirPath();
			QString dependencyfilesPath = exePath + "/../Solver/";
			QList<QStringList> dependencyfiles = _editorSolver->getDependecyFiles();
			copySolverDependencyFiles(dependencyfiles, dependencyfilesPath);
		}
		
		FastCAEDesigner::ParaManagerData::getInstance()->writeObserverConfig();

		this->quitCustomizer();		
	}

	//撤销定制
	void CustomActionManager::on_cancelCustom_slot()
	{
		//QMessageBox::StandardButton b = QMessageBox::warning(_mainWindow, QString(tr("Warning")), QString(tr("Quit customizer without saving options?")), QMessageBox::Yes|QMessageBox::No);
		QMessageBox msgBox(QMessageBox::Warning, tr("Warning"), tr("Quit customizer without saving options?"));
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		msgBox.setButtonText(QMessageBox::Yes, tr("Yes"));
		msgBox.setButtonText(QMessageBox::No, tr("No"));

		int b = msgBox.exec();

		if (b != QMessageBox::Yes) return;
		this->quitCustomizer();
		FastCAEDesigner::ParaManagerData::getInstance()->setReadFlag(true);
	}

	//发布版本
	void CustomActionManager::on_share_slot()
	{
#ifdef Q_OS_WIN
		ConfigOption::GlobalConfig* globalConfig = ConfigOption::ConfigOption::getInstance()->getGlobalConfig();
		QString name = globalConfig->getSoftName();
		QString version = globalConfig->getVersion();

		if (name.isEmpty())
			name = QString("FastCAE");
		//if (version.isEmpty())
			version = QString("1.0.0");

		FastCAEDesigner::Generateinstallationpackage dlg(name,version);
		dlg.exec();
#endif
	}

	//基本参数设定
	void CustomActionManager::OnParaBaseSetup()
	{
		//xuxinwei 20200305
		_subWindowManager = _mainWindow->getSubWindowManager();
		FastCAEDesigner::ParaBasicSetup dlg(FastCAEDesigner::DataManager::getInstance()->getGlobalConfig(), _mainWindow, _subWindowManager);

		//FastCAEDesigner::ParaBasicSetup dlg(FastCAEDesigner::DataManager::getInstance()->getGlobalConfig(), _mainWindow);
		dlg.SetLogoFileName(FastCAEDesigner::DataManager::getInstance()->GetLogoFileName());
		dlg.SetWelcomeFileName(FastCAEDesigner::DataManager::getInstance()->GetWelcomeFileName());

		if (QDialog::Accepted == dlg.exec())
		{
			FastCAEDesigner::DataManager::getInstance()->SetLogoFileName(dlg.GetLogoFileName());
			FastCAEDesigner::DataManager::getInstance()->SetWelcomeFileName(dlg.GetWelcomeFileName());
		}
	}

	//设定用户手册
	void CustomActionManager::OnParaUserManualSetup()
	{
		FastCAEDesigner::ParaUserManualSetup dlg(FastCAEDesigner::DataManager::getInstance()->getGlobalConfig(), _mainWindow);
		dlg.exec();
	}

	void CustomActionManager::OnParaFunctionParaSetup()
	{
		_functionTreeSetup = new FastCAEDesigner::FunctionTreeSetup(_mainWindow);
		_mainWindow->addDockWidget(Qt::LeftDockWidgetArea, _functionTreeSetup);
//		_functionTreeSetup->show();
	}


	//输入几何参数设定
	void CustomActionManager::OnParaImportGeometrySetup()
	{
		FastCAEDesigner::ParaImportGeometrySetup dlg(true,_mainWindow);
		int r = dlg.exec();
		if (r == QDialog::Accepted)
		{
			QString suffix = dlg.getSuffix();
			QString exportSuffix = FastCAEDesigner::DataManager::getInstance()->GetExportGeometrySuffix();

			if ((suffix.isEmpty()) && (exportSuffix.isEmpty()))
				_geoSuffixNull = true;
			else
				_geoSuffixNull = false;

			updateImportGeometryAction(suffix);
			updateGeometrySelectAndView();
		}

	}

	//输出几何参数设定
	void CustomActionManager::OnParaExportGeometrySetup()
	{
		FastCAEDesigner::ParaImportGeometrySetup dlg(false, _mainWindow);
		int r = dlg.exec();

		if (r == QDialog::Accepted)
		{
			QString suffix = dlg.getSuffix();
			QString importSuffix = FastCAEDesigner::DataManager::getInstance()->GetImportGeometrySuffix();

			if ((suffix.isEmpty()) && (importSuffix.isEmpty()))
				_geoSuffixNull = true;
			else
				_geoSuffixNull = false;

			updateGeometrySelectAndView();
		}
	}

	//输入网格文件类型设定
	void CustomActionManager::OnParaImportMeshSetup()
	{
		FastCAEDesigner::ParaImportMeshSetup dlg(_mainWindow);
		int r = dlg.exec();

		if (r == QDialog::Accepted)
		{
			QString suffix = dlg.getSuffix();
			QString exportSuffix = FastCAEDesigner::DataManager::getInstance()->GetExportMeshSuffix();

			if ((suffix.isEmpty()) && (exportSuffix.isEmpty()))
				_meshSuffixNull = true;
			else
				_meshSuffixNull = false;

			updateImportMeshAction(suffix);
			updateMeshSelectAndView();
		}
	}
	//输出网格文件类型设定
	void CustomActionManager::OnParaExportMeshSetup()
	{
		FastCAEDesigner::ParaExportMeshSetup dlg(_mainWindow);
		int r = dlg.exec();
		if (r == QDialog::Accepted)
		{
			QString suffix = dlg.getExportSuffix();
			QString importSuffix = FastCAEDesigner::DataManager::getInstance()->GetImportMeshSuffix();

			if ((suffix.isEmpty()) && (importSuffix.isEmpty()))
				_meshSuffixNull = true;
			else
				_meshSuffixNull = false;

			updateMeshSelectAndView();
		}

	}


	//建立定制菜单
	void CustomActionManager::FillCustomMenu()
	{
		QString des = _plugin->getDescribe();
		_menu = new QMenu(des);
		_cusAction = _mainWindow->menuBar()->insertMenu(nullptr, _menu);
		//开始定制菜单
		_startAction = new QAction(tr("Start Customizer"), _mainWindow);
		_menu->addAction(_startAction);

		//完成定制菜单
		_finishAction = new QAction(tr("Finish Customizer"), _mainWindow);
		_menu->addAction(_finishAction);

		//取消定制菜单
		_cancelAction = new QAction(tr("Cancel Customizer"), _mainWindow);
		_menu->addAction(_cancelAction);

		//发布软件
		_shareAction = new QAction(tr("Share & Release"), _mainWindow);
		_menu->addAction(_shareAction);

		connect(_startAction, SIGNAL(triggered()), this, SLOT(on_startCustom_slot()));
		connect(_finishAction, SIGNAL(triggered()), this, SLOT(on_finishCustom_slot()));
		connect(_cancelAction, SIGNAL(triggered()), this, SLOT(on_cancelCustom_slot()));
		connect(_shareAction, SIGNAL(triggered()), this, SLOT(on_share_slot()));

		_startAction->setEnabled(true);
		_finishAction->setEnabled(false);
		_cancelAction->setEnabled(false);
	}

	//建立帮助菜单
	void CustomActionManager::FillHelpMenu()
	{
		_menuHelp = CreateTopMenu(tr("Help"));
		_userInfo = CreateAction(tr("About"), ":/QUI/icon/about_us.png", tr(""), _menuHelp, _mainWindow, true);
		_userDoc = CreateAction(tr("Users manual"), ":/QUI/icon/help.png", tr(""), _menuHelp, _mainWindow, true);
//		QAction *designer = CreateAction(tr("Function para setup"), ":/QUI/icon/help.png", tr(""), _menuHelp, _mainWindow, true);

		connect(_userInfo, SIGNAL(triggered()), this, SLOT(OnParaBaseSetup()));
		connect(_userDoc, SIGNAL(triggered()), this, SLOT(OnParaUserManualSetup()));
//		connect(designer, SIGNAL(triggered()), this, SLOT(OnParaFunctionParaSetup()));
	}

	//建立文件菜单
	void CustomActionManager::FillFileMenu()
	{
		_menuFile = CreateTopMenu(tr("File"));

		CreateAction(tr("New"), ":/QUI/icon/createNew.png", tr("Ctrl+N"), _menuFile, _mainWindow);
		CreateAction(tr("Open"), ":/QUI/icon/open.png", tr("Ctrl+O"), _menuFile, _mainWindow);
		CreateAction(tr("Close"), ":/QUI/icon/stop.png", tr("Ctrl+Q"), _menuFile, _mainWindow);
		CreateActionSeparator(_menuFile);
		_importMesh = CreateAction(tr("Import mesh"), ":/QUI/icon/importMesh.png", tr(""), _menuFile, _mainWindow, true);
		_exportMesh = CreateAction(tr("Export mesh"), ":/QUI/icon/exportMesh.png", tr(""), _menuFile, _mainWindow, true);
		_importGeometry = CreateAction(tr("Import geometry"), ":/QUI/icon/importGeometry.png", tr(""), _menuFile, _mainWindow, true);
		_exportGeometry = CreateAction(tr("Export geometry"), ":/QUI/icon/exportGeometry.png", tr(""), _menuFile, _mainWindow, true);
		CreateActionSeparator(_menuFile);
		CreateAction(tr("Save"), ":/QUI/icon/save.png", tr("Ctrl+S"), _menuFile, _mainWindow);
		CreateAction(tr("Save as"), ":/QUI/icon/saveAs.png", tr("Ctrl+Shift+S"), _menuFile, _mainWindow);
		CreateActionSeparator(_menuFile);
		CreateAction(tr("Save script"), ":/QUI/icon/saveScript.png", tr(""), _menuFile, _mainWindow);
		CreateAction(tr("Execution script"), ":/QUI/icon/execScript.png", tr("Ctrl+R"), _menuFile, _mainWindow);
		CreateActionSeparator(_menuFile);
		CreateAction(tr("Save pictures"), ":/QUI/icon/saveImage.png", tr(""), _menuFile, _mainWindow);
		CreateAction(tr("Work dir"), ":/QUI/icon/open.png", tr(""), _menuFile, _mainWindow);
		CreateActionSeparator(_menuFile);
		CreateAction(tr("Newest File"), "", tr(""), _menuFile, _mainWindow);

		connect(_importGeometry, SIGNAL(triggered()), this, SLOT(OnParaImportGeometrySetup()));
		connect(_exportGeometry, SIGNAL(triggered()), this, SLOT(OnParaExportGeometrySetup()));
		connect(_importMesh, SIGNAL(triggered()), this, SLOT(OnParaImportMeshSetup()));
		connect(_exportMesh, SIGNAL(triggered()), this, SLOT(OnParaExportMeshSetup()));
	}

	//建立几何菜单
	void CustomActionManager::FillGeometryMenu()
	{
		_menuGeometry = CreateTopMenu(tr("Geometry"));
		CreateAction(tr("Undo"), ":/QUI/geometry/undo.png", tr("Ctrl+Z"), _menuGeometry, _mainWindow);
		CreateAction(tr("Redo"), ":/QUI/geometry/redo.png", tr("Ctrl+Y"), _menuGeometry, _mainWindow);
		CreateActionSeparator(_menuGeometry);

		//_geoFeatureModeling = CreateAction(tr("Feature modeling"), _menuGeometry, _mainWindow);
		//_geoFeatureOperation = CreateAction(tr("Feature operation"), _menuGeometry, _mainWindow);
		//_geoDraft = CreateAction(tr("Draft"), _menuGeometry, _mainWindow);

		/*FastCAEDesigner::QFWidgetAction *_geoFeatureModelingCheckbox{};
		FastCAEDesigner::QFWidgetAction *_geoFeatureOperationCheckbox{};
		FastCAEDesigner::QFWidgetAction *_geoDraftCheckbox{};*/

		_geoFeatureModelingCheckbox = new FastCAEDesigner::QFWidgetAction(_menuGeometry);
		_geoFeatureModelingCheckbox->SetText(tr("Feature modeling"));

		_geoFeatureOperationCheckbox = new FastCAEDesigner::QFWidgetAction(_menuGeometry);
		_geoFeatureOperationCheckbox->SetText(tr("Feature operation"));

		_geoDraftCheckbox = new FastCAEDesigner::QFWidgetAction(_menuGeometry);
		_geoDraftCheckbox->SetText(tr("Sketch"));

		_menuGeometry->addAction(_geoFeatureModelingCheckbox);
		_menuGeometry->addAction(_geoFeatureOperationCheckbox);
		_menuGeometry->addAction(_geoDraftCheckbox);

		CreateActionSeparator(_menuGeometry);

		//保存子菜单项，在退出定制时清除菜单项。
		_actionList.append(_geoFeatureModelingCheckbox);
		_actionList.append(_geoFeatureOperationCheckbox);
		_actionList.append(_geoDraftCheckbox);

		CreateAction(tr("Views"), _menuGeometry, _mainWindow);
		CreateAction(tr("Selected"), _menuGeometry, _mainWindow);

		/**/
		//connect(_importGeometry, SIGNAL(triggered()), this, SLOT(OnParaImportGeometrySetup()));
		//connect(_exportGeometry, SIGNAL(triggered()), this, SLOT(OnParaExportGeometrySetup()));

		/*connect(_geoFeatureModelingCheckbox,
			SIGNAL(signal_CheckBoxStateChanged(int)),
			this,SLOT(SetGeometryFeatureModeling(int)));
			*/
		connect(_geoFeatureModelingCheckbox,
			SIGNAL(signal_CheckBoxStateChanged(int)),
			this, SLOT(UpdateFeatureModingToolBar(int)));
		connect(_geoFeatureOperationCheckbox,
			SIGNAL(signal_CheckBoxStateChanged(int)),
			this, SLOT(UpdateFeatureOperationsToolBar(int)));
		connect(_geoDraftCheckbox, SIGNAL(signal_CheckBoxStateChanged(int)),
			this, SLOT(UpdateCreateSketchToolBar(int)));
		
	}

	//建立网格菜单
	void CustomActionManager::FillMeshMenu()
	{
		_menuMesh = CreateTopMenu(tr("Mesh"));


		/*FastCAEDesigner::QFWidgetAction *_meshSurfaceCheckbox{};
		FastCAEDesigner::QFWidgetAction *_meshSolidCheckbox{};*/

		_meshSurfaceCheckbox = new FastCAEDesigner::QFWidgetAction(_menuMesh);
		_meshSurfaceCheckbox->SetText(tr("Mesh surface"));

		_meshSolidCheckbox = new FastCAEDesigner::QFWidgetAction(_menuMesh);
		_meshSolidCheckbox->SetText(tr("Mesh solid"));

		_menuMesh->addAction(_meshSurfaceCheckbox);
		_menuMesh->addAction(_meshSolidCheckbox);

		CreateAction(tr("Gen mesh"), ":/QUI/icon/solumationsetting.png", tr(""), _menuMesh, _mainWindow);
		CreateActionSeparator(_menuMesh);

		/*FastCAEDesigner::QFWidgetAction *_meshCheckingCheckbox{};
		FastCAEDesigner::QFWidgetAction *_meshCreateCheckbox{};*/

		_meshCheckingCheckbox = new FastCAEDesigner::QFWidgetAction(_menuMesh);
		_meshCheckingCheckbox->SetText(tr("Checking"));

		_meshCreateCheckbox = new FastCAEDesigner::QFWidgetAction(_menuMesh);
		_meshCreateCheckbox->SetText(tr("Create set"));

		_menuMesh->addAction(_meshCheckingCheckbox);
		_menuMesh->addAction(_meshCreateCheckbox);

		CreateAction(tr("Views"), _menuMesh, _mainWindow);
		CreateAction(tr("Select"), _menuMesh, _mainWindow);

		//connect(_importGeometry, SIGNAL(triggered()), this, SLOT(OnParaImportGeometrySetup()));
		//connect(_exportGeometry, SIGNAL(triggered()), this, SLOT(OnParaExportGeometrySetup()));

		//保存子菜单项，在退出定制时清除菜单项。
		_actionList.append(_meshSurfaceCheckbox);
		_actionList.append(_meshSolidCheckbox);
		_actionList.append(_meshCheckingCheckbox);
		_actionList.append(_meshCreateCheckbox);

		connect(_meshSurfaceCheckbox,SIGNAL(signal_CheckBoxStateChanged(int)),
			this, SLOT(UpdateSurfaceAction(int)));
		connect(_meshSolidCheckbox,SIGNAL(signal_CheckBoxStateChanged(int)),
			this, SLOT(UpdateSolidAction(int)));
		connect(_meshCheckingCheckbox,SIGNAL(signal_CheckBoxStateChanged(int)),
			this, SLOT(UpdateCheckMeshAction(int)));
		connect(_meshCreateCheckbox,SIGNAL(signal_CheckBoxStateChanged(int)),
			this, SLOT(UpdateCreateSetAction(int)));

	}

	//建立求解菜单
	void CustomActionManager::FillSolveMenu()
	{
		_menuSolve = CreateTopMenu(tr("Solve"));

		CreateAction(tr("Solve"), ":/QUI/icon/solve.png", tr("F5"), _menuSolve, _mainWindow, false);
		_solverManager = CreateAction(tr("Solver Manager"), ":/QUI/icon/setting.png", tr(""), _menuSolve, _mainWindow, true);

		connect(_solverManager, SIGNAL(triggered()), this, SLOT(OnParaSolverManager()));
	}

	//建立菜单分隔符
	QAction* CustomActionManager::CreateActionSeparator(QMenu *menu)
	{
		QAction* action = menu->addSeparator();   //添加分割线
		_actionList.append(action);
		return action;
	}

	//建立子菜单项
	QAction* CustomActionManager::CreateAction(QString text, QObject* parent, bool enable)
	{
		QAction*  action = new QAction(text, parent);
		action->setEnabled(enable);
		_actionList.append(action);
		return action;
	}

	//建立子菜单，并将子项插入值顶层菜单中
	QAction* CustomActionManager::CreateAction(QString text, QMenu* parentMenu, QObject* parent, bool enable)
	{
		QAction*  action = new QAction(text, parent);
		action->setEnabled(enable);
		parentMenu->addAction(action);
		_actionList.append(action);
		return action;
	}

	//建立子菜单，并将子项插入值顶层菜单中.带图标和快捷键
	QAction* CustomActionManager::CreateAction(QString text, QString icon, QString ShortcutKey, QMenu* parentMenu, QObject* parent, bool enable)
	{
		QAction*  action = new QAction(text, parent);
		action->setEnabled(enable);
		action->setIcon(QIcon(icon));
		action->setShortcut(ShortcutKey);
		parentMenu->addAction(action);
		_actionList.append(action);
		return action;
	}

	//建立顶层菜单
	QMenu* CustomActionManager::CreateTopMenu(QString text, bool enable)
	{
		if (nullptr == _mainWindow || nullptr == _mainWindow->menuBar())
		{
		//	QMessageBox::critical(NULL, tr("Error"), tr("mainwindow or menuBar is nullptr"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

			QMessageBox msgBox(QMessageBox::Critical, tr("Error"), tr("mainwindow or menuBar is nullptr"));
			msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
			msgBox.setButtonText(QMessageBox::Yes, tr("Yes"));
			msgBox.setButtonText(QMessageBox::No, tr("No"));
		}

		QMenu* menu = new QMenu(text);
		_mainWindow->menuBar()->insertMenu(_cusAction, menu);
		menu->setEnabled(enable);
		_menuAndtoolbarList.append(menu);
		return menu;
	}

	//卸载插件时，删除子菜单和顶层菜单以及工具栏
	template <typename T>
	void CustomActionManager::RemoveList(QList<T> list)
	{
		for (int i = 0; i < list.count(); i++)
		{
			T item = list.at(i);

			if (nullptr == item)
				continue;

			delete item;
			item = nullptr;
		}
	}


	//建立子项，并将其插入工具栏，带图标
	QAction* CustomActionManager::CreateAction(QString text, QString icon, QToolBar* parentTool, QObject* parent)
	{
		QAction*  action = new QAction(text, parent);
		action->setIcon(QIcon(icon));
		//action->setEnabled(false);
		parentTool->addAction(action);
		parentTool->setMovable(true);
		_actionList.append(action);
		//_menuAndtoolbarList.append(parentTool);
		return action;
	}

	//创建工具栏
	QToolBar* CustomActionManager::CreateToolBar(QString text)
	{
		QToolBar* toolbar = _mainWindow->addToolBar(text);
		_menuAndtoolbarList.append(toolbar);

		return toolbar;
	}

	//建立文件工具栏
	void CustomActionManager::FillFileToolBar()
	{
		_fileToolBar = CreateToolBar("file");
		//_fileToolBar = _mainWindow->addToolBar("File");
		//_menuAndtoolbarList.append(_fileToolBar);

		CreateAction(tr("New"), ":/QUI/icon/createNew.png", _fileToolBar, _mainWindow);
		CreateAction(tr("Open"), ":/QUI/icon/open.png", _fileToolBar, _mainWindow);
		CreateAction(tr("Save"), ":/QUI/icon/save.png", _fileToolBar, _mainWindow);
		_importMeshAction = CreateAction(tr("Import Mesh"), ":/QUI/icon/importMesh.png", _fileToolBar, _mainWindow);
		_importGeoAction = CreateAction(tr("Import Geometry"), ":/QUI/icon/importGeometry.png", _fileToolBar, _mainWindow);

	}

	//建立网格剖分工具栏
	void CustomActionManager::FillSplitMeshToolBar()
	{
		_splitMeshToolBar = CreateToolBar("SplitMesh");
		//_splitMeshToolBar = _mainWindow->addToolBar("SplitMesh");
		//_menuAndtoolbarList.append(_splitMeshToolBar);

		_surfaceMesh = CreateAction(tr("SurfaceMesh"), ":/QUI/icon/surfaceMesh.png", _splitMeshToolBar, _mainWindow);
		_solidMesh = CreateAction(tr("SolidMesh"), ":/QUI/icon/solidMesh.png", _splitMeshToolBar, _mainWindow);
		CreateAction(tr("GenMesh"), ":/QUI/icon/solumationsetting.png", _splitMeshToolBar, _mainWindow);
	}

	//建立选择工具栏
	void CustomActionManager::FillSelectToolBar()
	{
		_selectToolBar = CreateToolBar("Select");
		//_selectToolBar = _mainWindow->addToolBar("Select");
		//_menuAndtoolbarList.append(_selectToolBar);

		_selectOff = CreateAction(tr("selectOff"), ":/QUI/icon/selectOff.png", _selectToolBar, _mainWindow);
		_meshSelect1 = CreateAction(tr("selectMeshNode"), ":/QUI/icon/selectNode.png", _selectToolBar, _mainWindow);
		_meshSelect2 = CreateAction(tr("selectMeshCell"), ":/QUI/icon/selectElement.png", _selectToolBar, _mainWindow);
		_meshSelect3 = CreateAction(tr("BoxMeshNode"), ":/QUI/icon/boxNode.png", _selectToolBar, _mainWindow);
		_meshSelect4 = CreateAction(tr("BoxMeshCell"), ":/QUI/icon/boxCell.png", _selectToolBar, _mainWindow);
		_selectToolBar->addSeparator();
		_geoSelect1 = CreateAction(tr("SelectPoint"), ":/QUI/geometry/selectpoint.png", _selectToolBar, _mainWindow);
		_geoSelect2 = CreateAction(tr("SelectCurve"), ":/QUI/geometry/selectwire.png", _selectToolBar, _mainWindow);
		_geoSelect3 = CreateAction(tr("SelectFace"), ":/QUI/geometry/selectface.png", _selectToolBar, _mainWindow);
		_geoSelect4 = CreateAction(tr("SelectGeometryBody"), ":/QUI/geometry/selectbody.png", _selectToolBar, _mainWindow);
	}

	//建立查看网格几何工具栏
	void CustomActionManager::FillViewToolBar()
	{
		_viewToolBar = CreateToolBar("View");
		//_viewToolBar = _mainWindow->addToolBar("View");
		//_menuAndtoolbarList.append(_viewToolBar);

		_meshDisplay1 = CreateAction(tr("DisplayNode"), ":/QUI/icon/node.png", _viewToolBar, _mainWindow);
		_meshDisplay2 = CreateAction(tr("DisplayWireFrame"), ":/QUI/icon/wireFrame.png", _viewToolBar, _mainWindow);
		_meshDisplay3 = CreateAction(tr("DisplaySurface"), ":/QUI/icon/face.png", _viewToolBar, _mainWindow);
		_meshDisplay4 = CreateAction(tr("DisplaySurfaceEdge"), ":/QUI/icon/faceWithEdge.png", _viewToolBar, _mainWindow);
		_viewToolBar->addSeparator();
		_geoDisplay1 = CreateAction(tr("DisplayPoint"), ":/QUI/geometry/pointDisplay.png", _viewToolBar, _mainWindow);
		_geoDisplay2 = CreateAction(tr("DisplayCurve"), ":/QUI/geometry/edgeDisplay.png", _viewToolBar, _mainWindow);
		_geoDisplay3 = CreateAction(tr("DisplayFace"), ":/QUI/geometry/facedisplay.png", _viewToolBar, _mainWindow);
	}

	//建立求解工具栏
	void CustomActionManager::FillSolveToolBar()
	{
		_solveToolBar = CreateToolBar("Solve");
		//_solveToolBar = _mainWindow->addToolBar("Solve");
		//_menuAndtoolbarList.append(_solveToolBar);

		CreateAction(tr("Solve"), ":/QUI/icon/solve.png", _solveToolBar, _mainWindow);
	}

	//建立视图工具栏
	void CustomActionManager::FillViewerToolBar()
	{
		_viewerToolBar = CreateToolBar("Viewer");
		//_viewerToolBar = _mainWindow->addToolBar("Viewer");
		//_menuAndtoolbarList.append(_viewerToolBar);

		CreateAction(tr("FitView"), ":/QUI/icon/fit.png", _viewerToolBar, _mainWindow);
		CreateAction(tr("ViewXPlus"), ":/QUI/icon/xPlus.png", _viewerToolBar, _mainWindow);
		CreateAction(tr("ViewXMinus"), ":/QUI/icon/xMinus.png", _viewerToolBar, _mainWindow);
		CreateAction(tr("ViewYPlus"), ":/QUI/icon/yPlus.png", _viewerToolBar, _mainWindow);
		CreateAction(tr("ViewYMinus"), ":/QUI/icon/yMinus.png", _viewerToolBar, _mainWindow);
		CreateAction(tr("ViewZPlus"), ":/QUI/icon/zPlus.png", _viewerToolBar, _mainWindow);
		CreateAction(tr("ViewZMinus"), ":/QUI/icon/zMinus.png", _viewerToolBar, _mainWindow);
		_viewerToolBar->addSeparator();
	}

	//建立网格检测创建组件工具栏
	void CustomActionManager::FillCheckCreatToolBar()
	{
		_checkCreatToolBar = CreateToolBar("CheckCreate");
		//_checkCreatToolBar = _mainWindow->addToolBar("CheckCreate");
		//_menuAndtoolbarList.append(_checkCreatToolBar);

		_meshCheck = CreateAction(tr("Checking"), ":/QUI/icon/meshChecking.png", _checkCreatToolBar, _mainWindow);
		_meshCreateSet = CreateAction(tr("Create Set"), ":/QUI/icon/createSet.png", _checkCreatToolBar, _mainWindow);
	}

	//建立撤销恢复工具栏
	void CustomActionManager::FillURDoToolBar()
	{
		_URDoToolBar = CreateToolBar("URDo");
		//_URDoToolBar = _mainWindow->addToolBar("URDo");
		//_menuAndtoolbarList.append(_URDoToolBar);

		_geoUndo = CreateAction(tr("undo"), ":/QUI/geometry/undo.png", _URDoToolBar, _mainWindow);
		_geoRedo = CreateAction(tr("redo"), ":/QUI/geometry/redo.png", _URDoToolBar, _mainWindow);
	}

	//建立草绘工具栏
	void CustomActionManager::FillCreateSketchToolBar()
	{
		_createSketchToolBar = CreateToolBar("CreateSketch");
		//_createSketchToolBar = _mainWindow->addToolBar("CreateSketch");
		//_menuAndtoolbarList.append(_createSketchToolBar);

		CreateAction(tr("Create Sketch"), ":/QUI/icon/createSketch.png", _createSketchToolBar, _mainWindow);
	}

	//建立几何建模工具栏
	void CustomActionManager::FillFeatureModingToolBar()
	{
		_featureModingToolBar = CreateToolBar("FeatureModing");
		//_featureModingToolBar = _mainWindow->addToolBar("FeatureModing");
		//_menuAndtoolbarList.append(_featureModingToolBar);

		CreateAction(tr("CreateBox"), ":/QUI/geometry/box.png", _featureModingToolBar, _mainWindow);
		CreateAction(tr("CreateCylinder"), ":/QUI/geometry/cylinder.png", _featureModingToolBar, _mainWindow);
		CreateAction(tr("CreaterCone"), ":/QUI/geometry/cone.png", _featureModingToolBar, _mainWindow);
		CreateAction(tr("CreaterSphere"), ":/QUI/geometry/sphere.png", _featureModingToolBar, _mainWindow);
		CreateAction(tr("Create_Surface"), ":/QUI/geometry/face.png", _featureModingToolBar, _mainWindow);
		CreateAction(tr("Create Line"), ":/QUI/geometry/line.png", _featureModingToolBar, _mainWindow);
		CreateAction(tr("Create Point"), ":/QUI/geometry/point.png", _featureModingToolBar, _mainWindow);
		CreateAction(tr("Create Datum Plane"), ":/QUI/icon/datumPlane.png", _featureModingToolBar, _mainWindow);
	}

	//建立几何处理工具栏1
	void CustomActionManager::FillFeatureOperations1ToolBar()
	{
		_featureOperations1ToolBar = CreateToolBar("FeatureOperations1");
		//_featureOperations1ToolBar = _mainWindow->addToolBar("FeatureOperations1");
		//_menuAndtoolbarList.append(_featureOperations1ToolBar);

		CreateAction(tr("Chamfer"), ":/QUI/geometry/chamfer.png", _featureOperations1ToolBar, _mainWindow);
		CreateAction(tr("Fillet"), ":/QUI/geometry/fillet.png", _featureOperations1ToolBar, _mainWindow);
		CreateAction(tr("Variable Fillet"), ":/QUI/geometry/variableFillet.png", _featureOperations1ToolBar, _mainWindow);
	}

	//建立几何处理工具栏2
	void CustomActionManager::FillFeatureOperations2ToolBar()
	{
		_featureOperations2ToolBar = CreateToolBar("FeatureOperations2");
		//_featureOperations2ToolBar = _mainWindow->addToolBar("FeatureOperations2");
		//_menuAndtoolbarList.append(_featureOperations2ToolBar);

		CreateAction(tr("BoolCut"), ":/QUI/geometry/cut.png", _featureOperations2ToolBar, _mainWindow);
		CreateAction(tr("BoolFause"), ":/QUI/geometry/fuse.png", _featureOperations2ToolBar, _mainWindow);
		CreateAction(tr("BoolCommon"), ":/QUI/geometry/common.png", _featureOperations2ToolBar, _mainWindow);
	}

	//建立几何处理工具栏3
	void CustomActionManager::FillFeatureOperations3ToolBar()
	{
		_featureOperations3ToolBar = CreateToolBar("FeatureOperations3");
		//_featureOperations3ToolBar = _mainWindow->addToolBar("FeatureOperations3");
		//_menuAndtoolbarList.append(_featureOperations3ToolBar);

		CreateAction(tr("MirrorFeature"), ":/QUI/geometry/mirror.png", _featureOperations3ToolBar, _mainWindow);
		CreateAction(tr("Move"), ":/QUI/geometry/move.png", _featureOperations3ToolBar, _mainWindow);
		CreateAction(tr("Rotate"), ":/QUI/geometry/Rotate.png", _featureOperations3ToolBar, _mainWindow);
		CreateAction(tr("MakeMatrix"), ":/QUI/geometry/matrix.png", _featureOperations3ToolBar, _mainWindow);
		CreateAction(tr("Extrude"), ":/QUI/geometry/extrude.png", _featureOperations3ToolBar, _mainWindow);
		CreateAction(tr("Revol"), ":/QUI/geometry/revolve.png", _featureOperations3ToolBar, _mainWindow);
		CreateAction(tr("loft"), ":/QUI/geometry/loft.png", _featureOperations3ToolBar, _mainWindow);
		CreateAction(tr("Sweep"), ":/QUI/geometry/sweep.png", _featureOperations3ToolBar, _mainWindow);
	}

	/*void CustomActionManager::UpdateActionStatus()
	{
		auto geoConfig = FastCAEDesigner::DataManager::getInstance()->getGeoConfig();

		_geoFeatureModelingCheckbox->setChecked(geoConfig->isGeometryModelingEnabled());
		_geoFeatureOperationCheckbox->setChecked(geoConfig->isGeometryOperationsEnabled());
		_geoDraftCheckbox->setChecked(geoConfig->isCreateSketchEnabled());
	}*/
	//更新创建草图工具栏
	void CustomActionManager::UpdateCreateSketchToolBar(int status)
	{
		bool b = (status > 0) ? true : false;
		FastCAEDesigner::DataManager::getInstance()->SetGeometryCreateSketch(b);
		_createSketchToolBar->setVisible(b);

		if ((b == true) || (_geoFeatureModelingCheckbox->getCheckBoxChecked()) || (_geoFeatureOperationCheckbox->getCheckBoxChecked()))
			_geoOperateNull = false;
		else
			_geoOperateNull = true;
		
		updateGeometrySelectAndView();
	}
	//更新特征建模工具栏
	void CustomActionManager::UpdateFeatureModingToolBar(int status)
	{
		bool b = (status > 0) ? true : false;
		FastCAEDesigner::DataManager::getInstance()->SetGeometryFeatureModeling(b);
		_featureModingToolBar->setVisible(b);	

		if ((b == true) || (_geoDraftCheckbox->getCheckBoxChecked()) || (_geoFeatureOperationCheckbox->getCheckBoxChecked()))
			_geoOperateNull = false;
		else
			_geoOperateNull = true;

		updateGeometrySelectAndView();
	}
	//更新特征操作工具栏
	void CustomActionManager::UpdateFeatureOperationsToolBar(int status)
	{
		bool b = (status > 0) ? true : false;
		FastCAEDesigner::DataManager::getInstance()->SetGeometryFeatureOperatins(b);
		_featureOperations1ToolBar->setVisible(b);
		_featureOperations2ToolBar->setVisible(b);
		_featureOperations3ToolBar->setVisible(b);

		if ((b == true) || (_geoFeatureModelingCheckbox->getCheckBoxChecked()) || (_geoDraftCheckbox->getCheckBoxChecked()))
			_geoOperateNull = false;
		else
			_geoOperateNull = true;

		updateGeometrySelectAndView();
	}
	//更新Mesh相关操作对应工具栏
	void CustomActionManager::UpdateSurfaceAction(int status)
	{
		bool b = (status > 0) ? true : false;
		FastCAEDesigner::DataManager::getInstance()->SetSurfaceMesh(b);
		_surfaceMesh->setVisible(b);
		
		if ((b == true) || (_meshSolidCheckbox->getCheckBoxChecked()))
			_meshOperateNull = false;
		else
			_meshOperateNull = true;

		updateMeshSelectAndView();
	}
	void CustomActionManager::UpdateSolidAction(int status)
	{
		bool b = (status > 0) ? true : false;
		FastCAEDesigner::DataManager::getInstance()->SetSolidMesh(b);
		_solidMesh->setVisible(b);

		if ((b == true) || (_meshSurfaceCheckbox->getCheckBoxChecked()))
			_meshOperateNull = false;
		else
			_meshOperateNull = true;

		updateMeshSelectAndView();
	}
	void CustomActionManager::UpdateCheckMeshAction(int status)
	{
		bool b = (status > 0) ? true : false;
		FastCAEDesigner::DataManager::getInstance()->SetCheckMesh(b);
		_meshCheck->setVisible(b);
	}
	void CustomActionManager::UpdateCreateSetAction(int status)
	{
		bool b = (status > 0) ? true : false;
		FastCAEDesigner::DataManager::getInstance()->SetMeshCreateSet(b);
		_meshCreateSet->setVisible(b);
	}
	void CustomActionManager::updateImportGeometryAction(QString suffix)
	{
		if (suffix.isEmpty())
			_importGeoAction->setVisible(false);
		else
			_importGeoAction->setVisible(true);
	}
	void CustomActionManager::updateImportMeshAction(QString suffix)
	{
		if (suffix.isEmpty())
			_importMeshAction->setVisible(false);
		else
			_importMeshAction->setVisible(true);
	}
	//初始化菜单工具栏
	void CustomActionManager::initActionstatus(FastCAEDesigner::QFWidgetAction* action,bool on)
	{
		if (action == nullptr)
			return;
		action->SetChechBoxChecked(on);
	}
	void CustomActionManager::initMenuAndToolBar()
	{
		//初始化菜单工具栏
		bool isGeoModeling = FastCAEDesigner::DataManager::getInstance()->GetGeometryFeatureModeling();
		bool isGeoOperations = FastCAEDesigner::DataManager::getInstance()->GetGeometryFeatureOperatins();
		bool isCreateSketch = FastCAEDesigner::DataManager::getInstance()->GetGeometryCreateSketch();

		if ((isGeoModeling == false) && (isGeoOperations == false) && (isCreateSketch == false))
			_geoOperateNull = true;

		bool isSurfaceMesh = FastCAEDesigner::DataManager::getInstance()->GetSurfaceMesh();
		bool isSolidMesh = FastCAEDesigner::DataManager::getInstance()->GetSolidMesh();
		bool isCheckMesh = FastCAEDesigner::DataManager::getInstance()->GetCheckMesh();
		bool isCreateSet = FastCAEDesigner::DataManager::getInstance()->GetMeshCreateSet();

		if ((isSurfaceMesh == false) && (isSolidMesh == false))
			_meshOperateNull = true;

		QString importGeoSuffix = FastCAEDesigner::DataManager::getInstance()->GetImportGeometrySuffix();
		QString exportGeoSuffix = FastCAEDesigner::DataManager::getInstance()->GetExportGeometrySuffix();
		QString importMeshSuffix = FastCAEDesigner::DataManager::getInstance()->GetImportMeshSuffix();
		QString exportMeshSuffix = FastCAEDesigner::DataManager::getInstance()->GetExportMeshSuffix();

		if ((importMeshSuffix.isEmpty()) && (exportMeshSuffix.isEmpty()))
			_meshSuffixNull = true;
		
		if ((importGeoSuffix.isEmpty()) && (exportGeoSuffix.isEmpty()))
			_geoSuffixNull = true;

		updateImportMeshAction(importMeshSuffix);
		updateImportGeometryAction(importGeoSuffix);

		initActionstatus(_geoFeatureModelingCheckbox, isGeoModeling);
		initActionstatus(_geoFeatureOperationCheckbox, isGeoOperations);
		initActionstatus(_geoDraftCheckbox, isCreateSketch);
		initActionstatus(_meshSurfaceCheckbox, isSurfaceMesh);
		initActionstatus(_meshSolidCheckbox, isSolidMesh);
		initActionstatus(_meshCheckingCheckbox, isCheckMesh);
		initActionstatus(_meshCreateCheckbox, isCreateSet);

		_featureModingToolBar->setVisible(isGeoModeling);
		_featureOperations1ToolBar->setVisible(isGeoOperations);
		_featureOperations2ToolBar->setVisible(isGeoOperations);
		_featureOperations3ToolBar->setVisible(isGeoOperations);
		_createSketchToolBar->setVisible(isCreateSketch);
		_surfaceMesh->setVisible(isSurfaceMesh);
		_solidMesh->setVisible(isSolidMesh);
		_meshCheck->setVisible(isCheckMesh);
		_meshCreateSet->setVisible(isCreateSet);

		updateGeometrySelectAndView();
		updateMeshSelectAndView();

	}

	void CustomActionManager::reTranslate()
	{
		_menu->setTitle(tr("Customizer"));

		_startAction->setText(tr("Start Customizer"));
		_finishAction->setText(tr("Finish Customizer"));
		_cancelAction->setText(tr("Cancel Customizer"));
		_shareAction->setText(tr("Share & Release"));
		
	}

	void CustomActionManager::quitCustomizer()
	{
		//Added xvdongming 
		RemoveList<QAction*>(_actionList);
		RemoveList<QWidget*>(_menuAndtoolbarList);
		delete _functionTreeSetup;
		_functionTreeSetup = nullptr;
		//Added xvdongming 

		delete _editorSolver;
		_editorSolver = nullptr;

		_plugin->isUsing(false);
		_startAction->setEnabled(true);
		_finishAction->setEnabled(false);
		_cancelAction->setEnabled(false);
		_shareAction->setEnabled(true);
		_mainWindow->getCustomizerHelper()->finishCustomizer();
	}

	void CustomActionManager::OnParaSolverManager()
	{
		_editorSolver = new FastCAEDesigner::EditorSolverManager(_mainWindow);
		_editorSolver->exec();
	}

	void CustomActionManager::copySolverDependencyFiles(QList<QStringList> dependencyfiles,QString path)
	{
		const int n = dependencyfiles.count();
		for (int i = 0; i < n; i++)
		{
			QStringList names = dependencyfiles.at(i);
			for (int j = 0; j < names.count(); j++)
			{
				FastCAEDesigner::DataManager::getInstance()->CopyFileToSystem(names.at(j), path);
			}
		}
	}

	void CustomActionManager::updateMeshSelectAndView()
	{
		if ((_meshSuffixNull == false) || (_meshOperateNull == false))
		{
			_meshSelect1->setVisible(true);
			_meshSelect2->setVisible(true);
			_meshSelect3->setVisible(true);
			_meshSelect4->setVisible(true);
			_meshDisplay1->setVisible(true);
			_meshDisplay2->setVisible(true);
			_meshDisplay3->setVisible(true);
			_meshDisplay4->setVisible(true);
		}
		else
		{
			_meshSelect1->setVisible(false);
			_meshSelect2->setVisible(false);
			_meshSelect3->setVisible(false);
			_meshSelect4->setVisible(false);
			_meshDisplay1->setVisible(false);
			_meshDisplay2->setVisible(false);
			_meshDisplay3->setVisible(false);
			_meshDisplay4->setVisible(false);
		}

		updateSelectOff();
	}

	void CustomActionManager::updateGeometrySelectAndView()
	{
		if ((_geoSuffixNull == false) || (_geoOperateNull == false))
		{
			_geoSelect1->setVisible(true);
			_geoSelect2->setVisible(true);
			_geoSelect3->setVisible(true);
			_geoSelect4->setVisible(true);
			_geoDisplay1->setVisible(true);
			_geoDisplay2->setVisible(true);
			_geoDisplay3->setVisible(true);
			_geoUndo->setVisible(true);
			_geoRedo->setVisible(true);
		}
		else
		{
			_geoSelect1->setVisible(false);
			_geoSelect2->setVisible(false);
			_geoSelect3->setVisible(false);
			_geoSelect4->setVisible(false);
			_geoDisplay1->setVisible(false);
			_geoDisplay2->setVisible(false);
			_geoDisplay3->setVisible(false);
			_geoUndo->setVisible(false);
			_geoRedo->setVisible(false);
		}

		updateSelectOff();
	}

	void CustomActionManager::updateSelectOff()
	{
		if ((_meshOperateNull == false) || (_geoOperateNull == false) || (_meshSuffixNull == false) || (_geoSuffixNull == false))
			_selectOff->setVisible(true);
		else
			_selectOff->setVisible(false);
	}

}
