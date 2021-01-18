#include "EditorDoubleValue.h"
#include "ui_EditorDoubleValue.h"
#include "DataProperty/ParameterDouble.h"
#include <QDoubleValidator>
#include <QTimer>
#include <QDebug>
#include "InputValidator.h"

namespace FastCAEDesigner
{
	EditorDoubleValue::EditorDoubleValue(DataProperty::ParameterDouble* model, QWidget *parent) :
		QDialog(parent),
		ui(new Ui::EditorDoubleValue),
		_model(model)
	{
		ui->setupUi(this);
		Init();
	}

	EditorDoubleValue::~EditorDoubleValue()
	{
		delete ui;
		_errorList.clear();
		_usedNameList.clear();
		delete _validator;
		_validator = nullptr;
	}

	void EditorDoubleValue::SetEditModel(bool b)
	{
		if (b)
			ui->txtName->setEnabled(false);
		else
			ui->txtName->setEnabled(true);
	}

	//初始化错误代码对应的错误信息列表
	void  EditorDoubleValue::InitErrorList()
	{
		_errorList.insert(NameIsEmpty, tr("Name is empty."));
		_errorList.insert(UnitIsEmpty, tr("Unit is empty."));
		_errorList.insert(ValueOutOfRange, tr("Value out of range."));
		_errorList.insert(RangeSetupError, tr("Range setting error."));
		_errorList.insert(TheNameInUse, tr("The name is already in use"));
	}

	//初始化函数
	void EditorDoubleValue::Init()
	{
		UpdateDataToUi();
		_decimals = _model->getAccuracy();
		connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(OnBtnOkClicked()));
		connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
		//connect(ui->spBox_precision, SIGNAL(valueChanged(int)), this, SLOT(SetInputValidator(int)));
		connect(ui->spBox_precision, SIGNAL(valueChanged(int)), this, SLOT(precisionChanged()));
		//connect(ui->spBox_precision, SIGNAL(valueChanged(QString)), this, SLOT(OnSetInputValidator(QString)));
		SetInputValidator(_decimals);//控件数据限制设定
		InitErrorList();
	}
	
	//void EditorDoubleValue::OnSetInputValidator(QString decimals)
	void EditorDoubleValue::precisionChanged()
	{
		int deci = ui->spBox_precision->value();
	//	int deci = decimals.toInt();
		SetInputValidator(deci);
	}
	
// 	void EditorDoubleValue::SetInputValidator(QString decimals)
// 	{
// 		int deci = decimals.toInt();
// 		//SetInputValidator(deci);
// 	}

	void EditorDoubleValue::SetInputValidator(int decimals)
	{
		//double min = -2147483647;DBL_MIN
		//double max = 2147483647;
		//double min = DBL_MIN;
		//double max = DBL_MAX;
		double min = -37777777777;
		double max = 37777777777;

		_validator = new QDoubleValidator(min, max, decimals, this);
		_validator->setNotation(QDoubleValidator::StandardNotation);
		ui->txtValue->setValidator(_validator);
		ui->txtMin->setValidator(_validator);
		ui->txtMax->setValidator(_validator);
	}

	//校验数据设定是否正确，根据错误的状况返回响应的错误代码
	int EditorDoubleValue::IsDataOk()
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
	void EditorDoubleValue::UpdateUiToLocal()
	{
		_name = ui->txtName->text().trimmed();
		_unit = ui->txtUnit->text().trimmed();
		_val = ui->txtValue->text().toDouble();
		_min = ui->txtMin->text().toDouble();
		_max = ui->txtMax->text().toDouble();
		_range[0] = _min;
		_range[1] = _max;
		_decimals = ui->spBox_precision->text().toInt();
	}

	//刷新model数据到UI
	void EditorDoubleValue::UpdateDataToUi()
	{
		ui->txtName->setText(_model->getDescribe());
		ui->txtUnit->setText(_model->getUnit());
		//ui->txtValue->setText(QString::number(_model->getValue()));
		_model->getRange(_range);
		ui->txtMin->setText(QString::number(_range[0]));
		ui->txtMax->setText(QString::number(_range[1]));
		int accuracy = (_model->getAccuracy() > 10) ? 10 : _model->getAccuracy();
		ui->spBox_precision->setValue(accuracy);
		double d = _model->getValue();
		QString s = QString("%1").arg(d, 0, 'f', accuracy);
		ui->txtValue->setText(s);
	}

	//刷新Ui数据到model
	void EditorDoubleValue::UpdateUiToData()
	{
		UpdateUiToLocal();
		_model->setDescribe(_name);
		_model->setUnit(_unit);
		_model->setValue(_val);
		_model->setRange(_range);
		_model->setAccuracy(_decimals);
	}

	//确认设定槽函数
	void EditorDoubleValue::OnBtnOkClicked()
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
	void EditorDoubleValue::OnTimeout()
	{
		ui->lbl_info->setText("");
		ui->lbl_info->hide();
	}

	//设置已经使用的变量名称列表
	void EditorDoubleValue::SetUsedNameList(QList<QString> list)
	{
		_usedNameList = list;
	}

	

}
