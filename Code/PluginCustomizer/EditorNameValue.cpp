#include "EditorNameValue.h"
#include "ui_EditorNameValue.h"
#include "InputValidator.h"
#include <QTimer>


namespace FastCAEDesigner
{
	EditorNameValue::EditorNameValue(QWidget *parent) :
		QDialog(parent),
		ui(new Ui::EditorNameValue)
	{
		ui->setupUi(this);
		ui->comboBox->setVisible(false);

		connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(OnBtnOkClicked()));
		connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
	}

	EditorNameValue::EditorNameValue(MonitorModel* model, QWidget *parent) :
		QDialog(parent),
		ui(new Ui::EditorNameValue),
		_monitorModel(model)
	{
		ui->setupUi(this);
		this->setWindowTitle(tr("Monitor File Name"));
		ui->comboBox->setVisible(true);
		ui->comboBox->insertItem(0, tr(".dat"));
		

		connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(OnBtnOkClicked_File()));
		connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
	}

	EditorNameValue::EditorNameValue(CurveModel* model, QWidget *parent) :
		QDialog(parent),
		ui(new Ui::EditorNameValue),
		_curveModel(model)
	{
		ui->setupUi(this);
		this->setWindowTitle(tr("2D Plot File Name"));
		ui->comboBox->setVisible(true);
		ui->comboBox->insertItem(0, tr(".dat"));

		connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(OnBtnOkClicked_File()));
		connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
	}

	EditorNameValue::EditorNameValue(VectorModel* model, QWidget *parent) :
		QDialog(parent),
		ui(new Ui::EditorNameValue),
		_vectorModel(model)
	{
		ui->setupUi(this);
		this->setWindowTitle(tr("3D Graph File Name"));
		ui->comboBox->setVisible(true);
		ui->comboBox->insertItem(0, tr(".vtk"));
		ui->comboBox->insertItem(1, tr(".sol"));
		ui->comboBox->insertItem(2, tr(".cgns"));
		ui->comboBox->insertItem(3, tr(".dat"));

		connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(OnBtnOkClicked_File()));
		connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
	}

	EditorNameValue::~EditorNameValue()
	{
		delete ui;
	}

	QString EditorNameValue::GetNameString()
	{
		return _name;
	}

	void EditorNameValue::SetNameString(QString name)
	{
		_name = name;
		ui->txtName->setText(name);
	}

	//确认设定槽函数
	void EditorNameValue::OnBtnOkClicked()
	{
		QString name = ui->txtName->text().trimmed();
		
		if (_usedNameList.contains(name))
		{
			QString errorMsg = tr("Record already exists, operation failed");
			ui->lbl_info->setText(errorMsg);
			ui->lbl_info->show();
			QTimer::singleShot(3000, this, SLOT(OnTimeout()));
			return;
		}

		_name = ui->txtName->text().trimmed();
		this->accept();
		close();
	}

	void EditorNameValue::OnBtnOkClicked_File()
	{
		QString name = ui->txtName->text().trimmed();
		name.append(ui->comboBox->currentText().trimmed());

		if (ui->txtName->text().trimmed() == "")
		{
			QString errorMsg = tr("The name is empty.");
			ui->lbl_info->setText(errorMsg);
			ui->lbl_info->show();
			QTimer::singleShot(3000, this, SLOT(OnTimeout()));
			return;
		}
		else if (_usedNameList.contains(name))
		{
			QString errorMsg = tr("The name is already in used.");
			ui->lbl_info->setText(errorMsg);
			ui->lbl_info->show();
			QTimer::singleShot(3000, this, SLOT(OnTimeout()));
			return;
		}
		else if (InputValidator::getInstance()->FileNameIsAllow(ui->txtName->text().trimmed()) == false)
		{
			QString errorMsg = tr("The name is illegal string.");
			ui->lbl_info->setText(errorMsg);
			ui->lbl_info->show();
			QTimer::singleShot(3000, this, SLOT(OnTimeout()));
			return;
		}

		_name = name;
		//_name = ui->txtName->text().trimmed();
		this->accept();
		close();
	}

	void EditorNameValue::SetUsedNameList(QList<QString> usedNameList)
	{
		_usedNameList = usedNameList;
	}

	//定时器槽函数
	void EditorNameValue::OnTimeout()
	{
		ui->lbl_info->setText("");
		ui->lbl_info->hide();
	}

}