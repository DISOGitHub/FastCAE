#ifndef TASKTHREAD_H
#define TASKTHREAD_H

#include "ActionType.h"
#include <QObject>
#include <QRunnable>

namespace ModelData {
class ModelDataBase;
}

namespace Plugins {
class SelectTaskTransferData;
}

/**
 * @brief The CommitRunable class
  * * 任务上传执行类 *
 */
class CommitRunable : public QObject, public QRunnable
{
    Q_OBJECT
public:
//    using Ptr = std::shared_ptr<CommitRunable>;
    CommitRunable()
    {

    }

    CommitRunable(QString jobName, QString filePath) :  jobName(jobName) , filePath(filePath)
    {

    }

    ~CommitRunable()
    {

    }
    void run() override;

    const QString &getJobName() const;
    void setJobName(const QString &value);

    QString getFilePath() const;
    void setFilePath(const QString &value);

signals:
    void signalCommitState(QString fileName, int state);
    void signalFileSize(QString fileName, int size);
    void signalCommitedSize(QString fileName, int schedule);
    void signalSpeedSize(QString fileName, QString s);

private:
	bool copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist);
	void clearFolder(const QString &folderFullPath);
private slots:

private:
    QString uuid;
    QString jobName;
    QString filePath;
};

/**
 * @brief The ExecuteRunable class
 * * 计算任务执行类 *
 */
class ExecuteRunable : public QObject, public QRunnable
{
    Q_OBJECT
public:
//    using Ptr = std::shared_ptr<ExecuteRunable>;
    ExecuteRunable()
    {

    }

    ExecuteRunable(QString jobName, QString filePath) :  jobName(jobName) , filePath(filePath)
    {

    }

    ~ExecuteRunable()
    {

    }
    void run() override;

    const QString &getJobName() const;
    void setJobName(const QString &value);

    QString getFilePath() const;
    void setFilePath(const QString &value);

signals:
    void signalExecuteState(QString fileName, int state, int jobid);
    void signalExecuteJobID(QString fileName, int id);

private slots:

private:
    QString jobName;
    QString filePath;
};

/**
 * @brief The ResultRunable class
 * * 结果下载执行类 *
 */
class ResultRunable : public QObject, public QRunnable
{
    Q_OBJECT
public:
//    using Ptr = std::shared_ptr<ResultRunable>;
    ResultRunable()
    {

    }

    ResultRunable(QString jobName, QString filePath) :  jobName(jobName) , filePath(filePath)
    {

    }

    ~ResultRunable()
    {

    }
    void run() override;

//    QString getFileName() const;
//    void setFileName(const QString &value);

    const QString &getJobName() const;
    void setJobName(const QString &value);

    QString getFilePath() const;
    void setFilePath(const QString &value);

signals:
    void signalDownState(QString fileName, int s);
    void signalFileSize(QString fileName, int size);
    void signalDownedSize(QString fileName, int size);
    void signalSpeedSize(QString fileName, QString s);

private slots:

private:
    QString jobName;
    QString filePath;

};

#endif // TASKTHREAD_H
