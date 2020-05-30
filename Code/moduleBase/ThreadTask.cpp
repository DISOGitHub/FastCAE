#include "ThreadTask.h"
#include "processBar.h"
#include "mainWindow/mainWindow.h"
#include "mainWindow/SubWindowManager.h"
#include "ThreadTaskManager.h"
#include <QThread>
#include <QDebug>

namespace ModuleBase
{
	ThreadTask::ThreadTask(GUI::MainWindow* m) :_mainwindow(m)
	{
		_process = new ProcessBar(_mainwindow,this);
//		_thread = new QThread{};
		this->moveToThread(&_thread);
		_thread.start();
		ThreadTaskManager* tm = ThreadTaskManager::getInstance();
		tm->appendToThreadTaskList(this);

		connect(this, SIGNAL(taskFinished(ThreadTask*)), tm, SLOT(removeTask(ThreadTask*)));
		connect(this, &ThreadTask::start, this, &ThreadTask::run);
		connect(this, &ThreadTask::closeThread, &_thread, &QThread::quit,Qt::AutoConnection);
		connect(this, SIGNAL(showProcessBar(QWidget*)), _mainwindow, SIGNAL(addProcessBarSig(QWidget*)));

		emit showProcessBar(_process);
	}

	ThreadTask::~ThreadTask()
	{
//		qDebug() << "task des";
	}

	void ThreadTask::stop()
	{
		_threadRuning = false;
	}

	void ThreadTask::threadTaskFinished()
	{
		emit setRange(0, 100);
		_process->setAutoClose(false);
		emit setFinishedStatus(100);
		_thread.sleep(5);

		_process->setAutoClose(true);
		emit setFinishedStatus(100);
		emit taskFinished(this);
		delete this;
	}

// 	void ThreadTask::threadTaskFinished()
// 	{
// 		emit setRange(0, 100);
// 		emit setFinishedStatus(100);
// 		emit taskFinished(this);
// 		delete this;
// 	}

	void ThreadTask::run()
	{
		_threadRuning = true;
	}

}
