#include "signalHandler.h"
#include "ui_mainWindow.h"
#include "mainWindow.h"
#include "solveProcessManager.h"
#include <QTreeWidgetItem>
#include <QFileDialog>
#include <QDir>
#include <QProcess>
#include <QMessageBox>
#include <assert.h>
#include <QFileDialog>
#include "settings/busAPI.h"
#include "meshData/meshKernal.h"
#include "ModelData/modelDataSingleton.h"
#include "ModelData/modelDataBase.h"
#include "geometry/geometryData.h"
#include "geometry/geometryExporter.h"
#include "meshData/meshSingleton.h"
#include "IO/ProjectFileIO.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/ProjectTreeConfig.h"
#include "ConfigOptions/SolverConfig.h"
#include "DataProperty/modelTreeItemType.h"
#include "IO/SolverInfoWriter.h"
#include "IO/IOConfig.h"
#include "SolverControl/SolverControlerBase.h"
#include "PostWidgets/RealTimeWindowBase.h"
#include "PostWidgets/Post2DInterface.h"
#include "PostWidgets/Post3DInterface.h"
#include "SubWindowManager.h"
#include "MainWidgets/preWindow.h"
#include "Material/MaterialSingletion.h"
#include "MainWidgets/DialogSelectMesher.h"
#include "MainWidgets/DialogSavePicture.h"
#include "MainWidgets/DialogMeshChecking.h"
#include "MainWidgets/ControlPanel.h"
#include "python/PyAgent.h"
#include "PluginManager/PluginManager.h"
#include "PluginManager/pluginBase.h"
#include <QMessageBox>
#include "GeometryCommand/GeoCommandList.h"
#include "GeometryCommand/GeoCommandImport.h"
#include "GeometryWidgets/dialogCreateBox.h"
#include "GeometryWidgets/dialogCreateCylinder.h"
#include "GeometryWidgets/dialogCreateSphere.h"
#include "GeometryWidgets/dialogCreateCone.h"
#include "GeometryWidgets/dialogMakeFillet.h"
#include "GeometryWidgets/dialogMakeChamfer.h"
#include "GeometryWidgets/dialogBoolOperation.h"
#include "GeometryWidgets/dialogMirrorFeature.h"
#include "GeometryWidgets/dialogMakeExtrusion.h"
#include "GeometryWidgets/dialogVariableFillet.h"
#include "GeometryWidgets/dialogCreatePoint.h"
#include "GeometryWidgets/dialogCreateLine.h"
#include "GeometryWidgets/dialogCreateFace.h"
#include "GeometryWidgets/dialogMoveFeature.h"
#include "GeometryWidgets/dialogMakeRevol.h"
#include "GeometryWidgets/dialogRotateFeature.h"
#include "GeometryWidgets/dialogMakeLoft.h"
#include "GeometryWidgets/dialogCreateDatumplane.h"
#include "GeometryWidgets/dialogMakeSweep.h"
#include "GeometryWidgets/dialogMakeMatrix.h"
#include "GeometryWidgets/dialogCreateCylindricalComplex.h"
#include "GeometryWidgets/dialogCreateBoxComplex.h"
#include "GeometryWidgets/dialogMeasureDistance.h"
#include "GeometryWidgets/dialogGeoSplitter.h"
#include "GeometryWidgets/dialogMakeFillHole.h"
#include "GeometryWidgets/dialogMakeRemoveSurface.h"
#include "GeometryWidgets/dialogMakeFillGap.h"
#include "Gmsh/GmshModule.h"
#include "UserGuidence/DialogUserGuidence.h"
#include "moduleBase/ThreadControl.h"

