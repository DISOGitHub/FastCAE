#include "ProjectTreeWithBasicNode.h"
#include "ModelData/modelDataBaseExtend.h"
#include "ConfigOptions/TreeItemData.h"
#include "meshData/meshSet.h"
#include "DialogImport.h"
#include "meshData/meshSingleton.h"
#include "DialogAddBC.h"
#include "BCBase/BCBase.h"
#include "BCBase/BCUserDef.h"
#include "mainWindow/mainWindow.h"
#include "DialogRemoveReport.h"
#include "ModelData/simulationSettingBase.h"
#include "ModelData/solverSettingBase.h"
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include "DialogAssignMaterial.h"
#include "DataProperty/PropertyBase.h"
#include "DataProperty/PropertyString.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/PostConfig.h"
#include "ConfigOptions/PostConfigInfo.h"
#include "ConfigOptions/PostCurve.h"
#include "ConfigOptions/GlobalConfig.h"
#include "PostWidgets/Post2DWidget.h"
#include "PostWidgets/Post3DWidget.h"
#include "PostWidgets/Post2DInterface.h"
#include "PostWidgets/Post3DInterface.h"
#include "PostWidgets/RealTimeWindowBase.h"
#include "Material/MaterialSingletion.h"
#include "Material/Material.h"
#include <QTreeWidgetItem>
#include "python/PyAgent.h"
#include <QAction>
#include <QMenu>
#include <QFileInfo>
#include <QMessageBox>
#include <QSignalMapper>
#include <assert.h>
#include <QDebug>

namespace ProjectTree
{
	ProjectTreeWithBasicNode::ProjectTreeWithBasicNode(GUI::MainWindow* mainwindow)
		:ProjectTreeBase(mainwindow)
	{
		connect(this, SIGNAL(highLightSet(MeshData::MeshSet*)), mainwindow, SIGNAL(highLightSetSig(MeshData::MeshSet*)));
		connect(mainwindow, SIGNAL(closePostWindowSig(Post::PostWindowBase*)), this, SLOT(closePostWindow(Post::PostWindowBase*)));
		connect(this, SIGNAL(openRealTimeWin(Post::RealTimeWindowBase*, int)), mainwindow, SIGNAL(openRealTimeWindowSig(Post::RealTimeWindowBase*, int)));
		_nodeMenu = new QMenu;
		_cellMeun = new QMenu;
	}

	ProjectTreeWithBasicNode::~ProjectTreeWithBasicNode()
	{
		if (_postMapper != nullptr) delete _postMapper;
		if (_nodeMenu != nullptr) delete _nodeMenu;
		if (_cellMeun != nullptr) delete _cellMeun;
	}

	

