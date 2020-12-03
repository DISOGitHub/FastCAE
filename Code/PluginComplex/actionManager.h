#ifndef ACTIONCOMPLEXMANAGER_H_
#define ACTIONCOMPLEXMANAGER_H_

#include <QObject>

class QMainWindow;
class QMenu;
class QToolBar;
class QAction;

namespace MainWidget
{
	class PreWindow;
}

namespace Plugins
{
	class ComplexPlugin;

	class ActionComplexManager : public QObject
	{
		Q_OBJECT
	public:
		ActionComplexManager(QMainWindow* m, ComplexPlugin* plug);
		~ActionComplexManager();

		void preWindowOpened(MainWidget::PreWindow* pw);
		void preWindowClosed();

		void release();

	private:
		void addAction(QAction* ac);

	private slots:
	    //在这里添加action槽函数



	private:
		ComplexPlugin* _plugin{};
		QMainWindow* _mainwindow{};
		MainWidget::PreWindow* _preWindow{};

		QMenu* _menu{};
		QToolBar* _toolBar{};
	};
}


#endif