namespace GUI
{
	SignalHandler::SignalHandler(MainWindow* mainwindow) : _mainWindow(mainwindow)
	{
		///注册工程树鼠标事件传递给signalHandler
		connect(mainwindow, SIGNAL(treeMouseEvent(int, QTreeWidgetItem*, int)), this, SLOT(handleTreeMouseEvent(int, QTreeWidgetItem*, int)));
		connect(mainwindow, SIGNAL(solveProjectSig(int, int)), this, SLOT(solveProjectPy(int, int)));//liu
		connect(mainwindow->getUi()->actionSurfaceMesh, SIGNAL(triggered()), this, SLOT(generateSurfaceMesh()));
		connect(mainwindow->getUi()->actionSolidMesh, SIGNAL(triggered()), this, SLOT(generateSolidMesh()));
		connect(mainwindow->getUi()->actionFluidMesh, SIGNAL(triggered()), this, SLOT(generateFluidMesh()));
		connect(mainwindow->getUi()->actionGenMesh, SIGNAL(triggered()), this, SLOT(genMesh()));
		connect(mainwindow, SIGNAL(appendGeneratedMesh(QString, vtkDataSet*)), this, SLOT(appendGeneratedMesh(QString, vtkDataSet*)));
		connect(mainwindow->getUi()->actionEnglish, SIGNAL(triggered()), this, SLOT(on_actionEnglish()));
		connect(mainwindow->getUi()->actionChinese, SIGNAL(triggered()), this, SLOT(on_actionChinese()));
		connect(mainwindow, SIGNAL(updateActionStatesSig()), this, SLOT(updateActionsStates()));
		connect(mainwindow->getUi()->action2DPlot, SIGNAL(triggered()), this, SLOT(open2DPlotWindow()));
		connect(mainwindow->getUi()->action3DGraph, SIGNAL(triggered()), this, SLOT(open3DGraphWindow()));
		connect(mainwindow, SIGNAL(closePostWindowSig(Post::PostWindowBase*)), this, SLOT(closePostWindow(Post::PostWindowBase*)));
		connect(mainwindow->getUi()->actionSave_Picture, SIGNAL(triggered()), this, SLOT(saveImange()));
		connect(mainwindow->getUi()->actionChecking, SIGNAL(triggered()), this, SLOT(meshChecking()));
		connect(mainwindow, SIGNAL(showDialogSig(QDialog*)), this, SLOT(showDialog(QDialog*)));
		connect(mainwindow, SIGNAL(clearDataSig()), this, SLOT(clearData()));

		connect(this, SIGNAL(importMeshPySig(QString, QString, int)), this, SLOT(importMeshSlot(QString, QString, int)));
		connect(this, SIGNAL(exportMeshPySig(QString, QString, int)), this, SLOT(exportMeshSlot(QString, QString, int)));

		connect(this, SIGNAL(open3DGraphWindowPySig()), this, SLOT(open3DGraphWindowPy()));
		connect(this, SIGNAL(open2DPlotWindowPySig()), this, SLOT(open2DPlotWindowPy()));
		connect(this, SIGNAL(openProjectFileSig(QString)), this, SLOT(openProjectFile(QString)));
		connect(this, SIGNAL(saveToProjectFileSig(QString)), this, SLOT(saveToProjectFile(QString)));
		connect(this, SIGNAL(solveProjectSig(int, int)), this, SLOT(solveProject(int, int)));
		connect(this, SIGNAL(projectFileProcessedSig(QString, bool, bool)), this, SLOT(projectFileProcessed(QString, bool, bool)));

		connect(mainwindow->getUi()->actionUser_Guidance, SIGNAL(triggered()), this, SLOT(showUserGuidence()));

		//创建几何
		connect(mainwindow->getUi()->actionUndo, SIGNAL(triggered()), this, SLOT(undo()));
		connect(mainwindow->getUi()->actionRedo, SIGNAL(triggered()), this, SLOT(redo()));
		connect(mainwindow->getUi()->actionCreateBox, SIGNAL(triggered()), this, SLOT(createBox()));
		connect(mainwindow->getUi()->actionCreateCylinder, SIGNAL(triggered()), this, SLOT(createCylinder()));
		connect(mainwindow->getUi()->actionCreaterSphere, SIGNAL(triggered()), this, SLOT(CreateSphere()));
		connect(mainwindow->getUi()->actionCreaterCone, SIGNAL(triggered()), this, SLOT(CreateCone()));
		connect(mainwindow->getUi()->actionFillet, SIGNAL(triggered()), this, SLOT(CreateFilet()));
		connect(mainwindow->getUi()->actionVariable_Fillet, SIGNAL(triggered()), this, SLOT(CreateVariableFillet()));
		connect(mainwindow->getUi()->actionChamfer, SIGNAL(triggered()), this, SLOT(CreateChamfer()));
		connect(mainwindow->getUi()->actionBoolCut, SIGNAL(triggered()), this, SLOT(CreateBoolCut()));
		connect(mainwindow->getUi()->actionBoolFause, SIGNAL(triggered()), this, SLOT(CreateBoolFause()));
		connect(mainwindow->getUi()->actionBoolCommon, SIGNAL(triggered()), this, SLOT(CreateBoolCommon()));
		connect(mainwindow->getUi()->actionMirrorFeature, SIGNAL(triggered()), this, SLOT(MirrorFeature()));
		connect(mainwindow->getUi()->actionExtrude, SIGNAL(triggered()), this, SLOT(CreateExtrusion()));
		connect(mainwindow->getUi()->actionCreate_Point, SIGNAL(triggered()), this, SLOT(CreatePoint()));
		connect(mainwindow->getUi()->actionCreate_Line, SIGNAL(triggered()), this, SLOT(CreateEdge()));
		connect(mainwindow->getUi()->actionCreate_Face, SIGNAL(triggered()), this, SLOT(CreateFace()));
		connect(mainwindow->getUi()->actionMoveFeature, SIGNAL(triggered()), this, SLOT(MoveFeature()));
		connect(mainwindow->getUi()->actionRotateFeature, SIGNAL(triggered()), this, SLOT(RotateFeature()));
		connect(mainwindow->getUi()->actionRevol, SIGNAL(triggered()), this, SLOT(CreateRevol()));
		connect(mainwindow->getUi()->actionLoft, SIGNAL(triggered()), this, SLOT(CreateLoft()));
		//connect(mainwindow->getUi()->actionCreateCylindricalComplex, SIGNAL(triggered()), this, SLOT(CreateCylindricalComplex()));
		//connect(mainwindow->getUi()->actionCreateBoxComplex, SIGNAL(triggered()), this, SLOT(CreateBoxComplex()));
		connect(mainwindow->getUi()->actionSweep, SIGNAL(triggered()), this, SLOT(CreateSweep()));
		connect(mainwindow->getUi()->actionCreateDatumPlane, SIGNAL(triggered()), this, SLOT(CreateDatumplane()));
		connect(mainwindow->getUi()->actionDrawLine, SIGNAL(triggered()), this, SLOT(DrawGraphicsLine()));
		connect(mainwindow->getUi()->actionDrawRectangle, SIGNAL(triggered()), this, SLOT(DrawGraphicsRectangle()));
		connect(mainwindow->getUi()->actionDrawCircle, SIGNAL(triggered()), this, SLOT(DrawGraphicsCircle()));
		connect(mainwindow->getUi()->actionDrawArc, SIGNAL(triggered()), this, SLOT(DrawGraphicsArc()));
		connect(mainwindow->getUi()->actionDrawPolyline, SIGNAL(triggered()), this, SLOT(DrawGraphicsPolyline()));
		connect(mainwindow->getUi()->actionMakeMatrix, SIGNAL(triggered()), this, SLOT(MakeMatrix()));
		connect(mainwindow->getUi()->actionDrawSpline, SIGNAL(triggered()), this, SLOT(DrawGraphSpline()));
		connect(mainwindow->getUi()->actionMeasure_Distance, SIGNAL(triggered()), this, SLOT(MeasureDistance()));
		connect(mainwindow->getUi()->actionGeoSplitter, SIGNAL(triggered()), this, SLOT(GeoSplitter()));

		connect(mainwindow->getUi()->actionFillHole, SIGNAL(triggered()), this, SLOT(MakeFillHole()));
		connect(mainwindow->getUi()->actionRemoveSurface, SIGNAL(triggered()), this, SLOT(MakeRemoveSurface()));
		connect(mainwindow->getUi()->actionFillGap, SIGNAL(triggered()), this, SLOT(MakeFillGap()));
		//connect(mainwindow->getUi()->actionDemo, SIGNAL(triggered()), this, SLOT(showDemo()));

		_solveProcessManager = new SolveProcessManager;
	}
	SignalHandler::~SignalHandler()
	{
		clearData();
//		ModuleBase::ProjectTreeInfo::getInstance()->clearData();
		ConfigOption::ConfigOption::getInstance()->clearAllConfig();
//		ConfigOption::ProjectTreeConfig::getInstance()->clearData();
		delete _solveProcessManager;
	}
	void SignalHandler::on_actionNew()
	{
		emit _mainWindow->createPhysiceModelSig();
	}

