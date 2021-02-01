#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#define ENABLE_DATABASE_SQLITE      1
#include "Dataface.hpp"
#include "ParaTask.h"

int ParaTask::loginServer(const QString &serverAddr, const QString &userName, const QString &secureKey, QString &userToken)
{
    QMap<QString, QString> reqHeaders;
    reqHeaders.insert("Content-Type", "application/x-www-form-urlencoded");

    QString reqBody = QString("token_type=TOKEN&third_party=SELF&email=")+ userName + QString("&password=") + secureKey;
    QString urlPath = serverAddr + QString("/user/api/login");
    HttpResponse *response;
    int ret = sendPostRequest(urlPath, reqHeaders, reqBody.toLocal8Bit(), &response);
//    ilog(Info, "HTTP Status: %d", response->status());
//    ilog(Info, response->body()->toString());
    if (ret != 1) {
        qDebug("failed to login server");
        return 0;
    }

    /// 登录服务器，获取用户令牌
    QMap<QString, QString> headers = response->headers();
    userToken = headers["PARA_TOKEN"];
    return 1;
}

int ParaTask::uploadArchive(QString serverAddr, QString userToken, QString remoteFilePath, QString bytesRange, QByteArray fileData)
{
    QMap<QString, QString> reqHeaders;
    reqHeaders.insert("content-type", "application/octet-stream");
    reqHeaders.insert("PARA_TOKEN", userToken);
    reqHeaders.insert("Content-Range", bytesRange);

    QString urlPath = serverAddr + QString("/api/pcs/file/upload?path=prn:pcs:sftp:BSCC-A2:BSCC-A2:sc71306:file:") + remoteFilePath;
    HttpResponse *response;
    int ret = sendPostRequest(urlPath, reqHeaders, fileData, &response);
//    ilog(Info, "HTTP Status: %d", response->status());
//    ilog(Info, response->body()->toString());
    if (ret != 1) {
        qDebug("upload failed");
        return 0;
    }

    return 1;
}

int ParaTask::downloadArchive(QString serverAddr, QString userToken, QString remoteFilePath, QString bytesRange, QByteArray &fileData)
{
    QMap<QString, QString> reqHeaders;
    reqHeaders.insert("PARA_TOKEN", userToken);
    reqHeaders.insert("range", bytesRange);

    QString urlPath = serverAddr + QString("/api/pcs/file/download?path=prn:pcs:sftp:BSCC-A2:BSCC-A2:sc71306:file:") + remoteFilePath;
    HttpResponse *response;
    int ret = sendGetRequest(urlPath, reqHeaders, &response);
//    ilog(Info, "HTTP Status: %d", response->status());
    if (ret != 1) {
        qDebug("download failed");
        return 0;
    }

    fileData = response->data();
    return 1;
}

int ParaTask::zipRemotePath(QString serverAddr, QString userToken, QString destDir, QString zipFileName, QString workDir, QString subPath)
{
    QJsonArray paraArray;
    paraArray.append(subPath);
    QJsonObject json;
    json.insert("dst", "prn:pcs:sftp:BSCC-A2:BSCC-A2:sc71306:file:" + destDir);
    json.insert("fileName", zipFileName);
    json.insert("workDir", "prn:pcs:sftp:BSCC-A2:BSCC-A2:sc71306:file:" + workDir);
    json.insert("subPaths", QJsonValue(paraArray));
    QString jsonData = QString(QJsonDocument(json).toJson(QJsonDocument::Compact));
    ///qDebug() << jsonData;

    QMap<QString, QString> reqHeaders;
    reqHeaders.insert("content-type", "application/json");
    reqHeaders.insert("PARA_TOKEN", userToken);

    QString urlPath = serverAddr + QString("/api/pcs/tasks/file/zip");
    HttpResponse *response;
    int ret = sendPostRequest(urlPath, reqHeaders, jsonData.toLocal8Bit(), &response);
//    ilog(Info, "HTTP Status: %d", response->status());
//    ilog(Info, response->body()->toString());
    if (ret != 1) {
        qDebug("failed to zip remote path");
        return 0;
    }

    return 1;
}

int ParaTask::unzipRemoteArchive(QString serverAddr, QString userToken, QString zipFilePath, QString taskPath)
{
    QJsonObject json;
    json.insert("dst", "prn:pcs:sftp:BSCC-A2:BSCC-A2:sc71306:file:" + taskPath);
    json.insert("src", "prn:pcs:sftp:BSCC-A2:BSCC-A2:sc71306:file:" + zipFilePath);
    QString jsonData = QString(QJsonDocument(json).toJson(QJsonDocument::Compact));
    ///qDebug() << jsonData;

    QMap<QString, QString> reqHeaders;
    reqHeaders.insert("content-type", "application/json");
    reqHeaders.insert("PARA_TOKEN", userToken);

    QString urlPath = serverAddr + QString("/api/pcs/tasks/file/unzip");
    HttpResponse *response;
    int ret = sendPostRequest(urlPath, reqHeaders, jsonData.toLocal8Bit(), &response);
//    ilog(Info, "HTTP Status: %d", response->status());
//    ilog(Info, response->body()->toString());
    if (ret != 1) {
        qDebug("failed to upzip remote archive");
        return 0;
    }

    return 1;
}

