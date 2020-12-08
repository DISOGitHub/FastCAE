#include "SubWindowManager.h"
#include "ui_mainWindow.h"
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QList>
#include <QToolBar>
#include "MainWidgets/preWindow.h"
#include "MainWidgets/ControlPanel.h"
#include "meshData/meshSingleton.h"
#include "geometry/geometryData.h"
#include "PostWidgets/PostWindowBase.h"
#include "PostWidgets/RealTimeWindowBase.h"
#include "moduleBase/graphWindowBase.h"
#include "XGenerateReport/ReportWindow.h"
#include "mainWindow.h"
#include "signalHandler.h"
#include "python/PyAgent.h"
#include "GeometryCommand/GeoCommandPy.h"
#include "StartPage.h"
#include <QDebug>
#include <assert.h>

namespace GUI
{
	SubWindowManager::SubWindowManager(MainWindow* mainwindow, QMdiArea* mdiArea, SignalHandler* hander,MainWidget::ControlPanel* cp)
		:_mainWindow(mainwindow), _mdiArea(mdiArea), _signalHander(hander), _controlPanel(cp)
	{
		connect(mainwindow, SIGNAL(closePreWindowSig()), this, SLOT(closePreWindow()));
		connect(mainwindow, SIGNAL(openPostWindowSig(Post::PostWindowBase*)), this, SLOT(openPostWindow(Post::PostWindowBase*)));
		connect(mainwindow, SIGNAL(openRealTimeWindowSig(Post::RealTimeWindowBase*,int)), this, SLOT(openRealTimeWindow(Post::RealTimeWindowBase*)));
		connect(mainwindow, SIGNAL(showPostWindowInfoSig(int,int)), this, SLOT(showPostWindowInfo(int,int)));
		connect(mainwindow, SIGNAL(closePostWindowSig(Post::PostWindowBase*)), this, SLOT(closePostWindow(Post::PostWindowBase*)));
		connect(_mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(subWindowActived(QMdiSubWindow*)));
		connect(mainwindow, SIGNAL(closeRealTimeWindowSig(Post::RealTimeWindowBase*)), this, SLOT(closeRealTimeWindow(Post::RealTimeWindowBase*)));
		connect(mainwindow, SIGNAL(openReportWindowSig(XReport::ReportWindow*)), this, SLOT(openReportWindow(XReport::ReportWindow*)));
		connect(mainwindow, SIGNAL(closeReportWindowSig(XReport::ReportWindow*)), this, SLOT(closeReportWindow(XReport::ReportWindow*)));
		connect(mainwindow, SIGNAL(saveImageSig(QString, int, Post::PostWindowBase*, int, int)), this, SLOT(saveImage(QString, int, Post::PostWindowBase*, int, int)));
		connect(mainwindow, SIGNAL(saveImage(int, int, QString)), this, SLOT(saveImage(int, int, QString)));

		connect(this, SIGNAL(setViewSig(int, QString, QString)), this, SLOT(setView(int, QString, QString)));
		connect(this, SIGNAL(setViewValueSig(int, QString, int, int, int, int, int, int, int, int,int)), this, SLOT(setViewValue(int, QString, int, int, int, int, int, int, int, int,int)));
		connect(this, SIGNAL(saveImageSig(int, int, int, QString, QString)), this, SLOT(saveImage(int, int, int, QString, QString)));
		connect(this, SIGNAL(openPreWindowSig()), this, SLOT(openPreWindow()));

//		openStartPage();
	}
	SubWindowManager::~SubWindowManager()
	{

	}
	void SubWindowManager::openPreWindow()
	{
		if (isPreWindowOpened())
		{
			Py::PythonAagent::getInstance()->unLock();
			return;
		}
		if (_preWindow.first == nullptr || _preWindow.second == nullptr)
		{
			_preWindow.second = new MainWidget::PreWindow(_mainWindow);
			_preWindow.first = _mdiArea->addSubWindow(_preWindow.second);
			_signalHander->updateActionsStates();
			Command::GeometryCommandPy::init(_mainWindow, _preWindow.second);
		}
//		else
		updatePreActors();
		_preWindow.first->showMaximized();
		_preWindow.first->setFocus();
		_mdiArea->setActiveSubWindow(_preWindow.first);
		_mainWindow->getUi()->actionPre_Window->setEnabled(false);
		emit _mainWindow->preWindowOpenedSig(_preWindow.second);
		Py::PythonAagent::getInstance()->unLock();
	}

