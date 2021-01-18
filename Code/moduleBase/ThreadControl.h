#ifndef THREADCONTROL_H
#define THREADCONTROL_H

#include <QObject>
#include "moduleBaseAPI.h"

class QThread;

namespace ModuleBase 
{
	class ThreadTask;
	class ThreadTaskManager;

	class MODULEBASEAPI ThreadControl : public QObject
	{
		Q_OBJECT
	public:
		explicit ThreadControl(ThreadTask* task);
		~ThreadControl();

	signals:
		void threadStart();
		void threadStop();

	private slots:
		void deleteThread();
		void sleepThread(int n);

	private:
		ThreadTask* m_threadTask;
		QThread* m_thread;
		ThreadTaskManager* m_threadManager;
	};
}
#endif // THREADCONTROL_H
