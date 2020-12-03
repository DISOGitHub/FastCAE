#include "actionComplexManager.h"
#include "ComplexPlugin.h"
#include "../mainWindow/mainWindow.h"
#include "dialogCreateCylindricalComplex.h"
#include "dialogCreateBoxComplex.h"
#include "mainWindow/SubWindowManager.h"
#include "mainWindow/MainWindowPy.h"
#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QDialog>
#include <QApplication>
#include <QDebug>

namespace Plugins
{


	ActionComplexManager::ActionComplexManager(GUI::MainWindow* m, ComplexPlugin* p)
		:_mainwindow(m), _plugin(p)
	{
		QString des = _plugin->getDescribe();
		_menu = _mainwindow->menuBar()->addMenu(des);
		_toolBar = _mainwindow->addToolBar(des);

		QAction* cyac = new QAction(tr("Connecting Rod"), _mainwindow);
		cyac->setIcon(QIcon("://icons/liangan1.png"));
		this->addAction(cyac);
		_actionList.append(cyac);

		QAction* boxac = new QAction(tr("Floating Body"), _mainwindow);
		boxac->setIcon(QIcon("://icons/futi1.png"));
		this->addAction(boxac);
		_actionList.append(boxac);

		connect(cyac, SIGNAL(triggered()), this, SLOT(CreateCylindricalComplex()));
		connect(boxac, SIGNAL(triggered()), this, SLOT(CreateBoxComplex()));
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

	void ActionComplexManager::CreateCylindricalComplex()
	{
		GUI::SubWindowManager* sw = _mainwindow->getSubWindowManager();
		
		if (!sw->isPreWindowOpened())
			emit sw->openPreWindowSig();
		MainWidget::PreWindow* p = sw->getPreWindow();
		GeometryWidget::CreateCylindricalComplexDialog* dlg = new GeometryWidget::CreateCylindricalComplexDialog(_mainwindow, p);
		emit _mainwindow->showDialogSig(dlg);
	}

	void ActionComplexManager::CreateBoxComplex()
	{
		GUI::SubWindowManager* sw = _mainwindow->getSubWindowManager();

		if (!sw->isPreWindowOpened())
			emit sw->openPreWindowSig();
		MainWidget::PreWindow* p = sw->getPreWindow();
		GeometryWidget::CreateBoxComplexDialog* dlg = new GeometryWidget::CreateBoxComplexDialog(_mainwindow, p);
		emit _mainwindow->showDialogSig(dlg);
	}

	void ActionComplexManager::release()
	{
		_menu->clear();
		delete _menu;
		_menu = nullptr;
		_mainwindow->removeToolBar(_toolBar);
	}

	void ActionComplexManager::reTranslate()
	{
		_menu->setTitle(tr("Combinant"));
		_toolBar->setWindowTitle(tr("Combinant"));

		_actionList.at(0)->setText(tr("Connecting Rod"));
		_actionList.at(1)->setText(tr("Floating Body"));
	}

}
