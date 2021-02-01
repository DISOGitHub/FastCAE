#include "TaskData.h"
#include "TaskMacro.h"
#include "Dispatcher.h"
#include "taskmgm/TaskCommit.h"

#include "IO/IOConfig.h"
#include "ModelData/modelDataSingleton.h"
#include "ModelData/modelDataBase.h"

#include <QDebug>
#include <QThreadPool>
#include <QFile>
#include <QEventLoop>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


using namespace  Plugins;

TaskDataObserver::Ptr TaskDataObserver::_ptr = nullptr;
TaskStore::Ptr TaskStore::_ptr = nullptr;
void TaskDataObserver::init()
{
    REGISTER_DIS_FUN(std::bind(&TaskDataObserver::processAction, this, std::placeholders::_1));
    _implPtr = CreateInstance::Instance<TaskDataObserverImpl>();
    _implPtr->init();
}

void TaskDataObserver::processAction(const TransferData::Ptr& data)
{
    qDebug() <<"TaskDataObserver::processAction";
    _implPtr->processAction(data);
}

void TaskDataObserverImpl::init()
{
    //    REGISTER_DIS_FUN(std::bind(&TaskDataObserver::processAction, this, std::placeholders::_1));
    //    _taskStorePtr = CreateInstance::Instance<TaskStore>();
}

