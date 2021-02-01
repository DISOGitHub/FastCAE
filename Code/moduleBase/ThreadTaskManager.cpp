#include "ThreadTaskManager.h"
#include "ThreadTask.h"
#include "ThreadControl.h"
#include <QDebug>

namespace ModuleBase
{
	ThreadTaskManager* ThreadTaskManager::_instance = nullptr;

	ThreadTaskManager* ThreadTaskManager::getInstance()
	{
		if (_instance == nullptr)
		{
			_instance = new ThreadTaskManager;
		}
		return _instance;
	}

	void ThreadTaskManager::appendToThreadTaskList(ThreadTask* t)
	{
		_threadTaskList.append(t);
	}

	void ThreadTaskManager::clearThreadControlList()
	{
		for (int i=0;i<m_threadContol_list.size();i++)
		{
			ThreadControl* tc = m_threadContol_list.at(i);
			if(tc==nullptr)continue;
			emit tc->threadStop();
		}

		m_threadContol_list.clear();
	}

	void ThreadTaskManager::appendToThreadContolList(ThreadControl* tc)
	{
		m_threadContol_list.append(tc);
	}

	void ThreadTaskManager::clearThreadTaskList()
	{
		QList<ThreadTask*> ts = _threadTaskList;
		for (int i = 0; i < getThreadTaskCount(); i++)
		{
			ThreadTask* t = ts.at(i);
			if (t == nullptr) continue;
			t->stop();
		}
		_threadTaskList.clear();
	}

	void ThreadTaskManager::removeTask(ThreadTask* t)
	{
		_threadTaskList.removeOne(t);
	}

	void ThreadTaskManager::removeThread(ThreadControl* tc)
	{
		m_threadContol_list.removeOne(tc);
	}

	int ThreadTaskManager::getThreadTaskCount()
	{
		return _threadTaskList.size();
	}

	ThreadTask* ThreadTaskManager::getThreadTaskFromIndex(int index)
	{
		if (0 < index || index >= getThreadTaskCount())
			return nullptr;

		return _threadTaskList.at(index);
	}

}