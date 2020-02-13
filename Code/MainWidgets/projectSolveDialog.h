#ifndef _PROJECTSOLVEDIALOG_H_
#define _PROJECTSOLVEDIALOG_H_

#include "mainWidgetsAPI.h"
#include "SelfDefObject/QFDialog.h"
#include "moduleBase/messageWindowBase.h"

namespace Ui
{
	class projectSolveDialog;
}
namespace GUI
{
	class MainWindow;
}

namespace MainWidget
{

	class MAINWIDGETSAPI ProjcctSolveDialog : public QFDialog
	{
		Q_OBJECT
	public:
		ProjcctSolveDialog(GUI::MainWindow* mainwindow, bool &showDlg,int proid = -1);
		~ProjcctSolveDialog();

	private:
		void init();

	signals:
		void solveProject(int modelIndex, int solverIndex);

		void sig_display_message(ModuleBase::Message message);

	private slots:
		void on_solveButton_clicked();
		void on_cancleButton_clicked();
	private:
		void closeEvent(QCloseEvent *event);

	private:
		Ui::projectSolveDialog* _ui{};
		GUI::MainWindow* _mainWindow{};
		bool &_showDlg;
		int _proID{ -1 };

	};


}


#endif
