#include "saveimagedialog.h"
#include "ui_saveimagedialog.h"

saveImageDialog::saveImageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::saveImageDialog)
{
    ui->setupUi(this);
	saveImage_prop_data.image_wid=600;
	saveImage_prop_data.image_hei=400;
	saveImage_prop_data.flag_saveImage=true;
	saveImage_prop_data.saveImage_fileName="";
	saveImage_prop_data.flag_savePov=false;
	saveImage_prop_data.savePov_fileName="";
	ui->saveImage_checkBox->setChecked(saveImage_prop_data.flag_saveImage);
	ui->saveImage_groupBox->setEnabled(saveImage_prop_data.flag_saveImage);
	ui->savePOV_checkBox->setChecked(saveImage_prop_data.flag_savePov);
	ui->savePov_groupBox->setEnabled(saveImage_prop_data.flag_savePov);
	if (saveImage_prop_data.saveImage_fileName!="")
		ui->imagePath_lineEdit->setText(saveImage_prop_data.saveImage_fileName);
	if (saveImage_prop_data.savePov_fileName != "")
		ui->povPath_lineEdit->setText(saveImage_prop_data.savePov_fileName);
	QString tep;
	ui->wid_lineEdit->setText(tep.setNum(saveImage_prop_data.image_wid,10));
	ui->hei_lineEdit->setText(tep.setNum(saveImage_prop_data.image_hei, 10));

}

saveImageDialog::~saveImageDialog()
{
    delete ui;
}

void saveImageDialog::setImageSize(QSize tep_size)
{
	saveImage_prop_data.image_wid = tep_size.width();
	saveImage_prop_data.image_hei = tep_size.height();
	QString tep;
	ui->wid_lineEdit->setText(tep.setNum(saveImage_prop_data.image_wid, 10));
	ui->hei_lineEdit->setText(tep.setNum(saveImage_prop_data.image_hei, 10));
}

void saveImageDialog::on_saveImage_checkBox_clicked(bool checked)
{
	saveImage_prop_data.flag_saveImage = checked;
	ui->saveImage_groupBox->setEnabled(checked);
}


void saveImageDialog::on_imagePath_pushButton_clicked()
{
	QString filePath;
	filePath = QFileDialog::getSaveFileName(this, tr("Save Image"), "tep_image", tr("images(*.png *.jpg *.bmp *.eps)"));
	ui->imagePath_lineEdit->setText(filePath);
	saveImage_prop_data.saveImage_fileName = filePath;
}

void saveImageDialog::on_savePOV_checkBox_clicked(bool checked)
{
	saveImage_prop_data.flag_savePov = checked;
	ui->savePov_groupBox->setEnabled(checked);
}

void saveImageDialog::on_povPath_pushButton_clicked()
{
	QString filePath;
	filePath = QFileDialog::getSaveFileName(this, tr("Save POV"), "tep_pov", tr("images(*.pov)"));
	ui->povPath_lineEdit->setText(filePath);
	saveImage_prop_data.savePov_fileName = filePath;
}

void saveImageDialog::on_save_pushButton_clicked()
{
	accept();
}

void saveImageDialog::on_close_pushButton_clicked()
{
	reject();
}

void saveImageDialog::on_wid_lineEdit_textChanged(const QString &arg1)
{
	bool ok;
	saveImage_prop_data.image_wid = arg1.toInt(&ok, 10);
}

void saveImageDialog::on_hei_lineEdit_textChanged(const QString &arg1)
{
	bool ok;
	saveImage_prop_data.image_hei = arg1.toInt(&ok, 10);
}
