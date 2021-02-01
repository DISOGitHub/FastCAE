#ifndef _IOBASECLASS_H_
#define _IOBASECLASS_H_

#include <QString>
#include <QStringList>
#include <QFile>
#include <QList>
#include <QVector>
#include "moduleBaseAPI.h"

class QDomDocument;
class QTextStream;
class QDataStream;

namespace ModuleBase
{

	class MODULEBASEAPI IOBase
	{
	public:
		IOBase(const QString& fileName);
		IOBase();
		~IOBase();
		void setFileName(const QString &name);
		///读取实时曲线数据
		void readRealTimeData(QList<QVector<double>> &x, QList<QVector<double>> &y, QString fileName);

	public:
		virtual bool read();
		virtual bool write(QString s = QString());

	protected:
		QString _filename;
		QFile _file;
		QTextStream *_stream{};
		QDataStream* _dataStream{};

		QDomDocument* _doc{};

	};

}

 
#endif