	void ProjectTreeWithBasicNode::initBasicNode(QTreeWidgetItem* phyroot)
	{
		_root = new QTreeWidgetItem(phyroot, TreeItemType::ProjectRoot);
		_root->setFlags(_root->flags() | Qt::ItemIsEditable);
		_root->setIcon(0, QIcon("://QUI/icon/physics.png"));
		int id = _data->getID();
		_root->setText(0, _data->getName());  //根据项目名称确定
		_root->setData(0, Qt::UserRole, _data->getID());
		_root->setExpanded(true);
		double time = _data->getSolveTime();
		if (time > 0) _solveStatus = Finished;
		_data->setOutputFileName(_outputFile);

// 		_geometryRootItem = new QTreeWidgetItem(_root, TreeItemType::ProjectGeometry);
// 		_geometryRootItem->setText(0, tr("Geometry"));
// 		_geometryRootItem->setIcon(0, QIcon("://QUI/icon/geometry.png"));
// 		_textItemHash["Geometry"] = _geometryRootItem;

		_meshRootItem = new QTreeWidgetItem(_root, TreeItemType::ProjectMesh);
		_meshRootItem->setText(0, tr("Mesh Set"));
		_meshRootItem->setIcon(0, QIcon("://QUI/icon/mesh.png"));
		_textItemHash["Mesh Set"] = _meshRootItem;

		_simulationSettingItem = new QTreeWidgetItem(_root, TreeItemType::ProjectSimulationSetting);
		_simulationSettingItem->setText(0, tr("Simulation Setting"));
		_simulationSettingItem->setIcon(0, QIcon("://QUI/icon/solumationsetting.png"));
		_textItemHash["Simulation Setting"] = _simulationSettingItem;

		_boundaryConditionItem = new QTreeWidgetItem(_root, TreeItemType::ProjectBoundaryCondation);
		_boundaryConditionItem->setText(0, tr("Boundary Condition"));
		_boundaryConditionItem->setIcon(0, QIcon("://QUI/icon/bc.png"));
		_textItemHash["Boundary Condition"] = _boundaryConditionItem;

		_solverSettingItem = new QTreeWidgetItem(_root, TreeItemType::ProjectSolver);
		_solverSettingItem->setText(0, tr("Solver Setting"));
		_solverSettingItem->setIcon(0, QIcon("://QUI/icon/setting.png"));
		_textItemHash["Solver Setting"] = _solverSettingItem;

		_monitorRootItem = new QTreeWidgetItem(_root, TreeItemType::ProjectMonitor);
		_monitorRootItem->setText(0, tr("Monitor"));
		_monitorRootItem->setIcon(0, QIcon("://QUI/icon/monitor.png"));
		_textItemHash["Monitor"] = _monitorRootItem;

		_postRootItem = new QTreeWidgetItem(_root, TreeItemType::ProjectPost);
		_postRootItem->setText(0, tr("Post"));
		_postRootItem->setIcon(0, QIcon("://QUI/icon/post.png"));
		_textItemHash["Post"] = _postRootItem;

		_post2DItem = new QTreeWidgetItem(_postRootItem, TreeItemType::ProJectPost2DGraph);
		_post2DItem->setText(0, tr("2D Plot"));
		_post2DItem->setIcon(0, QIcon("://QUI/icon/curve.png"));
		_textItemHash["2D Plot"] = _post2DItem;

		_postCounterItem = new QTreeWidgetItem(_postRootItem, TreeItemType::ProjectPostCounter);
		_postCounterItem->setText(0, tr("Counter"));
		_postCounterItem->setIcon(0, QIcon("://QUI/icon/counter.png"));
		_textItemHash["Counter"] = _postCounterItem;

		_postVectorItem = new QTreeWidgetItem(_postRootItem, TreeItemType::ProjectPostVector);
		_postVectorItem->setText(0, tr("Vector"));
		_postVectorItem->setIcon(0, QIcon("://QUI/icon/vector.png"));
		_textItemHash["Vector"] = _postVectorItem;

		_postStreamLineItem = new QTreeWidgetItem(_postRootItem, TreeItemType::ProjectPostStreamLine);
		_postStreamLineItem->setText(0, tr("Stream Line"));
		_postStreamLineItem->setIcon(0, QIcon("://QUI/icon/streamline.png"));
		_textItemHash["Stream Line"] = _postStreamLineItem;
		_postStreamLineItem->setHidden(true);

		_reportItem = new QTreeWidgetItem(_root, TreeItemType::ProjectReport);
		_reportItem->setText(0, tr("Report"));
		_reportItem->setHidden(true);
		_textItemHash["Report"] = _reportItem;
	}
	void ProjectTreeWithBasicNode::reTranslate()
	{
		if (_geometryRootItem != nullptr)
			_geometryRootItem->setText(0, tr("Geometry"));
		_meshRootItem->setText(0, tr("Mesh Set"));
		_simulationSettingItem->setText(0, tr("Simulation Setting"));
		_boundaryConditionItem->setText(0, tr("Boundary Condition"));
		_solverSettingItem->setText(0, tr("Solver Setting"));
		_monitorRootItem->setText(0, tr("Monitor"));
		_postRootItem->setText(0, tr("Post"));
		_post2DItem->setText(0, tr("2D Plot"));
		_postCounterItem->setText(0, tr("Counter"));
		_postVectorItem->setText(0, tr("Vector"));
		_postStreamLineItem->setText(0, tr("Stream Line"));
// 		_post3DItem->setText(0, tr("3D Graph"));
 		_reportItem->setText(0, tr("Report"));

		ProjectTreeBase::reTranslate();
	}
	void ProjectTreeWithBasicNode::setData(ModelData::ModelDataBase* data)
	{
		ProjectTreeBase::setData(data);
		_modelDataExtend = dynamic_cast<ModelData::ModelDataBaseExtend*>(_data);

	}
	void ProjectTreeWithBasicNode::contextMenu(QMenu* menu)
	{
		QAction* action = nullptr;
		TreeItemType type = (TreeItemType)_currentItem->type();
		auto glo = ConfigOption::ConfigOption::getInstance()->getGlobalConfig();
		switch (type)
		{
		case ProjectGeometry:
			action = menu->addAction(tr("Import Geometry"));
			connect(action, SIGNAL(triggered()), this, SLOT(importGeometry()));
			break;
		case ProjectMesh:
			action = menu->addAction(tr("Import"));
			connect(action, SIGNAL(triggered()), this, SLOT(importMesh()));
		
			if (glo->isMaterialEnabled())
			{
				action = menu->addAction(tr("Assign Material"));
				connect(action, SIGNAL(triggered()), this, SLOT(assignMaterial()));
			}
			break;
		case ProjectMeshChild:
			action = menu->addAction(tr("Remove"));
			connect(action, SIGNAL(triggered()), this, SLOT(removeItem()));
			break;
		case ProjectBoundaryCondation:
			action = menu->addAction(tr("Add Boundary Condition"));
			connect(action, SIGNAL(triggered()), this, SLOT(addBC()));
			break;
		case ProjectBoundaryCondationChild:
			action = menu->addAction(tr("Remove"));
			connect(action, SIGNAL(triggered()), this, SLOT(removeItem()));
			break;
		case  ProjectMonitor:
			action = menu->addAction(tr("View"));
			connect(action, SIGNAL(triggered()), this, SLOT(viewRealTimeWindow()));
			break;
		case ProJectPost2DGraph:
			this->d2PlotContextMenu(menu);
			break;
		case ProJectPost2DGraphChild:
			action = menu->addAction(tr("Remove"));
			connect(action, SIGNAL(triggered()), this, SLOT(removeItem()));
			break;
		case  ProjectPostCounter:
			this->counterContextMenu(menu);
			break;
		case ProjectPostCounterChild:
			action = menu->addAction(tr("Remove"));
			connect(action, SIGNAL(triggered()), this, SLOT(removeItem()));
			break;
		case  ProjectPostVector:
			this->vectorContextMenu(menu);
			break;
		case  ProjectPostVectorChild:
			action = menu->addAction(tr("Remove"));
			connect(action, SIGNAL(triggered()), this, SLOT(removeItem()));
			break;
		case ProjectPostStreamLine:
			break;
		case ProjectReport:
			action = menu->addAction(tr("Create Report"));
			connect(action, SIGNAL(triggered()), this, SLOT(createReport()));
			break;
		case ProjectReportChild:
			action = menu->addAction(tr("Remove"));
			connect(action, SIGNAL(triggered()), this, SLOT(removeItem()));
			break;
		default:
			break;
		}
		ProjectTreeBase::contextMenu(menu);
	}
	void ProjectTreeWithBasicNode::importMesh()
	{
		ImportDialog dlg(_mainWindow, _modelDataExtend,MeshSet);
		dlg.exec();
// 		if (dlg.exec() == QDialog::Accepted)
// 			updateMeshSubTree();
	}
	void ProjectTreeWithBasicNode::importGeometry()
	{
		ImportDialog dlg(_mainWindow, _modelDataExtend, Geometry);
		dlg.exec();
	}
	void ProjectTreeWithBasicNode::updateTree()
	{
		updateGeometrySubTree();
		updateMeshSubTree();
		updateBCSubTree();
		updateMonitorTree();
		updatePostTree();
		updateReportTree();
	}
	void ProjectTreeWithBasicNode::updateMeshSubTree()
	{ 
		if (_meshRootItem == nullptr) return;
		_meshRootItem->takeChildren();
		MeshData::MeshData* meshdata = MeshData::MeshData::getInstance();
		Material::MaterialSingleton* materialData = Material::MaterialSingleton::getInstance();
		
		QList<int> compIDs = _modelDataExtend->getMeshSetList();
		for (int i = 0; i < compIDs.size(); ++i)
		{
			int id = compIDs.at(i);
			MeshData::MeshSet* set = meshdata->getMeshSetByID(id);
//			assert(set != nullptr);
			if (set == nullptr) continue;
			QString name = set->getName();
			MeshData::SetType type = set->getSetType();
			QString icon = "://QUI/icon/nodeset.png";
			if (type == MeshData::Element )
			{
				icon = "://QUI/icon/eleset.png";
				if (_modelDataExtend->isMaterialSetted(id))
				{
					int materialid = _modelDataExtend->getMaterialID(id);
					Material::Material* m = materialData->getMaterialByID(materialid);
					if (m != nullptr)
					{
						QString mname = m->getName();
						name.append(QString("-[%1]").arg(mname));
					}
				}
			}
			else if (type == MeshData::Family)
			{
				icon = "://QUI/icon/family.png";
			}
				
			QTreeWidgetItem* temp = new QTreeWidgetItem(_meshRootItem, TreeItemType::ProjectMeshChild);
			temp->setIcon(0, QIcon(icon));
			temp->setText(0, name);
			temp->setData(0, Qt::UserRole, id);
		}
		_meshRootItem->setExpanded(true);
		emit highLightSet(nullptr);
	}
	void ProjectTreeWithBasicNode::updateBCSubTree()
	{
		if (_boundaryConditionItem == nullptr) return;
		_boundaryConditionItem->takeChildren();
		const int nBC = _modelDataExtend->getBCCount();
		for (int i = 0; i < nBC; ++i)
		{
			QString icon;
			BCBase::BCBase* bc = _modelDataExtend->getBCAt(i);
			if (bc == nullptr) continue;
			BCBase::BCType ty = bc->getType();
			QString stype;
			if (ty == BCBase::UserDef)
			{
				stype = ((BCBase::BCUserDef*)bc)->getName();
				QString sicon = ((BCBase::BCUserDef*)bc)->getIcon();
				icon = qApp->applicationDirPath() + "/../ConfigFiles/Icon/" + sicon;
//				qDebug() << icon;
			}
			else
			{
				stype = BCBase::BCTypeToString(ty);
				icon = BCBase::BCIconByType(ty);
			}
			
			QString setname = bc->getMeshSetName();

			QTreeWidgetItem* item = new QTreeWidgetItem(_boundaryConditionItem, TreeItemType::ProjectBoundaryCondationChild);
			QString text = QString("%1 @ %2").arg(stype).arg(setname);
			item->setText(0, text);
			item->setIcon(0, QIcon(icon));
		}
		_boundaryConditionItem->setExpanded(true);
		emit highLightSet(nullptr);
	}
	void ProjectTreeWithBasicNode::updateMonitorTree()
	{
		_monitorRootItem->takeChildren();
		QList<ConfigOption::PostCurve*> curvelist = _modelDataExtend->getMonitorCurves();
//		qDebug() << curvelist.size();
		for (int i = 0; i < curvelist.size(); ++i)
		{
			ConfigOption::PostCurve* c = curvelist.at(i);
			QString text = c->getDescribe();
			QTreeWidgetItem* tempitem = new QTreeWidgetItem(_monitorRootItem, ProjectMonitorChild);
			tempitem->setText(0, text);
			tempitem->setIcon(0, QIcon("://QUI/icon/curve.png"));
		}
	}
	void ProjectTreeWithBasicNode::updatePostTree()
	{
		_post2DItem->takeChildren();
		_postCounterItem->takeChildren();
		_postVectorItem->takeChildren();
//		QStringList d2v = _modelDataExtend->getPlot2DVariable();
		QList<ConfigOption::PostCurve*> d2v = _modelDataExtend->getPlotCurves();
		for (int i = 0; i < d2v.size(); ++i)
		{
			QTreeWidgetItem* item = new QTreeWidgetItem(_post2DItem, ProJectPost2DGraphChild);
			QString text = d2v.at(i)->getDescribe();
			item->setText(0, text);
			item->setIcon(0, QIcon("://QUI/icon/curve.png"));
		}
		QStringList scalarv = _modelDataExtend->getScalarVariable();
		for (int i = 0; i < scalarv.size(); ++i)
		{
			QTreeWidgetItem* item = new QTreeWidgetItem(_postCounterItem, ProjectPostCounterChild);
			QString text = scalarv.at(i);
			if (text.endsWith("_cell000"))
			{
				text.replace("_cell000", " @ Cell");
			}
			else text.append(" @ Node");
			item->setIcon(0, QIcon("://QUI/icon/counter.png"));
			item->setText(0, text);
		}
		QStringList vectorv = _modelDataExtend->getVectorVariable();
		for (int i = 0; i < vectorv.size(); ++i)
		{
			QTreeWidgetItem* item = new QTreeWidgetItem(_postVectorItem, ProjectPostVectorChild);
			QString text = vectorv.at(i);
			if (text.endsWith("_cell000"))
			{
				text.replace("_cell000", " @ Cell");
			}
			else text.append(" @ Node");
			item->setIcon(0, QIcon("://QUI/icon/vector.png"));
			item->setText(0, text);
		}
		_post2DItem->setExpanded(true);
		_postCounterItem->setExpanded(true);
		_postVectorItem->setExpanded(true);
	}

