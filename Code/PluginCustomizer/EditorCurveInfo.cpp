#include "EditorCurveInfo.h"
#include "ui_EditorCurveInfo.h"
#include "InputValidator.h"
#include <QTimer>

namespace FastCAEDesigner{
	EditorCurveInfo::EditorCurveInfo(QWidget *parent) :
		QDialog(parent),
		ui(new Ui::EditorCurveInfo)
	{
		ui->setupUi(this);
		InitErrorList();
		connect(ui->OkPB, SIGNAL(clicked()), this, SLOT(OnOKPBClicked()));
		connect(ui->CancelPB, SIGNAL(clicked()), this, SLOT(close()));
	}

	EditorCurveInfo::EditorCurveInfo(QString curveName, QString XVariable, QString YVariable, QWidget *parent) :
		QDialog(parent),
		ui(new Ui::EditorCurveInfo),
		_YVariable(YVariable),
		_XVariable(XVariable),
		_CurveName(curveName)
	{
		ui->setupUi(this);
		InitErrorList();
		InitCurveInfo(_CurveName, _XVariable, _YVariable);
		connect(ui->OkPB, SIGNAL(clicked()), this, SLOT(OnOKPBClicked()));
		connect(ui->CancelPB, SIGNAL(clicked()), this, SLOT(close()));
	}

	EditorCurveInfo::~EditorCurveInfo()
	{
		delete ui;
		_nameUsedList.clear();
	}

	void EditorCurveInfo::OnOKPBClicked()
	{
		_CurveName = ui->CurveNameLE->text().trimmed();
		_XVariable = ui->XVariableLE->text().trimmed();
		_YVariable = ui->YVariableLE->text().trimmed();

		int error = IsNameSure();

		if (error != 0)
		{
			QString errorMsg = _errorList[error];
			ui->ErrorText->setText(errorMsg);
			ui->ErrorText->show();
			QTimer::singleShot(3000, this, SLOT(OnTimeout()));
			return;
		}

		this->accept();
		close();
	}

	QString EditorCurveInfo::getCurveName()
	{
		return _CurveName;
	}
	QString EditorCurveInfo::getXVariable()
	{
		return _XVariable;
	}
	QString EditorCurveInfo::getYVariable()
	{
		return _YVariable;
	}

	void EditorCurveInfo::InitCurveInfo(QString curvename, QString xvariable, QString yvariable)
	{
		ui->CurveNameLE->setText(curvename);
		ui->XVariableLE->setText(xvariable);
		ui->YVariableLE->setText(yvariable);
	}
	void EditorCurveInfo::InitErrorList()
	{
		_errorList.insert(CurveNameIsEmpty, tr("Curve name is empty."));
		_errorList.insert(CurveNameInUse, tr("Curve name is already in used."));
		_errorList.insert(XVariableIsEmpty, tr("X variable is empty."));
		_errorList.insert(YVariableIsEmpty, tr("Y variable is empty."));
		_errorList.insert(IllegalName, tr("The name is illegal string."));
	}

	void EditorCurveInfo::setNameUsedList(QList<QString> list)
	{
		_nameUsedList = list;
	}

	void EditorCurveInfo::OnTimeout()
	{
		ui->ErrorText->setText("");
		ui->ErrorText->hide();
	}

	int EditorCurveInfo::IsNameSure()
	{
		if (_nameUsedList.contains(_CurveName))
			return CurveNameInUse;

		if (_CurveName.isEmpty())
			return CurveNameIsEmpty;

		if (_XVariable.isEmpty())
			return XVariableIsEmpty;

		if (_YVariable.isEmpty())
			return YVariableIsEmpty;

		if (InputValidator::getInstance()->FileNameIsAllow(_CurveName) == false)
			return IllegalName;

		return 0;
	}
}