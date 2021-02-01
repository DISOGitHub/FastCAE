#ifndef EDITORINTVALUE_H
#define EDITORINTVALUE_H

#include <QDialog>
#include <QHash>
#include <QTimer>


namespace Ui {
class EditorIntValue;
}

namespace DataProperty
{
	class ParameterInt;
}


namespace FastCAEDesigner
{
	class EditorIntValue : public QDialog
	{
		Q_OBJECT

	public:
		EditorIntValue(QWidget *parent = 0);
		EditorIntValue(DataProperty::ParameterInt* model,QWidget *parent = 0);
		~EditorIntValue();
		void SetUsedNameList(QList<QString> list);
		void SetEditModel(bool b);
		

	private slots:
		void OnBtnOkClicked();
		void OnTimeout();

	private:
		void UpdateDataToUi();
		void UpdateUiToData();
		void Init();
		void InitErrorList();
		int IsDataOk();
		void UpdateUiToLocal();

	private:
		
		enum InputErrorCode
		{
			NameIsEmpty = 1,
			UnitIsEmpty,
			ValueOutOfRange,
			RangeSetupError,
			TheNameInUse,
		};

	private:
		Ui::EditorIntValue *ui;
		DataProperty::ParameterInt* _model;
		QHash<int, QString> _errorList{};
		QList<QString> _usedNameList;

		QString _name;
		QString _unit;
		int _val;
		int _min;
		int _max;

	};
}
#endif // EDITORINTVALUE_H
