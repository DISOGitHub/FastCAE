#include "TaskMgeWidget.h"
#include "ui_TaskMgeWidget.h"
#include "TaskMacro.h"
#include "TaskData.h"
#include "Dispatcher.h"
#include "TaskThread.h"
#include "taskmgm/TaskCommit.h"
#include "LoginDialog.h"
#include "CreatTaskDialog.h"
#include "DownTaskDialog.h"
#include "CommitTaskDialog.h"
#include "monitortaskinfo.h"

#include "IO/IOConfig.h"
#include "ModelData/modelDataSingleton.h"
#include "ModelData/modelDataBase.h"

#include <QTableWidgetItem>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

template<class K, class V, class M>
void initTaskStat(const K& k, const V& v, M& m)
{
    m[k] = v;
}

namespace Plugins {

QMap<TASK_STATE, QString> gStateMap;

ControlWidget::ControlWidget(QWidget *parent) : QWidget(parent)
{
    commitButton = new QPushButton(tr("commit"));
    calButton = new QPushButton(tr("calc"));
    stopButton = new QPushButton(tr("stop"));
    dowButton = new QPushButton(tr("down"));
    delButton = new QPushButton(tr("delete"));
    updateButton = new QPushButton(tr("query"));
    QString qss = "QPushButton:!enabled{"
                  "background-color: #adadad;"
                  "color: #gray;"
                  "border-width: 1px;"
                  "border-color: #1d8de0;"
                  "border-style: outset;"
                  "border-radius: 2;"
                  "min-width: 80px;"
                  "min-height: 24px;}";
    connect(commitButton, SIGNAL(clicked()), this, SIGNAL(commit_clicked()));
    connect(calButton, SIGNAL(clicked()), this, SIGNAL(cal_clicked()));
    connect(delButton, SIGNAL(clicked()), this, SIGNAL(del_clicked()));
    connect(dowButton, SIGNAL(clicked()), this, SIGNAL(dow_clicked()));
    connect(stopButton, SIGNAL(clicked()), this, SIGNAL(stop_clicked()));
    connect(updateButton, SIGNAL(clicked()), this, SIGNAL(query_clicked()));
    commitButton->setStyleSheet(qss);
    calButton->setStyleSheet(qss);
    delButton->setStyleSheet(qss);
    dowButton->setStyleSheet(qss);
    stopButton->setStyleSheet(qss);
    updateButton->setStyleSheet(qss);
    QHBoxLayout* l = new QHBoxLayout(this);
    l->setMargin(1);
    l->setSpacing(2);
    l->addStretch();
    l->addWidget(commitButton);
    l->addStretch();
    l->addWidget(calButton);
    l->addStretch();
    l->addWidget(stopButton);
    l->addStretch();
    l->addWidget(updateButton);
    l->addStretch();
    l->addWidget(dowButton);
    l->addStretch();
    l->addWidget(delButton);
    this->setLayout(l);
}

void ControlWidget::reTranslate()
{
    commitButton->setText(tr("commit"));
    calButton->setText(tr("calc"));
    stopButton->setText(tr("stop"));
    dowButton->setText(tr("down"));
    delButton->setText(tr("delete"));
    updateButton->setText(tr("query"));
}

void ControlWidget::setEnableState(TASK_STATE state)
{
    /// TODO:优化用状态机替换
    switch (state) {
    case TASK_STATE_LGN:
    {

    }
        break;
    case TASK_STATE_LGNFAILED:
        /// QMessageBox::warning(this, tr(PROMPT_MSS), tr("server login failed, please contact FaseCAE"));
        break;
    case TASK_STATE_CREATE:
    {
        commitButton->setEnabled(true);
        calButton->setEnabled(false);
        delButton->setEnabled(true);
        dowButton->setEnabled(false);
        stopButton->setEnabled(false);
        updateButton->setEnabled(false);
    }
        break;
    case TASK_STATE_CREATED:
    {
        commitButton->setEnabled(true);
        calButton->setEnabled(false);
        delButton->setEnabled(true);
        dowButton->setEnabled(false);
        stopButton->setEnabled(false);
        updateButton->setEnabled(false);
    }
        break;
    case TASK_STATE_CREATECOMPLETE:
    {
        commitButton->setEnabled(true);
        calButton->setEnabled(false);
        delButton->setEnabled(true);
        dowButton->setEnabled(false);
        stopButton->setEnabled(false);
        updateButton->setEnabled(false);
    }
        break;
    case TASK_STATE_CREATEDFAILED:
    {
        commitButton->setEnabled(true);
        calButton->setEnabled(false);
        delButton->setEnabled(true);
        dowButton->setEnabled(false);
        stopButton->setEnabled(false);
        updateButton->setEnabled(false);
    }
        break;
    case TASK_STATE_UCOMMIT:
    {
        commitButton->setEnabled(false);
        calButton->setEnabled(true);
        delButton->setEnabled(true);
        dowButton->setEnabled(false);
        stopButton->setEnabled(false);
        updateButton->setEnabled(false);
    }
        break;
    case TASK_STATE_UCOMMITEDCOMPLETE:
    {
        commitButton->setEnabled(false);
        calButton->setEnabled(true);
        delButton->setEnabled(true);
        dowButton->setEnabled(false);
        stopButton->setEnabled(false);
        updateButton->setEnabled(false);
    }
        break;
    case TASK_STATE_UCOMMITEDFAILED:
    {
        commitButton->setEnabled(true);
        calButton->setEnabled(false);
        delButton->setEnabled(true);
        dowButton->setEnabled(false);
        stopButton->setEnabled(false);
        updateButton->setEnabled(false);
    }
        break;
    case TASK_STATE_PENDING:
    case TASK_STATE_RUN:
    case TASK_STATE_CALCING:
    {
        commitButton->setEnabled(false);
        calButton->setEnabled(false);
        delButton->setEnabled(false);
        dowButton->setEnabled(false);
        stopButton->setEnabled(true);
        updateButton->setEnabled(true);
        stopButton->setVisible(true);
        calButton->hide();
    }
        break;
    case TASK_STATE_CALCOMPLETE:
    {
        commitButton->setEnabled(false);
        calButton->setEnabled(false);
        delButton->setEnabled(false);
        dowButton->setEnabled(true);
        stopButton->setEnabled(false);
        updateButton->setEnabled(false);
    }
        break;
    case TASK_STATE_FAIL:
    case TASK_STATE_CALCFAILED:
    {
        commitButton->setEnabled(false);
        calButton->setEnabled(true);
        delButton->setEnabled(true);
        dowButton->setEnabled(false);
        stopButton->setEnabled(false);
        updateButton->setEnabled(false);
        stopButton->hide();
        calButton->setVisible(true);
    }
        break;
    case TASK_STATE_DRESULT:
    {
        commitButton->setEnabled(false);
        calButton->setEnabled(false);
        delButton->setEnabled(true);
        dowButton->setEnabled(true);
        stopButton->setEnabled(false);
        updateButton->setEnabled(false);
    }
        break;
    case TASK_STATE_DRESULTFAILED:
    {
        commitButton->setEnabled(false);
        calButton->setEnabled(false);
        delButton->setEnabled(false);
        dowButton->setEnabled(true);
        stopButton->setEnabled(false);
        updateButton->setEnabled(false);
    }
        break;
    case TASK_STATE_DRESULTCOMPLETE:
    {
        commitButton->setEnabled(false);
        calButton->setEnabled(false);
        delButton->setEnabled(false);
        dowButton->setEnabled(false);
        stopButton->setEnabled(false);
        updateButton->setEnabled(false);
    }
        break;
    case TASK_STATE_SEL:
    {

    }
        break;
    case TASK_STATE_SELFAILED:
    {
        commitButton->setEnabled(false);
        calButton->setEnabled(false);
        delButton->setEnabled(true);
        dowButton->setEnabled(false);
        stopButton->setEnabled(true);
        updateButton->setEnabled(true);
    }
        break;
    case TASK_STATE_SELCOMPLETE:
    {
        commitButton->setEnabled(false);
        calButton->setEnabled(false);
        delButton->setEnabled(true);
        dowButton->setEnabled(false);
        stopButton->setEnabled(false);
        updateButton->setEnabled(true);
    }
        break;
    case TASK_STATE_STOP:
    {

    }
        break;
    case TASK_STATE_STOPFAILED:
    {
        commitButton->setEnabled(false);
        calButton->setEnabled(false);
        delButton->setEnabled(false);
        dowButton->setEnabled(false);
        stopButton->setEnabled(true);
        updateButton->setEnabled(true);
        stopButton->setVisible(true);
        calButton->hide();
    }
        break;
    case TASK_STATE_CANCEL:
    case TASK_STATE_STOPCOMPLETE:
    {
        commitButton->setEnabled(false);
        calButton->setEnabled(true);
        delButton->setEnabled(false);
        dowButton->setEnabled(false);
        stopButton->setEnabled(false);
        updateButton->setEnabled(false);
        calButton->setVisible(true);
        stopButton->hide();
    }
        break;
    case TASK_STATE_PREPARE:
    {
        //			delButton->setEnabled(true);
    }
        break;
    case TASK_STATE_DEL:
        break;
    case TASK_STATE_DELFAILED:
    {
        delButton->setEnabled(true);
    }
        break;
    case TASK_STATE_DELCOMPLETE:
        break;
    default:
        break;
    }
}

QPushButton *ControlWidget::getCommitButton() const
{
    return commitButton;
}

void ControlWidget::setCommitButton(QPushButton *value)
{
    commitButton = value;
}

QPushButton *ControlWidget::getCalButton() const
{
    return calButton;
}

void ControlWidget::setCalButton(QPushButton *value)
{
    calButton = value;
}

QPushButton *ControlWidget::getStopButton() const
{
    return stopButton;
}

void ControlWidget::setStopButton(QPushButton *value)
{
    stopButton = value;
}

QPushButton *ControlWidget::getDowButton() const
{
    return dowButton;
}

void ControlWidget::setDowButton(QPushButton *value)
{
    dowButton = value;
}

QPushButton *ControlWidget::getDelButton() const
{
    return delButton;
}

void ControlWidget::setDelButton(QPushButton *value)
{
    delButton = value;
}

QPushButton *ControlWidget::getUpdateButton() const
{
    return updateButton;
}

void ControlWidget::setUpdateButton(QPushButton *value)
{
    updateButton = value;
}


TaskMgeWidget::TaskMgeWidget(GUI::MainWindow*parent) :
    QWidget(nullptr),
    downDialog(new DownTaskDialog),
    commitDialog(new CommitTaskDialog),
    ui(new Ui::TaskMgeWidget),_mainWindow(parent)
{
    //    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);    //自适应宽度
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    initAllTaskState();

    QString mainAddr, userAddr, userName, hashKey;
    mainAddr = Config->GetMainServerName();
    userAddr = Config->GetUserServerName();
    userName = Config->GetUserName();
    hashKey = Config->GetHashKey();
    ///qDebug() << mainAddr << userAddr << userName << hashKey;
    TaskCommitMgr::loginServer(mainAddr, userAddr, userName, hashKey);
    this->setWindowTitle(tr("TaskMge"));
}

TaskMgeWidget::~TaskMgeWidget()
{
    qDebug() << "Destroy TaskMgeWidget";
    /** 唤醒监控线程使其正确退出 */
    if(nullptr != monitorRunable)
    {
        monitorRunable->setQuit(true);
        monitorRunable->notifyRun();
        monitorRunable->wait(1000);
        disconnect(monitorRunable, SIGNAL(signalTaskData(TaskData::Ptr)), this, SLOT(onQuertInfo(TaskData::Ptr)));
        monitorRunable->exit();
        monitorRunable->terminate();
        monitorRunable->deleteLater();
    }
    downDialog->deleteLater();
    commitDialog->deleteLater();
    delete ui;
    emit taskMgeWidgetDestoryed();
}

void TaskMgeWidget::init()
{
    QString name = "TaskMgeWidget";
    REGISTER_DATA_FUN(name, std::bind(&TaskMgeWidget::updataTaskData, this, std::placeholders::_1));
    downDialog->init();
    commitDialog->init();
    monitorRunable = new MonitorTaskInfo;
    monitorRunable->start();
    qRegisterMetaType<TaskData::Ptr>("TaskData::Ptr");
    connect(monitorRunable, SIGNAL(signalTaskData(TaskData::Ptr)), this, SLOT(onQuertInfo(TaskData::Ptr)), Qt::QueuedConnection);
    ui->cupTimeLabel->setText(QString::number(Config->getAvailableTime()));
}

void TaskMgeWidget::initAllTaskState()
{
    initTaskStat(TASK_STATE_CREATE, tr("WaitCommit"), gStateMap);
    initTaskStat(TASK_STATE_CREATECOMPLETE, tr("WaitCommit"), gStateMap);
    initTaskStat(TASK_STATE_UCOMMIT, tr("Commiting"), gStateMap);
    initTaskStat(TASK_STATE_UCOMMITEDFAILED, tr("CommitedFailed"), gStateMap);
    initTaskStat(TASK_STATE_UCOMMITEDCOMPLETE, tr("WaitCalc"), gStateMap);
    initTaskStat(TASK_STATE_CALCING, tr("Calcing"), gStateMap);
    initTaskStat(TASK_STATE_CALCFAILED, tr("CalcingFiled"), gStateMap);
    initTaskStat(TASK_STATE_CALCOMPLETE, tr("WaitDown"), gStateMap);
    initTaskStat(TASK_STATE_DRESULT, tr("Downloading"), gStateMap);
    initTaskStat(TASK_STATE_DRESULTFAILED, tr("DownloadFailed"), gStateMap);
    initTaskStat(TASK_STATE_DRESULTCOMPLETE, tr("Downloaded"), gStateMap);
    initTaskStat(TASK_STATE_STOP, tr("Cancel"), gStateMap);
    initTaskStat(TASK_STATE_STOPFAILED, tr("CancelFiled"), gStateMap);
    initTaskStat(TASK_STATE_STOPCOMPLETE, tr("Canceled"), gStateMap);
    initTaskStat(TASK_STATE_DEL, tr("Delete"), gStateMap);
    initTaskStat(TASK_STATE_DELFAILED, tr("DeleteFiled"), gStateMap);
    initTaskStat(TASK_STATE_DELCOMPLETE, tr("Deleted"), gStateMap);

    initTaskStat(TASK_STATE_PREPARE, tr("Prepare"), gStateMap);
    initTaskStat(TASK_STATE_PENDING, tr("Pending"), gStateMap);
    //        initTaskStat(TASK_STATE_CANCEL, tr("Deleted"), gStateMap);
    initTaskStat(TASK_STATE_FAIL, tr("Fail"), gStateMap);
    initTaskStat(TASK_STATE_SUSPENDED, tr("Suspended"), gStateMap);
    initTaskStat(TASK_STATE_RUN, tr("Run"), gStateMap);
    initTaskStat(TASK_STATE_CANCELLING, tr("Cancelling"), gStateMap);
    initTaskStat(TASK_STATE_UNKNOWN, tr("Unknown"), gStateMap);

}

void TaskMgeWidget::updataTaskData(TaskData::Ptr &&ptr)
{
    qDebug() << "TaskMgeWidget::updataTaskData";
    {
        ControlWidget* clt = getControlWidgetFromTable(ptr->getTaskName());
        if (nullptr != clt)
        {
            clt->setEnableState(ptr->getState());
        }
    }
    switch (ptr->getState()) {
    case TASK_STATE_LGN:
        break;
    case TASK_STATE_LGNFAILED:
        //        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, tr("server login failed, please contact FaseCAE"));
        //        QMessageBox::warning(nullptr, tr(PROMPT_MSS), tr("server login failed, please contact FaseCAE"));
        break;
    case TASK_STATE_CREATE:
    {
        ///addItem(ptr);
    }
        break;
    case TASK_STATE_CREATED:
    {
        TaskData p = *(ptr.get());///setState(ptr->getStoreState());
        p.setState(ptr->getStoreState());

        addItem(&p);
        addMonitor(p);
        ControlWidget* clt = getControlWidgetFromTable(ptr->getTaskName());
        if (nullptr != clt)
        {
            clt->setEnableState(p.getState());
        }
        setItemState(ptr->getTaskName(), (TASK_STATE)p.getState());
        updataUsedTime(ptr->getTaskName(), QString("%1").arg(ptr->getUsedCPUTime()));
    }
        break;
    case TASK_STATE_CREATECOMPLETE:
    {
        addItem(ptr);
        setItemState(ptr->getTaskName(), (TASK_STATE)ptr->getState());
        updataUsedTime(ptr->getTaskName(), QString("%1").arg(ptr->getUsedCPUTime()));
        ControlWidget* clt = getControlWidgetFromTable(ptr->getTaskName());
        if (nullptr != clt)
        {
            clt->setEnableState(ptr->getState());
        }
        QString msg = ptr->getTaskName() + ":" + tr("created task");
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, msg);
        //        QMessageBox::about(nullptr, tr(ABOUT), tr("created task"));
    }
        break;
    case TASK_STATE_CREATEDFAILED:
    {
        ///setItemState(ptr->getTaskName(), (TASK_STATE)ptr->getState());
        QString msg = ptr->getTaskName() + ":" + tr("failed to create a task");
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, msg);
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, tr("Warningt:No permission or duplicate name"));
        //        QMessageBox::about(nullptr, tr(ABOUT), tr("failed to create a task"));
    }
        break;
    case TASK_STATE_DEL:
    {

    }
        break;
    case TASK_STATE_DELFAILED:
    {
        setItemState(ptr->getTaskName(), (TASK_STATE)ptr->getState());
        QString msg = ptr->getTaskName() + ":" + tr("failed to delete a task");
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, msg);
        //QMessageBox::about(this, tr("about"), tr("failed to delete a task"));
    }
        break;
    case TASK_STATE_DELCOMPLETE:
    {
        setItemState(ptr->getTaskName(), (TASK_STATE)ptr->getState());
        removwItemFromTaskName(ptr->getTaskName());
        QString msg = ptr->getTaskName() + ":" + tr("task deleted");
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, msg);
        //        QMessageBox::about(nullptr, tr(ABOUT), tr("task deleted"));
    }
        break;
    case TASK_STATE_UCOMMIT:
    {

    }
        break;
    case TASK_STATE_UCOMMITEDFAILED:
    {
        setItemState(ptr->getTaskName(), (TASK_STATE)ptr->getState());
        QString msg = ptr->getTaskName() + ":" + tr("failed to upload task file");
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, msg);
        //        QMessageBox::about(nullptr, tr(ABOUT), tr("failed to upload task file"));
    }
        break;
    case TASK_STATE_UCOMMITEDCOMPLETE:
    {
        setItemState(ptr->getTaskName(), (TASK_STATE)ptr->getState());
        QString msg = ptr->getTaskName() + ":" + tr("upload task file successfully");
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, msg);
        ///QMessageBox::about(nullptr, tr(ABOUT), tr("upload task file successfully"));
    }
        break;
    case TASK_STATE_CALC:
    {

    }
        break;
    case TASK_STATE_CALCING:
    {
        setItemState(ptr->getTaskName(), (TASK_STATE)ptr->getState());
        QString msg = ptr->getTaskName() + ":" + tr("submited tasks");
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, msg);
        /// QMessageBox::about(this, tr(ABOUT), tr("submited tasks"));
    }
        break;
    case TASK_STATE_CALCFAILED:
    {
        setItemState(ptr->getTaskName(), (TASK_STATE)ptr->getState());
        QString msg = ptr->getTaskName() + ":" + tr("failed to submit tasks");
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, msg);
        //        QMessageBox::about(nullptr, tr(ABOUT), tr("failed to submit tasks"));
    }
        break;
    case TASK_STATE_CALCOMPLETE:
    {
        setItemState(ptr->getTaskName(), (TASK_STATE)ptr->getState());
        updataUsedTime(ptr->getTaskName(), QString("%1").arg(ptr->getUsedCPUTime()));
        QString msg = ptr->getTaskName() + ":" + tr("submit the task successfully");
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, msg);
        //        QMessageBox::about(nullptr, tr(ABOUT), tr("submit the task successfully"));
    }
        break;
    case TASK_STATE_DRESULT:
    {

    }
        break;
    case TASK_STATE_DRESULTFAILED:
    {
        QString msg = ptr->getTaskName() + ":" + tr("failed to download the result file");
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, msg);
        //        QMessageBox::about(nullptr, tr(ABOUT), tr("failed to download the result file"));
    }
        break;
    case TASK_STATE_DRESULTCOMPLETE:
    {
        QString msg = ptr->getTaskName() + ":" + tr("downloaded the result file successfully");
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, msg);
        //        QMessageBox::about(nullptr, tr(ABOUT), tr("downloaded the result file successfully"));
    }
        break;
    case TASK_STATE_SEL:
    {
        addMonitor(ptr);
    }
        break;
    case TASK_STATE_SELFAILED:
    {
        QString msg = ptr->getTaskName() + ":" + tr("failed to refresh the task");
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, msg);
        //       QMessageBox::about(nullptr, tr(ABOUT), tr("failed to refresh the task"));
    }
        break;
    case TASK_STATE_SELCOMPLETE:
    {
        QString msg = ptr->getTaskName() + ":" + tr("refresh the task successfully");
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, msg);
        //       QMessageBox::about(nullptr, tr(ABOUT), tr("refresh the task successfully"));
    }
        break;
    case TASK_STATE_STOP:

        break;
    case TASK_STATE_STOPFAILED:
    {
        QString msg = ptr->getTaskName() + ":" + tr("failed to cancel the task");
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, msg);
        //       QMessageBox::about(nullptr, tr(ABOUT), tr("failed to cancel the task"));
    }
        break;
    case TASK_STATE_STOPCOMPLETE:
    {
        setItemState(ptr->getTaskName(), (TASK_STATE)ptr->getState());
        QString msg = ptr->getTaskName() + ":" + tr("cancel the task successfully");
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, msg);
        //        QMessageBox::about(nullptr, tr(ABOUT), tr("cancel the task successfully"));
    }
        break;
    case TASK_STATE_PREPARE:
    {
        setItemState(ptr->getTaskName(), (TASK_STATE)ptr->getState());
        QString msg = ptr->getTaskName() + ":" + tr("the mission is in preparation");
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, msg);
        //        QMessageBox::about(nullptr, tr(ABOUT), tr("cancel the task successfully"));
    }
        break;
    case TASK_STATE_CANCEL:
    {
        //            setItemState(ptr->getTaskName(), (TASK_STATE)ptr->getState());
        //            Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, tr("cancel the task successfully"));
        //        QMessageBox::about(nullptr, tr(ABOUT), tr("cancel the task successfully"));
    }
        break;
    case TASK_STATE_FAIL:
    {
        setItemState(ptr->getTaskName(), (TASK_STATE)ptr->getState());
        QString msg = ptr->getTaskName() + ":" + tr("the task calculation failed");
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, msg);
        //        QMessageBox::about(nullptr, tr(ABOUT), tr("cancel the task successfully"));
    }
        break;
    case TASK_STATE_PENDING:
    {
        setItemState(ptr->getTaskName(), (TASK_STATE)ptr->getState());
        QString msg = ptr->getTaskName() + ":" + tr("the Task pending");
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, msg);
        //        QMessageBox::about(nullptr, tr(ABOUT), tr("cancel the task successfully"));
    }
        break;
    case TASK_STATE_SUSPENDED:
    {
        setItemState(ptr->getTaskName(), (TASK_STATE)ptr->getState());
        QString msg = ptr->getTaskName() + ":" + tr("the Task suspended");
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, msg);
        //        QMessageBox::about(nullptr, tr(ABOUT), tr("cancel the task successfully"));
    }
        break;
    case TASK_STATE_RUN:
        //        {
        //            setItemState(ptr->getTaskName(), (TASK_STATE)ptr->getState());
        //            Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, tr("cancel the task successfully"));
        //            //        QMessageBox::about(nullptr, tr(ABOUT), tr("cancel the task successfully"));
        //        }
        //        break;
    case TASK_STATE_CANCELLING:
    {
        setItemState(ptr->getTaskName(), (TASK_STATE)ptr->getState());
        QString msg = ptr->getTaskName() + ":" + tr("the task is calculating");
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, msg);
        //        QMessageBox::about(nullptr, tr(ABOUT), tr("cancel the task successfully"));
    }
        break;
    case TASK_STATE_UNKNOWN:
    {
        setItemState(ptr->getTaskName(), (TASK_STATE)ptr->getState());
        QString msg = ptr->getTaskName() + ":" + tr("unknown task information");
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, msg);
        //        QMessageBox::about(nullptr, tr(ABOUT), tr("cancel the task successfully"));
    }
        break;
    default:
        break;
    }
}

