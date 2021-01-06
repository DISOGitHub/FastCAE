#include "EditorIntValue.h"
#include "ui_EditorIntValue.h"
#include "DataProperty/ParameterInt.h"
#include <QValidator>
#include <QMessageBox>
#include "InputValidator.h"

namespace FastCAEDesigner
{
	EditorIntValue::EditorIntValue(QWidget *parent) :
		QDialog(parent),
		ui(new Ui::EditorIntValue)
	{
		ui->setupUi(this);

		connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(OnBtnOkClicked()));
		connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
	}

	EditorIntValue::EditorIntValue(DataProperty::ParameterInt* model, QWidget *parent):
		QDialog(parent),
		ui(new Ui::EditorIntValue),
		_model(model)
	{
		ui->setupUi(this);
		Init();
		
	}

	EditorIntValue::~EditorIntValue()
	{
		delete ui;
	}

	void EditorIntValue::SetEditModel(bool b)
	{
		if (b)
			ui->txtName->setEnabled(false);
		else
			ui->txtName->setEnabled(true);
	}

	//初始化错误代码对应的错误信息列表
	void  EditorIntValue::InitErrorList()
	{
		_errorList.insert(NameIsEmpty, tr("Name is empty."));
		_errorList.insert(UnitIsEmpty, tr("Unit is empty."));
		_errorList.insert(ValueOutOfRange, tr("Value out of range."));
		_errorList.insert(RangeSetupError, tr("Range setting error."));
		_errorList.insert(TheNameInUse, tr("The name is already in use"));
	}

	//初始化函数
	void EditorIntValue::Init()
	{
		UpdateDataToUi();
		connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(OnBtnOkClicked()));
		connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
		
		//控件数据限制设定
		int min = -2147483647;
		int max = 2147483647;
		QValidator *validator = new QIntValidator(min, max, this);
		ui->txtValue->setValidator(validator);
		ui->txtMin->setValidator(validator);
		ui->txtMax->setValidator(validator);
		InitErrorList();
	}

	//校验数据设定是否正确，根据错误的状况返回响应的错误代码
	int EditorIntValue::IsDataOk()
	{
		UpdateUiToLocal();
		
		if (_usedNameList.contains(_name))
			return TheNameInUse;
		
		if (_name.isEmpty())
			return NameIsEmpty;

		//if (_unit.isEmpty())
		//	return UnitIsEmpty;
		
		if (_min > _max)
			return RangeSetupError;

		if (_val<_min || _val>_max)
			return ValueOutOfRange;


		return 0;
	}

	//刷新Ui数据到本地变量
	void EditorIntValue::UpdateUiToLocal()
	{
		_name = ui->txtName->text().trimmed();
		_unit = ui->txtUnit->text().trimmed();
		_val = ui->txtValue->text().toInt();
		_min = ui->txtMin->text().toInt();
		_max = ui->txtMax->text().toInt();
	}

	//刷新model数据到UI
	void EditorIntValue::UpdateDataToUi()
	{
		ui->txtName->setText(_model->getDescribe());
		ui->txtUnit->setText(_model->getUnit());
		ui->txtValue->setText(QString::number(_model->getValue()));
		int range[2];
		_model->getRange(range);
		ui->txtMin->setText(QString::number(range[0]));
		ui->txtMax->setText(QString::number(range[1]));
	}

	//刷新Ui数据到model
	void EditorIntValue::UpdateUiToData()
	{
		UpdateUiToLocal();
		int range[2]{_min, _max};
		_model->setDescribe(_name);
		_model->setUnit(_unit);
		_model->setValue(_val);
		_model->setRange(range);
	}

	//确认设定槽函数
	void EditorIntValue::OnBtnOkClicked()
	{
		int errorCode = IsDataOk();

		if (0 != errorCode)
		{
			QString errorMsg = _errorList[errorCode];
			ui->lbl_info->setText(errorMsg);
			ui->lbl_info->show();
			QTimer::singleShot(3000, this, SLOT(OnTimeout()));
			return;
		}

		UpdateUiToData();
		this->accept();
		close();
	}

	//定时器槽函数
	void EditorIntValue::OnTimeout()
	{
		ui->lbl_info->setText("");
		ui->lbl_info->hide();
	}

	//设置已经使用的变量名称列表
	void EditorIntValue::SetUsedNameList(QList<QString> list)
	{
		_usedNameList = list;
	}

}