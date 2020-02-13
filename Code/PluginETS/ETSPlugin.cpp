#include "ETSPlugin.h"
#include "ETSControlPanel.h"
#include "mainWindow/mainWindow.h"
#include "MainWidgets/ControlPanel.h"

void Register(GUI::MainWindow* m, QList<Plugins::PluginBase*>* plugs)
{
	auto p = new ETS::ETSPlugin(m);
	plugs->append(p);
}

namespace ETS
{

	ETSPlugin::ETSPlugin(GUI::MainWindow* m) :_mainWindow(m)
	{
	}

	ETSPlugin::~ETSPlugin()
	{

	}

	bool ETSPlugin::install()
	{
		_FastCAEControlPanel = _mainWindow->getControlPanel();
		_FastCAEControlPanel->setVisible(false);
		_etsControlPanel = new ETSControlPanel(_mainWindow);
		_mainWindow->addDockWidget(Qt::LeftDockWidgetArea, _etsControlPanel);
		return true;
	}

	bool ETSPlugin::uninstall()
	{
		_mainWindow->removeDockWidget(_etsControlPanel);
		delete _etsControlPanel;
		_etsControlPanel = nullptr;

		_FastCAEControlPanel->setVisible(true);

		return true;
	}

	void ETSPlugin::reTranslate(QString lang)
	{

	}

}