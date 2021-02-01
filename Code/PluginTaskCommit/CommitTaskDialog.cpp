#include "CommitTaskDialog.h"
#include "ui_CommitTaskDialog.h"
#include "TaskMacro.h"
#include <QProgressBar>
#include <QDebug>
#include <QMessageBox>

using namespace  Plugins;

CommitTaskDialog::CommitTaskDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommitTaskDialog)
{
    ui->setupUi(this);
//    addItem();
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->addUploadPushButton->hide();
}

CommitTaskDialog::~CommitTaskDialog()
{
    delete ui;
}

void CommitTaskDialog::init()
{
    QString name = "CommitTaskDialog";
    TaskDataObserver::getInstance()->regis(name,std::bind(&CommitTaskDialog::updateTaskData, this, std::placeholders::_1));
//    REGISTER_DATA_FUN(name, std::bind(&CommitTaskDialog::updateTaskData, this, std::placeholders::_1));
}

void CommitTaskDialog::updateTaskData(TaskData::Ptr &&ptr)
{
    qDebug() << "CommitTaskDialog::updataTaskData";
    switch (ptr->getState()) {
    case TASK_STATE_LGN:
        break;
    case TASK_STATE_CREATE:
    {

    }
        break;
    case TASK_STATE_DEL:
        break;
    case TASK_STATE_UCOMMIT:
    {
        addItem(ptr);
    }
        break;
    case TASK_STATE_DRESULT:
        break;
    case TASK_STATE_SEL:
        break;
    case TASK_STATE_CALCING:
    {

    }
        break;
    default:
        break;
    }
}

bool CommitTaskDialog::getRepeatUploadTask(QString name)
{
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i)
    {
        QTableWidgetItem* item = ui->tableWidget->item(0,0);
        if(item->text() == name)
            return true;
    }
    return false;
}

void CommitTaskDialog::addItem(const TaskData::Ptr &ptr)
{
    if(getRepeatUploadTask(ptr->getTaskName()))
    {
        QMessageBox::about(this, tr("prompt"), tr("Task upload already exists"));
        return;
    }
    QTableWidgetItem* nameItem =  new QTableWidgetItem(ptr->getTaskName());
    QTableWidgetItem* sizeItem =  new QTableWidgetItem("");
    QTableWidgetItem* speedItem =  new QTableWidgetItem("");

    QProgressBar *pbar = new QProgressBar;
    pbar->setValue(0);
    pbar->setStyleSheet(
                "QProgressBar {border: 2px solid grey;   border-radius: 5px;"
                "background-color: #FFFFFF;"
                "text-align: center;}"
                "QProgressBar::chunk {background-color: rgb(0,250,0) ;}");

    QTableWidgetItem* stateItem =  new QTableWidgetItem("");
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(row + 1);
    ui->tableWidget->setItem(row, 0, nameItem);
    ui->tableWidget->setItem(row, 1, sizeItem);
    ui->tableWidget->setCellWidget(row, 2, pbar);
    ui->tableWidget->setItem(row, 3, speedItem);
    ui->tableWidget->setItem(row, 4, stateItem);
}

void CommitTaskDialog::updateTranslatorUI()
{
//    QStringList strs = {tr("upFile"), tr("size"), tr("progress"),tr("speed"),tr("describe")};
//    ui->tableWidget->setHorizontalHeaderLabels(strs);
    ui->tableWidget->horizontalHeaderItem(0)->setText(tr("upFile"));
    ui->tableWidget->horizontalHeaderItem(1)->setText(tr("size"));
    ui->tableWidget->horizontalHeaderItem(2)->setText(tr("progress"));
    ui->tableWidget->horizontalHeaderItem(3)->setText(tr("speed"));
    ui->tableWidget->horizontalHeaderItem(4)->setText(tr("describe"));
    ui->quitPushButton->setText(tr("quit"));
    ui->addUploadPushButton->setText(tr("addUpload"));
    ui->clearPushButton->setText(tr("clearCompleted"));
    this->setWindowTitle(tr("TaskUpload"));
}

void CommitTaskDialog::onFileSize(QString fileName, int size)
{
    qDebug() << "onFileSize " << size ;
    QTableWidgetItem* item;
    QList<QTableWidgetItem*> listItem = ui->tableWidget->findItems(fileName,  Qt::MatchExactly);
    foreach (auto& var, listItem) {
        item = ui->tableWidget->item(var->row(), 1);
        item->setText(QString("%1").arg(size));
    }
    qDebug() << "item text " << item->text() ;
}

void CommitTaskDialog::onCommitedSize(QString fileName, int schedule)
{
    qDebug() << "onDownedSize = " << schedule;
    QList<QTableWidgetItem*> listItem = ui->tableWidget->findItems(fileName,  Qt::MatchExactly);
    foreach (auto& var, listItem) {
        QProgressBar *pbar = static_cast<QProgressBar*>( ui->tableWidget->cellWidget(var->row(), 2));
        pbar->setValue(schedule);
    }
}

void CommitTaskDialog::onSpeedSize(QString fileName, QString s)
{
    QTableWidgetItem* item;
    QList<QTableWidgetItem*> listItem = ui->tableWidget->findItems(fileName,  Qt::MatchExactly);
    foreach (auto& var, listItem) {
        item = ui->tableWidget->item(var->row(), 3);
        item->setText(s);
    }
}

void Plugins::CommitTaskDialog::on_clearPushButton_clicked()
{
    int count = ui->tableWidget->rowCount();
    for (int i = count-1; i >= 0; --i) {
        QProgressBar *pbar = static_cast<QProgressBar*>( ui->tableWidget->cellWidget(i, 2));
        if(100 == pbar->value())
        {
            ui->tableWidget->removeRow(i);
        }

    }
}

void Plugins::CommitTaskDialog::on_quitPushButton_clicked()
{
    this->hide();
}
