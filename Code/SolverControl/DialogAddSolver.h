#ifndef ADDSOLVERDIALOG_H
#define ADDSOLVERDIALOG_H

#include "SelfDefObject/QFDialog.h"
#include <QHash>
#include <QString>

namespace Ui
{
	class AddSolverDialog;
}

namespace GUI
{
	class MainWindow;
}

namespace ConfigOption
{
	class SolverInfo;
	class SolverOption;
}

namespace SolverControl
{
	class SolverManagerDialog;

	class AddSolverDialog :public QFDialog
	{
		Q_OBJECT
	public:
		AddSolverDialog(GUI::MainWindow* mainwin, SolverManagerDialog* parent, ConfigOption::SolverInfo* solver);
		~AddSolverDialog();

	private:
		void init();
		void initCombox();

	private slots:
		void on_out_OkButton_clicked();
		void on_out_CancelButton_clicked();
		void on_fileSelect_clicked();
		void on_fileSelect_1_clicked();

	private:
		Ui::AddSolverDialog* _ui{};
		SolverManagerDialog* _parent{};

		ConfigOption::SolverOption* _solverOption{};
		ConfigOption::SolverInfo*  _modifySolver{};
		bool _isEdit{ false };

		QHash<QString, QString> _inpTextData{};
		QHash<QString, QString> _transferTextData{};
	};
}


#endif
