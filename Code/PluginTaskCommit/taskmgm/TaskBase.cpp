#define ENABLE_DATABASE_SQLITE      1
#include "Dataface.hpp"
#include "TaskBase.h"
#include "ParaTask.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QCoreApplication>


TaskBase * TaskBase::Create(const int type)
{
    switch (type) {
    case CLOUD_PARATERA:
        return new ParaTask;
    default:
        perror("that is unsupported or nonexistant.\nCheck your config and try again.");
        return NULL;
    }
}

int TaskBase::sendPostRequest(QString urlPath, QMap<QString, QString> reqHeaders, QByteArray reqBody, HttpResponse **response)
{
    HttpRequest *request = new HttpRequest();
    HttpBody *body = new HttpBody(reqBody);
    request->setBody(body);
    request->setMethod("POST");
    request->setUrl(QUrl(urlPath));
    request->setHeaders(reqHeaders);

    HttpClient *client = new HttpClient();
    ///client->sslCheck();
    *response = client->send(request);
    delete client;
    delete body;
    delete request;
    if ((*response)->isSuccessful()) {
        return 1;
    }

    return 0;
}

int TaskBase::sendGetRequest(QString urlPath, QMap<QString, QString> reqHeaders, HttpResponse **response)
{
    HttpRequest *request = new HttpRequest();
    request->setMethod("GET");
    request->setUrl(QUrl(urlPath));
    request->setHeaders(reqHeaders);

    HttpClient *client = new HttpClient();
    ///client->sslCheck();
    *response = client->send(request);
    delete client;
    delete request;
    if ((*response)->isSuccessful()) {
        return 1;
    }

    return 0;
}

int TaskBase::sendPutRequest(QString urlPath, QMap<QString, QString> reqHeaders, HttpResponse **response)
{
    HttpRequest *request = new HttpRequest();
    request->setMethod("PUT");
    request->setUrl(QUrl(urlPath));
    request->setHeaders(reqHeaders);

    HttpClient *client = new HttpClient();
    ///client->sslCheck();
    *response = client->send(request);
    delete client;
    delete request;
    if ((*response)->isSuccessful()) {
        return 1;
    }

    return 0;
}

int TaskBase::sendDeleteRequest(QString urlPath, QMap<QString, QString> reqHeaders, HttpResponse **response)
{
    HttpRequest *request = new HttpRequest();
    request->setMethod("DELETE");
    request->setUrl(QUrl(urlPath));
    request->setHeaders(reqHeaders);

    HttpClient *client = new HttpClient();
    ///client->sslCheck();
    *response = client->send(request);
    delete client;
    delete request;
    if ((*response)->isSuccessful()) {
        return 1;

   }

    return 0;
}

