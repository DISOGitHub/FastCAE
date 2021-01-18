#ifndef EDITORDOUBLEVALUE_H
#define EDITORDOUBLEVALUE_H

#include <QDialog>
#include <QHash>
#include <QValidator>

namespace Ui {
class EditorDoubleValue;
}

namespace DataProperty
{
	class ParameterDouble;
}

namespace FastCAEDesigner
{
	class EditorDoubleValue : public QDialog
	{
		Q_OBJECT

	public:
		EditorDoubleValue(DataProperty::ParameterDouble* model, QWidget *parent = 0);
		~EditorDoubleValue();
		void SetUsedNameList(QList<QString> list);
		void SetEditModel(bool b);

	private slots:
		void OnBtnOkClicked();
		void OnTimeout();
		//void SetInputValidator(int decimals);
		//void OnSetInputValidator(QString decimals);
		void precisionChanged();

	private:
		void SetInputValidator(int decimals);
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
		Ui::EditorDoubleValue *ui;
		DataProperty::ParameterDouble* _model;
		QHash<int, QString> _errorList{};
		QList<QString> _usedNameList;
		QDoubleValidator *_validator;
		QString _name;
		QString _unit;
		double _val;
		double _min;
		double _max;
		double _range[2];
		int _decimals;
	};
}

#endif // EDITORDOUBLEVALUE_H
