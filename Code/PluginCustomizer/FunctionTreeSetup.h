#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QResizeEvent>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDialog>
#include <QObject>
#include <QTreeWidgetItem>
#include <QResizeEvent>
#include "moduleBase/dockWidgetBase.h"

namespace GUI
{
	class MainWindow;
}


namespace Ui {
	class FunctionTreeSetup;
}

namespace MainWidget
{
	class PropertyTable;
}

namespace DataProperty
{
	class ParameterBase;
	class DataBase;
	class ParameterGroup;
}

namespace ConfigOption
{
	class ProjectTreeInfo;
	class PostCurve;
	class PostConfigInfo;
}

namespace FastCAEDesigner
{
	class ModelBase;

	class FunctionTreeSetup : public ModuleBase::DockWidgetBase
	{
		Q_OBJECT
	public:
		FunctionTreeSetup(GUI::MainWindow* mainwindow, QWidget *parent = 0);
		~FunctionTreeSetup();
		QList<ModelBase*> GetPhysicsList();
		QList<ModelBase*> GetMaterialList();

	public:
		void resizeEvent(QResizeEvent *e);
		void showEvent(QShowEvent *);

		int returnParent(QTreeWidgetItem* treeparent);
		QMenu* createTreeRootMenu(QMenu* menu, QTreeWidgetItem* treeNode);
		QMenu* createTreeNodeMenu(QMenu* menu, QTreeWidgetItem* childNode);
		QMenu* createTreePostMenu(QMenu* menu,QTreeWidgetItem* postNode);

// 		QMenu* createShowMenu(QMenu* menu, QTreeWidgetItem* treeNode);
// 		QAction* createInsertAction();
// 		QAction* createHideOrDeleteAction();
// 	signals:
// 		void sendChildItem(QTreeWidgetItem*	child);
// 		

	private slots:
		void OnProjectTreeRightClicked(const QPoint &point);
		void OnInsertChild();
		void OnHideThis();
		void OnShowThis(int childindex);
		void OnDeleteItem(/*int flag*/);
		void OnInsertPhysicsTree();
		void OnProjectTreeDoubleClicked(QTreeWidgetItem* treeItem);
		void OnProjectTreeItemClicked(QTreeWidgetItem* treeItem,int index);

		void OnInsertMaterialTree();
		void OnDeleteAllMaterialItem();

		void onShowParameterLinkage(int type);

	private:
		void Init();
		void InitTreeWidget();
		void FillTreeWidget();
		void FillTreePhysics();
		void FillTreeMaterial();
		//void updatePhysicsTree();
		void UpdateDataBase(ModelBase* model);

		QTreeWidgetItem* CreateTreeWidgetItem(QTreeWidgetItem *root, QString text, QString iconName);
		//ModelBase* CreateCase(QString nameChn, QString nameEng, QString icon);
		ModelBase* CreatePhysics(QString nameEng, QString nameChn);

		void DeletePhysicsModel(ModelBase *model);
		QList<QString> GetCaseUsedNameList(QList<ModelBase*> modelList, int nameType);

		//初始化构造功能树及节点的数据
		void InitProjectData();
		void IniMaterialProjectData();
		void InitPhysicsProjectData();
		void FillSimulationAndSolverChildModel(ModelBase* caseRoot, ConfigOption::ProjectTreeInfo* treeModel, int projectTreeType);
		ModelBase* CreateModelFactory(int type);
		ModelBase* GetSpecialTypeModel(QList<ModelBase*> modelList, int type);
		void FillSimulationRootParameterData(ModelBase* model, int treeType, int dataType);
		void FillBcData(ModelBase* model, int treeType);
		void FillPostData(ModelBase* model, int treeType);
		void FillMonitorData(ModelBase* model, int treeType);
		DataProperty::DataBase* GetChildDataBase(int type, int id);
		int getDisableItemType(QString disableItem);
		void InitPhysicsTreeNodeVisable(QStringList disableItemList, ModelBase* caseRootModel);
		QList<ConfigOption::PostCurve*> getPostCurve(QList<ConfigOption::PostCurve*>, QString);
		//初始化构造功能树及节点的数据
		void ClearModelBaseList(QList<ModelBase*> &list);

		//获取参数信息列表
		void insertParameterToList(DataProperty::DataBase* model);

		//20200327
		void SetParentModel(QTreeWidgetItem* item);

		ModelBase* getSimulationSolverChildNameModel(QList<ModelBase*> modelList, QString name);
		ModelBase* getChildModelFromName(ModelBase* model, QString name);

	private:
		Ui::FunctionTreeSetup *ui;
		GUI::MainWindow* _mainwindow;
		MainWidget::PropertyTable *_propTable{nullptr};
		QTreeWidgetItem* _curretnItem{ nullptr };
		QTreeWidgetItem* _materialRoot{ nullptr };
		QTreeWidgetItem* _physicsModelRoot{ nullptr };

		QList<ModelBase*> _physicsList;
		QList<ModelBase*> _materialList;

		//xuxinwei   20200325
		QList<DataProperty::ParameterBase*> _parameterList;
		QList<DataProperty::ParameterGroup*> _parameterGroupList;
		
	};
}
#endif // WIDGET_H
