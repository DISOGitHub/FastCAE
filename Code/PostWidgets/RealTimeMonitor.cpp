#include "RealTimeMonitor.h"
#include <QTimer>
#include <QFileInfo>
#include <QDebug>

namespace Post
{
	RealTimeMonitor::RealTimeMonitor()
	{
		_timer = new QTimer;
		connect(_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
	}
	RealTimeMonitor::~RealTimeMonitor()
	{
		if (_timer) delete _timer;
	}
	void RealTimeMonitor::appendFile(const QString& file)
	{
		QDateTime time = QDateTime::currentDateTime();
		_fileTimeHash[file] = time;
	}
	void RealTimeMonitor::setTimeStep(int t)
	{
		_timeStep = t;
		if (_timer)
			_timer->setInterval(t);
	}
	void RealTimeMonitor::onTimeout()
	{
		QList<QString> files = _fileTimeHash.keys();
		const int n = files.size();
		for (int i = 0; i < n; ++i)
		{
			QString f = files.at(i);
			QFileInfo file(f);
			if (!file.exists()) continue;;
			QDateTime lastmodify = file.lastModified();
			if (lastmodify != _fileTimeHash.value(f))
			{
				_fileTimeHash[f] = lastmodify;
				emit fileChanged(f);
			}
		}
	}
	void RealTimeMonitor::run()
	{
		_timer->start();
	}
	void RealTimeMonitor::stop()
	{
		_timer->stop();
	}
}