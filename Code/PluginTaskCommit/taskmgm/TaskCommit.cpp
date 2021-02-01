#include "TaskCommit.h"

#include "TaskData.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

TaskBase* TaskCommitMgr::paraTask = new ParaTask;
TaskCommitMgr::TaskCommitMgr()
{

}

int TaskCommitMgr::loginServer(const QString &mainAddr, const QString &userAddr, const QString &userName, const QString &hashKey)
{
#if 1
    QMap<QString, QString> headers;
    ///headers.insert("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
    headers.insert("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
    ///QUuid id = QUuid::createUuid();
    ///qDebug() << id.toString();
    ///headers.insert("postman-token", id.toString());

    HttpRequest *request = new HttpRequest();
#if 0
    QString passwd = "-QFKJSZCH2020";
    QString strMd5 = QCryptographicHash::hash(passwd.toLatin1(),QCryptographicHash::Md5).toHex();
    qDebug() << strMd5;
#endif
    HttpBody *body = new HttpBody(QString("token_type=TOKEN&third_party=SELF&email=")+ userName + QString("&password=") + hashKey);
    request->setBody(body);
    request->setMethod("POST");
    request->setUrl(QUrl(userAddr + QString("/user/api/login")));
    request->setHeaders(headers);

    HttpClient *client = new HttpClient();
    client->sslCheck();
    HttpResponse *response = client->send(request);

    qDebug() << "Headers:" << response->headers();
    qDebug() << "HTTP Status:" << response->status();
    qDebug() << "Body:" << response->body()->toString();

    if (response->isSuccessful()) {
        ///Config->SetAuthHeaders(response->headers());
        QMap<QString, QString> headers = response->headers();
        Config->SetUserToken(headers["PARA_TOKEN"]);
        ///Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, response->body()->toString());
        return 1;
    } else {
        ///Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(1, response->body()->toString());
    }
    return 0;
#else
    //paraTask->loginServer(mainAddr, userAddr, userName, hashKey);
#endif
}

int TaskCommitMgr::getRemoteFileSize(QString mainAddr, QString userToken, QString dloadFilePath, int &fileSize)
{
    QMap<QString, QString> headers;
    ///headers.insert("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
    headers.insert("cache-control", "no-cache");
    headers.insert("PARA_TOKEN", userToken);
    ///headers.insert("range", "bytes=0-1024");
    HttpRequest *request = new HttpRequest();
    request->setMethod("GET");
    request->setUrl(QUrl(mainAddr + QString("/api/pcs/file/info?path=prn:pcs:sftp:BSCC-A2:BSCC-A2:sc71306:file:") + dloadFilePath));
    request->setHeaders(headers);
    ///qDebug() << headers;
    ///qDebug() << request->url();

    HttpClient *client = new HttpClient();
    ///client->sslCheck();
    HttpResponse *response = client->send(request);
    ///qDebug() << "Headers:" << response->headers();
    qDebug() << "HTTP Status:" << response->status();
    qDebug() << "Body:" << response->body()->toString();
    ///qDebug() << "Data:" << response->data();

    if (response->isSuccessful()) {
        qDebug() << "get file size successfully";
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, response->body()->toString());
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
    } else {

    }

    qDebug() << "failed to get file size";
    return -1;
}

int TaskCommitMgr::saveTaskinfo(QString gridFilePath, QString configFilePath, QString uploadPath, QString downloadPath)
{
    return 0;
}

