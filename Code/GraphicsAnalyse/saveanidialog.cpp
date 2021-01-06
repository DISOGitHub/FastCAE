#include "saveanidialog.h"
#include "ui_saveanidialog.h"

saveAniDialog::saveAniDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::saveAniDialog)
{
    ui->setupUi(this);
	ui->frameNum_spinBox->setEnabled(false);
}

saveAniDialog::~saveAniDialog()
{
    delete ui;
}

void saveAniDialog::updateDialog(saveAni_prop* data)
{
	QString str;
	ani_prop_data = data;
	ui->aniRate_doubleSpinBox->setValue(ani_prop_data->frame_rate);
	ui->endFrame_spinBox->setRange(0, ani_prop_data->total_frame_num - 1);
	ui->endFrame_spinBox->setValue(ani_prop_data->frame_end_xh);
	ui->startFrame_spinBox->setRange(0, ani_prop_data->total_frame_num - 1);
	ui->startFrame_spinBox->setValue(ani_prop_data->frame_start_xh);
	ui->frameNum_spinBox->setRange(0, ani_prop_data->total_frame_num);
	ui->saveImage_checkBox->setChecked(ani_prop_data->flag_saveImage);
	ui->saveImage_groupBox->setEnabled(ani_prop_data->flag_saveImage);
	ui->imageFormat_comboBox->setCurrentText(ani_prop_data->saveImage_format);
	if (ani_prop_data->saveImage_path != "")
		ui->imagePath_lineEdit->setText(ani_prop_data->saveImage_path);
	ui->savePOV_checkBox->setChecked(ani_prop_data->flag_savePov);
	ui->savePov_groupBox->setEnabled(ani_prop_data->flag_saveImage);
	if (ani_prop_data->savePov_path != "")
		ui->povPath_lineEdit->setText(ani_prop_data->savePov_path);
	int tep_val = ani_prop_data->frame_end_xh - ani_prop_data->frame_start_xh;
	if (tep_val < 0)
		tep_val = 0;
	else
	{
		tep_val = tep_val / ani_prop_data->frame_step;
		tep_val++;
	}
	ui->frameNum_spinBox->setValue(tep_val);
	ui->frameStep_spinBox->setRange(1, ani_prop_data->total_frame_num - 1);
	ui->frameStep_spinBox->setValue(ani_prop_data->frame_step);
	ui->pngWid_lineEdit->setText(str.setNum(ani_prop_data->image_wid, 10));
	ui->pngHei_lineEdit->setText(str.setNum(ani_prop_data->image_hei, 10));
	if (ani_prop_data->total_frame_num < 2)
		ui->saveAni_Btn->setEnabled(false);
	else
		ui->saveAni_Btn->setEnabled(true);

	//ui->savePOV_checkBox->setVisible(false);
	//ui->savePov_groupBox->setVisible(false);
}

void saveAniDialog::on_aniRate_doubleSpinBox_valueChanged(double arg1)
{
	ani_prop_data->frame_rate = arg1;
}

void saveAniDialog::on_frameNum_spinBox_valueChanged(int arg1)
{
	ani_prop_data->frame_num = arg1;
}

void saveAniDialog::on_frameStep_spinBox_valueChanged(int arg1)
{
	ani_prop_data->frame_step = arg1;
	
	int tep_val = ani_prop_data->frame_end_xh - ani_prop_data->frame_start_xh;
	if (tep_val < 0)
		tep_val = 0;
	else
	{
		tep_val = tep_val / ani_prop_data->frame_step;
		tep_val++;
	}
	ui->frameNum_spinBox->setValue(tep_val);
}

void saveAniDialog::on_startFrame_spinBox_valueChanged(int arg1)
{
	ani_prop_data->frame_start_xh = arg1;
	int tep_val = ani_prop_data->frame_end_xh - ani_prop_data->frame_start_xh;
	if (tep_val < 0)
		tep_val = 0;
	else
	{
		tep_val = tep_val / ani_prop_data->frame_step;
		tep_val++;
	}
	ui->frameNum_spinBox->setValue(tep_val);
}

void saveAniDialog::on_endFrame_spinBox_valueChanged(int arg1)
{
	ani_prop_data->frame_end_xh = arg1;
	int tep_val = ani_prop_data->frame_end_xh - ani_prop_data->frame_start_xh;
	if (tep_val < 0)
		tep_val = 0;
	else
	{
		tep_val = tep_val / ani_prop_data->frame_step;
		tep_val++;
	}
	ui->frameNum_spinBox->setValue(tep_val);
}

void saveAniDialog::on_pngWid_lineEdit_editingFinished()
{
	bool ok;
	ani_prop_data->image_wid = ui->pngWid_lineEdit->text().toInt(&ok, 10);
}

void saveAniDialog::on_pngHei_lineEdit_editingFinished()
{
	bool ok;
	ani_prop_data->image_hei = ui->pngHei_lineEdit->text().toInt(&ok, 10);
}

void saveAniDialog::on_saveAni_Btn_clicked()
{
	if (ani_prop_data->frame_end_xh < ani_prop_data->frame_start_xh)
		return;
	if (ani_prop_data->total_frame_num <= 1)
		return;
	//////////////////////////////
	QString tep_filename;
	tep_filename = QFileDialog::getSaveFileName(this, tr("Save Avi"), "tep_avi", tr("animation(*.avi)"));
	if (tep_filename == "")
		return;
	ani_prop_data->avi_filename = tep_filename;
	////////////////////////////////
	ani_prop_data->flag_save = true;
	accept();
}

void saveAniDialog::on_cancle_Btn_clicked()
{
	ani_prop_data->flag_save = false;
    close();
}

void saveAniDialog::on_imageFormat_comboBox_currentIndexChanged(int index)
{
	if (index == -1)
		return;
	ani_prop_data->saveImage_format = ui->imageFormat_comboBox->currentText();
}

void saveAniDialog::on_imagePath_pushButton_clicked()
{
	QString filePath;
	filePath = QFileDialog::getExistingDirectory();
	ui->imagePath_lineEdit->setText(filePath);
	ani_prop_data->saveImage_path = filePath;
}

void saveAniDialog::on_povPath_pushButton_clicked()
{
	QString filePath;
	filePath = QFileDialog::getExistingDirectory();
	ui->povPath_lineEdit->setText(filePath);
	ani_prop_data->savePov_path = filePath;
}

void saveAniDialog::on_saveImage_checkBox_clicked(bool checked)
{
	ani_prop_data->flag_saveImage = checked;
	ui->saveImage_groupBox->setEnabled(checked);
}

void saveAniDialog::on_savePOV_checkBox_clicked(bool checked)
{
	ani_prop_data->flag_savePov = checked;
	ui->savePov_groupBox->setEnabled(checked);
}
