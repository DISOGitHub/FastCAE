#include "dialogInputData.h"
#include "ui_dialogInputData.h"


namespace PluginShip
{
	InputDataDialog::InputDataDialog()
	{
		_ui = new Ui::InputDataDialog;
		_ui->setupUi(this);
	}

	InputDataDialog::~InputDataDialog()
	{

	}
	
	void InputDataDialog::accept()
	{
		double axis1 = _ui->lineEdit_1->text().toDouble();
		double axis2 = _ui->lineEdit_2->text().toDouble();
		emit sendAxis(axis1, axis2);
		QDialog::accept();
		this->close();
	}

}