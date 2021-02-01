#ifndef EDITORPATHVALUE_H
#define EDITORPATHVALUE_H

#include <QDialog>

namespace Ui {
class EditorPathValue;
}

namespace DataProperty{
	class ParameterPath;

}
namespace FastCAEDesigner{
	class EditorPathValue : public QDialog
	{
		Q_OBJECT

	public:
		explicit EditorPathValue(QWidget *parent = 0);
		EditorPathValue(DataProperty::ParameterPath*, QWidget *parent = 0);
		~EditorPathValue();

		void Init();
		void UpdateUiDisplay(bool);
		void InitErrorList();
		void UpdateDataToUi();
		void UpdateUiToData();
		void setNameUsedList(QList<QString> list);
		int IsNameSure();
		int IsFileSuffixSure();
		int IsFileSuffixSure(QString suffix);

		void setFileSuffixEnable(bool enable);

	private slots:
		void OnComboxChanged(int);
		void OnOkPBtnClicked();
		void OnPathPBtnClicked();
		void OnTimeout();

	private:
		DataProperty::ParameterPath* _model;
		QList<QString> _usedNameList;
		QHash<int, QString> _errorList{};
		enum InputErrorCode
		{
			NameIsEmpty = 1,
			TheNameInUse,
			FileSuffixIsEmpty,
			FileSuffixIsNotSure,
			IllegalName,
		};

	private:
		Ui::EditorPathValue *_ui;
	};
}
#endif // EDITORPATHVALUE_H