	void SubWindowManager::openGeometryWindow()
	{
		//if (_geometryWindow.first == nullptr || _geometryWindow.second == nullptr)
		//{
		//	_geometryWindow.second = new GenerateGeometry::GenerateGeometryWidget(_mainWindow);
		//	_geometryWindow.first = _mdiArea->addSubWindow(_geometryWindow.second);
		//}
		//_preWindow.first->showMaximized();
		//_preWindow.first->setFocus();
	}


	void SubWindowManager::updatePreActors()
	{
		updatePreGeometryActor();
		updatePreMeshActor();
	}
	void SubWindowManager::updatePreMeshActor()
	{
		if (_preWindow.second != nullptr)
		{
			_preWindow.second->updateMeshActor();
		}
	}
	void SubWindowManager::updatePreGeometryActor()
	{
		if (_preWindow.second != nullptr)
		{
			_preWindow.second->updateGeometryActor();
		}
	}
	void SubWindowManager::closePreWindow()
	{
		_preWindow.first = nullptr;
		_preWindow.second = nullptr;
		_mainWindow->getUi()->actionPre_Window->setEnabled(true);

		emit _mainWindow->updateGeometryTreeSig();
		emit _mainWindow->updateMeshTreeSig();
		emit _mainWindow->updateSetTreeSig();
		emit _mainWindow->updatePhysicsTreeSignal();
		emit _mainWindow->updateProperty(nullptr);
		emit _mainWindow->updateActionStatesSig();

//		_signalHander->clearData();
	}
	void SubWindowManager::openPostWindow(Post::PostWindowBase* pw)
	{
		if (pw == nullptr)
		{
			assert(0);
			return;
		}
		QMdiSubWindow* sw = nullptr;
		QList<Post::PostWindowBase*> pwl = _postWindow.values();
		if (pwl.contains(pw))
		{
			sw = _postWindow.key(pw);
		}
		else
		{
			sw = _mdiArea->addSubWindow(pw);
			_mdiArea->cascadeSubWindows();
			_postWindow[sw] = pw;
		}
		if (sw != nullptr)
		{
			sw->activateWindow();
			sw->showMaximized();
			sw->setFocus();
		}
		Py::PythonAagent::getInstance()->unLock();
		emit _mainWindow->updateActionStatesSig();
	
	}
	

