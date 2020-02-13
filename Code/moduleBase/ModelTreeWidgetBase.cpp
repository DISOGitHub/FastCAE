#include "ModelTreeWidgetBase.h"
#include "mainWindow/mainWindow.h"
#include <QContextMenuEvent>
#include "moduleBase/modelTreeItemType.h"
#include <QMenu>
#include <assert.h>



namespace ModuleBase
{

	ModelTreeWidgetBase::ModelTreeWidgetBase(GUI::MainWindow* mainwindow, QWidget* parent /* = 0 */) : _mainWindow(mainwindow)
	{
		init();
	}
	ModelTreeWidgetBase::~ModelTreeWidgetBase()
	{

	}
	void ModelTreeWidgetBase::init()
	{
		this->setHeaderHidden(true);

		///注册了TreeWidget 左键单击
		connect(this, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(singleClicked(QTreeWidgetItem*, int)));
		///注册了TreeWidget 左键双击
		connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(doubleClicked(QTreeWidgetItem*, int)));
		///注册鼠标事件传递通知给mainWindow
		connect(this, SIGNAL(mouseEvent(int, QTreeWidgetItem*, int)), _mainWindow, SIGNAL(treeMouseEvent(int, QTreeWidgetItem*, int)));
		///注册由mainWindow传递通知过来更新树
		connect(_mainWindow, SIGNAL(updateTreeSignal()), this, SLOT(updateTree()));
		///注册打印信息传递通知给mainWindow
		connect(this, SIGNAL(printMessageSignal(QString)), _mainWindow, SIGNAL(printMessageToMessageWindow(QString)));
	}
	void ModelTreeWidgetBase::updateTree()
	{

	}
	QTreeWidgetItem* ModelTreeWidgetBase::getProjectRoot(QTreeWidgetItem* item) const
	{
		if (NULL == item && NULL == _curretnItem)
			return NULL;
		if (item == nullptr)
			item = _curretnItem;

		while (item->type() != TreeItemType::ProjectRoot && item->parent() != nullptr)
		{
			item = item->parent();
		}
		if (item != nullptr) return item;
		return nullptr;

	}
	QTreeWidgetItem* ModelTreeWidgetBase::getProjectRoot(const int pid) const
	{
		int n = this->topLevelItemCount();
		for (int i = 0; i < n; ++i)
		{
			QTreeWidgetItem* item = topLevelItem(i);
			int id = item->data(0, Qt::UserRole).toInt();
			if (id == pid)
				return item;
		}
		return nullptr;
	}
	///contextMenuEvent   重写了底层函数   相当于右键点击
	void ModelTreeWidgetBase::contextMenuEvent(QContextMenuEvent *evnt)
	{
		emit printMessageSignal("ModelTreeWidgetBase contextMenuEvent star");

		_curretnItem = currentItem();
		if (!_curretnItem) return;
		QTreeWidgetItem* item = getProjectRoot(_curretnItem);
		if (NULL == item)
			return;
		assert(item);
		const int proID = item->data(0, Qt::UserRole).toInt();
		on_TreeMouseEvent(1, _curretnItem, proID);
		emit(mouseEvent(1, _curretnItem, proID));
	}
	void ModelTreeWidgetBase::singleClicked(QTreeWidgetItem* item, int col)
	{
		_curretnItem = item;
		QTreeWidgetItem* rootitem = getProjectRoot(_curretnItem);
		assert(rootitem);
		const int proID = rootitem->data(0, Qt::UserRole).toInt();
		on_TreeMouseEvent(0, _curretnItem, proID);
		emit mouseEvent(0, _curretnItem, proID);
	}
	void ModelTreeWidgetBase::doubleClicked(QTreeWidgetItem* item, int col)
	{
		_curretnItem = item;
		QTreeWidgetItem* rootitem = getProjectRoot(_curretnItem);
		assert(rootitem);
		const int proID = rootitem->data(0, Qt::UserRole).toInt();

		on_TreeMouseEvent(0, _curretnItem, proID);
		emit mouseEvent(2, item, proID);
	}

	void ModelTreeWidgetBase::on_TreeMouseEvent(int evevntType, QTreeWidgetItem* item, int id)
	{
		//ProjectData::ProjectData* projectdata = ProjectData::Singleton::getInstance()->getProjectDataByID(id);
		//assert(projectdata != nullptr);
		ProjectData::ProjectData* projectdata = NULL;
		switch (evevntType)
		{
		case 0://左键单击
			on_singleClicked(item, projectdata);
			break;
		case 1://右键单击
			createContextMenu(item, projectdata);
			break;
		case 2://左键双击
			on_doubleClicked(item, projectdata);
			break;
		default:
			assert(0);
			break;
		}
	}
	void ModelTreeWidgetBase::on_singleClicked(QTreeWidgetItem* item, ProjectData::ProjectData* projectData)
	{

	}
	void ModelTreeWidgetBase::on_doubleClicked(QTreeWidgetItem* item, ProjectData::ProjectData* projectData)
	{

	}
	void ModelTreeWidgetBase::createContextMenu(QTreeWidgetItem* item, ProjectData::ProjectData* projectData)
	{

	}

}