	void ProjectTreeWithBasicNode::removeItem()
	{
		TreeItemType type = (TreeItemType)_currentItem->type();
		if (_currentItem->type() == ProjectMeshChild)
		{
			int index = _meshRootItem->indexOfChild(_currentItem);
			if (_data->isComponentUsed(index))
			{
				if (QMessageBox::Yes != QMessageBox::warning(_mainWindow, tr("Warning"), tr("This Set has been used, still remove?"), QMessageBox::Yes, QMessageBox::No))
					return;
			}
			_modelDataExtend->removeMeshSetAt(index);
			updateMeshSubTree();
			updateBCSubTree();
		}
		else if (_currentItem->type() == ProjectBoundaryCondationChild)
		{
			int index = _boundaryConditionItem->indexOfChild(_currentItem);
			_modelDataExtend->removeBCAt(index);
			updateBCSubTree();
		}
		else if (_currentItem->type() == ProjectReportChild)
		{
			int index = _reportItem->indexOfChild(_currentItem);
			RemoveReportDialog dlg(_mainWindow);
			if (dlg.exec() == QDialog::Accepted)
			{
				QString filename = _modelDataExtend->getReportAt(index);
				QFile f(filename);
				if (f.exists() && dlg.isRemoveCompletely())
					f.remove();
				_modelDataExtend->removeReportAt(index);
				updateReportTree();
			}
		}
		else if (_currentItem->type() == ProJectPost2DGraphChild)
		{
			const int index = _post2DItem->indexOfChild(_currentItem);
			QString name = _currentItem->text(0);
			qDebug() << name;
			_modelDataExtend->removePlotCurve(index);
			updatePostTree();
			auto p2d = _modelDataExtend->getPost2DWindow();
			if (p2d != nullptr)
			{
				p2d->removeCurve(name);
			}
		}

		else if (_currentItem->type() == ProjectPostCounterChild)
		{
			const int index = _postCounterItem->indexOfChild(_currentItem);
			_modelDataExtend->removeScalarVariable(index);
			updatePostTree();
		}
		else if (_currentItem->type() == ProjectPostVectorChild)
		{
			const int index = _postVectorItem->indexOfChild(_currentItem);
			_modelDataExtend->removeVectorVariable(index);
			updatePostTree();
		}
		emit disPlayProp(nullptr);
	}
	void ProjectTreeWithBasicNode::createReport()
	{
//		_modelDataExtend->appendReport("D:/test.docx");

		updateReportTree();
	}
	void ProjectTreeWithBasicNode::addBC()
	{
 		AddBCDialog dlg(_mainWindow, _modelDataExtend);
		dlg.exec();
// 		if (dlg.exec() == QDialog::Accepted)
// 			updateBCSubTree();
	}
	void ProjectTreeWithBasicNode::singleClicked()
	{
		TreeItemType type = (TreeItemType)_currentItem->type();
		if (type == ProjectMeshChild)
		{
			const int index = _meshRootItem->indexOfChild(_currentItem);
			if (index >= 0)
			{
				const int id = _modelDataExtend->getMeshSetList().at(index);
				MeshData::MeshSet* set = MeshData::MeshData::getInstance()->getMeshSetByID(id);
				emit highLightSet(set);
			}
		}
		else if (type == ProjectBoundaryCondationChild)
		{
			const int index = _boundaryConditionItem->indexOfChild(_currentItem);
			BCBase::BCBase* bc = _modelDataExtend->getBCAt(index);
			assert(bc != nullptr);
			MeshData::MeshSet* set = bc->getMeshSet();
			emit highLightSet(set);
			emit disPlayProp(bc);
		}
		else if (type == ProjectSimulationSetting)
		{
			ModelData::SimlutationSettingBase* s = _data->getSimlutationSetting();
			emit disPlayProp(s);
		}
		else if (type == ProjectSolver)
		{
			ModelData::SolverSettingBase* s = _data->getSolverSetting();
			emit disPlayProp(s);
		}
		else if (type == ProjectMonitorChild)
		{
			const int index = _monitorRootItem->indexOfChild(_currentItem);
//			QList<DataProperty::DataBase*> mon = _modelDataExtend->getMonitorList();
			emit disPlayProp(_modelDataExtend->getMonitorCurveAt(index));
			if (_realTimeWin != nullptr)
			{
				QString name = _currentItem->text(0);
				_realTimeWin->viewRealTimeWindow(name);
			}
			
			
		}
		else if (type == ProJectPost2DGraphChild)
		{
			const int index = _post2DItem->indexOfChild(_currentItem);
			ConfigOption::PostCurve * temp = _modelDataExtend->getPlotCurves().at(index);
			emit disPlayProp(temp);
			QString text = _currentItem->text(0);
			viewPlot2D(text);
		}
		else if (type == ProjectPostCounterChild)
		{
			QString text = _currentItem->text(0);
			text.remove(" @ Node");
			text.replace(" @ Cell", "_cell000");
			viewCounter(text);
		}
		else if (type == ProjectPostVectorChild)
		{
			QString text = _currentItem->text(0);
			text.remove(" @ Node");
			text.replace(" @ Cell", "_cell000");
			viewVector(text);
		}
		else
		{
			emit disPlayProp(nullptr);
		}
		const int index = _currentItem->data(0, Qt::UserRole + 1).toInt() - 1;
		if (index >= 0)
		{
			ConfigOption::TreeItem* item = _itemList.at(index);
			const int id = item->getDataID();
			DataProperty::DataBase* d = _modelDataExtend->getConfigData(id);
			emit disPlayProp(d);
		}
	
		ProjectTreeBase::singleClicked();
	}
	void ProjectTreeWithBasicNode::doubleClicked()
	{
		qDebug() << "ss";
	}

