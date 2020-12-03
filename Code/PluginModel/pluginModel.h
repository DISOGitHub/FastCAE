#ifndef _MODELPLUGIN_H_
#define _MODELPLUGIN_H_

#include "ModelPluginAPI.h"
#include "PluginManager/pluginBase.h"
#include "DataProperty/modelTreeItemType.h"
#include <vtkDataSet.h>
#include "meshData/meshSingleton.h"

class QTextStream;
namespace GUI
{
	class MainWindow;
}
namespace MainWidget
{
	class PreWindow;
}

namespace ModelData
{
	class ModelDataBase;
	class ModelDataBaseExtend;
}
namespace ProjectTree
{
	class ProjectTreeBase;
}

namespace Material
{
	class Material;
}

namespace Geometry
{
	class GeoComponent;
}

namespace BCBase
{
	class BCDisplacement;
}

namespace Plugins
{
	class MODELPLUGINAPI ModelPlugin : public PluginBase
	{
		Q_OBJECT
	public:
		ModelPlugin(GUI::MainWindow* m);
		~ModelPlugin();

		//加载插件
		bool install() override;
		//卸载插件
		bool uninstall() override;
		//获取mainwindow
		static GUI::MainWindow* getMWpt();

	private:
		static GUI::MainWindow* _mainwindow;		
	};
}

extern "C"
{
	void MODELPLUGINAPI Register(GUI::MainWindow* m, QList<Plugins::PluginBase*>* plugs);

// 	bool MODELPLUGINAPI CreateModel(int t, QPair<int,ModelData::ModelDataBase*>*);
// 
// 	bool MODELPLUGINAPI CreateTree(int, GUI::MainWindow* m, QPair<int, ProjectTree::ProjectTreeBase*>*);
// 
// 	//求解器文件写出title, workingDir, filter, filter
 	bool MODELPLUGINAPI WriteOut(QString path, ModelData::ModelDataBase* d);
	bool MODELPLUGINAPI writeAMeshBCtoBCFile(BCBase::BCDisplacement*, MeshData::MeshSet*, QTextStream&);
	bool MODELPLUGINAPI writeAGeoBCtoBCFile(BCBase::BCDisplacement*, Geometry::GeoComponent*, QTextStream&);

	void MODELPLUGINAPI WriteOutBC(QDomDocument* doc, QDomElement* parent, QString& path, ModelData::ModelDataBaseExtend* model);
	void MODELPLUGINAPI WriteOutMeshSet(QDomDocument* doc, QDomElement* parent, QList<int>& msID, QString& path);
	void MODELPLUGINAPI WriteOutGeoComponent(QDomDocument* doc, QDomElement* parent, QList<int>& gcIDs, QString& path);
	void MODELPLUGINAPI GenerateMesh(QDomDocument* doc, QDomElement* parent, QMultiHash<int, int>& Surface, QMultiHash<int, int>& Body, QString& path);

// 	//数据转换
// 	bool MODELPLUGINAPI transfer(QString);
	//网格导入方法  
	bool MODELPLUGINAPI importMeshFun(QString fileName);
// 	//导出网格的方法
// 	bool MODELPLUGINAPI exportMeshFun(QString, int);
// 	//创建材料
// 	Material::Material* createMaterial(QString);
}


#endif