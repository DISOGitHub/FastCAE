#include "DialogSelectMesher.h"
#include "ui_DialogSelectMesher.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/SolverConfig.h"
#include "ConfigOptions/MesherInfo.h"

namespace MainWidget
{
	MesherDialog::MesherDialog(GUI::MainWindow* mainwindow)
		: QFDialog(mainwindow), _ui(new Ui::MesherDialog)
	{
		_ui->setupUi(this);
		ConfigOption::SolverOption* solvers = ConfigOption::ConfigOption::getInstance()->getSolverOption();
		const int n = solvers->getMesherCount();
		if (n == 1) _isSingleMesher = true;

		for (int i = 0; i < n; ++i)
		{
			ConfigOption::MesherInfo* info = solvers->getMesherAt(i);
			QString text = info->getMesherName();
			_ui->mesherBox->addItem(text);
		}
	}

	MesherDialog::~MesherDialog()
	{
		delete _ui;
	}

	bool MesherDialog::isSingleMesher()
	{
		return _isSingleMesher;
	}

	DataProperty::DataBase* MesherDialog::getMesherInfo()
	{
		ConfigOption::SolverOption* solvers = ConfigOption::ConfigOption::getInstance()->getSolverOption();
		const int index = _ui->mesherBox->currentIndex();
		return solvers->getMesherAt(index);
	}

}