	void ProjectTreeWithBasicNode::updateReportTree()
	{
		if (_reportItem == nullptr) return;
		_reportItem->takeChildren();
		const int nReport = _modelDataExtend->getReportCount();
		for (int i = 0; i < nReport; ++i)
		{
			QString report = _modelDataExtend->getReportAt(i);
			QFileInfo file(report);
			if (!file.exists()) continue;
			QString text = file.fileName();
			QTreeWidgetItem* item = new QTreeWidgetItem(_reportItem, TreeItemType::ProjectReportChild);
			item->setText(0, text);
		}
		_reportItem->setExpanded(true);
	}
	void ProjectTreeWithBasicNode::assignMaterial()
	{
		AssignMaterialDialog dlg(_mainWindow, _modelDataExtend);
		if (dlg.exec() == QDialog::Accepted)
		{
			this->updateMeshSubTree();
		}
	}
	void ProjectTreeWithBasicNode::d2PlotContextMenu(QMenu* menu)
	{
		ConfigOption::PostConfig* option = ConfigOption::ConfigOption::getInstance()->getPostConfig();
		ConfigOption::PostConfigInfo* info = option->getPostConfigInfo(_treeType);
		if (info == nullptr) return;
		if (_postMapper != nullptr)
		{
			delete _postMapper;
			_postMapper = nullptr;
		}

		_postMapper = new QSignalMapper(this);
//		QStringList d2Variables = info->get2DVariable();
		QList<ConfigOption::PostCurve*> curlist = info->getCurveList();
		for (int i = 0; i < curlist.size(); ++i)
		{
			QString text = curlist.at(i)->getDescribe();
			if (text.isEmpty()) continue;
			QAction* action = menu->addAction(text);
			connect(action, SIGNAL(triggered()), _postMapper, SLOT(map()));
			_postMapper->setMapping(action, text);
		}
		connect(_postMapper, SIGNAL(mapped(QString)), this, SLOT(viewPlot2D(QString)));
	}
	void ProjectTreeWithBasicNode::counterContextMenu(QMenu* menu)
	{
		ConfigOption::PostConfig* option = ConfigOption::ConfigOption::getInstance()->getPostConfig();
		ConfigOption::PostConfigInfo* info = option->getPostConfigInfo(_treeType);
		if (info == nullptr) return;
		if (_postMapper != nullptr)
		{
			delete _postMapper;
			_postMapper = nullptr;
		}
		_postMapper = new QSignalMapper(this);

		QAction* nodeaction = menu->addAction(tr("Node Scalar"));
		QAction* cellaction = menu->addAction(tr("Cell Scalar"));
		_nodeMenu->clear();
		_cellMeun->clear();
		nodeaction->setMenu(_nodeMenu);
		cellaction->setMenu(_cellMeun);
		
		QStringList scalarVariables = info->getNodeScalarVariable();
// 		if (scalarVariables.at(0) != "")
// 		{

		for (int i = 0; i < scalarVariables.size(); ++i)
		{
			QString text = scalarVariables.at(i);
			if (text.isEmpty()) continue;
			qDebug() << text;
			QAction* action = _nodeMenu->addAction(text);
			connect(action, SIGNAL(triggered()), _postMapper, SLOT(map()));
			_postMapper->setMapping(action, text);
		}
//		}
		scalarVariables = info->getCellScalarVariable();
//		if (scalarVariables.at(0) != "")
//		{

		for (int i = 0; i < scalarVariables.size(); ++i)
		{
			QString text = scalarVariables.at(i);
			if (text.isEmpty()) continue;
			QAction* action = _cellMeun->addAction(text);
			connect(action, SIGNAL(triggered()), _postMapper, SLOT(map()));
			_postMapper->setMapping(action, text.append("_cell000"));
		}
//		}
		connect(_postMapper, SIGNAL(mapped(QString)), this, SLOT(viewCounter(QString)));
	}
	void ProjectTreeWithBasicNode::vectorContextMenu(QMenu* menu)
	{
		ConfigOption::PostConfig* option = ConfigOption::ConfigOption::getInstance()->getPostConfig();
		ConfigOption::PostConfigInfo* info = option->getPostConfigInfo(_treeType);
		if (info == nullptr) return;
		if (_postMapper != nullptr)
		{
			delete _postMapper;
			_postMapper = nullptr;
		}
		_postMapper = new QSignalMapper(this);

		QAction* nodeaction = menu->addAction(tr("Node Vector"));
		QAction* cellaction = menu->addAction(tr("Cell Vector"));
		_nodeMenu->clear();
		_cellMeun->clear();
		nodeaction->setMenu(_nodeMenu);
		cellaction->setMenu(_cellMeun);

		QStringList vectorVariables = info->getNodeVectorVariable();
// 		if (vectorVariables.at(0) != "")
// 		{

		for (int i = 0; i < vectorVariables.size(); ++i)
		{
			QString text = vectorVariables.at(i);
			if (text.isEmpty()) continue;
			QAction* action = _nodeMenu->addAction(text);
			connect(action, SIGNAL(triggered()), _postMapper, SLOT(map()));
			_postMapper->setMapping(action, text);
		}
//		}
		vectorVariables = info->getCellVectorVariable();
// 		if (vectorVariables.at(0) != "")
// 		{

		for (int i = 0; i < vectorVariables.size(); ++i)
		{
			QString text = vectorVariables.at(i);
			if (text.isEmpty()) continue;
			QAction* action = _cellMeun->addAction(text);
			connect(action, SIGNAL(triggered()), _postMapper, SLOT(map()));
			_postMapper->setMapping(action, text.append("_cell000"));
		}
//		}
		connect(_postMapper, SIGNAL(mapped(QString)), this, SLOT(viewVector(QString)));

	}
	void ProjectTreeWithBasicNode::closePostWindow(Post::PostWindowBase* w)
	{
		auto p2d = _modelDataExtend->getPost2DWindow();
		auto p3d = _modelDataExtend->getPost3DWindow();
		if (p2d == w)
		{
//			_post2DWindow = nullptr;
			_modelDataExtend->setPost2DWindow(nullptr);
			_modelDataExtend->clearPlotCurve();
			this->updatePostTree();
		}
		if ( p3d == w)
		{
//			_post3DWindow = nullptr;
			_modelDataExtend->setPost3DWindow(nullptr);
			_modelDataExtend->clear3DVariable();
			this->updatePostTree();
		}
	}
	void ProjectTreeWithBasicNode::viewPlot2D(QString variable)
	{
		auto p2d = _modelDataExtend->getPost2DWindow();

		if (p2d == nullptr)
		{
// 			ConfigOption::PostConfig* option = ConfigOption::ConfigOption::getInstance()->getPostConfig();
// 			ConfigOption::PostConfigInfo* info = option->getPostConfigInfo(_treeType);
			p2d = new Post::Post2DWindowInterface(_mainWindow, _data->getID());
			_modelDataExtend->setPost2DWindow(p2d);
//			QStringList filenames = info->getPost2DFile();
			QStringList  filenames = _modelDataExtend->getAbsolutePost2DFiles();

			for (int i = 0; i < filenames.size(); ++i)
			{
				QString filename = filenames.at(i);
			//	QString file = _data->getPath() + "/Result/" + filename;
			//	qDebug() << filename;
				p2d->openfile(filename);
			}
			
		}
		emit openPostWindowSig(p2d);

		if (_modelDataExtend->isPostCurveExist(variable)) return;

		ConfigOption::PostConfig* postinfo = ConfigOption::ConfigOption::getInstance()->getPostConfig();
		ConfigOption::PostConfigInfo* info = postinfo->getPostConfigInfo(_treeType);
		ConfigOption::PostCurve* curve = info->getCurveByName(variable);
		ConfigOption::PostCurve* newcurve = new ConfigOption::PostCurve;
		newcurve->copy(curve);
		p2d->addCurve(curve->getDescribe(), curve->getFile(), curve->getXVariable(), curve->getYVariable());
		_modelDataExtend->apppendPlotCurve(newcurve);
		this->updatePostTree();

		emit disPlayProp(newcurve);
	}