int TaskCommitMgr::downLoad(QString mainAddr, QString userToken, QString dloadFilePath, QString bytesRange, QByteArray &fileData)
{
#if 1
    QMap<QString, QString> headers;
    headers.insert("cache-control", "no-cache");
    headers.insert("PARA_TOKEN", userToken);
    headers.insert("range", bytesRange);
    HttpRequest *request = new HttpRequest();
    request->setMethod("GET");
    request->setUrl(QUrl(mainAddr + QString("/api/pcs/file/download?path=prn:pcs:sftp:BSCC-A2:BSCC-A2:sc71306:file:") + dloadFilePath));
    request->setHeaders(headers);
    ///qDebug() << headers;
    ///qDebug() << request->url();

    HttpClient *client = new HttpClient();
    ///client->sslCheck();
    HttpResponse *response = client->send(request);
    ///qDebug() << "Headers:" << response->headers();
    qDebug() << "HTTP Status:" << response->status();
    ///qDebug() << "Body:" << response->body()->toString();
    //qDebug() << "Data:" << response->data();

    if (response->isSuccessful()) {
        qDebug() << "download successful";
        fileData = response->data();
//        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, response->body()->toString());
        return 1;
    } else {
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(1, response->body()->toString());
    }

    qDebug() << "download failed";
    return 0;
#else
    paraTask->downloadArchive(mainAddr, userToken, dloadFilePath, bytesRange, fileData);
#endif
}

int TaskCommitMgr::upLoad(const QString &mainAddr, const QString &userToken, const QString &uploadFilePath, const QString &bytesRange, const QByteArray &fileData)
{
#if 1
    QMap<QString, QString> headers;
    headers.insert("content-type", "application/octet-stream");
    headers.insert("cache-control", "no-cache");
    headers.insert("PARA_TOKEN", userToken);
    headers.insert("Content-Range", bytesRange);
    HttpRequest *request = new HttpRequest();
    HttpBody *body = new HttpBody(fileData);
    request->setBody(body);
    request->setMethod("POST");
    request->setUrl(QUrl(mainAddr + QString("/api/pcs/file/upload?path=prn:pcs:sftp:BSCC-A2:BSCC-A2:sc71306:file:") + uploadFilePath));
    request->setHeaders(headers);
    qDebug() << headers;

    HttpClient *client = new HttpClient();
    ///client->sslCheck();
    HttpResponse *response = client->send(request);
    ///qDebug() << "Headers:" << response->headers();
    qDebug() << "HTTP Status:" << response->status();
    qDebug() << "Body:" << response->body()->toString();
    delete client;

    if (response->isSuccessful()) {
        qDebug() << "uploaded successfully";
        ///Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, response->body()->toString());
        return 1;
    } else {
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(2, response->body()->toString());
    }
    return 0;

#else
    paraTask->uploadArchive(mainAddr, userToken, uploadFilePath, bytesRange, fileData);
#endif
}

int TaskCommitMgr::execute(QString serverAddr, QString userToken, QString taskName, QString workDir, QString bootScript, QString scriptParamter, QString &resBody)
{
    int issucess = paraTask->executeRemoteJob(serverAddr, userToken, taskName, workDir,bootScript,scriptParamter,resBody);
    Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, resBody); /// 整理info状态之后 在整理。
    return issucess;
}

int TaskCommitMgr::makeRemoteDir(QString mainAddr, QString userToken, QString pathName)
{
#if 1
    QJsonObject json;
    json.insert("path", "prn:pcs:sftp:BSCC-A2:BSCC-A2:sc71306:file:" + pathName);
    ///json.insert("recursive", "false");
    QString jsonData = QString(QJsonDocument(json).toJson(QJsonDocument::Compact));

    QMap<QString, QString> headers;
    headers.insert("content-type", "application/json");
    headers.insert("PARA_TOKEN", userToken);
    HttpRequest *request = new HttpRequest();
    HttpBody *body = new HttpBody(jsonData);
    request->setBody(body);
    request->setMethod("POST");
    request->setUrl(QUrl(mainAddr + QString("/api/pcs/file/mkdir")));
    request->setHeaders(headers);

    HttpClient *client = new HttpClient();
    ///client->sslCheck();
    HttpResponse *response = client->send(request);
    qDebug() << "Headers:" << response->headers();
    qDebug() << "HTTP Status:" << response->status();
    qDebug() << "Body:" << response->body()->toString();
    delete client;

    if (response->isSuccessful()) {
        qDebug() << "make remote dir successfully completed";
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, response->body()->toString());
        return 1;
    } else {
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(2, response->body()->toString());
    }
    return 0;

