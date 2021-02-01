#ifndef _TASK_BASE_H_
#define _TASK_BASE_H_

#include <QString>
#include "httpclient.hpp"
using namespace Restful::Http;

/// 任务接口类
class TaskBase
{
public:
    TaskBase(){}

    virtual ~TaskBase(){}

    /// 超算云类型
    enum CloudType {
        /// 并行科技
        CLOUD_PARATERA,
    };

    /// 登录云服务器
    virtual int loginServer(const QString &serverAddr, const QString &userName, const QString &secureKey, QString &userToken) = 0;

    /// 上传配置数据文件压缩包
    virtual int uploadArchive(QString serverAddr, QString userToken, QString uploadFilePath, QString bytesRange, QByteArray fileData) = 0;

    /// 下载远程计算结果压缩包
    virtual int downloadArchive(QString serverAddr, QString userToken, QString dloadFilePath, QString bytesRange, QByteArray &fileData) = 0;

    /// 打包远程目录下的文件
    virtual int zipRemotePath(QString serverAddr, QString userToken, QString destDir, QString zipFileName, QString workDir, QString subPath) = 0;

    /// 解压远程目录下的压缩包
    virtual int unzipRemoteArchive(QString serverAddr, QString userToken, QString zipFilePath, QString taskPath) = 0;

    /// 执行远程作业
    virtual int executeRemoteJob(QString serverAddr, QString userToken, QString taskName, QString workDir, QString bootScript, QString scriptParamter, QString &resBody) = 0;

    /// 创建远程目录
    virtual int makeRemoteDir(QString serverAddr, QString userToken, QString pathName) = 0;

    /// 获取远程文件大小
    virtual int getRemoteFileSize(QString serverAddr, QString userToken, QString remoteFilePath, int &fileSize) = 0;

    /// 获取远程作业信息
    virtual int getRemoteJobInfo(QString serverAddr, QString userToken, QString jobId, QString &resBody) = 0;

    /// 取消远程作业
    virtual int cancelRemoteJob(QString serverAddr, QString userToken, QString jobId, QString &resBody) = 0;

    /// 删除远程作业目录
    virtual int removeRemoteDir(QString serverAddr, QString userToken, QString taskDir, QString &resBody) = 0;

    /// 创建一个任务对象
    /// type: 超算云类型(CLOUD_PARATERA)
    static TaskBase * Create(const int type);

    /// https post请求
    int sendPostRequest(QString urlPath, QMap<QString, QString> reqHeaders, QByteArray reqBody, HttpResponse **response);

    /// https get请求
    int sendGetRequest(QString urlPath, QMap<QString, QString> reqHeaders, HttpResponse **response);

    /// https put请求
    int sendPutRequest(QString urlPath, QMap<QString, QString> reqHeaders, HttpResponse **response);

    /// https delete请求
    int sendDeleteRequest(QString urlPath, QMap<QString, QString> reqHeaders, HttpResponse **response);

    /// sql
    int appendTaskInfo(QString userId, QString taskName, QString status, int nodes, int cores, QString uploadPath, QString dloadPath);
    int updateTaskInfo(QString userId, QString taskName, QString jobId, QString status, QString startTime, QString endTime);
    int updateTaskJobID(QString userId, QString taskName, QString jobId);
    int updateTaskStatus(QString userId, QString taskName, QString status);
    int updateStartTime(QString userId, QString taskName, QString startTime);
    int updateEndTime(QString userId, QString taskName, QString endTime);
    int updateCpuSize(QString userId, QString taskName, QString CpuSize);
    int removeTaskInfo(QString userId, QString taskName);
    int readAllTaskInfo(QString userId, QString &jsonData);
    int readTaskInfo(QString userId, QString taskName, QString &jsonData);

private:
};

extern "C"
{
    /// 创建任务管理对象
    TaskBase * CreateTaskBase(const int type = TaskBase::CLOUD_PARATERA);
    /// 释放任务管理对象
    void ReleaseTaskBase(TaskBase * const task);
}

#endif /// _TASK_BASE_H_
