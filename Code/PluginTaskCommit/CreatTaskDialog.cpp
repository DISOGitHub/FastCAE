#include "CreatTaskDialog.h"
#include "ui_CreatTaskDialog.h"
#include "TaskMacro.h"

#include "IO/IOConfig.h"
#include "ModelData/modelDataSingleton.h"
#include "ModelData/modelDataBase.h"

#include <QDateTime>
#include <QDebug>
#include <QMessageBox>

using namespace  Plugins;

CreatTaskDialog::CreatTaskDialog(GUI::MainWindow *parent) :
    QFDialog(parent),
    ui(new Ui::CreatTaskDialog)
{
    ui->setupUi(this);
//    QPushButton* pb = new QPushButton();
//    pb->setIcon(QIcon(":/images/images/arrow.png"));
//    pb->resize(30, ui->pathLineEdit->height());
//    pb->setFixedWidth(30);
//    pb->setMaximumWidth(30);
//    QHBoxLayout *l = new QHBoxLayout(ui->pathLineEdit);
//    l->setMargin(0);
//    l->addStretch();
//    l->addWidget(pb);
//    ui->pathLineEdit->setLayout(l);
    ui->solverComboBox->addItem("风雷");
    this->setWindowTitle(tr("CreateTask"));
}

CreatTaskDialog::~CreatTaskDialog()
{
    delete ui;
}

void CreatTaskDialog::init()
{
    ModelData::ModelDataSingleton *s = ModelData::ModelDataSingleton::getinstance();
    int n = s->getModelCount();
    qDebug() << "TaskCommit:: Model Size : " << n;
    for (int i = 0; i < n; ++i)
    {
        ModelData::ModelDataBase *d=  s->getModelAt(i);
        ui->caseComboBox->addItem(d->getName());
    }

}

CreateTaskTransferData::Ptr CreatTaskDialog::getCreateTaskTransferDataPtr()
{
    qDebug() << "CreatTaskDialog::getTaskDataPtr()";
    return ptr;
}

void CreatTaskDialog::on_createPushButton_clicked()
{
    if(ui->nameLineEdit->text().isEmpty())
    {
        QMessageBox::about(this, tr(ABOUT), tr(INPUT_NAME_WARNING));
        return;
    }
    ptr = CreateInstance::Instance<CreateTaskTransferData>();

    QDateTime current_time = QDateTime::currentDateTime();
    QString StrCurrentTime = current_time.toString("yyyy-MM-dd hh:mm:ss ddd");
    ptr->setTime(StrCurrentTime);
    ptr->setJobName(ui->nameLineEdit->text());
    ptr->setNodeSize(ui->nodeSpinBox->value());
    ptr->setCpuSize(ui->cpuSpinBox->value());
    ptr->setDistr(ui->textEdit->toPlainText());
    ModelData::ModelDataSingleton *s = ModelData::ModelDataSingleton::getinstance();
    ModelData::ModelDataBase *d=  s->getModelAt(ui->caseComboBox->currentIndex());
    ptr->setUpPath(d->getPath());
    ptr->setModeldata(d);
//    ptr->setCommandLine(ui->commandLineEdit->text());
    this->close();
    this->setResult(1);
//    ptr->_distr =
}
