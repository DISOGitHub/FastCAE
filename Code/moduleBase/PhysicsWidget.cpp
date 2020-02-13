#include "PhysicsWidget.h"
#include "mainWindow/mainWindow.h"
#include <QContextMenuEvent>
#include "moduleBase/modelTreeItemType.h"
#include "DialogCreatePhysicsModel.h"
#include "TreeConfigReader.h"
#include "ModelData/modelDataSingleton.h"
#include "ModelData/modelDataBase.h"
#include "ProjectTreeList.h"
#include "ProjectTreeBase.h"
#include "ProjectTreeType1.h"
#include "ProjectTreeType2.h"
#include <QMenu>
#include <QAction>
#include <assert.h>
#include <QDebug>

namespace ModuleBase
{

	PhysicsWidget::PhysicsWidget(GUI::MainWindow* mainwindow, QWidget* parent /* = 0 */) : _mainWindow(mainwindow)
	{
	//	this->setContentsMargins(0, 0, 0, 0);
		_materialRoot = new QTreeWidgetItem(this, TreeItemType::MaterialRoot);
		_materialRoot->setText(0, tr("Materials"));
		_physicsModelRoot = new QTreeWidgetItem(this, TreeItemType::PhyaicsModelRoot);
		_physicsModelRoot->setText(0, tr("Physics Model"));
		this->addTopLevelItem(_materialRoot);
		this->addTopLevelItem(_physicsModelRoot);
		TreeConfigReader reader(":/configFiles/treeConfig.config");
		reader.read();
		init();
	}
	PhysicsWidget::~PhysicsWidget()
	{
		this->clear();
	}
	void PhysicsWidget::init()
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
		/*鼠标事件*/
		connect(_mainWindow, SIGNAL(treeMouseEvent(int, QTreeWidgetItem*, int)), this, SLOT(on_TreeMouseEvent(int, QTreeWidgetItem*, int)));
		/*创建物理模型*/
		connect(_mainWindow, SIGNAL(createPhysiceModelSig()), this, SLOT(createPhysicsModel()));

	}

	QTreeWidgetItem* PhysicsWidget::getProjectRoot(QTreeWidgetItem* item) const
	{
		if (nullptr == item && nullptr == _curretnItem)
			return nullptr;
		if (item == nullptr)
			item = _curretnItem;
		if (item->parent() == nullptr) return item;
		if (item->type() == TreeItemType::MaterialRoot || item->parent()->type() == TreeItemType::MaterialRoot) return item;

		while (item->type() != TreeItemType::ProjectRoot)
		{
			item = item->parent();
			if (item == nullptr) break;
		}
		if (item != nullptr) return item;
		return nullptr;

	}
	QTreeWidgetItem* PhysicsWidget::getProjectRoot(const int pid) const
	{
		int n = _physicsModelRoot->childCount();
		for (int i = 0; i < n; ++i)
		{
			QTreeWidgetItem* item = _physicsModelRoot->child(i);
			int id = item->data(0, Qt::UserRole).toInt();
			if (id == pid)
				return item;
		}
		return nullptr;
	}
	///contextMenuEvent   重写了底层函数   相当于右键点击
	void PhysicsWidget::contextMenuEvent(QContextMenuEvent *evnt)
	{
		emit printMessageSignal("PhysicsWidget contextMenuEvent star");

		_curretnItem = currentItem();
		if (!_curretnItem) return;
		QTreeWidgetItem* item = getProjectRoot(_curretnItem);
		if (nullptr == item)
			return;
		assert(item);
		const int proID = item->data(0, Qt::UserRole).toInt();
//		on_TreeMouseEvent(1, _curretnItem, proID);
		emit(mouseEvent(1, _curretnItem, proID));
	}
	void PhysicsWidget::singleClicked(QTreeWidgetItem* item, int col)
	{
		_curretnItem = item;
		QTreeWidgetItem* rootitem = getProjectRoot(_curretnItem);
		assert(rootitem);
		const int proID = rootitem->data(0, Qt::UserRole).toInt();
//		on_TreeMouseEvent(0, _curretnItem, proID);
		emit mouseEvent(0, _curretnItem, proID);
	}
	void PhysicsWidget::doubleClicked(QTreeWidgetItem* item, int col)
	{
		_curretnItem = item;
		QTreeWidgetItem* rootitem = getProjectRoot(_curretnItem);
		assert(rootitem);
		const int proID = rootitem->data(0, Qt::UserRole).toInt();

//		on_TreeMouseEvent(0, _curretnItem, proID);
		emit mouseEvent(2, item, proID);
	}

	void PhysicsWidget::on_TreeMouseEvent(int evevntType, QTreeWidgetItem* item, int id)
	{
		if (id == 0)
		{
			switch (evevntType)
			{
			case 0://左键单击
				on_singleClicked(item);
				break;
			case 1://右键单击
				createContextMenu(item);
				break;
			case 2://左键双击
				on_doubleClicked(item);
				break;
			default:
				assert(0);
				break;
			}
		}
		else if (_idTreeHash.contains(id))
		{
			ProjectTreeBase* tree = _idTreeHash[id];
			tree->on_MouseEvent(evevntType, item);
		}
	}
	void PhysicsWidget::on_singleClicked(QTreeWidgetItem* item)
	{

	}
	void PhysicsWidget::on_doubleClicked(QTreeWidgetItem* item)
	{

	}
	void PhysicsWidget::createContextMenu(QTreeWidgetItem* item)
	{
		QMenu pop_menu;
		QAction *action = nullptr;

		if (item->type() == TreeItemType::MaterialRoot)
		{
		}
		else if (item->type() == TreeItemType::PhyaicsModelRoot)
		{
			action = pop_menu.addAction("Create Physics Model");
			connect(action, SIGNAL(triggered()), this, SLOT(createPhysicsModel()));
		}
		pop_menu.exec(QCursor::pos());
	}
	void PhysicsWidget::createPhysicsModel()
	{
		CreatePhysicsModel dlg(this);
		dlg.exec();
	}
	void PhysicsWidget::updateTree()
	{
		blockSignals(true);
		_physicsModelRoot->takeChildren();
		QList<ProjectTreeBase*> ptrees = _idTreeHash.values();
		for (int i = 0; i < ptrees.size(); ++i) delete ptrees.at(i);
		_idTreeHash.clear();

		ModelData::ModelDataSingleton *s = ModelData::ModelDataSingleton::getinstance();
		int n = s->getModelCount();
		for (int i = 0; i < n; ++i)
		{
			ModelData::ModelDataBase* model = s->getModelAt(i);
			const ProjectTreeType treeType = model->getTreeType();
			const int modelid = model->getID();
			ProjectTreeInfo* info = ProjectTreeInfo::getInstance();
			assert(info->isContains(treeType));
			ProjectTreeBase* treeTemplate = info->getProjectTree(treeType);
			ProjectTreeBase* tree = nullptr;
			switch (treeType)
			{
			case  TreeType + 1:
				tree = new ProjectTreeType1;
				
			case TreeType + 2:
				tree = new ProjectTreeType2;
			default:
				break;
			}
			treeTemplate->copy(tree);
			tree->setData(model);
			tree->createTree(_physicsModelRoot, _mainWindow);
			_idTreeHash[modelid] = tree;
		}
		_physicsModelRoot->setExpanded(true);
		blockSignals(false);
	}
}