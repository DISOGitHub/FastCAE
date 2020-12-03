#ifndef _COMPLEXPLUGIN_H_
#define _COMPLEXPLUGIN_H_

#include "ComplexPluginAPI.h"
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
	class ActionComplexManager;

	class COMPLEXPLUGINAPI ComplexPlugin : public PluginBase
	{
		Q_OBJECT
	public:
		ComplexPlugin(GUI::MainWindow* m);
		~ComplexPlugin();

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
		ActionComplexManager* _actions{};
	};
}



extern "C"
{
    void COMPLEXPLUGINAPI Register(GUI::MainWindow* m, QList<Plugins::PluginBase*>* plugs);
}


#endif