void TaskMgeWidget::commitTask(CommitRunable *cit)
{

    connect(cit, SIGNAL(signalFileSize(QString, int)), commitDialog, SLOT(onFileSize(QString, int)), Qt::QueuedConnection);
    connect(cit, SIGNAL(signalCommitedSize(QString, int)), commitDialog, SLOT(onCommitedSize(QString, int)), Qt::QueuedConnection);
    connect(cit, SIGNAL(signalSpeedSize(QString, QString)), commitDialog, SLOT(onSpeedSize(QString, QString)), Qt::QueuedConnection);
    connect(cit, SIGNAL(signalCommitState(QString, int)), this, SLOT(onCommitState(QString, int)), Qt::QueuedConnection);

    CommitTaskTransferData::Ptr ptr = CreateInstance::Instance<CommitTaskTransferData>(cit);
    TRANSFER_DATA_FUN(ptr, TASK_STATE_UCOMMIT);
}

void TaskMgeWidget::calcTask(ExecuteRunable *exec)
{
    connect(exec, SIGNAL(signalExecuteState(QString, int, int)), this, SLOT(onExecuteState(QString, int, int)), Qt::QueuedConnection);
    connect(exec, SIGNAL(signalExecuteJobID(QString, int)), this, SLOT(onExecuteJobID(QString, int)), Qt::QueuedConnection);
    ExecTaskTransferData::Ptr ptr = CreateInstance::Instance<ExecTaskTransferData>(exec);
    TRANSFER_DATA_FUN(ptr, TASK_STATE_CALC);
}

