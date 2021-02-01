#include "DataManager.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/BCConfig.h"
#include "ConfigOptions/BCConfigReader.h"
#include "ConfigOptions/DataConfig.h"
#include "ConfigOptions/DataConfigReader.h"
#include "ConfigOptions/GlobalConfig.h"
#include "ConfigOptions/GeometryConfig.h"
#include "ConfigOptions/MeshConfig.h"
#include "ConfigOptions/SolverConfig.h"
#include "ConfigOptions/GlobalConfigReader.h"
#include "ConfigOptions/PostConfig.h"
#include "ConfigOptions/ObserverConfig.h"
#include "ConfigOptions/ObserverConfigReader.h"
#include "ConfigOptions/MaterialConfig.h"
#include "ConfigOptions/ProjectTreeConfig.h"
#include "ConfigOptions/TreeConfigReader.h"
#include "WriterGlobalConfig.h"
#include "FileHelper.h"
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include "WriteTreeConfig.h"
#include "WriterBCConfig.h"
#include "WriterDataConfig.h"
#include "writerMaterialConfig.h"
#include "ConfigOptions/ProjectTreeInfo.h"
#include "ModelBase.h"
#include "DataProperty/modelTreeItemType.h"
#include "ConfigOptions/ProjectTreeInfo.h"

namespace FastCAEDesigner
{
	DataManager* DataManager::_instance = nullptr;

	
	DataManager::DataManager(QObject *parent) : QObject(parent)
	{
		_bcConfig = nullptr;
		_dataConfig = nullptr;
		_globalConfig = nullptr;
		_geoConfig = nullptr;
		_meshConfig = nullptr;
		_postConfig = nullptr;
		_solverOption = nullptr;
		_materialConfig = nullptr;
		_projectTreeConfig = nullptr;
		
		Init();
	}

	DataManager::~DataManager()
	{
		if (_bcConfig != nullptr) 
			delete _bcConfig;
		_bcConfig = nullptr;

		if (_dataConfig != nullptr) 
			delete _dataConfig;
		_dataConfig = nullptr;

		if (_globalConfig != nullptr) 
			delete _globalConfig;
		_globalConfig = nullptr;

		if (_geoConfig != nullptr) 
			delete _geoConfig;
		_geoConfig = nullptr;

		if (_meshConfig != nullptr) 
			delete _meshConfig;
		_meshConfig = nullptr;

		if (_postConfig != nullptr) 
			delete _postConfig;
		_postConfig = nullptr;

		if (_solverOption != nullptr) 
			delete _solverOption;
		_solverOption = nullptr;

		if (_materialConfig != nullptr) 
			delete _materialConfig;
		_materialConfig = nullptr;

		if (_projectTreeConfig != nullptr) 
			delete _projectTreeConfig;
		_projectTreeConfig = nullptr;

		_physicsList.clear();
		DictTreeItemToModel.clear();

		_iconNameList.clear();//20200324 xuxinwei

		_allParameterDict.clear();
		_allParameterGroupDict.clear();
		_treeList.clear();
	}
	
	DataManager* DataManager::getInstance()
	{
		if (_instance == nullptr)
			_instance = new DataManager;
		return _instance;
	}

	//初始化
	void DataManager::Init()
	{
		_bcConfig = new ConfigOption::BCConfig();
		_dataConfig = new ConfigOption::DataConfig();
		_globalConfig = new ConfigOption::GlobalConfig();
		_geoConfig = new ConfigOption::GeometryConfig();
		_meshConfig = new ConfigOption::MeshConfig();
		_postConfig = new ConfigOption::PostConfig();
		_solverOption = new ConfigOption::SolverOption();
		_observerConfig = new ConfigOption::ObserverConfig();
		_materialConfig = new ConfigOption::MaterialConfig();
		_projectTreeConfig = new ConfigOption::ProjectTreeConfig();
	}

	

	void DataManager::DeleteModelToDict(QTreeWidgetItem* treeItem)
	{
// 		QMap<QTreeWidgetItem*, ModelBase*>::iterator it;
// 		//for (it == DictTreeItemToModel.begin(); it != DictTreeItemToModel.end();)
// 		for (it = DictTreeItemToModel.begin(); it != DictTreeItemToModel.end();)
// 		{
// 			if (treeItem == it.key())
// 			{
// 				DictTreeItemToModel.erase(it);
// 			}
// 			
// 		}
		DictTreeItemToModel.remove(treeItem);

	}
	void DataManager::DeleteModelToDict(QList<QTreeWidgetItem*> treeItemList)
	{
		//QMap<QTreeWidgetItem*, ModelBase*>::iterator it;
		//for (it == DictTreeItemToModel.begin(); it != DictTreeItemToModel.end();)
		for (int i = 0; i < treeItemList.count(); i++)
		{
			DictTreeItemToModel.remove(treeItemList.at(i));
			
		}	

	}

