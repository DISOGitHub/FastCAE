#include "Editor3DFileValue.h"
#include "ui_Editor3DFileValue.h"
#include "InputValidator.h"
#include <QTimer>
#include <QButtonGroup>

namespace FastCAEDesigner{
	Editor3DFileValue::Editor3DFileValue(QWidget *parent) :
		QDialog(parent),
		ui(new Ui::Editor3DFileValue)
	{
		ui->setupUi(this);
		Init();
	}

	Editor3DFileValue::Editor3DFileValue(QString name, QString type, QString dataType, QWidget *parent) :
		QDialog(parent),
		ui(new Ui::Editor3DFileValue),
		_dataType(dataType),
		_type(type),
		_name(name)
	{
		ui->setupUi(this);
		
		Init();
	}

	Editor3DFileValue::~Editor3DFileValue()
	{
		delete ui;
	}

	void Editor3DFileValue::Init()
	{
		QButtonGroup* typeGroup = new QButtonGroup(this);
		QButtonGroup* dataTypeGroup = new QButtonGroup(this);

		typeGroup->addButton(ui->ScalarRB,1);
		typeGroup->addButton(ui->VectorRB,2);
		ui->ScalarRB->setChecked(true);

		dataTypeGroup->addButton(ui->NodeRB,1);
		dataTypeGroup->addButton(ui->CellRB,2);
		ui->NodeRB->setChecked(true);

		InitErrorList();
		ui->NameLE->setText(_name);

		if (_type == "scalar")
			ui->ScalarRB->setChecked(true);
		if (_type == "vector")
			ui->VectorRB->setChecked(true);
		if (_dataType == "node")
			ui->NodeRB->setChecked(true);
		if (_dataType == "cell")
			ui->CellRB->setChecked(true);

		connect(ui->OkPBtn, SIGNAL(clicked()), this, SLOT(OnOkPBtnClicked()));
		connect(ui->CancelPBtn, SIGNAL(clicked()), this, SLOT(close()));
// 		connect(ui->ScalarRB, SIGNAL(clicked()), this, SLOT(OnTypeChanged()));
// 		connect(ui->VectorRB, SIGNAL(clicked()), this, SLOT(OnTypeChanged()));
// 		connect(ui->NodeRB, SIGNAL(clicked()), this, SLOT(OnDataTypeChanged()));
// 		connect(ui->CellRB, SIGNAL(clicked()), this, SLOT(OnDataTypeChanged()));
	}

	void Editor3DFileValue::InitErrorList()
	{
		_errorList.insert(NameIsEmpty, tr("Name is empty."));
		_errorList.insert(TheNameInuse, tr("The name is already in use."));
		_errorList.insert(TypeIsEmpty, tr("Type is empty."));
		_errorList.insert(DataTypeIsEmpty, tr("Datatype is empty."));
		_errorList.insert(IllegalName, tr("The name is illegal string."));
	}
	
	QString Editor3DFileValue::getName()
	{
		return _name;
	}

	QString Editor3DFileValue::getType()
	{
		return _type;
	}

	QString Editor3DFileValue::getDataType()
	{
		return _dataType;
	}

	void Editor3DFileValue::OnOkPBtnClicked()
	{
		_name = ui->NameLE->text().trimmed();
		if (ui->ScalarRB->isChecked())
			_type = QString("scalar");
		if (ui->VectorRB->isChecked())
			_type = QString("vector");
		if (ui->NodeRB->isChecked())
			_dataType = QString("node");
		if (ui->CellRB->isChecked())
			_dataType = QString("cell");

		int errorCode = IsDataSure();
		
		if (errorCode != 0)
		{
			QString errorMsg = _errorList[errorCode];
			ui->ErrorText->setText(errorMsg);
			ui->ErrorText->show();
			QTimer::singleShot(3000, this, SLOT(OnTimeout()));
			return;
		}

		this->accept();
		close();
	}

	void Editor3DFileValue::OnTypeChanged()
	{
		if (ui->ScalarRB->isChecked())
			ui->VectorRB->setChecked(false);
		else
			ui->VectorRB->setChecked(true);
			
	}

	void Editor3DFileValue::OnDataTypeChanged()
	{
		if (ui->NodeRB->isChecked())
			ui->CellRB->setChecked(false);
		else
			ui->CellRB->setChecked(true);
	}

	int Editor3DFileValue::IsDataSure()
	{
		if (_name.isEmpty())
			return NameIsEmpty;

		if (_nameUsedList.contains(_name))
			return TheNameInuse;

		if (_type.isEmpty())
			return TypeIsEmpty;

		if (_dataType.isEmpty())
			return DataTypeIsEmpty;

		if (InputValidator::getInstance()->FileNameIsAllow(_name) == false)
			return IllegalName;

		return 0;
	}

	void Editor3DFileValue::setNameUsedList(QList<QString> list)
	{
		_nameUsedList = list;
	}

	void Editor3DFileValue::OnTimeout()
	{
		ui->ErrorText->setText("");
		ui->ErrorText->hide();
	}
}
