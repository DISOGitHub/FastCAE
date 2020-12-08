#include "curve_saveimagedialog.h"
#include "ui_curve_saveimagedialog.h"

curve_saveImageDialog::curve_saveImageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::curve_saveImageDialog)
{
    ui->setupUi(this);
	saveImage_prop_data.image_wid=600;
	saveImage_prop_data.image_hei=400;
	saveImage_prop_data.saveImage_fileName="";
	if (saveImage_prop_data.saveImage_fileName!="")
		ui->imagePath_lineEdit->setText(saveImage_prop_data.saveImage_fileName);
	QString tep;
	ui->wid_lineEdit->setText(tep.setNum(saveImage_prop_data.image_wid,10));
	ui->hei_lineEdit->setText(tep.setNum(saveImage_prop_data.image_hei, 10));

}

curve_saveImageDialog::~curve_saveImageDialog()
{
    delete ui;
}


void curve_saveImageDialog::on_imagePath_pushButton_clicked()
{
	QString filePath;
	filePath = QFileDialog::getSaveFileName(this, tr("Save Image"), "tep_image", "images(*.png *.jpg *.bmp *.pdf)");
	ui->imagePath_lineEdit->setText(filePath);
	saveImage_prop_data.saveImage_fileName = filePath;
}

void curve_saveImageDialog::on_save_pushButton_clicked()
{
	accept();
}

void curve_saveImageDialog::on_close_pushButton_clicked()
{
	reject();
}

void curve_saveImageDialog::on_wid_lineEdit_textChanged(const QString &arg1)
{
	bool ok;
	saveImage_prop_data.image_wid = arg1.toInt(&ok, 10);
}

void curve_saveImageDialog::on_hei_lineEdit_textChanged(const QString &arg1)
{
	bool ok;
	saveImage_prop_data.image_hei = arg1.toInt(&ok, 10);
}