	void DataManager::ClearModelDict()
	{
		this->DictTreeItemToModel.clear();
	}

	ModelBase* DataManager::GetModelFromDict(QTreeWidgetItem* treeItem)
	{
		if (nullptr == treeItem)
			return nullptr;

		return DictTreeItemToModel[treeItem];
	}

	void DataManager::InsertModelToDict(QTreeWidgetItem* treeItem, ModelBase* model)
	{
		DictTreeItemToModel.insert(treeItem, model);
	}

	//写配置信息到系统
	bool DataManager::WriteInfoToServerPath(GUI::MainWindow* mainWindow)
	{
		WriterGlobalConfig globalConfigWriter(mainWindow);
		_globalConfig->enableMaterial((_materialList.count() > 0) ? true : false);//Added xvdongming 2020-02-15 如果当前材料数不为空，则设定材料节点显示，否则设置隐藏
		globalConfigWriter.Write(_globalConfig,_geoConfig,_meshConfig);
		CopyLogoAndWelcomImageToSystem();
		CopyUserManualToSystem();

		WriteTreeConfig treeConfigWriter;
		treeConfigWriter.Write(_physicsList);
		
		QList<ModelBase*> bcModelList = DataManager::GetSpecifiedTypeModelList(_physicsList, TreeItemType::ProjectBoundaryCondation);
		WriteBCConfig bcConfigWriter;
		bcConfigWriter.Write(bcModelList);

		WriteDataConfig dataConfigWriter;
		dataConfigWriter.Write(_physicsList);

		WriterMaterialConfig materialConfigWriter;
		materialConfigWriter.Write(_materialList);
		     
		return true;
	}

	//获取树列表中，指定类型节点的对象列表
	QList<ModelBase*> DataManager::GetSpecifiedTypeModelList(QList<ModelBase*> modelList, TreeItemType type)
	{
		QList<ModelBase*> bcModelList;
		int count = modelList.count();
		
		if (0 == count)
			return bcModelList;

		for (int i = 0; i < count; i++)
		{
			ModelBase* model = modelList.at(i);
			if (nullptr == model)
				continue;
			QList<ModelBase*> childNodeList = model->GetChildList();
			
			for (int j = 0; j < childNodeList.count(); j++)
			{
				ModelBase* child = childNodeList.at(j);
				if (nullptr == child)
					continue;
				
				//if (child->GetType() == TreeItemType::ProjectBoundaryCondation)
				if (child->GetType() == type)
					bcModelList.append(child);
			}
		}

		return bcModelList;
	}
	
	void DataManager::appendParameterNameList(QString name)
	{
		if (_parameterNameList.contains(name))
			return;

		_parameterNameList.append(name);
	}

	QList<QString> DataManager::getParameterNameList()
	{
		return _parameterNameList;
	}

	void DataManager::removeParameterName(QString name)
	{
		_parameterNameList.removeOne(name);
	}

	void DataManager::appendParaGroupNameList(QString name)
	{
		if (_paraGroupNameList.contains(name))
			return;

		_paraGroupNameList.append(name);
	}

	QList<QString> DataManager::getParaGroupNameList()
	{
		return _paraGroupNameList;
	}

	void DataManager::removeParaGroupName(QString name)
	{
		_paraGroupNameList.removeOne(name);
	}

	void DataManager::CopyFileToSystem(QString fileName, QString path)
	{
		QDir iconDir(path);
		if (!iconDir.exists())
			iconDir.mkpath(path);

		if (!fileName.isEmpty())
		{
			QFileInfo fileInfo(fileName);
			QString destFileName = fileInfo.fileName();
			bool b = FileHelper::CopyFileToPath(fileName, path + destFileName, true);
			qDebug() << b;
		}
	}

	void DataManager::CopyFileToSystem(QString fileName, QString path, QString destFileName)
	{
		QDir iconDir(path);
		if (!iconDir.exists())
			iconDir.mkpath(path);

		if (!fileName.isEmpty())
		{
			QFileInfo fileInfo(fileName);
			bool b = FileHelper::CopyFileToPath(fileName, path + destFileName, true);
			qDebug() << b;
		}
	}