#else
    paraTask->makeRemoteDir(mainAddr, userToken, pathName);
#endif
}

int TaskCommitMgr::upzipRemoteArchive(QString mainAddr, QString userToken, QString taskPath, QString zipFilePath)
{
#if 1
    QJsonObject json;
    json.insert("dst", "prn:pcs:sftp:BSCC-A2:BSCC-A2:sc71306:file:" + taskPath);
    json.insert("src", "prn:pcs:sftp:BSCC-A2:BSCC-A2:sc71306:file:" + zipFilePath);
    QString jsonData = QString(QJsonDocument(json).toJson(QJsonDocument::Compact));
    qDebug() << jsonData;

    QMap<QString, QString> headers;
    headers.insert("content-type", "application/json");
    headers.insert("PARA_TOKEN", userToken);
    HttpRequest *request = new HttpRequest();
    HttpBody *body = new HttpBody(jsonData);
    request->setBody(body);
    request->setMethod("POST");
    request->setUrl(QUrl(mainAddr + QString("/api/pcs/tasks/file/unzip")));
    request->setHeaders(headers);

    HttpClient *client = new HttpClient();
    ///client->sslCheck();
    HttpResponse *response = client->send(request);
    qDebug() << "Headers:" << response->headers();
    qDebug() << "HTTP Status:" << response->status();
    qDebug() << "Body:" << response->body()->toString();
    delete client;

    if (response->isSuccessful()) {
        qDebug() << "unzip remote archive successfully completed";
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, response->body()->toString());
        return 1;
    } else {
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(2, response->body()->toString());
    }
    return 0;
#else
    paraTask->unzipRemoteArchive(mainAddr, userToken, taskPath, zipFilePath);
#endif
}

int TaskCommitMgr::doesRemoteFileExist(QString mainAddr, QString userToken, QString zipFilePath)
{
#if 1
    QMap<QString, QString> headers;
    headers.insert("cache-control", "no-cache");
    headers.insert("PARA_TOKEN", userToken);
    HttpRequest *request = new HttpRequest();
    request->setMethod("GET");
    request->setUrl(QUrl(mainAddr + QString("/api/pcs/file/info?path=prn:pcs:sftp:BSCC-A2:BSCC-A2:sc71306:file:") + zipFilePath));
    request->setHeaders(headers);

    HttpClient *client = new HttpClient();
    ///client->sslCheck();
    HttpResponse *response = client->send(request);
    ///qDebug() << "Headers:" << response->headers();
    qDebug() << "HTTP Status:" << response->status();
    qDebug() << "Body:" << response->body()->toString();
    ///qDebug() << "Data:" << response->data();

    if (response->isSuccessful()) {
        qDebug() << "get file info successfully";
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, response->body()->toString());
        return 1;
    } else {
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(2, response->body()->toString());
    }

    return 0;

#else

#endif
}

int TaskCommitMgr::zipRemotePath(QString mainAddr, QString userToken, QString dloadPath, QString zipFileName, QString workPath, QString subPath)
{
#if 1
    QJsonArray paraArray;
    paraArray.append(subPath);
    QJsonObject json;
    json.insert("dst", "prn:pcs:sftp:BSCC-A2:BSCC-A2:sc71306:file:" + dloadPath);
    json.insert("fileName", zipFileName);
    json.insert("workDir", "prn:pcs:sftp:BSCC-A2:BSCC-A2:sc71306:file:" + workPath);
    json.insert("subPaths", QJsonValue(paraArray));
    QString jsonData = QString(QJsonDocument(json).toJson(QJsonDocument::Compact));
    qDebug() << jsonData;

    QMap<QString, QString> headers;
    headers.insert("content-type", "application/json");
    headers.insert("PARA_TOKEN", userToken);
    HttpRequest *request = new HttpRequest();
    HttpBody *body = new HttpBody(jsonData);
    request->setBody(body);
    request->setMethod("POST");
    request->setUrl(QUrl(mainAddr + QString("/api/pcs/tasks/file/zip")));
    request->setHeaders(headers);

    HttpClient *client = new HttpClient();
    ///client->sslCheck();
    HttpResponse *response = client->send(request);
    qDebug() << "Headers:" << response->headers();
    qDebug() << "HTTP Status:" << response->status();
    qDebug() << "Body:" << response->body()->toString();
    delete client;

    if (response->isSuccessful()) {
        qDebug() << "zip remote path successfully completed";
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, response->body()->toString());
        return 0;
    } else {
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(2, response->body()->toString());
    }
    return -1;