	SolveProcessManager* SignalHandler::getSolveProcessManager()
	{
		return _solveProcessManager;
	}

	bool SignalHandler::openProjectFile(QString fileName)
	{
		//add a reader
		QFile f(fileName);
		if (!f.exists()) return false;

		this->clearData(false);
		
		IO::ProjectFileIO* reader = new IO::ProjectFileIO(_mainWindow,this ,fileName, true);
		ModuleBase::ThreadControl* tc = new ModuleBase::ThreadControl(reader);
		emit tc->threadStart();
	}

	void SignalHandler::projectFileProcessed(QString fileName, bool success ,bool read)
	{
		ModuleBase::Message m;
		m.type = ModuleBase::Error_Message;
		if (success)
			m.type = ModuleBase::Normal_Message;
		if (read)
		{
			if (success)
			{
				emit _mainWindow->updateGeometryTreeSig();
				emit _mainWindow->updateMeshTreeSig();
				emit _mainWindow->updateSetTreeSig();
				emit _mainWindow->updatePhysicsTreeSignal();

				_mainWindow->setCurrentFile(fileName);
				QString md5 = getMD5();
				_mainWindow->setMD5(md5);
				_mainWindow->getSubWindowManager()->openPreWindow();
				emit _mainWindow->updateActionStatesSig();
				m.message = QString("\"%1\" has been Opened !").arg(fileName);
			}
			else
			{
				clearData(false);
				m.message = QString("\"%1\" open failed !").arg(fileName);
			}
				

			Setting::BusAPI::instance()->appendRecentFile(fileName);
			_mainWindow->updateRecentMenu();
			
		}
		else
		{
			if (success)
			{
				Setting::BusAPI::instance()->appendRecentFile(fileName);
				_mainWindow->setCurrentFile(fileName);
				QString md5 = getMD5();
				_mainWindow->setMD5(md5);
				_mainWindow->updateRecentMenu();
				m.message = QString("\"%1\" has been Saved !").arg(fileName);
			}
			else
			{
				m.message = QString("\"%1\" save failed !").arg(fileName);
			}

		}

		emit _mainWindow->printMessageToMessageWindow(m);
		Py::PythonAagent::getInstance()->unLock();
	}

	void SignalHandler::saveToProjectFile(QString filename)
	{
		//add a writer
		IO::ProjectFileIO* writer = new IO::ProjectFileIO( _mainWindow, this,filename, false);
		ModuleBase::ThreadControl* tc = new ModuleBase::ThreadControl(writer);
		emit tc->threadStart();
	}

	void SignalHandler::on_actionSolve()
	{
		
	}

	void SignalHandler::on_actionEnglish()
	{
		_mainWindow->ChangeLanguage("English");
	}

	void SignalHandler::on_actionChinese()
	{
		_mainWindow->ChangeLanguage("Chinese");
	}

	void SignalHandler::handleTreeMouseEvent(int eventtype, QTreeWidgetItem*item, int proID)
	{
		Q_UNUSED(proID);
		if (item->type() != ProjectPostSetting) return;
		switch (eventtype)
		{
		case 0:
// 			Post3D::Post3DWindow* w = _mainWindow->getPostWindowByID(proID);
// 			if (w == nullptr) return;
// 			w->showWindow();			
			break;
		}
	}

	void SignalHandler::handleSingleClickEvent(QTreeWidgetItem*item, int proID)
	{
		Q_UNUSED(item);
		Q_UNUSED(proID);
	}

	bool SignalHandler::importMeshSlot(const QString& fileName, const QString& suffix, int modelId)
	{
		IMPORTMESHFUN fp = IO::IOConfigure::getMeshImporter(suffix);
		if (!fp)
		{
			ModuleBase::Message m;
			m.type = ModuleBase::Error_Message;
			m.message = QString("\"%1\" interface has not been registered !").arg(suffix);
			emit _mainWindow->printMessageToMessageWindow(m);
			return false;
		}
		fp(fileName, modelId);
		return true;
	}

	bool SignalHandler::exportMeshSlot(const QString& fileName, const QString& suffix, int modelId)
	{
		EXPORTMESHFUN fp = IO::IOConfigure::getMeshExporter(suffix);
		if (!fp)
		{
			ModuleBase::Message m;
			m.type = ModuleBase::Error_Message;
			m.message = QString("\"%1\" interface has not been registered !").arg(suffix);
			emit _mainWindow->printMessageToMessageWindow(m);
			return false;
		}
		fp(fileName, modelId);
//		MeshData::MeshData::getInstance()->generateDisplayDataSet();
	}

	bool SignalHandler::importGeometry(const QStringList &filenames)
	{
		Command::GeoCommandImport* c = new Command::GeoCommandImport(_mainWindow, _mainWindow->getSubWindowManager()->getPreWindow());
		c->setFileName(filenames);
		bool ok = Command::GeoComandList::getInstance()->executeCommand(c);
//		_mainWindow->getSubWindowManager()->setView(-1, "PreWindow", "fit");
		return ok;
	}

