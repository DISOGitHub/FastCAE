#ifndef ACTIONHULLMANAGER_H_
#define ACTIONHULLMANAGER_H_

#include <QObject>
#include <QString>
#include <QList>

class QMenu;
class QToolBar;
class QAction;
class QDialog;

namespace GUI
{
	class MainWindow;
};

namespace MainWidget
{
	class PreWindow;
}
namespace GeometryWidget
{
	class HullTreeDock;
};
namespace Plugins
{
	class HullCutPlugin;

	class ActionHullManager : public QObject
	{
		Q_OBJECT
	public:
		ActionHullManager(GUI::MainWindow*  m, HullCutPlugin* plug);
		~ActionHullManager();

		void preWindowOpened(MainWidget::PreWindow* pw);
		void preWindowClosed();

		void release();
		void reTranslate();

	private:
		
		void addAction(QAction* ac);
	    //void showDialog(QDialog* dia);
		//void openPreWinPy();

	private slots:
	    //在这里添加action槽函数
		void CreateHull();

	private:
		HullCutPlugin* _plugin{};
		GUI::MainWindow*  _mainwindow{};
		MainWidget::PreWindow* _preWindow{};

		QMenu* _menu{};
		QToolBar* _toolBar{};

		QList<QAction*> _actionList{};
	};
}


#endif