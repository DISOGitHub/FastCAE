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
		
		//this->moveToThread(&_thread);
		//_thread.start();
// 		_thread = new QThread{};
// 		this->moveToThread(_thread);
// 		_thread->start();
		
// 		ThreadTaskManager* tm = ThreadTaskManager::getInstance();
// 		tm->appendToThreadTaskList(this);

		//connect(this, SIGNAL(taskFinished(ThreadTask*)), tm, SLOT(removeTask(ThreadTask*)));
		//connect(this, &ThreadTask::start, this, &ThreadTask::run);
		//connect(&_thread, &QThread::start, this, &ThreadTask::run);
		//connect(this, &ThreadTask::closeThread, &_thread, &QThread::quit,Qt::AutoConnection);
		connect(this, SIGNAL(showProcessBar(QWidget*)), _mainwindow, SIGNAL(addProcessBarSig(QWidget*)));
		//connect(_thread, &QThread::finished, _thread, &QThread::deleteLater);
	//	connect(_thread, &QThread::finished, _thread, &QObject::deleteLater);
		//connect(_thread, &QThread::finished, this, &QObject::deleteLater);

		//connect(tm, &ThreadTaskManager::stopThread, this, &ThreadTask::stop);

		emit showProcessBar(_process);
	}

	ThreadTask::~ThreadTask()
	{
// 		_thread->quit();
// 		_thread->wait();
		qDebug() << "task del"<<endl;
	}

	void ThreadTask::stop()
	{
		_threadRuning = false;
// 		emit taskFinished(this);
// 		_thread->quit();
// 		delete this;
	}

	void ThreadTask::threadTaskFinished()
	{
		emit setRange(0, 100);
		_process->setAutoClose(false);
		emit setFinishedStatus(100);
		//_thread.sleep(5);
		//_thread->sleep(5);
		emit threadWait(2);

		_process->setAutoClose(true);
		emit setFinishedStatus(100);
		//emit taskFinished(this);
		//_thread.terminate();
		emit threadFinished();
	//	_thread->quit();
	//	_thread->wait();
	//	//emit taskFinished(this);
		
		//delete this;
	
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