	//拷贝用户手册到系统
	void DataManager::CopyUserManualToSystem()
	{
		QString destPath = FileHelper::GetSystemConfigPath() + "..//Doc//";
		//CopyFileToSystem(_userManual, destPath);
		CopyUserManualFileToSystem(_userManual, destPath);//20200306
	}

	//拷贝logo、welcome文件到系统
	void DataManager::CopyLogoAndWelcomImageToSystem()
	{
		QString destPath = FileHelper::GetSystemConfigPath() + "icon//";
		CopyFileToSystem(_logoFileName, destPath);
		CopyFileToSystem(_welcomeFileName, destPath);
	}

	//从系统读取配置信息到本地
	bool DataManager::ReadInfoFromServerToLocal()
	{
		ReadGlobalConfig();
		ReadGeometryConfig();
		ReadMeshConfig();
		ReadProjectTreeConfig();
		return true;
	}
	
	//读取基础配置信息
	bool DataManager::ReadGlobalConfig()
	{
		ConfigOption::GlobalConfig* globalConfig = ConfigOption::ConfigOption::getInstance()->getGlobalConfig();

		if (nullptr == _globalConfig)
		{
			_globalConfig = new ConfigOption::GlobalConfig();
		}

		_globalConfig->clearData();
		_globalConfig->setSoftName(globalConfig->getSoftName());
		_globalConfig->setChineseName(globalConfig->getChineseName());
		_globalConfig->setCorporation(globalConfig->getCorporation());
		_globalConfig->setEMail(globalConfig->getEMail());
		_globalConfig->setLogo(globalConfig->getLogo());
		_globalConfig->setVersion(globalConfig->getVersion());
		_globalConfig->setWebsite(globalConfig->getWebsite());
		_globalConfig->setWelcome(globalConfig->getWelcome());
		_globalConfig->enableMaterial(globalConfig->isMaterialEnabled());
		_globalConfig->SetUserManual(globalConfig->GetUserManual());

		//加载logo、welcom文件名称
		QString destPath = FileHelper::GetSystemConfigPath() + "icon//";
		QString logoFile = _globalConfig->getLogo();
		
		if (!logoFile.isEmpty())
		{
			_logoFileName = destPath + logoFile;
			setIconNameList(logoFile);
		}
// 		if (!logoFile.isEmpty())
// 			_logoFileName = destPath + logoFile;
		
		QString welcomeFile = _globalConfig->getWelcome();

		if (!welcomeFile.isEmpty())
		{
			_welcomeFileName = destPath + welcomeFile;
			setIconNameList(welcomeFile);
		}
			

// 		if (!welcomeFile.isEmpty())
// 			_welcomeFileName = destPath + welcomeFile;

// 		destPath = FileHelper::GetSystemConfigPath() + "doc//";
// 		QString userManualFile = _globalConfig->GetUserManual();
// 
// 		if (!userManualFile.isEmpty())
// 			_userManual = destPath + userManualFile;
		

		destPath = FileHelper::GetSystemConfigPath();
		QDir desDir(destPath);
		desDir.cdUp();
		QString manualPath = desDir.path() + "/Doc/";
		QString userManualFile = _globalConfig->GetUserManual();

		if (!userManualFile.isEmpty())
			_userManual = manualPath + userManualFile;
		
		return true;
	}
	//读取geometry相关信息
	bool DataManager::ReadGeometryConfig()
	{
		ConfigOption::GeometryConfig* geoConfig = ConfigOption::ConfigOption::getInstance()->getGeometryConfig();

		if (nullptr == _geoConfig)
		{
			_geoConfig = new ConfigOption::GeometryConfig();
		}

		_geoConfig->clearData();
// 		_geoConfig->enableGeometry(geoConfig->isGeometryEnabled());
// 		_geoConfig->enableImportGeometry(geoConfig->isImportGeometryEnabled());
// 		_geoConfig->enableCreateGeometry(geoConfig->isCreateGeometryEnabled());
		//_geoConfig->setSuffix(geomConfig->getSuffix());
		_isGeoModeling = geoConfig->isGeometryModelingEnabled();
		_geoConfig->enableGeometryModeling(geoConfig->isGeometryModelingEnabled());
		_isGeoOperations = geoConfig->isGeometryOperationsEnabled();
		_geoConfig->enableGeometryOperations(geoConfig->isGeometryOperationsEnabled());
		_isCreateSketch = geoConfig->isCreateSketchEnabled();
		_geoConfig->enableCreateSketch(geoConfig->isCreateSketchEnabled());
		_importGeoSuffix = geoConfig->getImportSuffix();
		_geoConfig->setImportSuffix(geoConfig->getImportSuffix());
		_exportGeoSuffix = geoConfig->getExportSuffix();
		_geoConfig->setExportSuffix(geoConfig->getExportSuffix());
		_isGeometryEdit = geoConfig->isGeometryEditEnabled();
		_geoConfig->enableGeometryEdit(geoConfig->isGeometryEditEnabled());
		_isGeoCreateSet = geoConfig->isGeometryCreateSetEnabled();
		_geoConfig->enableGeometryCreateSet(geoConfig->isGeometryCreateSetEnabled());
		_isMeasureDistance = geoConfig->isMeasureDistanceEnabled();
		_geoConfig->enableMeasureDsitance(geoConfig->isMeasureDistanceEnabled());

		return true;
	}
	//读取mesh相馆信息
	bool DataManager::ReadMeshConfig()
	{
		ConfigOption::MeshConfig* meshConfig = ConfigOption::ConfigOption::getInstance()->getMeshConfig();

		if (nullptr == _meshConfig)
		{
			_meshConfig = new ConfigOption::MeshConfig();
		}
		_meshConfig->clearData();
		//_meshConfig->enableImportMesh(meshConfig->isImportMeshEnabled);
		//_meshConfig->enableMesh(meshConfig->isMeshEnabled());
		_importMeshSuffix = meshConfig->getImportSuffix();
		_meshConfig->setImportSuffix(meshConfig->getImportSuffix());
		//_meshConfig->enableExportMesh(meshConfig->isExportMeshEnabled());
		_exportMeshSuffix = meshConfig->getExportSuffix();
		_meshConfig->setExportSuffix(meshConfig->getExportSuffix());
//		_meshConfig->enableMeshGeneration(meshConfig->isMeshGenerationEnabled());
		_isSurfaceMesh = meshConfig->isSurfaceMeshEnabled();
		_meshConfig->enableSurfaceMesh(meshConfig->isSurfaceMeshEnabled());
		_isSolidMesh = meshConfig->isSolidMeshEnabled();
		_meshConfig->enableSolidMesh(meshConfig->isSolidMeshEnabled());
		_isCreateSet = meshConfig->isComponentEnabled();
		_meshConfig->enableComponent(meshConfig->isComponentEnabled());
//		_meshConfig->enableGmsher(meshConfig->isGmsher());
		_isCheckMesh = meshConfig->getCheckMeshEnabled();
		_meshConfig->setCheckMesh(meshConfig->getCheckMeshEnabled());
		
		_isFluidMesh = meshConfig->isFluidMeshEnabled();
		_meshConfig->enableFluidMesh(meshConfig->isFluidMeshEnabled());

		_isFilterMesh = meshConfig->isFilterMeshEnabled();
		_meshConfig->enableFilterMesh(meshConfig->isFilterMeshEnabled());
		
		_isMeshModeling = meshConfig->isMeshModelingEnabled();
		_meshConfig->enableMeshModeling(meshConfig->isMeshModelingEnabled());

		return true;
	}