	bool SignalHandler::exportGeometry(QString f)
	{
		ModuleBase::Message m;
		Geometry::GeometryExporter exporter(f);
		if (!exporter.write())
		{
			m.type = ModuleBase::Error_Message;
			m.message = QString("Failed export geometry to \"%1\"").arg(f);
			emit _mainWindow->printMessageToMessageWindow(m);
		}
		m.type = ModuleBase::Normal_Message;
		m.message = QString("Export Geometry to \"%1\"").arg(f);
		emit _mainWindow->printMessageToMessageWindow(m);
		return true;
	}

	QString SignalHandler::getMD5()
	{
		QString md5 = "";
		QString p = ModelData::ModelDataSingleton::getinstance()->getMD5();
		QString g = Geometry::GeometryData::getInstance()->getMD5();
		QString m = MeshData::MeshData::getInstance()->getMD5();
		md5 = p + g + m;
		return md5;
	}

	void SignalHandler::clearData(bool unlock)
	{
		_mainWindow->clearWidgets();
		ModelData::ModelDataSingleton::getinstance()->clear();
		Geometry::GeometryData::getInstance()->clear();
		MeshData::MeshData::getInstance()->clear();
		Material::MaterialSingleton::getInstance()->clear();
		Command::GeoComandList::getInstance()->clearAllCommands();
		
		emit _mainWindow->updateMeshTreeSig();
		emit _mainWindow->updateGeometryTreeSig();
		emit _mainWindow->updatePhysicsTreeSignal();
		emit _mainWindow->updateMaterialTreeSig();
		if (unlock)
			Py::PythonAagent::getInstance()->unLock();
	}

	void SignalHandler::solveProjectPy(int projectIndex, int solverIndex)
	{
		ModelData::ModelDataSingleton* modelData = ModelData::ModelDataSingleton::getinstance();
		ModelData::ModelDataBase* model = modelData->getModelAt(projectIndex);
		const int id = model->getID();
		if (_solveProcessManager->isSolving(id))
		{
			QString name = model->getName();
			QMessageBox::warning(_mainWindow, QString(tr("Warning")), QString(tr("%1 is Solving , Please wait...")).arg(name));
			return;
		}

		QString pycode = QString("MainWindow.solveProject(%1,%2)").arg(projectIndex).arg(solverIndex);
		qDebug() << pycode;
		Py::PythonAagent::getInstance()->submit(pycode);
	}

	void SignalHandler::solveProject(int projectIndex, int solverIndex)
	{
		ModelData::ModelDataSingleton* modelData = ModelData::ModelDataSingleton::getinstance();
		ModelData::ModelDataBase* model = modelData->getModelAt(projectIndex);

		ConfigOption::SolverOption*  solvers = ConfigOption::ConfigOption::getInstance()->getSolverOption();
		ConfigOption::SolverInfo* solver = solvers->getSolverAt(solverIndex);
		
		SolverControl::SolverControlBase* solverControl = new SolverControl::SolverControlBase(_mainWindow, solver, model);
		solverControl->setSolverDescription(QString(tr("Solving-%1")).arg(model->getName()));
		int id = model->getID();
		_solveProcessManager->insertProcess(id, solverControl);
		solverControl->startSolver();
		//Py::PythonAagent::getInstance()->unLock();
	}

	void SignalHandler::generateSurfaceMesh()
	{
		MainWidget::PreWindow* prewin = _mainWindow->getSubWindowManager()->getPreWindow();
		Gmsh::GmshModule::getInstance(_mainWindow)->exec(1, prewin);
	}

	void SignalHandler::generateSolidMesh()
	{
		MainWidget::PreWindow* prewin = _mainWindow->getSubWindowManager()->getPreWindow();
		Gmsh::GmshModule::getInstance(_mainWindow)->exec(2, prewin);
	}

	void SignalHandler::generateFluidMesh()
	{
		MainWidget::PreWindow* prewin = _mainWindow->getSubWindowManager()->getPreWindow();
		Gmsh::GmshModule::getInstance(_mainWindow)->exec(0, prewin);
	}

	void SignalHandler::genMesh()
	{
		DataProperty::DataBase* info = nullptr;
		MainWidget::MesherDialog dlg(_mainWindow);
		if (dlg.isSingleMesher() || (dlg.exec() == QDialog::Accepted))
			info = dlg.getMesherInfo();
		else
			return;
		
		emit _mainWindow->updateProperty(info);
	}

	void SignalHandler::appendGeneratedMesh(QString name, vtkDataSet* dataset)
	{
		MeshData::MeshKernal* k = new MeshData::MeshKernal;
		k->setName(name);
		k->setMeshData(dataset);
		MeshData::MeshData::getInstance()->appendMeshKernal(k);
		_mainWindow->getSubWindowManager()->updatePreMeshActor();
		ModuleBase::Message m;
		m.type = ModuleBase::Normal_Message;
		m.message = tr("Mesh Generated \"%1\"").arg(name);
		emit _mainWindow->printMessageToMessageWindow(m);
		emit _mainWindow->updateMeshTreeSig();
	}

