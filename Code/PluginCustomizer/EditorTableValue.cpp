#include "ui_EditorTableValue.h"
#include "EditorTableValue.h"
#include "EditorNameValue.h"
#include "DataProperty/ParameterTable.h"
#include "InputValidator.h"
#include <QTimer>
#include <QMessageBox>
#include <QDebug>
// #include <QTableView>
// #include <QTableWidget>
// #include <QHeaderView>
#include <QIntValidator>


namespace FastCAEDesigner{
	EditorTableValue::EditorTableValue(QWidget *parent) :
		QDialog(parent),
		ui(new Ui::EditorTableValue)
	{
		ui->setupUi(this);
		ui->tableWidget->verticalHeader()->setVisible(false);
		Init();
	}

	EditorTableValue::EditorTableValue(DataProperty::ParameterTable* model, QWidget *parent) :
		QDialog(parent),
		ui(new Ui::EditorTableValue),
		_model(model)
	{
		ui->setupUi(this);
		ui->tableWidget->verticalHeader()->setVisible(false);
		Init();
	}

	EditorTableValue::~EditorTableValue()
	{
		delete ui;
		_usedNameList.clear();
	}
	
	void EditorTableValue::Init()
	{
		UpdateDataToUi();
		InitErrorList();
		connect(ui->OkPBtn, SIGNAL(clicked()), this, SLOT(OnOkPBtnClicked()));
		connect(ui->CancelPBtn, SIGNAL(clicked()), this, SLOT(close()));
		connect(ui->RefreshPBtn, SIGNAL(clicked()), this, SLOT(OnRefreshPBtnClicked()));
		connect(ui->tableWidget->horizontalHeader(), SIGNAL(sectionDoubleClicked(int)), this, SLOT(OnModifyHeader(int)));

		//Added xvdongming 2019-12-25
		int min = 0;
		int max = 2147483647;//4294967295;
		QValidator *validator = new QIntValidator(min, max, this);
		ui->RowsLE->setValidator(validator);
		ui->ColumnsLE->setValidator(validator);
		//Added xvdongming 2019-12-25
	}

	void EditorTableValue::InitErrorList()
	{
		_errorList.insert(NameIsEmpty, tr("Name is empty."));
		_errorList.insert(TheNameInUse, tr("The name is already in use."));
		_errorList.insert(RowsIsZero, tr("Table rows is zero."));
		_errorList.insert(ColumnsIsZero, tr("Table columns is zero."));
		_errorList.insert(IllegalName, tr("The name is illegal string."));
		_errorList.insert(TableDataError, tr("The data in table is error!"));
		_errorList.insert(TableNotInit, tr("The form is not initialized."));
	}
	void EditorTableValue::UpdateDataToUi()
	{
		ui->NameLE->setText(_model->getDescribe());
		int rowCount = _model->getRowCount();
		int columnCount = _model->getColumnCount();
		if (rowCount == 0 || columnCount == 0)
		{
			ui->RowsLE->setText(QString::number(2));
			ui->ColumnsLE->setText(QString::number(3));
			ui->tableWidget->setRowCount(2);
			ui->tableWidget->setColumnCount(3);
			InitTableData(2, 3);
		}
		else
		{
			QStringList titleList = _model->getTitle();
			ui->RowsLE->setText(QString::number(rowCount));
			ui->ColumnsLE->setText(QString::number(columnCount));
			ui->tableWidget->setRowCount(rowCount);
			ui->tableWidget->setColumnCount(columnCount);
			InsertDataToTable(rowCount, columnCount,titleList);
		}
		

	}
	void EditorTableValue::UpdateUiToData()
	{
		_model->setDescribe(ui->NameLE->text().trimmed());
		int rows = ui->RowsLE->text().toInt();
		int columns = ui->ColumnsLE->text().toInt();
		_model->setRowCount(rows);
		_model->setColumnCount(columns);
		InsertDataToModel(rows, columns);
		//_model->setValue();
	}

