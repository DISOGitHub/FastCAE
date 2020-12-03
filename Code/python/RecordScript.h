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

		int _states{ 0 };//-1��ͣ 0 �������� 1 ֹͣ
	
	};
}

#endif // _RECORDSCRIPT_H_