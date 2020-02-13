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
	class MODULEBASEAPI ModelTreeWidgetBase : public QTreeWidget
	{
		Q_OBJECT
	public:
		ModelTreeWidgetBase(GUI::MainWindow* mainwindow, QWidget* parent = 0);
		~ModelTreeWidgetBase();
		
		QTreeWidgetItem* getProjectRoot(QTreeWidgetItem* item = nullptr ) const;
		QTreeWidgetItem* getProjectRoot(const int pid) const;

	public slots:
		virtual void updateTree();
	protected slots:
		void contextMenuEvent(QContextMenuEvent *event) override;
		void singleClicked(QTreeWidgetItem* item,int col);
		void doubleClicked(QTreeWidgetItem* item, int col);
		

		void on_TreeMouseEvent(int evevntType, QTreeWidgetItem* item, int projectID);

	signals:
		/*鼠标信号，eventType 0- 左键单机，1-右键单击，2- 左键双击*/
		void mouseEvent(int eventType, QTreeWidgetItem* item, int projectID);

		void printMessageSignal(QString _msg);

	private:
		void init();
		virtual void on_singleClicked(QTreeWidgetItem* item, ProjectData::ProjectData* projectData);
		virtual void on_doubleClicked(QTreeWidgetItem* item, ProjectData::ProjectData* projectData);
		virtual void createContextMenu(QTreeWidgetItem* item, ProjectData::ProjectData* projectData);
	
	protected:
		GUI::MainWindow* _mainWindow{};
		QTreeWidgetItem* _curretnItem{};

	};

}



#endif