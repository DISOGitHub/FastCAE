#ifndef EDITORBOOLVALUE_H
#define EDITORBOOLVALUE_H

#include <QDialog>
#include <QHash>


namespace Ui {
class EditorBoolValue;
}

namespace DataProperty
{
	class ParameterBool;
}

namespace FastCAEDesigner
{
	class EditorBoolValue : public QDialog
	{
		Q_OBJECT

	public:
		EditorBoolValue(DataProperty::ParameterBool* model, QWidget *parent = 0);
		~EditorBoolValue();
		void SetUsedNameList(QList<QString> list);
		void SetEditModel(bool b);

	private slots:
		void OnBtnOkClicked();
		void OnTimeout();

	private:
		void Init();
		void InitErrorList();
		int IsDataOk();
		void UpdateDataToUi();
		void UpdateUiToData();

	private:
		enum InputErrorCode
		{
			NameIsEmpty = 1,
			TheNameInUse,
		};

	private:
		Ui::EditorBoolValue *ui;
		DataProperty::ParameterBool* _model;
		QHash<int, QString> _errorList{};
		QList<QString> _usedNameList;
	};
}

#endif // EDITORBOOLVALUE_H
