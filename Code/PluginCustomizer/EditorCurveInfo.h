#ifndef EDITORCURVEINFO_H
#define EDITORCURVEINFO_H

#include <QDialog>

namespace Ui {
class EditorCurveInfo;
}

namespace FastCAEDesigner{
	class EditorCurveInfo : public QDialog
	{
		Q_OBJECT

	public:
		explicit EditorCurveInfo(QWidget *parent = 0);
		EditorCurveInfo(QString curveName, QString XVariable, QString YVariable, QWidget *parent = 0);
		~EditorCurveInfo();
	
	//	void init();

	//	void setCurveName(QString);
	//	void setXVariable(QString);
	//	void setYVariable(QString);
		QString getCurveName();
		QString getXVariable();
		QString getYVariable();
		void InitCurveInfo(QString,QString,QString);
		void InitErrorList();
		void setNameUsedList(QList<QString> list);
		int IsNameSure();
		
	private slots:
		void OnOKPBClicked();
	//	void OnCancelPBClicked();
		void OnTimeout();
		

	private:
		QString _CurveName;
		QString _XVariable;
		QString _YVariable;
		QList<QString> _nameUsedList;
		QHash<int, QString> _errorList{};
		enum ErrorInfo{
			CurveNameIsEmpty = 1,
			CurveNameInUse,
			XVariableIsEmpty,
			YVariableIsEmpty,
			IllegalName,
		};

	private:
		Ui::EditorCurveInfo *ui;
	};
}
#endif // EDITORCURVEINFO_H