	bool DataManager::ReadProjectTreeConfig()
	{
		ConfigOption::ProjectTreeConfig* projectTreeConfig = ConfigOption::ConfigOption::getInstance()->getProjectTreeConfig();

		if (nullptr == _projectTreeConfig)
		{
			_projectTreeConfig = new ConfigOption::ProjectTreeConfig();
		}
		_projectTreeConfig->clearData();

		QList<ProjectTreeType> types = projectTreeConfig->getTreeTypes();
		for (ProjectTreeType t : types)
		{
			ConfigOption::ProjectTreeInfo* tree = projectTreeConfig->getProjectTree(t);
			ConfigOption::ProjectTreeInfo* newTree = new ConfigOption::ProjectTreeInfo;
			tree->copy(newTree);
			_projectTreeConfig->appendTree(t, newTree);
		}

		//_treeHashType.count();
		//projectTreeConfig->getProjectTree();


		return true;
	}

	//属性赋值、读取函数
	//返回基础配置数据指针
	void DataManager::SetGeometryFeatureModeling(bool on)
	{
		//qDebug() << on;
		//qDebug() << "DataManager::SetGeometryFeatureModeling(int state):" << state;
		if (nullptr == _geoConfig)
			return;
		//bool b = (state > 0) ? true : false;
		_geoConfig->enableGeometryModeling(on);
		//return true;
	}
	//几何参数设置
	void DataManager::SetGeometryFeatureOperatins(bool on)
	{
		if (nullptr == _geoConfig)
			return;
		_geoConfig->enableGeometryOperations(on);
	}
	void DataManager::SetGeometryCreateSketch(bool on)
	{
		if (nullptr == _geoConfig)
			return;
		_geoConfig->enableCreateSketch(on);
	}

