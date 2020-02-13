#ifndef PLUGINMANAGER_H_
#define PLUGINMANAGER_H_

#include "PluginManagerAPI.h"
#include "DataProperty/DataBase.h"
#include "pluginBase.h"
#include <QList>

namespace GUI
{
	class MainWindow;
}

namespace Plugins
{
	class PluginBase;

	class PLUGINMANAGERAPI PluginManager : public DataProperty::DataBase
	{
		Q_OBJECT
	public:
		static PluginManager* getInstance();

		//加载插件
		void loadPlugs(GUI::MainWindow* m);
		//加载单个插件
		bool loadPlugin(QString name);
		//卸载插件
		bool releasePlugin(QString name);
		//卸载插件
		void releasePlugs();
		//翻译
		void reTranslate(QString lang);
		//获取插件
		PluginBase* getPluginByDescribe(QString des);
		//根据类型获取插件
		QList<PluginBase*> getPluginsByType(PluginType t);
	
	public slots:
		void manage();

	private:
		PluginManager() = default;
		~PluginManager() = default;

	private:
		static PluginManager* _instance;
		GUI::MainWindow* _mainWindow{};

		QList<PluginBase*> _pluginList{};

	};
}



#endif

