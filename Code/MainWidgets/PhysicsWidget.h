/***************
模型树基类
libaojun
2017.09.05
***************/
#ifndef _MODELTREEBASE_H_
#define _MODELTREEBASE_H_

#include <QTreeWidget>
#include <QHash>
#include "moduleBase/messageWindowBase.h"

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

namespace ProjectTree
{
	class ProjectTreeBase;
}

namespace DataProperty
{
	class DataBase;
}

namespace MainWidget
{
	class /*MODULEBASEAPI*/ PhysicsWidget : public QTreeWidget
	{
		Q_OBJECT
	public:
		PhysicsWidget(GUI::MainWindow* mainwindow, QWidget* parent = 0);
		~PhysicsWidget();
		
		QTreeWidgetItem* getProjectRoot(QTreeWidgetItem* item = nullptr ) const;
		QTreeWidgetItem* getProjectRoot(const int pid) const;
		ProjectTree::ProjectTreeBase* getProjectByID(int id);

		void clearData();
		void reTranslate();
		void updateMaterialStatus();
		
	signals:
		void updatePhysicsTreePySig();
	    void importProjectSig(int id);
		void caseRenameSig(int pid, QString newname);
		void showMessage(ModuleBase::Message);

	public slots:
		virtual void updateTree();
		void updateMaterialTree();
		void updatePhysicsTree();
		void removeCaseComponentSlot(int);
		void renameCaseComponentSlot(int);


	protected slots:
		void contextMenuEvent(QContextMenuEvent *event) override;
		void singleClicked(QTreeWidgetItem* item,int col);
		void doubleClicked(QTreeWidgetItem* item, int col);
		void on_TreeMouseEvent(int evevntType, QTreeWidgetItem* item, int projectID);
		void createPhysicsModel();
		void renameProject();
		void deleteProject();
		void solveProject();
		void importProjectPy(); 
		void importProject(int id);
		void openProjectPath();

		///<MG load material from lib
		void slot_load_from_material_lib();

		//从材料库中删除
		void slot_remove_from_material_lib();

		///<MG create material
		void slot_create_material();

		///<MG delete material
		void slot_delete_material();

		///<MG add to material lib
		void slot_add_to_material_lib();

//		void slot_action_open();

	signals:
		/*鼠标信号，eventType 0- 左键单机，1-右键单击，2- 左键双击*/
		void mouseEvent(int eventType, QTreeWidgetItem* item, int projectID);
		//打印信息
		void printMessageSignal(QString _msg);
		///显示属性框
		void disPlayProp(DataProperty::DataBase* pops); 
		//求解
		void solveProject(int id);
		//更新action状态
		void updateActionStates();

		void updatePropertyTableTree();
		void deleteCaseSig(int caseid);
		void endaleGraphWinKey(bool on);

	private:
		void init();
		void on_singleClicked(QTreeWidgetItem* item);
		void on_doubleClicked(QTreeWidgetItem* item);
		void createContextMenu(QTreeWidgetItem* item);
		void closeEditor(QWidget * editor, QAbstractItemDelegate::EndEditHint hint) override;
		
		void caseRename(int pid, QString newname);

		//******封装脚本调用，其他禁用********
		friend class MainWidgetPy;
		void deleteCase(int caseid);

		//**********************************
	
	protected:
		GUI::MainWindow* _mainWindow{};
		QTreeWidgetItem* _curretnItem{};
		QTreeWidgetItem* _materialRoot{};
		QTreeWidgetItem* _physicsModelRoot{};
		QHash<int, ProjectTree::ProjectTreeBase*> _idTreeHash{};
		//Ui::DialogCreateMaterial _ui{};
	};

}



#endif