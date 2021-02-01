#include "TaskProWidget.h"
#include "ui_TaskProWidget.h"
#include "TaskMgeWidget.h"
#include "TaskCommitPlugin.h"
#include "TaskMacro.h"
#include "Dispatcher.h"
#include "TaskThread.h"
#include "taskmgm/TaskCommit.h"
#include "CreatTaskDialog.h"
#include "mainWindow/mainWindow.h"
#include "MainWidgets/ControlPanel.h"

#include <QMenu>
#include <QTreeWidgetItem>
#include <QDebug>
#include <QMessageBox>
#include <QState>

//static QTreeWidgetItem *tasksItem = nullptr;
//static QTreeWidgetItem *resultsItem = nullptr;
//static QTreeWidgetItem *curItem = nullptr;
namespace Plugins {

	TaskProWidget::TaskProWidget(QWidget *parent)
		: QWidget(parent)
		, ui(new Ui::TaskProWidget)
	{
		ui->setupUi(this);
		ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
		//		QList<QTreeWidgetItem *> items;
		ui->treeWidget->setColumnCount(1);  // 设置列
		_tasksItem = new QTreeWidgetItem(ui->treeWidget, QStringList(tr("Tasks")));
		_resultsItem = new QTreeWidgetItem(ui->treeWidget, QStringList(tr("Results")));
		ui->treeWidget->addTopLevelItem(_tasksItem);
		ui->treeWidget->addTopLevelItem(_resultsItem);
		// 		items.append(tasksItem);
		// 		items.append(resultsItem);
				// 添加顶层节点
		//		ui->treeWidget->insertTopLevelItems(0, items);
				// 节点始终保持展开
//		ui->treeWidget->setItemsExpandable(false);
		ui->treeWidget->expandAll();

		/// menu
		QList<QAction*> lists;
		crt = new QAction(tr("CreateTask"), this);
		commit = new QAction(tr("Commit"), this);
		calc = new QAction(tr("Calculate"), this);
		down = new QAction(tr("Download"), this);
		hcl = new QAction(tr("post"), this);
		of = new QAction(tr("openfile"), this);
		query = new QAction(tr("query"), this);
		hcl->setVisible(false);
		of->setVisible(false);
		query->setVisible(false);
		lists << crt << commit << calc << down << hcl << of << query;
		taskTreeMenu = new QMenu;
		taskTreeMenu->addActions(lists);

		/// connect
		connect(crt, SIGNAL(triggered()), this, SLOT(onCreateTask()));
		connect(commit, SIGNAL(triggered()), this, SLOT(onCommitTask()));
		connect(calc, SIGNAL(triggered()), this, SLOT(onCalcTask()));
		connect(down, SIGNAL(triggered()), this, SLOT(onDownResult()));
	}

	TaskProWidget::~TaskProWidget()
	{
		delete ui;
		if (taskTreeMenu)
		{
			taskTreeMenu->deleteLater();
		}
	}

	void TaskProWidget::init()
	{
        QString name = "TaskTree";
        REGISTER_DATA_FUN("TaskTree", std::bind(&TaskProWidget::updataTaskData, this, std::placeholders::_1));
	}

