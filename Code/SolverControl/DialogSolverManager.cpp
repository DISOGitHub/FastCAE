#include "DialogSolverManager.h"
#include "ui_DialogSolverManager.h"
#include "DialogAddSolver.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/SolverConfig.h"
#include "ConfigOptions/SolverInfo.h"
#include <QCoreApplication>

namespace SolverControl
{
	SolverManagerDialog::SolverManagerDialog(GUI::MainWindow* mw) : /*QFDialog(mw),*/
		_ui(new Ui::SolverManagerDialog), _mainWindow(mw)
	{
		_ui->setupUi(this);
		connect(_ui->solverList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(listItemClicked(QListWidgetItem*)));
		_solvers = ConfigOption::ConfigOption::getInstance()->getSolverOption();
		update();
	}
	SolverManagerDialog::~SolverManagerDialog()
	{
		this->writeConfig();
		delete _ui;
	}
	
	void SolverManagerDialog::on_out_AddSolverButton_clicked()
	{
		AddSolverDialog dlg(_mainWindow, this, nullptr);
		dlg.exec();
	}
	
	void SolverManagerDialog::on_out_RemoveButton_clicked()
	{
		QListWidgetItem* item = _ui->solverList->currentItem();
		if (item == nullptr) return;

		bool ok = false;
		int index = item->data(Qt::UserRole).toInt(&ok);
		if (!ok) return;
		_solvers->removeSolver(index);
		this->update();
	}

	void SolverManagerDialog::update()
	{
		_ui -> solverList->clear();

		const int nsolver = _solvers->getSolverCount();
		for (int i = 0; i < nsolver; ++i)
		{
			ConfigOption::SolverInfo* info = _solvers->getSolverAt(i);
			QString name = info->getName();
			QListWidgetItem *item = new QListWidgetItem(name);
			item->setData(Qt::UserRole, i);
			_ui->solverList->addItem(item);
		}
	}

	void SolverManagerDialog::on_out_ModifyButton_clicked()
	{
		QListWidgetItem* item = _ui->solverList->currentItem();
		if (item == nullptr) return;

		bool ok = false;
		int index = item->data(Qt::UserRole).toInt(&ok);
		if (!ok) return;
		ConfigOption::SolverInfo* info = _solvers->getSolverAt(index);
		if (info == nullptr) return;
		AddSolverDialog dlg(_mainWindow, this, info);
		dlg.exec();



	}

	void SolverManagerDialog::writeConfig()
	{
		QString exePath = qApp->applicationDirPath();
		QString configPath = exePath + "/../ConfigFiles/";
		_solvers->write(configPath + "SolverConfig.config");
	}

}