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
		//�����ļ���ָ��Ŀ¼
		static bool CopyFileToPath(QString sourceDir, QString destDir, bool coverFileIfExist);
		//����ָ��Ŀ¼�µ������ļ���ָ��Ŀ¼
		static bool CopyDirectoryFiles(const QString &fromDir, const QString &destDir, bool coverFileIfExist);
		//��ȡϵͳ�����ļ�Ŀ¼
		static QString GetSystemConfigPath();
	};
}


#endif