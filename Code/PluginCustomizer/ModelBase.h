#ifndef MODELBASE_H
#define MODELBASE_H

#include <QObject>
#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "Common.h"


enum EModelType
{
    Model_Type_TopRoot = 1,
    Model_Type_Root,
    Model_Type_Mesh,
    Model_Type_Simulation,
    Model_Type_Border,
    Model_Type_Solve,
    Model_Type_Monitor,
    Model_Type_Post,
};

namespace DataProperty
{
	class DataBase;
}


namespace FastCAEDesigner
{

	class ModelBase : public QObject
	{
		Q_OBJECT
	public:
		ModelBase(QObject *parent = nullptr);
		ModelBase(QString name, QObject *parent = nullptr);
		ModelBase(QString name, QString iconName, QObject *parent = nullptr);
		ModelBase(QString nameEng, QString nameChn, QString iconName, QObject *parent);
		ModelBase(QString nameEng, QString nameChn, int type, QObject *parent = nullptr);
		ModelBase(QString nameEng, QString nameChn, QString iconName, int type, QObject *parent);
		ModelBase(QString fileName, int type, QObject *parent);
		~ModelBase();

	signals:
		void signal_update();

	public:
		virtual QWidget* ShowEditor(QWidget *widget);
		virtual void ShowEditor();
		virtual int ShowEditor(QTreeWidgetItem* treeItem, QWidget* parent);
		
		virtual void AddNode(ModelBase *model);
		//virtual void AddNode(QTreeWidgetItem* parentItem, ModelBase* model);
		virtual void DelNode(ModelBase *model);
		//virtual void DelNode(QTreeWidgetItem* parentItem, ModelBase* model);
		virtual ModelBase* CreateChildModel(QString nameEng, QString nameChn, QString icon, QObject* parent);
		//virtual ModelBase* CreateChildModel(QString fileName, QObject* parent);
		virtual ModelBase* CreateChildModel(QTreeWidgetItem* parentTreeItem);

		//virtual QMenu* CreateRightClickedMenu(ModelBase* thisModle);




		QTreeWidgetItem* CreateTreeItem(QTreeWidgetItem *root);
		
		void Clear();

		QString GetEngName();
		void SetEngName(QString name);
		int GetType();
		void SetType(int type);
		QString GetIconName();
		void SetIconName(QString iconName);
		QString GetChnName();
		void SetChnName(QString chnName);

		void SetIsHaveAddButton(bool b);
		bool GetIsHaveAddButton();
		void SetIsHaveDelButton(bool b);
		bool GetIsHaveDelButton();
		void SetIsHaveHideButton(bool b);
		bool GetIsHaveHideButton();
		void SetVisible(bool b);
		bool GetVisible();
		void SetIsEdit(bool b);
		bool GetIsEdit();
		void SetParentModelBase(ModelBase* parentModelBase);
		ModelBase* GetParentModelBase();
		DataProperty::DataBase* GetDataBase();
		void SetDataBase(DataProperty::DataBase* dataBase);
		//获取给定数据列表中，已经使用的中文或英文名字列表.参数：0:中文、1：英文
		QList<QString> GetUsedNameList(int nameType);
		QList<ModelBase*> GetChildList();

		virtual QString getFileName(); //xuxinwei 20200319
		//QString getFileName();
		void setFileName(QString name);

		int GetId();
		void SetId(int id);

		void Copy(ModelBase* data);

		void setTreeType(int type);
		int getTreeType();
		

	protected:
		QTreeWidgetItem* CreateTreeWidgetItem(QTreeWidgetItem *root, QString text, QString iconName);

	protected:
		QList<ModelBase*> _childNodes; //子节点列表
		QString _nameEng; //英文说明
		QString _nameChn; //中文说明
		QString _iconName;//图标名称
		int _id;          //Id序号 
		int _type = 0;    //模型类型
		bool _isHaveAddButton{ false };
		bool _isHaveDelButton{ false };
		bool _isHaveHideButton{ false };
		bool _visible{ true };//是否可见
		bool _isEdit{ false };//是否可以编辑
		DataProperty::DataBase* _dataBase{nullptr};  //模型的参数、属性具体的数据集合
		ModelBase* _parentModelBase{nullptr};        //父节点指针

		QString _fileName;
		int _treeType{ -1 };
	};

}

#endif // MODELBASE_H
