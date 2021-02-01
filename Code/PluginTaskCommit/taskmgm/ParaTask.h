#ifndef _PARA_TASK_H_
#define _PARA_TASK_H_

#include "TaskBase.h"

/// 任务管理接口类
class ParaTask : public TaskBase
{
public:
    ParaTask(){}
    virtual ~ParaTask(){}

    virtual int loginServer(const QString &serverAddr, const QString &userName, const QString &secureKey, QString &userToken);

    virtual int uploadArchive(QString serverAddr, QString userToken, QString uploadFilePath, QString bytesRange, QByteArray fileData);

    virtual int downloadArchive(QString serverAddr, QString userToken, QString dloadFilePath, QString bytesRange, QByteArray &fileData);

    virtual int zipRemotePath(QString serverAddr, QString userToken, QString destDir, QString zipFileName, QString workDir, QString subPath);

    virtual int unzipRemoteArchive(QString serverAddr, QString userToken, QString zipFilePath, QString taskPath);

    virtual int executeRemoteJob(QString serverAddr, QString userToken, QString taskName, QString workDir, QString bootScript, QString scriptParamter, QString &resBody);

    virtual int makeRemoteDir(QString serverAddr, QString userToken, QString pathName);

    virtual int getRemoteFileSize(QString serverAddr, QString userToken, QString remoteFilePath, int &fileSize);

    virtual int getRemoteJobInfo(QString serverAddr, QString userToken, QString jobId, QString &resBody);

    virtual int cancelRemoteJob(QString serverAddr, QString userToken, QString jobId, QString &resBody);

    virtual int removeRemoteDir(QString serverAddr, QString userToken, QString remoteTaskDir, QString &resBody);

private:
};

#endif ///_PARA_TASK_H_
