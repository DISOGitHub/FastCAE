#include "CustomizerHelper.h"
#include "mainWindow.h"
#include "signalHandler.h"
#include "SubWindowManager.h"
//#include <ui_MainWindow.h>
#include <ui_mainWindow.h>
#include "ConfigOptions/GeometryConfig.h"
#include "ConfigOptions/MeshConfig.h"
#include "ConfigOptions/PostConfig.h"
#include "ConfigOptions/ProjectTreeConfig.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/ConfigDataReader.h"
#include "ConfigOptions/GlobalConfig.h"
#include "MainWidgets/ControlPanel.h"
#include "MainWidgets/messageWindow.h"
#include <python/PyAgent.h>
#include "settings/busAPI.h"
#include "IO/IOConfig.h"
#include <QAction>
#include <QToolBar>
#include <QMenuBar>
#include <QMessageBox>
#include <QDebug>

namespace GUI
{
	CustomizerHelper::CustomizerHelper(MainWindow* m, Ui::MainWindow* ui)
		:_mainWindow(m), _ui(ui)
	{
		_menuList.append(_ui->menuFile);
		_menuList.append(_ui->menuView);
		_menuList.append(_ui->menuGeometry);
		_menuList.append(_ui->menuMesher);
		_menuList.append(_ui->menuSolve);
		_menuList.append(_ui->menuPost);
		_menuList.append(_ui->menuWindows);
		_menuList.append(_ui->menuSettings);
		_menuList.append(_ui->menuPlugins);
		_menuList.append(_ui->menuHelp);

		_toolBarList.append(_ui->FileToolBar);
		_toolBarList.append(_ui->SolveToolBar);
		_toolBarList.append(_ui->ViewToolBar);
		_toolBarList.append(_ui->SelectToolBar);
		_toolBarList.append(_ui->MeshToolBar);
		_toolBarList.append(_ui->DisplayToolBar);
		_toolBarList.append(_ui->SetToolBar);
		_toolBarList.append(_ui->RedoToolBar);
		_toolBarList.append(_ui->StartSketchToolBar);
		_toolBarList.append(_ui->GeomertryFeatureToolBar);
		_toolBarList.append(_ui->ChamferToolBar);
		_toolBarList.append(_ui->BoolToolBar);
		_toolBarList.append(_ui->FeatureOpertionToolBar);
		_toolBarList.append(_ui->SketchToolBar);
		_toolBarList.append(_ui->MeasureToolBar);

		connect(m, SIGNAL(updateInterfaces()), this, SLOT(registerInterface()));
	}

	void CustomizerHelper::startCustomizer()
	{
		qDebug() << "mainWindow start cus";
 
		for (QToolBar* tb : _toolBarList)
			tb->setVisible(false);

		for (QMenu* me : _menuList)
		{
			QList<QAction*> acs = me->findChildren<QAction*>();
			for (auto a : acs)
				a->setVisible(false);
		}
		_mainWindow->getControlPanel()->updataPropertyTab(nullptr);
		_mainWindow->getSignalHandler()->clearData();
//		_mainWindow->getSubWindowManager()->closeAllSubWindow();
		auto gloc = ConfigOption::ConfigOption::getInstance()->getGlobalConfig();
		QString web = gloc->getWebsite();
		_mainWindow->getSubWindowManager()->openUrl(web);
		_mainWindow->getControlPanel()->setVisible(false);
		_mainWindow->getMessageWindow()->setEnabled(false);
	}

	void CustomizerHelper::finishCustomizer()
	{
		qDebug() << "mainWindow finish cus";

		for (QToolBar* tb : _toolBarList)
			tb->setVisible(true);

		for (QMenu* me : _menuList)
		{
			QList<QAction*> acs = me->findChildren<QAction*>();
			for (auto a : acs)
				a->setVisible(true);
		}
		_ui->SketchToolBar->setVisible(false);
		_mainWindow->getControlPanel()->setVisible(true);
		_mainWindow->getMessageWindow()->setEnabled(true);

		this->registerInterface();

		auto gloc = ConfigOption::ConfigOption::getInstance()->getGlobalConfig();
		QString web = gloc->getWebsite();
		qDebug() << web;
		_mainWindow->getSubWindowManager()->openUrl(web);
		
		
		
	}