	void ProjectTreeWithBasicNode::viewCounter(QString variable)
	{
		int caseid = _modelDataExtend->getID();
		QString pycode = QString("Post.viewCounter(%1,\"%2\")").arg(caseid).arg(variable);
		qDebug() << pycode;
		Py::PythonAagent::getInstance()->submit(pycode);
	}
	void ProjectTreeWithBasicNode::viewCounterPost(QString variable)
	{
		auto p3d = _modelDataExtend->getPost3DWindow();

		if (p3d == nullptr)
		{
			ConfigOption::PostConfig* option = ConfigOption::ConfigOption::getInstance()->getPostConfig();
			ConfigOption::PostConfigInfo* info = option->getPostConfigInfo(_treeType);
			QString filename = info->getPost3DFile();
			QString file = _data->getPath() + "/Result/" + filename;
//			if (!isFileExist(file)) return;
			p3d = new Post::Post3DWindowInterface(_mainWindow, _data->getID());
			_modelDataExtend->setPost3DWindow(p3d);
			p3d->getWindow()->openFile(file);
			
		}
		emit openPostWindowSig(p3d);
		QStringList variablelist = _modelDataExtend->getScalarVariable();
		if (!variablelist.contains(variable))
		{
		_modelDataExtend->appendScalarVariable(variable);
		this->updatePostTree();
		}
		Post::Post3DWindow* w = p3d->getWindow();
		PipelineObject* popobj = w->getCurrentPipelineObj();
		w->viewCounter(popobj, variable);
		w->displayLegendBar(true);

		emit disPlayProp(nullptr);
		//Py::PythonAagent::getInstance()->unLock();
	}
	void ProjectTreeWithBasicNode::viewVector(QString variable)
	{
		int caseid = _modelDataExtend->getID();

		QString pycode = QString("Post.viewVector(%1,\"%2\")").arg(caseid).arg(variable);
		qDebug() << pycode;
		Py::PythonAagent::getInstance()->submit(pycode);
	}

