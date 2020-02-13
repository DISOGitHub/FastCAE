#include "dockWidgetBase.h"
#include "mainWindow/mainWindow.h"
#include "XDockTitleBarWidget.h"

namespace ModuleBase
{
	DockWidgetBase::DockWidgetBase(GUI::MainWindow* mainwindow, ModuleType type) :
		CModuleBase(mainwindow, type)/*, QDockWidget(mainwindow)*/
	{
	}
	DockWidgetBase::~DockWidgetBase()
	{

	}
	void DockWidgetBase::repaintTitleBar()
	{
		QWidget* old = this->titleBarWidget();
		QString name = this->windowTitle();
		_titleWidget = new XDockTitleBarWidget(tr("%1").arg(name));
		this->setTitleBarWidget(_titleWidget);
		delete old;
	}
	void DockWidgetBase::reTranslate()
	{
		if (_titleWidget == nullptr) return;
		repaintTitleBar();
	}
}