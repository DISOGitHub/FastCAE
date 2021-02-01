#ifndef EDITORDESCRIPTTIONSETUP_H
#define EDITORDESCRIPTTIONSETUP_H

#include <QDialog>
#include <QTreeWidgetItem>
#include <QWidget>
#include <QMenu>
#include <QResizeEvent>
#include <QModelIndex>
#include <QTableWidget>

namespace Ui {
class EditorDescripttionSetup;
}

namespace FastCAEDesigner
{
	class ModelBase;
	class CustomParameterModel;
}

namespace DataProperty
{
	class ParameterBase;
	class ParameterList;
	class ParameterGroup;
	class DataBase;
}

namespace FastCAEDesigner
{
	class EditorDescripttionSetup : public QDialog
	{
		Q_OBJECT

	public:
		EditorDescripttionSetup(QTreeWidgetItem* treeItem, CustomParameterModel* model, QWidget *parent = 0);
		~EditorDescripttionSetup();
		void resizeEvent(QResizeEvent *e);
		virtual void showEvent(QShowEvent *e);

	signals:
		void dispalyParameterLinkageManager();

	private slots:
		void OnBtnOkClicked();
		void OnBtnLoadIconClicked();

		//参数列表操作槽函数
		void OnBtnAddParaClicked();
		void OnBtnEditParaClicked();
		void OnBtnDelParaClicked();
		void OnBtnClearAllParaClicked();
		void TableWidgetParaDoubleClicked(QModelIndex modelIndex);
		void TableWidgetParaClicked(QModelIndex modelIndex);
		void TableWidgetParaCurrentCellChanged();


		//参数组操作槽函数
		void OnBtnAddGroupClicked();
		void OnBtnDelGroupClicked();
		void OnBtnEditGroupClicked();
		void OnBtnClearAllGroupClicked();
		void TableWidgetGoupDoubleClicked(QModelIndex modelIndex);
		void TableWidgetGroupClicked(QModelIndex modelIndex);
		void TableWidgetGroupCurrentCellChanged();

		//参数组--参数列表操作槽函数
		void OnBtnAddGroupParaClicked();
		void OnBtnEditGroupParaClicked();
		void OnBtnDelGroupParaClicked();
		void OnBtnClearAllGroupParaClicked(); 
		void TableWidgetGroupParaDoubleClicked(QModelIndex modelIndex);
		void TableWidgetGroupParaClicked(QModelIndex modelIndex);
		void TableWidgetGroupParaCurrentCellChanged();

		//建立参数
		void OnCreateIntClicked();
		void OnCreateBoolClicked();
		void OnCreateDoubleClicked();
		void OnCreateStringClicked();
		void OnCreateEnumClicked();
		void OnCreateTableClicked();
		void OnCreatePathClicked();
	
		void OnTimeout();
		void OnBtnShowGroupClicked();

		//xuxinwei
		void OnParameterLinkagePBtnClicked();

	private:
		void Init();
		void UpdateDataToUi();
		void UpdateUiToData();
		void SetIsEdit(bool b);
		void CreateAddParameterMenu();
		void EditModel(DataProperty::ParameterBase* model);

		void FillParameterList();
		void FillGroupList();
		void FillParameterList(DataProperty::ParameterList* parameterList, QTableWidget* tableWidget);
		void FillParameterList(QTableWidget* tableWidget);

		QStringList GetModelString(DataProperty::ParameterBase* parameter);
		void InsertModelInParameterList(DataProperty::ParameterBase* model);
		QList<QString> GetParameterNameList();
		QList<QString> GetParameterNameList(QList<DataProperty::ParameterBase*> parameterList);
		QList<QString> GetParameterGroupNameList();
		void ResizeTableWidget(QTableWidget* widget, int colNum);

		DataProperty::ParameterBase* GetCurrentSelecctedParameter();
		void DeleteParameter(DataProperty::ParameterBase* model);
		DataProperty::ParameterGroup* GetCurrentSelecctedGroup();
		DataProperty::ParameterBase* GetSelectedGroupParameter();

		//20200326 xuxinwei  获取算例名称
		int getCaseType(ModelBase* model);
		void deleteGroupAndParameters(DataProperty::ParameterGroup* group);
		void removeNameFromList(DataProperty::ParameterBase* base);

	private:
		//当前操作对象枚举说明
		enum ECurrentOpObject
		{
			ParaList =1, //参数列表
			ParaGroupList //参数组--参数列表
		};
	private:
		Ui::EditorDescripttionSetup *ui;
		CustomParameterModel* _model{nullptr};           //当前编辑的model
		QTreeWidgetItem* _treeItem{nullptr};
		QMenu *topMenu;
		//DataProperty::ParameterGroup* _currentParameterGroup{ nullptr };                   //当前操作的参数组
		//DataProperty::ParameterBase* _currentParameterList_Model{ nullptr };        //当前选中参数列表中的参数
		//DataProperty::ParameterBase* _currentParameterGroupList_Model{ nullptr };   //当前选中参数组中的参数

		int _paraListColNum{ 4 };//参数列表列数
		int _groupListColNum{ 2 };//参数组列表列数
		ECurrentOpObject _currentOpObject{ ParaList };//当前操作的时参数列表还是参数组的参数列表

		//20200326   xuxinwei  仿真和求解所有参数列表
		QList<DataProperty::ParameterBase*> _parameterList;
		QList<DataProperty::ParameterGroup*> _parameterGroupList;
		int _treeType{};
		int _currentType{ -1 };
	};
}
#endif // EDITORDESCRIPTTIONSETUP_H