void TaskMgeWidget::downTask(ResultRunable *result)
{
    connect(result, SIGNAL(signalFileSize(QString, int)), downDialog, SLOT(onFileSize(QString, int)), Qt::QueuedConnection);
    connect(result, SIGNAL(signalDownedSize(QString, int)), downDialog, SLOT(onDownedSize(QString, int)), Qt::QueuedConnection);
    connect(result, SIGNAL(signalSpeedSize(QString, QString)), downDialog, SLOT(onSpeedSize(QString, QString)), Qt::QueuedConnection);
    connect(result, SIGNAL(signalDownState(QString, int)), this, SLOT(onDownState(QString, int)), Qt::QueuedConnection);

    DownTaskTransferData::Ptr&& ptr = CreateInstance::Instance<DownTaskTransferData>(result);
    TRANSFER_DATA_FUN(ptr, TASK_STATE_DRESULT);
}

void TaskMgeWidget::updateTranslatorUI()
{
    //    ui->tableWidget->setHorizontalHeaderLabels(header);
    this->setWindowTitle(tr("TaskMge"));
    ui->tableWidget->horizontalHeaderItem(0)->setText(tr("name"));
    ui->tableWidget->horizontalHeaderItem(1)->setText(tr("time"));
    ui->tableWidget->horizontalHeaderItem(2)->setText(tr("state"));
    ui->tableWidget->horizontalHeaderItem(3)->setText(tr("control"));
    ui->tableWidget->horizontalHeaderItem(4)->setText(tr("usedTime"));
    ui->tableWidget->horizontalHeaderItem(5)->setText(tr("distribution"));
    ui->PushButton->setText(tr("login"));
    ui->label->setText(tr("AvailableTime:"));
    ui->ctkPushButton->setText(tr("CreateTask"));
    ui->commitPushButton->setText(tr("uploadList"));
    ui->resultDownPushButton->setText(tr("downList"));
    ui->clearPushButton->setText(tr("Clear"));
    downDialog->updateTranslatorUI();
    commitDialog->updateTranslatorUI();
    initAllTaskState();
    for(int i = 0; i < ui->tableWidget->rowCount(); ++i)
    {
        ControlWidget* clw = static_cast<ControlWidget*>(ui->tableWidget->cellWidget(i, 3));
        clw->reTranslate();
        QTableWidgetItem* item = ui->tableWidget->item(i, 2);
        TASK_STATE s = (TASK_STATE)item->data(Qt::UserRole + 5).toInt();
        item->setText(gStateMap[s]);
    }
}