void TaskDataObserverImpl::processAction(const TransferData::Ptr& data)
{
    bool isBroadcast = false;
    TASK_STATE type = data->taskType();
    auto tptr = CreateInstance::Instance<TaskData>();
    switch (type) {
    case TASK_STATE_LGN:
        break;
    case TASK_STATE_LGNFAILED:
        broadcast(tptr);
        break;
    case TASK_STATE_LGNCOMPLETE:
        break;
    case TASK_STATE_CREATE:
    {
        const CreateTaskTransferData::Ptr& dptr = std::static_pointer_cast<CreateTaskTransferData>(data);
        WRITEINPFILE writeOut = IO::IOConfigure::getInputFileWriter("PHengLEI");
        if(nullptr == writeOut)
        {
        }
        ModelData::ModelDataBase * d = dptr->getModeldata();
        QString path = d->getPath();
#if 1
        if(!writeOut(path, d))
        {
            qDebug() << "Writeout File Failed";
            tptr->setState(TASK_STATE_CREATEDFAILED);
            tptr->setTaskName(dptr->getJobName());
            tptr->setCreateTime(dptr->time());
            tptr->setCpuSize(dptr->getCpuSize());
            tptr->setNodeSize(dptr->getNodeSize());
            tptr->setUpLoadPath(dptr->getUpPath());
            tptr->setDownLoadPath(dptr->getDownPath());
            broadcast(tptr);
            return;
        }
#endif
        isBroadcast = true;
        QString userId = Config->GetUserId();
        QString status = QString::number(TASK_STATE_CREATECOMPLETE);
        QString taskName = dptr->getJobName();
        //        QString uploadPath = Config->GetUploadPath();
        //        QString dloadPath = Config->GetDownloadPath();
        START_FILE_SH(path, PHengLEIv3d0);
        int ret = TaskCommitMgr::appendTaskInfo(userId, taskName, status, dptr->getNodeSize(), dptr->getCpuSize(), path, path);
        if(ret == 0)
        {
            tptr->setState(TASK_STATE_CREATECOMPLETE);
        } else {
            tptr->setState(TASK_STATE_CREATEDFAILED);
            QString error = QObject::tr("Create task failed. Error: sql store failed");
            tptr->setErrorInfo(error);
        }

        tptr->setTaskName(taskName);
        tptr->setCreateTime(dptr->time());
        tptr->setCpuSize(dptr->getCpuSize());
        tptr->setNodeSize(dptr->getNodeSize());
        tptr->setUpLoadPath(path);
        tptr->setDownLoadPath(path+"/results");
        //        GetStorePtr->addTaskData(std::move(tptr));
        broadcast(tptr);
        /// 查找路径下的文件，进行打包。打包成功 isBroadcast = true;
    }
        break;
    case    TASK_STATE_CREATED:
    {
        const CreatedTaskTransferData::Ptr& dptr = std::static_pointer_cast<CreatedTaskTransferData>(data);
        tptr->setTaskName(dptr->getJobName());
        tptr->setCreateTime(dptr->time());
        tptr->setCpuSize(dptr->getCpuSize());
        tptr->setNodeSize(dptr->getNodeSize());
        tptr->setUpLoadPath(dptr->getUpPath());
        tptr->setDownLoadPath(dptr->getDownPath());
        tptr->setState(dptr->taskType());
        tptr->setStoreState(dptr->getLoadTasKState());
        tptr->setJobID(dptr->getJobID());
        tptr->setUsedCPUTime(dptr->getUsedTime());
        broadcast(tptr);
    }
        break;
    case TASK_STATE_CREATEDFAILED:
        tptr->setTaskName(data->getJobName());
        tptr->setState(data->taskType());
        broadcast(tptr);
        break;
    case TASK_STATE_CALC:
    {
        isBroadcast = false;
        const ExecTaskTransferData::Ptr& dptr = std::static_pointer_cast<ExecTaskTransferData>(data);
        QThreadPool::globalInstance()->start(dptr->getExecut());
    }
        break;
    case TASK_STATE_CALCING:
    {
        tptr->setTaskName(data->getJobName());
        tptr->setState(data->taskType());
        tptr->setJobID(data->getJobID());
        broadcast(tptr);
        tptr->setState(TASK_STATE_SEL);
        broadcast(tptr);
    }
        break;
    case TASK_STATE_CALCFAILED:
    {
        tptr->setTaskName(data->getJobName());
        tptr->setState(data->taskType());
        broadcast(tptr);
    }
        break;
    case TASK_STATE_CREATECOMPLETE:
    case TASK_STATE_CALCOMPLETE:
    case TASK_STATE_UCOMMITEDCOMPLETE:
    case TASK_STATE_DRESULTCOMPLETE:
    {
        tptr->setTaskName(data->getJobName());
        tptr->setState(data->taskType());
        broadcast(tptr);
    }
        break;
    case TASK_STATE_UCOMMIT:
    {
        isBroadcast = false;
        const CommitTaskTransferData::Ptr& dptr = std::static_pointer_cast<CommitTaskTransferData>(data);
        tptr->setTaskName(dptr->getTaskName());
        tptr->setState(TASK_STATE_UCOMMIT);
        broadcast(tptr);
        QThreadPool::globalInstance()->start(dptr->getCommit());
    }
        break;
    case TASK_STATE_UCOMMITEDFAILED:
    {
        tptr->setTaskName(data->getJobName());
        tptr->setState(data->taskType());
        broadcast(tptr);
    }
        break;
    case TASK_STATE_DRESULT:
    {
        isBroadcast = false;
        const DownTaskTransferData::Ptr& dptr = std::static_pointer_cast<DownTaskTransferData>(data);
        tptr->setTaskName(dptr->getTaskName());
        tptr->setState(TASK_STATE_DRESULT);
        broadcast(tptr);
        QThreadPool::globalInstance()->start(dptr->getResult());
    }
        break;
    case TASK_STATE_DRESULTFAILED:
    {
        tptr->setTaskName(data->getJobName());
        tptr->setState(data->taskType());
        broadcast(tptr);
    }
        break;

    case TASK_STATE_SEL:
    {
        const SelectTaskTransferData::Ptr& dptr = std::static_pointer_cast<SelectTaskTransferData>(data);
        tptr->setTaskName(dptr->getJobName());
        //        tptr->setState(TASK_STATE_SELCOMPLETE);
        tptr->setJobID(dptr->getJobID());
        QString mainAddr = Config->GetMainServerName();
        QString userToken = Config->GetUserToken();
        QString resBody;
        if(1 != TaskCommitMgr::getRemoteJobInfo(mainAddr, userToken, QString::number(tptr->getJobID()), resBody))
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
            TASK_STATE ts = TASK_STATE_NON;
            if ("done" == status.toLower())
            {
                ts = TASK_STATE_CALCOMPLETE;
                tptr->setUsedCPUTime(MS_TO_HOUR(endTime.toULongLong() - startTime.toULongLong())*cupCount.toInt());
                tptr->setCpuSize(cupCount.toInt());
                int ret = TaskCommitMgr::updateCpuSize( userId, dptr->getJobName(), cupCount);
                if (ret != 0) {
                    qDebug() << "failed to update core count";
                }
                /// 更新本地数据库任务信息
                ///
                //                    QString userId = Config->GetUserId();
                //                    int ret = TaskCommitMgr::updateTaskInfo(userId, dptr->getJobName(), QString("%1").arg(tptr->getJobID()), QString("%1").arg(TASK_STATE_CALCOMPLETE), startTime, endTime);
                //                    if (ret != 0) {
                //                        qDebug() << "failed to update task info";
                //                    }
            } else if("prepare" == status.toLower()) {
                ts = TASK_STATE_PREPARE;
            } else if("cancel" == status.toLower()) {
                ts = TASK_STATE_STOPCOMPLETE;
            } else if("fail" == status.toLower()) {
                ts = TASK_STATE_FAIL;
            } else if("pending" == status.toLower()) {
                ts = TASK_STATE_PENDING;
            } else if("suspended" == status.toLower()) {
                ts = TASK_STATE_SUSPENDED;
            } else if("run" == status.toLower()) {
                ts = TASK_STATE_RUN;
            } else if("cancelling" == status.toLower()) {
                ts = TASK_STATE_CANCELLING;
            } else {
                ts = TASK_STATE_UNKNOWN;
            }
            tptr->setState(ts);

            int ret = TaskCommitMgr::updateTaskInfo(userId, dptr->getJobName(), QString("%1").arg(tptr->getJobID()), QString("%1").arg(ts), startTime, endTime);
            if (ret != 0) {
                qDebug() << "failed to update task info";
            }

        }
        qDebug() << "resBody : " << resBody;
        broadcast(tptr);
    }
        break;
    case TASK_STATE_SELFAILED:
    {
        tptr->setTaskName(data->getJobName());
        tptr->setState(data->taskType());
        broadcast(tptr);
    }
        break;
    case TASK_STATE_SELCOMPLETE:
    {
        tptr->setTaskName(data->getJobName());
        tptr->setState(data->taskType());
        broadcast(tptr);
    }
        break;
    case TASK_STATE_STOP:
    {
        const SelectTaskTransferData::Ptr& dptr = std::static_pointer_cast<SelectTaskTransferData>(data);
        tptr->setTaskName(dptr->getJobName());
        tptr->setState(TASK_STATE_SEL);
        tptr->setJobID(dptr->getJobID());
        QString mainAddr = Config->GetMainServerName();
        QString userToken = Config->GetUserToken();
        QString resBody;
        tptr->setState(TASK_STATE_STOPCOMPLETE);
        qDebug("cancel job id = %d", tptr->getJobID());
        QString userId = Config->GetUserId();
        if(1 != TaskCommitMgr::cancelRemoteJob(mainAddr, userToken, QString::number(tptr->getJobID()), resBody))
        {
            tptr->setState(TASK_STATE_STOPFAILED);
        } else {
            QString mainAddr = Config->GetMainServerName();
            QString userToken = Config->GetUserToken();
            QString resBody;
            if(1 != TaskCommitMgr::getRemoteJobInfo(mainAddr, userToken, QString::number(tptr->getJobID()), resBody))
            {
                tptr->setState(TASK_STATE_SELFAILED);
            } else {
                /// 取出结果信息
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

                int ret = TaskCommitMgr::updateTaskInfo(userId, dptr->getJobName(), QString("%1").arg(tptr->getJobID()), QString("%1").arg(TASK_STATE_STOPCOMPLETE), startTime, endTime);
                if (ret != 0) {
                    qDebug() << "failed to update task info";
                }
            }
            int ret = TaskCommitMgr::updateTaskStatus(userId, dptr->getJobName(), QString("%1").arg(TASK_STATE_STOPCOMPLETE));
            if (ret != 0) {
                qDebug() << "failed to update task info";
            }
        }
        qDebug() << "resBody : " << resBody;
        broadcast(tptr);
    }
        break;
    case TASK_STATE_STOPFAILED:
    {
        tptr->setTaskName(data->getJobName());
        tptr->setState(data->taskType());
        broadcast(tptr);
    }
        break;
    case TASK_STATE_STOPCOMPLETE:
    {
        tptr->setTaskName(data->getJobName());
        tptr->setState(data->taskType());
        broadcast(tptr);
    }
        break;
    case TASK_STATE_DEL:
    {
        const deleteTaskTransferData::Ptr& dptr = std::static_pointer_cast<deleteTaskTransferData>(data);
        tptr->setTaskName(dptr->getJobName());
        tptr->setState(TASK_STATE_SEL);
        QString mainAddr = Config->GetMainServerName();
        QString userToken = Config->GetUserToken();
        QString resBody;
        tptr->setState(TASK_STATE_DELCOMPLETE);
        QString remoteTaskDir = Config->GetUploadPath() + "/" + dptr->getJobName();
        if(1 != TaskCommitMgr::removeRemoteDir(mainAddr, userToken, remoteTaskDir, resBody))
        {
            tptr->setState(TASK_STATE_DELFAILED);
        } else {
            if(0 == TaskCommitMgr::removeTaskInfo(Config->GetUserId(), dptr->getJobName()))
            {
                qDebug() << "remove taskinfo successed";
                tptr->setState(TASK_STATE_DELCOMPLETE);
            } else {
                qDebug() << "failed to remove task";
                tptr->setState(TASK_STATE_DELFAILED);
                QString error = QObject::tr("failed to delete the task, taskname : %1").arg(tptr->getTaskName());
                tptr->setErrorInfo(error);
            }
        }
        qDebug() << "resBody : " << resBody;
        broadcast(tptr);
    }
        break;
    case TASK_STATE_DELFAILED:
    {
        tptr->setTaskName(data->getJobName());
        tptr->setState(data->taskType());
        broadcast(tptr);
    }
        break;
    case TASK_STATE_DELCOMPLETE:
    {
        tptr->setTaskName(data->getJobName());
        tptr->setState(data->taskType());
        broadcast(tptr);
    }
        break;
    default:
        break;
    }
}

