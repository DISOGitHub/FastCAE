#ifndef HULLCUTPLUGIN_H_
#define HULLCUTPLUGIN_H_

#include "HullPluginAPI.h"
#include "PluginManager/pluginBase.h"

namespace GUI
{
	class MainWindow;
}
namespace MainWidget
{
	class PreWindow;
}

namespace Plugins
{
	class ActionHullManager;

	class HULLPLUGINAPI HullCutPlugin : public PluginBase
	{
		Q_OBJECT
	public:
		HullCutPlugin(GUI::MainWindow* m);
		~HullCutPlugin();

		//¼ÓÔØ²å¼þ
		bool install() override;
		//Ð¶ÔØ²å¼þ
		bool uninstall() override;
		//·­Òë
		void reTranslate(QString lang) override;
		

	private slots:
		void preWindowOpened(MainWidget::PreWindow* w);
		void preWindowClosed();

	private:
		GUI::MainWindow* _mainwindow{};
		ActionHullManager* _actions{};
	};
}



extern "C"
{
	void HULLPLUGINAPI Register(GUI::MainWindow* m, QList<Plugins::PluginBase*>* plugs);
}


#endif