	void TaskProWidget::updataTaskData(TaskData::Ptr &&ptr)
	{
		qDebug() << "TaskProWidget::updataTaskData";
		//    QTreeWidgetItem* fItem = ui->treeWidget.g

		switch (ptr->getState()) {
		case TASK_STATE_LGN: /// login
			break;
		case TASK_STATE_LGNFAILED:
			break;
		case TASK_STATE_LGNCOMPLETE:
			break;
		case TASK_STATE_CREATECOMPLETE:
			addItem(_tasksItem, ptr);
			break;
		case TASK_STATE_CREATE:
			break;
		case TASK_STATE_CREATED:
		{
			TaskData p = *(ptr.get());///setState(ptr->getStoreState());
			p.setState(ptr->getStoreState());
			addItem(_tasksItem, p);
		}
		break;
		case TASK_STATE_DEL:
			break;
		case TASK_STATE_DELFAILED:
			break;
		case TASK_STATE_DELCOMPLETE:
		{
			QTreeWidgetItem * item = getItemFromTaskNode(ptr->getTaskName());
			_tasksItem->removeChild(item);
		}
		break;
		case TASK_STATE_CREATEDFAILED:
		case TASK_STATE_UCOMMIT:
		case TASK_STATE_UCOMMITEDFAILED:
		case TASK_STATE_UCOMMITEDCOMPLETE:
		case TASK_STATE_CALCING:
        case TASK_STATE_RUN:
        case TASK_STATE_FAIL:
        case TASK_STATE_PENDING:
		case TASK_STATE_CALCFAILED:
		case TASK_STATE_CALCOMPLETE:
		case TASK_STATE_DRESULT:
		case TASK_STATE_DRESULTFAILED:
		{
			QTreeWidgetItem * item = getItemFromTaskNode(ptr->getTaskName());
			if (nullptr != item)
			{
				QVariant var(ptr->getState());
				item->setData(0, Qt::UserRole + 1, var);
			}
		}
		break;
		case TASK_STATE_DRESULTCOMPLETE:
		{
			QTreeWidgetItem * item = getItemFromTaskNode(ptr->getTaskName());
			if (nullptr != item)
			{
				QVariant var(ptr->getState());
				item->setData(0, Qt::UserRole + 1, var);
			}
			addItem(_resultsItem, ptr);
		}
		break;
		case TASK_STATE_SEL:
			break;
		case TASK_STATE_SELFAILED:
			break;
		case TASK_STATE_SELCOMPLETE:
			break;
		default:
			break;
		}
	}

	bool TaskProWidget::getActiveTaskMgrWidget()
	{
		return true;
	}

	QTreeWidgetItem *TaskProWidget::getItemFromTaskNode(QString taskName)
	{
		qDebug() << "getItemFromTaskNode";
		int count = _tasksItem->childCount();
		for (int i = 0; i < count; ++i) {
			QTreeWidgetItem* item = _tasksItem->child(i);
			if (item->text(0) == taskName)
				return item;
		}
		return nullptr;
	}

	void TaskProWidget::addItem(QTreeWidgetItem *parent, const TaskData &p)
	{
		QTreeWidgetItem* item = new QTreeWidgetItem;
		item->setText(0, p.getTaskName());
		TASK_STATE t = p.getState();
		///changeState(t);
		QVariant var(t);
		item->setData(0, Qt::UserRole + 1, var);
		QVariant upathvar(p.getUpLoadPath());
		item->setData(0, Qt::UserRole + 2, upathvar);
		QVariant dpathvar(p.getDownLoadPath());
		item->setData(0, Qt::UserRole + 3, dpathvar);
		QVariant jobvar(p.getJobID());
		item->setData(0, Qt::UserRole + 4, jobvar);
		parent->addChild(item);
        ui->treeWidget->expandAll();
	}

	void TaskProWidget::addItem(QTreeWidgetItem *parent, const TaskData::Ptr &ptr)
	{
		QTreeWidgetItem* item = new QTreeWidgetItem;
		item->setText(0, ptr->getTaskName());
		TASK_STATE t = ptr->getState();
		///changeState(t);
		QVariant var(t);
		item->setData(0, Qt::UserRole + 1, var);
		QVariant upathvar(ptr->getUpLoadPath());
		item->setData(0, Qt::UserRole + 2, upathvar);
		QVariant dpathvar(ptr->getDownLoadPath());
		item->setData(0, Qt::UserRole + 3, dpathvar);
		QVariant jobvar(ptr->getJobID());
		item->setData(0, Qt::UserRole + 4, jobvar);
		parent->addChild(item);
        ui->treeWidget->expandAll();
	}

