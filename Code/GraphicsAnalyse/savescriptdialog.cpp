#include "savescriptdialog.h"
#include "ui_savescriptdialog.h"

saveScriptDialog::saveScriptDialog(QWidget *parent) :
    QDialog(parent),
	ui(new Ui::saveScriptDialog)
{
    ui->setupUi(this);
	saveScript_prop_data.image_wid = 600;
	saveScript_prop_data.image_hei = 400;
	saveScript_prop_data.flag_saveImage = false;
	saveScript_prop_data.plot_wid = 600;
	saveScript_prop_data.plot_hei = 400;
	saveScript_prop_data.flag_plotSize = false;
	saveScript_prop_data.saveImage_fileName =QDir::currentPath()+"/tep_image.png";
	saveScript_prop_data.flag_saveAvi = false;
	saveScript_prop_data.flag_autoExit = false;
	saveScript_prop_data.flag_adapt = false;
	saveScript_prop_data.timeMS_autoExit = 500;
	saveScript_prop_data.saveAvi_fileName = QDir::currentPath() + "/tep_avi.avi";
	saveScript_prop_data.saveScript_fileName = QDir::currentPath() + "/tep_script.scr";
	ui->saveImage_checkBox->setChecked(saveScript_prop_data.flag_saveImage);
	ui->plotSize_checkBox->setChecked(saveScript_prop_data.flag_plotSize);
	ui->saveImage_groupBox->setEnabled(saveScript_prop_data.flag_saveImage);
	ui->saveAvi_checkBox->setChecked(saveScript_prop_data.flag_saveAvi);
	ui->saveAvi_groupBox->setEnabled(saveScript_prop_data.flag_saveAvi);
	ui->imagePath_lineEdit->setText(saveScript_prop_data.saveImage_fileName);
	ui->aviPath_lineEdit->setText(saveScript_prop_data.saveAvi_fileName);
	ui->scriptPath_lineEdit->setText(saveScript_prop_data.saveScript_fileName);
	QString tep;
	ui->wid_lineEdit->setText(tep.setNum(saveScript_prop_data.image_wid, 10));
	ui->hei_lineEdit->setText(tep.setNum(saveScript_prop_data.image_hei, 10));
	ui->autoExit_checkBox->setChecked(saveScript_prop_data.flag_autoExit);
	ui->autoExit_groupBox->setEnabled(saveScript_prop_data.flag_autoExit);
	ui->exitTime_spinBox->setValue(saveScript_prop_data.timeMS_autoExit);
	ui->plotSize_checkBox->setVisible(false);
	ui->adapt_checkBox->setChecked(saveScript_prop_data.flag_adapt);
}

saveScriptDialog::~saveScriptDialog()
{
    delete ui;
}

void saveScriptDialog::setImageSize(QSize tep_size)
{
	saveScript_prop_data.image_wid = tep_size.width();
	saveScript_prop_data.image_hei = tep_size.height();
	
	QString tep;
	ui->wid_lineEdit->setText(tep.setNum(saveScript_prop_data.image_wid, 10));
	ui->hei_lineEdit->setText(tep.setNum(saveScript_prop_data.image_hei, 10));
}

void saveScriptDialog::on_saveImage_checkBox_clicked(bool checked)
{
	saveScript_prop_data.flag_saveImage = checked;
	ui->saveImage_groupBox->setEnabled(checked);
}


void saveScriptDialog::on_imagePath_pushButton_clicked()
{
	QString filePath;
	filePath = QFileDialog::getSaveFileName(this, tr("Save Image"), "tep_image", tr("images(*.png *.jpg *.bmp *.eps)"));
	ui->imagePath_lineEdit->setText(filePath);
	saveScript_prop_data.saveImage_fileName = filePath;
}

void saveScriptDialog::on_saveAvi_checkBox_clicked(bool checked)
{
	saveScript_prop_data.flag_saveAvi = checked;
	ui->saveAvi_groupBox->setEnabled(checked);
}

void saveScriptDialog::on_aviPath_pushButton_clicked()
{
	QString filePath;
	filePath = QFileDialog::getSaveFileName(this, tr("Save Avi"), "tep_avi", tr("animation(*.avi)"));
	ui->aviPath_lineEdit->setText(filePath);
	saveScript_prop_data.saveAvi_fileName = filePath;
}

void saveScriptDialog::on_save_pushButton_clicked()
{
	accept();
}

void saveScriptDialog::on_close_pushButton_clicked()
{
	reject();
}

void saveScriptDialog::on_wid_lineEdit_textChanged(const QString &arg1)
{
	bool ok;
	saveScript_prop_data.image_wid = arg1.toInt(&ok, 10);
}

void saveScriptDialog::on_hei_lineEdit_textChanged(const QString &arg1)
{
	bool ok;
	saveScript_prop_data.image_hei = arg1.toInt(&ok, 10);
}
void saveScriptDialog::on_scriptPath_pushButton_clicked()
{
	QString filePath;
	filePath = QFileDialog::getSaveFileName(this, tr("Save Script"), "tep_script", tr("script(*.scr)"));
	ui->scriptPath_lineEdit->setText(filePath);
	saveScript_prop_data.saveScript_fileName = filePath;
}
void saveScriptDialog::on_autoExit_checkBox_clicked(bool checked)
{
	saveScript_prop_data.flag_autoExit = checked;
	ui->autoExit_groupBox->setEnabled(checked);
}

void saveScriptDialog::on_exitTime_spinBox_valueChanged(int arg1)
{
	saveScript_prop_data.timeMS_autoExit = arg1;
}

void saveScriptDialog::on_plotSize_checkBox_clicked(bool checked)
{
	saveScript_prop_data.flag_plotSize = checked;
}

void saveScriptDialog::on_adapt_checkBox_clicked(bool checked)
{
	saveScript_prop_data.flag_adapt = checked;
}
