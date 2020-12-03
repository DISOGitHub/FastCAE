#include "actionHullCutManager.h"
#include "HullCutPlugin.h"
#include "../mainWindow/mainWindow.h"
#include "mainWindow/SubWindowManager.h"
#include "mainWindow/MainWindowPy.h"
#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QDialog>
#include <QApplication>
#include <QDebug>
#include <QTreeWidget>
#include "dialogShipTree.h"
#include "mainWindow/mainWindow.h"

namespace Plugins
{
	ActionHullManager::ActionHullManager(GUI::MainWindow* m, HullCutPlugin* p)
		:_mainwindow(m), _plugin(p)
	{
		QString des = _plugin->getDescribe();
		_menu = _mainwindow->menuBar()->addMenu(des);
		_toolBar = _mainwindow->addToolBar(des);


		QAction* boxac = new QAction(tr("Hull"), _mainwindow);
		//boxac->setIcon(QIcon("://icons/futi1.png"));
		this->addAction(boxac);
		_actionList.append(boxac);

		connect(boxac, SIGNAL(triggered()), this, SLOT(CreateHull()));
	}


	ActionHullManager::~ActionHullManager()
	{
		//_actionList.back()->setEnabled(true);
	}
	
	void ActionHullManager::preWindowOpened(MainWidget::PreWindow* pw)
	{
		_preWindow = pw; 
	}

	void ActionHullManager::preWindowClosed()
	{
		_preWindow = nullptr;
	}

	void ActionHullManager::CreateHull()
	{
		// _mainWindow->getUi()
		QAction* act = _actionList.back();
		if (!act->isEnabled()) return;
		
		
		PluginShip::ShipTreeDock*hulldock = new PluginShip::ShipTreeDock(_mainwindow,_actionList.back());

		hulldock->setObjectName("dockWidget test");
		hulldock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);  //锚接部件 允许停靠的区域，左右
		_mainwindow->addDockWidget(Qt::RightDockWidgetArea, hulldock);
		act->setEnabled(false);
	}

	void ActionHullManager::addAction(QAction* ac)
	{
		_menu->addAction(ac);
		_toolBar->addAction(ac);
	}

	
	

	void ActionHullManager::release()
	{
		_menu->clear();
		delete _menu;
		_menu = nullptr;
		_mainwindow->removeToolBar(_toolBar);
	}



	void ActionHullManager::reTranslate()
	{
		_menu->setTitle(tr("ShipPlugin"));
		_actionList.at(0)->setText(tr("Hull"));
	}

}
