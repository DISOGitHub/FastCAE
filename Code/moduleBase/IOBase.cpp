#include "IOBase.h"
#include <QFile>
#include <QTextStream>
#include <QDomDocument>

namespace ModuleBase
{
	IOBase::IOBase(const QString& fileName) : _filename(fileName)
	{
		_file.setFileName(_filename);
	}
	IOBase::IOBase()
	{
	}
	IOBase::~IOBase()
	{
		if (_stream) delete _stream;
		if (_doc) delete _doc;
		if (_dataStream) delete _dataStream;
	}
	void IOBase::setFileName(const QString &name)
	{
		_filename = name;
		_file.setFileName(_filename);
	}
	bool IOBase::read()
	{
		return false;
	}
	bool IOBase::write(QString s )
	{
		return false;
	}
	void IOBase::readRealTimeData(QList<QVector<double>> &xs, QList<QVector<double>> &ys, QString fileName)
	{
		this->setFileName(fileName);
		if (!_file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
		_stream = new QTextStream(&_file);
		QVector<double> x; QVector<double> y;
		while (!_stream->atEnd())
		{
			QString line = _stream->readLine().simplified();
			if (line.isEmpty()) continue;
			QStringList xy = line.split(" ");
			double xd = xy[0].toDouble();
			double yd = xy[1].toDouble();
			x.append(xd);
			y.append(yd);
		}
		xs.append(x);
		ys.append(y);
	}
}