	void SubWindowManager::openRealTimeWindow(Post::RealTimeWindowBase* pw)
	{
		QMdiSubWindow* sw = nullptr;
		QList<Post::RealTimeWindowBase*> pwl = _realTimeWindow.values();
		if (pwl.contains(pw))
		{
			sw = _realTimeWindow.key(pw);
		}
		else
		{
			sw = _mdiArea->addSubWindow(pw);
			_realTimeWindow[sw] = pw;
		}
		if (sw != nullptr)
		{
			sw->showMaximized();
			sw->setFocus();
		}
		emit _mainWindow->updateActionStatesSig();
	}
	ModuleBase::GraphWindowBase* SubWindowManager::getCurrentWindow()
	{
		QMdiSubWindow * activeWindow = _mdiArea->activeSubWindow();
		if (activeWindow == nullptr) return nullptr;
		else if (activeWindow == _preWindow.first) return _preWindow.second;
		else if (_postWindow.contains(activeWindow)) return _postWindow[activeWindow];
		else return nullptr;
	}
	Post::PostWindowBase* SubWindowManager::getPostWindowByIDType(int id, int type)
	{
		QList<Post::PostWindowBase*> pwl = _postWindow.values();
		for (int i = 0; i < pwl.size(); ++i)
		{
			Post::PostWindowBase* w = pwl.at(i);
			if (w == nullptr) continue;
			int wid = w->getID();
			int wtype = (int)w->getPostWindowType();
			if (wid == id && wtype == type)
				return w;
		}
		return nullptr;
	}
	void SubWindowManager::removeToolBars()
	{
		const int n = _currentToolBars.size();
		for (int i = 0; i < n; ++i)
		{
			QToolBar* t = _currentToolBars.at(i);
			_mainWindow->removeToolBar(t);
		}
		_currentToolBars.clear();
	}
	void SubWindowManager::showPostWindowInfo(int id, int type)
	{
		Post::PostWindowBase* pwb = getPostWindowByIDType(id, type);
		if (pwb == nullptr) return;
		QWidget* ptree = pwb->getTreeWidget();
		QWidget* pprop = pwb->getPropWidget();
		QMdiSubWindow* subWin = _postWindow.key(pwb);
		if (subWin != nullptr)
		{
			subWin->showMaximized();
			subWin->setFocus();
		}
		_controlPanel->updatePostWidget(ptree, pprop);
		this->removeToolBars();
		_mainWindow->addToolBarBreak();
		QList<QToolBar*> toolBars = pwb->getToolBarList();
		for (int i = 0; i < toolBars.size(); ++i)
		{
			_mainWindow->addToolBar(toolBars.at(i));
			(toolBars.at(i))->show();
			_currentToolBars.append(toolBars.at(i));
		}
	}
	void SubWindowManager::closePostWindow(Post::PostWindowBase* w)
	{
		QMdiSubWindow* subw = _postWindow.key(w);
		if (subw == nullptr) return;
		removeToolBars();
		if (_controlPanel != nullptr)
			_controlPanel->updatePostWidget(nullptr, nullptr);
		emit _mainWindow->updateProperty(nullptr);
		_postWindow.remove(subw);
		QList<Post::PostWindowBase*> postsubwlist = _postWindow.values();
		if (_preWindow.first != nullptr)
			showPreWindow();
		else if (postsubwlist.size() > 0)
		{
			Post::PostWindowBase* w = postsubwlist.at(0);
			int id = w->getID();
			Post::PostWindowType type = w->getPostWindowType();
			showPostWindowInfo(id, type);
		}
		emit _mainWindow->updateActionStatesSig();
	}
	void SubWindowManager::showPreWindow()
	{
		removeToolBars();
		if (_controlPanel == nullptr) return;
		_controlPanel->updatePostWidget(nullptr, nullptr);
		if (_preWindow.first != nullptr)
		{
			_preWindow.first->showMaximized();
			_preWindow.first->setFocus();
		}
	}
	void SubWindowManager::showRealTimeWindow(QMdiSubWindow* w)
	{
		removeToolBars();
		_controlPanel->updatePostWidget(nullptr, nullptr);
		if (_realTimeWindow.contains(w))
		{
			w->showMaximized();
			w->setFocus();
		}
	}
	void SubWindowManager::subWindowActived(QMdiSubWindow* sw)
	{
		if (sw == nullptr) return;
		if (sw == _preWindow.first) showPreWindow();
		else if (_realTimeWindow.contains(sw)) showRealTimeWindow(sw);
		else if (_postWindow.contains(sw))
		{
			Post::PostWindowBase* pwb = _postWindow.value(sw);
			int id = pwb->getID();
			Post::PostWindowType type = pwb->getPostWindowType();
			showPostWindowInfo(id, type);
		}

	}
	void SubWindowManager::closeRealTimeWindow(Post::RealTimeWindowBase* w)
	{
		QMdiSubWindow* subw = _realTimeWindow.key(w);
		if (subw == nullptr) return;
		_realTimeWindow.remove(subw);
		if (_preWindow.first != nullptr)
			showPreWindow();
		emit _mainWindow->updateActionStatesSig();
	}
	void SubWindowManager::setView(QString view)
 	{
		/*qDebug() << view;
		ModuleBase::GraphWindowBase* gwb = getCurrentWindow();
		if (gwb == nullptr) return;
		gwb->setView(view);*/
		ModuleBase::GraphWindowBase* gwb = getCurrentWindow();
		if (gwb == nullptr) return;
		const int id = gwb->getID();
		QString win = gwb->getStringGraphWindowType();
		qDebug() << win;
		QString pycode = QString("MainWindow.setView(%1,\"%2\",\"%3\")").arg(id).arg(win).arg(view);
		qDebug() << pycode;
		Py::PythonAagent::getInstance()->submit(pycode);
		//gwb->setView(view);
	}
	
	MainWidget::PreWindow* SubWindowManager::getPreWindow()
	{
		return _preWindow.second;
	}
	void SubWindowManager::openReportWindow(XReport::ReportWindow* w)
	{
		QMdiSubWindow* sw = nullptr;
		QList<XReport::ReportWindow*> rws = _reportWindow.values();
		if (rws.contains(w))
		{
			sw = _reportWindow.key(w);
		}
		else
		{
			sw = _mdiArea->addSubWindow(w);
			_reportWindow[sw] = w;
		}
		if (sw != nullptr)
		{
			sw->showMaximized();
			sw->setFocus();
		}
		emit _mainWindow->updateActionStatesSig();
	}
	void SubWindowManager::closeReportWindow(XReport::ReportWindow* w)
	{
		QMdiSubWindow* s = _reportWindow.key(w);
		if (s != nullptr)
		{
			_reportWindow.remove(s);
		}
	}
	bool SubWindowManager::isPreWindowOpened()
	{
		if (_preWindow.first == nullptr || _preWindow.second == nullptr)
			return false;
		return true;
	}
	bool SubWindowManager::isPostWindowOpened()
	{
		if (_postWindow.size() > 0)
			return true;
		return false;
	}
	void SubWindowManager::saveImage(QString fileName, int winType, Post::PostWindowBase*winhandle, int w, int h)
	{
		if (winType == 0)
		{
			MainWidget::PreWindow* preW = _preWindow.second;
			if (preW == nullptr) return;
			preW->saveImage(fileName, w, h, false);
		}
		else if (winType == 1)
		{
			if (_postWindow.key(winhandle) == nullptr) return;
			winhandle->saveImage(fileName, w, h, false);
		}
	}