	void EditorTableValue::setNameUsedList(QList<QString> list)
	{
		_usedNameList = list;
	}
	void EditorTableValue::OnOkPBtnClicked()
	{
		/*
		int error = IsNameSure();
		if (error != 0)
		{
			QString errorMsg = _errorList[error];
			ui->ErrorText->setText(errorMsg);
			ui->ErrorText->show();
			QTimer::singleShot(3000, this, SLOT(OnTimeout()));
			return;
		}

		int rows = ui->RowsLE->text().toInt();
		int columns = ui->ColumnsLE->text().toInt();

		bool b = IsDataSure(rows, columns);
		if (b == false)
		{
			ui->ErrorText->setText(tr("The data in table is error!"));
			ui->ErrorText->show();
			QTimer::singleShot(3000, this, SLOT(OnTimeout()));
			return;
		}

		UpdateUiToData();
		this->accept();
		close();
		*/

		//Added xvdongming 2019-12-24
		int errorCode = IsDataOk();
		if (errorCode != 0)
		{
			QString errorMsg = _errorList[errorCode];
			ui->ErrorText->setText(errorMsg);
			ui->ErrorText->show();
			QTimer::singleShot(3000, this, SLOT(OnTimeout()));
			return;
		}

		UpdateUiToData();
		this->accept();
		close();
		//Added xvdongming 2019-12-24
	}

	//Added xvdongming 2019-12-24
	int EditorTableValue::IsDataOk()
	{
		int rows = ui->RowsLE->text().toInt();
		int columns = ui->ColumnsLE->text().toInt();
		int formRows = ui->tableWidget ->rowCount();
		int formCols = ui->tableWidget->columnCount();
		QString name = ui->NameLE->text().trimmed();

		if (_usedNameList.contains(name))
			return TheNameInUse;

		if (name.isEmpty())
			return NameIsEmpty;

		//if (InputValidator::getInstance()->FileNameIsAllow(name) == false)
	    //		return IllegalName;

		if (0 == rows)
			return RowsIsZero;
		
		if (0 == columns)
			return ColumnsIsZero;

		if (rows != formRows || columns != formCols)
			return TableNotInit;

		if (!IsTableDataSure(rows, columns))
			return TableDataError;

		return 0;
	}

