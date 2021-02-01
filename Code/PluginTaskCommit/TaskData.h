#ifndef TASKDATA_H
#define TASKDATA_H

//#include "Dispatcher.h"
#include "TaskThread.h"
#include "ActionType.h"
#include "NonCopyable.h"

#include <QString>
#include <QMap>

namespace ModelData {
class ModelDataBase;
}



namespace Plugins {

class Dispatcher;

/**
 * @brief The TransferData class
 * 传输数据 用于将前端数据传输到数据处理模块 *
 */
class TransferData {
public:
    using Ptr = std::shared_ptr<TransferData>;
    TransferData();
    ~TransferData()  = default ;

    TASK_STATE taskType() const;
    void setTaskType(const TASK_STATE &actionType);

    QString getJobName() const;
    void setJobName(const QString &value);

    int getJobID() const;
    void setJobID(int value);

private:
    TASK_STATE _taskType{TASK_STATE_NON};
    QString _jobName;
    int jobID;
};

/**
 * @brief The LoginData class
 * 传输登录数据 *
 */
class LoginData : public TransferData
{
public:
    using Ptr = std::shared_ptr<LoginData>;
    LoginData() = default;
    ~LoginData() = default;

private:


};

/**
 * @brief The CreateTaskTransferData class
 * 传输创建任务数据 *
 */
class CreateTaskTransferData : public TransferData
{
public:
    using Ptr = std::shared_ptr<CreateTaskTransferData>;
    CreateTaskTransferData() = default;
    ~CreateTaskTransferData() = default;

    const QString &time() const;
    void setTime(const QString &time);

    const QString& distr() const;
    void setDistr(const QString &distr);

    const QString& commandLine() const;
    void setCommandLine(const QString &commandLine);

    short getCpuSize() const;
    void setCpuSize(short value);

    short getNodeSize() const;
    void setNodeSize(short value);

    QString getDownPath() const;
    void setDownPath(const QString &downPath);

    QString getUpPath() const;
    void setUpPath(const QString &upPath);

    ModelData::ModelDataBase *getModeldata() const;
    void setModeldata(ModelData::ModelDataBase *value);

    float getUsedTime() const;
    void setUsedTime(float value);

protected:
    QString _time{""};
    QString _distr{""};
    QString _downPath{""};
    QString _upPath{""};
    QString _commandLine{""};
    short cpuSize;
    short nodeSize;
    float usedTime;
    ModelData::ModelDataBase* _modelData;

};

/**
 * @brief The CreatedTaskTransferData class
 * 传输已经创建好的任务数据(from db) *
 */
class CreatedTaskTransferData : public CreateTaskTransferData
{
public:
    using Ptr = std::shared_ptr<CreatedTaskTransferData>;
    CreatedTaskTransferData() = default;
    ~CreatedTaskTransferData() = default;

    TASK_STATE getLoadTasKState() const;
    void setLoadTasKState(const TASK_STATE &value);

private:
    TASK_STATE loadTasKState = TASK_STATE_CREATED;

};

/**
 * @brief The CommitTaskTransferData class
 * 传输上传任务数据 *
 */
class CommitTaskTransferData : public TransferData
{
public:
    using Ptr = std::shared_ptr<CommitTaskTransferData>;
    CommitTaskTransferData(CommitRunable* c);
    ~CommitTaskTransferData()  = default ;

    const QString &getDistr() const;
    void setDistr(const QString &value);

    const QString &getTaskName();

    CommitRunable *getCommit() const;
    void setCommit(CommitRunable *value);

private:
    CommitRunable* commit;
    QString distr;
};

/**
 * @brief The DownTaskTransferData class
 * 传输下载结果数据类 *
 */
class DownTaskTransferData : public TransferData
{
public:
    using Ptr = std::shared_ptr<DownTaskTransferData>;
    DownTaskTransferData(ResultRunable* r);
    ~DownTaskTransferData()  = default ;

    ResultRunable *getResult() const;
    void setResult(ResultRunable *value);
    const QString &getTaskName();

private:
    ResultRunable* result;
};

/**
 * @brief The ExecTaskTransferData class
 * 传输计算任务数据类 *
 */
class ExecTaskTransferData : public TransferData
{
public:
    using Ptr = std::shared_ptr<ExecTaskTransferData>;
    ExecTaskTransferData(ExecuteRunable* e);
    ~ExecTaskTransferData()  = default ;

    ExecuteRunable *getExecut() const;
    void setExecut(ExecuteRunable *value);

    const QString &getDistr() const;
    void setDistr(const QString &value);

    const QString &getTaskName();

private:
    ExecuteRunable* execut;
    QString distr;
};

/**
 * @brief The SelectTaskTransferData class
 * 传输查询任务信息数据类 *
 */
class SelectTaskTransferData : public TransferData
{
public:
    using Ptr = std::shared_ptr<SelectTaskTransferData>;
    SelectTaskTransferData();
    ~SelectTaskTransferData()  = default ;

    const QString &getDistr() const;
    void setDistr(const QString &value);

private:
    QString distr;
};

/**
 * @brief The CancelTaskTransferData class
 * 传输取消任务计算数据类 *
 */
class CancelTaskTransferData : public TransferData
{
public:
    using Ptr = std::shared_ptr<CancelTaskTransferData>;
    CancelTaskTransferData();
    ~CancelTaskTransferData()  = default ;

