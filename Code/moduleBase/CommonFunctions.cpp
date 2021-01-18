#include "CommonFunctions.h"
#include <QDir>
#include <QDebug>

bool RemoveDir(QString fullpath)
{
	qDebug() << fullpath;
	QDir dir(fullpath);
	if (!dir.exists()) return true;

	dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //设置过滤
	QFileInfoList fileList = dir.entryInfoList(); // 获取所有的文件信息
	for(QFileInfo file: fileList)
	{ 
		if (file.isFile())
		{ // 是文件，删除
			file.dir().remove(file.fileName());
		}
		else
		{ // 递归删除
			RemoveDir(file.absoluteFilePath());
		}
	}
	return dir.rmpath(dir.absolutePath()); // 删除文件夹

}

QString MODULEBASEAPI doubleToString(double v, int acc)
{
	QString format = QString("%.%1f").arg(acc);
	QByteArray ba = format.toLatin1();

	QString vs = QString::asprintf(ba.data(), v);
	return vs;
}
