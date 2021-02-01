#include "LineEditDialog.h"
#include "ui_LineEditDialog.h"

namespace SelfDefObj
{
	LineEditDialog::LineEditDialog(GUI::MainWindow* m, QString &text)
		:QFDialog(m), _text(text), _ui(new Ui::LineEditDialog)
	{
		_ui->setupUi(this);
		_ui->lineEdit->setText(_text);
	}

	LineEditDialog::~LineEditDialog()
	{
		if (_ui!= nullptr)
		{
			delete _ui;
		}
	}

	void LineEditDialog::accept()
	{
		QString t = _ui->lineEdit->text();
		_text = t;
		QDialog::accept();
	}

	void LineEditDialog::setDescribe(QString d)
	{
		_ui->label->setText(d);
	}


}