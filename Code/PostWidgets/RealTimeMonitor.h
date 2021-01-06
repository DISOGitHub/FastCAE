#ifndef REALTIMEMONITOR_H
#define REALTIMEMONITOR_H

#include <QThread>
#include <QHash>
#include <QString>
#include <QDateTime>

class QTimer;

namespace Post
{
	class RealTimeMonitor : public QThread
	{
		Q_OBJECT
	public:
		RealTimeMonitor();
		~RealTimeMonitor();
		void appendFile(const QString& file);
		void setTimeStep(int t);
		void run() override;
		void stop();

	signals:
		void fileChanged(QString file);


	private slots :
		void onTimeout();

	private:
		QHash<QString, QDateTime> _fileTimeHash;
		int _timeStep{ -1 };
		QTimer* _timer{};
	};
}


#endif