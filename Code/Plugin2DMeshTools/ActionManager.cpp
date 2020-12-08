#include "ActionManager.h"
#include "mainWindow/mainWindow.h"
#include <QMenuBar>
#include <QToolBar>
#include <QMenu>
#include <QAction>

namespace D2MeshTools
{
	ActionManager::ActionManager(GUI::MainWindow* mw)
		:_mainWindow(mw)
	{}

	void ActionManager::install()
	{
		_menu = _mainWindow->menuBar()->addMenu(QString(tr("2D Mesh Tools")));
		_toolBar = _mainWindow->addToolBar(QString(tr("2D Mesh Tools")));

		QAction* normal = new QAction(tr("Normal"), this);
		this->addAction(normal);
	}

	void ActionManager::unInstall()
	{
		delete _menu;
		_menu = nullptr;
		_mainWindow->removeToolBar(_toolBar);
	}

	void ActionManager::translate()
	{
		_menu->setTitle(QString(tr("2D Mesh Tools")));
		_toolBar->setWindowTitle(QString(tr("2D Mesh Tools")));

		_actions[0]->setText(tr("Normal"));
	}

	void ActionManager::addAction(QAction* ac)
	{
		_menu->addAction(ac);
		_toolBar->addAction(ac);
		_actions.append(ac);
	}

}