void TaskMgeWidget::addMonitor(const TaskData::Ptr &ptr)
{
    switch (ptr->getState()) {
    case TASK_STATE_SEL:
    case TASK_STATE_CALCING:
    case TASK_STATE_PENDING:
    case TASK_STATE_RUN:
    {
        SelectTaskTransferData::Ptr sptr = CreateInstance::Instance<SelectTaskTransferData>();
        sptr->setJobName(ptr->getTaskName());
        sptr->setJobID(ptr->getJobID());
        monitorRunable->addMonitorTask(sptr);
    }
        break;
    default:
        break;
    }
}

void TaskMgeWidget::addMonitor(const TaskData &p)
{
    switch (p.getState()) {
    case TASK_STATE_SEL:
    case TASK_STATE_CALCING:
    case TASK_STATE_PENDING:
    case TASK_STATE_RUN:
    {
        SelectTaskTransferData::Ptr ptr = CreateInstance::Instance<SelectTaskTransferData>();
        ptr->setJobName(p.getTaskName());
        ptr->setJobID(p.getJobID());
        monitorRunable->addMonitorTask(ptr);
    }
        break;
    default:
        break;
    }
}

void TaskMgeWidget::updateTableWidget(const TaskStore::Ptr& ptr)
{
    for (int i = 0; i < ptr->getTaskDataSize(); ++i)
    {
        const TaskData::Ptr& tptr = ptr->getTaskData(i);
        addItem(tptr);
    }
}

