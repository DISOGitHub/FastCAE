#include "Monitortaskinfo.h"

#include "taskmgm/TaskCommit.h"
#include "TaskMacro.h"
#include "quazip/include/JlCompress.h"
#include "IO/IOConfig.h"
#include "ModelData/modelDataSingleton.h"
#include "ModelData/modelDataBase.h"
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QElapsedTimer>
#include <QWaitCondition>
#include <QMutex>
#include <QMutexLocker>
#include <QThreadPool>


#define TIMER_TIMEOUT   (30*1000)
static QWaitCondition waitcondition;
static QMutex mutex;
using namespace Plugins;
MonitorTaskInfo::MonitorTaskInfo()
{
    qRegisterMetaType<TaskData::Ptr>("TaskData::Ptr");
}

void MonitorTaskInfo::run()
{
    auto tptr = CreateInstance::Instance<Plugins::TaskData>();
    QString mainAddr = Config->GetMainServerName();
    QString userToken = Config->GetUserToken();
    while(!quit)
    {
        mutex.lock();
        while(tasks.isEmpty())
        {
            waitcondition.wait(&mutex);
            if(quit)
            {
                mutex.unlock();
                return;
            }
        }
        for(int i = 0; i < tasks.size(); ++i)
        {
            Plugins::SelectTaskTransferData::Ptr ptr = tasks.at(i);
            MonitorRunnable* runnable = new MonitorRunnable(std::move(ptr));
            connect(runnable, SIGNAL(signalTaskInfo(TaskData::Ptr)), this, SLOT(onTaskData(TaskData::Ptr)));
            QThreadPool::globalInstance()->start(runnable);
        }
        mutex.unlock();
        Sleep(TIMER_TIMEOUT);
    }
}

void MonitorTaskInfo::timerEvent(QTimerEvent *event)
{
//    waitcondition.notify_all();
}

void MonitorTaskInfo::notifyRun()
{
    waitcondition.wakeAll();
}

void MonitorTaskInfo::addMonitorTask(Plugins::SelectTaskTransferData::Ptr &ptr)
{
    QMutexLocker locker(&mutex);
    tasks.append(ptr);
    waitcondition.wakeAll();
    startMonitorTimer();
}

void MonitorTaskInfo::removeMonitorTask(const QString &jobName)
{
    for(int i = 0; i < tasks.size(); ++i)
    {
        Plugins::SelectTaskTransferData::Ptr ptr = tasks.at(i);
        if(jobName == ptr->getJobName())
            tasks.removeAt(i); return;
    }
}

void MonitorTaskInfo::startMonitorTimer()
{
    timerID = this->startTimer(TIMER_TIMEOUT);
}

void Plugins::MonitorTaskInfo::killMonitorTimer()
{

}

bool MonitorTaskInfo::getQuit() const
{
    return quit;
}

void MonitorTaskInfo::setQuit(bool value)
{
    quit = value;
    waitcondition.notify_all();
}

void MonitorTaskInfo::onTaskData(TaskData::Ptr ptr)
{
    TASK_STATE status = ptr->getState();
    switch (status) {
    case TASK_STATE_CALCOMPLETE:
    case TASK_STATE_STOPCOMPLETE:
    case TASK_STATE_FAIL:
        removeMonitorTask(ptr->getTaskName());
        break;
    default:
        break;
    }
    emit signalTaskData(ptr);
}

MonitorRunnable::MonitorRunnable(SelectTaskTransferData::Ptr &&ptr) : ptr(ptr)
{

}

void MonitorRunnable::run()
{
    auto tptr = CreateInstance::Instance<Plugins::TaskData>();
    QString mainAddr = Config->GetMainServerName();
    QString userToken = Config->GetUserToken();
//    Plugins::SelectTaskTransferData::Ptr ptr = tasks.at(i);
    tptr->setTaskName(ptr->getJobName());
    tptr->setJobID(ptr->getJobID());
    QString resBody;
    if(1 != TaskCommitMgr::getRemoteJobInfo(mainAddr, userToken, QString::number(ptr->getJobID()), resBody))
    {
        tptr->setState(TASK_STATE_SELFAILED);
    } else {
        /// 取出结果信息
        QString userId = Config->GetUserId();
        QString status, startTime, endTime, cupCount;
        QJsonParseError err;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(resBody.toUtf8(), &err);
        if (QJsonParseError::NoError == err.error) {
            if (jsonDoc.isObject()) {
                QVariantMap result = jsonDoc.toVariant().toMap();
                status = result["status"].toString();
                startTime = result["startTime"].toString();
                endTime = result["endTime"].toString();
                cupCount = result["slots"].toString();
            }
        }
        TASK_STATE ts = TASK_STATE_CALCING;
        if ("done" == status.toLower())
        {
            ts = TASK_STATE_CALCOMPLETE;
//            qulonglong
            qulonglong usedTime = endTime.toULongLong() - startTime.toULongLong();
            tptr->setUsedCPUTime(MS_TO_HOUR(usedTime)*cupCount.toInt());
            tptr->setCpuSize(cupCount.toInt());
            int ret = TaskCommitMgr::updateCpuSize(userId, ptr->getJobName(), cupCount);
            if (ret != 0) {
                qDebug() << "failed to update core count";
            }
        } else if("prepare" == status.toLower()) {
            ts = TASK_STATE_PREPARE;
        } else if("cancel" == status.toLower()) {
            ts = TASK_STATE_STOPCOMPLETE;
        } else if("fail" == status.toLower()) {
            ts = TASK_STATE_FAIL;
        } else if("pending" == status.toLower()) {
            ts = TASK_STATE_PENDING;
        } else if("suspended" == status.toLower()) {
            //                    ts = TASK_STATE_SUSPENDED;
        } else if("run" == status.toLower()) {
            ts = TASK_STATE_RUN;
        } else if("cancelling" == status.toLower()) {
            ts = TASK_STATE_STOPCOMPLETE;
        } else {
            //                    ts = TASK_STATE_CALCING;
        }
        tptr->setState(ts);
        int ret = TaskCommitMgr::updateTaskInfo(userId, ptr->getJobName(), QString("%1").arg(ptr->getJobID()), QString("%1").arg(ts), startTime, endTime);
        if (ret != 0) {
            qDebug() << "failed to update task info";
        }
    }
    emit signalTaskInfo(tptr);
}

void MonitorRunnable::notifyRun()
{
    waitcondition.notify_all();
}
