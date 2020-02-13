#include "pluginModel.h"
#include "mainWindow/mainWindow.h"
#include "model.h"
#include "tree.h"
#include <QString>
#include "ModelData/modelDataFactory.h"
#include "ModelData/modelDataSingleton.h"
#include "ModelData/modelDataBase.h"
#include "MainWidgets/ProjectTreeFactory.h"
#include "IO/IOConfig.h"
#include "Material/MaterialFactory.h"
#include "Material/Material.h"
#include <QDebug>

void Register(GUI::MainWindow* m, QList<Plugins::PluginBase*>* ps)
{
	Plugins::PluginBase* p = new Plugins::ModelPlugin(m);
	ps->append(p);
}

namespace Plugins
{
	ModelPlugin::ModelPlugin(GUI::MainWindow* m)
	{
		_mainwindow = m;
		_describe = "model Test plugin";
	}


	ModelPlugin::~ModelPlugin()
	{

	}

	bool ModelPlugin::install()
	{
		ModelData::ModelDataFactory::registerType(PluginDefType+1, "PluginType0001", CreateModel);
		MainWidget::ProjectTreeFactory::registerType(PluginDefType + 1, CreateTree);

		IO::IOConfigure::RegisterInputFile("out", WriteOut);
		IO::IOConfigure::RegisterOutputTransfer("trans", transfer);
		IO::IOConfigure::RegisterMeshImporter("iii", importMeshFun);
		IO::IOConfigure::RegisterMeshExporter("iii", exportMeshFun);

		Material::MaterialFactory::registerType("ma1", createMaterial);
		return true;
	}

	bool ModelPlugin::uninstall()
	{
		ModelData::ModelDataFactory::removeType(PluginDefType + 1);
		MainWidget::ProjectTreeFactory::removeType(PluginDefType + 1);

		IO::IOConfigure::RemoveInputFile("out");
		IO::IOConfigure::RemoveOutputTransfer("trans");
		IO::IOConfigure::RemoveMeshImporter("iii");
		IO::IOConfigure::RemoveMeshExporter("iii");

		Material::MaterialFactory::removeType("ma1");
		return true;
	}

}

bool CreateModel(int t, QPair<int, ModelData::ModelDataBase*>* p)
{
	if (t == ProjectTreeType::PluginDefType + 1)
	{
		auto m = new ModelData::ModelDataPlugin((ProjectTreeType)t);
		p->first = t;
		p->second = m;
		return true;
	}
	return false;
		

}

bool CreateTree(int tp, GUI::MainWindow* m, QPair<int, ProjectTree::ProjectTreeBase*>* t)
{
	if (tp == PluginDefType + 1)
	{
		t->first = tp;
		t->second = new ProjectTree::PluginTree(m);
		return true;
	}
	return false;
}

bool  WriteOut(QString path, ModelData::ModelDataBase* d)
{
	qDebug() << d->getName();
	qDebug() << "writeTest";
	return false;
}

bool transfer(QString path)
{
	qDebug() << path;
	qDebug() << "transfer";
	return true;
}

bool MODELPLUGINAPI importMeshFun(QString)
{
	qDebug() << "meshImport";
	return true;
}

bool MODELPLUGINAPI exportMeshFun(QString, int)
{
	qDebug() << "meshExport";
	return true;
}

Material::Material* createMaterial(QString t)
{
	if (t != "ma1") return nullptr;

	auto m = new Material::Material;
	
	auto p = m->appendParameter(DataProperty::Para_Double);
	p->setDescribe("ma Name");
	return m;
}
