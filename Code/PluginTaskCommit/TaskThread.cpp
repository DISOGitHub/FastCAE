#include "TaskThread.h"
#include "taskmgm/TaskCommit.h"
#include "quazip/include/JlCompress.h"
#include "IO/IOConfig.h"
#include "ModelData/modelDataSingleton.h"
#include "ModelData/modelDataBase.h"
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QElapsedTimer>
#include <QWaitCondition>
#include <QMutex>
#include <QMutexLocker>

void ResultRunable::run()
{
    int fileSize;
    QByteArray fileData;
    QString mainAddr = Config->GetMainServerName();
    QString userToken = Config->GetUserToken();

    QString dloadPath = Config->GetDownloadPath(); /// public3/home/sc71306/PHengLEI/PHengLEICase
    QString zipFileName = Config->GetResultFileName();
    QString workPath = Config->GetSolverPath();
    QString subPath = "results";
    QString downFileName = jobName + "_result.zip";


    int ret = TaskCommitMgr::zipRemotePath(mainAddr, userToken, dloadPath + "/" + jobName, downFileName, dloadPath + "/" + jobName, subPath);
    if (ret != 0) {
        qDebug() << "failed to zip remote path";
        emit signalDownState(jobName, TASK_STATE_DRESULTFAILED);
        return ;
    }
//    dloadFilePath = dloadPath + "/" + downFileName;
    QString dloadFilePath = dloadPath+ "/" + jobName + "/" + downFileName;
    int stime = 5000;
    int maxtimes = Config->getDownTimeout() / stime;
    int curtims = 0;
    do{
        Sleep(5000);
        ret = TaskCommitMgr::getRemoteFileSize(mainAddr, userToken,dloadFilePath , fileSize);
        ++curtims;
    } while (ret != 0 & curtims <= maxtimes);

    if (ret != 0) {
        qDebug() << "failed to get remote file size";
        Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(2, "failed to get remote file size");
        emit signalDownState(jobName, TASK_STATE_DRESULTFAILED);
        return ;
    }

    qDebug() << fileSize;
    QString df = filePath + "/"+ downFileName;
    qDebug() << df;
    QFile file(df);
    if (!file.open(QIODevice::WriteOnly))
    {
        emit signalDownState(jobName, TASK_STATE_DRESULTFAILED);
        return;
    }
    int step = fileSize*0.1;
    // taskName + "_result.zip";
    emit signalFileSize(jobName ,fileSize);

    double senedSize = 0;
    QElapsedTimer mstimer;

    for(int i = 0; i < fileSize; i += step) {
        int start = i;
        int end = fileSize - 1;
        if (i + step <= fileSize) {
            end = i + step - 1;
        }

        QString bytesRange = QString("bytes=%1-%2").arg(start).arg(end);
        qDebug() << bytesRange;
        mstimer.start();
        if(0 == TaskCommitMgr::downLoad(mainAddr, userToken, dloadFilePath, bytesRange, fileData))
        {
            qDebug() << "down error";
            emit signalDownState(jobName, TASK_STATE_DRESULTFAILED);
            file.close();
            return;
        }
        float time = (double)mstimer.nsecsElapsed() / (double)1000000000;
        qDebug() << "fileData.size = " << fileData.size();
        qDebug() << "time = " << time;
        int speed = fileData.size() / 1024 / time;
        emit signalSpeedSize(jobName , QString("%1k/s").arg(speed));
        qDebug() << "fileSize = " << fileSize;
        senedSize += fileData.size();
        double r = senedSize / fileSize;
        r *= 100;
        qDebug() << "r = " << r;
        emit signalDownedSize(jobName ,r);
        file.write(fileData);
    }
    emit signalDownedSize(jobName ,100);
    file.close();
    qDebug() << "df: " << filePath;
    QString userId = Config->GetUserId();
    TaskCommitMgr::updateTaskStatus(userId, jobName, QString("%1").arg(TASK_STATE_DRESULTCOMPLETE));
    JlCompress::extractDir(df, filePath);
    QString msg = "task name:" + jobName + ",savedpath:" + filePath;
    /// qDebug() << msg;
    emit signalDownState(jobName, TASK_STATE_DRESULTCOMPLETE);
    Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, msg);
}

