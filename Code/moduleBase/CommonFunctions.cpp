#include "CommonFunctions.h"
#include <QDir>
#include <QDebug>

bool RemoveDir(QString fullpath)
{
	qDebug() << fullpath;
	QDir dir(fullpath);
	if (!dir.exists()) return true;

	dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //���ù���
	QFileInfoList fileList = dir.entryInfoList(); // ��ȡ���е��ļ���Ϣ
	for(QFileInfo file: fileList)
	{ 
		if (file.isFile())
		{ // ���ļ���ɾ��
			file.dir().remove(file.fileName());
		}
		else
		{ // �ݹ�ɾ��
			RemoveDir(file.absoluteFilePath());
		}
	}
	return dir.rmpath(dir.absolutePath()); // ɾ���ļ���

}