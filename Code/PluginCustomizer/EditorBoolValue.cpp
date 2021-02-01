#include "EditorBoolValue.h"
#include "ui_EditorBoolValue.h"
#include "DataProperty/ParameterBool.h"
#include <QTimer>
#include "InputValidator.h"

namespace FastCAEDesigner
{
	EditorBoolValue::EditorBoolValue(DataProperty::ParameterBool* model, QWidget *parent) :
		QDialog(parent),
		ui(new Ui::EditorBoolValue),
		_model(model)
	{
		ui->setupUi(this);
		Init();

	}

	EditorBoolValue::~EditorBoolValue()
	{
		delete ui;
		_usedNameList.clear();
	}

	void EditorBoolValue::SetEditModel(bool b)
	{
		if (b)
			ui->txtName->setEnabled(false);
		else
			ui->txtName->setEnabled(true);
	}

	//初始化函数
	void EditorBoolValue::Init()
	{
		QStringList strList;
		strList << tr("true") << tr("false");
		ui->cbo_value->addItems(strList);
		UpdateDataToUi();
		InitErrorList();
		connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(OnBtnOkClicked()));
		connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
	}

	//初始化错误代码对应的错误信息列表
	void  EditorBoolValue::InitErrorList()
	{
		_errorList.insert(NameIsEmpty, tr("Name is empty."));
		_errorList.insert(TheNameInUse, tr("The name is already in use"));
	}
	
	//校验数据设定是否正确，根据错误的状况返回响应的错误代码
	int EditorBoolValue::IsDataOk()
	{
		QString name = ui->txtName->text().trimmed();
		
		if (_usedNameList.contains(name))
			return TheNameInUse;

		if (name.isEmpty())
			return NameIsEmpty;

		return 0;
	}
	//确认设定槽函数
	void EditorBoolValue::OnBtnOkClicked()
	{
		//int errorCode = IsDataOk();
		QString name = ui->txtName->text().trimmed();
		QString msg = "";
		int errorCode = InputValidator::getInstance()->ValidateName(name, _usedNameList, msg);

		if (0 != errorCode)
		{
			//QString errorMsg = _errorList[errorCode];
			QString errorMsg = msg;
			ui->lbl_info->setText(errorMsg);
			ui->lbl_info->show();
			QTimer::singleShot(3000, this, SLOT(OnTimeout()));
			return;
		}

		UpdateUiToData();
		this->accept();
		close();
	}
	//刷新model数据到UI
	void EditorBoolValue::UpdateDataToUi()
	{
		ui->txtName->setText(_model->getDescribe());

		if (_model->getValue())
			ui->cbo_value->setCurrentIndex(0);
		else
			ui->cbo_value->setCurrentIndex(1);
	}

	//刷新Ui数据到model
	void EditorBoolValue::UpdateUiToData()
	{
		QString name = ui->txtName->text().trimmed();
		_model->setDescribe(name);

		if (0 == ui->cbo_value->currentIndex())
			_model->setValue(true);
		else
			_model->setValue(false);
	}

	//定时器槽函数
	void EditorBoolValue::OnTimeout()
	{
		ui->lbl_info->setText("");
		ui->lbl_info->hide();
	}

	//设置已经使用的变量名称列表
	void EditorBoolValue::SetUsedNameList(QList<QString> list)
	{
		_usedNameList = list;
	}
}