const QString& ResultRunable::getJobName() const
{
    return jobName;
}

void ResultRunable::setJobName(const QString &value)
{
    jobName = value;
}

QString ResultRunable::getFilePath() const
{
    return filePath;
}

void ResultRunable::setFilePath(const QString &value)
{
    filePath = value;
}

void ExecuteRunable::run()
{
    QString serverAddr = Config->GetMainServerName();
    QString userToken = Config->GetUserToken();
    /// 执行同步作业
    QString solverDir = Config->GetSolverPath();
    QString bootScript = Config->GetBootScript();
    QString scriptParamter = "cpu=1";
    QString taskName = Config->GetTaskName();
    QString resBody;
    int ret = TaskCommitMgr::execute(serverAddr, userToken, jobName, solverDir, bootScript, jobName, resBody);
    if (1 != ret) {
        qDebug() << "failed to execute synchronization job: " << resBody;
        emit signalExecuteState(jobName, TASK_STATE_CALCFAILED, 0);
        return ;
    }

    /// TODO: 用正则进行匹配/解析json
    QString time;
    for(int j = 0; j < resBody.length(); j++)
    {
        if(resBody[j] >= '0' && resBody[j] <= '9')
            time.append(resBody[j]);
    }
    qDebug() << "resBody: " << resBody;
    emit signalExecuteJobID(jobName, time.toInt());
    qDebug() << "jobid: " << time;
    QString userId = Config->GetUserId();
    //QString taskName = Config->GetTaskName();
    QString status = QString::number(TASK_STATE_CALCING);
    ret = TaskCommitMgr::updateTaskInfo(userId, jobName, time, status, "0", "0");
    if (ret != 0)
    {
        qDebug() << "failed to update task info";
    }
    emit signalExecuteState(jobName, (int)TASK_STATE_CALCING, time.toInt());
#if 0
    ///
    QString mainAddr = Config->GetMainServerName();
    if(1 != TaskCommitMgr::getRemoteJobInfo(mainAddr, userToken, time, resBody))
    {
//        tptr->setState(TASK_STATE_SELFAILED);
        ret = TaskCommitMgr::updateTaskInfo(userId, jobName, time, status, "0", "0");
        if (ret != 0)
        {
            qDebug() << "failed to update task info";
        }

    } else {
        /// 取出结果信息
            QString status, startTime, endTime;
            QJsonParseError err;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(resBody.toUtf8(), &err);
            if (QJsonParseError::NoError == err.error) {
                if (jsonDoc.isObject()) {
                    QVariantMap result = jsonDoc.toVariant().toMap();
                    status = result["status"].toString();
                    startTime = result["startTime"].toString();
                    endTime = result["endTime"].toString();
                }
            }
            TASK_STATE ts = TASK_STATE_NON;
            if ("done" == status.toLower())
            {
                ts = TASK_STATE_CALCOMPLETE;
//                tptr->setUsedCPUTime(endTime.toULongLong()-startTime.toULongLong());
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
//            tptr->setState(ts);
            QString userId = Config->GetUserId();
            int ret = TaskCommitMgr::updateTaskInfo(userId, jobName, time, QString("%1").arg(ts), startTime, endTime);
            if (ret != 0) {
                qDebug() << "failed to update task info";
            }
    }
#endif
}

const QString& ExecuteRunable::getJobName() const
{
    return jobName;
}

void ExecuteRunable::setJobName(const QString &value)
{
    jobName = value;
}

QString ExecuteRunable::getFilePath() const
{
    return filePath;
}

void ExecuteRunable::setFilePath(const QString &value)
{
    filePath = value;
}

void CommitRunable::run()
{
    std::vector<bool> uloadState;
    QString mainAddr = Config->GetMainServerName();
    QString userToken = Config->GetUserToken();
    /// 上传打包文件
    QString jobZip = jobName + "_up.zip";
    QString uploadFilePath = Config->GetUploadPath() + "/" + jobZip;

	//复制需要上传的文件
	QDir sourceDir(filePath + "/bin");
	QString targetstr(filePath + "/UpLoad");
	this->clearFolder(targetstr);

	QString srcbin{ filePath + "/bin" };
	QString desbin{ filePath + "/UpLoad/bin" };
	this->clearFolder(desbin);
	QString srcgrid{ filePath + "/grid" };
	QString desgrid{ filePath + "/UpLoad/grid" };
	this->clearFolder(desgrid);
	if (!(this->copyDirectoryFiles(srcbin, desbin, true) && this->copyDirectoryFiles(srcgrid, desgrid, true)))
    {
        emit signalCommitState(jobName, TASK_STATE_UCOMMITEDFAILED);
        return;
    }


	QFile::copy(filePath + "/start.sh", filePath + "/UpLoad/start.sh");
	filePath = filePath + "/UpLoad";
   
	
	
	qDebug() << "compressDir : " << JlCompress::compressDir(jobZip, filePath);
    QString fd = filePath + "/" + jobZip;
    QFile file(jobZip);
    int fileSize = file.size();
    qDebug() << fileSize;
    if (fileSize <= 0)
    {
        emit signalCommitState(jobName, TASK_STATE_UCOMMITEDFAILED);
        return;
    }
    if(!file.open(QIODevice::ReadOnly))
    {
        emit signalCommitState(jobName, TASK_STATE_UCOMMITEDFAILED);
        return;
    }
    int step = fileSize*0.1;
    emit signalFileSize(jobName ,fileSize);
    double senedSize = 0;
    QElapsedTimer mstimer;

    for(int i = 0; i < fileSize; i += step) {
        int start = i;
        int end = fileSize - 1;
        if (i + step <= fileSize) {
            end = i + step - 1;
        }

        QString bytesRange = QString("bytes %1-%2/%3").arg(start).arg(end).arg(fileSize);
        qDebug() << bytesRange;
        QByteArray fileData = file.read(step);
        qDebug() << fileData.length();
        mstimer.start();
        if(0 == TaskCommitMgr::upLoad(mainAddr, userToken, uploadFilePath, bytesRange, fileData))
        {
            emit signalCommitState(jobName, TASK_STATE_UCOMMITEDFAILED);
            return;
        }
        float time = (double)mstimer.nsecsElapsed() / (double)1000000000;
        int speed = fileData.size() / 1024 / time;
        emit signalSpeedSize(jobName , QString("%1k/s").arg(speed));

        qDebug() << "fileSize = " << fileSize;
        senedSize += fileData.size();
        double r = senedSize / fileSize;
        r *= 100;
        qDebug() << "r = " << r;
        emit signalCommitedSize(jobName ,r);
    }
    file.close();
    /// 判断zip文件是否存在
    int ret =TaskCommitMgr::doesRemoteFileExist(mainAddr, userToken, uploadFilePath);
    if (ret == 0) {
        qDebug() << "zip file is not exist";
        emit signalCommitState(jobName, TASK_STATE_UCOMMITEDFAILED);
        return ;
    }
    /// 在远端以任务名创建目录
    QString taskPath = Config->GetUploadPath() + "/" + jobName;
    ret = TaskCommitMgr::makeRemoteDir(mainAddr, userToken, taskPath);
    if (ret == 0) {
        qDebug() << "failed to make remote dir";
        emit signalCommitState(jobName, TASK_STATE_UCOMMITEDFAILED);
        return ;
    }
    /// 解压到指定路径(uploadPath + taskName)
    ret = TaskCommitMgr::upzipRemoteArchive(mainAddr, userToken, taskPath, uploadFilePath);
    if (ret == 0) {
        qDebug() << "failed to upzip remote archive";
        emit signalCommitState(jobName, TASK_STATE_UCOMMITEDFAILED);
        return ;
    }
    emit signalCommitState(jobName, TASK_STATE_UCOMMITEDCOMPLETE);
    QString reBody;
    QString userId = Config->GetUserId();
    if(TaskCommitMgr::updateTaskInfo(userId, jobName, "0", QString("%1").arg(TASK_STATE_UCOMMITEDCOMPLETE), "0", "0"))
    {

    }

    Plugins::TaskCommitPlugin::_taskCommitPlugin->printMessage(0, jobName + ",upload path:" + uploadFilePath); /// 整理info状态之后 在整理。
    ret = TaskCommitMgr::removeRemoteDir(mainAddr, userToken, uploadFilePath, reBody);
    if (ret == 0) {
        qDebug() << "failed to upzip remote archive :" << reBody;
        ///emit signalCommitState(jobName, TASK_STATE_UCOMMITEDFAILED);
        return ;
    }
}

const QString &CommitRunable::getJobName() const
{
    return jobName;
}

void CommitRunable::setJobName(const QString &value)
{
    jobName = value;
}

QString CommitRunable::getFilePath() const
{
    return filePath;
}

void CommitRunable::setFilePath(const QString &value)
{
    filePath = value;
}
bool CommitRunable::copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist)
{
	QDir sourceDir(fromDir);
	QDir targetDir(toDir);
	if (!targetDir.exists()) {    /**< 如果目标目录不存在，则进行创建 */
		if (!targetDir.mkdir(targetDir.absolutePath()))
			return false;
	}

	QFileInfoList fileInfoList = sourceDir.entryInfoList();
	for (QFileInfo fileInfo : fileInfoList) {
		if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
			continue;

		if (fileInfo.isDir()) {    /**< 当为目录时，递归的进行copy */
			if (!copyDirectoryFiles(fileInfo.filePath(),
				targetDir.filePath(fileInfo.fileName()),
				coverFileIfExist))
				return false;
		}
		else {            /**< 当允许覆盖操作时，将旧文件进行删除操作 */
			if (coverFileIfExist && targetDir.exists(fileInfo.fileName())) {
				targetDir.remove(fileInfo.fileName());
			}

			/// 进行文件copy
			if (!QFile::copy(fileInfo.filePath(),
				targetDir.filePath(fileInfo.fileName()))) {
				return false;
			}
		}
	}
	return true;
}

