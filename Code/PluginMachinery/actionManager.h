#ifndef ACTIONMANAGER_H_
#define ACTIONMANAGER_H_

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
	class MachineryPlugin;

	class ActionManager : public QObject
	{
		Q_OBJECT
	public:
		ActionManager(QMainWindow* m, MachineryPlugin* plug);
		~ActionManager();

		void preWindowOpened(MainWidget::PreWindow* pw);
		void preWindowClosed();

		void release();

	private:
		void addAction(QAction* ac);

	private slots:
	    //在这里添加action槽函数



	private:
		MachineryPlugin* _plugin{};
		QMainWindow* _mainwindow{};
		MainWidget::PreWindow* _preWindow{};

		QMenu* _menu{};
		QToolBar* _toolBar{};
	};
}


#endif