#ifndef EDITORNAMEVALUE_H
#define EDITORNAMEVALUE_H

#include <QDialog>

namespace Ui {
class EditorNameValue;
}

namespace FastCAEDesigner
{
	class MonitorModel;
	class CurveModel;
	class VectorModel;

	class EditorNameValue : public QDialog
	{
		Q_OBJECT

	public:
		explicit EditorNameValue(QWidget *parent = 0);
		EditorNameValue(MonitorModel* model, QWidget *parent = 0);
		EditorNameValue(CurveModel* model, QWidget *parent = 0);
		EditorNameValue(VectorModel* model, QWidget *parent = 0);
		~EditorNameValue();
		QString GetNameString();
		void SetNameString(QString name);
		void SetUsedNameList(QList<QString> usedNameList);

	private slots :
		void OnBtnOkClicked();
		void OnTimeout();
		void OnBtnOkClicked_File();

	private:
		Ui::EditorNameValue *ui;
		QList<QString> _usedNameList;
		QString _name;

		MonitorModel* _monitorModel;
		CurveModel* _curveModel;
		VectorModel* _vectorModel;
	
	};
}
#endif // EDITORNAMEVALUE_H
