#ifndef _D2MESHTOOLSPLUGIN_H__
#define _D2MESHTOOLSPLUGIN_H__

#include "2DMeshToolsAPI.h"
#include "PluginManager/pluginBase.h"

namespace D2MeshTools
{
	class ActionManager;

	class D2MESHTOOLSAPI D2MeshToolsPlugin : public Plugins::PluginBase
	{
	public:
		D2MeshToolsPlugin(GUI::MainWindow* m);
		~D2MeshToolsPlugin() = default;

		//���ز��
		bool install() override;
		//ж�ز��
		bool uninstall() override;
		//����
		void reTranslate(QString lang) override;

	private:
		GUI::MainWindow* _mainwindow{};

		ActionManager*  _actions{};
	};
}


extern "C"
{
	void D2MESHTOOLSAPI Register(GUI::MainWindow* m, QList<Plugins::PluginBase*>* plugs);
}

#endif