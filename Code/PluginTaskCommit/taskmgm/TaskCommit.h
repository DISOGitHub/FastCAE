#ifndef TASKCOMMIT_H
#define TASKCOMMIT_H
#include <QString>
#include "TaskCommitPlugin.h"
#include "NonCopyable.h"
#include "GlobalSetting.hpp"
#include "ParaTask.h"

/**
 * @brief The TaskCommitMgr class
 * 任务提交管理类.超算及数据库操作入口 *
 */
class TaskCommitMgr : public NonCopyable
{
public:
    TaskCommitMgr();
    ~TaskCommitMgr() =default ;

public:
    static int loginServer(const QString &mainAddr, const QString &userAddr, const QString &userName, const QString &hashKey);
    static int getRemoteFileSize(QString mainAddr, QString userToken, QString dloadFilePath, int &fileSize);
    static int saveTaskinfo(QString gridFilePath, QString configFilePath, QString uploadPath, QString downloadPath);
    static int downLoad(QString mainAddr, QString userToken, QString dloadFilePath, QString bytesRange, QByteArray &fileData);
    static int upLoad(const QString& mainAddr, const QString& userToken, const QString& uploadFilePath, const QString& bytesRange, const QByteArray& fileData);
    static int execute(QString serverAddr, QString userToken, QString taskName, QString workDir, QString bootScript, QString scriptParamter, QString &resBody);
    static int makeRemoteDir(QString mainAddr, QString userToken, QString pathName);
    static int upzipRemoteArchive(QString mainAddr, QString userToken, QString taskPath, QString zipFilePath);
    static int doesRemoteFileExist(QString mainAddr, QString userToken, QString zipFilePath);
    static int zipRemotePath(QString mainAddr, QString userToken, QString dloadPath, QString zipFileName, QString workPath, QString subPath);
    static int getRemoteJobInfo(QString serverAddr, QString userToken, QString jobId, QString &resBody);
    static int cancelRemoteJob(QString serverAddr, QString userToken, QString jobId, QString &resBody);
    static int removeRemoteDir(QString serverAddr, QString userToken, QString remoteTaskDir, QString &resBody);
    static bool isLoginSucceed();

    /// sql
    static int appendTaskInfo(QString userId, QString taskName, QString status, int nodes, int cores, QString uploadPath, QString dloadPath);
    static int updateTaskInfo(QString userId, QString taskName, QString jobId, QString status, QString startTime, QString endTime);
    static int updateTaskJobID(QString userId, QString taskName, QString jobId);
    static int updateTaskStatus(QString userId, QString taskName, QString status);
    static int updateStartTime(QString userId, QString taskName, QString startTime);
    static int updateEndTime(QString userId, QString taskName, QString endTime);
    static int updateCpuSize(QString userId, QString taskName, QString cupSize);
    static int removeTaskInfo(QString userId, QString taskName);
    static int readAllTaskInfo(QString userId, QString &jsonData);
    static int readTaskInfo(QString userId, QString taskName, QString &jsonData);

protected:
    static TaskBase* paraTask;

private:


};

#endif // TASKCOMMIT_H
