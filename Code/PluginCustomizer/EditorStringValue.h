#ifndef EDITORSTRINGVALUE_H
#define EDITORSTRINGVALUE_H

#include <QDialog>

namespace Ui {
class EditorStringValue;
}

namespace DataProperty
{
	class ParameterString;
}
namespace FastCAEDesigner{
	class EditorStringValue : public QDialog
	{
		Q_OBJECT

	public:
		explicit EditorStringValue(QWidget *parent = 0);
		EditorStringValue(DataProperty::ParameterString*, QWidget *parent = 0);
		~EditorStringValue();

		void Init();
		void UpdateDataToUi();
		void InitErrorList();
		void UpdateUiToData();
		void SetUsedNameList(QList<QString> list);
		int NameIsSure();

	private slots:
		void OnOKPBtnClicked();
		void OnTimeout();

	private:
		DataProperty::ParameterString* _model;
		QList<QString> _nameUsedList;
		enum InputErrorCode
		{
			NameIsEmpty = 1,
			TheNameInUse,
			NameIllegal,
		};
		QHash<int, QString> _errorList{};

	private:
		Ui::EditorStringValue *ui;
	};
}
#endif // EDITORSTRINGVALUE_H
