#ifndef EDITORTABLEVALUE_H
#define EDITORTABLEVALUE_H

#include <QDialog>
#include <QTableWidgetItem>

namespace Ui {
class EditorTableValue;
}
namespace DataProperty{
	class ParameterTable;
}

namespace FastCAEDesigner{
	class EditorTableValue : public QDialog
	{
		Q_OBJECT

	public:
		explicit EditorTableValue(QWidget *parent = 0);
		EditorTableValue(DataProperty::ParameterTable*,QWidget *parent = 0);
		~EditorTableValue();

		void InitErrorList();
		int IsNameSure();
		int IsTableRowsOrColumnsZero();
		int IsDataOk();//Added xvdongming 2019-12-24
		bool IsTableDataSure(int rows, int columns);

		void Init();
		void UpdateDataToUi();
		void UpdateUiToData(); 
		void setNameUsedList(QList<QString> list);
		void InsertDataToTable(int, int);
		void InsertDataToTable(int, int, QStringList);
		void InsertDataToModel(int, int);
		void InitTableData(int, int);
		bool IsDataSure(int,int);
		
	private slots:
		void OnOkPBtnClicked();
		void OnTimeout();
		void OnRefreshPBtnClicked();
		void OnModifyHeader(int);

	private:
		QList<QString> _usedNameList;
		QHash<int, QString> _errorList{};
		DataProperty::ParameterTable* _model;
		enum InputErrorCode
		{
			NameIsEmpty = 1,
			TheNameInUse,
			RowsIsZero,
			ColumnsIsZero,
			IllegalName,
			TableDataError,
			TableNotInit,
		};

	private:
		Ui::EditorTableValue *ui;
	};
}
#endif // EDITORTABLEVALUE_H
