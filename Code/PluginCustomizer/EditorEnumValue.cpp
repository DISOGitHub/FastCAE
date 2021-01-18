#include "EditorEnumValue.h"
#include "ui_EditorEnumValue.h"
#include "DataProperty/ParameterSelectable.h"
#include <QTimer>
#include "EditorEnumListValue.h"
#include "InputValidator.h"

namespace FastCAEDesigner
{
	EditorEnumValue::EditorEnumValue(DataProperty::ParameterSelectable* model, QWidget *parent) :
		QDialog(parent),
		ui(new Ui::EditorEnumValue),
		_model(model)
	{
		ui->setupUi(this);
		Init();
	}
		
	EditorEnumValue::~EditorEnumValue()
	{
		delete ui;
	}

	void EditorEnumValue::Init()
	{
		InitErrorList();
		UpdateDataToUi();

		connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(OnBtnOkClicked()));
		connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
		connect(ui->btnEdit, SIGNAL(clicked()), this, SLOT(OnBtnEditClicked()));
	}
	
	//初始化错误代码对应的错误信息列表
	void  EditorEnumValue::InitErrorList()
	{
		_errorList.insert(NameIsEmpty, tr("Name is empty."));
		_errorList.insert(TheNameInUse, tr("The name is already in use."));
		_errorList.insert(EnumDataIsEmpty, tr("Value is empty."));

	}

	//校验数据设定是否正确，根据错误的状况返回响应的错误代码
	int EditorEnumValue::IsDataOk()
	{
		_name = ui->txtName->text().trimmed();

		if (_usedNameList.contains(_name))
			return TheNameInUse;

		if (_name.isEmpty())
			return NameIsEmpty;

		if (ui->cbo_Value->currentIndex() < 0)
			return EnumDataIsEmpty;

		return 0;
	}

	//确认设定槽函数
	void EditorEnumValue::OnBtnOkClicked()
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
	
	void EditorEnumValue::OnBtnEditClicked()
	{
		EditorEnumListValue dlg(this);
		dlg.SetEnumList(_enumValueList);

		int r = dlg.exec();
		
		if (r == QDialog::Accepted)
		{
			QStringList valueList = dlg.GetEnumList();
			_enumValueList = valueList;
			FillComboBox(_enumValueList);
		}
	}
	//刷新model数据到UI
	void EditorEnumValue::UpdateDataToUi()
	{
		ui->txtName->setText(_model->getDescribe());
		_enumValueList = _model->getOption();
		FillComboBox(_enumValueList);
		int index = _model->getCurrentIndex();
		ui->cbo_Value->setCurrentIndex(index);
	}

	//刷新Ui数据到model
	void EditorEnumValue::UpdateUiToData()
	{
		_name = ui->txtName->text().trimmed();
		_currentIndex = ui->cbo_Value->currentIndex();
		_model->setDescribe(_name);
		_model->setCurrentIndex(_currentIndex);
		_model->setOption(_enumValueList);
	}

	void EditorEnumValue::FillComboBox(QStringList list)
	{
		ui->cbo_Value->clear();
		ui->cbo_Value->insertItems(0, list);
		ui->cbo_Value->setCurrentIndex(0);
	}

	//定时器槽函数
	void EditorEnumValue::OnTimeout()
	{
		ui->lbl_info->setText("");
		ui->lbl_info->hide();
	}

	//设置已经使用的变量名称列表
	void EditorEnumValue::SetEnumValueList(QStringList enumValueList)
	{
		_enumValueList = enumValueList;
	}

	void EditorEnumValue::SetUsedNameList(QList<QString> usedNameList)
	{
		_usedNameList = usedNameList;
	}

}