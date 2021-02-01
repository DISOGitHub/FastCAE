#ifndef __GLOBAL_SETTING_HPP__
#define __GLOBAL_SETTING_HPP__

#include <QDebug>
#include <QFile>
#include <QXmlStreamReader>
#include <QSettings>
#include <QApplication>
///#include "Logger.hpp"
///#include "httpclient.hpp"
///using namespace Restful::Http;

/**
 * @brief 单实例类定义实现时需加入的宏
 * @param class_name 类名
 * @param ret_type 获取实例时返回类型
 */
#define DECL_SIGNLETON( class_name ,ret_type )                      \
    public:     static ret_type Instance(void)      \
{                                                           \
    static class_name _theInstance;                         \
    return &_theInstance;                                   \
    }


class GlobalConfig
{
    DECL_SIGNLETON(GlobalConfig, GlobalConfig*)
    public:
        GlobalConfig()
    {
        userId = "100";
        jobId = "0";
        appDir = QCoreApplication::applicationFilePath();
        appDir = appDir.left(appDir.lastIndexOf("/"));
        configDir = appDir + "/config";
        LoadServerInfo();
        LoadTaskInfo();
    }

    virtual ~GlobalConfig() {}

public:
    inline QString GetMainServerName()
    {
        return mainAddr;
    }

    inline QString GetUserServerName()
    {
        return userAddr;
    }

    inline QString GetUserName()
    {
        return userName;
    }

    inline QString GetHashKey()
    {
        return hashKey;
    }

    inline QString GetZipFileName()
    {
        return taskName + ".zip";
    }

    inline QString GetResultFileName()
    {
        return taskName + "_result.zip";
    }

    inline QString GetZipDir()
    {
        return zipPath;
    }

    inline QString GetUnzipDir()
    {
        return unzipPath;
    }

    inline QString GetUploadPath()
    {
        return uploadPath;
    }

    inline QString GetDownloadPath()
    {
        return dloadPath;
    }

    inline QString GetUploadFilePath()
    {
        return uploadPath + "/" + GetZipFileName();
    }

    inline QString GetDownloadFilePath()
    {
        return dloadPath + "/" + GetResultFileName();
    }

    inline QString GetSolverPath()
    {
        return solverDir;
    }

    inline QString GetConfigDir()
    {
        return configDir;
    }

    inline void SetUserToken(QString token)
    {
        userToken = token;
    }

    inline QString GetUserToken()
    {
        return userToken;
    }
    
    inline void SetAuthHeaders(QMap<QString, QString> headers)
    {
        authHeaders = headers;
        ///qDebug() << authHeaders;
    }
    
    inline QMap<QString, QString> GetAuthHeaders()
    {
        return authHeaders;
    }
    
    inline QString GetBootScript()
    {
        return bootScript;
    }

    inline void SetUserId(QString id)
    {
        userId = id;
    }

    inline QString GetUserId()
    {
        return userId;
    }

    inline QString GetAppDir()
    {
        return appDir;
    }

    inline QString GetTaskName()
    {
        return taskName;
    }

    inline QString GetJobId()
    {
        return jobId;
    }

    inline void SetJobId(QString id)
    {
        jobId = id;
    }

    bool getAutoLogin() const
    {
        return autoLogin;
    }

    void setAutoLogin(bool value)
    {
        autoLogin = value;
    }

    QString getUser() const
    {
        return user;
    }

    void setUser(const QString &value)
    {
        user = value;
    }

    QString getPassword() const
    {
        return password;
    }

    void setPassword(const QString &value)
    {
        password = value;
    }

    int getDownTimeout() const
    {
        return downTimeout;
    }

    void setDownTimeout(int value)
    {
        downTimeout = value;
    }

    int getReqTimeout() const
    {
        return reqTimeout;
    }

    void setReqTimeout(int value)
    {
        reqTimeout = value;
    }

    int getAvailableTime() const
    {
        return availableTime;
    }

    void setAvailableTime(int value)
    {
        availableTime = value;
    }


public:
    int LoadServerInfo()
    {
        QFile xmlFile(configDir + "/server.conf");
        if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "failed to read server.conf";
            return -1;
        }

        QXmlStreamReader xmlReader(&xmlFile);
        xmlReader.readNext();
        while(!xmlReader.atEnd()) {
            if (xmlReader.isStartElement()) {
                if (("Server" == xmlReader.name()) &&
                        (xmlReader.attributes().hasAttribute("MainAddr")) &&
                        (xmlReader.attributes().hasAttribute("UserAddr")) &&
                        (xmlReader.attributes().hasAttribute("UserName")) &&
                        (xmlReader.attributes().hasAttribute("SecureKey"))) {
                    mainAddr = xmlReader.attributes().value("MainAddr").toString();
                    userAddr = xmlReader.attributes().value("UserAddr").toString();
                    userName = xmlReader.attributes().value("UserName").toString();
                    hashKey = xmlReader.attributes().value("SecureKey").toString();
                }
            }

            xmlReader.readNext();
        }

        xmlFile.close();
        return 0;
    }

    int LoadTaskInfo()
    {
        QString iniConfig = configDir + "/task.ini";
        QFile iniFile(iniConfig);
        if (!iniFile.exists()) {
            qDebug() << "failed to read task.ini";
            return -1;
        }

        QSettings iniRead(iniConfig, QSettings::IniFormat);
        taskName = iniRead.value("/task/job_name").toString();
        zipPath = iniRead.value("/task/local_zip_path").toString();
        unzipPath = iniRead.value("/task/local_unzip_path").toString();
        uploadPath = iniRead.value("/task/remote_upload_path").toString();
        dloadPath = iniRead.value("/task/remote_download_path").toString();
        solverDir = iniRead.value("/task/solver_path").toString();
        bootScript = iniRead.value("/task/boot_script").toString();
        autoLogin = iniRead.value("/task/auto").toString().toInt();
        user = iniRead.value("/task/user").toString();
        password = iniRead.value("/task/password").toString();
        downTimeout = iniRead.value("/task/downtimeout").toString().toInt();
        reqTimeout = iniRead.value("/task/reqtimeout").toString().toInt();
        availableTime = iniRead.value("/task/availableTime").toString().toInt();

        ///uploadFilePath = uploadPath + "/" + taskName + ".zip";
        ///dloadFilePath = dloadPath + "/" + taskName + "_r.zip";
        return 0;
    }

private:
    QString appDir;
    QString configDir;
    QString userToken{""};
    QString userId;
    QMap<QString, QString> authHeaders;
    ///server info
    QString mainAddr;
    QString userAddr;
    QString userName;
    QString hashKey;
    ///task info
    QString taskName;
    QString zipPath;
    QString unzipPath;
    QString uploadPath;
    QString dloadPath;
    ///QString uploadFilePath;
    ///QString dloadFilePath;
    QString solverDir;
    QString bootScript;
    QString jobId;
    ///login
    bool autoLogin;
    QString user;
    QString password;
    /// time
    int downTimeout;
    int reqTimeout;
    int availableTime;
};

#define Config      GlobalConfig::Instance()

#endif
