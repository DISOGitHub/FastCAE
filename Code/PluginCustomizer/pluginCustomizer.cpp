#include "pluginCustomizer.h"
#include "customizerActionManager.h"
#include <QSettings>
#include <QCoreApplication>
#include <QApplication>
#include <QTranslator>
#include <assert.h>
#include <QDebug>

void  Register(GUI::MainWindow* m, QList<Plugins::PluginBase*>* plugs)
{
	Plugins::PluginBase* p = new Plugins::CustomizerPlugin(m);
	plugs->append(p);
}

namespace Plugins
{
	CustomizerPlugin::CustomizerPlugin(GUI::MainWindow* m)
		:_mainWindow(m)
	{
		_actionManager = new CustomActionManager(this, m);
		_describe = QString(tr("Customizer"));
	}

	CustomizerPlugin::~CustomizerPlugin()
	{
		if (_actionManager != nullptr) delete _actionManager;
	}

	bool CustomizerPlugin::install()
	{
		if (_actionManager != nullptr)
			_actionManager->init();
		else return false;

		PluginBase::install();

		return true;
	}

	bool CustomizerPlugin::uninstall()
	{
		if (_isUsing) return false;
		_actionManager->release();

		PluginBase::uninstall();

		return true;
	}

	void CustomizerPlugin::isUsing(bool u)
	{
		_isUsing = u;
	}

	void CustomizerPlugin::readINI(QSettings* settings)
	{
	}

	void CustomizerPlugin::writeINI(QSettings* settings)
	{
	}

	void CustomizerPlugin::reTranslate(QString lang)
	{
		auto app = static_cast<QApplication*>(QCoreApplication::instance());
		app->removeTranslator(_translator);
		if (lang.toLower() == "chinese")
		{
			bool ok = _translator->load("://translation/transtrations_zh_CN");
			assert(ok);
			app->installTranslator(_translator);
		}
		_actionManager->reTranslate();
	}

}


