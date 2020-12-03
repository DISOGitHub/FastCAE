#ifndef ACTIONCOMPLEXMANAGER_H_
#define ACTIONCOMPLEXMANAGER_H_

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

namespace Plugins
{
	class ComplexPlugin;

	class ActionComplexManager : public QObject
	{
		Q_OBJECT
	public:
		ActionComplexManager(GUI::MainWindow*  m, ComplexPlugin* plug);
		~ActionComplexManager();

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
		void CreateCylindricalComplex();
		void CreateBoxComplex();



	private:
		ComplexPlugin* _plugin{};
		GUI::MainWindow*  _mainwindow{};
		MainWidget::PreWindow* _preWindow{};

		QMenu* _menu{};
		QToolBar* _toolBar{};

		QList<QAction*> _actionList{};
	};
}


#endif