void CommitRunable::clearFolder(const QString &folderFullPath)
{
	QDir dir(folderFullPath);
	if (!dir.exists()) {
		if (!dir.mkdir(dir.absolutePath()))
			return;
	}
	else {

		QFileInfoList    fileList;
		QFileInfo        curFile;
		QFileInfoList    fileListTemp;
		int           infoNum;
		int           i;
		int           j;

		fileList = dir.entryInfoList(QDir::Dirs | QDir::Files
			| QDir::Readable | QDir::Writable
			| QDir::Hidden | QDir::NoDotAndDotDot
			, QDir::Name);

		while (fileList.size() > 0)
		{
			infoNum = fileList.size();

			for (i = infoNum - 1; i >= 0; i--)
			{
				curFile = fileList[i];
				if (curFile.isFile())
				{
					QFile fileTemp(curFile.filePath());
					fileTemp.remove();
					fileList.removeAt(i);
				}

				if (curFile.isDir())
				{
					QDir dirTemp(curFile.filePath());
					fileListTemp = dirTemp.entryInfoList(QDir::Dirs | QDir::Files
						| QDir::Readable | QDir::Writable
						| QDir::Hidden | QDir::NoDotAndDotDot
						, QDir::Name);
					if (fileListTemp.size() == 0)
					{
						dirTemp.rmdir(".");
						fileList.removeAt(i);
					}
					else
					{
						for (j = 0; j < fileListTemp.size(); j++)
						{
							if (!(fileList.contains(fileListTemp[j])))
							{
								fileList.append(fileListTemp[j]);
							}
						}
					}
				}
			}
		}
		//dir.rmdir(".");
	}
}
