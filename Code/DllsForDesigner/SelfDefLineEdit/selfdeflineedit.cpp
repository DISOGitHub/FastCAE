#include "selfdeflineedit.h"
#include "ui_selfdeflineedit.h"

SelfDefLineEdit::SelfDefLineEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelfDefLineEdit)
{
    ui->setupUi(this);
}

SelfDefLineEdit::~SelfDefLineEdit()
{
    delete ui;
}