	void SubWindowManager::saveImage(int w, int h, QString f)
	{
		ModuleBase::GraphWindowBase* gw = this->getCurrentWindow();
		if (gw == nullptr)
		{
			_mainWindow->printMessage(ModuleBase::Error_Message, tr("No GraphWindow opened!"));
			return;
		}
		QString type = gw->getStringGraphWindowType();
		const int id = gw->getID();
		QString pycode = QString("MainWindow.saveImage(%1,%2,%3,\"%4\",\"%5\")").arg(w).arg(h).arg(id).arg(type).arg(f);
		qDebug() << pycode;
		Py::PythonAagent::getInstance()->submit(pycode);

	}

	void SubWindowManager::saveImage(int w, int h, int id, QString winType, QString file)
	{
		ModuleBase::GraphWindowBase* win = this->getWindowByTypeID(winType, id);
		if (win == nullptr) return;
		win->saveImage(file, w, h, false);
		Py::PythonAagent::getInstance()->unLock();
	}
	void SubWindowManager::setView(int id, QString winType, QString view)//liu
	{
		ModuleBase::GraphWindowBase* gwb = getWindowByTypeID(winType,id);
		if (gwb == nullptr) return;
		gwb->setView(view);
		Py::PythonAagent::getInstance()->unLock();
	}
	void SubWindowManager::setViewValue(int id, QString win, int x1, int x2, int x3, int y1, int y2, int y3, int z1, int z2, int z3)
	{
		ModuleBase::GraphWindowBase* gwb = getWindowByTypeID(win,id);
		if (gwb == nullptr) return;
		gwb->setViewValue(x1, x2, x3, y1, y2, y3, z1, z2, z3);
		Py::PythonAagent::getInstance()->unLock();
	}
	/*void SubWindowManager::setViewPreValue(int id, QString win, int x1, int x2, int x3, int y1, int y2, int y3, int z1, int z2, int z3)
	{
	ModuleBase::GraphWindowBase* gwb = getCurrentWindow();
	if (gwb == nullptr) return;
	gwb->setViewPreValue(x1, x2, x3, y1, y2, y3, z1, z2, z3);
	}*/
	void SubWindowManager::reTranslate()
	{
		StartPage* sp = _startPage.second;
		if (sp != nullptr) sp->reTranslate();

		MainWidget::PreWindow* prew = _preWindow.second;
		if (prew != nullptr) prew->reTranslate();

		int n = 0;
		QList<Post::PostWindowBase*> postWinList = _postWindow.values();
		n = postWinList.size();
		for (int i = 0; i < n; ++i)
		{
			Post::PostWindowBase* pw = postWinList.at(i);
			if (pw != nullptr) pw->reTranslate();
		}
		QList<Post::RealTimeWindowBase*> rwList = _realTimeWindow.values();
		n = rwList.size();
		for (int i = 0; i < n; ++i)
		{
			Post::RealTimeWindowBase* rw = rwList.at(i);
			if (rw != nullptr) rw->reTranslate();
		}
		QList< XReport::ReportWindow*> repWin = _reportWindow.values();
		n = repWin.size();
		for (int i = 0; i < n; ++i)
		{
			XReport::ReportWindow* w = repWin.at(i);
			w->reTranslate();
		}

	}

	void SubWindowManager::closeGeometryWindow()
	{
		_geometryWindow.first = nullptr;
		_geometryWindow.second = nullptr;

		_signalHander->clearData(false);
		emit _mainWindow->updateGeometryTreeSig();
		emit _mainWindow->updateMeshTreeSig();
		emit _mainWindow->updatePhysicsTreeSignal();
		emit _mainWindow->updateProperty(nullptr);
	}

	GenerateGeometry::GenerateGeometryWidget* SubWindowManager::getGeometryWindow()
	{
		if (!_geometryWindow.second)
		{
			openGeometryWindow();
		}
		return _geometryWindow.second;
	}

