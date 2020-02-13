#include "PluginManager.h"
#include "pluginBase.h"
#include "PluginManageDialog.h"
#include "settings/busAPI.h"
#include <QApplication>
#include <QFileInfoList>
#include <QDir>
#include <QDebug>

#ifdef Q_OS_WIN
#include <windows.h>
#include <WinBase.h>
#endif

#ifdef Q_OS_UNIX
#include <dlfcn.h>
#endif

namespace Plugins
{
	PluginManager* PluginManager::_instance = nullptr;

	PluginManager* PluginManager::getInstance()
	{
		if (_instance == nullptr)
			_instance = new PluginManager;
		return _instance;
	}

	void PluginManager::releasePlugs()
	{
		int nplug = _pluginList.size();
		for (int i = 0; i < nplug; ++i)
		{
			Plugins::PluginBase* p = _pluginList.at(i);
			bool ok = p->uninstall();
			if (!ok) continue;
#ifdef _DEBUG
			delete p;
#endif
#ifdef Q_OS_WIN
			HMODULE m = p->getWinModule();
#ifndef _DEBUG
			delete p;
#endif			
			FreeLibrary(m);
			
#endif
#ifdef Q_OS_LINUX
			void* m = p->getLinuxModule();
//			delete p;
			dlclose(m);
#endif
//			delete p;
		}
		_pluginList.clear();

	}

	void PluginManager::loadPlugs(GUI::MainWindow *m)
	{
		_mainWindow = m;
		QStringList plugins = Setting::BusAPI::instance()->getPlugins();
		const QString plugdir = QApplication::applicationDirPath() + "/plugins/";
		QDir dir(plugdir);
		if (!dir.exists())
		{
			plugins.clear();
			Setting::BusAPI::instance()->setPlugins(plugins);
			return;
		}

		for (int i = 0; i < plugins.size(); ++i)
		{
			QString pluginname = plugins.at(i);
			bool ok = loadPlugin(pluginname);
			if (!ok)
				plugins.removeOne(pluginname);
			
		}
		Setting::BusAPI::instance()->setPlugins(plugins);
	}


	bool PluginManager::loadPlugin(QString pluginname)
	{
		QString lang = Setting::BusAPI::instance()->getLanguage();
		const QString plugdir = QApplication::applicationDirPath() + "/plugins/";
		QString plugpath = plugdir + pluginname;

		qDebug() << pluginname;

		typedef void(*Reg)(GUI::MainWindow*, QList<Plugins::PluginBase*>*);
		Reg fun = nullptr;
		

#ifdef Q_OS_WIN
		if (!pluginname.toLower().startsWith("plugin")) return false;
		if (!pluginname.toLower().endsWith(".dll")) return false;

		HMODULE hmodel = LoadLibrary(LPCWSTR(plugpath.utf16()));
		if (hmodel)
		{
			fun = (Reg)GetProcAddress(hmodel, "Register");
			if (fun)
			{
				fun(_mainWindow, &_pluginList);
				Plugins::PluginBase* pls = _pluginList.last();
				qDebug() << "Plugin: " << pls->getDescribe();
				pls->install();
				pls->setFileName(pluginname);
				pls->setWinModule(hmodel);
				pls->reTranslate(lang);
			}
			else
			{
				FreeLibrary(hmodel);
				return false;
			}

		}
#endif

#ifdef Q_OS_LINUX
		if (!pluginname.toLower().startsWith("libplugin")) return false;
		if (!pluginname.toLower().endsWith(".so")) return false;
		void* pHandle = dlopen(plugpath.toLatin1().data(), RTLD_NOW);
		if (!pHandle) return false;
		fun = (Reg)dlsym(pHandle, "Register");
		if (fun)
		{
			fun(_mainWindow, &_pluginList);
			Plugins::PluginBase* pls = _pluginList.last();
			qDebug() << "Plugin: " << pls->getDescribe();
			pls->install();
			pls->setFileName(pluginname);
			pls->setLinuxModule(pHandle);
			pls->reTranslate(lang);
		}
		else
		{
			//plugins.removeOne(pluginname);
			dlclose(pHandle);
			return false;
		}
#endif
		return true;
	}

	void PluginManager::reTranslate(QString lang)
	{
		const int n = _pluginList.size();
		for (int i = 0; i < n; ++i)
		{
			auto p = _pluginList.at(i);
			p->reTranslate(lang);
		}
			
	}

	PluginBase* PluginManager::getPluginByDescribe(QString des)
	{
		for(auto p : _pluginList)
		{
			if (des.toLower() == p->getDescribe().toLower())
				return p;
		}
		return nullptr;
	}

	void PluginManager::manage()
	{
		PluginManageDialog dlg(_mainWindow, this);
		dlg.exec();
	}

	bool PluginManager::releasePlugin(QString name)
	{
		for(auto p : _pluginList)
		{
			if (name == p->getFileName())
			{
				bool ok = p->uninstall();
				if (!ok) return false;
#ifdef _DEBUG
				delete p;
#endif
#ifdef Q_OS_WIN
				HMODULE m = p->getWinModule();
#ifndef _DEBUG
				delete p;
#endif			
				FreeLibrary(m);

#endif
#ifdef Q_OS_LINUX
				void* m = p->getLinuxModule();
				//			delete p;
				dlclose(m);
#endif
				_pluginList.removeOne(p);
				break;
			}
		}
		return true;
	}

	QList<PluginBase*> PluginManager::getPluginsByType(PluginType t)
	{
		QList<PluginBase*> ps;

		for (PluginBase* p : _pluginList)
		{
			if (p->getType() == t)
				ps.append(p);
		}

		return ps;
	}




}