	void DataManager::setGeometryEdit(bool on)
	{
		if (nullptr == _geoConfig)
			return;
		_geoConfig->enableGeometryEdit(on);
	}

	void DataManager::setGeometryCreateSet(bool on)
	{
		if (nullptr == _geoConfig)
			return;
		_geoConfig->enableGeometryCreateSet(on);
	}

	void DataManager::setMeasureDistance(bool on)
	{
		if (nullptr == _geoConfig)
			return;
		_geoConfig->enableMeasureDsitance(on);
	}

	void DataManager::SetImportGeometry(QString suffix)
	{
		if (nullptr == _geoConfig)
			return;
		_geoConfig->setImportSuffix(suffix);
		_importGeoSuffix = suffix;
	}
	void DataManager::SetExportGeometry(QString suffix)
	{
		if (nullptr == _geoConfig)
			return;
		_geoConfig->setExportSuffix(suffix);
		_exportGeoSuffix = suffix;
	}

	ConfigOption::MeshConfig* DataManager::getMeshConfig()
	{
		return _meshConfig;
	}
	
	ConfigOption::ProjectTreeConfig* DataManager::GetProjectTreeConfig()
	{
		return _projectTreeConfig;
	}

	//网格参数设置
	void DataManager::SetSurfaceMesh(bool on)
	{
		if (nullptr == _meshConfig)
			return;
		_meshConfig->enableSurfaceMesh(on);
	}
	void DataManager::SetSolidMesh(bool on)
	{
		if (nullptr == _meshConfig)
			return;
		_meshConfig->enableSolidMesh(on);
	}
	void DataManager::SetMeshCreateSet(bool on)
	{
		if (nullptr == _meshConfig)
			return;
		_meshConfig->enableComponent(on);
	}
	void DataManager::SetCheckMesh(bool on)
	{
		if (nullptr == _meshConfig)
			return;
		_meshConfig->setCheckMesh(on);
	}
	void DataManager::SetImportMesh(QString suffix)
	{
		if (nullptr == _meshConfig)
			return;
		_meshConfig->setImportSuffix(suffix);
		_importMeshSuffix = suffix;
	}
	void DataManager::SetExportMesh(QString suffix)
	{
		if (nullptr == _meshConfig)
			return;
		_meshConfig->setExportSuffix(suffix);
		_exportMeshSuffix = suffix;
	}


	void DataManager::setFluidMesh(bool on)
	{
		if (nullptr == _meshConfig)
			return;
		_meshConfig->enableFluidMesh(on);
	}

	void DataManager::setFilterMesh(bool on)
	{
		if (nullptr == _meshConfig)
			return;
		_meshConfig->enableFilterMesh(on);
	}

	void DataManager::setMeshModeling(bool on)
	{
		if (nullptr == _meshConfig)
			return;
		_meshConfig->enableMeshModeling(on);
	}

	ConfigOption::GlobalConfig* DataManager::getGlobalConfig()
	{
		return _globalConfig;
	}

	//返回logo文件名称
	QString DataManager::GetLogoFileName()
	{
		return _logoFileName;
	}

	//设置logo文件名称
	void DataManager::SetLogoFileName(QString fileName)
	{
		_logoFileName = fileName;
	}

	//返回welcome文件名称
	QString DataManager::GetWelcomeFileName()
	{
		return _welcomeFileName;
	}

	//设置welcome文件名称
	void DataManager::SetWelcomeFileName(QString fileName)
	{
		_welcomeFileName = fileName;
	}
	
	//获取用户手册文件名称
	QString DataManager::GetUserManual()
	{
		return _userManual;
	}

	//设置用户手册用户名称
	void DataManager::SetUserManual(QString userManual)
	{
		_userManual = userManual;
	}

	//属性赋值、读取函数


	//返回geometry和mesh相关信息
	bool DataManager::GetGeometryFeatureModeling()
	{
		return _isGeoModeling;
	}
	bool DataManager::GetGeometryFeatureOperatins()
	{
		return _isGeoOperations;
	}
	bool DataManager::GetGeometryCreateSketch()
	{
		return _isCreateSketch;
	}