void TaskDataObserverImpl::regis(const QString &className, OBS_FUN &&fun)
{
    //    _broadcasts.emplace(std::tuple<QString, OBS_FUN>(className, std::forward<OBS_FUN>(fun)));
    _broadcasts[className] = fun;
    //    _broadcasts.emplace_back(std::forward<OBS_FUN>(fun));
}

void TaskDataObserverImpl::regis(QString className, OBS_FUN &fun)
{
    _broadcasts[className] = fun;
}


void TaskDataObserverImpl::remove(const QString& className)
{
    //    if(_broadcasts)
}

void TaskDataObserverImpl::broadcast(TaskData::Ptr &ptr)
{
    for(const auto& f : _broadcasts)
    {
        f.second(std::move(ptr));
    }
}

TASK_STATE TaskData::getState() const
{
    return _state;
}

void TaskData::setState(const TASK_STATE &state)
{
    _state = state;
}

QString TaskData::getTaskName() const
{
    return _taskName;
}

void TaskData::setTaskName(const QString &value)
{
    _taskName = value;
}

QString TaskData::getDistr() const
{
    return _distr;
}

void TaskData::setDistr(const QString &distr)
{
    _distr = distr;
}

QString TaskData::getCreateTime() const
{
    return _createTime;
}

void TaskData::setCreateTime(const QString &createTime)
{
    _createTime = createTime;
}

