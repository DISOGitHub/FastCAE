#include "2DMeshToolsPlugin.h"
#include "actionManager.h"
#include <QApplication>
#include <QTranslator>
#include <assert.h>

void Register(GUI::MainWindow* m, QList<Plugins::PluginBase*>* ps)
{
	Plugins::PluginBase* p = new D2MeshTools::D2MeshToolsPlugin(m);
	ps->append(p);
}


namespace D2MeshTools
{
	D2MeshToolsPlugin::D2MeshToolsPlugin(GUI::MainWindow* m)
	{
		_mainwindow = m;
		_describe = "2D Mesh Tools";
	}


	bool D2MeshToolsPlugin::install()
	{
		_actions = new ActionManager(_mainwindow);
		_actions->install();
		return true;
	}

	bool D2MeshToolsPlugin::uninstall()
	{
		_actions->unInstall();
		delete _actions;
		return true;
	}

	void D2MeshToolsPlugin::reTranslate(QString lang)
	{
		auto app = static_cast<QApplication*>(QCoreApplication::instance());
		app->removeTranslator(_translator);
		if (lang.toLower() == "chinese")
		{
			bool ok = _translator->load(":/2DMeshTools/2DMeshTools_zh_CN");
			assert(ok);
			app->installTranslator(_translator);
		}
		_actions->translate();
	}

}
