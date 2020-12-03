#include "FunctionTreeSetup.h"
#include "ui_FunctionTreeSetup.h"
#include <QString>
#include <QIcon>
#include <QTreeWidgetItem>
#include <QScrollBar>
#include <QDebug>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QTreeWidget>
#include <QMessageBox>
#include <QIcon>
#include <QPixmap>
#include <QPushButton>
#include <QPalette>
#include <QMenu>
#include <QAction>
#include <QSignalMapper>
#include "DataManager.h"
#include "mainWindow/mainWindow.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/ProjectTreeConfig.h"
#include "ConfigOptions/ProjectTreeInfo.h"
#include "ConfigOptions/TreeItemData.h"
#include "MainWidgets/PropertyTable.h"
#include "ConfigOptions/DataConfig.h"
#include "DataProperty/DataBase.h"
#include "DataProperty/modelTreeItemType.h"
#include "ModelData/modelDataSingleton.h"
#include "ModelData/modelDataBase.h"
#include "ProjectTree/projectTreeBase.h"
#include "MainWidgets/ProjectTreeFactory.h"
#include "DataProperty/DataBase.h"
#include "DataProperty/ParameterBase.h"
#include "DataProperty/ParameterList.h"
#include "DataProperty/ParameterInt.h"
#include "DataProperty/ParameterDouble.h"
#include "DataProperty/ParameterBool.h"
#include "DataProperty/ParameterSelectable.h"
#include "DataProperty/ParameterTable.h"
#include "DataProperty/ParameterString.h"
#include "DataProperty/ParameterPath.h"
#include "ModelBase.h"
#include "DataProperty/modelTreeItemType.h"
#include "BoundaryModel.h"
#include "Common.h"
#include "TreeInformation.h"
#include "Post2DCurveModel.h"
#include "CustomParameterModel.h"
#include "Post3DFileModel.h"
#include "MeshModel.h"
#include "SimulationModel.h"
#include "SolverModel.h"
#include "MonitorModel.h"
#include "PostModel.h"
#include "MonitorModel.h"
#include "PostModel.h"
#include "CurveModel.h"
#include "VectorModel.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/BCConfig.h"
#include "ConfigOptions/PostConfig.h"
#include "ConfigOptions/PostConfigInfo.h"
#include "ConfigOptions/PostCurve.h"
#include "ConfigOptions/MaterialConfig.h"
#include "BCBase/BCBase.h"
#include "BCBase/BCUserDef.h"
#include "Material/Material.h"
#include "FileHelper.h"
#include "DataProperty/ParameterGroup.h"
#include "ParaLinkageManager.h"



#pragma execution_character_set("utf-8")



namespace FastCAEDesigner
{

	FunctionTreeSetup::FunctionTreeSetup(GUI::MainWindow* mainwindow, QWidget *parent) :
		DockWidgetBase(mainwindow, ModuleBase::ModuleType::ProjectWindows),
		ui(new Ui::FunctionTreeSetup),
		_mainwindow(mainwindow)
	{
		ui->setupUi(this);
// 		setAttribute(Qt::WA_ShowModal, true);//属性设置
// 		setWindowModality(Qt::ApplicationModal);//设置阻塞类型
		ui->treeWidget->header()->setVisible(false); //隐藏表头
		ui->propTabWidget->setCurrentIndex(0);
		_propTable = new MainWidget::PropertyTable(_mainwindow, ui->prop /*this*/);
		_propTable->setGeometry(0, 0, 258, 300);
		resizeEvent(nullptr);
		Init();
		//FillTreePhysics();
		FillTreeWidget();
		ui->treeWidget->setExpandsOnDoubleClick(false);

		ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
		connect(ui->treeWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(OnProjectTreeRightClicked(const QPoint&)));
		connect(ui->treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(OnProjectTreeDoubleClicked(QTreeWidgetItem*)));
		connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(OnProjectTreeItemClicked(QTreeWidgetItem*, int)));

