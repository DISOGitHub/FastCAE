#include "EditorStringValue.h"
#include "ui_EditorStringValue.h"
#include "DataProperty/ParameterString.h"
#include "InputValidator.h"
#include <QTimer>

namespace FastCAEDesigner{
	EditorStringValue::EditorStringValue(QWidget *parent) :
		QDialog(parent),
		ui(new Ui::EditorStringValue)
	{
		ui->setupUi(this);
	}

	EditorStringValue::EditorStringValue(DataProperty::ParameterString* model, QWidget *parent) :
		QDialog(parent),
		ui(new Ui::EditorStringValue),
		_model(model)
	{
		ui->setupUi(this);
		Init();
	}

	EditorStringValue::~EditorStringValue()
	{
		delete ui;
		_nameUsedList.clear();
	}

	void EditorStringValue::Init()
	{
		UpdateDataToUi();
		InitErrorList();
		connect(ui->OkPBtn, SIGNAL(clicked()), this, SLOT(OnOKPBtnClicked()));
		connect(ui->CancelPBtn, SIGNAL(clicked()), this, SLOT(close()));
	}
	void EditorStringValue::InitErrorList()
	{
		_errorList.insert(NameIsEmpty, tr("Name is empty."));
		_errorList.insert(TheNameInUse, tr("The name is already in use."));
		_errorList.insert(NameIllegal, tr("The name is illegal string."));
	}
	void EditorStringValue::UpdateDataToUi()
	{
		ui->NameLE->setText(_model->getDescribe());
		ui->ValueLE->setText(_model->getValue());
	}
	void EditorStringValue::UpdateUiToData()
	{
		_model->setDescribe(ui->NameLE->text().trimmed());
		_model->setValue(ui->ValueLE->text().trimmed());
	}

	void EditorStringValue::SetUsedNameList(QList<QString> list)
	{
		_nameUsedList = list;
	}
	void EditorStringValue::OnOKPBtnClicked()
	{
		int error = NameIsSure();
		if (error != 0)
		{
			QString errorMsg = _errorList[error];
			ui->ErrorText->setText(errorMsg);
			ui->ErrorText->show();
			QTimer::singleShot(3000, this, SLOT(OnTimeout()));
			return;
		}

		UpdateUiToData();
		this->accept();
		close();
	}
	int EditorStringValue::NameIsSure()
	{
		QString name = ui->NameLE->text().trimmed();

		if (_nameUsedList.contains(name))
			return TheNameInUse;

		if (name.isEmpty())
			return NameIsEmpty;

		if (InputValidator::getInstance()->FileNameIsAllow(name) == false)
			return NameIllegal;

		return 0;
	}
	void EditorStringValue::OnTimeout()
	{
		ui->ErrorText->setText("");
		ui->ErrorText->hide();
	}

}