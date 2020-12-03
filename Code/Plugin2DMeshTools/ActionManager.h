#ifndef _D2MESHTOOLSACTIONS_H__
#define _D2MESHTOOLSACTIONS_H__

#include <QObject>
#include <QList>

namespace GUI
{
	class MainWindow;
}

class QToolBar;
class QMenu;
class QAction;

namespace D2MeshTools
{
	class ActionManager : public QObject
	{
		Q_OBJECT
	public:
		ActionManager(GUI::MainWindow* mw);
		~ActionManager() = default;

		void install();
		void unInstall();
		void translate();

	private:
		void addAction(QAction* ac);

	private:
		GUI::MainWindow* _mainWindow{};

		QList<QAction*> _actions{};
		QToolBar* _toolBar{};
		QMenu* _menu{};

	};

}


#endif