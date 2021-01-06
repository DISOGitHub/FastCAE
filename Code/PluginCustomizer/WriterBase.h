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
		//��ȡ�洢���ļ�����Ŀ¼����
		QString GetFileName();
		//�����ļ�
		bool SaveFile(QDomDocument &doc);
		//�ļ�����--������Ŀ¼��
		QString _fileName;
		QMap<QString, QString> insertQString(QStringList var, QStringList val);
		QString SaveIconToSystem(QString fileName);
	};
}

#endif