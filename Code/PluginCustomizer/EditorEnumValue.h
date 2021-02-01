#ifndef EDITORENUMVALUE_H
#define EDITORENUMVALUE_H

#include <QDialog>
#include <QStringList>

namespace Ui {
class EditorEnumValue;
}

namespace DataProperty
{
	class  ParameterSelectable;
}

namespace FastCAEDesigner
{
	class EditorEnumValue : public QDialog
	{
		Q_OBJECT

	public:
		EditorEnumValue(DataProperty::ParameterSelectable* model, QWidget *parent = 0);
		~EditorEnumValue();
		void SetEnumValueList(QStringList enumValueList);
		void SetUsedNameList(QList<QString> usedNameList);

	private slots:
		void OnBtnOkClicked();
		void OnBtnEditClicked();
		void OnTimeout();

	private:
		int IsDataOk();
		void Init();
		void InitErrorList();
		void FillComboBox(QStringList list);
		void UpdateDataToUi();
		void UpdateUiToData();

	private:

		enum InputErrorCode
		{
			NameIsEmpty = 1,
			TheNameInUse,
			EnumDataIsEmpty,
		};

	private:
		Ui::EditorEnumValue *ui;
		DataProperty::ParameterSelectable* _model;
		QHash<int, QString> _errorList{};
		QList<QString> _usedNameList;
		QString _name;
		int _currentIndex;
		QStringList _enumValueList;
	};
}

#endif // EDITORENUMVALUE_H
