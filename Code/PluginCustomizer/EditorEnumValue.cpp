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
	
	//��ʼ����������Ӧ�Ĵ�����Ϣ�б�
	void  EditorEnumValue::InitErrorList()
	{
		_errorList.insert(NameIsEmpty, tr("Name is empty."));
		_errorList.insert(TheNameInUse, tr("The name is already in use."));
		_errorList.insert(EnumDataIsEmpty, tr("Value is empty."));

	}

	//У�������趨�Ƿ���ȷ�����ݴ����״��������Ӧ�Ĵ������
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

	//ȷ���趨�ۺ���
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
	//ˢ��model���ݵ�UI
	void EditorEnumValue::UpdateDataToUi()
	{
		ui->txtName->setText(_model->getDescribe());
		_enumValueList = _model->getOption();
		FillComboBox(_enumValueList);
		int index = _model->getCurrentIndex();
		ui->cbo_Value->setCurrentIndex(index);
	}

	//ˢ��Ui���ݵ�model
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

	//��ʱ���ۺ���
	void EditorEnumValue::OnTimeout()
	{
		ui->lbl_info->setText("");
		ui->lbl_info->hide();
	}

	//�����Ѿ�ʹ�õı��������б�
	void EditorEnumValue::SetEnumValueList(QStringList enumValueList)
	{
		_enumValueList = enumValueList;
	}

	void EditorEnumValue::SetUsedNameList(QList<QString> usedNameList)
	{
		_usedNameList = usedNameList;
	}

}