int ParaTask::executeRemoteJob(QString serverAddr, QString userToken, QString taskName, QString workDir, QString bootScript, QString scriptParamter, QString &resBody)
{
    QJsonArray paraArray;
    paraArray.append(scriptParamter);
    QJsonObject submitJson;
    submitJson.insert("bootScript", bootScript);
    submitJson.insert("workingDir", "prn:pcs:sftp:BSCC-A2:BSCC-A2:sc71306:file:" + workDir);
    submitJson.insert("scriptParamters", QJsonValue(paraArray));
    ///submitJson.insert("scriptParamters", scriptParamters);

    QJsonObject json;
    json.insert("jobName", taskName);
    json.insert("submitProfile", QJsonValue(submitJson));
    QString jsonData = QString(QJsonDocument(json).toJson(QJsonDocument::Compact));

    QMap<QString, QString> reqHeaders;
    reqHeaders.insert("content-type", "application/json");
    reqHeaders.insert("PARA_TOKEN", userToken);

    QString urlPath = serverAddr + QString("/api/pcs/jobs/sbs");
    HttpResponse *response;
    int ret = sendPostRequest(urlPath, reqHeaders, jsonData.toLocal8Bit(), &response);
    resBody = response->body()->toString();
//    ilog(Info, "HTTP Status: %d", response->status());
//    ilog(Info, resBody);
    if (ret != 1) {
        qDebug("failed to execute synchronization job");
        qDebug() << resBody;
        return 0;
    }

    return 1;
}

int ParaTask::makeRemoteDir(QString serverAddr, QString userToken, QString pathName)
{
    QJsonObject json;
    json.insert("path", "prn:pcs:sftp:BSCC-A2:BSCC-A2:sc71306:file:" + pathName);
    ///json.insert("recursive", "false");
    QString jsonData = QString(QJsonDocument(json).toJson(QJsonDocument::Compact));

    QMap<QString, QString> reqHeaders;
    reqHeaders.insert("content-type", "application/json");
    reqHeaders.insert("PARA_TOKEN", userToken);

    QString urlPath = serverAddr + QString("/api/pcs/file/mkdir");
    HttpResponse *response;
    int ret = sendPostRequest(urlPath, reqHeaders, jsonData.toLocal8Bit(), &response);
//    ilog(Info, "HTTP Status: %d", response->status());
//    ilog(Info, response->body()->toString());
    if (ret != 1) {
        qDebug("failed to make remote dir");
        return 0;
    }

    return 1;
}

int ParaTask::getRemoteFileSize(QString serverAddr, QString userToken, QString remoteFilePath, int &fileSize)
{
    QMap<QString, QString> reqHeaders;
    reqHeaders.insert("PARA_TOKEN", userToken);

    QString urlPath = serverAddr + QString("/api/pcs/file/info?path=prn:pcs:sftp:BSCC-A2:BSCC-A2:sc71306:file:") + remoteFilePath;
    HttpResponse *response;
    int ret = sendGetRequest(urlPath, reqHeaders, &response);
//    ilog(Info, "HTTP Status: %d", response->status());
//    ilog(Info, response->body()->toString());
    if (ret != 1) {
        qDebug("failed to get file size");
        return 0;
    }

    QString json = response->body()->toString();
    QJsonParseError err;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8(), &err);
    if (QJsonParseError::NoError == err.error) {
        if (jsonDoc.isObject()) {
            QVariantMap result = jsonDoc.toVariant().toMap();
            fileSize = result["size"].toInt();
            return 0;
        }
    }

    return 1;
}

int ParaTask::getRemoteJobInfo(QString serverAddr, QString userToken, QString jobId, QString &resBody)
{
    QMap<QString, QString> reqHeaders;
    reqHeaders.insert("PARA_TOKEN", userToken);

    QString urlPath = serverAddr + QString("/api/pcs/jobs/") + jobId;
    HttpResponse *response;
    int ret = sendGetRequest(urlPath, reqHeaders, &response);
    resBody = response->body()->toString();
//    ilog(Info, "HTTP Status: %d", response->status());
//    ilog(Info, resBody);
    if (ret != 1) {
        qDebug("failed to get job information");
        return 0;
    }
    return 1;
}

int ParaTask::cancelRemoteJob(QString serverAddr, QString userToken, QString jobId, QString &resBody)
{
    QMap<QString, QString> reqHeaders;
    reqHeaders.insert("PARA_TOKEN", userToken);

    QString urlPath = serverAddr + QString("/api/pcs/jobs/") + jobId + "/cancel";
    HttpResponse *response;
    int ret = sendPutRequest(urlPath, reqHeaders, &response);
    resBody = response->body()->toString();
    qDebug() << "cancel = " << resBody;
//    ilog(Info, "HTTP Status: %d", response->status());
//    ilog(Info, response->body()->toString());
    if (ret != 1) {
        qDebug("failed to cancle job");
        return 0;
    }
    return 1;
}

int ParaTask::removeRemoteDir(QString serverAddr, QString userToken, QString remoteTaskDir, QString &resBody)
{
    QMap<QString, QString> reqHeaders;
    reqHeaders.insert("PARA_TOKEN", userToken);

    QString urlPath = serverAddr + QString("/api/pcs/file/rm?path=prn:pcs:sftp:BSCC-A2:BSCC-A2:sc71306:file:") + remoteTaskDir;
    HttpResponse *response;
    int ret = sendDeleteRequest(urlPath, reqHeaders, &response);
    resBody = response->body()->toString();
//    ilog(Info, "HTTP Status: %d", response->status());
//    ilog(Info, response->body()->toString());
    if (ret != 1) {
//        ilog(Error, "failed to remove dir");
        return 0;
    }

    return 1;
}
