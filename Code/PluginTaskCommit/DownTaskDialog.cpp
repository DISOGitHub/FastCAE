#include "DownTaskDialog.h"
#include "ui_DownTaskDialog.h"
#include "TaskMacro.h"
#include <QProgressBar>
#include <QDebug>
#include <QMessageBox>

using namespace  Plugins;

DownTaskDialog::DownTaskDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownTaskDialog)
{
    ui->setupUi(this);
//    addItem();
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->addDownPushButton->hide();
}

DownTaskDialog::~DownTaskDialog()
{
    delete ui;
}

void DownTaskDialog::init()
{
    QString name = "DownTaskDialog";
    REGISTER_DATA_FUN("DownTaskDialog", std::bind(&DownTaskDialog::updateTaskData, this, std::placeholders::_1));
}

void DownTaskDialog::updateTaskData(TaskData::Ptr &&ptr)
{
    qDebug() << "DownTaskDialog::updataTaskData";
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
        break;
    case TASK_STATE_DRESULT:
    {
        addItem(ptr);
    }
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

bool DownTaskDialog::removeDownloadTask(QString name)
{
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i)
    {
        QTableWidgetItem* item = ui->tableWidget->item(0,0);
        if(item->text() == name)
            ui->tableWidget->removeRow(i); return true;
    }
    return false;
}

void DownTaskDialog::addItem(const TaskData::Ptr &ptr)
{
    removeDownloadTask(ptr->getTaskName());
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

void DownTaskDialog::updateTranslatorUI()
{
//    QStringList strs = {tr("downFile"), tr("size"), tr("progress"),tr("speed"),tr("describe")};
//    ui->tableWidget->setHorizontalHeaderLabels(strs);
    ui->tableWidget->horizontalHeaderItem(0)->setText(tr("downFile"));
    ui->tableWidget->horizontalHeaderItem(1)->setText(tr("size"));
    ui->tableWidget->horizontalHeaderItem(2)->setText(tr("progress"));
    ui->tableWidget->horizontalHeaderItem(3)->setText(tr("speed"));
    ui->tableWidget->horizontalHeaderItem(4)->setText(tr("describe"));
    ui->pushButton->setText(tr("quit"));
    ui->addDownPushButton->setText(tr("addDown"));
    ui->clearPushButton->setText(tr("clearCompleted"));
    this->setWindowTitle(tr("TaskDownload"));
}

void DownTaskDialog::onFileSize(QString fileName, int size)
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

void DownTaskDialog::onDownedSize(QString fileName, int schedule)
{
    qDebug() << "onDownedSize = " << schedule;
    QList<QTableWidgetItem*> listItem = ui->tableWidget->findItems(fileName,  Qt::MatchExactly);
    foreach (auto& var, listItem) {
        QProgressBar *pbar = static_cast<QProgressBar*>( ui->tableWidget->cellWidget(var->row(), 2));
        pbar->setValue(schedule);
    }
}

void DownTaskDialog::onSpeedSize(QString fileName, QString s)
{
    QTableWidgetItem* item;
    QList<QTableWidgetItem*> listItem = ui->tableWidget->findItems(fileName,  Qt::MatchExactly);
    foreach (auto& var, listItem) {
        item = ui->tableWidget->item(var->row(), 3);
        item->setText(s);
    }
}

void Plugins::DownTaskDialog::on_pushButton_clicked()
{
    this->hide();
}

void Plugins::DownTaskDialog::on_clearPushButton_clicked()
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