	void ProjectTreeWithBasicNode::viewVectorPost(QString variable)
	{
		auto p3d = _modelDataExtend->getPost3DWindow();
		if (p3d == nullptr)
		{
			ConfigOption::PostConfig* option = ConfigOption::ConfigOption::getInstance()->getPostConfig();
			ConfigOption::PostConfigInfo* info = option->getPostConfigInfo(_treeType);
			QString filename = info->getPost3DFile();
			QString file = _data->getPath() + "/Result/" + filename;
	//		if (!isFileExist(file)) return;
			p3d = new Post::Post3DWindowInterface(_mainWindow, _data->getID());
			_modelDataExtend->setPost3DWindow(p3d);
			p3d->getWindow()->openFile(file);
			
		}
		emit openPostWindowSig(p3d);
		QStringList variablelist = _modelDataExtend->getVectorVariable();
		if (!variablelist.contains(variable))
		{
			_modelDataExtend->appendVectorVariable(variable);
			this->updatePostTree();
		}
		Post::Post3DWindow* w = p3d->getWindow();
		PipelineObject* popobj = w->getCurrentPipelineObj();
		w->viewVector(popobj, variable);
		w->displayLegendBar(true);

		emit disPlayProp(nullptr);
	}
	void ProjectTreeWithBasicNode::viewRealTimeWindow()
	{
		QStringList files;//= _modelDataExtend->getMonitorNameList();
		if (files.size() > 1) return;
		for (int i = 0; i < files.size(); ++i)
		{
			QString fi = files.at(i);
			QFile f(fi);
			if (!f.exists())
			{
				QMessageBox::warning(nullptr, tr("Warning"), tr("File %1 is not exist").arg(fi));
				return;
			}
		}
		
		if (_realTimeWin == nullptr)
		{
			_realTimeWin = new Post::RealTimeWindowBase(_mainWindow, _data->getID());
			emit openRealTimeWin(_realTimeWin, _data->getID());
		}
	}

