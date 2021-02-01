#ifndef EDITORENUMLISTVALUE_H
#define EDITORENUMLISTVALUE_H

#include <QDialog>
#include <QList>


namespace Ui {
class EditorEnumListValue;
}

namespace FastCAEDesigner
{

	class EditorEnumListValue : public QDialog
	{
		Q_OBJECT

	public:
		explicit EditorEnumListValue(QWidget *parent = 0);
		~EditorEnumListValue();
		void SetEnumList(QStringList valList);
		QStringList GetEnumList();

	private slots:
		void OnBtnOkClicked();
		void OnBtnAddClicked();
		void OnBtnInsertClicked();
		void OnBtnDelClicked();
		void OnBtnClearAllClicked();
		//void OnTimeout();

	private:
		void Init();
		QList<QString> GetUsedName();

	private:
		Ui::EditorEnumListValue *ui;
		QStringList _valList;
	};
}
#endif // EDITORENUMLISTVALUE_H