void TaskMgeWidget::setItemState(const QString &jobName, TASK_STATE s)
{
    QList<QTableWidgetItem*> listItem = ui->tableWidget->findItems(jobName, Qt::MatchExactly);
    foreach(auto& var, listItem) {
        QTableWidgetItem* item = ui->tableWidget->item(var->row(), 2);
        qDebug() << "State:" << gStateMap[s];
        item->setText(gStateMap[s]);
        QVariant stateVar(s);
        item->setData(Qt::UserRole + 5, stateVar);
    }
}

void TaskMgeWidget::updataUsedTime(const QString &jobName, const QString &time)
{
    QList<QTableWidgetItem*> listItem = ui->tableWidget->findItems(jobName, Qt::MatchExactly);
    foreach(auto& var, listItem) {
        QTableWidgetItem* item = ui->tableWidget->item(var->row(), 4);
        item->setText(time);
    }
}

ControlWidget *TaskMgeWidget::getControlWidgetFromTable(QString taskName)
{
    qDebug() << "onExecuteState";
    QList<QTableWidgetItem*> listItem = ui->tableWidget->findItems(taskName, Qt::MatchExactly);
    qDebug() << "onExecuteState " << listItem.size();
    foreach(auto& var, listItem)
    {
        return static_cast<ControlWidget*>(ui->tableWidget->cellWidget(var->row(), 3));
    }
    return nullptr;
}

