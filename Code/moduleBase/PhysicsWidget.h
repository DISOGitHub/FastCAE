/***************
模型树基类
libaojun
2017.09.05
***************/
#ifndef _MODELTREEBASE_H_
#define _MODELTREEBASE_H_

#include <QTreeWidget>
#include "moduleBaseAPI.h"
#include "moduleBase/ModuleBase.h"
#include <qelapsedtimer.h>
#include <qthread.h>
#include <QHash>

class QMenu;
class QTreeWidgetItem;

namespace GUI
{
	class MainWindow;
}

namespace ProjectData
{
	class ProjectData;
}

namespace ModuleBase
{
	class ProjectTreeBase;

	class MODULEBASEAPI PhysicsWidget : public QTreeWidget
	{
		Q_OBJECT
	public:
		PhysicsWidget(GUI::MainWindow* mainwindow, QWidget* parent = 0);
		~PhysicsWidget();
		
		QTreeWidgetItem* getProjectRoot(QTreeWidgetItem* item = nullptr ) const;
		QTreeWidgetItem* getProjectRoot(const int pid) const;

	public slots:
		virtual void updateTree();

	protected slots:
		void contextMenuEvent(QContextMenuEvent *event) override;
		void singleClicked(QTreeWidgetItem* item,int col);
		void doubleClicked(QTreeWidgetItem* item, int col);
		

		void on_TreeMouseEvent(int evevntType, QTreeWidgetItem* item, int projectID);
		void createPhysicsModel();

	signals:
		/*鼠标信号，eventType 0- 左键单机，1-右键单击，2- 左键双击*/
		void mouseEvent(int eventType, QTreeWidgetItem* item, int projectID);

		void printMessageSignal(QString _msg);

	private:
		void init();
		void on_singleClicked(QTreeWidgetItem* item);
		void on_doubleClicked(QTreeWidgetItem* item);
		void createContextMenu(QTreeWidgetItem* item);
	
	protected:
		GUI::MainWindow* _mainWindow{};
		QTreeWidgetItem* _curretnItem{};
//		QHash<QTreeWidgetItem*, ProjectTree*> _itemTreeHash{};
		QTreeWidgetItem* _materialRoot{};
		QTreeWidgetItem* _physicsModelRoot{};

		QHash<int, ProjectTreeBase*> _idTreeHash{};
	};

}



#endif