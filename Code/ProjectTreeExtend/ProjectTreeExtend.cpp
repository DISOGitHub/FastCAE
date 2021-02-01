#include "ProjectTreeExtend.h"
#include "ProjectTreeConfig.h"
#include "ModelData/modelDataBaseExtend.h"
#include <QTreeWidgetItem>
#include <QSignalMapper>
#include <QMenu>
#include <QDebug>


namespace ProjectTree
{
	ProjectTreeExtend::ProjectTreeExtend(GUI::MainWindow* mainwindow)
		:ProjectTreeWithBasicNode(mainwindow)
	{

	}
	ProjectTreeExtend::~ProjectTreeExtend()
	{
		if (_rootMapper != nullptr) delete _rootMapper;
		if (_conMapper != nullptr) delete _conMapper;
	}
	void ProjectTreeExtend::contextMenu(QMenu* menu)
	{
		//qDebug() << _name << "  Right click";
		ProjectTreeWithBasicNode::contextMenu(menu);
		if (_currentItem->type() == TreeItemType::ProjectRoot)
		{
			if (_rootMapper != nullptr) 
			{
				delete _rootMapper;
				_rootMapper = nullptr;
			}
			
			QStringList  funs = ProjectTreeConfig::getRootContextMenuDescribe(_treeType);
			if (!funs.isEmpty())
			{
				_rootMapper = new QSignalMapper;
				QAction* action{};
				for (QString des : funs)
				{
					action = menu->addAction(des);
					connect(action, SIGNAL(triggered()), _rootMapper, SLOT(map()));
					_rootMapper->setMapping(action, des);
				}
				connect(_rootMapper, SIGNAL(mapped(QString)), this, SLOT(rootConMenu(QString)));
			}	
		}
		else
		{
			if (_conMapper != nullptr)
			{
				delete _conMapper;
				_conMapper = nullptr;
			}
			QString itemdes = _textItemHash.key(_currentItem);
			QStringList funs = ProjectTreeConfig::getContextMenuDescribe(_treeType,itemdes);
			if (!funs.isEmpty())
			{
				_conMapper = new QSignalMapper;
				QAction* action{};
				for (QString des : funs)
				{
					action = menu->addAction(des);
					connect(action, SIGNAL(triggered()), _conMapper, SLOT(map()));
					_conMapper->setMapping(action, des);
				}
				connect(_conMapper, SIGNAL(mapped(QString)), this, SLOT(itemConMenu(QString)));
			}

		}
		
		
	}
	void ProjectTreeExtend::singleClicked()
	{
//		qDebug() << _name << "  single click";;
		this->getCurrentItemData();
		ProjectTreeWithBasicNode::singleClicked();
	}
	void ProjectTreeExtend::doubleClicked()
	{
		//qDebug() << _name << "  double click";

		ProjectTreeWithBasicNode::doubleClicked();
	}

	void ProjectTreeExtend::createTree(QTreeWidgetItem* root, GUI::MainWindow* mainwindow)
	{
		ProjectTreeBase::createTree(root, mainwindow);
		//**********对配置节点进行操作***********
		

		//***************
	}

	void ProjectTreeExtend::updateComponentSubTree()
	{
		////add here********


		/////************

		ProjectTreeWithBasicNode::updateComponentSubTree();
	}



	void ProjectTreeExtend::rootConMenu(QString des)
	{
		CONTECTMENU fun = ProjectTree::ProjectTreeConfig::getRootContextMenuFunction(_treeType, des);
		if (fun != nullptr)
			fun(_modelDataExtend, this);
	}

	void ProjectTreeExtend::itemConMenu(QString des)
	{
		QString itemdes = _textItemHash.key(_currentItem);
		CONTECTMENU fun = ProjectTree::ProjectTreeConfig::getRootContextMenuFunction(_treeType, des,itemdes);
		if (fun != nullptr)
			fun(_modelDataExtend, this);
		
	}

}