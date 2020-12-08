#include "DialogWorkingDir.h"
#include "ui_DialogWorkingDir.h"
#include "mainSetting.h"
#include "QFileDialog"
#include <QDir>
#include <QMessageBox>
#include <QCloseEvent>

namespace Setting
{

	WorkingDirDialog::WorkingDirDialog(GUI::MainWindow* mw, MainSetting* setting) : /*QFDialog(mw),*/
_ui(new Ui::DialogWorkingDir), _setting(setting)
{

	_ui->setupUi(this);
	init();
}
WorkingDirDialog::~WorkingDirDialog()
{
	delete _ui;
}
void WorkingDirDialog::init()
{
	QString path = _setting->getWorkingDir();
	_ui->lineEdit->setText(path);
}

void WorkingDirDialog::accept()
{
	QString path = _ui->lineEdit->text();
	QDir dir(path);
	if (!dir.exists() || path.isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("The Dir is not exist!"));
		_ui->lineEdit->clear();
		return;
	}
	_setting->setWorkingDir(path);
	QDialog::accept();
}
void WorkingDirDialog::on_pushButton_clicked()
{
	const QString title = tr("Select a Directory");
	const QString dir = _ui->lineEdit->text();
	const QString path = QFileDialog::getExistingDirectory(this, title, dir);
	if (path.isEmpty()) return;
	_ui->lineEdit->setText(path);
}
void WorkingDirDialog::reject()
{
	QString w = _setting->getWorkingDir();
	if (w.isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Must set Working Dir first!"));
		return;
	}
	QDialog::reject();
}
void WorkingDirDialog::closeEvent(QCloseEvent *e)
{
	QString w = _setting->getWorkingDir();
	if (w.isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Must set Working Dir first!"));
		e->ignore();
		return;
	}
	else
		QDialog::closeEvent(e);
}


}//end of namespace
