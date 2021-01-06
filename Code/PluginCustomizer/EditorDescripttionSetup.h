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

		//�����б�����ۺ���
		void OnBtnAddParaClicked();
		void OnBtnEditParaClicked();
		void OnBtnDelParaClicked();
		void OnBtnClearAllParaClicked();
		void TableWidgetParaDoubleClicked(QModelIndex modelIndex);
		void TableWidgetParaClicked(QModelIndex modelIndex);
		void TableWidgetParaCurrentCellChanged();


		//����������ۺ���
		void OnBtnAddGroupClicked();
		void OnBtnDelGroupClicked();
		void OnBtnEditGroupClicked();
		void OnBtnClearAllGroupClicked();
		void TableWidgetGoupDoubleClicked(QModelIndex modelIndex);
		void TableWidgetGroupClicked(QModelIndex modelIndex);
		void TableWidgetGroupCurrentCellChanged();

		//������--�����б�����ۺ���
		void OnBtnAddGroupParaClicked();
		void OnBtnEditGroupParaClicked();
		void OnBtnDelGroupParaClicked();
		void OnBtnClearAllGroupParaClicked(); 
		void TableWidgetGroupParaDoubleClicked(QModelIndex modelIndex);
		void TableWidgetGroupParaClicked(QModelIndex modelIndex);
		void TableWidgetGroupParaCurrentCellChanged();

		//��������
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

		//20200326 xuxinwei  ��ȡ��������
		int getCaseType(ModelBase* model);
		void deleteGroupAndParameters(DataProperty::ParameterGroup* group);
		void removeNameFromList(DataProperty::ParameterBase* base);

	private:
		//��ǰ��������ö��˵��
		enum ECurrentOpObject
		{
			ParaList =1, //�����б�
			ParaGroupList //������--�����б�
		};
	private:
		Ui::EditorDescripttionSetup *ui;
		CustomParameterModel* _model{nullptr};           //��ǰ�༭��model
		QTreeWidgetItem* _treeItem{nullptr};
		QMenu *topMenu;
		//DataProperty::ParameterGroup* _currentParameterGroup{ nullptr };                   //��ǰ�����Ĳ�����
		//DataProperty::ParameterBase* _currentParameterList_Model{ nullptr };        //��ǰѡ�в����б��еĲ���
		//DataProperty::ParameterBase* _currentParameterGroupList_Model{ nullptr };   //��ǰѡ�в������еĲ���

		int _paraListColNum{ 4 };//�����б�����
		int _groupListColNum{ 2 };//�������б�����
		ECurrentOpObject _currentOpObject{ ParaList };//��ǰ������ʱ�����б��ǲ�����Ĳ����б�

		//20200326   xuxinwei  �����������в����б�
		QList<DataProperty::ParameterBase*> _parameterList;
		QList<DataProperty::ParameterGroup*> _parameterGroupList;
		int _treeType{};
		int _currentType{ -1 };
	};
}
#endif // EDITORDESCRIPTTIONSETUP_H
