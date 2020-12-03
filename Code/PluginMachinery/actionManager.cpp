#include "actionManager.h"
#include "MachineryPlugin.h"
#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>

namespace Plugins
{


	ActionManager::ActionManager(QMainWindow* m, MachineryPlugin* p)
		:_mainwindow(m), _plugin(p)
	{
		QString des = _plugin->getDescribe();
		_menu = _mainwindow->menuBar()->addMenu(des);
		_toolBar = _mainwindow->addToolBar(des);

		QAction* ac = new QAction("gear", _mainwindow);
		this->addAction(ac);


	}

	ActionManager::~ActionManager()
	{

	}

	void ActionManager::preWindowOpened(MainWidget::PreWindow* pw)
	{
		_preWindow = pw;
	}

	void ActionManager::preWindowClosed()
	{
		_preWindow = nullptr;
	}

	void ActionManager::addAction(QAction* ac)
	{
		_menu->addAction(ac);
		_toolBar->addAction(ac);
	}

	void ActionManager::release()
	{
		_menu->clear();
		delete _menu;
		_menu = nullptr;
		_mainwindow->removeToolBar(_toolBar);
	}


}