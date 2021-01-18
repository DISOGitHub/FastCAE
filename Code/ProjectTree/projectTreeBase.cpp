#include "projectTreeBase.h"
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QMenu>
#include <QAction>
#include <QTime>
#include "ConfigOptions/TreeItemData.h"
#include "ConfigOptions/ProjectTreeInfo.h"
#include "mainWindow/mainWindow.h"
#include "ModelData/modelDataBase.h"
#include "ModelData/modelDataSingleton.h"
#include "settings/busAPI.h"
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QCoreApplication>

namespace ProjectTree
{
	ProjectTreeBase::ProjectTreeBase(GUI::MainWindow *mainwindow)
		:_mainWindow(mainwindow)
	{
		if (_mainWindow != nullptr)
		{
			connect(this, SIGNAL(openPostWindowSig(Post::PostWindowBase*)), _mainWindow, SIGNAL(openPostWindowSig(Post::PostWindowBase*)));
			connect(this, SIGNAL(disPlayProp(DataProperty::DataBase*)), _mainWindow, SIGNAL(updateProperty(DataProperty::DataBase*)));
			connect(this, SIGNAL(showPostWindowInfo(int, int)), _mainWindow, SIGNAL(showPostWindowInfoSig(int, int)));
			connect(_mainWindow, SIGNAL(solveProjectSig(int, int)), this, SLOT(solveBegin(int)));
			connect(_mainWindow, SIGNAL(processFinished(int)), this, SLOT(solveFinished(int)));
			connect(_mainWindow, SIGNAL(openRealTimeWindowSig(Post::RealTimeWindowBase*, int)), this, SLOT(realTimeWindowOpened(Post::RealTimeWindowBase*, int)));
			connect(_mainWindow, SIGNAL(closeRealTimeWindowSig(Post::RealTimeWindowBase*)), this, SLOT(realTimeWindowClosed(Post::RealTimeWindowBase*)));
			connect(this, SIGNAL(dispalyParaWidget(QWidget*)), _mainWindow, SIGNAL(updateParaWidget(QWidget*)));
		}
	}
	ProjectTreeBase::~ProjectTreeBase()
	{
		if (_root == nullptr) return;
		_root->takeChildren();
		_itemList.clear();
	}
	void ProjectTreeBase::clearTreeItems()
	{
		const int n = _itemList.size();
		for (int i = 0; i < n; ++i)
		{
			delete _itemList[i];
		}
		_itemList.clear();
	}
	void ProjectTreeBase::setName(const QString &fileName)
	{
		_name = fileName;
	}
	QString ProjectTreeBase::getName()
	{
		return _name;
	}
	void ProjectTreeBase::setType(ProjectTreeType type)
	{
		_treeType = type;
	}
	void ProjectTreeBase::appendItem(ConfigOption::TreeItem* item)
	{
		_itemList.append(item);
	}
	void ProjectTreeBase::createTree(QTreeWidgetItem* phyroot, GUI::MainWindow* mainwindow)
	{
		if (_data == nullptr) return;
		_mainWindow = mainwindow;
 //		QTreeWidgetItem* root = new QTreeWidgetItem(phyroot, TreeItemType::ProjectRoot);

		initBasicNode(phyroot);
		QString lang = Setting::BusAPI::instance()->getLanguage();
		double time = _data->getSolveTime();
		if (time > 0) _solveStatus = Finished;
		_data->setOutputFileName(_outputFile);

		int n = _itemList.size();
		for (int i = 0; i < n; ++i)
		{
			QTreeWidgetItem* parent = _root;
			ConfigOption::TreeItem* item = _itemList.at(i);
			if (item == nullptr) continue;
			QString sparent = item->getParent();
			if (_textItemHash.contains(sparent)) parent = _textItemHash.value(sparent);
			QTreeWidgetItem* treeitem = new QTreeWidgetItem(parent, item->getType());
			treeitem->setData(0, Qt::UserRole + 1, i + 1);  //留空0，标记为不是配置文件创建
			treeitem->setData(1, Qt::UserRole, item->getText());  //英文 
			treeitem->setData(1, Qt::UserRole + 1, item->getChinese());//中文
			QString text = item->getText();
			if (lang == "Chinese")
				text = item->getChinese();
			if (text.isEmpty()) text = item->getText();

			treeitem->setText(0,text);
			QString sicon = item->getIcon();
			QString icon = qApp->applicationDirPath() + "/../ConfigFiles/Icon/" + sicon;
			treeitem->setIcon(0, QIcon(icon));
//			qDebug() << text << item->getType();
			_textItemHash[item->getText()] = treeitem;
		}
		for (int i = 0; i < _disableItems.size(); ++i)
		{
			QString s = _disableItems.at(i);
			auto item = _textItemHash.value(s);
			if (item == nullptr) continue;
			item->setHidden(true);
		}
		
	}
	void ProjectTreeBase::initBasicNode(QTreeWidgetItem* phyroot)
	{
		_root = new QTreeWidgetItem(phyroot, TreeItemType::ProjectRoot);
		_root->setFlags(_root->flags() | Qt::ItemIsEditable);
		_root->setIcon(0, QIcon(":/QUI/icon/physics.png"));
		int id = _data->getID();
		_root->setText(0, _data->getName());  //根据项目名称确定
		_root->setData(0, Qt::UserRole, _data->getID());
		_root->setExpanded(true);
		
	}
	void ProjectTreeBase::on_MouseEvent(int eventType, QTreeWidgetItem* item)
	{   
		_currentItem = item;
// 		TreeItemType type = (TreeItemType)item->type();
// 		if (type == Undefined) return;

		switch (eventType)
		{
		case 0: singleClicked();break;
		case 1:createContextMenu();break;
		case 2:doubleClicked();break;
		default:break;
		}
	}
// 	void ProjectTreeBase::copy(ProjectTreeBase* tree)
// 	{
// 	//	ProjectTree* tree = new ProjectTree;
// 		tree->setType(_treeType);
// 		tree->setName(_name);
// 		for (int i = 0; i < _itemList.size(); ++i)
// 		{
// 			ConfigOption::TreeItem* item = _itemList.at(i);
// 			ConfigOption::TreeItem* newitem = new ConfigOption::TreeItem;
// 			newitem->copy(item);
// 			tree->appendItem(newitem);
// 		}
// 		tree->setOutputFileName(_outputFile);
// 		tree->setDisableItems(_disableItems);
// //		_disableItems = tree->getDisableItems();
// 		qDebug() << _disableItems;
// //		return tree;
// 	}
	void ProjectTreeBase::copy(ConfigOption::ProjectTreeInfo* info)
	{
		_treeType = info->type();
		_name = info->getName();
		QList<ConfigOption::TreeItem*> items = info->getItemList();
		for (int i = 0; i < items.size(); ++i)
		{
			ConfigOption::TreeItem* item = items.at(i);
			ConfigOption::TreeItem* newitem = new ConfigOption::TreeItem;
			newitem->copy(item);
			this->appendItem(newitem);
		}
		_outputFile = info->getOutputFileName();
		_disableItems = info->getDisableItems();

	}
	void ProjectTreeBase::setData(ModelData::ModelDataBase* data)
	{
		_data = data;
		transferData();
	}
	ModelData::ModelDataBase* ProjectTreeBase::getData()
	{
		return _data;
	}
	void ProjectTreeBase::setDisableItems(QStringList s)
	{
		_disableItems = s;
	}
	QStringList ProjectTreeBase::getDisableItems()
	{
		return _disableItems;
	}

