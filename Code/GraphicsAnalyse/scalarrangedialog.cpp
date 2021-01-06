#include "scalarrangedialog.h"
#include "ui_scalarrangedialog.h"

scalarRangeDialog::scalarRangeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::scalarRangeDialog)
{
    ui->setupUi(this);
}

scalarRangeDialog::~scalarRangeDialog()
{
    delete ui;
}

void scalarRangeDialog::updateForm(double* val)
{
	QString tep;
	ui->mini_lineEdit->setText(tep.setNum(val[0],'g',6));
	ui->maxi_lineEdit->setText(tep.setNum(val[1], 'g', 6));
}

void scalarRangeDialog::on_rescale_pushButton_clicked()
{
	double val[2];
    bool ok;
    val[0] = ui->mini_lineEdit->text().trimmed().toDouble(&ok);
    val[1] = ui->maxi_lineEdit->text().trimmed().toDouble(&ok);
	emit sig_update_scalarRange(val);
	this->close();
}

void scalarRangeDialog::on_close_pushButton_clicked()
{
	this->close();
}