	void TaskProWidget::changeState(TASK_STATE s)
	{
		crt->setEnabled(false);
		switch (s) {
		case TASK_STATE_CREATECOMPLETE:
		{
			commit->setEnabled(true);
			calc->setEnabled(false);
			down->setEnabled(false);
			of->setEnabled(false);
			hcl->setEnabled(false);
			query->setEnabled(false);
		}
		break;
		case TASK_STATE_UCOMMIT:
		{
			commit->setEnabled(true);
			calc->setEnabled(false);
			down->setEnabled(false);
			of->setEnabled(false);
			hcl->setEnabled(false);
			query->setEnabled(false);
		}
		break;
		case TASK_STATE_UCOMMITEDFAILED:
		{
			commit->setEnabled(true);
			calc->setEnabled(false);
			down->setEnabled(false);
			of->setEnabled(false);
			hcl->setEnabled(false);
			query->setEnabled(false);
		}
		break;
		case TASK_STATE_UCOMMITEDCOMPLETE:
		{
			commit->setEnabled(false);
			calc->setEnabled(true);
			down->setEnabled(false);
			of->setEnabled(false);
			hcl->setEnabled(false);
			query->setEnabled(false);
		}
		break;
		case TASK_STATE_DELCOMPLETE:
		{
			commit->setEnabled(false);
			calc->setEnabled(true);
			down->setEnabled(false);
			of->setEnabled(false);
			hcl->setEnabled(false);
			query->setEnabled(false);
		}
		break;
		case TASK_STATE_CALCING:
		{
			commit->setEnabled(false);
			calc->setEnabled(false);
			down->setEnabled(false);
			of->setEnabled(false);
			hcl->setEnabled(false);
			query->setEnabled(true);
		}
		break;
		case TASK_STATE_CALCFAILED:
		{
			commit->setEnabled(false);
			calc->setEnabled(true);
			down->setEnabled(false);
			of->setEnabled(false);
			hcl->setEnabled(false);
			query->setEnabled(false);
		}
		break;
		case TASK_STATE_CALCOMPLETE:
		{
			commit->setEnabled(false);
			calc->setEnabled(false);
			down->setEnabled(true);
			of->setEnabled(false);
			hcl->setEnabled(false);
			query->setEnabled(false);
		}
		break;
		case TASK_STATE_DRESULTFAILED:
		{
			commit->setEnabled(false);
			calc->setEnabled(false);
			down->setEnabled(true);
			of->setEnabled(false);
			hcl->setEnabled(false);
			query->setEnabled(false);
		}
		break;
		case TASK_STATE_DRESULTCOMPLETE:
		{
			commit->setEnabled(false);
			calc->setEnabled(false);
			down->setEnabled(false);
			of->setEnabled(true);
			hcl->setEnabled(true);
			query->setEnabled(false);
		}
		break;
		case TASK_STATE_STOP:
		{

		}
		break;
		case TASK_STATE_STOPFAILED:
		{

		}
		break;
		case TASK_STATE_STOPCOMPLETE:
		{
			commit->setEnabled(false);
			calc->setEnabled(true);
			down->setEnabled(false);
			of->setEnabled(false);
			hcl->setEnabled(false);
			query->setEnabled(false);
		}
		break;
		default:
			break;
		}
	}

	TaskMgeWidget *TaskProWidget::getTMgeWidget() const
	{
		return tMgeWidget;
	}

	void TaskProWidget::setTMgeWidget(TaskMgeWidget *value)
	{
		tMgeWidget = value;
	}

	void TaskProWidget::clearTask()
	{
		for (int i = 0; i < _tasksItem->childCount(); ++i)
		{
			QTreeWidgetItem* item = _tasksItem->child(i);
			_tasksItem->removeChild(item);
		}
		for (int i = 0; i < _resultsItem->childCount(); ++i)
		{
			QTreeWidgetItem* item = _resultsItem->child(i);
			_resultsItem->removeChild(item);
		}
	}


	void TaskProWidget::reTranslate()
	{
        ///ui->retranslateUi(this);
		_tasksItem->setText(0, tr("Tasks"));
		_resultsItem->setText(0, tr("Results"));
		auto mw = TaskCommitPlugin::getMainWindow();
		auto conpan = mw->getControlPanel();
		conpan->setTabText(this, tr("TaskTree"));
        crt->setText(tr("CreateTask"));
        commit->setText(tr("Commit"));
        calc->setText(tr("Calculate"));
        down->setText(tr("Download"));
        hcl->setText(tr("post"));
        of->setText(tr("openfile"));
        query->setText(tr("query"));
	}

	void TaskProWidget::onCreateTask()
	{
		TaskCommitPlugin::getSelf()->onViewTaskWindow();
		tMgeWidget->on_ctkPushButton_clicked();
	}

