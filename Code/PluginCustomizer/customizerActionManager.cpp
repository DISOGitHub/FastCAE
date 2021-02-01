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
#include "SARibbonBar/SARibbonCategory.h"
#include "SARibbonBar/SARibbonMenu.h"
#include "SARibbonBar/SARibbonBar.h"
#include "SARibbonBar/SARibbonPannel.h"

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
		//xuxinwei 20200324 
		FastCAEDesigner::DataManager::getInstance()->clearIconNameList();
		_mainWindow->getCustomizerHelper()->startCustomizer();
		//Added xvdongming
		//给出正在定制的标识
		QString title = _mainWindow->windowTitle();
		_mainWindow->setWindowTitle(title + " ---- " + tr("Customizing"));
		FastCAEDesigner::DataManager::getInstance()->ReadInfoFromServerToLocal();

		if (!_mainWindow->isUseRibbon())
		{
			_actionList.clear();
			_menuAndtoolbarList.clear();

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
			fillGeometryEditToolBar();
			fillMeasuredDistanceToolBar();

			//Added xvdongming
			initMenuAndToolBar();	//初始化菜单工具栏
		}
		else
		{
			//auto pannels = _custom_page->pannelList();
			//_mainWindow->getUi()->changeActionState(Ui::MainWindowRibbon::customState);
		}
		_startAction->setEnabled(false);
		_finishAction->setEnabled(true);
		_cancelAction->setEnabled(true);
		_shareAction->setEnabled(false);
		//libaojun
		OnParaFunctionParaSetup();
	}

	//卸载插件
	void CustomActionManager::release()
	{
		if (_mainWindow->isUseRibbon())
		{
			delete _startAction;
			delete _finishAction;
			delete _cancelAction;
			delete _shareAction;
			_startAction = nullptr;
			_finishAction = nullptr;
			_cancelAction = nullptr;
			_shareAction = nullptr;

			QList<SARibbonPannel*> pannels = _custom_page->pannelList();
			for (SARibbonPannel* pannel : pannels)
			{
				_custom_page->removePannel(pannel);
				pannel = nullptr;
			}

			SARibbonBar* ribbon = _mainWindow->ribbonBar();
			ribbon->removeCategory(_custom_page);
			_custom_page = nullptr;
		}
		else
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
	}

	//完成定制
	void CustomActionManager::on_finishCustom_slot()
	{
		FastCAEDesigner::DataManager::getInstance()->SetPhysicsList(_functionTreeSetup->GetPhysicsList());
		FastCAEDesigner::DataManager::getInstance()->SetMaterialList(_functionTreeSetup->GetMaterialList());
		FastCAEDesigner::DataManager::getInstance()->WriteInfoToServerPath(_mainWindow);

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
		QStringList list;
		list << tr("Start Customizer") << tr("Finish Customizer") << tr("Cancel Customizer") << tr("Share & Release");
		auto value = _mainWindow->createWidgetAndAction(des, list);
		QList<QAction*> actions = value.second;
		for (QAction* action : actions)
		{
			if (action->objectName() == tr("Start Customizer"))
			{
				connect(action, SIGNAL(triggered()), this, SLOT(on_startCustom_slot()));
				_startAction = action;
				_startAction->setText(tr("Start Customizer"));
				QIcon startCustomizer_icon;
				startCustomizer_icon.addFile(QString::fromUtf8(":/startCustomizer.png"), QSize(), QIcon::Normal, QIcon::Off);
				_startAction->setIcon(startCustomizer_icon);
			}
			else if (action->objectName() == tr("Finish Customizer"))
			{
				connect(action, SIGNAL(triggered()), this, SLOT(on_finishCustom_slot()));
				_finishAction = action;
				_finishAction->setText(tr("Finish Customizer"));
				QIcon finishCustomizer_icon;
				finishCustomizer_icon.addFile(QString::fromUtf8(":/finishCustomizer.png"), QSize(), QIcon::Normal, QIcon::Off);
				_finishAction->setIcon(finishCustomizer_icon);
			}
			else if (action->objectName() == tr("Cancel Customizer"))
			{
				connect(action, SIGNAL(triggered()), this, SLOT(on_cancelCustom_slot()));
				_cancelAction = action;
				_cancelAction->setText(tr("Cancel Customizer"));
				QIcon cancelCustomizer_icon;
				cancelCustomizer_icon.addFile(QString::fromUtf8(":/cancelCustomizer.png"), QSize(), QIcon::Normal, QIcon::Off);
				_cancelAction->setIcon(cancelCustomizer_icon);
			}
			else if (action->objectName() == tr("Share & Release"))
			{
				connect(action, SIGNAL(triggered()), this, SLOT(on_share_slot()));
				_shareAction = action;
				_shareAction->setText(tr("Share & Release"));
				QIcon shareRelease_icon;
				shareRelease_icon.addFile(QString::fromUtf8(":/shareRelease.png"), QSize(), QIcon::Normal, QIcon::Off);
				_shareAction->setIcon(shareRelease_icon);
			}
		}

		_startAction->setEnabled(true);
		_finishAction->setEnabled(false);
		_cancelAction->setEnabled(false);
		QWidget* widget = value.first;

		if (_mainWindow->isUseRibbon())
		{
			_custom_page = dynamic_cast<SARibbonCategory*>(widget);
			_custom_page->setWindowTitle(tr("Customizer"));
		}
		else 
		{
			_menu = dynamic_cast<QMenu*>(widget);
			_cusAction = _mainWindow->menuBar()->insertMenu(nullptr, _menu);
		}
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

		_geometryEditCheckbox = new FastCAEDesigner::QFWidgetAction(_menuGeometry);
		_geometryEditCheckbox->SetText(tr("Geometry Edit"));
		_measuredDistanceCheckbox = new FastCAEDesigner::QFWidgetAction(_menuGeometry);
		_measuredDistanceCheckbox->SetText(tr("Measure Distance"));
		_geoSetCreateCheckbox = new FastCAEDesigner::QFWidgetAction(_menuGeometry);
		_geoSetCreateCheckbox->SetText(tr("CreateGeoComponent"));

		_menuGeometry->addAction(_geometryEditCheckbox);
		_menuGeometry->addAction(_measuredDistanceCheckbox);
		_menuGeometry->addAction(_geoSetCreateCheckbox);
		_actionList.append(_geometryEditCheckbox);
		_actionList.append(_measuredDistanceCheckbox);
		_actionList.append(_geoSetCreateCheckbox);
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
		connect(_geometryEditCheckbox, SIGNAL(signal_CheckBoxStateChanged(int)),
			this, SLOT(updateGeometryEditToolBar(int)));
		connect(_measuredDistanceCheckbox, SIGNAL(signal_CheckBoxStateChanged(int)),
			this, SLOT(updateMeasuredDistanceToolBar(int)));
		connect(_geoSetCreateCheckbox, SIGNAL(signal_CheckBoxStateChanged(int)),
			this, SLOT(updateCreateGeoSetAction(int)));
		
	}

	//建立网格菜单
	void CustomActionManager::FillMeshMenu()
	{
		_menuMesh = CreateTopMenu(tr("Mesh"));


		/*FastCAEDesigner::QFWidgetAction *_meshSurfaceCheckbox{};
		FastCAEDesigner::QFWidgetAction *_meshSolidCheckbox{};*/

		_meshSurfaceCheckbox = new FastCAEDesigner::QFWidgetAction(_menuMesh);
		_meshSurfaceCheckbox->SetText(tr("SurfaceMesh"));

		_meshSolidCheckbox = new FastCAEDesigner::QFWidgetAction(_menuMesh);
		_meshSolidCheckbox->SetText(tr("SolidMesh"));

		_meshFluidCheckbox = new FastCAEDesigner::QFWidgetAction(_menuMesh);
		_meshFluidCheckbox->SetText(tr("FluidMesh"));

		_menuMesh->addAction(_meshSurfaceCheckbox);
		_menuMesh->addAction(_meshSolidCheckbox);
		_menuMesh->addAction(_meshFluidCheckbox);

		CreateAction(tr("Gen mesh"), ":/QUI/icon/solumationsetting.png", tr(""), _menuMesh, _mainWindow);
		CreateActionSeparator(_menuMesh);

		/*FastCAEDesigner::QFWidgetAction *_meshCheckingCheckbox{};
		FastCAEDesigner::QFWidgetAction *_meshCreateCheckbox{};*/

		_meshCheckingCheckbox = new FastCAEDesigner::QFWidgetAction(_menuMesh);
		_meshCheckingCheckbox->SetText(tr("Checking"));

		_meshCreateCheckbox = new FastCAEDesigner::QFWidgetAction(_menuMesh);
		_meshCreateCheckbox->SetText(tr("Create set"));

		_meshFilterCheckbox = new FastCAEDesigner::QFWidgetAction(_menuMesh);
		_meshFilterCheckbox->SetText(tr("FilterMesh"));

		_meshModelingCheckbox = new FastCAEDesigner::QFWidgetAction(_menuMesh);
		_meshModelingCheckbox->SetText(tr("Mesh Modeling"));

		_menuMesh->addAction(_meshCheckingCheckbox);
		_menuMesh->addAction(_meshFilterCheckbox);
		_menuMesh->addAction(_meshCreateCheckbox);
		_menuMesh->addAction(_meshModelingCheckbox);

		CreateAction(tr("Views"), _menuMesh, _mainWindow);
		CreateAction(tr("Select"), _menuMesh, _mainWindow);

		//connect(_importGeometry, SIGNAL(triggered()), this, SLOT(OnParaImportGeometrySetup()));
		//connect(_exportGeometry, SIGNAL(triggered()), this, SLOT(OnParaExportGeometrySetup()));

		//保存子菜单项，在退出定制时清除菜单项。
		_actionList.append(_meshSurfaceCheckbox);
		_actionList.append(_meshSolidCheckbox);
		_actionList.append(_meshCheckingCheckbox);
		_actionList.append(_meshCreateCheckbox);
		_actionList.append(_meshFluidCheckbox);
		_actionList.append(_meshFilterCheckbox);
		_actionList.append(_meshModelingCheckbox);


		connect(_meshSurfaceCheckbox,SIGNAL(signal_CheckBoxStateChanged(int)),
			this, SLOT(UpdateSurfaceAction(int)));
		connect(_meshSolidCheckbox,SIGNAL(signal_CheckBoxStateChanged(int)),
			this, SLOT(UpdateSolidAction(int)));
		connect(_meshCheckingCheckbox,SIGNAL(signal_CheckBoxStateChanged(int)),
			this, SLOT(UpdateCheckMeshAction(int)));
		connect(_meshCreateCheckbox,SIGNAL(signal_CheckBoxStateChanged(int)),
			this, SLOT(UpdateCreateSetAction(int)));
		connect(_meshFluidCheckbox, SIGNAL(signal_CheckBoxStateChanged(int)),
			this, SLOT(updateFluidAction(int)));
		connect(_meshFilterCheckbox, SIGNAL(signal_CheckBoxStateChanged(int)),
			this, SLOT(updateFilterMeshAction(int)));
		connect(_meshModelingCheckbox, SIGNAL(signal_CheckBoxStateChanged(int)),
			this, SLOT(updateMeshModelingAction(int)));

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
		_fluidMesh = CreateAction(tr("FluidMesh"), ":/QUI/icon/meshFluid.png", _splitMeshToolBar, _mainWindow);
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
		_filterMesh = CreateAction(tr("FilterMesh"), ":/QUI/icon/meshFilter.png", _checkCreatToolBar, _mainWindow);
		_meshCreateSet = CreateAction(tr("Create Set"), ":/QUI/icon/meshComponent.png", _checkCreatToolBar, _mainWindow);
		_geoSetCreate = CreateAction(tr("CreateGeoComponent"), ":/QUI/geometry/geoComponent.png", _checkCreatToolBar, _mainWindow);
		_meshModeling = CreateAction(tr("Mesh Modeling"), ":/QUI/icon/meshmodeling.png", _checkCreatToolBar, _mainWindow);
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

	void CustomActionManager::fillGeometryEditToolBar()
	{
		_geometryEditToolBar = CreateToolBar("GeometryEdit");

		CreateAction(tr("Split"), ":/QUI/geometry/geoSSplit.png", _geometryEditToolBar, _mainWindow);
		CreateAction(tr("FillHole"), ":/QUI/geometry/geoFillHole.png", _geometryEditToolBar, _mainWindow);
		CreateAction(tr("RemoveSurface"), ":/QUI/geometry/geoRemoveFace.png", _geometryEditToolBar, _mainWindow);
		CreateAction(tr("FillGap"), ":/QUI/geometry/geoFixSurface.png", _geometryEditToolBar, _mainWindow);
	}

	void CustomActionManager::fillMeasuredDistanceToolBar()
	{
		_measuredDistanceToolBar = CreateToolBar("MeasuredDistance");
		CreateAction(tr("MeasuredDistance"), ":/QUI/geometry/geoMeasure.png", _measuredDistanceToolBar, _mainWindow);
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

		if ((b == true) || (_geoFeatureModelingCheckbox->getCheckBoxChecked()) || (_geoFeatureOperationCheckbox->getCheckBoxChecked())
			|| (_geometryEditCheckbox->getCheckBoxChecked()))
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

		if ((b == true) || (_geoDraftCheckbox->getCheckBoxChecked()) || (_geoFeatureOperationCheckbox->getCheckBoxChecked())
			|| (_geometryEditCheckbox->getCheckBoxChecked()))
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

		if ((b == true) || (_geoFeatureModelingCheckbox->getCheckBoxChecked()) 
			|| (_geoDraftCheckbox->getCheckBoxChecked()) || (_geometryEditCheckbox->getCheckBoxChecked()))
			_geoOperateNull = false;
		else
			_geoOperateNull = true;

		updateGeometrySelectAndView();
	}

	void CustomActionManager::updateGeometryEditToolBar(int status)
	{
		bool b = (status > 0) ? true : false;
		FastCAEDesigner::DataManager::getInstance()->setGeometryEdit(b);
		_geometryEditToolBar->setVisible(b);

		if ((b == true) || (_geoDraftCheckbox->getCheckBoxChecked()) || (_geoFeatureOperationCheckbox->getCheckBoxChecked())
			|| (_geoFeatureModelingCheckbox->getCheckBoxChecked()))
			_geoOperateNull = false;
		else
			_geoOperateNull = true;

		updateGeometrySelectAndView();
	}

	void CustomActionManager::updateMeasuredDistanceToolBar(int status)
	{
		bool b = (status > 0) ? true : false;
		FastCAEDesigner::DataManager::getInstance()->setMeasureDistance(b);
		_measuredDistanceToolBar->setVisible(b);
	}

	void CustomActionManager::updateCreateGeoSetAction(int status)
	{
		bool b = (status > 0) ? true : false;
		FastCAEDesigner::DataManager::getInstance()->setGeometryCreateSet(b);
		_geoSetCreate->setVisible(b);
	}

	//更新Mesh相关操作对应工具栏
	void CustomActionManager::UpdateSurfaceAction(int status)
	{
		bool b = (status > 0) ? true : false;
		FastCAEDesigner::DataManager::getInstance()->SetSurfaceMesh(b);
		_surfaceMesh->setVisible(b);
		
		if ((b == true) || (_meshSolidCheckbox->getCheckBoxChecked()) || (_meshFluidCheckbox->getCheckBoxChecked()))
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

		if ((b == true) || (_meshSurfaceCheckbox->getCheckBoxChecked()) || (_meshFluidCheckbox->getCheckBoxChecked()))
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

	void CustomActionManager::updateFluidAction(int status)
	{
		bool b = (status > 0) ? true : false;
		FastCAEDesigner::DataManager::getInstance()->setFluidMesh(b);
		_fluidMesh->setVisible(b);

		if ((b == true) || (_meshSurfaceCheckbox->getCheckBoxChecked()) || (_meshSolidCheckbox->getCheckBoxChecked()))
			_meshOperateNull = false;
		else
			_meshOperateNull = true;

		updateMeshSelectAndView();
	}

	void CustomActionManager::updateFilterMeshAction(int status)
	{
		bool b = (status > 0) ? true : false;
		FastCAEDesigner::DataManager::getInstance()->setFilterMesh(b);
		_filterMesh->setVisible(b);
	}

	void CustomActionManager::updateMeshModelingAction(int status)
	{
		bool b = (status > 0) ? true : false;
		FastCAEDesigner::DataManager::getInstance()->setMeshModeling(b);
		_meshModeling->setVisible(b);
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
		bool isGeometryEdit = FastCAEDesigner::DataManager::getInstance()->getGeometryEdit();
		bool isGeoCreateSet = FastCAEDesigner::DataManager::getInstance()->getGeometryCreateSet();
		bool isMeasureDistance = FastCAEDesigner::DataManager::getInstance()->getMeasureDistance();

		if ((isGeoModeling == false) && (isGeoOperations == false) && (isCreateSketch == false) && (isGeometryEdit == false))
			_geoOperateNull = true;

		bool isSurfaceMesh = FastCAEDesigner::DataManager::getInstance()->GetSurfaceMesh();
		bool isSolidMesh = FastCAEDesigner::DataManager::getInstance()->GetSolidMesh();
		bool isCheckMesh = FastCAEDesigner::DataManager::getInstance()->GetCheckMesh();
		bool isCreateSet = FastCAEDesigner::DataManager::getInstance()->GetMeshCreateSet();
		bool isFluidMesh = FastCAEDesigner::DataManager::getInstance()->getFluidMesh();
		bool isFilterMesh = FastCAEDesigner::DataManager::getInstance()->getFilterMesh();
		bool isMeshModeling = FastCAEDesigner::DataManager::getInstance()->getMeshModeling();

		if ((isSurfaceMesh == false) && (isSolidMesh == false) && (isFluidMesh == false))
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
		initActionstatus(_geoSetCreateCheckbox, isGeoCreateSet);
		initActionstatus(_geometryEditCheckbox, isGeometryEdit);
		initActionstatus(_measuredDistanceCheckbox, isMeasureDistance);

		initActionstatus(_meshSurfaceCheckbox, isSurfaceMesh);
		initActionstatus(_meshSolidCheckbox, isSolidMesh);
		initActionstatus(_meshCheckingCheckbox, isCheckMesh);
		initActionstatus(_meshCreateCheckbox, isCreateSet);
		initActionstatus(_meshFluidCheckbox, isFluidMesh);
		initActionstatus(_meshFilterCheckbox, isFilterMesh);
		initActionstatus(_meshModelingCheckbox, isMeshModeling);

		_featureModingToolBar->setVisible(isGeoModeling);
		_featureOperations1ToolBar->setVisible(isGeoOperations);
		_featureOperations2ToolBar->setVisible(isGeoOperations);
		_featureOperations3ToolBar->setVisible(isGeoOperations);
		_createSketchToolBar->setVisible(isCreateSketch);
		_geoSetCreate->setVisible(isGeoCreateSet);
		_geometryEditToolBar->setVisible(isGeometryEdit);
		_measuredDistanceToolBar->setVisible(isMeasureDistance);

		_surfaceMesh->setVisible(isSurfaceMesh);
		_solidMesh->setVisible(isSolidMesh);
		_meshCheck->setVisible(isCheckMesh);
		_meshCreateSet->setVisible(isCreateSet);
		_fluidMesh->setVisible(isFluidMesh);
		_filterMesh->setVisible(isFilterMesh);
		_meshModeling->setVisible(isMeshModeling);

		updateGeometrySelectAndView();
		updateMeshSelectAndView();

	}

	void CustomActionManager::reTranslate()
	{
		_startAction->setText(tr("Start Customizer"));
		_finishAction->setText(tr("Finish Customizer"));
		_cancelAction->setText(tr("Cancel Customizer"));
		_shareAction->setText(tr("Share & Release"));

		if (!_mainWindow->isUseRibbon())
			_menu->setTitle(QCoreApplication::translate("CustomActionManager", "Customizer", nullptr));
		else
			_custom_page->setWindowTitle(QCoreApplication::translate("CustomActionManager", "Customizer", nullptr));
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
