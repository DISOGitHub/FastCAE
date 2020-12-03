#include "ThermalFluidPlugin.h"
#include "ModelData/modelDataFactory.h"
#include "ModelData/modelDataSingleton.h"
#include "ModelData/modelDataBase.h"
#include "MainWidgets/ProjectTreeFactory.h"
#include "meshData/MeshFactory.h"
#include "TFCouplingData.h"
#include "TFCouplingTree.h"
#include "TFRevMeshSet.h"
#include <QApplication>
#include <QTranslator>
#include <assert.h>

void Register(GUI::MainWindow* m, QList<Plugins::PluginBase*>* ps)
{
	Plugins::PluginBase* p = new ThermalFluid::ThermalFluidPlugin(m);
	ps->append(p);
}


namespace ThermalFluid
{
	ThermalFluidPlugin::ThermalFluidPlugin(GUI::MainWindow* m)
	{
		_mainwindow = m;
		_describe = "Thermal-Fluid Coupling";
	}


	bool ThermalFluidPlugin::install()
	{
		ModelData::ModelDataFactory::registerType(TFCouplingType, "Thermal-Fluid", CreateModel);
		MainWidget::ProjectTreeFactory::registerType(TFCouplingType, CreateTree);
		MeshData::MeshFactory::registerFunction(int(TFREVMESHSETTYPE), CreateRevSet);
		return true;
	}

	bool ThermalFluidPlugin::uninstall()
	{
		ModelData::ModelDataFactory::removeType(TFCouplingType);
		MainWidget::ProjectTreeFactory::removeType(TFCouplingType);
		MeshData::MeshFactory::remove(int(TFREVMESHSETTYPE));
		return true;
	}

	void ThermalFluidPlugin::reTranslate(QString lang)
	{
		auto app = static_cast<QApplication*>(QCoreApplication::instance());
		app->removeTranslator(_translator);
		if (lang.toLower() == "chinese")
		{
			bool ok = _translator->load(":/TF/TFCoupling_zh_CN.qm");
			assert(ok);
			app->installTranslator(_translator);
		}
	}
}


bool CreateModel(int t, QPair<int, ModelData::ModelDataBase*>* p)
{
	if (t == ProjectTreeType::TFCouplingType)
	{
		auto m = new ThermalFluid::TFCouplingModelData((ProjectTreeType)t);
		p->first = t;
		p->second = m;
		return true;
	}
	return false;
}

bool CreateTree(int tp, GUI::MainWindow* m, QPair<int, ProjectTree::ProjectTreeBase*>* t)
{
	if (tp == TFCouplingType)
	{
		t->first = tp;
		t->second = new ThermalFluid::TFCouplingTree(m);
		return true;
	}
	return false;
}

MeshData::MeshSet* CreateRevSet(int type)
{
	if (type == (int)TFREVMESHSETTYPE)
		return new ThermalFluid::TFRevMeshSet;
}
