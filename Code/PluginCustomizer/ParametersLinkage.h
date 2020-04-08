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
		explicit ParametersLinkage(QWidget *parent = 0);
		//ParametersLinkage(QList<DataProperty::ParameterBase*> parameterList, QWidget *parent = 0);
		ParametersLinkage(ParaLinkageData* linkData, QString tree, QWidget *parent = 0);

		~ParametersLinkage();	

		ParaLinkageData* getParameterLinkageData();

	private:
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
		void insertActiveTableWidget(int row, DataProperty::ParameterBase* model);
		void insertDrivenTableWidget(int row, DataProperty::ParameterBase* model);
		void insertDrivenGroupTableWidget(int row, DataProperty::ParameterGroup* model);

		//20200330  xuxinwei
		void setLinkageVisable(bool b);
		void setParameterTableVisable(bool b);

		//QList<DataProperty::ParameterBase*> getDrivenParameterList();
		void getDrivenParameterList();
		//void refreshTableWidget(int row, int column, QList<DataProperty::ParameterBase*> list);

		QWidget* getGroupNameWidget(int row, DataProperty::ParameterBase* model);
		QWidget* getParameterValue(int row, DataProperty::ParameterBase* model);

		void setActiveList();
		void setDrivenList();
		void setDrivenGroupList();
		void setActiveParameterValue(int row,DataProperty::ParameterBase* model);
		void setDrivenParameterValue(int row, DataProperty::ParameterBase* model);
		void setDrivenParameterEditAndVisible(int row, DataProperty::ParameterBase* model);


		void completeNameAndTypeConfiguration();
		void completeActiveConfiguration();
		void completeDrivenConfiguration();
		void completeDrivenGrouopConfiguration();	

		void updateUiToData();
		void updateDataToUi();

		void setAllParameter_GroupList();
		void copyData(DataProperty::ParameterBase* base);

	private slots:
		void onLastStepQPBClicked();
		void onNextStepQPBClicked();
		//void setParameterList(DataProperty::ParameterBase* model);
		//void setParameterList(int index);
		void setParameterValue(int index);
		void OnTimeout();


	private:
		QString _name;
		QString _caseName;
		QList<DataProperty::ParameterBase*> _activeList;
		QList<DataProperty::ParameterBase*> _drivenList;
		QList<DataProperty::ParameterGroup*> _drivenGroupList;

		QList<DataProperty::ParameterBase*> _allParameterList;
		QList<DataProperty::ParameterBase*> _remainParameterList;
		QList<DataProperty::ParameterGroup*> _allParameterGroupList;

		ParaLinkageData* _paraLinkData;

		//20200330    xuxinwei
		int _page{0};

	private:
		Ui::ParametersLinkage *ui;
	};
}

#endif // PARAMETERSLINKAGE_H