	bool DataManager::getGeometryEdit()
	{
		return _isGeometryEdit;
	}

	bool DataManager::getGeometryCreateSet()
	{
		return _isGeoCreateSet;
	}

	bool DataManager::getMeasureDistance()
	{
		return _isMeasureDistance;
	}

	QString DataManager::GetImportGeometrySuffix()
	{
		return _importGeoSuffix;
	}
	QString DataManager::GetExportGeometrySuffix()
	{
		return _exportGeoSuffix;
	}
	bool DataManager::GetSurfaceMesh()
	{
		return _isSurfaceMesh;
	}
	bool DataManager::GetSolidMesh()
	{
		return _isSolidMesh;
	}
	bool DataManager::GetMeshCreateSet()
	{
		return _isCreateSet;
	}
	bool DataManager::GetCheckMesh()
	{
		return _isCheckMesh;
	}

	bool DataManager::getFluidMesh()
	{
		return  _isFluidMesh;
	}

	bool DataManager::getFilterMesh()
	{
		return _isFilterMesh;
	}

	bool DataManager::getMeshModeling()
	{
		return _isMeshModeling;
	}

	QString DataManager::GetImportMeshSuffix()
	{
		return _importMeshSuffix;
	}
	QString DataManager::GetExportMeshSuffix()
	{
		return _exportMeshSuffix;
	}

	void DataManager::SetPhysicsList(QList<ModelBase*> list)
	{
		_physicsList.clear();
		_physicsList = list;
	}

	void DataManager::SetMaterialList(QList<ModelBase*> list)
	{
		_materialList.clear();
		_materialList = list;
	}
	//202.1.18
	QList<ModelBase*> DataManager::getParameterList(TreeItemType type)
	{
		_parameterList = _parameterListDict[type];
		return _parameterList;
	}

	void DataManager::setParameterListDict(TreeItemType type, QList<ModelBase*> modelList)
	{
		_parameterListDict.insert(type, modelList);
	}

	//20200306
	void DataManager::CopyUserManualFileToSystem(QString fileName, QString path)
	{
		QDir desDir(path);
		if (!desDir.exists())
			desDir.mkpath(path);

		if (!fileName.isEmpty())
		{
			QString fileSuffix = fileName.split(".").last();
			QStringList desFileNameList{};

			desFileNameList.append(QString("UserManual"));
			desFileNameList.append(fileSuffix);

			QString desFileName = desFileNameList.join(".");

			bool b = FileHelper::CopyFileToPath(fileName, path + desFileName, true);
			qDebug() << b;
		}
	}

	//20200324
	void DataManager::setIconNameList(QString iconName)
	{
		if (iconName.isEmpty())
			return;
		_iconNameList.append(iconName);
	}

	bool DataManager::getIconNameIsAvailable(QString iconName)
	{
		for (int i = 0; i < _iconNameList.count(); i++)
		{
			if (_iconNameList.contains(iconName))
				return false;
		}

		return true;
	}

	void DataManager::clearIconNameList()
	{
		this->_iconNameList.clear();
	}

	void DataManager::removeIconNameFromList(QString iconName)
	{
		_iconNameList.removeOne(iconName);
	}

	//20200325 xuxinwei
	void DataManager::setAllParameterListDict(int type, QList<DataProperty::ParameterBase*> list)
	{
		_allParameterDict.insert(type, list);
	}

	QList<DataProperty::ParameterBase*> DataManager::getAllParameterList(int type)
	{
		return _allParameterDict[type];
	}

// 	void DataManager::setParametersLinkageList(QList<FastCAEDesigner::ParametersLinkage*> list)
// 	{
// 		_parametersLinkageList = list;
// 	}
// 
// 	QList<FastCAEDesigner::ParametersLinkage*> DataManager::getParametersLinkageList()
// 	{
// 		return _parametersLinkageList;
// 	}
	//20200325 xuxinwei


	void DataManager::setAllParameterGroupListDict(int type, QList<DataProperty::ParameterGroup*> list)
	{
		_allParameterGroupDict.insert(type, list);
	}

	QList<DataProperty::ParameterGroup*> DataManager::getAllParameterGroupList(int type)
	{
		return _allParameterGroupDict[type];
	}

	void DataManager::setTreeList(QString name)
	{
		_treeList.append(name);
	}

	QList<QString> DataManager::getTreeList()
	{
		return _treeList;
	}
}
