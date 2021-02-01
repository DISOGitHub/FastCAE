#ifndef WRITERBASE_H
#define WRITERBASE_H

#include <QString>
#include <QCoreApplication>
#include <QDomDocument>

namespace FastCAEDesigner
{
	class WriterBase
	{
	public:
		WriterBase();
		~WriterBase();

	protected:
		//获取存储的文件完整目录名称
		QString GetFileName();
		//保存文件
		bool SaveFile(QDomDocument &doc);
		//文件名称--不包含目录名
		QString _fileName;
		QMap<QString, QString> insertQString(QStringList var, QStringList val);
		QString SaveIconToSystem(QString fileName);
	};
}

#endif