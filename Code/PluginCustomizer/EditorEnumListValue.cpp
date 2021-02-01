#include "EditorEnumListValue.h"
#include "ui_EditorEnumListValue.h"
#include "EditorNameValue.h"
#include <QListWidgetItem>
#include <QDebug>
#include <QMessageBox>

namespace FastCAEDesigner
{

	EditorEnumListValue::EditorEnumListValue(QWidget *parent) :
		QDialog(parent),
		ui(new Ui::EditorEnumListValue)
	{
		ui->setupUi(this);
		Init();
	}

	EditorEnumListValue::~EditorEnumListValue()
	{
		delete ui;
	}

	void EditorEnumListValue::Init()
	{
		connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(OnBtnOkClicked()));
		connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
		connect(ui->btnAdd, SIGNAL(clicked()), this, SLOT(OnBtnAddClicked()));
		connect(ui->btnInsert, SIGNAL(clicked()), this, SLOT(OnBtnInsertClicked()));
		connect(ui->btnDel, SIGNAL(clicked()), this, SLOT(OnBtnDelClicked()));
		connect(ui->btnClearAll, SIGNAL(clicked()), this, SLOT(OnBtnClearAllClicked()));
	}

	QList<QString> EditorEnumListValue::GetUsedName()
	{
		QList<QString> usedNameList;
		int count = ui->listWidget->count();

		for (int i = 0; i < count; i++)
		{
			QListWidgetItem* item = ui->listWidget->item(i);
			usedNameList.append(item->text());
		}

		return usedNameList;
	}

	//确认设定槽函数
	void EditorEnumListValue::OnBtnOkClicked()
	{
		_valList.clear();
		int count = ui->listWidget->count();

		for (int i = 0; i < count; i++)
		{
			QListWidgetItem* item = ui->listWidget->item(i);
			_valList.append(item->text());
		}

		this->accept();
		close();
	}
	
	void EditorEnumListValue::OnBtnAddClicked()
	{
		EditorNameValue dlg(this);
		dlg.SetUsedNameList(GetUsedName());
		int r = dlg.exec();
		
		if (r == QDialog::Accepted)
		{
			ui->listWidget->addItem(dlg.GetNameString());
		}
	}

	void EditorEnumListValue::OnBtnInsertClicked()
	{
		EditorNameValue dlg(this);
		dlg.SetUsedNameList(GetUsedName());
		int r = dlg.exec();

		if (r == QDialog::Accepted)
		{
			int count = ui->listWidget->count();
			int index = ui->listWidget->currentIndex().row();
			qDebug() << QString("count:%1,Index:%2").arg(count).arg(index);
			index = (index < 0) ? 0 : index;
			ui->listWidget->insertItem(index, dlg.GetNameString());
		}
	}

	void EditorEnumListValue::OnBtnDelClicked()
	{
		int count = ui->listWidget->count();

		if (0 == count)
			return;

		int index = ui->listWidget->currentRow();
		
		if (index >= 0)
		{
			QListWidgetItem *item = ui->listWidget->takeItem(index);
			if (item != nullptr)
				delete item;
		}
	}

	void EditorEnumListValue::OnBtnClearAllClicked()
	{
		int count = ui->listWidget->count();
		
		if (0 == count)
			return;

		QString errorMsg = tr("You confirm this operation");
		QMessageBox messageBox(QMessageBox::NoIcon, tr("Error"), errorMsg, QMessageBox::Yes | QMessageBox::No, NULL);;
		
		if (messageBox.exec() == QMessageBox::Yes)
		{
			int num = ui->listWidget->count();
			ui->listWidget->setFocus();

			for (int i = 0; i < num; i++)
			{
				QListWidgetItem *item = ui->listWidget->takeItem(ui->listWidget->currentRow());
				if (item != nullptr)
					delete item;
			}
		}
	}

	void EditorEnumListValue::SetEnumList(QStringList valList)
	{
		_valList = valList;
		ui->listWidget->clear();
		ui->listWidget->insertItems(0, _valList);
	}

	QStringList EditorEnumListValue::GetEnumList()
	{
		return _valList;
	}
}