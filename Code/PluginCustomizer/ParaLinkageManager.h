#ifndef PARALINKAGEMANAGER_H
#define PARALINKAGEMANAGER_H

#include <QDialog>
#include <QMap>

class QListWidgetItem;

namespace DataProperty{
	class ParameterBase;
	class ParameterGroup;
}


namespace Ui {
class ParaLinkageManager;
}

namespace FastCAEDesigner{
	class ParametersLinkage;
	class ParaLinkageData;
	class ParaManagerData;

	class ParaLinkageManager : public QDialog
	{
		Q_OBJECT

	public:
		
		ParaLinkageManager(int type, QWidget *parent = 0);
		~ParaLinkageManager();

	private:
		//更新参数预览界面
		void init();
		void initParameterTableWidget();
		void initTableWidgetTitle();
		//显示相关参数信息
		void refreshParameterLinkage(ParaLinkageData* data);
		void showActiveList(QList<DataProperty::ParameterBase*> list);
		void showDrivenList(QList<DataProperty::ParameterBase*> list);
		void showDrivenGroupList(QList<DataProperty::ParameterGroup*> list);

		void insertActiveDataToTable(int row, DataProperty::ParameterBase* model);
		void insertDrivenDataToTable(int row, DataProperty::ParameterBase* model);
		void insertDrivenGroupDataToTable(int row, DataProperty::ParameterGroup* model);

		void clearTableWidget();

	private slots:
		void onCreateQPBClicked();
		void onEditQPBClicked();
		void onDeleteQPBClicked();
		void onItemClicked(QListWidgetItem* item);
//		void onItemClicked(int index);


	private:
		int _type{ -1 };
		QList<ParametersLinkage*> _parametersLinkageList;
		QList<ParaLinkageData*> _paraLinkageDataList;
		QList<QString> _usedNameList{};

		//QMap<QListWidgetItem*, ParaLinkageData*> _paraDataDict;


	private:
		Ui::ParaLinkageManager *ui;
	};
}
#endif // PARALINKAGEMANAGER_H
