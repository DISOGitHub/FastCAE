#include "ThreadControl.h"
#include "ThreadTask.h"
#include "ThreadTaskManager.h"
#include <QThread>
#include <QCoreApplication>
#include <QTime>

namespace ModuleBase {
	ThreadControl::ThreadControl(ThreadTask* task):
		m_threadManager(ThreadTaskManager::getInstance()),
		m_threadTask(task)
	{
		m_thread = new QThread;
		m_threadTask->moveToThread(m_thread);

		m_threadManager->appendToThreadContolList(this);

		connect(this,SIGNAL(threadStart()),m_threadTask,SLOT(run()));
		//connect(this, &ThreadControl::threadStart, m_threadTask, &ThreadTask::run);
		connect(m_thread, &QThread::finished, m_threadTask, &QObject::deleteLater);
		connect(m_thread, &QThread::finished, m_thread, &QObject::deleteLater);
		connect(m_threadTask, SIGNAL(threadFinished()), this, SLOT(deleteThread()));
		connect(this, SIGNAL(threadStop()), m_threadTask, SLOT(stop()));
		connect(m_threadTask, SIGNAL(threadWait(int)), this, SLOT(sleepThread(int)));

		m_thread->start();

		/*emit threadStart();*/
	}

	ThreadControl::~ThreadControl()
	{
		m_thread->quit();
		m_thread->wait();
	}

	void ThreadControl::deleteThread()
	{
		m_threadManager->removeThread(this);
		delete this;
	}

	void ThreadControl::sleepThread(int n)
	{
 		m_thread->sleep(n);//线程sleep  主线程堵塞

	}

}