void TaskMgeWidget::removwItemFromTaskName(QString name)
{
    qDebug() << "onExecuteState";
    QList<QTableWidgetItem*> listItem = ui->tableWidget->findItems(name, Qt::MatchExactly);
    qDebug() << "onExecuteState " << listItem.size();
    foreach(auto& var, listItem)
    {
        ui->tableWidget->removeRow(var->row());
    }
}

void TaskMgeWidget::clearTask()
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->clearContents();
}


void TaskMgeWidget::addItem(const TaskData::Ptr& ptr)
{
    QTableWidgetItem* nameItem = new QTableWidgetItem(ptr->getTaskName());
    QVariant varUpPath(ptr->getUpLoadPath());
    nameItem->setData(Qt::UserRole + 1, varUpPath);
    QVariant varDownPath(ptr->getDownLoadPath());
    nameItem->setData(Qt::UserRole + 2, varDownPath);
    QVariant jobID(ptr->getJobID());
    qDebug() << "addItem job id = " << ptr->getJobID();
    nameItem->setData(Qt::UserRole + 3, jobID);
    QTableWidgetItem* timeItem = new QTableWidgetItem(ptr->getCreateTime());
    QTableWidgetItem* statItem = new QTableWidgetItem(gStateMap[ptr->getState()]);
    ///ptr->setState(ptr->getState()); /// 任务状态 例如表示等待上传、计算中
    float usedTime = ptr->getUsedCPUTime();
    ui->cupTimeLabel->setText(QString("%1").arg(ui->cupTimeLabel->text().toFloat()-usedTime));
    QTableWidgetItem* cpuTimeItem = new QTableWidgetItem(QString("%1").arg(ptr->getUsedCPUTime()));
    QTableWidgetItem* distrItem = new QTableWidgetItem(ptr->getDistr());
    ControlWidget* clw = new ControlWidget;
    connect(clw, SIGNAL(commit_clicked()), this, SLOT(onCommitClick()));
    connect(clw, SIGNAL(cal_clicked()), this, SLOT(onCalClick()));
    connect(clw, SIGNAL(del_clicked()), this, SLOT(onDelClick()));
    connect(clw, SIGNAL(dow_clicked()), this, SLOT(onDowClick()));
    connect(clw, SIGNAL(stop_clicked()), this, SLOT(onStopClick()));
    connect(clw, SIGNAL(query_clicked()), this, SLOT(onQueryClick()));
    clw->getStopButton()->hide();
    //    clw->setEnabled(ptr->getState());
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(row + 1);
    ui->tableWidget->setItem(row, 0, nameItem);
    ui->tableWidget->setItem(row, 1, timeItem);
    ui->tableWidget->setItem(row, 2, statItem);
    ui->tableWidget->setCellWidget(row, 3, clw);
    ui->tableWidget->setItem(row, 4, cpuTimeItem);
    ui->tableWidget->setItem(row, 5, distrItem);
    setItemState(ptr->getTaskName(), ptr->getState());
    ControlWidget* clt = getControlWidgetFromTable(ptr->getTaskName());
    if (nullptr != clt)
    {
        clt->setEnableState(ptr->getState());
    }
}

