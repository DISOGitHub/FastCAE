#ifndef _EDITORSOLVERMANAGER_H_
#define _EDITORSOLVERMANAGER_H_

#include <QDialog>
#include "SolverControl/solverControlAPI.h"
#include <QDomDocument>
#include <QDomElement>

class QListWidgetItem;

namespace ConfigOption
{
	class SolverOption;
	class SolverInfo;
};

namespace Ui
{
	class EditorSolverManager;
}

namespace GUI
{
	class MainWindow;
}

namespace FastCAEDesigner
{
	class EditorSolverManager : public QDialog
	{
		Q_OBJECT
	public:
		EditorSolverManager(GUI::MainWindow* mw);
		~EditorSolverManager();
		void update();
		void writeConfig();

		void setDependencyFiles(QStringList);
		QList<QStringList> getDependecyFiles();

	private slots:
	    void on_out_AddSolverButton_clicked();
		void on_out_RemoveButton_clicked();
		void on_out_ModifyButton_clicked();


	private:
		Ui::EditorSolverManager* _ui{};
		GUI::MainWindow* _mainWindow{};

		ConfigOption::SolverOption* _solvers{};
		QList<QStringList> _dependencyFiles{};
	};


}

#endif
