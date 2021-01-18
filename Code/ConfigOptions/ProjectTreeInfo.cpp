#include "ProjectTreeInfo.h"
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QMenu>
#include <QAction>
#include <QTime>
#include "TreeItemData.h"
#include <QDebug>

namespace ConfigOption
{

	void ProjectTreeInfo::clearTreeItems()
	{
		const int n = _itemList.size();
		for (int i = 0; i < n; ++i)
		{
			delete _itemList[i];
		}
		_itemList.clear();
	}
	void ProjectTreeInfo::setName(const QString &fileName)
	{
		_name = fileName;
	}
	QString ProjectTreeInfo::getName()
	{
		return _name;
	}
	void ProjectTreeInfo::setType(ProjectTreeType type)
	{
		_treeType = type;
	}
	void ProjectTreeInfo::appendItem(TreeItem* item)
	{
		_itemList.append(item);
	}
// 	void ProjectTreeInfo::createTree(QTreeWidgetItem* phyroot, GUI::MainWindow* mainwindow)
// 	{
// #ifndef CONFIGOPTIONS_API
// 		if (_data == nullptr) return;
// 		_mainWindow = mainwindow;
//  //		QTreeWidgetItem* root = new QTreeWidgetItem(phyroot, TreeItemType::ProjectRoot);
// 
// 		initBasicNode(phyroot);
// 		QString lang = Setting::BusAPI::instance()->getLanguage();
// 		double time = _data->getSolveTime();
// 		if (time > 0) _solveStatus = Finished;
// 		_data->setOutputFileName(_outputFile);
// 
// 		int n = _itemList.size();
// 		for (int i = 0; i < n; ++i)
// 		{
// 			QTreeWidgetItem* parent = _root;
// 			TreeItem* item = _itemList.at(i);
// 			if (item == nullptr) continue;
// 			QString sparent = item->getParent();
// 			if (_textItemHash.contains(sparent)) parent = _textItemHash.value(sparent);
// 			QTreeWidgetItem* treeitem = new QTreeWidgetItem(parent, item->getType());
// 			treeitem->setData(0, Qt::UserRole + 1, i + 1);  //留空0，标记为不是配置文件创建
// 			treeitem->setData(1, Qt::UserRole, item->getText());  //英文 
// 			treeitem->setData(1, Qt::UserRole + 1, item->getChinese());//中文
// 			QString text = item->getText();
// 			if (lang == "Chinese")
// 				text = item->getChinese();
// 			if (text.isEmpty()) text = item->getText();
// 
// 			treeitem->setText(0,text);
// 			QString sicon = item->getIcon();
// 			QString icon = qApp->applicationDirPath() + "/../ConfigFiles/Icon/" + sicon;
// 			treeitem->setIcon(0, QIcon(icon));
// //			qDebug() << text << item->getType();
// 			_textItemHash[item->getText()] = treeitem;
// 		}
// 		for (int i = 0; i < _disableItems.size(); ++i)
// 		{
// 			QString s = _disableItems.at(i);
// 			auto item = _textItemHash.value(s);
// 			if (item == nullptr) continue;
// 			item->setHidden(true);
// 		}
// #endif	
// 	}
// 	void ProjectTreeInfo::initBasicNode(QTreeWidgetItem* phyroot)
// 	{
// #ifndef CONFIGOPTIONS_API
// 		_root = new QTreeWidgetItem(phyroot, TreeItemType::ProjectRoot);
// 		_root->setFlags(_root->flags() | Qt::ItemIsEditable);
// 		_root->setIcon(0, QIcon(":/QUI/icon/physics.png"));
// 		int id = _data->getID();
// 		_root->setText(0, _data->getName());  //根据项目名称确定
// 		_root->setData(0, Qt::UserRole, _data->getID());
// 		_root->setExpanded(true);
// #endif	
// 	}
// 	void ProjectTreeInfo::on_MouseEvent(int eventType, QTreeWidgetItem* item)
// 	{   
// 		_currentItem = item;
// // 		TreeItemType type = (TreeItemType)item->type();
// // 		if (type == Undefined) return;
// 
// 		switch (eventType)
// 		{
// 		case 0: singleClicked();break;
// 		case 1:createContextMenu();break;
// 		case 2:doubleClicked();break;
// 		default:break;
// 		}
// 	}
	void ProjectTreeInfo::copy(ProjectTreeInfo* tree)
	{
	//	ProjectTree* tree = new ProjectTree;
		tree->setType(_treeType);
		tree->setName(_name);
		for (int i = 0; i < _itemList.size(); ++i)
		{
			TreeItem* item = _itemList.at(i);
			TreeItem* newitem = new TreeItem;
			newitem->copy(item);
			tree->appendItem(newitem);
		}
		tree->setOutputFileName(_outputFile);
		tree->setDisableItems(_disableItems);
//		_disableItems = tree->getDisableItems();
		qDebug() << _disableItems;
//		return tree;
	}
// 	void ProjectTreeInfo::setData(ModelData::ModelDataBase* data)
// 	{
// 		_data = data;
// 	}
// 	ModelData::ModelDataBase* ProjectTreeInfo::getData()
// 	{
// 		return _data;
// 	}
	void ProjectTreeInfo::setDisableItems(QStringList s)
	{
		_disableItems = s;
	}
	QStringList ProjectTreeInfo::getDisableItems()
	{
		return _disableItems;
	}
// 	void ProjectTreeInfo::singleClicked()
// 	{
// 		qDebug() << "parent s";
// 	
// 	}
// 	void ProjectTreeInfo::doubleClicked()
// 	{
// 		qDebug() << "parent d";
// 	}
// 	void ProjectTreeInfo::createContextMenu()
// 	{
// //		qDebug() << "parent c";
// 		QMenu pop_menu;
// 		contextMenu(&pop_menu);
// 		pop_menu.exec(QCursor::pos());
// 	}
// 	void ProjectTreeInfo::contextMenu(QMenu* menu)
// 	{
// 		QAction* action = nullptr;
// 		int itemindex = _currentItem->data(0, Qt::UserRole + 1).toInt();
// 		TreeItemType type = (TreeItemType)_currentItem->type();
// 		if (itemindex > 0)
// 		{
// 			TreeItem* treeItem = _itemList.at(itemindex - 1);
// 			const int n = treeItem->getContextMenuCount();
// 			if (n < 1) return;
// 			for (int i = 0; i < n; ++i)
// 			{
// 				QString menuText = treeItem->getContextMenuAt(i);
// 				action = menu->addAction(menuText);
// 				switch (type)
// 				{
// 				case ProjectPostSetting:
// 					if (0 == i) connect(action, SIGNAL(triggered()), this, SLOT(openPostWindow()));
// 
// 					break;
// 				case ProjectMesh:
// 					break;
// 				case ProjectSolver:
// 					break;
// 				default: break;
// 				}
// 			}
// 		}
// 	}
// 	void ProjectTreeInfo::updateTree()
// 	{
// 
// 	}
// 	void ProjectTreeInfo::updateTreeByType(const TreeItemType type)
// 	{
// 
// 	}
// 	QList<QTreeWidgetItem*> ProjectTreeInfo::getItemByType(const TreeItemType type,QTreeWidgetItem* root)
// 	{
// 		QList<QTreeWidgetItem*> items;
// 		if (root == nullptr) root = _root;
// 		if (root->type() == type) items.append(root);
// 		const int n = root->childCount();
// 		for (int i = 0; i < n; ++i)
// 		{
// 			QTreeWidgetItem* cc = root->child(i);
// //			qDebug() << cc->text(0);
// 			if (cc->type() == type)
// 				items.append(cc);
// 			if (cc->childCount() > 0)
// 			{
// 				QList<QTreeWidgetItem*> il = getItemByType(type, cc);
// 				items.append(il);
// 			}
// 		}
// 		return items;
// 	}
// 	SolveStatus ProjectTreeInfo::getSolveStatus()
// 	{
// 		return _solveStatus;
// 	}
// 	void ProjectTreeInfo::solveBegin(int proIndex)
// 	{
// #ifndef CONFIGOPTIONS_API
// 		ModelData::ModelDataSingleton* s = ModelData::ModelDataSingleton::getinstance();
// 		int id = s->getModelIDByIndex(proIndex);
// 		if (id < 0) return;
// 		if (id != _data->getID()) return;
// 		_solveStatus = Solving;
// 		if (_timer == nullptr) _timer = new QTime;
// 		_timer->start();
// 		emit solveStatusChanged();
// #endif
// 	}
// 	void ProjectTreeInfo::solveFinished(int proid)
// 	{
// #ifndef CONFIGOPTIONS_API
// 		if (proid != _data->getID()) return;
// 		_solveStatus = Finished;
// 		if (_timer == nullptr) return;
// 		double time = _timer->elapsed();
// //		qDebug() << time;
// 		delete _timer; _timer = nullptr;
// 		_data->setSolveTime(time);
// 		emit solveStatusChanged();
// #endif
// 	}
	void ProjectTreeInfo::setOutputFileName(QString fileName)
	{
		_outputFile = fileName;
	}
// 	void ProjectTreeInfo::reTranslate()
// 	{
// 		QString lang = Setting::BusAPI::instance()->getLanguage();
// 		QList<QTreeWidgetItem*> itemList = _textItemHash.values();
// 		const int n = itemList.size();
// 		if (lang == "Chinese")
// 		{
// 			for (int i = 0; i < n; ++i)
// 			{
// 				QTreeWidgetItem* item = itemList.at(i);
// 				if (item == nullptr) continue;
// 				QString text = item->data(1, Qt::UserRole + 1).toString();
// 				if (text.isEmpty()) continue;
// 				item->setText(0, text);
// 			}
// 		}
// 		else if (lang == "English")
// 		{
// 			for (int i = 0; i < n; ++i)
// 			{
// 				QTreeWidgetItem* item = itemList.at(i);
// 				if (item == nullptr) continue;
// 				QString text = item->data(1, Qt::UserRole).toString();
// 				if (text.isEmpty()) continue;
// 				item->setText(0, text);
// 			}
// 		}
// 	}
	void ProjectTreeInfo::setChinese(QString s)
	{
		_chinese = s;
	}
	QString ProjectTreeInfo::getChinese()
	{
		return _chinese;
	}

	QList<TreeItem*> ProjectTreeInfo::getItemList()
	{
		return _itemList;
	}

	QString ProjectTreeInfo::getOutputFileName()
	{
		return _outputFile;
	}

	// 	void ProjectTreeInfo::realTimeWindowOpened(Post::RealTimeWindowBase* w, int id)
// 	{
// #ifndef CONFIGOPTIONS_API
// 		if (id == _data->getID())
// 			_realTimeWin = w;
// #endif
// 
// 	}
// 	void ProjectTreeInfo::realTimeWindowClosed(Post::RealTimeWindowBase* w)
// 	{
// 		if (_realTimeWin == w)
// 			_realTimeWin = nullptr;
// 	}
// 	bool ProjectTreeInfo::isFileExist(QString fileName)
// 	{
// 		QFile f(fileName);
// 		if (!f.exists())
// 		{
// 			QMessageBox::warning(nullptr, tr("Warning"), tr("File %1 is not exist!").arg(fileName));
// 			return false;
// 		}
// 		return true;
// 	}


}