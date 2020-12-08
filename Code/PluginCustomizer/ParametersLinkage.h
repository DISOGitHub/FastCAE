#ifndef PARAMETERSLINKAGE_H
#define PARAMETERSLINKAGE_H

#include <QDialog>

namespace Ui {
class ParametersLinkage;
}

namespace DataProperty{
	class ParameterBase;
	class ParameterGroup;
}


namespace FastCAEDesigner{
	class ParaLinkageData;

	class ParametersLinkage : public QDialog
	{
		Q_OBJECT

	public:
		ParametersLinkage(ParaLinkageData* linkData, int type, QWidget *parent = 0);

		~ParametersLinkage();	

		ParaLinkageData* getParameterLinkageData();

		void setUsedNameList(QList<QString> list);

	private:

		//�Բ�������������г�ʼ��
		void init();
		//void initTreeComboBox();
		void initActiveTableWidget();
	//	void updateActiveTableWidget();
		void initDrivenTableWidget();
		//void updateDrivenTableWidget();
		void initDrivenGroupTableWidget();
		//void updateDrivenGroupTableWidget();

		//20200326 xuxinwei
		//void createTableWidget(int row, int column);
		//����ز�����Ϣ����TableWidget��
		void insertActiveTableWidget(int row, DataProperty::ParameterBase* model);
		void insertDrivenTableWidget(int row, DataProperty::ParameterBase* model);
		void insertDrivenGroupTableWidget(int row, DataProperty::ParameterGroup* model);

		//20200330  xuxinwei
		//��ui�������ʾ���в���
		void setLinkageVisable(bool b);
		void setParameterTableVisable(bool b);

		//QList<DataProperty::ParameterBase*> getDrivenParameterList();
		//��ȡ���������������б��ʣ�µ����в���
		void getDrivenParameterList();
		void getDrivenParameterGroupList();
		//void refreshTableWidget(int row, int column, QList<DataProperty::ParameterBase*> list);

		QWidget* getGroupNameWidget(int row, DataProperty::ParameterBase* model);
		QWidget* getParameterValue(int row, DataProperty::ParameterBase* model);

		//�������Ӷ������б�
		void setActiveList();
		void setDrivenList();
		void setDrivenGroupList();
		//�������Ӷ�����ֵ���Լ��ɼ���
		void setActiveParameterValue(int row,DataProperty::ParameterBase* model);
		void setDrivenParameterValue(int row, DataProperty::ParameterBase* model);
		void setDrivenParameterEditAndVisible(int row, DataProperty::ParameterBase* model);

		//���������ķֲ�����
		void completeNameAndTypeConfiguration();
		void completeActiveConfiguration();
		void completeDrivenConfiguration();
		void completeDrivenGrouopConfiguration();	

		//�������ݺ�data���н���
		void updateUiToData();
		void updateDataToUi();

		//�����в����Լ���������б���
		void setAllParameterList();
		void setAllGroupList();
		void copyData(DataProperty::ParameterBase* base);

		//����ѡ������Ż�
		bool getParameterState(QList<DataProperty::ParameterBase*> list, DataProperty::ParameterBase* data);
		void repalceDataValue(DataProperty::ParameterBase* o, DataProperty::ParameterBase* n);
		bool getParameterGroupState(QList<DataProperty::ParameterGroup*> list, DataProperty::ParameterGroup* data);
		//void repalceDataValue(DataProperty::ParameterGroup* o, DataProperty::ParameterBase* n);

		void clearAllParameterList();

		

	private slots:
		//��һ����һ������
		void onLastStepQPBClicked();
		void onNextStepQPBClicked();
		//void setParameterList(DataProperty::ParameterBase* model);
		//void setParameterList(int index);
		void setParameterValue(int index);

		void OnTimeout();


	private:
		QString _name;
		int _type;
		QList<DataProperty::ParameterBase*> _activeList;
		QList<DataProperty::ParameterBase*> _drivenList;
		QList<DataProperty::ParameterGroup*> _drivenGroupList;

		QList<DataProperty::ParameterBase*> _allParameterList;
		QList<DataProperty::ParameterBase*> _remainParameterList;
		QList<DataProperty::ParameterGroup*> _allParameterGroupList;

		ParaLinkageData* _paraLinkData;

		//20200330    xuxinwei
		int _page{0};
		QList<QString> _usedNameList{};

	private:
		Ui::ParametersLinkage *ui;
	};
}

#endif // PARAMETERSLINKAGE_H