	void SignalHandler::updateActionsStates()
	{
		Ui::MainWindow* ui = _mainWindow->getUi();
		SubWindowManager* subwins = _mainWindow->getSubWindowManager();
		MeshData::MeshData* meshData = MeshData::MeshData::getInstance();
		Geometry::GeometryData* geoData = Geometry::GeometryData::getInstance();
		ModelData::ModelDataSingleton* modelData = ModelData::ModelDataSingleton::getinstance();

		//全部置灰
		ui->actionSave->setEnabled(false);
		ui->actionSaveAs->setEnabled(false);
		ui->actionSolve->setEnabled(false);
		ui->actionViewXPlus->setEnabled(false);
		ui->actionViewXMinus->setEnabled(false);
		ui->actionViewYPlus->setEnabled(false);
		ui->actionViewYMinus->setEnabled(false);
		ui->actionViewZPlus->setEnabled(false);
		ui->actionViewZMinus->setEnabled(false);
		ui->actionFitView->setEnabled(false);
		ui->actionSelectOff->setEnabled(false);
		ui->actionSelectMeshNode->setEnabled(false);
		ui->actionSelectMeshCell->setEnabled(false);
		ui->actionBoxMeshNode->setEnabled(false);
		ui->actionBoxMeshCell->setEnabled(false);
		//ui->actionSelectGeoBody->setEnabled(false);
		ui->actionGenMesh->setEnabled(false);
		ui->actionSurfaceMesh->setEnabled(false);
		ui->actionSolidMesh->setEnabled(false);
		ui->actionFluidMesh->setEnabled(false);
		ui->actionExportMesh->setEnabled(false);
		ui->actionSelectOff->setEnabled(false);
		ui->actionDisplayNode->setEnabled(false);
		ui->actionDisplaySurface->setEnabled(false);
		ui->actionDisplaySurfaceEdge->setEnabled(false);
		ui->actionDisplayWireFrame->setEnabled(false);
		ui->actionCreate_Set->setEnabled(false);
		ui->actionChecking->setEnabled(false);
		ui->actionFilterMesh->setEnabled(false);
		ui->actionDisplayPoint->setEnabled(false);
		ui->actionDisplayCurve->setEnabled(false);
		ui->actionDisplayFace->setEnabled(false);
		ui->actionSelectPoint->setEnabled(false);
		ui->actionSelectCurve->setEnabled(false);
		ui->actionSelectFace->setEnabled(false);
		ui->actionSelectGeometryBody->setEnabled(false);
		ui->actionCreateGeoComponent->setEnabled(false);
		ui->actionVTKTranslation->setEnabled(false);

		//打开可用Action
		const int nMesh = meshData->getKernalCount();
		const int ngeo = geoData->getGeometrySetCount();
		const int nModel = modelData->getModelCount();
		bool needSave = Plugins::PluginManager::getInstance()->hasInfoToSave();

		if (nMesh + ngeo + nModel > 0 || needSave)
		{
			ui->actionSave->setEnabled(true);
			ui->actionSaveAs->setEnabled(true);
		}
		if (nModel > 0 || needSave)
		{
			ui->actionSolve->setEnabled(true);
		}
		if (subwins->isPreWindowOpened() || subwins->isPostWindowOpened())
		{
			ui->actionViewXPlus->setEnabled(true);
			ui->actionViewXMinus->setEnabled(true);
			ui->actionViewYPlus->setEnabled(true);
			ui->actionViewYMinus->setEnabled(true);
			ui->actionViewZPlus->setEnabled(true);
			ui->actionViewZMinus->setEnabled(true);
			ui->actionFitView->setEnabled(true);
			ui->actionSelectOff->setEnabled(true);
			ui->actionSelectMeshNode->setEnabled(true);
			ui->actionSelectMeshCell->setEnabled(true);
			//ui->actionSelectGeoBody->setEnabled(true);
			ui->actionBoxMeshNode->setEnabled(true);
			ui->actionBoxMeshCell->setEnabled(true);

		}
		if (ngeo > 0 && subwins->isPreWindowOpened())
		{
			ui->actionSurfaceMesh->setEnabled(true);
			ui->actionSolidMesh->setEnabled(true);
			ui->actionFluidMesh->setEnabled(true);
			ui->actionDisplayPoint->setEnabled(true);
			ui->actionDisplayCurve->setEnabled(true);
			ui->actionDisplayFace->setEnabled(true);

			ui->actionSelectPoint->setEnabled(true);
			ui->actionSelectCurve->setEnabled(true);
			ui->actionSelectFace->setEnabled(true);
			ui->actionSelectGeometryBody->setEnabled(true);
			ui->actionCreateGeoComponent->setEnabled(true);
		}
		if (nMesh >0 && subwins->isPreWindowOpened())
		{
			ui->actionFilterMesh->setEnabled(true);
			ui->actionCreate_Set->setEnabled(true);
			ui->actionChecking->setEnabled(true);
			ui->actionExportMesh->setEnabled(true);
			ui->actionDisplayNode->setEnabled(true);
			ui->actionDisplaySurface->setEnabled(true);
			ui->actionDisplaySurfaceEdge->setEnabled(true);
			ui->actionDisplayWireFrame->setEnabled(true);
			ui->actionVTKTranslation->setEnabled(true);
		}
		if (subwins->isPostWindowOpened())
		{
			ui->actionViewXPlus->setEnabled(true);
			ui->actionViewXMinus->setEnabled(true);
			ui->actionViewYPlus->setEnabled(true);
			ui->actionViewYMinus->setEnabled(true);
			ui->actionViewZPlus->setEnabled(true);
			ui->actionViewZMinus->setEnabled(true);
			ui->actionFitView->setEnabled(true);
		}

		const int n = ConfigOption::ConfigOption::getInstance()->getSolverOption()->getSolverCount();
		if (n > 0)
			ui->actionGenMesh->setEnabled(true);

		bool showGuidence = Setting::BusAPI::instance()->isEnableUserGuidence();

	}

	void SignalHandler::open2DPlotWindow()
	{
		QString pycode = QString("MainWindow.openPost2D()");
		Py::PythonAagent::getInstance()->submit(pycode);
	}

	void SignalHandler::open3DGraphWindow()
	{
		QString pycode = QString("MainWindow.openPost3D()");
		Py::PythonAagent::getInstance()->submit(pycode);
	}