		this->repaintTitleBar();
	}

	FunctionTreeSetup::~FunctionTreeSetup()
	{
		delete ui;

		/*
		for (int i = 0; i < _physicsList.count(); i++)
		{
			ModelBase *model = _physicsList.at(i);
			if (nullptr == model)
				continue;
			//model->Clear();
			delete model;
			model = nullptr;
		}

		_physicsList.clear();
		*/
		ClearModelBaseList(_physicsList);
		ClearModelBaseList(_materialList);
	}

	void FunctionTreeSetup::resizeEvent(QResizeEvent *e)
	{
		int w = ui->prop->width();//ui->treeWidget->width();
		_propTable->resize(w);
		//QDockWidget::resizeEvent(e);
		QWidget::resizeEvent(e);
	}

	void FunctionTreeSetup::showEvent(QShowEvent *)
	{
		resizeEvent(nullptr);
	}

	void FunctionTreeSetup::InitProjectData()
	{
		InitPhysicsProjectData();
		IniMaterialProjectData();
	}

	void FunctionTreeSetup::IniMaterialProjectData()
	{
		ConfigOption::MaterialConfig* materialConfig = ConfigOption::ConfigOption::getInstance()->getMaterialConfig();

		if (nullptr == materialConfig)
			return;

		QStringList typeList = materialConfig->getMaterialTypes();
		int count = typeList.count();
		
		for (int i = 0; i < count; i++)
		{
			QString type = typeList.at(i);
			Material::Material* material = materialConfig->getMaterialByType(type);

			
			QString name = material->getType();
			QString icon = Image_Material;
			DataProperty::DataBase *dataBase = material;
			DataProperty::DataBase* modelDataBase = new DataProperty::DataBase();

			if (nullptr == modelDataBase)
				continue;

			modelDataBase->copy(dataBase);
			ModelBase* childModel = CreateModelFactory(TreeItemType::MaterialChild);
			childModel->SetEngName(name);
			childModel->SetChnName(name);
			childModel->SetIconName(icon);
			childModel->SetDataBase(modelDataBase);
			_materialList.append(childModel);
		}

		typeList.clear();
	}

	//初始化
	void FunctionTreeSetup::InitPhysicsProjectData()
	{
		ConfigOption::ProjectTreeConfig* projectTreeConfig = ConfigOption::ConfigOption::getInstance()->getProjectTreeConfig();

		if (nullptr == projectTreeConfig)
			return;

		QList<ProjectTreeType> projectTreeTypeList = projectTreeConfig->getTreeTypes();

		for (int i = 0; i < projectTreeTypeList.count(); i++)
		{
			ProjectTreeType type = projectTreeTypeList.at(i);
			ConfigOption::ProjectTreeInfo* treeModel = projectTreeConfig->getProjectTree(type);

			if (nullptr == treeModel)
				continue;

			_parameterList.clear();
			_parameterGroupList.clear();

			QString nameEng = treeModel->getName();
			QString nameChn = treeModel->getChinese();
			QStringList disableItemList = treeModel->getDisableItems();

			ModelBase* caseRoot = CreatePhysics(nameEng, nameChn);
			caseRoot->setTreeType(type);

			InitPhysicsTreeNodeVisable(disableItemList,caseRoot);
			FillSimulationAndSolverChildModel(caseRoot, treeModel, type);

			QList<ModelBase*> rootModelList = caseRoot->GetChildList();

			ModelBase* simulationModel = GetSpecialTypeModel(rootModelList, TreeItemType::ProjectSimulationSetting);
			FillSimulationRootParameterData(simulationModel, type, TSimulationData);

			ModelBase* solverModel = GetSpecialTypeModel(rootModelList, TreeItemType::ProjectSolver);
			FillSimulationRootParameterData(solverModel, type, TSolverData);

			ModelBase* bcModel = GetSpecialTypeModel(rootModelList, TreeItemType::ProjectBoundaryCondation);
			FillBcData(bcModel, type);

			ModelBase* monitorModel = GetSpecialTypeModel(rootModelList, TreeItemType::ProjectMonitor);
			FillMonitorData(monitorModel, type);

			ModelBase* postModel = GetSpecialTypeModel(rootModelList, TreeItemType::ProjectPost);
			FillPostData(postModel, type);

			_physicsList.append(caseRoot);

			//20200325 xuxinwei  添加仿真以及求解参数列表 
			DataManager::getInstance()->setTreeList(nameChn);
			DataManager::getInstance()->setAllParameterListDict(type, _parameterList);
			DataManager::getInstance()->setAllParameterGroupListDict(type, _parameterGroupList);
			//20200325 xuxinwei
		}

		projectTreeTypeList.clear();
	}

	void FunctionTreeSetup::InitPhysicsTreeNodeVisable(QStringList disableItemList,ModelBase* caseRootModel)
	{
		QList<ModelBase*> childModelList = caseRootModel->GetChildList();

		for (int i = 0; i < disableItemList.count(); i++)
		{
			int type = getDisableItemType(disableItemList.at(i));

			if (((TreeItemType)type == TreeItemType::ProJectPost2DGraph) || ((TreeItemType)type == TreeItemType::ProjectPost3DGraph))
			{
				QList<ModelBase*> postChild = GetSpecialTypeModel(childModelList, TreeItemType::ProjectPost)->GetChildList();

				ModelBase* model = GetSpecialTypeModel(postChild, type);

				if (model == nullptr)
					continue;

				model->SetVisible(false);
			}
			else
			{
				ModelBase* model = GetSpecialTypeModel(childModelList, type);

				if (model == nullptr)
					continue;

				model->SetVisible(false);
			}	
		}
	}

	int FunctionTreeSetup::getDisableItemType(QString key)
	{
		QMap<QString, TreeItemType> disableItem;
		
		disableItem.insert("Mesh Set", TreeItemType::ProjectComponent);
		disableItem.insert("Boundary Condition", TreeItemType::ProjectBoundaryCondation);
		disableItem.insert("Simulation Setting", TreeItemType::ProjectSimulationSetting);
		disableItem.insert("Solver Setting", TreeItemType::ProjectSolver);
		disableItem.insert("Monitor", TreeItemType::ProjectMonitor);
		disableItem.insert("Post", TreeItemType::ProjectPost);
		disableItem.insert("2D Plot", TreeItemType::ProJectPost2DGraph);
		disableItem.insert("Countor", TreeItemType::ProjectPost3DGraph);
		disableItem.insert("Vector", TreeItemType::ProjectPost3DGraph);

		return disableItem[key];
	}

	void FunctionTreeSetup::FillBcData(ModelBase* model, int treeType)
	{
		ProjectTreeType type = (ProjectTreeType)treeType;
		ConfigOption::BCConfig *bcConfig = ConfigOption::ConfigOption::getInstance()->getBCConfig();

		if (nullptr == bcConfig)
			return;

		int count = bcConfig->getBCCount(type);

		//Modified xvdongming 2020-04-02 解决当边界根节点下面没有子节点时，边界条件无法填充的问题
		//if (0 == count)
		//	return;


		QStringList conditionList = bcConfig->getEnabledType(type);
		BoundaryModel* bcModel = (BoundaryModel*)model;
		QString conditionStr = conditionList.join(",");
		bcModel->SetCommonCondition(conditionStr);

		for (int i = 0; i < count; i++)
		{
			BCBase::BCUserDef* bcData = bcConfig->getBCAt(i, type);
			QString nameEng = bcData->getName();
			QString nameChh = bcData->getChinese();
			QString icon = bcData->getIcon();
			DataProperty::DataBase *dataBase = bcData;
			DataProperty::DataBase* modelDataBase = new DataProperty::DataBase();

			if (nullptr == modelDataBase)
				continue;

			modelDataBase->copy(dataBase);
			ModelBase* childModel = CreateModelFactory(TreeItemType::ProjectBoundaryCondationChild);
			childModel->SetEngName(nameEng);
			childModel->SetChnName(nameChh);

			//xuxinwei
			if (!icon.isEmpty())
			{
				DataManager::getInstance()->setIconNameList(icon);
			}

			//Added xvdongming 将文件名称加上系统icon目录名
			QString destPath = FileHelper::GetSystemConfigPath() + "icon/";
			icon = destPath + icon;
			//Added xvdongming 

			childModel->SetIconName(icon);
			childModel->SetDataBase(modelDataBase);
			model->AddNode(childModel);
		}
	}

	void FunctionTreeSetup::FillSimulationRootParameterData(ModelBase* model, int treeType, int dataType)
	{
		ProjectTreeType _type = (ProjectTreeType)treeType;
		ConfigOption::DataConfig* dataConfig = ConfigOption::ConfigOption::getInstance()->getDataConfig();
		DataProperty::DataBase* dataBase = nullptr;

		if (nullptr == dataConfig)
			return;

		if (nullptr == model)
			return;

		if (dataType == TSimulationData)
			dataBase = dataConfig->getSimlutationData(_type);
		else if (dataType == TSolverData)
			dataBase = dataConfig->getSolverSettingData(_type);

		DataProperty::DataBase* modelDataBase = new DataProperty::DataBase();
		modelDataBase->copy(dataBase);
		model->SetDataBase(modelDataBase);

		//20200325   xuxinwei
// 		for (int i = 0; i < modelDataBase->getParameterCount(); i++)
// 		{
// 			DataProperty::ParameterBase* base = modelDataBase->getParameterAt(i);
// 			_parameterList.append(base);
// 		}
		insertParameterToList(modelDataBase);
	}

	void FunctionTreeSetup::FillSimulationAndSolverChildModel(ModelBase* caseRoot, ConfigOption::ProjectTreeInfo* treeModel, int projectTreeType)
	{
		QList<ModelBase*> caseTree = caseRoot->GetChildList();
		QList<ConfigOption::TreeItem*> treeChildList = treeModel->getItemList();

		for (int i = 0; i < treeChildList.count(); i++)
		{
			ConfigOption::TreeItem* treeItem = treeChildList.at(i);
			QString parentString = treeItem->getParent();
			TreeItemType type = treeItem->getType();
			int id = treeItem->getDataID();
			ModelBase* parentModel = nullptr;

			if (parentString == "Simulation Setting")
				parentModel = GetSpecialTypeModel(caseTree, ProjectSimulationSetting);
			else if (parentString == "Solver Setting")
				parentModel = GetSpecialTypeModel(caseTree, ProjectSolver);
			else if (type == TreeItemType::ProjectSimulationSettingGrandSon)
				parentModel = getSimulationSolverChildNameModel(caseTree,parentString);
			else if (type == TreeItemType::ProjectSolverGrandSon)
				parentModel = getSimulationSolverChildNameModel(caseTree, parentString);
			

			if (nullptr == parentModel)
				continue;

			ModelBase* childModel = CreateModelFactory(type);
			if (nullptr == childModel)
				continue;

			childModel->SetChnName(treeItem->getChinese());
			childModel->SetEngName(treeItem->getText());
			QString destPath = FileHelper::GetSystemConfigPath() + "icon/";
			
			//QString iconPath = destPath + treeItem->getIcon();
			QString iconPath = "";
			QString iconName = treeItem->getIcon();

// 			if (!iconName.isEmpty())
// 				iconPath = destPath + treeItem->getIcon();
			//xuxinwei 20200324
			if (!iconName.isEmpty())
			{
				iconPath = destPath + treeItem->getIcon();
				DataManager::getInstance()->setIconNameList(iconName);
			}
			//xuxinwei 20200324

			//childModel->SetIconName(treeItem->getIcon());
			childModel->SetIconName(iconPath);
			parentModel->AddNode(childModel);

			DataProperty::DataBase* dataBase = GetChildDataBase(projectTreeType, id);

			if (nullptr == dataBase)
				continue;

			DataProperty::DataBase* modelDataBase = new DataProperty::DataBase();

			if (nullptr == modelDataBase)
				continue;

			modelDataBase->copy(dataBase);
			childModel->SetDataBase(modelDataBase);

			//20200325 xuxinwei
// 			for (int i = 0; i < modelDataBase->getParameterCount(); i++)
// 			{
// 				DataProperty::ParameterBase* base = modelDataBase->getParameterAt(i);
// 				_parameterList.append(base);
// 			}
			insertParameterToList(modelDataBase);
			
		}

		caseTree.clear();
		treeChildList.clear();
	}

	DataProperty::DataBase* FunctionTreeSetup::GetChildDataBase(int type, int id)
	{
		ConfigOption::DataConfig* dataConfig = ConfigOption::ConfigOption::getInstance()->getDataConfig();
		if (nullptr == dataConfig)
			return nullptr;

		DataProperty::DataBase* dataBase = dataConfig->getConfigData((ProjectTreeType)type, id);
		return dataBase;
	}

	void FunctionTreeSetup::FillPostData(ModelBase* model, int treeType)
	{
		ProjectTreeType type = (ProjectTreeType)treeType;
		ConfigOption::PostConfig *postConfig = ConfigOption::ConfigOption::getInstance()->getPostConfig();

		if (nullptr == postConfig)
			return;

		ConfigOption::PostConfigInfo* postConfigInfo = postConfig->getPostConfigInfo(type);

		if (postConfigInfo == nullptr)
			return;

		QList<ModelBase*> childModelList = model->GetChildList();
		ModelBase* plotModel = GetSpecialTypeModel(childModelList, TreeItemType::ProJectPost2DGraph);
		ModelBase* graphModel = GetSpecialTypeModel(childModelList, TreeItemType::ProjectPost3DGraph);

		int plotCount = postConfigInfo->getPost2DFile().count();
		QList<ConfigOption::PostCurve*> plotCurveList = postConfigInfo->getCurveList();

		for (int i = 0; i < plotCount; i++)
		{
			ModelBase* curveModel = CreateModelFactory(TreeItemType::ProJectPost2DGraphChild);
			QString name = postConfigInfo->getPost2DFile().at(i);
			QList<ConfigOption::PostCurve*> curveList = getPostCurve(plotCurveList, name);

			curveModel->setFileName(name);
			((Post2DCurveModel*)curveModel)->setFileName(name);
			((Post2DCurveModel*)curveModel)->setCurveList(curveList);

			plotModel->AddNode(curveModel);
		}
		
		QString grapChildName = postConfigInfo->getPost3DFile();
		QStringList nodeScalar = postConfigInfo->getNodeScalarVariable();
		QStringList cellScalar = postConfigInfo->getCellScalarVariable();
		QStringList nodeVector = postConfigInfo->getNodeVectorVariable();
		QStringList cellVector = postConfigInfo->getCellVectorVariable();

		if (grapChildName != "")
		{
			ModelBase* graphChildModel = CreateModelFactory(TreeItemType::ProjectPost3DGraphChild);

			graphChildModel->setFileName(grapChildName);
			((Post3DFileModel*)graphChildModel)->setFileName(grapChildName);
			((Post3DFileModel*)graphChildModel)->setDataList(nodeScalar, nodeVector, cellScalar, cellVector);
			
			graphModel->AddNode(graphChildModel);
		}
	}

	void FunctionTreeSetup::FillMonitorData(ModelBase* model, int treeType)
	{
		ProjectTreeType type = (ProjectTreeType)treeType;
		ConfigOption::DataConfig *dataConfig = ConfigOption::ConfigOption::getInstance()->getDataConfig();

		if (nullptr == dataConfig)
			return;

		QStringList fileName = dataConfig->getMonitorFile((ProjectTreeType)treeType);
		QList<ConfigOption::PostCurve*> monitorCurveList = dataConfig->getMonitorCurves((ProjectTreeType)treeType);

		for (int i = 0; i < fileName.count(); i++)
		{
			ModelBase* monitorChildModel = CreateModelFactory(TreeItemType::ProjectMonitorChild);
			QString name = fileName.at(i);
			QList<ConfigOption::PostCurve*> curveList = getPostCurve(monitorCurveList, name);

			monitorChildModel->setFileName(name);
			((Post2DCurveModel*)monitorChildModel)->setFileName(name);
			((Post2DCurveModel*)monitorChildModel)->setCurveList(curveList);

			model->AddNode(monitorChildModel);
		}
		
	}

	ModelBase* FunctionTreeSetup::GetSpecialTypeModel(QList<ModelBase*> modelList, int type)
	{
		TreeItemType _type = (TreeItemType)type;
		int count = modelList.count();

		for (int i = 0; i < count; i++)
		{
			ModelBase* item = modelList.at(i);

			if (nullptr == item)
				continue;

			if (_type == item->GetType())
				return item;
		}

		return nullptr;
	}

	ModelBase* FunctionTreeSetup::CreateModelFactory(int type)
	{
		TreeItemType _type = (TreeItemType)type;

		if (_type == TreeItemType::ProjectSimulationSettingChild)
			return new CustomParameterModel("", "", "", _type, this);

		if (_type == TreeItemType::ProjectSolverChild)
			return new CustomParameterModel("", "", "", _type, this);

		if (_type == TreeItemType::ProjectBoundaryCondationChild)
			return new CustomParameterModel("", "", "", _type, this);

		if (_type == TreeItemType::ProjectMonitorChild)
			return new Post2DCurveModel("", _type, this);

		if (_type == TreeItemType::ProJectPost2DGraphChild)
			return new Post2DCurveModel("", _type, this);

		if (_type == TreeItemType::ProjectPost3DGraphChild)
			return new Post3DFileModel("", _type, this);

		if (_type == TreeItemType::MaterialChild)
			return new CustomParameterModel("", "", "", _type, this);

		if (_type == TreeItemType::ProjectSimulationSettingGrandSon)
			return new CustomParameterModel("", "", "", _type, this);

		if (_type == TreeItemType::ProjectSolverGrandSon)
			return new CustomParameterModel("", "", "", _type, this);

		return new ModelBase("", "", "", TreeItemType::ProjectRoot, this);
	}

	QList<ConfigOption::PostCurve*> FunctionTreeSetup::getPostCurve(QList<ConfigOption::PostCurve*> list, QString name)
	{
		QList<ConfigOption::PostCurve*> ChildList;

		for (int i = 0; i < list.count(); i++)
		{
			ConfigOption::PostCurve* curve = list.at(i);
			QString n = curve->getFile();
			if (name == n)
				ChildList.append(curve);
		}

		return ChildList;
	}
	

	void FunctionTreeSetup::Init()
	{
		ui->projectTab->resize(300, 200);
		ui->propLayout->addWidget(_propTable);
		ui->propTabWidget->setTabPosition(QTabWidget::South);
		ui->propTabWidget->tabBar()->hide();


		_materialRoot = new QTreeWidgetItem(ui->treeWidget, TreeItemType::MaterialRoot);
		_materialRoot->setText(0, tr("Materials"));
		_materialRoot->setIcon(0, QIcon(":/QUI/icon/material.png"));

		_physicsModelRoot = new QTreeWidgetItem(ui->treeWidget, TreeItemType::PhyaicsModelRoot);
		_physicsModelRoot->setText(0, tr("Case"));
		_physicsModelRoot->setIcon(0, QIcon(Image_Physics));
		_physicsModelRoot->setTextColor(0, QColor(255, 0, 0));
		
		//ModelBase *caseList = new ModelBase("Case list", "Case list", PhyaicsModelRoot, this);
		//_physicsModelRoot = caseList->CreateTreeItem();
		//QTreeWidgetItem *case1 = CreateCaseNode(_physicsModelRoot, "Case1", Image_Physics);
		//QTreeWidgetItem *case2 = CreateCaseNode(_physicsModelRoot, "Case2", Image_Physics);
		//QTreeWidgetItem *case2TreeWidgetItem = _case1->CreateTreeItem(_physicsModelRoot);

		//临时修改
		//ModelBase *case1 = CreateCase("Case1234", "Case1234", Image_Physics);
		//_physicsList.append(case1);
		//临时修改

		ui->treeWidget->addTopLevelItem(_materialRoot);
		ui->treeWidget->addTopLevelItem(_physicsModelRoot);
		ui->treeWidget->expandAll();
		InitProjectData();
	}
	
	QTreeWidgetItem* FunctionTreeSetup::CreateTreeWidgetItem(QTreeWidgetItem *root, QString text, QString iconName)
	{
		QTreeWidgetItem *item = new QTreeWidgetItem(root, QStringList(text));
		item->setIcon(0, QIcon(iconName));
		return item;
	}
	
	void FunctionTreeSetup::InitTreeWidget()
	{
		ui->treeWidget->clear();
		DataManager::getInstance()->ClearModelDict();
		//DataManager::DictTreeItemToModel->clear();
		
		_materialRoot = new QTreeWidgetItem(ui->treeWidget, TreeItemType::MaterialRoot);
		_materialRoot->setText(0, tr("Materials"));
		_materialRoot->setIcon(0, QIcon(":/QUI/icon/material.png"));
		//_materialRoot->setTextColor(0, QColor(255, 0, 0));

		_physicsModelRoot = new QTreeWidgetItem(ui->treeWidget, TreeItemType::PhyaicsModelRoot);
		_physicsModelRoot->setText(0, tr("Case"));
		_physicsModelRoot->setIcon(0, QIcon(Image_Physics));
		//_physicsModelRoot->setTextColor(0, QColor(255, 0, 0));
	}

	void FunctionTreeSetup::FillTreeWidget()
	{
		InitTreeWidget();
		FillTreePhysics();
		FillTreeMaterial();
	}
	
	void FunctionTreeSetup::FillTreeMaterial()
	{
		for (int i = 0; i < _materialList.count(); i++)
		{
			ModelBase *model = _materialList.at(i);
			if (nullptr == model)
				continue;
			QTreeWidgetItem *treeItem = model->CreateTreeItem(_materialRoot);
			treeItem->setExpanded(true);
		}

		//ui->treeWidget->expandAll();
		_materialRoot->setExpanded(true);

		return;
	}

	void FunctionTreeSetup::FillTreePhysics()
	{
		/*
		ui->treeWidget->clear();
		DataManager::getInstance()->ClearModelDict();
		_physicsModelRoot = new QTreeWidgetItem(ui->treeWidget, TreeItemType::PhyaicsModelRoot);
		_physicsModelRoot->setText(0, tr("Case list"));
		_physicsModelRoot->setIcon(0, QIcon(Image_Physics));
		_physicsModelRoot->setTextColor(0, QColor(255, 0, 0));
		*/

		for (int i = 0; i < _physicsList.count(); i++)
		{
			ModelBase *model = _physicsList.at(i);
			if (nullptr == model)
				continue;
			QTreeWidgetItem *treeItem = model->CreateTreeItem(_physicsModelRoot);
			treeItem->setExpanded(true);
		}

		//ui->treeWidget->expandAll();
		_physicsModelRoot->setExpanded(true);

		return;
	}

	void FunctionTreeSetup::OnProjectTreeRightClicked(const QPoint &point)
	{
		QMenu* menu = new QMenu(ui->treeWidget);

		//QModelIndex indexSelect = ui->treeWidget->indexAt(point);
		QString s = ui->treeWidget->currentItem()->text(0)/*indexSelect.data().toString()*/;

		ModelBase* model = DataManager::getInstance()->GetModelFromDict(ui->treeWidget->currentItem());
		int treeParent = 0;
		if (ui->treeWidget->currentItem()->parent() != nullptr)
			treeParent = returnParent(ui->treeWidget->currentItem());

		if (((s == "材料") || (s == "Materials")) && (ui->treeWidget->currentItem()->parent() == nullptr))
		{
			QAction* insertchild = new QAction(tr("Add Material"), this);
			QAction* deleteItem = new QAction(tr("Clear All"), this);
			menu->addAction(insertchild);
			
			//Added xvdongming 2019-12-24 只有材料列表中有数据时，才显示【清除所有】菜单。
			if (_materialList.count() > 0)
			{
				menu->addAction(deleteItem);
			}
			//Added xvdongming 2019-12-24 只有材料列表中有数据时，才显示【清除所有】菜单。
			
			connect(insertchild, SIGNAL(triggered()), this, SLOT(OnInsertMaterialTree()));
			connect(deleteItem, SIGNAL(triggered()), this, SLOT(OnDeleteAllMaterialItem()));
		}
		else if (((s == "Case") || (s == "算例"))&& (ui->treeWidget->currentItem()->parent() == nullptr))
		{
			QAction* insertchild = new QAction(tr("Add Case"), this);
			//QAction* hidethis = new QAction(tr("hide this"), this);
			menu->addAction(insertchild);
			//menu->addAction(hidethis);
			connect(insertchild, SIGNAL(triggered()), this, SLOT(OnInsertPhysicsTree()));
			//connect(hidethis, SIGNAL(triggered()), ui->treeWidget, SLOT(OnHideThis()));
		}
		else
		{
			if (treeParent == 1)
			{
				createTreeRootMenu(menu, ui->treeWidget->currentItem());
			}
			else if ((treeParent == 2) && (model->GetType() == TreeItemType::ProjectPost))
			{
				createTreePostMenu(menu, ui->treeWidget->currentItem());
			}
			else if (treeParent == 2)
			{
				createTreeNodeMenu(menu, ui->treeWidget->currentItem());
			}
			else if (treeParent == 3)
			{
				QAction* deleteItem = new QAction(tr("Delete"), this);
				menu->addAction(deleteItem);
				connect(deleteItem, SIGNAL(triggered()), this, SLOT(OnDeleteItem())); 				
			}			

		}

		if (menu == nullptr)
			return;

		menu->exec(QCursor::pos());
	}

	void FunctionTreeSetup::OnInsertChild()
	{
		
		QTreeWidgetItem* parentItem = ui->treeWidget->currentItem();

		if (nullptr == parentItem)
			return;

		ModelBase* parentModel = DataManager::getInstance()->GetModelFromDict(parentItem);

		if (nullptr == parentModel)
			return;

		ModelBase* childModel = parentModel->CreateChildModel(parentItem);
		parentItem->setExpanded(true);
	}

	void FunctionTreeSetup::OnHideThis()
	{
		QTreeWidgetItem* childItem = ui->treeWidget->currentItem();
		ModelBase* childModel = DataManager::getInstance()->GetModelFromDict(childItem);
		if (childModel != nullptr)
		{
			childModel->SetVisible(false);
			//FillTree();
			childItem->setHidden(true);
		}
		//ui->treeWidget->currentItem()->setDisabled(true);
		//ui->treeWidget->currentItem()->setExpanded(false);
	}

	void FunctionTreeSetup::OnShowThis(int childindex)
	{
		//qDebug() << i;
		QTreeWidgetItem* childItem = ui->treeWidget->currentItem()->child(childindex);
		ModelBase* childModel = DataManager::getInstance()->GetModelFromDict(childItem);
		if (childModel != nullptr)
		{
			childModel->SetVisible(true);
			//FillTree();
			childItem->setHidden(false);
		}
		
	}

	int FunctionTreeSetup::returnParent(QTreeWidgetItem* treeparent)
	{
		int intParent;
		QString s = treeparent->parent()->text(0);
		ModelBase* model = DataManager::getInstance()->GetModelFromDict(treeparent->parent());

		if ((model == nullptr) && ((s == "Materials") || (s == "材料")))
			return 3;
		
		//Added xvdongming 2020-02-11 添加判断父节点类型
		if (nullptr == model)
		{
			return 1;//这个值我不确定，鑫伟仔细看看。
		}

		if (model->GetType() == TreeItemType::ProjectRoot)
		{
			intParent = 2;
		}
		//Added xvdongming 2020-02-11 添加判断父节点类型
		else if ((s == "Case") || (s == "算例"))
		{
			intParent = 1;
		}
		else if ((model->GetType() == TreeItemType::ProjectComponent) || (model->GetType() == TreeItemType::ProjectMonitor)
			|| (model->GetType() == TreeItemType::ProjectBoundaryCondation) /*|| (model->GetType() == TreeItemType::ProjectSolver) */
			/*|| (model->GetType() == TreeItemType::ProjectSimulationSetting)*/ || (model->GetType() == TreeItemType::ProJectPost2DGraph)			  
			|| (model->GetType() == TreeItemType::ProjectPost3DGraph) || (model->GetType() == TreeItemType::ProjectSimulationSettingChild)
			|| (model->GetType() == TreeItemType::ProjectSolverChild))
		{
			intParent = 3;
		}
		else
		{
			intParent = 2;
		}
// 		else if ((s == "Mesh") || (s == "Simulation") || (s == "Boundary") || (s == "Curve")
// 			|| (s == "Vector") || (s == "Solve") || (s == "Monitor"))
// 		{
// 			intParent = 3;
// 		}
// 		else if (s == "Post")
// 		{
// 			intParent = 4;
// 		}
// 		else
// 		{
// 			intParent = 2;
// 		}
		

		return intParent;
	}

	void FunctionTreeSetup::DeletePhysicsModel(ModelBase *model)
	{
		for (int i = 0; i < _physicsList.count(); i++)
		{
			ModelBase *item = _physicsList.at(i);
			if (nullptr == item)
				continue;
			if (item == model)
			{
				_physicsList.removeOne(model);
				break;
			}
		}
	}

	void FunctionTreeSetup::OnDeleteItem(/*int flag*/)
	{
		//Added xvdongming 2019-12-27
		QString title = tr("Warning");
		QString msg = tr("Current node and data will be cleared. Are you sure?");
		QMessageBox::StandardButton b = QMessageBox::warning(_mainWindow, title, msg, QMessageBox::Yes | QMessageBox::No);
		
		if (b != QMessageBox::Yes)
			return;
		//Added xvdongming 2019-12-27
		
		QTreeWidgetItem* parentItem = ui->treeWidget->currentItem()->parent();
		QTreeWidgetItem* childItem = ui->treeWidget->currentItem();
		ModelBase* parentModel = DataManager::getInstance()->GetModelFromDict(parentItem);
		ModelBase* childModel = DataManager::getInstance()->GetModelFromDict(childItem);
		TreeItemType modelType = (TreeItemType)childModel->GetType();

		//如果为材料的子节点
		if (nullptr == parentModel &&  modelType == TreeItemType::MaterialChild)
		{
			_materialList.removeOne(childModel);
			delete childModel;
			childModel = nullptr;
			FillTreeWidget();
			return;
		}

		//如果为算例的根节点
		if (parentModel == nullptr && childModel!= nullptr)
		{
			//parentModel->DelNode(childModel);
			//FillTree();

			//QList<QTreeWidgetItem*> childList = childItem->takeChildren();
			//DataManager::getInstance()->DeleteModelToDict(childList);
			parentItem->removeChild(childItem);
			DeletePhysicsModel(childModel);
			//FillTreePhysics();
			FillTreeWidget(); 
			return;
		}

		//如果为算例的其他节点
		if ((parentModel != nullptr) && (childModel != nullptr))
		{
			if (childItem->childCount() == 0)
			{
				parentModel->DelNode(childModel);
				parentItem->removeChild(childItem);
				DataManager::getInstance()->DeleteModelToDict(childItem);
				//FillTree();
			}
		}	
	}

	void FunctionTreeSetup::OnInsertPhysicsTree()
	{
		QList<QString> usedChnNameList = GetCaseUsedNameList(_physicsList, TChinese);
		QList<QString> usedEngNameList = GetCaseUsedNameList(_physicsList, TEnglish);
		
		TreeInformation dlg(nullptr);
		dlg.SetUsedChnNameList(usedChnNameList);
		dlg.SetUsedEngNameList(usedEngNameList);
		dlg.SetWorkModel(TreeInformation::CreateRootNodeMode);
		
		if (dlg.exec() == QDialog::Rejected)
			return;

		ModelBase* treecase = CreatePhysics(dlg.GetEnglishName(), dlg.GetChineseName());
		_physicsList.append(treecase);
		//FillTreePhysics();
		FillTreeWidget();
		//if (treecase != nullptr)
		//{
		//	FillTreePhysics();
		//}
	}

	//获取给定数据列表中，已经使用的中文或英文名字列表.参数：0:中文、1：英文
	QList<QString> FunctionTreeSetup::GetCaseUsedNameList(QList<ModelBase*> modelList, int nameType)
	{
		QList<QString> nameList;
		int count = modelList.count();
		
		if (0 == count)
			return nameList;
		
// 		for (int i = 0; i < count; i++)
// 		{
// 			ModelBase* model = modelList.at(i);
// 			
// 			if (nullptr == model)
// 				continue;;
// 			
// 			if (0 == TChinese)
// 				nameList.append(model->GetChnName());
// 			else
// 				nameList.append(model->GetEngName());
// 		}
		if (nameType == TChinese)
		{
			for (int i = 0; i < count; i++)
			{
				ModelBase* model = modelList.at(i);

				if (nullptr == model)
					continue;;

				nameList.append(model->GetChnName());
			}
		}
		else
		{
			for (int i = 0; i < count; i++)
			{
				ModelBase* model = modelList.at(i);

				if (nullptr == model)
					continue;;

				nameList.append(model->GetEngName());
			}
		}
		
		return nameList;
	}

	ModelBase* FunctionTreeSetup::CreatePhysics(QString nameEng, QString nameChn)
	{
		ModelBase *root = new ModelBase(nameEng, nameChn, Image_Physics, TreeItemType::ProjectRoot, this);
		root->SetIsEdit(true);

		ModelBase *mesh = new MeshModel("Mesh", tr("Mesh"), Image_Mesh, this);
		ModelBase *simulation = new SimulationModel("Simulation", tr("Simulation"), Image_Simulation, this);
		ModelBase *boundary = new BoundaryModel("Boundary", tr("Boundary"), Image_BC, this);
		ModelBase *solver = new SolverModel("Solver", tr("Solver"), Image_Solve, this);
		ModelBase *monitor = new MonitorModel("Monitor", tr("Monitor"), Image_Monitor,this);		
		ModelBase *post = new PostModel("Post", tr("Post"), Image_Post, this);
		ModelBase *curve = new CurveModel("2DPlot", tr("2DPlot"), Image_Curve, this);
		ModelBase *vector = new VectorModel("3DGraph", tr("3DGraph"), Image_Vector, this);
		
		root->AddNode(mesh);
		root->AddNode(simulation);
		root->AddNode(boundary);
		root->AddNode(solver);
		root->AddNode(monitor);
		root->AddNode(post);
		post->AddNode(curve);
		post->AddNode(vector);

		return root;
		
		//ui->treeWidget->children();
	}

	QMenu* FunctionTreeSetup::createTreeRootMenu(QMenu* menu, QTreeWidgetItem* treeNode)
	{
		//QMenu* menu = new QMenu(ui->treeWidget);
		int flag = 0;
		QSignalMapper *signalMapper = new QSignalMapper(this);
		QSignalMapper *nameMapper = new QSignalMapper(this);

		ModelBase* model = DataManager::getInstance()->GetModelFromDict(treeNode);
		if (model == nullptr)
			return menu;

		int type = model->getTreeType();

		QMenu *childmenu = new QMenu(tr("Show Child"), ui->treeWidget);
		int childCount = treeNode->childCount();
		for (int i = 0; i < childCount; i++)
		{
			QTreeWidgetItem *childItem = treeNode->child(i);
			if (childItem == nullptr)
				continue;

			ModelBase *childModel = DataManager::getInstance()->GetModelFromDict(childItem);
			if (childModel == nullptr)
				continue;

			if (childModel->GetVisible() == false)
			{
				flag = 1;
				QAction* action = new QAction(childModel->GetChnName(), this);
				childmenu->addAction(action);
				connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
				signalMapper->setMapping(action, i);
			}

		}
		if (flag == 1)
		{
			menu->addMenu(childmenu);
			connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(OnShowThis(int)));
		}
		QAction* deletetree = new QAction(tr("Delete"), this);
		QAction* paraLinkage = new QAction(tr("ParameterLinkage"), this);

		menu->addAction(deletetree);
		menu->addAction(paraLinkage);
		connect(deletetree, SIGNAL(triggered()), this, SLOT(OnDeleteItem()));

		connect(paraLinkage, SIGNAL(triggered()), nameMapper, SLOT(map()));
		nameMapper->setMapping(paraLinkage, type);
		connect(nameMapper, SIGNAL(mapped(int)), this, SLOT(onShowParameterLinkage(int)));

		return menu;
	}
	QMenu* FunctionTreeSetup::createTreeNodeMenu(QMenu* menu, QTreeWidgetItem* childNode)
	{
		ModelBase* childModel = DataManager::getInstance()->GetModelFromDict(childNode);
		if (childModel == nullptr)
			return menu;
		if (childModel->GetType() == TreeItemType::ProjectSolverChild || childModel->GetType() == TreeItemType::ProjectSimulationSettingChild)
		{
			QAction* insertchild = new QAction(tr("Add Child"), this);
			QAction* deletechild = new QAction(tr("Delete"), this);

			menu->addAction(insertchild);
			menu->addAction(deletechild);

			connect(insertchild, SIGNAL(triggered()), this, SLOT(OnInsertChild()));
			connect(deletechild, SIGNAL(triggered()), this, SLOT(OnDeleteItem()));

			return menu;
		}
		else if (childModel->GetType() == TreeItemType::ProjectBoundaryCondation)
		{
			QAction* insertchild = new QAction(tr("Add Boundary Condition"), this);
			menu->addAction(insertchild);
			connect(insertchild, SIGNAL(triggered()), this, SLOT(OnInsertChild()));
		}
		else if ((childModel->GetType() == TreeItemType::ProjectComponent) ||
			(((childModel->GetType() == TreeItemType::ProjectPost3DGraph) && (childModel->GetChildList().count() >= 1))))
		{
			// 					QAction* insertchild = new QAction(tr("insert child"), this);
			// 					menu->addAction(insertchild);
			//connect(insertchild, SIGNAL(triggered()), this, SLOT(OnInsertChild()));
		}
		else if (childModel->GetType() == TreeItemType::ProjectMonitor)
		{
			QAction* insertchild = new QAction(tr("Add Monitor File"), this);
			menu->addAction(insertchild);
			connect(insertchild, SIGNAL(triggered()), this, SLOT(OnInsertChild()));
		}
		else if ((childModel->GetType() == TreeItemType::ProJectPost2DGraph) || (childModel->GetType() == TreeItemType::ProjectPost3DGraph))
		{
			QAction* insertchild = new QAction(tr("Add File"), this);
			menu->addAction(insertchild);
			connect(insertchild, SIGNAL(triggered()), this, SLOT(OnInsertChild()));
		}
		else
		{
			QAction* insertchild = new QAction(tr("Add Child"), this);
			menu->addAction(insertchild);
			connect(insertchild, SIGNAL(triggered()), this, SLOT(OnInsertChild()));
		}
		QAction* hidethis = new QAction(tr("Hide This"), this);
		menu->addAction(hidethis);
		connect(hidethis, SIGNAL(triggered()), this, SLOT(OnHideThis()));

		return menu;
	}
	QMenu* FunctionTreeSetup::createTreePostMenu(QMenu* menu, QTreeWidgetItem* postNode)
	{
		//QMenu* menu = new QMenu(ui->treeWidget);
		int flag = 0;
		QSignalMapper *signalMapper = new QSignalMapper(this);
		QMenu *childmenu = new QMenu(tr("Show Child"), ui->treeWidget);
		int childcount = postNode->childCount();
		for (int i = 0; i < childcount; i++)
		{
			QTreeWidgetItem* child = postNode->child(i);
			if (child == nullptr)
				continue;

			if (child->isHidden())
			{
				flag = 1;
				QAction* action = new QAction(child->text(0), this);
				childmenu->addAction(action);
				connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
				signalMapper->setMapping(action, i);
			}

		}
		if (flag == 1)
		{
			menu->addMenu(childmenu);
			connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(OnShowThis(int)));
		}
		//QAction* insertchild = new QAction(tr("insert child"), this);
		QAction* hidechild = new QAction(tr("Hide This"), this);
		//menu->addAction(insertchild);
		menu->addAction(hidechild);
		//connect(insertchild, SIGNAL(triggered()), this, SLOT(OnInsertChild()));
		connect(hidechild, SIGNAL(triggered()), this, SLOT(OnHideThis()));

		return menu;
	}
	void FunctionTreeSetup::OnProjectTreeDoubleClicked(QTreeWidgetItem* treeItem)
	{
		QTreeWidgetItem* parentItem = ui->treeWidget->currentItem()->parent();
		//if (parentItem == nullptr)
		//	return;

		ModelBase* parentModel = DataManager::getInstance()->GetModelFromDict(parentItem);
		//if (parentModel == nullptr)
		//	return;

		ModelBase *model = DataManager::getInstance()->GetModelFromDict(treeItem);
		if (nullptr == model)
			return;

		if (model->GetType() == TreeItemType::ProjectRoot)//算例根节点
		{
			QList<QString> usedChnNameList = GetCaseUsedNameList(_physicsList, TChinese);
			usedChnNameList.removeOne(model->GetChnName());
			QList<QString> usedEngNameList = GetCaseUsedNameList(_physicsList, TEnglish);
			usedEngNameList.removeOne(model->GetEngName());

			TreeInformation dlg(treeItem, model);
			dlg.SetUsedChnNameList(usedChnNameList);
			dlg.SetUsedEngNameList(usedEngNameList);
			dlg.SetWorkModel(TreeInformation::CreateRootNodeMode);
			int r = dlg.exec();

			if (QDialog::Accepted == r)
				UpdateDataBase(model);
			
			return;
		}
		else if (model->GetType() == TreeItemType::MaterialChild)//材料子节点
		{
			ModelBase* _parentModel = new ModelBase();
			
			for (int i = 0; i < _materialList.count(); i++)
			{
				ModelBase* item = _materialList.at(i);
				if (nullptr == item)
					continue;
				ModelBase* childModel = new ModelBase();
				childModel->Copy(item);
				_parentModel->AddNode(childModel);
			}

			model->SetParentModelBase(_parentModel);
			int r = model->ShowEditor(treeItem, this);

			if (QDialog::Accepted == r)
				UpdateDataBase(model);

			delete _parentModel;
			_parentModel = nullptr;
			return;

		}
		else//其他节点
		{
			SetParentModel(treeItem);

			model->SetParentModelBase(parentModel);
			int r = model->ShowEditor(treeItem, this);

			if (QDialog::Accepted == r)
				UpdateDataBase(model);

			return;
		}


	}

	void FunctionTreeSetup::UpdateDataBase(ModelBase* model)
	{
		if (nullptr == model)
			return;

		DataProperty::DataBase* dataBase = model->GetDataBase();
		
		if (nullptr == dataBase)
			return;
		 
		_propTable->updateTable(dataBase);
	}

	void FunctionTreeSetup::OnProjectTreeItemClicked(QTreeWidgetItem* treeItem, int index)
	{
		ModelBase* model = DataManager::getInstance()->GetModelFromDict(treeItem);
		if (model == nullptr)
			return;
		UpdateDataBase(model);
	}

	QList<ModelBase*> FunctionTreeSetup::GetPhysicsList()
	{
		return _physicsList;
	}

	QList<ModelBase*> FunctionTreeSetup::GetMaterialList()
	{
		return  _materialList;
	}

	void FunctionTreeSetup::OnDeleteAllMaterialItem()
	{
		if (0 == _materialList.count())
			return;


		QString title = tr("Warning");
		QString msg = tr("All record will be cleared. Are you sure?");
		QMessageBox::StandardButton b = QMessageBox::warning(_mainWindow, title, msg, QMessageBox::Yes | QMessageBox::No);
		
		if (b != QMessageBox::Yes) 
			return;

		ClearModelBaseList(_materialList);
		FillTreeWidget();
	}

	void FunctionTreeSetup::OnInsertMaterialTree()
	{
		QList<QString> usedChnNameList = GetCaseUsedNameList(_materialList, TChinese);
		QList<QString> usedEngNameList = GetCaseUsedNameList(_materialList, TEnglish);

		TreeInformation dlg(nullptr);
		dlg.SetUsedChnNameList(usedChnNameList);
		dlg.SetUsedEngNameList(usedEngNameList);
		dlg.SetWorkModel(TreeInformation::CreateRootNodeMode);

		if (dlg.exec() == QDialog::Rejected)
			return;

		ModelBase *material = new CustomParameterModel(dlg.GetEnglishName(), dlg.GetChineseName(), Image_Material, TreeItemType::MaterialChild, this);
		_materialList.append(material);
		FillTreeWidget();
	}

	void FunctionTreeSetup::ClearModelBaseList(QList<ModelBase*> &list)
	{
		for (int i = 0; i < list.count(); i++)
		{
			ModelBase *model = list.at(i);
			if (nullptr == model)
				continue;
			delete model;
			model = nullptr;
		}

		list.clear();
	}

	void FunctionTreeSetup::insertParameterToList(DataProperty::DataBase* model)
	{
		for (int i = 0; i < model->getParameterCount(); i++)
		{
			if (model->getParameterAt(i) == nullptr)
				continue;

			_parameterList.append(model->getParameterAt(i));
			DataManager::getInstance()->appendParameterNameList(model->getParameterAt(i)->getDescribe());
		}

		for (int i = 0; i < model->getParameterGroupCount(); i++)
		{
			DataProperty::ParameterGroup* paraGroup = model->getParameterGroupAt(i);
			if (paraGroup == nullptr)
				continue;

			_parameterGroupList.append(paraGroup);
			DataManager::getInstance()->appendParaGroupNameList(paraGroup->getDescribe());
		//	paraGroup->getParameterCount();
			for (int j = 0; j < paraGroup->getParameterCount(); j++)
			{
				if (paraGroup->getParameterAt(j) == nullptr)
					continue;

				_parameterList.append(paraGroup->getParameterAt(j));
				DataManager::getInstance()->appendParameterNameList(paraGroup->getParameterAt(j)->getDescribe());
			}
			//qDebug() << i;
		}
	}

	void FunctionTreeSetup::SetParentModel(QTreeWidgetItem* item)
	{
		while (item->parent() != nullptr)
		{
			ModelBase* model = DataManager::getInstance()->GetModelFromDict(item);
			ModelBase* parenModel = DataManager::getInstance()->GetModelFromDict(item->parent());

			model->SetParentModelBase(parenModel);

			item = item->parent();
		}
	}

	ModelBase* FunctionTreeSetup::getSimulationSolverChildNameModel(QList<ModelBase*> modelList, QString name)
	{
		for (int i = 0; i < modelList.count(); i++)
		{
			ModelBase* item = modelList.at(i);
			if (item == nullptr)
				continue;

			if (item->GetType() == TreeItemType::ProjectSolver || item->GetType() == TreeItemType::ProjectSimulationSetting)
			{
				ModelBase* model = getChildModelFromName(item, name);
				if (model != nullptr)
					return model;
			}

// 			if (item->GetType() == TreeItemType::ProjectSimulationSetting)
// 			{
// 				QList<ModelBase*> list = item->GetChildList();
// 				for (int j = 0; j < list.count(); j++)
// 				{
// 					ModelBase* model = list.at(j);
// 					if (model == nullptr)
// 						continue;
// 
// 					if (model->GetEngName() == name)
// 						return model;
// 					
// 				}
// 			}
// 			else if (item->GetType() == TreeItemType::ProjectSolver)
// 			{
// 				QList<ModelBase*> list = item->GetChildList();
// 				for (int j = 0; j < list.count(); j++)
// 				{
// 					ModelBase* model = list.at(j);
// 					if (model == nullptr)
// 						continue;
// 
// 					if (model->GetEngName() == name)
// 						return model;
// 
// 				}
// 			}
		}

		return nullptr;
	}

	ModelBase* FunctionTreeSetup::getChildModelFromName(ModelBase* base, QString name)
	{
		QList<ModelBase*> list = base->GetChildList();

		for (int i = 0; i < list.count(); i++)
		{
			ModelBase* model = list.at(i);
			if (model == nullptr)
				continue;

			if (model->GetEngName() == name)
				return model;

			ModelBase* child = getChildModelFromName(model, name);

			if (child != nullptr)
				return child;

		}

		return nullptr;
	}

	void FunctionTreeSetup::onShowParameterLinkage(int type)
	{
		ParaLinkageManager dlg(type);
		dlg.exec();
	}
}       