short TaskData::getCpuSize() const
{
    return _cpuSize;
}

void TaskData::setCpuSize(short value)
{
    _cpuSize = value;
}

short TaskData::getNodeSize() const
{
    return _nodeSize;
}

void TaskData::setNodeSize(short value)
{
    _nodeSize = value;
}

bool TaskData::getLoginSuccess() const
{
    return loginSuccess;
}

void TaskData::setLoginSuccess(bool value)
{
    loginSuccess = value;
}

QString TaskData::getUpLoadPath() const
{
    return _upLoadPath;
}

void TaskData::setUpLoadPath(const QString &upLoadPath)
{
    _upLoadPath = upLoadPath;
}

QString TaskData::getDownLoadPath() const
{
    return _DownLoadPath;
}

void TaskData::setDownLoadPath(const QString &DownLoadPath)
{
    _DownLoadPath = DownLoadPath;
}

QString TaskData::getErrorInfo() const
{
    return errorInfo;
}

void TaskData::setErrorInfo(const QString &value)
{
    errorInfo = value;
}

TASK_STATE TaskData::getStoreState() const
{
    return _storeState;
}

void TaskData::setStoreState(const TASK_STATE &storeState)
{
    _storeState = storeState;
}

int TaskData::getJobID() const
{
    return jobID;
}

void TaskData::setJobID(int value)
{
    jobID = value;
}

float TaskData::getUsedCPUTime() const
{
    return usedCPUTime;
}

void TaskData::setUsedCPUTime(float value)
{
    usedCPUTime = value;
}

void TaskStore::clearTaskData()
{
    std::vector<TaskData::Ptr> t;
    tasks.swap(t);
}

void TaskStore::addTaskData(TaskData::Ptr &&ptr)
{
    tasks.emplace_back(ptr);
}

int TaskStore::getTaskDataSize()
{
    return tasks.size();
}

const TaskData::Ptr &TaskStore::getTaskData(int id)
{
    return tasks[id];
}

