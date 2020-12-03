#ifndef THERMALFLUIDPLUGIN_H__
#define THERMALFLUIDPLUGIN_H__

#include "ThermalFluidAPI.h"
#include "PluginManager/pluginBase.h"

namespace ModelData
{
	class ModelDataBase;
}
namespace ProjectTree
{
	class ProjectTreeBase;
}
namespace MeshData
{
	class MeshSet;
}


namespace ThermalFluid
{
	class THERMALFLUIDAPI ThermalFluidPlugin : public Plugins::PluginBase
	{
	public:
		ThermalFluidPlugin(GUI::MainWindow* m);
		~ThermalFluidPlugin() = default;

		//¼ÓÔØ²å¼þ
		bool install() override;
		//Ð¶ÔØ²å¼þ
		bool uninstall() override;
		//·­Òë
		void reTranslate(QString lang) override;



	private:
		GUI::MainWindow* _mainwindow{};

	};
}


extern "C"
{
	void THERMALFLUIDAPI Register(GUI::MainWindow* m, QList<Plugins::PluginBase*>* plugs);

	bool THERMALFLUIDAPI CreateModel(int t, QPair<int, ModelData::ModelDataBase*>*);

	bool THERMALFLUIDAPI CreateTree(int, GUI::MainWindow* m, QPair<int, ProjectTree::ProjectTreeBase*>*);

	MeshData::MeshSet* CreateRevSet(int type);
}

#endif