	void TaskProWidget::onCommitTask()
	{
		if (!TaskCommitMgr::isLoginSucceed())
		{
            QMessageBox::warning(this, tr(PROMPT_MSS), tr(NOT_LOGIN));
			return;
		}
		TaskCommitPlugin::getSelf()->onViewTaskWindow();
		//    if(nullptr == tMgeWidget)
		//        return;
        CommitRunable* cit = new CommitRunable;
		cit->setJobName(curItem->text(0));
		cit->setFilePath(curItem->data(0, Qt::UserRole + 2).toString());
		//    changeState(TASK_STATE_UCOMMIT);
		QVariant var(TASK_STATE_UCOMMIT);
		curItem->setData(0, Qt::UserRole + 1, var);
		tMgeWidget->commitTask(cit);
	}

	void TaskProWidget::onCalcTask()
	{
		/// TODO: 界面提示，已经有下载是否重新下载
		if (!TaskCommitMgr::isLoginSucceed())
		{
            QMessageBox::warning(this, tr(PROMPT_MSS), tr(NOT_LOGIN));
			return;
		}
		TaskCommitPlugin::getSelf()->onViewTaskWindow();
		//    if(nullptr == tMgeWidget)
		//        return;
		ExecuteRunable* exec = new ExecuteRunable;
		exec->setJobName(curItem->text(0));
		//    changeState(TASK_STATE_CALCING);
		QVariant var(TASK_STATE_CALCING);
		curItem->setData(0, Qt::UserRole + 1, var);
		tMgeWidget->calcTask(exec);
	}

	void TaskProWidget::onDownResult()
	{
		if (!TaskCommitMgr::isLoginSucceed())
		{
            QMessageBox::warning(this, tr(PROMPT_MSS), tr(NOT_LOGIN));
			return;
		}
		TaskCommitPlugin::getSelf()->onViewTaskWindow();
		//    if(nullptr == tMgeWidget)
		//        return;
		ResultRunable* resultRunable = new ResultRunable;
		resultRunable->setFilePath(curItem->data(0, Qt::UserRole + 3).toString());
		resultRunable->setJobName(curItem->text(0));
		//    changeState(TASK_STATE_DRESULT);
		QVariant var(TASK_STATE_DRESULT);
		curItem->setData(0, Qt::UserRole + 1, var);
		tMgeWidget->downTask(resultRunable);
	}

}


void Plugins::TaskProWidget::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
	QTreeWidgetItem* curItem = ui->treeWidget->itemAt(pos);  //获取当前被点击的节点
	if (curItem == NULL)return;           //这种情况是右键的位置不在treeItem的范围内，即在空白位置右击
	QList<QAction*> actions = taskTreeMenu->actions();
	//    for (int i = 0; i < actions.size(); ++i) {
	//        actions.at(i)->setEnabled(true);
	//    }
	if (curItem == _tasksItem)
	{
		actions.at(0)->setEnabled(true);
		for (int i = 1; i < actions.size(); ++i) {
			actions.at(i)->setEnabled(false);
		}

	}
	else if (curItem == _resultsItem)
	{
		for (int i = 0; i < actions.size(); ++i) {
			actions.at(i)->setEnabled(false);
		}
	}
	else {
		changeState((TASK_STATE)curItem->data(0, Qt::UserRole + 1).toInt());
		//        if(curItem->parent() == tasksItem)
		//        {
		//            changeState((TASK_STATE)curItem->data(0,1).toInt());
		//            actions.at(0)->setEnabled(false);
		//            for (int i = 4; i < actions.size(); ++i) {
		//                actions.at(i)->setEnabled(false);
		//            }
		//        } else if(curItem->parent() == resultsItem)
		//        {
		//            changeState((TASK_STATE)curItem->data(0,1).toInt());
		//            for (int i = 0; i < 4; ++i) {
		//                actions.at(i)->setEnabled(false);
		//            }
		//        }
	}
	taskTreeMenu->exec(QCursor::pos());
}

void Plugins::TaskProWidget::on_treeWidget_itemPressed(QTreeWidgetItem *item, int column)
{
	curItem = item;
}