	DataProperty::DataBase* ProjectTreeWithBasicNode::getCurrentItemData()
	{
		if (_currentItem == nullptr) return nullptr;
		if (_currentItem == _simulationSettingItem) return _modelDataExtend->getSimlutationSetting();
		if (_currentItem == _solverSettingItem) return _modelDataExtend->getSolverSetting();

		const int index = _currentItem->data(0, Qt::UserRole + 1).toInt() - 1;
		if (index >= 0 )
		{
			ConfigOption::TreeItem* item = _itemList.at(index);
			const int id = item->getDataID();
			DataProperty::DataBase* d = _modelDataExtend->getConfigData(id);
			return d;
		}
		return nullptr;
	}

	

	void ProjectTreeWithBasicNode::updateGeometrySubTree()
	{
		if (_geometryRootItem == nullptr) return;
		_geometryRootItem->takeChildren();
		Geometry::GeometryData* geodata = Geometry::GeometryData::getInstance();
		
		QList<int> geoIDs = _modelDataExtend->getGeometryList();
		for (int i = 0; i < geoIDs.size(); ++i)
		{
			int id = geoIDs.at(i);
			Geometry::GeometrySet* set = geodata->getGeometrySetByID(id);
			if (set == nullptr) continue;
			QString name = set->getName();
			QString icon = "://QUI/icon/geometry.png";
			QTreeWidgetItem* temp = new QTreeWidgetItem(_geometryRootItem, TreeItemType::ProjectGeometryChild);
			temp->setIcon(0, QIcon(icon));
			temp->setText(0, name);
			temp->setData(0, Qt::UserRole, id);
		}
		_geometryRootItem->setExpanded(true);
	}

}