	//Added xvdongming 2019-12-24
	bool EditorTableValue::IsTableDataSure(int rows, int columns)
	{
		bool isOk = true;

		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < columns; j++)
			{
				//QString text = ui->tableWidget->item(i, j)->text().trimmed();
				QTableWidgetItem* item = ui->tableWidget->item(i, j);
				if (nullptr == item)
				{
					qDebug() << "EditorTableValue::IsTableDataSure(int rows, int columns):widgeetItem is nullptr";
					continue;
				}

				QString text = item->text().trimmed();
				text.toDouble(&isOk);

				if (!isOk)
					return isOk;

			}
		}

		return isOk;
	}

	int EditorTableValue::IsNameSure()
	{
		QString name = ui->NameLE->text().trimmed();

		if (_usedNameList.contains(name))
			return TheNameInUse;

		if (name.isEmpty())
			return NameIsEmpty;

		if (InputValidator::getInstance()->FileNameIsAllow(name) == false)
			return IllegalName;


		return 0;
	}
	
	bool EditorTableValue::IsDataSure(int rows, int columns)
	{
		
		bool isOk = true;

		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < columns; j++)
			{
				QString text = ui->tableWidget->item(i, j)->text().trimmed();
				text.toDouble(&isOk);
				
				if (!isOk)
					return isOk;
			}
		}

		return isOk;
	}

	void EditorTableValue::OnTimeout()
	{
		ui->ErrorText->setText("");
		ui->ErrorText->hide();
	}

	void EditorTableValue::OnRefreshPBtnClicked()
	{
		
		int error = IsTableRowsOrColumnsZero();
		if (error != 0)
		{
			QString errorMsg = _errorList[error];
			ui->ErrorText->setText(errorMsg);
			ui->ErrorText->show();
			QTimer::singleShot(3000, this, SLOT(OnTimeout()));
			return;
		}

// 		QMessageBox::StandardButton result = QMessageBox::information(this, tr("Prompt!"), tr("Whether to modify the table?"),
// 			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

		QMessageBox msgBox(QMessageBox::Information, tr("Prompt!"), tr("Whether to modify the table?"));
		//msgBox.setWindowTitle(tr("Prompt!"));
		//msgBox.setText(tr("Whether to modify the table?"));
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		msgBox.setButtonText(QMessageBox::Yes, tr("Yes"));
		msgBox.setButtonText(QMessageBox::No, tr("No"));

		int result = msgBox.exec();

		switch (result)
		{
		case QMessageBox::Yes:
		{	//	qDebug() << "Yes";
			int rows = ui->RowsLE->text().toInt();//Added xvdongming 2019-12-24
			int columns = ui->ColumnsLE->text().toInt();//Added xvdongming 2019-12-24
			ui->tableWidget->setRowCount(ui->RowsLE->text().toInt());
			ui->tableWidget->setColumnCount(ui->ColumnsLE->text().toInt());
			InsertDataToTable(rows, columns);//Added xvdongming 2019-12-24
			break;
		}
		case QMessageBox::No:
		//	qDebug() << "NO";
			ui->RowsLE->setText(QString::number(ui->tableWidget->rowCount()));
			ui->ColumnsLE->setText(QString::number(ui->tableWidget->columnCount()));
			break;
		default:
			break;
		}
	}

	int EditorTableValue::IsTableRowsOrColumnsZero()
	{
		int rowsCount = ui->RowsLE->text().toInt();
		int columnsCount = ui->ColumnsLE->text().toInt();

		if (rowsCount == 0)
			return RowsIsZero;

		if (columnsCount == 0)
			return ColumnsIsZero;
		
		return 0;
	}

	void EditorTableValue::InsertDataToTable(int rows, int columns)
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < columns; j++)
			{
				QString value = QString("%1").arg(_model->getValue(i, j));
				QTableWidgetItem* item = new QTableWidgetItem;
				item->setText(value);
				ui->tableWidget->setItem(i, j, item);
			}
		}
	}
	void EditorTableValue::InsertDataToTable(int rows, int columns, QStringList titleList)
	{
		for (int i = 0; i < titleList.count(); i++)
		{
			QTableWidgetItem* titleItem = new QTableWidgetItem;
			titleItem->setText(titleList.at(i));
			ui->tableWidget->setHorizontalHeaderItem(i, titleItem);
		}
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < columns; j++)
			{
				QString value = QString("%1").arg(_model->getValue(i, j));
				QTableWidgetItem* item = new QTableWidgetItem;
				item->setText(value);
				ui->tableWidget->setItem(i, j, item);	
			}
		}
	}

	void EditorTableValue::InitTableData(int rows, int columns)
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < columns; j++)
			{
				QTableWidgetItem* item = new QTableWidgetItem;
				item->setText(QString::number(0));
				ui->tableWidget->setItem(i, j, item);
			}
		}
	}

	void EditorTableValue::InsertDataToModel(int rows, int columns)
	{
		QStringList titlelist;
		for (int i = 0; i < columns; i++)
		{
			if (ui->tableWidget->horizontalHeaderItem(i) != nullptr)
				titlelist.append(ui->tableWidget->horizontalHeaderItem(i)->text());
			else
				titlelist.append(QString("%1").arg(i + 1));
		}
		//qDebug() << titlelist;
		_model->setTitle(titlelist);
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < columns; j++)
			{
				double value = 0;

				if (ui->tableWidget->item(i,j) != nullptr)
					value = ui->tableWidget->item(i, j)->text().toDouble();
				
				_model->setValue(i, j, value);
			}
		}
	}

	void EditorTableValue::OnModifyHeader(int column)
	{
		/*qDebug() << column;*/
		EditorNameValue dlg;

		dlg.setWindowTitle(tr("Table Title Name"));

		QString name, Name;
		if (ui->tableWidget->horizontalHeaderItem(column) != nullptr)
		{ 
			name = ui->tableWidget->horizontalHeaderItem(column)->text();
			dlg.SetNameString(name);
		}
		else
		{
			name = QString("%1").arg(column + 1);
			dlg.SetNameString(name);
		}
		
		int r = dlg.exec();

		if (r == QDialog::Accepted)
		{
			Name = dlg.GetNameString();
			QTableWidgetItem* item = new QTableWidgetItem;
			item->setText(Name);
			ui->tableWidget->setHorizontalHeaderItem(column, item);
		}

	}
}