TransferData::TransferData()
{

}

TASK_STATE TransferData::taskType() const
{
    return _taskType;
}

void TransferData::setTaskType(const TASK_STATE &actionType)
{
    _taskType = actionType;
}

QString TransferData::getJobName() const
{
    return _jobName;
}

void TransferData::setJobName(const QString &value)
{
    _jobName = value;
}

int TransferData::getJobID() const
{
    return jobID;
}

void TransferData::setJobID(int value)
{
    jobID = value;
}

const QString& CreateTaskTransferData::time() const
{
    return _time;
}

void CreateTaskTransferData::setTime(const QString &time)
{
    _time = time;
}

const QString& CreateTaskTransferData::distr() const
{
    return _distr;
}

void CreateTaskTransferData::setDistr(const QString &distr)
{
    _distr = distr;
}

const QString& CreateTaskTransferData::commandLine() const
{
    return _commandLine;
}

void CreateTaskTransferData::setCommandLine(const QString &commandLine)
{
    _commandLine = commandLine;
}

short CreateTaskTransferData::getCpuSize() const
{
    return cpuSize;
}

void CreateTaskTransferData::setCpuSize(short value)
{
    cpuSize = value;
}

short CreateTaskTransferData::getNodeSize() const
{
    return nodeSize;
}

void CreateTaskTransferData::setNodeSize(short value)
{
    nodeSize = value;
}

QString CreateTaskTransferData::getDownPath() const
{
    return _downPath;
}

void CreateTaskTransferData::setDownPath(const QString &downPath)
{
    _downPath = downPath;
}

QString CreateTaskTransferData::getUpPath() const
{
    return _upPath;
}

void CreateTaskTransferData::setUpPath(const QString &upPath)
{
    _upPath = upPath;
}

ModelData::ModelDataBase *CreateTaskTransferData::getModeldata() const
{
    return _modelData;
}

void CreateTaskTransferData::setModeldata(ModelData::ModelDataBase *value)
{
    _modelData = value;
}

float CreateTaskTransferData::getUsedTime() const
{
    return usedTime;
}

void CreateTaskTransferData::setUsedTime(float value)
{
    usedTime = value;
}

DownTaskTransferData::DownTaskTransferData(ResultRunable *r) : result(r)
{

}

ResultRunable *DownTaskTransferData::getResult() const
{
    return result;
}

void DownTaskTransferData::setResult(ResultRunable *value)
{
    result = value;
}

const QString &DownTaskTransferData::getTaskName()
{
    return result->getJobName();
}

ExecTaskTransferData::ExecTaskTransferData(ExecuteRunable *e) : execut(e)
{

}

ExecuteRunable *ExecTaskTransferData::getExecut() const
{
    return execut;
}

void ExecTaskTransferData::setExecut(ExecuteRunable *value)
{
    execut = value;
}

const QString& ExecTaskTransferData::getDistr() const
{
    return distr;
}

void ExecTaskTransferData::setDistr(const QString &value)
{
    distr = value;
}

const QString &ExecTaskTransferData::getTaskName()
{
    if(nullptr != execut)
    {
        return execut->getJobName();
    }
    return "";
}

CommitTaskTransferData::CommitTaskTransferData(CommitRunable *c) : commit(c)
{

}

const QString &CommitTaskTransferData::getDistr() const
{
    return distr;
}

void CommitTaskTransferData::setDistr(const QString &value)
{

}

const QString &CommitTaskTransferData::getTaskName()
{
    return commit->getJobName();
}

CommitRunable *CommitTaskTransferData::getCommit() const
{
    return commit;
}

void CommitTaskTransferData::setCommit(CommitRunable *value)
{
    commit = value;
}

SelectTaskTransferData::SelectTaskTransferData()
{

}


CancelTaskTransferData::CancelTaskTransferData()
{

}

QString CancelTaskTransferData::getDistr() const
{
    return distr;
}

void CancelTaskTransferData::setDistr(const QString &value)
{
    distr = value;
}

deleteTaskTransferData::deleteTaskTransferData()
{

}

QString deleteTaskTransferData::getDistr() const
{
    return distr;
}

void deleteTaskTransferData::setDistr(const QString &value)
{
    distr = value;
}

TASK_STATE CreatedTaskTransferData::getLoadTasKState() const
{
    return loadTasKState;
}

void CreatedTaskTransferData::setLoadTasKState(const TASK_STATE &value)
{
    loadTasKState = value;
}
