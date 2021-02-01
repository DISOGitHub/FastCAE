#include "TaskCommitPlugin.h"
#include "mainWindow/mainWindow.h"
#include "mainWindow/SubWindowManager.h"
#include "TaskProWidget.h"
#include "TaskMgeWidget.h"
#include "MainWidgets/ControlPanel.h"
#include "taskmgm/TaskCommit.h"
#include "Dispatcher.h"
#include "TaskMacro.h"

#include <mutex>

#include <QMdiArea>
#include <QMdiSubWindow>
#include <QAction>
#include <QCoreApplication>
#include <QApplication>
#include <QTranslator>
#include <assert.h>
#include <QDebug>
#include <QFile>
#include <QSettings>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMenu>


void Register(GUI::MainWindow* m, QList<Plugins::PluginBase*>* ps)
{
	Plugins::TaskCommitPlugin::_taskCommitPlugin = new Plugins::TaskCommitPlugin(m);
	Plugins::PluginBase* p = Plugins::TaskCommitPlugin::_taskCommitPlugin;///new Plugins::TaskCommitPlugin(m);
	ps->append(p);
}

namespace Plugins
{

	GUI::MainWindow* TaskCommitPlugin::_mainWindow = nullptr;
	Plugins::TaskCommitPlugin* TaskCommitPlugin::_taskCommitPlugin = nullptr;
	//static QAction* _action = nullptr;

	TaskCommitPlugin::TaskCommitPlugin(GUI::MainWindow *m) //: tmw(nullptr), tpw(nullptr)
	{
		_mainWindow = m;
		_describe = tr("TaskCommit plugin");

	}

	TaskCommitPlugin::~TaskCommitPlugin()
	{

	}

	bool TaskCommitPlugin::install()
	{
		auto app = static_cast<QApplication*>(QCoreApplication::instance());
		// 		_translator->load(":/tr/TaskCommit_zh_CN.qm");
		// 		app->installTranslator(_translator);
		TaskDataObserver::getInstance()->init();
		_tpw = new TaskProWidget;
		_tpw->init();
		_tmw = new TaskMgeWidget(_mainWindow);
		_tmw->init();
		QString appDir = QCoreApplication::applicationFilePath();
		appDir = appDir.left(appDir.lastIndexOf("/"));
		QString iniConfig = appDir + "/config/taskcmt.ini";
		QFile iniFile(iniConfig);
		if (!iniFile.exists()) {
			qDebug() << "failed to read taskcmt.ini";
			return false;
		}

        if (1 == Config->getAutoLogin())
		{
            QString user = Config->getUser();
            QString psw = Config->getPassword();
			QString mainAddr, userAddr, userName, hashKey;
			mainAddr = Config->GetMainServerName();
			userAddr = Config->GetUserServerName();
			userName = Config->GetUserName();
			hashKey = Config->GetHashKey();
			///qDebug() << mainAddr << userAddr << userName << hashKey;
			if (1 != TaskCommitMgr::loginServer(mainAddr, userAddr, userName, hashKey))
			{
				LoginData::Ptr loginPtr = CreateInstance::Instance<LoginData>();
				TRANSFER_DATA_FUN(loginPtr, TASK_STATE_LGNFAILED);
			}
		}
		GetStorePtr->clearTaskData();
		QString jsonData;
		if (0 == TaskCommitMgr::readAllTaskInfo(Config->GetUserId(), jsonData))
		{
			QJsonParseError err;
			QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData.toUtf8(), &err);
			if (QJsonParseError::NoError == err.error) {
				if (jsonDoc.isArray()) {
					QJsonArray array = jsonDoc.array();
					int size = array.size();
					for (int i = 0; i < size; ++i) {
						QVariantMap result = array.at(i).toVariant().toMap();
						QString jobId = result["job_id"].toString();
						QString task_name = result["task_name"].toString();
						QString status = result["status"].toString();
						QString nodes = result["nodes"].toString();
						QString cores = result["cores"].toString();
						QString upload_path = result["upload_path"].toString();
						QString dload_path = result["dload_path"].toString();
						QString start_time = result["start_time"].toString();
						QString end_time = result["end_time"].toString();
						QString create_time = result["create_time"].toString();
						TASK_STATE taskState = (TASK_STATE)status.toInt();
						TaskData::Ptr taskDataPtr = CreateInstance::Instance<TaskData>();
						taskDataPtr->setJobID(jobId.toInt());
						taskDataPtr->setTaskName(task_name);
						taskDataPtr->setState(taskState);
						taskDataPtr->setUpLoadPath(upload_path);
						taskDataPtr->setCpuSize(cores.toInt());
						taskDataPtr->setNodeSize(nodes.toInt());
						taskDataPtr->setDownLoadPath(dload_path);
//                        if(TASK_STATE_CALCOMPLETE == taskState)
                        if(end_time.toULongLong() > start_time.toULongLong())
                            taskDataPtr->setUsedCPUTime(MS_TO_HOUR(end_time.toULongLong() - start_time.toULongLong())*cores.toInt());
						//                    tptr->setLoadTasKState(taskState);
						taskDataPtr->setCreateTime(create_time);
						GetStorePtr->addTaskData(std::move(taskDataPtr));
					}
				}
			}
		}
		_action = new QAction("TaskMge");     //libaojun @2021/01/18
		QString menuName = "menuWindows";
		QMenu* menu = _mainWindow->getMenu(menuName);
		if (menu != nullptr)
			menu->addAction(_action);
#if 0
        MainWidget::ControlPanel* cpl = _mainWindow->getControlPanel();
        cpl->addTabWidgetPlugin(_tpw, tr("TaskTree"));
#endif

