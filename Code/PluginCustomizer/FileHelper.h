#ifndef FILEHELPER_H
#define FILEHELPER_H
#include <QString>

namespace FastCAEDesigner
{

	class FileHelper
	{
	public:
		FileHelper();
		~FileHelper();

	public:
		//拷贝文件到指定目录
		static bool CopyFileToPath(QString sourceDir, QString destDir, bool coverFileIfExist);
		//拷贝指定目录下的所有文件到指定目录
		static bool CopyDirectoryFiles(const QString &fromDir, const QString &destDir, bool coverFileIfExist);
		//获取系统配置文件目录
		static QString GetSystemConfigPath();
	};
}


#endif