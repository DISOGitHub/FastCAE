#ifndef ETSPLUGIN_H_
#define ETSPLUGIN_H_

#include "PluginManager/pluginBase.h"
#include "ETSPluginAPI.h"

namespace GUI
{
	class MainWindow;
}

class QWidget;

namespace ETS
{
	class ETSControlPanel;

	class ETSPlugin : public Plugins::PluginBase
	{
		Q_OBJECT
	public:
		ETSPlugin(GUI::MainWindow*  m);
		~ETSPlugin();

		//¼ÓÔØ²å¼þ
		virtual bool install() override;
		//Ð¶ÔØ²å¼þ
		virtual bool uninstall() override;
		//·­Òë
		virtual void reTranslate(QString lang) override;

	private:
		GUI::MainWindow* _mainWindow{};

		QWidget* _FastCAEControlPanel{};
		ETSControlPanel* _etsControlPanel{};
	};
}

extern "C"
{
	void ETSPLUGINAPI Register(GUI::MainWindow* m, QList<Plugins::PluginBase*>* plugs);
}

#endif