		connect(_action, SIGNAL(triggered()), this, SLOT(onViewTaskWindow()));
		connect(_tmw, SIGNAL(taskMgeWidgetDestoryed()), this, SLOT(onTaskWidowCloced()));

		return true;
	}

	bool TaskCommitPlugin::uninstall()
	{
		clearData();
		MainWidget::ControlPanel* cpl = _mainWindow->getControlPanel();
		cpl->removeTabWidgetPlugin(_tpw);
		delete _tpw;
		//   _mainWindow->removeActionForWindowsMenu(_action);
		QString menuName = "menuWindows";
		QMenu* menu = _mainWindow->getMenu(menuName);
		if (menu != nullptr)
			menu->removeAction(_action);
		delete _action;
		//	disconnect(_action, SIGNAL(triggered()), this, SLOT(onViewTaskWindow()));
		GUI::SubWindowManager* swm = _mainWindow->getSubWindowManager();
		QMdiArea* mdi = swm->getMdiArea();
		if (_msw != nullptr)
		{
			mdi->removeSubWindow(_msw);
			_msw->deleteLater();
		}
		QList<QMdiSubWindow *> list = _mainWindow->getSubWindowManager()->getMdiArea()->subWindowList();
		foreach(auto& var, list) {
			var->showMaximized();
			var->setFocus();
			mdi->setActiveSubWindow(var);
			break;
		}

		return true;
	}

	void TaskCommitPlugin::reTranslate(QString lang)
	{
		auto app = static_cast<QApplication*>(QCoreApplication::instance());
		app->removeTranslator(_translator);
		//     QString treeName = "TaskTree";
		//     QString mgeName = "TaskMge";
//        QMdiSubWindow * tmw = getTaskMgrWindow();
		if (lang.toLower() == "chinese")
		{
			bool ok = _translator->load(":/tr/TaskCommit_zh_CN.qm");
			assert(ok);
			app->installTranslator(_translator);
		}

		_action->setText(tr("TaskMge"));
		_tpw->reTranslate();
		_tmw->updateTranslatorUI();
#if 1 ///
        std::once_flag flag;
        std::call_once(flag, [&](){ MainWidget::ControlPanel* cpl = _mainWindow->getControlPanel();
            cpl->addTabWidgetPlugin(_tpw, tr("TaskTree")); });
#endif
	}

	QMdiSubWindow *TaskCommitPlugin::getTaskMgrWindow()
	{
		QList<QMdiSubWindow *> list = _mainWindow->getSubWindowManager()->getMdiArea()->subWindowList();
		foreach(auto& var, list) {
			qDebug() << " mgr is ext " << var->windowTitle();
			if (var->windowTitle() == tr("TaskMge"))
			{
				_mainWindow->getSubWindowManager()->getMdiArea()->setActiveSubWindow(var);
				return var;
			}
		}
		return nullptr;
	}

	bool TaskCommitPlugin::createTaskMgrWidget()
	{
		bool exist = false;

		QMdiSubWindow * tmw = getTaskMgrWindow();
		if (nullptr == tmw)
		{
            if(nullptr == _tmw)
            {
                _tmw = new TaskMgeWidget(_mainWindow);
                _tmw->init();
                _tmw->updateTranslatorUI();
            }
		}
		else {
			exist = true;
		}

#if 0
		QList<QMdiSubWindow *> list = _mainWindow->getSubWindowManager()->getMdiArea()->subWindowList();
		foreach(auto& var, list) {
			qDebug() << " mgr is ext " << var->windowTitle();
			if (var->windowTitle() == tr("TaskMge"))
			{
				_mainWindow->getSubWindowManager()->getMdiArea()->setActiveSubWindow(var);
				exist = true;
				break;
			}
		}
		if (!exist)
		{
			_tmw = new TaskMgeWidget;
			_tmw->init();
			_tmw->updateTranslatorUI();
	}
#endif

		return exist;
}

	void TaskCommitPlugin::printMessage(int type, const QString& msg)
	{
		emit _mainWindow->printMessageSig(type, msg);
	}

	void TaskCommitPlugin::clearData()
	{
		GetStorePtr->clearTaskData();
		DISPATCH_CLEAR();
	}

	void TaskCommitPlugin::onViewTaskWindow()
	{
		//    if(nullptr == tmw)
		//        return;
		if (createTaskMgrWidget())
			return;
        _tpw->reTranslate();
		_tmw->updateTranslatorUI();
		_tpw->setTMgeWidget(_tmw);
		_tpw->clearTask();
		_tmw->clearTask();
		GUI::SubWindowManager* swm = _mainWindow->getSubWindowManager();
		QMdiArea* mdi = swm->getMdiArea();
		_msw = mdi->addSubWindow(_tmw);
		_msw->showMaximized();
		_msw->setFocus();

		mdi->setActiveSubWindow(_msw);
		//    _action->setEnabled(false);
		qDebug() << "task size = " << GetStorePtr->getTaskDataSize();
		for (int i = 0; i < GetStorePtr->getTaskDataSize(); ++i)
		{
			const TaskData::Ptr& tptr = GetStorePtr->getTaskData(i);
			CreatedTaskTransferData::Ptr ptr = CreateInstance::Instance<CreatedTaskTransferData>();
			ptr->setJobID(tptr->getJobID());
			ptr->setJobName(tptr->getTaskName());
			ptr->setTaskType(TASK_STATE_CREATED);
			ptr->setUpPath(tptr->getUpLoadPath());
			ptr->setCpuSize(tptr->getCpuSize());
			ptr->setNodeSize(tptr->getNodeSize());
			ptr->setDownPath(tptr->getDownLoadPath());
			ptr->setLoadTasKState(tptr->getState());
			ptr->setTime(tptr->getCreateTime());
            ptr->setUsedTime(tptr->getUsedCPUTime());
			TRANSFER_DATA_FUN(ptr, TASK_STATE_CREATED); /// TASK_STATE_CREATED 表示是已经创建好的任务：from db
		}
	}


	void TaskCommitPlugin::onTaskWidowCloced()
	{
		_tmw = nullptr;
		_msw = nullptr;
	}

}
