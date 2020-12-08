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

		//获取加载的插件个数
		int getPluginsCount();
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
		//写出工程文件
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		//从工程文件读入
		void readDataFromProjectFile(QDomElement* e) override;
		//是否有数据需要保存到工程文件
		bool hasInfoToSave();

	signals:
		void updateActionStates();
	
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