void TaskMgeWidget::addItem(const TaskData *ptr)
{
    QTableWidgetItem* nameItem = new QTableWidgetItem(ptr->getTaskName());
    QVariant varUpPath(ptr->getUpLoadPath());
    nameItem->setData(Qt::UserRole + 1, varUpPath);
    QVariant varDownPath(ptr->getDownLoadPath());
    nameItem->setData(Qt::UserRole + 2, varDownPath);
    QVariant jobID(ptr->getJobID());
    qDebug() << "addItem job id = " << ptr->getJobID();
    nameItem->setData(Qt::UserRole + 3, jobID);
    QTableWidgetItem* timeItem = new QTableWidgetItem(ptr->getCreateTime());
    QTableWidgetItem* statItem = new QTableWidgetItem(gStateMap[ptr->getState()]);
    ///ptr->setState(ptr->getState()); /// 任务状态 例如表示等待上传、计算中
    float usedTime = ptr->getUsedCPUTime();
    ui->cupTimeLabel->setText(QString("%1").arg(ui->cupTimeLabel->text().toFloat()-usedTime));
    QTableWidgetItem* cpuTimeItem = new QTableWidgetItem(QString("%1").arg(ptr->getUsedCPUTime()));
    QTableWidgetItem* distrItem = new QTableWidgetItem(ptr->getDistr());
    ControlWidget* clw = new ControlWidget;
    connect(clw, SIGNAL(commit_clicked()), this, SLOT(onCommitClick()));
    connect(clw, SIGNAL(cal_clicked()), this, SLOT(onCalClick()));
    connect(clw, SIGNAL(del_clicked()), this, SLOT(onDelClick()));
    connect(clw, SIGNAL(dow_clicked()), this, SLOT(onDowClick()));
    connect(clw, SIGNAL(stop_clicked()), this, SLOT(onStopClick()));
    connect(clw, SIGNAL(query_clicked()), this, SLOT(onQueryClick()));
    clw->getStopButton()->hide();
    //    clw->setEnabled(ptr->getState());
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(row + 1);
    ui->tableWidget->setItem(row, 0, nameItem);
    ui->tableWidget->setItem(row, 1, timeItem);
    ui->tableWidget->setItem(row, 2, statItem);
    ui->tableWidget->setCellWidget(row, 3, clw);
    ui->tableWidget->setItem(row, 4, cpuTimeItem);
    ui->tableWidget->setItem(row, 5, distrItem);
    setItemState(ptr->getTaskName(), ptr->getState());
    ControlWidget* clt = getControlWidgetFromTable(ptr->getTaskName());
    if (nullptr != clt)
    {
        clt->setEnableState(ptr->getState());
    }
}

void TaskMgeWidget::onCommitClick()
{
    if (!TaskCommitMgr::isLoginSucceed())
    {
        QMessageBox::warning(this, tr(PROMPT_MSS), tr(NOT_LOGIN));
        return;
    }
    ControlWidget *senderObj = qobject_cast<ControlWidget*>(sender());
    if (senderObj == nullptr)
    {
        return;
    }
    senderObj->getCommitButton()->setEnabled(false);
    int row = -1;
    TABLE_CUR_ROW(row, ControlWidget);
    QTableWidgetItem* item = ui->tableWidget->item(row, 0);
    CommitRunable* cit = new CommitRunable;
    cit->setJobName(item->text());
    cit->setFilePath(item->data(Qt::UserRole + 1).toString());
    commitTask(cit);
    commitDialog->show();

}

void TaskMgeWidget::onCalClick()
{
    if (!TaskCommitMgr::isLoginSucceed())
    {
        QMessageBox::warning(this, tr(PROMPT_MSS), tr(NOT_LOGIN));
        return;
    }
    int row = -1;
    TABLE_CUR_ROW(row, ControlWidget);
    QTableWidgetItem* item = ui->tableWidget->item(row, 0);
    ExecuteRunable* exec = new ExecuteRunable;
    exec->setJobName(item->text());
    calcTask(exec);
    ControlWidget *senderObj = qobject_cast<ControlWidget*>(sender());
    if (senderObj == nullptr)
    {
        return;
    }
    senderObj->getCalButton()->hide();
    senderObj->getStopButton()->setVisible(true);

}

void TaskMgeWidget::onDelClick()
{
    if (!TaskCommitMgr::isLoginSucceed())
    {
        QMessageBox::warning(nullptr, tr(PROMPT_MSS), tr(NOT_LOGIN));
        return;
    }
    ControlWidget *senderObj = qobject_cast<ControlWidget*>(sender());
    if (senderObj == nullptr)
    {
        return;
    }
    senderObj->getDelButton()->setEnabled(false);
    int row = -1;
    TABLE_CUR_ROW(row, ControlWidget);
    QTableWidgetItem* item = ui->tableWidget->item(row, 0);
    deleteTaskTransferData::Ptr ptr = CreateInstance::Instance<deleteTaskTransferData>();
    ptr->setJobName(item->text());
    TRANSFER_DATA_FUN(ptr, TASK_STATE_DEL);
}

