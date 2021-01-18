#ifndef _DIALOGSOLVERMANAGER_H_
#define _DIALOGSOLVERMANAGER_H_

#include <QDialog>
#include "solverControlAPI.h"

class QListWidgetItem;

namespace ConfigOption
{
	class SolverOption;
};

namespace Ui
{
	class SolverManagerDialog;
}

namespace GUI
{
	class MainWindow;
}

namespace SolverControl
{
	class SOLVERCONTROLAPI SolverManagerDialog : public QDialog
	{
		Q_OBJECT
	public:
		SolverManagerDialog(GUI::MainWindow* mw);
		~SolverManagerDialog();
		void update();

	private slots:
	    void on_out_AddSolverButton_clicked();
		void on_out_RemoveButton_clicked();
		void on_out_ModifyButton_clicked();

	private:
		void writeConfig();

	private:
		Ui::SolverManagerDialog* _ui{};
		GUI::MainWindow* _mainWindow{};

		ConfigOption::SolverOption* _solvers{};
	};


}

#endif
