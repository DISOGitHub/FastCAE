#include "PostWindowBase.h"
#include "moduleBase/ModuleType.h"
#include "mainWindow/mainWindow.h"
#include <QCloseEvent>

namespace Post
{
	PostWindowBase::PostWindowBase(GUI::MainWindow* mainwindow, int proID, ModuleBase::GraphWindowType gt, PostWindowType t)
		:GraphWindowBase(mainwindow, proID, gt), _windowType(t)
	{
		connect(this, SIGNAL(closeWindowSignal(Post::PostWindowBase*)), mainwindow, SIGNAL(closePostWindowSig(Post::PostWindowBase*)));
	}
	PostWindowBase::~PostWindowBase()
	{
		removeAllGraphWindow();
	}
	void PostWindowBase::setWindowType(PostWindowType type)
	{
		_windowType = type;
	}
	PostWindowType PostWindowBase::getPostWindowType()
	{
		return _windowType;
	}
	void PostWindowBase::setTreeWidget(QWidget* w)
	{
		_treeWidget = w;
	}
	void PostWindowBase::setPropWidget(QWidget* w)
	{
		_propWidget = w;
	}

	QWidget* PostWindowBase::getPropWidget()
	{
		return _propWidget;
	}
	QWidget* PostWindowBase::getTreeWidget()
	{
		return _treeWidget;
	}
	void PostWindowBase::appendToolBar(QToolBar* toolbar)
	{
		_toolBarList.append(toolbar);
	}
	void PostWindowBase::removeToolBar(QToolBar* toolbar)
	{
		if (_toolBarList.contains(toolbar))
		{
			int index = _toolBarList.indexOf(toolbar);
			_toolBarList.removeAt(index);
		}
	}
	QList<QToolBar*> PostWindowBase::getToolBarList()
	{
		return _toolBarList;
	}
	void PostWindowBase::closeEvent(QCloseEvent *e)
	{
		emit closeWindowSignal(this);
		QWidget::closeEvent(e);
	}
	void PostWindowBase::setView(QString view)
	{
		const int n = _graphWidgetList.size();
		for (int i = 0; i < n; ++i)
		{
			GraphWindowBase* g = _graphWidgetList.at(i);
			g->setView(view);
		}
	}
	void PostWindowBase::setViewValue(int x1, int x2, int x3, int y1, int y2, int y3, int z1, int z2, int z3)
	{
		const int n = _graphWidgetList.size();
		for (int i = 0; i < n; ++i)
		{
			GraphWindowBase* g = _graphWidgetList.at(i);
			g->setViewValue(x1,x2,x3,y1,y2,y3,z1,z2,z3);
		}
	}
		
	void PostWindowBase::appendGraphWindow(ModuleBase::GraphWindowBase* g)
	{
		_graphWidgetList.append(g);
	}
	void PostWindowBase::removeGraphWindow(ModuleBase::GraphWindowBase* g)
	{
		if (!_graphWidgetList.contains(g)) return;
		const int index = _graphWidgetList.indexOf(g);
		removeGraphWindow(index);
	}
	void PostWindowBase::removeGraphWindow(int index)
	{
		if (index < 0 || index>_graphWidgetList.size()) return;
		ModuleBase::GraphWindowBase* g = _graphWidgetList.at(index);
		_graphWidgetList.removeAt(index);
		delete g;
	}
	void PostWindowBase::removeAllGraphWindow()
	{
		const int n = _graphWidgetList.size();
		for (int i = 0; i < n; ++i)
		{
			GraphWindowBase* gw = _graphWidgetList.at(i);
			delete gw;
		}
		_graphWidgetList.clear();
	}
}