int TaskBase::appendTaskInfo(QString userId, QString taskName, QString status, int nodes, int cores, QString uploadPath, QString dloadPath)
{
    Database * taskSQL;
    taskSQL = easydb::CreateDatabaseInterface(easydb::DB_SQLITE);
    if (NULL == taskSQL) {
        qDebug() << "failed to create database instace";
        return -1;
    }
    QString appDir = QCoreApplication::applicationFilePath();
    appDir = appDir.left(appDir.lastIndexOf("/"));
    QString dataPath = appDir + "/taskinfo.db";
    bool ret = taskSQL->Initialize(0, 0, 0, 0, dataPath.toStdString().c_str());
    if (!ret) {
        qDebug() << "failed to open database";
        delete taskSQL;
        return -1;
    }

    ret = taskSQL->WaitExecute("INSERT INTO t_task_info (user_id, task_name, status, nodes, cores, upload_path, dload_path, create_time) VALUES(\'%s\', \'%s\', \'%s\', %u, %u, \'%s\', \'%s\', \'%s\' )",
                               userId.toStdString().c_str(),
                               taskName.toStdString().c_str(),
                               status.toStdString().c_str(),
                               nodes,
                               cores,
                               uploadPath.toStdString().c_str(),
                               dloadPath.toStdString().c_str(),
                               QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz").toStdString().c_str());

    delete taskSQL;
    if (!ret) return -1;
    return 0;
}

int TaskBase::updateTaskInfo(QString userId, QString taskName, QString jobId, QString status, QString startTime, QString endTime)
{
    Database * taskSQL;
    taskSQL = easydb::CreateDatabaseInterface(easydb::DB_SQLITE);
    if (NULL == taskSQL) {
        qDebug() << "failed to create database instace";
        return -1;
    }

    QString appDir = QCoreApplication::applicationFilePath();
    appDir = appDir.left(appDir.lastIndexOf("/"));
    QString dataPath = appDir + "/taskinfo.db";
    bool ret = taskSQL->Initialize(0, 0, 0, 0, dataPath.toStdString().c_str());
    if (!ret) {
        qDebug() << "failed to open database";
        delete taskSQL;
        return -1;
    }

    taskSQL->WaitExecute("UPDATE t_task_info SET job_id=\'%s\',status=\'%s\',start_time=\'%s\',end_time=\'%s\' WHERE user_id=\'%s\' AND task_name=\'%s\' ",
                         jobId.toStdString().c_str(),
                         status.toStdString().c_str(),
                         startTime.toStdString().c_str(),
                         endTime.toStdString().c_str(),
                         userId.toStdString().c_str(),
                         taskName.toStdString().c_str());
    delete taskSQL;
    return 0;
}

int TaskBase::updateTaskJobID(QString userId, QString taskName, QString jobId)
{
    Database * taskSQL;
    taskSQL = easydb::CreateDatabaseInterface(easydb::DB_SQLITE);
    if (NULL == taskSQL) {
        qDebug() << "failed to create database instace";
        return -1;
    }

    QString appDir = QCoreApplication::applicationFilePath();
    appDir = appDir.left(appDir.lastIndexOf("/"));
    QString dataPath = appDir + "/taskinfo.db";
    bool ret = taskSQL->Initialize(0, 0, 0, 0, dataPath.toStdString().c_str());
    if (!ret) {
        qDebug() << "failed to open database";
        delete taskSQL;
        return -1;
    }

    taskSQL->WaitExecute("UPDATE t_task_info SET job_id=\'%s\' WHERE user_id=\'%s\' AND task_name=\'%s\' ",
                         jobId.toStdString().c_str(),
                         userId.toStdString().c_str(),
                         taskName.toStdString().c_str());
    delete taskSQL;
    return 0;
}

int TaskBase::updateTaskStatus(QString userId, QString taskName, QString status)
{
    Database * taskSQL;
    taskSQL = easydb::CreateDatabaseInterface(easydb::DB_SQLITE);
    if (NULL == taskSQL) {
        qDebug() << "failed to create database instace";
        return -1;
    }

    QString appDir = QCoreApplication::applicationFilePath();
    appDir = appDir.left(appDir.lastIndexOf("/"));
    QString dataPath = appDir + "/taskinfo.db";
    bool ret = taskSQL->Initialize(0, 0, 0, 0, dataPath.toStdString().c_str());
    if (!ret) {
        qDebug() << "failed to open database";
        delete taskSQL;
        return -1;
    }

    taskSQL->WaitExecute("UPDATE t_task_info SET status=\'%s\' WHERE user_id=\'%s\' AND task_name=\'%s\' ",
                         status.toStdString().c_str(),
                         userId.toStdString().c_str(),
                         taskName.toStdString().c_str());
    delete taskSQL;
    return 0;
}

int TaskBase::updateStartTime(QString userId, QString taskName, QString startTime)
{
    Database * taskSQL;
    taskSQL = easydb::CreateDatabaseInterface(easydb::DB_SQLITE);
    if (NULL == taskSQL) {
        qDebug() << "failed to create database instace";
        return -1;
    }

    QString appDir = QCoreApplication::applicationFilePath();
    appDir = appDir.left(appDir.lastIndexOf("/"));
    QString dataPath = appDir + "/taskinfo.db";
    bool ret = taskSQL->Initialize(0, 0, 0, 0, dataPath.toStdString().c_str());
    if (!ret) {
        qDebug() << "failed to open database";
        delete taskSQL;
        return -1;
    }

    taskSQL->WaitExecute("UPDATE t_task_info SET start_time=\'%s\' WHERE user_id=\'%s\' AND task_name=\'%s\' ",
                         startTime.toStdString().c_str(),
                         userId.toStdString().c_str(),
                         taskName.toStdString().c_str());
    delete taskSQL;
    return 0;
}

int TaskBase::updateEndTime(QString userId, QString taskName, QString endTime)
{
    Database * taskSQL;
    taskSQL = easydb::CreateDatabaseInterface(easydb::DB_SQLITE);
    if (NULL == taskSQL) {
        qDebug() << "failed to create database instace";
        return -1;
    }

    QString appDir = QCoreApplication::applicationFilePath();
    appDir = appDir.left(appDir.lastIndexOf("/"));
    QString dataPath = appDir + "/taskinfo.db";
    bool ret = taskSQL->Initialize(0, 0, 0, 0, dataPath.toStdString().c_str());
    if (!ret) {
        qDebug() << "failed to open database";
        delete taskSQL;
        return -1;
    }

    taskSQL->WaitExecute("UPDATE t_task_info SET end_time=\'%s\' WHERE user_id=\'%s\' AND task_name=\'%s\' ",
                         endTime.toStdString().c_str(),
                         userId.toStdString().c_str(),
                         taskName.toStdString().c_str());
    delete taskSQL;
    return 0;
}

int TaskBase::updateCpuSize(QString userId, QString taskName, QString CpuSize)
{
    Database * taskSQL;
    taskSQL = easydb::CreateDatabaseInterface(easydb::DB_SQLITE);
    if (NULL == taskSQL) {
        qDebug() << "failed to create database instace";
        return -1;
    }

    QString appDir = QCoreApplication::applicationFilePath();
    appDir = appDir.left(appDir.lastIndexOf("/"));
    QString dataPath = appDir + "/taskinfo.db";
    bool ret = taskSQL->Initialize(0, 0, 0, 0, dataPath.toStdString().c_str());
    if (!ret) {
        qDebug() << "failed to open database";
        delete taskSQL;
        return -1;
    }

    taskSQL->WaitExecute("UPDATE t_task_info SET cores=\'%s\' WHERE user_id=\'%s\' AND task_name=\'%s\' ",
                         CpuSize.toStdString().c_str(),
                         userId.toStdString().c_str(),
                         taskName.toStdString().c_str());
    delete taskSQL;
    return 0;
}

int TaskBase::removeTaskInfo(QString userId, QString taskName)
{
    Database * taskSQL;
    taskSQL = easydb::CreateDatabaseInterface(easydb::DB_SQLITE);
    if (NULL == taskSQL) {
        qDebug() << "failed to create database instace";
        return -1;
    }

    QString appDir = QCoreApplication::applicationFilePath();
    appDir = appDir.left(appDir.lastIndexOf("/"));
    QString dataPath = appDir + "/taskinfo.db";
    bool ret = taskSQL->Initialize(0, 0, 0, 0, dataPath.toStdString().c_str());
    if (!ret) {
        qDebug() << "failed to open database";
        delete taskSQL;
        return -1;
    }

    taskSQL->WaitExecute("DELETE FROM t_task_info WHERE user_id=\'%s\' AND task_name=\'%s\' ",
                         userId.toStdString().c_str(),
                         taskName.toStdString().c_str());
    delete taskSQL;
    return 0;
}

int TaskBase::readAllTaskInfo(QString userId, QString &jsonData)
{
    Database * taskSQL;
    taskSQL = easydb::CreateDatabaseInterface(easydb::DB_SQLITE);
    if (NULL == taskSQL) {
        qDebug() << "failed to create database instace";
        return -1;
    }
    QString appDir = QCoreApplication::applicationFilePath();
    appDir = appDir.left(appDir.lastIndexOf("/"));
    QString dataPath = appDir + "/taskinfo.db";
    bool ret = taskSQL->Initialize(0, 0, 0, 0, dataPath.toStdString().c_str());
    if (!ret) {
        qDebug() << "failed to open database";
        delete taskSQL;
        return -1;
    }

    QueryResult * result = taskSQL->Query("SELECT * FROM t_task_info WHERE user_id=\'%s\' ", userId.toStdString().c_str());

    Field * field;
    QJsonObject json;
    QJsonArray paraArray;
    if (result) {
        do {
            field = result->Fetch();
            json.insert("task_name", field[1].GetString());
            json.insert("job_id", field[2].GetString());
            json.insert("status", field[3].GetString());
            json.insert("nodes", field[4].GetString());
            json.insert("cores", field[5].GetString());
            json.insert("upload_path", field[6].GetString());
            json.insert("dload_path", field[7].GetString());
            json.insert("create_time", field[8].GetString());
            json.insert("start_time", field[9].GetString());
            json.insert("end_time", field[10].GetString());
            paraArray.append(QJsonValue(json));
        } while (result->NextRow());

        jsonData = QString(QJsonDocument(paraArray).toJson(QJsonDocument::Compact));
        taskSQL->FreeQueryResult(result);
    }

    delete taskSQL;
    return 0;
}

int TaskBase::readTaskInfo(QString userId, QString taskName, QString &jsonData)
{
    Database * taskSQL;
    taskSQL = easydb::CreateDatabaseInterface(easydb::DB_SQLITE);
    if (NULL == taskSQL) {
        qDebug() << "failed to create database instace";
        return -1;
    }

    QString appDir = QCoreApplication::applicationFilePath();
    appDir = appDir.left(appDir.lastIndexOf("/"));
    QString dataPath = appDir + "/taskinfo.db";
    bool ret = taskSQL->Initialize(0, 0, 0, 0, dataPath.toStdString().c_str());
    if (!ret) {
        qDebug() << "failed to open database";
        delete taskSQL;
        return -1;
    }

    QueryResult * result = taskSQL->Query("SELECT * FROM t_task_info WHERE user_id=\'%s\' AND task_name=\'%s\' ",
                                          userId.toStdString().c_str(),
                                          taskName.toStdString().c_str());
    Field * field;
    QJsonObject json;
    if (result) {
        field = result->Fetch();
        json.insert("task_name", field[1].GetString());
        json.insert("job_id", field[2].GetString());
        json.insert("status", field[3].GetString());
        json.insert("nodes", field[4].GetString());
        json.insert("cores", field[5].GetString());
        json.insert("upload_path", field[6].GetString());
        json.insert("dload_path", field[7].GetString());
        json.insert("start_time", field[8].GetString());
        json.insert("end_time", field[9].GetString());

        jsonData = QString(QJsonDocument(json).toJson(QJsonDocument::Compact));
        taskSQL->FreeQueryResult(result);
    }

    delete taskSQL;
    return 0;
}

/// 创建和删除任务对象
extern "C"
{
    TaskBase * CreateTaskBase(const int type)
    {
        return TaskBase::Create(type);
    }

    void ReleaseTaskBase(TaskBase *task)
    {
        delete task;
        task = NULL;
    }
}
