#ifndef _MACHINERYPLUGIN_H_
#define _MACHINERYPLUGIN_H_

#include "MachineryPluginAPI.h"
#include "PluginManager/pluginBase.h"


namespace MainWidget
{
	class PreWindow;
}

namespace Plugins
{
	class ActionManager;

	class MACHINERYPLUGINAPI MachineryPlugin : public PluginBase
	{
		Q_OBJECT
	public:
		MachineryPlugin(GUI::MainWindow* m);
		~MachineryPlugin();

		//¼ÓÔØ²å¼þ
		bool install() override;
		//Ð¶ÔØ²å¼þ
		bool uninstall() override;

	private slots:
		void preWindowOpened(MainWidget::PreWindow* w);
		void preWindowClosed();

	private:
		GUI::MainWindow* _mainwindow{};
		ActionManager* _actions{};
	};
}

extern "C"
{
	void MACHINERYPLUGINAPI Register(GUI::MainWindow* m, QList<Plugins::PluginBase*>* plugs);
}


#endif