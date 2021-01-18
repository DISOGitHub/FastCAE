#ifndef _RECORDTHREAD_H_
#define _RECORDTHREAD_H_

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class QFile;
class QTextStream;
class QTimer;

namespace Py
{
	class  RecordThread : public QThread
	{
		Q_OBJECT
	public:
		RecordThread(QObject *parent = nullptr);
		~RecordThread();

		void pause();
		void stop();
		void reStart();

	private:
		void run() override;

	private slots:
		void writeRecordScriptFile();


	private:
		QFile* _file{};
		QTextStream *_IO{};

		int _states{ 0 };//-1暂停 0 正常运行 1 停止
	
	};
}

#endif // _RECORDSCRIPT_H_