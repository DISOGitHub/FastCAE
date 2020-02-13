#include "ProjectTreeExtend.h"
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

	}
	void ProjectTreeExtend::contextMenu(QMenu* menu)
	{
		qDebug() << _name << "  Right click";



		ProjectTreeWithBasicNode::contextMenu(menu);
	}
	void ProjectTreeExtend::singleClicked()
	{
//		qDebug() << _name << "  single click";;
		this->getCurrentItemData();
		ProjectTreeWithBasicNode::singleClicked();
	}
	void ProjectTreeExtend::doubleClicked()
	{
		qDebug() << _name << "  double click";

		ProjectTreeWithBasicNode::doubleClicked();
	}

	void ProjectTreeExtend::createTree(QTreeWidgetItem* root, GUI::MainWindow* mainwindow)
	{
		ProjectTreeBase::createTree(root, mainwindow);
		//**********对配置节点进行操作***********
		

		//***************
	}

	void ProjectTreeExtend::updateMeshSubTree()
	{
		////add here********


		/////************

		ProjectTreeWithBasicNode::updateMeshSubTree();
	}



}