#include "mainWindow/mainWindow.h"
#include "MainWidgets/ControlPanel.h"
#include <QMainWindow>
#include <QMenu>
#include <QFile>
#include <QTimer>
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/PostConfig.h"
#include "ConfigOptions/MeshConfig.h"
#include "ConfigOptions/GeometryConfig.h"
#include "AircraftPlugin.h"
#include "IO/IOConfig.h"
#include "AircraftControlPanel.h"

void Register(GUI::MainWindow* m, QList<Plugins::PluginBase*>* plugs)
{
	auto p = new Aircraft::AircraftPlugin(m);
	plugs->append(p);
}

namespace Aircraft
{
	GUI::MainWindow* AircraftPlugin::_mainWindow = nullptr;

	AircraftPlugin::AircraftPlugin(GUI::MainWindow* m)
	{
		_mainWindow = m;
	}

	AircraftPlugin::~AircraftPlugin()
	{

	}

	GUI::MainWindow* AircraftPlugin::getMainWindow()
	{
		return _mainWindow;
	}


	
	bool AircraftPlugin::install()
	{
		_FastCAEControlPanel = _mainWindow->getControlPanel();
		MainWidget::ControlPanel* c = dynamic_cast<MainWidget::ControlPanel*>(_FastCAEControlPanel);
		if (c == nullptr) return false;

		_gent4ControlPanel = new Aircraft::AircraftControlPanel(_mainWindow);
		c->addTabWidgetPlugin(_gent4ControlPanel, "Aircraft");
		return true;
	}

	bool AircraftPlugin::uninstall()
	{
		return true;
	}

	void AircraftPlugin::reTranslate(QString lang)
	{

	}

	void AircraftPlugin::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{

	}

	void AircraftPlugin::readFromProjectFile(QDomElement* parent)
	{

	}




}