	void CustomizerHelper::registerInterface()
	{
		QString error = this->readConfigOptions();
		if (!error.isEmpty())
		{
// 			QMessageBox::StandardButton b = QMessageBox::critical(_mainWindow, "Error", QObject::tr("Error Occured %1 ! still continue?").arg(error), QMessageBox::Yes | QMessageBox::No);
// 			if (b == QMessageBox::No) return;
		}

		this->updateBasicInfo();

		ConfigOption::ConfigOption* option = ConfigOption::ConfigOption::getInstance();
		ConfigOption::GeometryConfig* geometryOption = option->getGeometryConfig();
		ConfigOption::MeshConfig* meshOption = option->getMeshConfig();

		

		bool ok = geometryOption->isImportGeometryEnabled();
		this->enableGeoImport(ok);
		ok = geometryOption->isExportGeometryEnabled();
		this->enableGeoExport(ok);
		ok = geometryOption->isCreateSketchEnabled();
		this->enableGeoSketch(ok);
		ok = geometryOption->isGeometryModelingEnabled();
		this->enableGeoFeatureModeling(ok);
		ok = geometryOption->isGeometryOperationsEnabled();
		this->enableGeoFeatureOperate(ok);
		ok = geometryOption->isGeometryEnabled();
		_ui->MeasureToolBar->setVisible(ok);

		ok = meshOption->isImportMeshEnabled();
		const QStringList plgsin = IO::IOConfigure::getMeshImporters();
		this->enableMeshImport(ok || (!plgsin.isEmpty()));
		ok = meshOption->isExportMeshEnabled();
		const QStringList plgsex = IO::IOConfigure::getMeshExporters();
		this->enableMeshExport(ok || (!plgsex.isEmpty()));
		ok = meshOption->isSolidMeshEnabled();
		this->enableSolidMesh(ok);
		ok = meshOption->isSurfaceMeshEnabled();
		this->enableSurfaceMesh(ok);
		ok = meshOption->isComponentEnabled();
		this->enableMeshComponent(ok);
		ok = meshOption->getCheckMeshEnabled();
		this->enableMeshCheck(ok);
		_ui->SetToolBar->setVisible(ok || meshOption->isComponentEnabled());


		bool geoEnable = geometryOption->isGeometryEnabled();
		_ui->menuGeometry->setEnabled(geoEnable);
		_ui->actionUndo->setVisible(geoEnable);
		_ui->actionRedo->setVisible(geoEnable);
		_ui->RedoToolBar->setVisible(geoEnable);
		_ui->MeasureToolBar->setVisible(geoEnable);
		this->enableGeoSelectAndView(geoEnable);

		bool meshEnable = meshOption->isMeshEnabled();
		_ui->menuMesher->setEnabled(meshEnable);
		this->enableMeshSelectAndView(meshEnable);
		_ui->SelectToolBar->setVisible(geoEnable || meshEnable);
		_ui->DisplayToolBar->setVisible(geoEnable || meshEnable);
//		qApp->processEvents();


		_mainWindow->getControlPanel()->registerEnabledModule();

		Py::PythonAagent::getInstance()->unLock();
	}


	QString CustomizerHelper::readConfigOptions()
	{
		QString error{};
		bool ok = false;
		QString path = qApp->applicationDirPath();
		ConfigOption::ConfigDataReader reader(path + "/../ConfigFiles/", ConfigOption::ConfigOption::getInstance());
		error = reader.read();

		if (!error.isEmpty()) error.resize(error.size() - 1);
		return error;
	}

