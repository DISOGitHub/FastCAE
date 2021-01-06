#include "curve_aboutdialog.h"
#include "ui_curve_aboutdialog.h"

curve_aboutDialog::curve_aboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::curve_aboutDialog)
{
    ui->setupUi(this);
}

curve_aboutDialog::~curve_aboutDialog()
{
    delete ui;
}

void curve_aboutDialog::on_pushButton_clicked()
{
    close();
}
