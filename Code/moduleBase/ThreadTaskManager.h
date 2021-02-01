#ifndef THREADTASKMANAGER_H
#define THREADTASKMANAGER_H

#include "moduleBaseAPI.h"
#include <QList>
#include <QObject>

namespace ModuleBase
{
	class ThreadTask;
	class ThreadControl;

	class MODULEBASEAPI ThreadTaskManager : public QObject
	{
		Q_OBJECT

	public:
		static ThreadTaskManager* getInstance();

		void clearThreadTaskList();
		void appendToThreadTaskList(ThreadTask* t);


		void clearThreadControlList();
		void appendToThreadContolList(ThreadControl* tc);
// 	signals:
// 		void stopThread();
		void removeThread(ThreadControl* tc);

	public slots:
		void removeTask(ThreadTask* t);
		

	private:
		ThreadTaskManager() = default;
		~ThreadTaskManager()= default;

		int getThreadTaskCount();
		ThreadTask* getThreadTaskFromIndex(int index);

	private:
		static ThreadTaskManager* _instance;

		QList<ThreadTask*> _threadTaskList{};
		QList<ThreadControl*> m_threadContol_list{};
	};
}
#endif // THREADTASKMANAGER_H


