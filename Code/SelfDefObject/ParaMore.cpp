#include "ParaMore.h"
#include "mainWindow/mainWindow.h"
#include "ui_ParaMore.h"
#include <QLineEdit>
#include <QHBoxLayout>
#include <QSize>

namespace SelfDefObj
{
	ParaMore::ParaMore(GUI::MainWindow* m, DataProperty::ParameterBase* data)
		:_ui(new Ui::ParaMore), _data(data)
	{
		connect(this, SIGNAL(lockGraphFocus(bool)), m, SIGNAL(enableGraphWindowKeyBoard(bool)));
		_ui->setupUi(this);
		_ui->gridLayout->setSpacing(0);
//		updateLineEdit();
	}

	ParaMore::~ParaMore()
	{
		if (_ui != nullptr) delete _ui;
	}

	void ParaMore::focusInEvent(QFocusEvent *e)
	{
		emit lockGraphFocus(false);
		QWidget::focusInEvent(e);
	}

	void ParaMore::focusOutEvent(QFocusEvent *e)
	{
		emit lockGraphFocus(true);
		QWidget::focusOutEvent(e);
	}
	void ParaMore::on_moreButton_clicked()
	{
		
	}

	void ParaMore::updateLineEdit()
	{
		_ui->lineEdit->clear();
	}
	void ParaMore::setText(QString text)
	{
		_ui->lineEdit->setText(text);
	}

}
