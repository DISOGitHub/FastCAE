#include "projectSolveDialog.h"
#include "ui_projectSolveDialog.h"
#include "settings/busAPI.h"
#include "moduleBase/processBar.h"
//#include "SolverControl/solverControler.h"
#include "mainWindow/mainWindow.h"
// #include "settings/SolverManager.h"
// #include "settings/SolverInfo.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/SolverConfig.h"
#include "ConfigOptions/SolverInfo.h"
#include "ModelData/modelDataSingleton.h"
#include "ModelData/modelDataBase.h"
#include <QFile>
#include <assert.h>
#include <QMessageBox>

namespace MainWidget
{
	ProjcctSolveDialog::ProjcctSolveDialog(GUI::MainWindow* mainwindow, bool &showDlg, int proid) : QFDialog(mainwindow),
		_ui(new Ui::projectSolveDialog), _mainWindow(mainwindow), _showDlg(showDlg), _proID(proid)
	{
		_ui->setupUi(this);
		connect(this, SIGNAL(solveProject(int, int)), mainwindow, SIGNAL(solveProjectSig(int, int)));
		connect(this, SIGNAL(sig_display_message(ModuleBase::Message)), _mainWindow, SIGNAL(printMessageToMessageWindow(ModuleBase::Message)));
		init();
	}

	ProjcctSolveDialog::~ProjcctSolveDialog()
	{
		delete _ui;
	}
	void ProjcctSolveDialog::init()
	{
		ModelData::ModelDataSingleton* modelData = ModelData::ModelDataSingleton::getinstance();
		const int modelCount = modelData->getModelCount();
		int index = -1;

		for (int i = 0; i < modelCount; ++i)
		{
			ModelData::ModelDataBase* model = modelData->getModelAt(i);
			const QString proname = model->getName();
			_ui->projectComboBox->addItem(proname);
			if (_proID == model->getID()) index = i;
		}
		if (_proID > 0)
		{
			_ui->projectComboBox->setCurrentIndex(index);
			_ui->projectComboBox->setEnabled(false);
		}

// 		Setting::SolverManager *manager = Setting::BusAPI::instance()->getSolverManager();
// 		const int sn = manager->getSolverCount();
// 		for (int i = 0; i < sn; ++i)
// 		{
// 			Setting::SolverInfo* info = manager->getSolverAt(i);
// 			QString name = info->getName();
// 			_ui->solverComboBox->addItem(name);
// 		}
		ConfigOption::SolverOption* solveroption = ConfigOption::ConfigOption::getInstance()->getSolverOption();
		const int sn = solveroption->getSolverCount();
		for (int i = 0; i < sn; ++i)
		{
			ConfigOption::SolverInfo* solver = solveroption->getSolverAt(i);
			QString name = solver->getName();
			_ui->solverComboBox->addItem(name);
		}
		if (_proID > 0 && sn == 1)
		{
			_showDlg = false;
			on_solveButton_clicked();
		}
		else if (modelCount == 1 && sn == 1)
		{
			_showDlg = false;
			on_solveButton_clicked();
		}
	}
	void ProjcctSolveDialog::on_solveButton_clicked()
	{
		const int modelIndex = _ui->projectComboBox->currentIndex();
		if (modelIndex < 0) return;
		const int solverIndex = _ui->solverComboBox->currentIndex();
		if (solverIndex < 0) return;
		ModelData::ModelDataBase* mb = ModelData::ModelDataSingleton::getinstance()->getModelAt(modelIndex);
		QVector<ModuleBase::Message>  messages;
		if (!mb->checkSolveableStatus(messages))
		{
			//QMessageBox::warning(this, tr("Warning"), tr("This project can not be solved !"));
			for (ModuleBase::Message message : messages)
			{
				emit sig_display_message(message);
			}
			this->close();
			return;
		}

		emit solveProject(modelIndex, solverIndex);
		
		this->accept();
		close();
	}
	void ProjcctSolveDialog::on_cancleButton_clicked()
	{
//		_startSolve = false;
		this->rejected();
		close();
	}
// 	void ProjcctSolveDialog::changeCurrentIndex(int index)
// 	{
// 		_currentIndex = index;
// 	}
	void ProjcctSolveDialog::closeEvent(QCloseEvent *event)
	{
// 		if (!_startSolve)
// 			emit solveFailed();
	}
}