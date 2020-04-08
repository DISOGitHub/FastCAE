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
		
		ParaLinkageManager(QString name, QWidget *parent = 0);
		~ParaLinkageManager();

		void init();
		void initParameterTableWidget();

		void showActiveList(QList<DataProperty::ParameterBase*> list);
		void showDrivenList(QList<DataProperty::ParameterBase*> list);
		void showDrivenGroupList(QList<DataProperty::ParameterGroup*> list);

		void insertActiveDataToTable(int row, DataProperty::ParameterBase* model);
		void insertDrivenDataToTable(int row, DataProperty::ParameterBase* model);
		void insertDrivenGroupDataToTable(int row, DataProperty::ParameterGroup* model);

	private slots:
		void onCreateQPBClicked();
		void onEditQPBClicked();
		void onDeleteQPBClicked();
		void onItemClicked(QListWidgetItem* item);
//		void onItemClicked(int index);


	private:
		QString _caseName{};
		QList<ParametersLinkage*> _parametersLinkageList;
		QList<ParaLinkageData*> _paraLinkageDataList;

		//QMap<QListWidgetItem*, ParaLinkageData*> _paraDataDict;


	private:
		Ui::ParaLinkageManager *ui;
	};
}
#endif // PARALINKAGEMANAGER_H