	void ProjectTreeBase::setCurrentItem(QTreeWidgetItem* item)
	{
		_currentItem = item;
	}

	void ProjectTreeBase::singleClicked()
	{
		//qDebug() << "parent s";
	
	}
	void ProjectTreeBase::doubleClicked()
	{
		qDebug() << "parent d";
	}
	void ProjectTreeBase::createContextMenu()
	{
//		qDebug() << "parent c";
		QMenu pop_menu;
		contextMenu(&pop_menu);
		pop_menu.exec(QCursor::pos());
	}
	void ProjectTreeBase::contextMenu(QMenu* menu)
	{
		QAction* action = nullptr;
		int itemindex = _currentItem->data(0, Qt::UserRole + 1).toInt();
		TreeItemType type = (TreeItemType)_currentItem->type();
		if (itemindex > 0)
		{
			ConfigOption::TreeItem* treeItem = _itemList.at(itemindex - 1);
			const int n = treeItem->getContextMenuCount();
			if (n < 1) return;
			for (int i = 0; i < n; ++i)
			{
				QString menuText = treeItem->getContextMenuAt(i);
				action = menu->addAction(menuText);
				switch (type)
				{
				case ProjectPostSetting:
					if (0 == i) connect(action, SIGNAL(triggered()), this, SLOT(openPostWindow()));

					break;
				case ProjectComponent:
					break;
				case ProjectSolver:
					break;
				default: break;
				}
			}
		}
	}
	void ProjectTreeBase::updateTree()
	{

	}
	void ProjectTreeBase::updateTreeByType(const TreeItemType type)
	{

	}
	QList<QTreeWidgetItem*> ProjectTreeBase::getItemByType(const TreeItemType type,QTreeWidgetItem* root)
	{
		QList<QTreeWidgetItem*> items;
		if (root == nullptr) root = _root;
		if (root->type() == type) items.append(root);
		const int n = root->childCount();
		for (int i = 0; i < n; ++i)
		{
			QTreeWidgetItem* cc = root->child(i);
//			qDebug() << cc->text(0);
			if (cc->type() == type)
				items.append(cc);
			if (cc->childCount() > 0)
			{
				QList<QTreeWidgetItem*> il = getItemByType(type, cc);
				items.append(il);
			}
		}
		return items;
	}
	SolveStatus ProjectTreeBase::getSolveStatus()
	{
		return _solveStatus;
	}
	void ProjectTreeBase::solveBegin(int proIndex)
	{
		ModelData::ModelDataSingleton* s = ModelData::ModelDataSingleton::getinstance();
		int id = s->getModelIDByIndex(proIndex);
		if (id < 0) return;
		if (id != _data->getID()) return;
		_solveStatus = Solving;
		if (_timer == nullptr) _timer = new QTime;
		_timer->start();
		emit solveStatusChanged();
	}
	void ProjectTreeBase::solveFinished(int proid)
	{
		if (proid != _data->getID()) return;
		_solveStatus = Finished;
		if (_timer == nullptr) return;
		double time = _timer->elapsed();
//		qDebug() << time;
		delete _timer; _timer = nullptr;
		_data->setSolveTime(time);
		emit solveStatusChanged();
	}
	void ProjectTreeBase::setOutputFileName(QString fileName)
	{
		_outputFile = fileName;
	}
	void ProjectTreeBase::reTranslate()
	{
		QString lang = Setting::BusAPI::instance()->getLanguage();
		QList<QTreeWidgetItem*> itemList = _textItemHash.values();
		const int n = itemList.size();
		if (lang == "Chinese")
		{
			for (int i = 0; i < n; ++i)
			{
				QTreeWidgetItem* item = itemList.at(i);
				if (item == nullptr) continue;
				QString text = item->data(1, Qt::UserRole + 1).toString();
				if (text.isEmpty()) continue;
				item->setText(0, text);
			}
		}
		else if (lang == "English")
		{
			for (int i = 0; i < n; ++i)
			{
				QTreeWidgetItem* item = itemList.at(i);
				if (item == nullptr) continue;
				QString text = item->data(1, Qt::UserRole).toString();
				if (text.isEmpty()) continue;
				item->setText(0, text);
			}
		}
	}
	void ProjectTreeBase::setChinese(QString s)
	{
		_chinese = s;
	}
	QString ProjectTreeBase::getChinese()
	{
		return _chinese;
	}
	void ProjectTreeBase::realTimeWindowOpened(Post::RealTimeWindowBase* w, int id)
	{
		if (id == _data->getID())
			_realTimeWin = w;

	}
	void ProjectTreeBase::realTimeWindowClosed(Post::RealTimeWindowBase* w)
	{
		if (_realTimeWin == w)
			_realTimeWin = nullptr;
	}
	bool ProjectTreeBase::isFileExist(QString fileName)
	{
		QFile f(fileName);
		if (!f.exists())
		{
			QMessageBox::warning(nullptr, tr("Warning"), tr("File %1 is not exist!").arg(fileName));
			return false;
		}
		return true;
	}


}