#else
    paraTask->zipRemotePath(mainAddr, userToken, dloadPath, zipFileName, workPath, subPath);
#endif
}

int TaskCommitMgr::getRemoteJobInfo(QString serverAddr, QString userToken, QString jobId, QString &resBody)
{
    int re = paraTask->getRemoteJobInfo(serverAddr, userToken, jobId, resBody);
    Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, resBody); /// 同 执行
    return re;
}

int TaskCommitMgr::cancelRemoteJob(QString serverAddr, QString userToken, QString jobId, QString &resBody)
{
    int re = paraTask->cancelRemoteJob(serverAddr, userToken, jobId, resBody);
    Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, resBody); /// 同 执行
    return re;
}

int TaskCommitMgr::removeRemoteDir(QString serverAddr, QString userToken, QString remoteTaskDir, QString &resBody)
{
    int re = paraTask->removeRemoteDir(serverAddr, userToken, remoteTaskDir, resBody);
    Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, resBody); /// 同 执行
    return paraTask->removeRemoteDir(serverAddr, userToken, remoteTaskDir, resBody);
}

/**
 * @brief TaskCommitMgr::isLoginSucceed TODO:断网或未连接网的后续处理*
 * @return
 */
bool TaskCommitMgr::isLoginSucceed()
{
    return "" == Config->GetUserToken() ? false : true;
}

int TaskCommitMgr::appendTaskInfo(QString userId, QString taskName, QString status, int nodes, int cores, QString uploadPath, QString dloadPath)
{
    return paraTask->appendTaskInfo(userId, taskName, status, nodes, cores, uploadPath, dloadPath);
}

int TaskCommitMgr::updateTaskInfo(QString userId, QString taskName, QString jobId, QString status, QString startTime, QString endTime)
{
    return paraTask->updateTaskInfo(userId, taskName, jobId, status, startTime, endTime);
}

int TaskCommitMgr::updateTaskJobID(QString userId, QString taskName, QString jobId)
{
   return paraTask->updateTaskJobID(userId, taskName, jobId);
}

int TaskCommitMgr::updateTaskStatus(QString userId, QString taskName, QString status)
{
    return paraTask->updateTaskStatus(userId, taskName, status);
}

int TaskCommitMgr::updateStartTime(QString userId, QString taskName, QString startTime)
{
    return paraTask->updateStartTime(userId, taskName, startTime);
}

int TaskCommitMgr::updateEndTime(QString userId, QString taskName, QString endTime)
{
    return paraTask->updateEndTime(userId, taskName, endTime);
}

int TaskCommitMgr::updateCpuSize(QString userId, QString taskName, QString cupSize)
{
    return paraTask->updateCpuSize(userId, taskName, cupSize);
}

int TaskCommitMgr::removeTaskInfo(QString userId, QString taskName)
{
    return paraTask->removeTaskInfo(userId, taskName);
}

int TaskCommitMgr::readAllTaskInfo(QString userId, QString &jsonData)
{
    return paraTask->readAllTaskInfo(userId, jsonData);
}

int TaskCommitMgr::readTaskInfo(QString userId, QString taskName, QString &jsonData)
{
    return paraTask->readTaskInfo(userId, taskName, jsonData);
}