	void CustomizerHelper::updateBasicInfo()
	{
		ConfigOption::ConfigOption* options = ConfigOption::ConfigOption::getInstance();
		ConfigOption::GlobalConfig* globalConfig = options->getGlobalConfig();

		const QString lang = Setting::BusAPI::instance()->getLanguage();
		QString softName = "FastCAE";
		if (lang.toLower() == "chinese")
		{
			QString f = globalConfig->getChineseName();
			if (!f.isEmpty()) softName = f;
		}
		else
		{
			QString f = globalConfig->getSoftName();
			if (!f.isEmpty()) softName = f;
		}
		_mainWindow->setWindowTitle(softName);

		const QString logoname = globalConfig->getLogo();
		QString path = qApp->applicationDirPath();
		const QString logo = path + "/../ConfigFiles/icon/" + logoname;
//		qDebug() << logo;
		_mainWindow->setIcon(logo);

	}

	void CustomizerHelper::enableGeoImport(bool on)
	{
		_ui->actionImportGeometry->setVisible(on);
	}

	void CustomizerHelper::enableGeoExport(bool on)
	{
		_ui->actionExportGeometry->setVisible(on);
	}

	void CustomizerHelper::enableGeoSketch(bool on)
	{
		_ui->menuSketch->setEnabled(on);
		_ui->StartSketchToolBar->setVisible(on);
	}

	void CustomizerHelper::enableGeoFeatureModeling(bool on)
	{
		_ui->menuFeature_Modeling->setEnabled(on);
		_ui->GeomertryFeatureToolBar->setVisible(on);
	}

	void CustomizerHelper::enableGeoFeatureOperate(bool on)
	{
		_ui->menuFeature_Operation->setEnabled(on);
		_ui->ChamferToolBar->setVisible(on);
		_ui->BoolToolBar->setVisible(on);
		_ui->FeatureOpertionToolBar->setVisible(on);
	}

	void CustomizerHelper::enableGeoSelectAndView(bool on)
	{
		_ui->menuSelect_2->setEnabled(on);
		_ui->menuView_3->setEnabled(on);

		_ui->actionDisplayPoint->setVisible(on);
		_ui->actionDisplayCurve->setVisible(on);
		_ui->actionDisplayFace->setVisible(on);

		_ui->actionSelectPoint->setVisible(on);
		_ui->actionSelectCurve->setVisible(on);
		_ui->actionSelectFace->setVisible(on);
		_ui->actionSelectGeometryBody->setVisible(on);
	}

	void CustomizerHelper::enableMeshImport(bool on)
	{
		_ui->actionImportMesh->setVisible(on);
	}

	void CustomizerHelper::enableMeshExport(bool on)
	{
		_ui->actionExportMesh->setVisible(on);
	}

	void CustomizerHelper::enableSolidMesh(bool on)
	{
		_ui->actionSolidMesh->setVisible(on);
	}

	void CustomizerHelper::enableSurfaceMesh(bool on)
	{
		_ui->actionSurfaceMesh->setVisible(on);
	}

	void CustomizerHelper::enableMeshCheck(bool on)
	{
		_ui->actionChecking->setVisible(on);
	}

	void CustomizerHelper::enableMeshComponent(bool on)
	{
		_ui->actionCreate_Set->setVisible(on);
	}

	void CustomizerHelper::enableMeshSelectAndView(bool on)
	{
		_ui->menuView_2->setEnabled(on);
		_ui->menuSelect->setEnabled(on);

		_ui->actionDisplayNode->setVisible(on);
		_ui->actionDisplayWireFrame->setVisible(on);
		_ui->actionDisplaySurface->setVisible(on);
		_ui->actionDisplaySurfaceEdge->setVisible(on);

		_ui->actionSelectMeshNode->setVisible(on);
		_ui->actionSelectMeshCell->setVisible(on);
		_ui->actionBoxMeshCell->setVisible(on);
		_ui->actionBoxMeshNode->setVisible(on);
	}




}