	void SignalHandler::open2DPlotWindowPy()
	{
		//QString pycode = QString("MainWindow.openNew2d()");
		//Py::PythonAagent::getInstance()->submit(pycode);

		QAction* act = _mainWindow->getUi()->action2DPlot;
		if (!act->isEnabled()) return;
		Post::Post2DWindowInterface* p2w = new Post::Post2DWindowInterface(_mainWindow,-1);
		p2w->reTranslate();
		emit _mainWindow->openPostWindowSig(p2w);
		emit _mainWindow->updateActionStatesSig();
		act->setEnabled(false);
	}

	void SignalHandler::open3DGraphWindowPy()
	{
		QAction* act = _mainWindow->getUi()->action3DGraph;
		if (!act->isEnabled()) return;
		Post::Post3DWindowInterface* post3d = new Post::Post3DWindowInterface(_mainWindow,-1);
		post3d->reTranslate();
		emit _mainWindow->openPostWindowSig(post3d);
		emit _mainWindow->updateActionStatesSig();
		act->setEnabled(false);
	} 

	void SignalHandler::closePostWindow(Post::PostWindowBase* p)
	{ 
		int id = p->getID();
		if (id != -1) return;
		if (p->getPostWindowType() == Post::D2)
			_mainWindow->getUi()->action2DPlot->setEnabled(true);
		else if (p->getPostWindowType() == Post::D3)
			_mainWindow->getUi()->action3DGraph->setEnabled(true);
	}

	void SignalHandler::saveImange()
	{
		MainWidget::SavePictureDialog d(_mainWindow,_mainWindow->getSubWindowManager()->getCurrentWindow());
		d.exec();
	}

	void SignalHandler::meshChecking()
	{
		MainWidget::MeshCheckingDialog dlg(_mainWindow);
		dlg.exec();
	}

	void SignalHandler::showUserGuidence(bool start)
	{
		bool show = false;
		QAction* ac = _mainWindow->getUi()->actionUser_Guidance;
		bool inis = Setting::BusAPI::instance()->isEnableUserGuidence();
		if (start)
		{
			show = inis;
			ac->setChecked(inis);
		}
		else
		{
			bool che = ac->isChecked();
			show = che;
			Setting::BusAPI::instance()->isEnableUserGuidence(show);
		}
		if (!show) return;
		if (!_launched) _launched = true;
		else return;

		auto dlg = new Guidence::UserGuidenceDialog(_mainWindow, ac);
		 dlg->show();
	}

	void SignalHandler::undo()
	{
		QString pycode = QString("MainWindow.undo()");
		Py::PythonAagent::getInstance()->submit(pycode);
		
		//Command::GeoComandList::getInstance()->undo();
	}

	void SignalHandler::redo()
	{
		QString pycode = QString("MainWindow.redo()"); 
		Py::PythonAagent::getInstance()->submit(pycode);
		//Command::GeoComandList::getInstance()->redo();
	}

	void SignalHandler::createBox()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		if (!sw->isPreWindowOpened())
			this->openPreWinPy();
		MainWidget::PreWindow* p = sw->getPreWindow();
		