    QString getDistr() const;
    void setDistr(const QString &value);

private:
    QString distr;
};

/**
 * @brief The deleteTaskTransferData class
 * 传输删除任务类 *
 */
class deleteTaskTransferData : public TransferData
{
public:
    using Ptr = std::shared_ptr<deleteTaskTransferData>;
    deleteTaskTransferData();
    ~deleteTaskTransferData()  = default ;

    QString getDistr() const;
    void setDistr(const QString &value);

private:
    QString distr;
};

/**
 * @brief The TaskData class
 * 数据更新到GUI界面数据类 *
 */
class TaskData
{
public:
    using Ptr = std::shared_ptr<TaskData>;
    TaskData()  = default ;
    virtual ~TaskData()  = default ;

    TASK_STATE getState() const;
    void setState(const TASK_STATE &state);

    QString getTaskName() const;
    void setTaskName(const QString &value);

    QString getDistr() const;
    void setDistr(const QString &distr);

    QString getCreateTime() const;
    void setCreateTime(const QString &createTime);

    short getCpuSize() const;
    void setCpuSize(short value);

    short getNodeSize() const;
    void setNodeSize(short value);

    bool getLoginSuccess() const;
    void setLoginSuccess(bool value);

    QString getUpLoadPath() const;
    void setUpLoadPath(const QString &upLoadPath);

    QString getDownLoadPath() const;
    void setDownLoadPath(const QString &DownLoadPath);

    QString getErrorInfo() const;
    void setErrorInfo(const QString &value);

    TASK_STATE getStoreState() const;
    void setStoreState(const TASK_STATE &storeState);

    int getJobID() const;
    void setJobID(int value);

    float getUsedCPUTime() const;
    void setUsedCPUTime(float value);

private:
    QString _taskName;
    QString _distr;
    QString _createTime;
    QString _upLoadPath;
    QString _DownLoadPath;
    QString errorInfo;
    float usedCPUTime{0}; /// hour
    int jobID;
    TASK_STATE _state{TASK_STATE_NON};
    TASK_STATE _storeState{TASK_STATE_NON};
    short _cpuSize;
    short _nodeSize;
    bool loginSuccess;
};

/**
 * @brief The TaskStore class
 * 数据存储类 *
 */
class TaskStore : public NonCopyable
{
public:
    using Ptr = std::shared_ptr<TaskStore>;
    TaskStore()  = default;
    TaskStore(const TaskStore& d) = delete ;
    static auto getInstance()
    {
       if(_ptr == nullptr)
           _ptr = CreateInstance::Instance<TaskStore>();
       return _ptr;
    }

    bool operator=(TaskStore& d) = delete ;
    ~TaskStore()  = default;
    void clearTaskData();
    void addTaskData(TaskData::Ptr&& ptr);
    int getTaskDataSize();
    const TaskData::Ptr& getTaskData(int id);

private:
    std::vector<TaskData::Ptr> tasks;
    static TaskStore::Ptr _ptr;
};

using OBS_FUN = std::function<void(TaskData::Ptr&&)>;
class ObjectImpl
{
public:
    using Ptr = std::shared_ptr<ObjectImpl>;

    virtual void init() = 0;

    virtual void processAction(const TransferData::Ptr &data)  = 0;

    virtual void regis(const QString& className, OBS_FUN&& fun)  = 0;

//    virtual void regis(QString className, OBS_FUN& fun) = 0;

    virtual void remove(const QString& className) = 0;

    virtual void broadcast(TaskData::Ptr &ptr) = 0;
};

/**
 * @brief The TaskDataObserverImpl class
 * 响应GUI action 的数据处理类 *
 */
class TaskDataObserverImpl : public ObjectImpl
{
public:
    using Ptr = std::shared_ptr<TaskDataObserverImpl>;

    TaskDataObserverImpl()  = default;
    ~TaskDataObserverImpl()  = default ;

    void init();

    void processAction(const TransferData::Ptr &data);

    void regis(const QString& className, OBS_FUN&& fun);

    void regis(QString className, OBS_FUN& fun);

    void remove(const QString& className);

    void broadcast(TaskData::Ptr &ptr);

private:
    std::map<QString, OBS_FUN> _broadcasts;
//    TaskStore::Ptr _taskStorePtr;
};

class TaskDataObserver : public NonCopyable
{
public:
    using Ptr = std::shared_ptr<TaskDataObserver>;
    TaskDataObserver() = default;
    ~TaskDataObserver() = default;
    /// waring mul thread
    static auto getInstance()
    {
       if(_ptr == nullptr)
       {
           _ptr = CreateInstance::Instance<TaskDataObserver>();
       }
       return _ptr;
    }

    void init();
    void processAction(const TransferData::Ptr &data);

//    template<class S, class FUN>
//    void regis(S& className, FUN&& fun)
    void regis(QString className, OBS_FUN&& fun)
    {
        _implPtr->regis(std::forward<QString>(className), std::forward<OBS_FUN>(fun));
    }

//    template<class S, class FUN>
//    void regis(QString& className, const FUN& fun)
    void regis(QString className, OBS_FUN& fun)
    {
        _implPtr->regis(className, fun);
    }

    template<class FUN>
    void remove(const QString& className)
    {
        _implPtr->remove(std::forward<FUN>(className));
    }

protected:


private:
    TaskDataObserverImpl::Ptr _implPtr = nullptr;
    static TaskDataObserver::Ptr _ptr;

};

}


#endif // TASKDATA_H
