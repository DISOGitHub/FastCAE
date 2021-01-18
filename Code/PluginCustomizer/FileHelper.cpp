#include "FileHelper.h"
#include <QDir>
#include <QString>
#include <QCoreApplication>
#include <QDebug>
#include <QFileInfo>

namespace FastCAEDesigner
{

	FileHelper::FileHelper()
	{
	}


	FileHelper::~FileHelper()
	{
	}

	//获取系统配置文件目录
	QString FileHelper::GetSystemConfigPath()
	{
		const QString configDir = QCoreApplication::applicationDirPath() + "//..//ConfigFiles//";
		QDir dir(configDir);
		if (!dir.exists())
			dir.mkpath(configDir);
//		qDebug() << dir.absolutePath();
		return dir.absolutePath()+"/";
	}

	//拷贝文件：
	bool FileHelper::CopyFileToPath(QString sourceDir, QString destDir, bool coverFileIfExist)
	{
	//	qDebug() << sourceDir << destDir;
// 		destDir.replace("\\", "/");
// 
// 		if (sourceDir == destDir)
// 			return true;
		
// 		if (!QFile::exists(sourceDir))
// 			return false;
		if (!QFile::exists(sourceDir))
			return false;

		QFileInfo sorFile(sourceDir);
		QFileInfo desFile(destDir);

 		if (sorFile.absoluteFilePath() == desFile.absoluteFilePath())
			return true;
		
		QDir *createfile = new QDir;
		bool exist = createfile->exists(destDir);

		if (exist)
		{
			if (coverFileIfExist)
			{
				createfile->remove(destDir);
			}
		}

		if (!QFile::copy(sourceDir, destDir))
			return false;
		
		delete createfile;
		createfile = nullptr;

		return true;
	}

	//拷贝文件夹：
	bool FileHelper::CopyDirectoryFiles(const QString &fromDir, const QString &destDir, bool coverFileIfExist)
	{
		QDir sourceDir(fromDir);
		QDir targetDir(destDir);

		if (!targetDir.exists()) /**< 如果目标目录不存在，则进行创建 */
		{
			if (!targetDir.mkdir(targetDir.absolutePath()))
				return false;
		}

		QFileInfoList fileInfoList = sourceDir.entryInfoList();

		foreach(QFileInfo fileInfo, fileInfoList)
		{
			if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
				continue;

			if (fileInfo.isDir()) /**< 当为目录时，递归的进行copy */
			{
				if (!CopyDirectoryFiles(fileInfo.filePath(),
					targetDir.filePath(fileInfo.fileName()),
					coverFileIfExist))
					return false;
			}
			else
			{            /**< 当允许覆盖操作时，将旧文件进行删除操作 */
				if (coverFileIfExist && targetDir.exists(fileInfo.fileName()))
				{
					targetDir.remove(fileInfo.fileName());
				}

				/// 进行文件copy
				if (!QFile::copy(fileInfo.filePath(),
					targetDir.filePath(fileInfo.fileName())))
				{
					return false;
				}
			}
		}

		return true;
	}
}