		GeometryWidget::CreateBoxDialog* dlg = new GeometryWidget::CreateBoxDialog(_mainWindow, p);
		this->showDialog(dlg);
	}

	void SignalHandler::createCylinder()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		if (!sw->isPreWindowOpened())
			this->openPreWinPy();
		MainWidget::PreWindow* p = sw->getPreWindow();

		GeometryWidget::CreateCylinderDialog* dlg = new GeometryWidget::CreateCylinderDialog(_mainWindow, p);
		this->showDialog(dlg);
	}

	void SignalHandler::CreateCone()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		if (!sw->isPreWindowOpened())
			this->openPreWinPy();
		MainWidget::PreWindow* p = sw->getPreWindow();

		GeometryWidget::CreateConeDialog* dlg = new GeometryWidget::CreateConeDialog(_mainWindow, p);
		this->showDialog(dlg);
	}

	void SignalHandler::CreateSphere()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		if (!sw->isPreWindowOpened())
			this->openPreWinPy();
		MainWidget::PreWindow* p = sw->getPreWindow();

		GeometryWidget::CreateSphereDialog* dlg = new GeometryWidget::CreateSphereDialog(_mainWindow, p);
		this->showDialog(dlg);
	}

	void SignalHandler::CreateFilet()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		if (!sw->isPreWindowOpened())
		{
			QMessageBox::warning(_mainWindow, tr("Warning"), tr("Open PreWindow First!"));
			return;
		}
		MainWidget::PreWindow* p = sw->getPreWindow();
		GeometryWidget::CreateFiletDialog* dlg = new GeometryWidget::CreateFiletDialog(_mainWindow, p);
		this->showDialog(dlg);
	}

	void SignalHandler::CreateChamfer()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		if (!sw->isPreWindowOpened())
		{
			QMessageBox::warning(_mainWindow, tr("Warning"), tr("Open PreWindow First!"));
			return;
		}
		MainWidget::PreWindow* p = sw->getPreWindow();
		GeometryWidget::CreateChamferDialog* dlg = new GeometryWidget::CreateChamferDialog(_mainWindow, p);
		this->showDialog(dlg);
	}

	void SignalHandler::CreateBoolCut()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		if (!sw->isPreWindowOpened())
		{
			QMessageBox::warning(_mainWindow, tr("Warning"), tr("Open PreWindow First!"));
			return;
		}
		MainWidget::PreWindow* p = sw->getPreWindow();
		GeometryWidget::BoolOpertionDialog* dlg = new GeometryWidget::BoolOpertionDialog(_mainWindow, p);
		dlg->setType(BoolCut);
		this->showDialog(dlg);
	}

	void SignalHandler::CreateBoolFause()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		if (!sw->isPreWindowOpened())
		{
			QMessageBox::warning(_mainWindow, tr("Warning"), tr("Open PreWindow First!"));
			return;
		}
		MainWidget::PreWindow* p = sw->getPreWindow();
		GeometryWidget::BoolOpertionDialog* dlg = new GeometryWidget::BoolOpertionDialog(_mainWindow, p);
		dlg->setType(BoolFause);
		this->showDialog(dlg);
	}

	void SignalHandler::CreateBoolCommon()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		if (!sw->isPreWindowOpened())
		{
			QMessageBox::warning(_mainWindow, tr("Warning"), tr("Open PreWindow First!"));
			return;
		}
		MainWidget::PreWindow* p = sw->getPreWindow();
		GeometryWidget::BoolOpertionDialog* dlg = new GeometryWidget::BoolOpertionDialog(_mainWindow, p);
		dlg->setType(BoolCommon);
		this->showDialog(dlg);
	}

	void SignalHandler::MirrorFeature()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		if (!sw->isPreWindowOpened())
		{
			QMessageBox::warning(_mainWindow, tr("Warning"), tr("Open PreWindow First!"));
			return;
		}
		MainWidget::PreWindow* p = sw->getPreWindow();
		
		GeometryWidget::MirorFeatureDialog* dlg = new GeometryWidget::MirorFeatureDialog(_mainWindow, p);
		this->showDialog(dlg);
	}

	void SignalHandler::CreateExtrusion()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		if (!sw->isPreWindowOpened())
		{
			QMessageBox::warning(_mainWindow, tr("Warning"), tr("Open PreWindow First!"));
			return;
		}
		MainWidget::PreWindow* p = sw->getPreWindow();
		GeometryWidget::CreateExtrusionDialog* dlg = new GeometryWidget::CreateExtrusionDialog(_mainWindow, p);
		this->showDialog(dlg);
	}

	void SignalHandler::CreateVariableFillet()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		if (!sw->isPreWindowOpened())
		{
			QMessageBox::warning(_mainWindow, tr("Warning"), tr("Open PreWindow First!"));
			return;
		}
		MainWidget::PreWindow* p = sw->getPreWindow();
		GeometryWidget::VariableFilletDialog* dlg = new GeometryWidget::VariableFilletDialog(_mainWindow, p);
		this->showDialog(dlg);
	}

	void SignalHandler::CreatePoint()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		if (!sw->isPreWindowOpened())
			this->openPreWinPy();
		MainWidget::PreWindow* p = sw->getPreWindow();

		GeometryWidget::CreatePointDialog* dlg = new GeometryWidget::CreatePointDialog(_mainWindow, p);
		this->showDialog(dlg);
	}

	void SignalHandler::CreateEdge()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		if (!sw->isPreWindowOpened())
			this->openPreWinPy();
		MainWidget::PreWindow* p = sw->getPreWindow();

		GeometryWidget::CreateLineDialog* dlg = new GeometryWidget::CreateLineDialog(_mainWindow, p);
		this->showDialog(dlg);
	}

	void SignalHandler::CreateFace()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		if (!sw->isPreWindowOpened())
			this->openPreWinPy();
		MainWidget::PreWindow* p = sw->getPreWindow();

		GeometryWidget::CreateFaceDialog* dlg = new GeometryWidget::CreateFaceDialog(_mainWindow, p);
		this->showDialog(dlg);
	}

	void SignalHandler::MoveFeature()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		if (!sw->isPreWindowOpened())
		{
			QMessageBox::warning(_mainWindow, tr("Warning"), tr("Open PreWindow First!"));
			return;
		}
		MainWidget::PreWindow* p = sw->getPreWindow();
		
		auto dlg = new GeometryWidget::MoveFeatureDialog(_mainWindow, p);
		this->showDialog(dlg);
	}

	void SignalHandler::RotateFeature()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		if (!sw->isPreWindowOpened())
		{
			QMessageBox::warning(_mainWindow, tr("Warning"), tr("Open PreWindow First!"));
			return;
		}
		MainWidget::PreWindow* p = sw->getPreWindow();
		auto dlg = new GeometryWidget::RotateFeatureDialog(_mainWindow, p);
		this->showDialog(dlg);
	}

	void SignalHandler::CreateLoft()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		if (!sw->isPreWindowOpened())
		{
			QMessageBox::warning(_mainWindow, tr("Warning"), tr("Open PreWindow First!"));
			return;
		}
		MainWidget::PreWindow *p = sw->getPreWindow();
		GeometryWidget::CreateLoftDialog* dlg = new GeometryWidget::CreateLoftDialog(_mainWindow, p);
		this->showDialog(dlg);
	}

	void SignalHandler::CreateSweep()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		if (!sw->isPreWindowOpened())
		{
			QMessageBox::warning(_mainWindow, tr("Warning"), tr("Open PreWindow First!"));
			return;
		}
		MainWidget::PreWindow *p = sw->getPreWindow();
		GeometryWidget::SweepDialog* dlg = new GeometryWidget::SweepDialog(_mainWindow, p);
		this->showDialog(dlg);
	}

	/*void SignalHandler::CreateCylindricalComplex()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		if (!sw->isPreWindowOpened())
			this->openPreWinPy();
		MainWidget::PreWindow* p = sw->getPreWindow();

		GeometryWidget::CreateCylindricalComplexDialog* dlg = new GeometryWidget::CreateCylindricalComplexDialog(_mainWindow, p);
		this->showDialog(dlg);
	}

	void SignalHandler::CreateBoxComplex()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		if (!sw->isPreWindowOpened())
			this->openPreWinPy();
		MainWidget::PreWindow* p = sw->getPreWindow();

		GeometryWidget::CreateBoxComplexDialog* dlg = new GeometryWidget::CreateBoxComplexDialog(_mainWindow, p);
		this->showDialog(dlg);
	}*/

	void SignalHandler::CreateDatumplane()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		if (!sw->isPreWindowOpened())
		{
			QMessageBox::warning(_mainWindow, tr("Warning"), tr("Open PreWindow First!"));
			return;
		}
		MainWidget::PreWindow *p = sw->getPreWindow();
		GeometryWidget::CreateDatumplaneDialog* dlg = new GeometryWidget::CreateDatumplaneDialog(_mainWindow, p);
		this->showDialog(dlg);
	}

	void SignalHandler::DrawGraphicsLine()
	{
	
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		MainWidget::PreWindow *p = sw->getPreWindow();
// 		QAction* action = _mainWindow->getUi()->actionDrawLine;
// 		if (action->isChecked())
		p->setSketchType(ModuleBase::SketchLine);
// 		else 
// 			p->setSketchType(ModuleBase::SketchNone);


	}

	void SignalHandler::DrawGraphicsRectangle()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		MainWidget::PreWindow *p = sw->getPreWindow();
