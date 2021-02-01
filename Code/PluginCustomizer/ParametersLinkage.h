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

		//对参数联动界面进行初始化
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
		//将相关参数信息插入TableWidget中
		void insertActiveTableWidget(int row, DataProperty::ParameterBase* model);
		void insertDrivenTableWidget(int row, DataProperty::ParameterBase* model);
		void insertDrivenGroupTableWidget(int row, DataProperty::ParameterGroup* model);

		//20200330  xuxinwei
		//对ui界面的显示进行操作
		void setLinkageVisable(bool b);
		void setParameterTableVisable(bool b);

		//QList<DataProperty::ParameterBase*> getDrivenParameterList();
		//获取配置完主动参数列表后剩下的所有参数
		void getDrivenParameterList();
		void getDrivenParameterGroupList();
		//void refreshTableWidget(int row, int column, QList<DataProperty::ParameterBase*> list);

		QWidget* getGroupNameWidget(int row, DataProperty::ParameterBase* model);
		QWidget* getParameterValue(int row, DataProperty::ParameterBase* model);

		//设置主从动参数列表
		void setActiveList();
		void setDrivenList();
		void setDrivenGroupList();
		//设置主从动参数值，以及可见性
		void setActiveParameterValue(int row,DataProperty::ParameterBase* model);
		void setDrivenParameterValue(int row, DataProperty::ParameterBase* model);
		void setDrivenParameterEditAndVisible(int row, DataProperty::ParameterBase* model);

		//参数联动的分步操作
		void completeNameAndTypeConfiguration();
		void completeActiveConfiguration();
		void completeDrivenConfiguration();
		void completeDrivenGrouopConfiguration();	

		//界面数据和data进行交互
		void updateUiToData();
		void updateDataToUi();

		//对所有参数以及参数组进行备份
		void setAllParameterList();
		void setAllGroupList();
		void copyData(DataProperty::ParameterBase* base);

		//参数选择界面优化
		bool getParameterState(QList<DataProperty::ParameterBase*> list, DataProperty::ParameterBase* data);
		void repalceDataValue(DataProperty::ParameterBase* o, DataProperty::ParameterBase* n);
		bool getParameterGroupState(QList<DataProperty::ParameterGroup*> list, DataProperty::ParameterGroup* data);
		//void repalceDataValue(DataProperty::ParameterGroup* o, DataProperty::ParameterBase* n);

		void clearAllParameterList();

		

	private slots:
		//上一步下一步操作
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
