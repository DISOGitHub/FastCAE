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

		//���ز��
		bool install() override;
		//ж�ز��
		bool uninstall() override;
		//����
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