void TaskMgeWidget::onDowClick()
{
    if (!TaskCommitMgr::isLoginSucceed())
    {
        QMessageBox::warning(this, tr(PROMPT_MSS), tr(NOT_LOGIN));
        return;
    }
    ControlWidget *senderObj = qobject_cast<ControlWidget*>(sender());
    if (senderObj == nullptr)
    {
        return;
    }
    senderObj->getDowButton()->setEnabled(false);
    int row = -1;
    TABLE_CUR_ROW(row, ControlWidget);
    QTableWidgetItem* item = ui->tableWidget->item(row, 0);
    ResultRunable* resultRunable = new ResultRunable;
    resultRunable->setFilePath(item->data(Qt::UserRole + 2).toString());
    resultRunable->setJobName(item->text());
    downTask(resultRunable);
    downDialog->show();
}

void TaskMgeWidget::onStopClick()
{
    if (!TaskCommitMgr::isLoginSucceed())
    {
        QMessageBox::warning(this, tr(PROMPT_MSS), tr(NOT_LOGIN));
        return;
    }
    ControlWidget *senderObj = qobject_cast<ControlWidget*>(sender());
    if (senderObj == nullptr)
    {
        return;
    }
    senderObj->getStopButton()->hide();
    senderObj->getCalButton()->setVisible(true);
    int row = -1;
    TABLE_CUR_ROW(row, ControlWidget);
    QTableWidgetItem* item = ui->tableWidget->item(row, 0);
    CancelTaskTransferData::Ptr ptr = CreateInstance::Instance<CancelTaskTransferData>();
    ptr->setJobName(item->text());
    qDebug() << "item->data(Qt::UserRole+3).toInt() = " << item->data(Qt::UserRole + 3).toInt();
    ptr->setJobID(item->data(Qt::UserRole + 3).toInt());
    TRANSFER_DATA_FUN(ptr, TASK_STATE_STOP);

}

void TaskMgeWidget::onQueryClick()
{
    if (!TaskCommitMgr::isLoginSucceed())
    {
        QMessageBox::warning(this, tr(PROMPT_MSS), tr(NOT_LOGIN));
        return;
    }
    ControlWidget *senderObj = qobject_cast<ControlWidget*>(sender());
    if (senderObj == nullptr)
    {
        return;
    }
    senderObj->getUpdateButton()->setEnabled(false);
    int row = -1;
    TABLE_CUR_ROW(row, ControlWidget);
    QTableWidgetItem* item = ui->tableWidget->item(row, 0);
    SelectTaskTransferData::Ptr ptr = CreateInstance::Instance<SelectTaskTransferData>();
    ptr->setJobName(item->text());
    qDebug() << "item->data(Qt::UserRole+3).toInt() = " << item->data(Qt::UserRole + 3).toInt();
    ptr->setJobID(item->data(Qt::UserRole + 3).toInt());
    ///monitorRunable->addMonitorTask(ptr);
    TRANSFER_DATA_FUN(ptr, TASK_STATE_SEL);
}

void TaskMgeWidget::onQuertInfo(TaskData::Ptr tptr)
{
    qDebug() << "onQuertInfo";
    updataTaskData(std::move(tptr));
}

void TaskMgeWidget::onExecuteState(QString fileName, int s, int jobid)
{

    qDebug() << "onExecuteState";
    TASK_STATE ts = (TASK_STATE)s;
    TransferData::Ptr ptr = CreateInstance::Instance<TransferData>();
    ptr->setJobName(fileName);
    ptr->setJobID(jobid);
    TRANSFER_DATA_FUN(ptr, ts);
    setItemState(fileName, (TASK_STATE)s);
}

void TaskMgeWidget::onCommitState(QString fileName, int s)
{
    TASK_STATE ts = (TASK_STATE)s;
    TransferData::Ptr ptr = CreateInstance::Instance<TransferData>();
    ptr->setJobName(fileName);
    TRANSFER_DATA_FUN(ptr, ts);
    setItemState(fileName, (TASK_STATE)s);
}

void TaskMgeWidget::onDownState(QString fileName, int s)
{
    TASK_STATE ts = (TASK_STATE)s;
    TransferData::Ptr ptr = CreateInstance::Instance<TransferData>();
    ptr->setJobName(fileName);
    TRANSFER_DATA_FUN(ptr, ts);
    setItemState(fileName, (TASK_STATE)s);
}

void TaskMgeWidget::onKillTimer()
{

}

void TaskMgeWidget::onExecuteJobID(QString fileName, int id)
{
    QList<QTableWidgetItem*> listItem = ui->tableWidget->findItems(fileName, Qt::MatchExactly);
    foreach(auto& var, listItem) {
        qDebug("var->row() = %d", var->row());
        QTableWidgetItem* item = ui->tableWidget->item(var->row(), 0);
        QVariant varID(id);
        item->setData(Qt::UserRole + 3, varID);
    }
}

/// 登录状态使能 控制
void Plugins::TaskMgeWidget::on_PushButton_clicked()
{
    LoginDialog dig;
    if (dig.exec())
    {
        //        CreateTaskTransferData::Ptr bptr = nullptr;
        //        TRANSFER_DATA_FUN(ACTION_LGN, bptr);
    }
}

void TaskMgeWidget::on_ctkPushButton_clicked()
{
    ModelData::ModelDataSingleton *s = ModelData::ModelDataSingleton::getinstance();
    int n = s->getModelCount();
    if (n < 1)
    {
        QMessageBox::about(this, tr("About"), tr("please creat case"));
        return;
    }
    CreatTaskDialog dig(_mainWindow);
    qDebug() << "CreatTaskDialog dig;";
    dig.init();
    if (dig.exec())
    {
        CreateTaskTransferData::Ptr&& bptr = dig.getCreateTaskTransferDataPtr();
        TRANSFER_DATA_FUN(bptr, TASK_STATE_CREATE);
    }
}

void TaskMgeWidget::on_clearPushButton_clicked()
{

}


void TaskMgeWidget::on_resultDownPushButton_clicked()
{
    downDialog->show();
}

}

void Plugins::TaskMgeWidget::on_commitPushButton_clicked()
{
    commitDialog->show();
}
