#include "ActionComplexManager.h"
#include "ComplexPlugin.h"
#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>

namespace Plugins
{


	ActionComplexManager::ActionComplexManager(QMainWindow* m, MachineryPlugin* p)
		:_mainwindow(m), _plugin(p)
	{
		QString des = _plugin->getDescribe();
		_menu = _mainwindow->menuBar()->addMenu(des);
		_toolBar = _mainwindow->addToolBar(des);

		QAction* ac = new QAction("gear", _mainwindow);
		this->addAction(ac);


	}

	ActionComplexManager::~ActionComplexManager()
	{

	}

	void ActionComplexManager::preWindowOpened(MainWidget::PreWindow* pw)
	{
		_preWindow = pw;
	}

	void ActionComplexManager::preWindowClosed()
	{
		_preWindow = nullptr;
	}

	void ActionComplexManager::addAction(QAction* ac)
	{
		_menu->addAction(ac);
		_toolBar->addAction(ac);
	}

	void ActionComplexManager::release()
	{
		_mainwindow->removeToolBar(_toolBar);
	}


}