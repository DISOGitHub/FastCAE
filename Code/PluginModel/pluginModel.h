#ifndef _MODELPLUGIN_H_
#define _MODELPLUGIN_H_

#include "ModelPluginAPI.h"
#include "PluginManager/pluginBase.h"
#include "DataProperty/modelTreeItemType.h"

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
}
namespace ProjectTree
{
	class ProjectTreeBase;
}

namespace Material
{
	class Material;
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

	

	private:
		GUI::MainWindow* _mainwindow{};
		
	};
}

extern "C"
{
	void MODELPLUGINAPI Register(GUI::MainWindow* m, QList<Plugins::PluginBase*>* plugs);

	bool MODELPLUGINAPI CreateModel(int t, QPair<int,ModelData::ModelDataBase*>*);

	bool MODELPLUGINAPI CreateTree(int, GUI::MainWindow* m, QPair<int, ProjectTree::ProjectTreeBase*>*);

	//求解器文件写出
	bool MODELPLUGINAPI WriteOut(QString path, ModelData::ModelDataBase* d);
	//数据转换
	bool MODELPLUGINAPI transfer(QString);
	//网格导入方法  
	bool MODELPLUGINAPI importMeshFun(QString filename);
	//导出网格的方法
	bool MODELPLUGINAPI exportMeshFun(QString, int);
	//创建材料
	Material::Material*  createMaterial(QString);

}


#endif