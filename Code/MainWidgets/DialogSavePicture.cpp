#include "DialogSavePicture.h"
#include "SelfDefObject/QFDialog.h"
#include "ui_DialogSavePicture.h"
#include "mainWindow/mainWindow.h"
#include <QFileDialog>
#include <QMessageBox>

namespace MainWidget
{
	SavePictureDialog::SavePictureDialog(GUI::MainWindow* mainwindow,QWidget* w) : QFDialog(mainwindow)
	{
		_ui = new Ui::SavePicDialog;
		_ui->setupUi(this);
		_ui->widthBox->setMaximum(10000);
		_ui->highBox->setMaximum(10000);
		_ui->widthBox->setMinimum(1);
		_ui->highBox->setMinimum(1);
		_widget = w;
		this->init();
		connect(this, SIGNAL(saveImageSig(int, int, QString)), mainwindow, SIGNAL(saveImage(int, int, QString)));
	}

	SavePictureDialog::~SavePictureDialog()
	{
		 if (_ui != nullptr) delete _ui;
	}

	void SavePictureDialog::on_browseButton_clicked()
	{
		QString text = QFileDialog::getSaveFileName(this, tr("Save Picture"), "", "PNG(*.png)");
		if (!text.isEmpty())
			_ui->lineEdit->setText(text);
	}

	void SavePictureDialog::init()
	{
		if (_widget == nullptr) return;
		const int w = _widget->width();
		const int h = _widget->height();
		_ui->highBox->setValue(h);
		_ui->widthBox->setValue(w);
	}

	void SavePictureDialog::accept()
	{
		const int w = _ui->widthBox->value();
		const int h = _ui->highBox->value();
		QString f = _ui->lineEdit->text();
		if (f.isEmpty())
		{
			QMessageBox::warning(this, QString(tr("Warning")), QString(tr("File name is empty !")));
			return;
		}
		emit saveImageSig(w, h, f);
		QFDialog::accept();
	}



}