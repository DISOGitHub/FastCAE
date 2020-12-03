#ifndef SHIPTREEDOCK_H_
#define SHIPTREEDOCK_H_

#include "HullPluginAPI.h"
#include "moduleBase/dockWidgetBase.h"
#include <QTreeWidget>

/*
namespace Ui
{
	class DialogHullCut;
}*/

namespace PluginShip
{
	

	class HULLPLUGINAPI ShipTreeDock :  public ModuleBase::DockWidgetBase
	{
		Q_OBJECT
	public:
		ShipTreeDock(GUI::MainWindow* m,QAction* ac);
		~ShipTreeDock();
		void init();
	private:
		void closeEvent(QCloseEvent *);
	
	private slots:
		//在这里添加action槽函数
	void slotOpenDialog(QTreeWidgetItem*item, int i);


		
		

	private:
		QTreeWidget * _treeWidget{};
		//Ui::DialogHullCut* _ui{};
		QAction* _ac;
	};
}



#endif