	void SubWindowManager::closeAllSubWindow()
	{
//		_mdiArea->setViewMode(QMdiArea::SubWindowView);
//		_mdiArea->setTabPosition(QTabWidget::North);
		QWidget* sp = _startPage.second;
		if (sp != nullptr)
		{
			_startPage.first->close();
			_mdiArea->removeSubWindow(sp);
			sp->close();
			delete sp;
		}

		MainWidget::PreWindow* pre = _preWindow.second;
		if (pre != nullptr)
		{
			_preWindow.first->close();
			_mdiArea->removeSubWindow(pre);
			pre->close();
			delete pre;
		}

		QList<Post::PostWindowBase*> postlist = _postWindow.values();
		for (int i = 0; i < postlist.size(); ++i)
		{
			Post::PostWindowBase* p = postlist.at(i);
			_postWindow.key(p)->close();
			_mdiArea->removeSubWindow(p);
			p->close();
			delete p;
		}

		QList<Post::RealTimeWindowBase*> realtimelist = _realTimeWindow.values();
		for (int i = 0; i < realtimelist.size(); ++i)
		{
			Post::RealTimeWindowBase* r = realtimelist.at(i);
			_realTimeWindow.key(r)->close();
			_mdiArea->removeSubWindow(r);
			r->close();
			delete r;
		}
		
		QList<XReport::ReportWindow*> reportlist = _reportWindow.values();
		for (int i = 0; i < reportlist.size(); ++i)
		{
			XReport::ReportWindow* r = reportlist.at(i);
			_reportWindow.key(r)->close();
			_mdiArea->removeSubWindow(r);
			r->close();
			delete r;
		}
		
	}

	ModuleBase::GraphWindowBase* SubWindowManager::getWindowByTypeID(QString type, int id)
	{
		ModuleBase::GraphWindowBase* g = nullptr;
		type = type.toLower();
		if (type == "prewindow")
		{
			g = _preWindow.second;
		}
		else if (type == "realtime")
		{
			QList<Post::RealTimeWindowBase*> wlist = _realTimeWindow.values();
			for (int i = 0; i < wlist.size(); ++i)
			{
				ModuleBase::GraphWindowBase* t = wlist.at(i);
				const int d = t->getID();
				if (id == d)
				{
					g = t;
					break;
				}
			}
		}
		else if (type == "post2d")
		{
			QList<Post::PostWindowBase*> wlist = _postWindow.values();
			for (int i = 0; i < wlist.size(); ++i)
			{
				ModuleBase::GraphWindowBase* t = wlist.at(i);
				const int d = t->getID();
				if (id == d && t->getGraphWindowType() == ModuleBase::Post2D)
				{
					g = t;
					break;
				}
			}
		}
		else if (type == "post3d")
		{
			QList<Post::PostWindowBase*> wlist = _postWindow.values();
 			for (int i = 0; i < wlist.size(); ++i)
			{
				ModuleBase::GraphWindowBase* t = wlist.at(i);
				const int d = t->getID();
				if (id == d && t->getGraphWindowType() == ModuleBase::Post3D)
				{
					g = t;
					break;
				}
			}
		}
		return g;
	}

	void SubWindowManager::openStartPage()
	{
		if (_startPage.first != nullptr || _startPage.second != nullptr) return;
		StartPage* startpage = new StartPage;
		QMdiSubWindow* sub = _mdiArea->addSubWindow(startpage);
		_startPage.first = sub;
		_startPage.second = startpage;
		sub->showMaximized();
		sub->setFocus();
		_mainWindow->getUi()->actionStart_Page->setEnabled(false);
		connect(startpage, SIGNAL(closePage()), this, SLOT(closeStartPage()));
	}

	void SubWindowManager::closeStartPage()
	{
		_startPage.first = nullptr;
		_startPage.second = nullptr;
		_mainWindow->getUi()->actionStart_Page->setEnabled(true);
	}

	void SubWindowManager::openUrl(QString web)
	{
		this->openStartPage();
		_startPage.second->load(web);
	}

	void SubWindowManager::setIcon(QString icon)
	{
		if (_startPage.first != nullptr)
			_startPage.first->setWindowIcon(QIcon(icon));
		if (_preWindow.first !=  nullptr)
			_preWindow.first->setWindowIcon(QIcon(icon));
		QList<QMdiSubWindow*> subw = _postWindow.keys() + _realTimeWindow.keys() + _reportWindow.keys();
		for (auto w : subw)
		{
			w->setWindowIcon(QIcon(icon));
		}
	}

}