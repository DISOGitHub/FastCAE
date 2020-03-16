/**********************************************************************************

功能：数据管理类，系统中需要设定的所有参数都由此类管理
建立：xvdongming
日期：2019-11
修改和维护：xvdongming

**********************************************************************************/


#ifndef DATAMANGER_H
#define DATAMANGER_H
#include <QObject>
#include <QMap>
#include <QTreeWidgetItem>

#include "DataProperty/modelTreeItemType.h"

namespace ConfigOption
{
	class BCConfig;
	class DataConfig;
	class GlobalConfig;
	class GeometryConfig;
	class MeshConfig;
	class PostConfig;
	class SolverOption;
	class ObserverConfig;
	class MaterialConfig;
	class ProjectTreeConfig;
	class ProjectTreeInfo;
	class TreeItem;
}

namespace FastCAEDesigner
{
	class ModelBase;
}

namespace FastCAEDesigner
{
	class DataManager : public QObject
	{
		Q_OBJECT
	private:
		DataManager(QObject *parent = 0);
		~DataManager();

	public:
		
		static DataManager* getInstance();
		bool ReadInfoFromServerToLocal();              //从系统读取配置信息到本地
		bool WriteInfoToServerPath();                  //写配置信息到系统
		void SetPhysicsList(QList<ModelBase*> list); 
		void SetMaterialList(QList<ModelBase*> list);

		ConfigOption::GlobalConfig* getGlobalConfig(); //获取基础配置信息 
		ConfigOption::GeometryConfig* getGeoConfig();
		ConfigOption::MeshConfig* getMeshConfig();
		ConfigOption::ProjectTreeConfig* GetProjectTreeConfig();
		//属性操作函数
		QString GetLogoFileName();                     //获取Logo文件名称
		void SetLogoFileName(QString fileName);        //设置Logo文件名称  
		QString GetWelcomeFileName();                  //获取welcome文件名称
		void SetWelcomeFileName(QString fileName);     //设置welcome文件名称
		QString GetUserManual();
		void SetUserManual(QString userManual);
		//属性操作函数

		void SetGeometryFeatureModeling(bool on);
		void SetGeometryFeatureOperatins(bool on);
		void SetGeometryCreateSketch(bool on);
		void SetImportGeometry(QString suffix);
		void SetExportGeometry(QString suffix);

		void SetSurfaceMesh(bool on);
		void SetSolidMesh(bool on);
		void SetMeshCreateSet(bool on);
		void SetCheckMesh(bool on);
		void SetImportMesh(QString suffix);
		void SetExportMesh(QString suffix);
		
		QString GetGeoSuffix();
		void SetGeoSuffix(QString suffix);

		//返回geometry和mesh相关信息
		bool GetGeometryFeatureModeling();
		bool GetGeometryFeatureOperatins();
		bool GetGeometryCreateSketch();
		bool GetSurfaceMesh();
		bool GetSolidMesh();
		bool GetMeshCreateSet();
		bool GetCheckMesh();
		QString GetImportGeometrySuffix();
		QString GetExportGeometrySuffix();
		QString GetImportMeshSuffix();
		QString GetExportMeshSuffix();

		QMap<QTreeWidgetItem*, ModelBase*> DictTreeItemToModel;		

		void ClearModelDict();
		ModelBase* GetModelFromDict(QTreeWidgetItem* treeItem);
		void InsertModelToDict(QTreeWidgetItem*, ModelBase*);
		void DeleteModelToDict(QTreeWidgetItem*);
		void DeleteModelToDict(QList<QTreeWidgetItem*> treeItemList);
		void CopyFileToSystem(QString fileName, QString path);
		//2020.1.18
		QList<ModelBase*> getParameterList(TreeItemType type);
		void setParameterListDict(TreeItemType type, QList<ModelBase*> modelList);

		//20200306
		void CopyUserManualFileToSystem(QString fileName, QString path);

	private:
		bool ReadGlobalConfig();                       //读取基础配置信息
		bool ReadGeometryConfig();

		void Init();                                   //初始化
		void CopyLogoAndWelcomImageToSystem();         //拷贝logo、welcome文件到系统配置目录下
		void CopyUserManualToSystem();
		void CopyFileToSystem(QString fileName, QString path, QString destFileName);

		bool ReadGeoConfig();		//读取几何信息
		bool ReadMeshConfig();		//读取网格信息
		bool ReadProjectTreeConfig();
		//bool ReadProjectTreeInfo();
		QList<ModelBase*> GetSpecifiedTypeModelList(QList<ModelBase*> modelList, TreeItemType type);

	private:
		static DataManager* _instance;
		ConfigOption::BCConfig* _bcConfig{};
		ConfigOption::DataConfig* _dataConfig{};
		ConfigOption::GlobalConfig* _globalConfig{};
		ConfigOption::GeometryConfig* _geoConfig{};
		ConfigOption::MeshConfig* _meshConfig{};
		ConfigOption::PostConfig* _postConfig{};
		ConfigOption::SolverOption* _solverOption{};
		ConfigOption::ObserverConfig* _observerConfig{};
		ConfigOption::MaterialConfig* _materialConfig{};
		ConfigOption::ProjectTreeConfig* _projectTreeConfig{};
		ConfigOption::ProjectTreeInfo* _projectTreeInformation{};

		bool _isGeoModeling{ false };	//Geometry modeling
		bool _isGeoOperations{ false };	//Geometry operations
		bool _isCreateSketch{ false };	//Create Sketch

		QString _logoFileName{};     //logo文件名称
		QString _welcomeFileName{};  //welcome文件名称
		QString _userManual{};       //用户手册
		QString _GeoSuffix;   

		QString _importGeoSuffix;	//import geometry suffix
		QString _exportGeoSuffix;	//export geometry suffix
		QString _importMeshSuffix;	//import mesh suffix
		QString _exportMeshSuffix;	//export mesh suffix

		bool _isSurfaceMesh{ false };	//surface mesh
		bool _isSolidMesh{ false };		//solid mesh
		bool _isCheckMesh{ false };		//check mesh
		bool _isCreateSet{ false };		//create set

		QList<ModelBase*> _physicsList;  //算例列表
		QList<ModelBase*> _materialList; //材料列表
		//2020.1.18
		QList<ModelBase*> _parameterList;
		QMap<TreeItemType, QList<ModelBase*>> _parameterListDict;
	};
}

#endif // DATAMANGER_H
