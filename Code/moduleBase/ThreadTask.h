#ifndef  THREADTASK_H
#define  THREADTASK_H

#include <QObject>
#include <QThread>
#include "moduleBaseAPI.h"

class QThread;

namespace GUI
{
	class MainWindow;
}

namespace ModuleBase{
	class ProcessBar;

	class MODULEBASEAPI ThreadTask : public QObject
	{
		Q_OBJECT

	public:
		ThreadTask(GUI::MainWindow* m/*QString title*/);
		~ThreadTask();	

	signals:
		void start();
		void showProcessBar(QWidget*);
		void closeThread();
		void setFinishedStatus(int);
		void setRange(int, int);
		void showButton(bool);
		void showInformation(QString s);
		//void taskFinished(ThreadTask* t);

		void threadFinished();
		void threadWait(int n);

	public slots:
		virtual void run() = 0;
		void stop();

	protected:
		///virtual void run() = 0;
		void threadTaskFinished();

	protected:
		ProcessBar* _process;
		GUI::MainWindow* _mainwindow;
		bool _threadRuning{ false };
		//QThread _thread;
		//QThread* _thread;
	};
}
#endif // THREADTASK_H