// 		QAction* action = _mainWindow->getUi()->actionDrawRectangle;
// 		if (action->isChecked())
			p->setSketchType(ModuleBase::SketchRect);
// 		else
// 			p->setSketchType(ModuleBase::SketchNone);
	}

	void SignalHandler::DrawGraphicsCircle()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		MainWidget::PreWindow *p = sw->getPreWindow();
// 		QAction* action = _mainWindow->getUi()->actionDrawCircle;
// 		if (action->isChecked())
			p->setSketchType(ModuleBase::SketchCircle);
// 		else
// 			p->setSketchType(ModuleBase::SketchNone);
	}

	void SignalHandler::DrawGraphicsArc()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		MainWidget::PreWindow *p = sw->getPreWindow();
// 		QAction* action = _mainWindow->getUi()->actionDrawArc;
// 		bool l = action->isChecked();
// 		if (action->isChecked())
			p->setSketchType(ModuleBase::SketchArc);
// 		else
// 			p->setSketchType(ModuleBase::SketchNone);
	}

	void SignalHandler::DrawGraphicsPolyline()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		MainWidget::PreWindow *p = sw->getPreWindow();
		p->setSketchType(ModuleBase::SketchPolyline);
	}
	void SignalHandler::DrawGraphSpline()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		MainWidget::PreWindow *p = sw->getPreWindow();
		p->setSketchType(ModuleBase::SketchSpline);
	}


	void SignalHandler::CreateRevol()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		if (!sw->isPreWindowOpened())
		{
			QMessageBox::warning(_mainWindow, tr("Warning"), tr("Open PreWindow First!"));
			return;
		}
		MainWidget::PreWindow* p = sw->getPreWindow();
		GeometryWidget::CreateRevolDialog* dlg = new GeometryWidget::CreateRevolDialog(_mainWindow, p);
		this->showDialog(dlg);
	}

	void SignalHandler::showDialog(QDialog* d)
	{
		int mx = _mainWindow->pos().x();
		int my = _mainWindow->pos().y();
		d->move(mx+20, my + 70);
		d->show();
	}

	void SignalHandler::MakeMatrix()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		if (!sw->isPreWindowOpened())
		{
			QMessageBox::warning(_mainWindow, tr("Warning"), tr("Open PreWindow First!"));
			return;
		}
		MainWidget::PreWindow *p = sw->getPreWindow();
		GeometryWidget::MakeMatrixDialog* dlg = new GeometryWidget::MakeMatrixDialog(_mainWindow, p);
		this->showDialog(dlg);
	}

	void SignalHandler::MeasureDistance()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		if (!sw->isPreWindowOpened())
		{
			QMessageBox::warning(_mainWindow, tr("Warning"), tr("Open PreWindow First!"));
			return;
		}
		MainWidget::PreWindow *p = sw->getPreWindow();
		GeometryWidget::MeasureDistanceDialog* dlg = new GeometryWidget::MeasureDistanceDialog(_mainWindow, p);
		this->showDialog(dlg);
	}

	void SignalHandler::GeoSplitter()
	{

		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		if (!sw->isPreWindowOpened())
		{
			QMessageBox::warning(_mainWindow, tr("Warning"), tr("Open PreWindow First!"));
			return;
		}
		MainWidget::PreWindow *p = sw->getPreWindow();
		GeometryWidget::GeoSplitterDialog* dlg = new GeometryWidget::GeoSplitterDialog(_mainWindow, p);
		this->showDialog(dlg);
	}

	void SignalHandler::MakeFillHole()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		if (!sw->isPreWindowOpened())
		{
			QMessageBox::warning(_mainWindow, tr("Warning"), tr("Open PreWindow First!"));
			return;
		}
		MainWidget::PreWindow *p = sw->getPreWindow();
		GeometryWidget::MakeFillHoleDialog* dlg = new GeometryWidget::MakeFillHoleDialog(_mainWindow, p);
		this->showDialog(dlg);
	}

	void SignalHandler::MakeRemoveSurface()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		if (!sw->isPreWindowOpened())
		{
			QMessageBox::warning(_mainWindow, tr("Warning"), tr("Open PreWindow First!"));
			return;
		}
		MainWidget::PreWindow *p = sw->getPreWindow();
		GeometryWidget::MakeRemoveSurfaceDialog* dlg = new GeometryWidget::MakeRemoveSurfaceDialog(_mainWindow, p);
		this->showDialog(dlg);
	}
	void SignalHandler::MakeFillGap()
	{
		SubWindowManager* sw = _mainWindow->getSubWindowManager();
		if (!sw->isPreWindowOpened())
		{
			QMessageBox::warning(_mainWindow, tr("Warning"), tr("Open PreWindow First!"));
			return;
		}
		MainWidget::PreWindow *p = sw->getPreWindow();
		GeometryWidget::MakeFillGapDialog* dlg = new GeometryWidget::MakeFillGapDialog(_mainWindow, p);
		this->showDialog(dlg);
	}
	void SignalHandler::openPreWinPy()
	{
		QString pycode = QString("MainWindow.openPreWindow()");
		Py::PythonAagent::getInstance()->submit(pycode);
	}	
}
