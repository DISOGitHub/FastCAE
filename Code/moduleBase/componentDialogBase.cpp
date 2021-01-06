#include "componentDialogBase.h"
#include "ui_componentDialogBase.h"
#include "mainWindow/mainWindow.h"
#include <QListWidgetItem>
#include <QDebug>
#include <assert.h>
#include <QMessageBox>
// #include "geometry/geometryData.h"
// #include "geometry/geometrySet.h"

namespace ModuleBase
{
	ComponentSelectDialogBase::ComponentSelectDialogBase(GUI::MainWindow* mainwindow, DataProperty::DataBase* avaliableData, DataProperty::DataBase* selectData)
		:QFDialog(mainwindow), _mainWindow(mainwindow), _availableData(avaliableData), _selectedData(selectData)
	{
		_ui = new Ui::ComponentDialogBase;
		_ui->setupUi(this);
		_ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("OK"));
		_ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
		_ui->availableList->setSelectionMode(QAbstractItemView::ExtendedSelection);
		_ui->selectedList->setSelectionMode(QAbstractItemView::ExtendedSelection);
//		init();
	}
	ComponentSelectDialogBase::~ComponentSelectDialogBase()
	{
		if (_ui != nullptr)
		{
			_ui->availableList->clear();
			_ui->selectedList->clear();
			delete _ui;
		}

	}
	void ComponentSelectDialogBase::init()
	{
		
	}
	void ComponentSelectDialogBase::accept()
	{
//		getSelectedItemIDNames();
		QDialog::accept();
	}

	void ComponentSelectDialogBase::setSelectedItemMaxNumber(int max, const QString & say)
	{
		_selectedItemMaxSize = max;
		_selectedOutOfRangeWord = say;
	}

// 	void ComponentSelectDialogBase::appendItemToSelectedList(QString name, int id, QString icon /*= ""*/)
// 	{
// 		int iSelectListItemNumber = _ui->selectedList->count();
// 		if (iSelectListItemNumber >= _selectedItemMaxSize)
// 		{
// 			QMessageBox::warning(nullptr, tr("Warning"), _selectedOutOfRangeWord);
// 			return;
// 		}
// 		QListWidgetItem* item = new QListWidgetItem(QIcon(icon), name);
// 		item->setData(Qt::UserRole, id);
// 		_ui->selectedList->addItem(item);
// 	}

	void ComponentSelectDialogBase::setAddButtonText(const QString & text)
	{
		_ui->addButton->setText(text);
	}

	void ComponentSelectDialogBase::setRemoveButtonText(const QString & text)
	{
		_ui->removeButton->setText(text);
	}

	void ComponentSelectDialogBase::appendItemToAvailableList(QString name, int id, QString icon /* = "" */)
	{
		QListWidgetItem* item = new QListWidgetItem(QIcon(icon), name);
		item->setData(Qt::UserRole, id);
		_ui->availableList->addItem(item);
	}
	void ComponentSelectDialogBase::appendItemToSelectedList(QString name, int id, QString icon /* = "" */)
	{
		QListWidgetItem* item = new QListWidgetItem(QIcon(icon), name);
		item->setData(Qt::UserRole, id);
		_ui->selectedList->addItem(item);
	}
	void ComponentSelectDialogBase::on_addButton_clicked()
	{
		int iSelectListItemNumber = _ui->selectedList->count();
		if (iSelectListItemNumber >= _selectedItemMaxSize)
		{
			QMessageBox::warning(nullptr, tr("Warning"), _selectedOutOfRangeWord);
			return;
		}

		QList<QListWidgetItem*> itemList = _ui->availableList->selectedItems();
		const int n = itemList.size();
		for (int i = 0; i < n; ++i)
		{
			QListWidgetItem* availitem = itemList.at(i);
			const int row = _ui->availableList->row(availitem);
			QListWidgetItem* selectitem = new QListWidgetItem(availitem->icon(),availitem->text());
			const int id = availitem->data(Qt::UserRole).toInt();
			selectitem->setData(Qt::UserRole, id);
			_ui->selectedList->addItem(selectitem);
			_ui->availableList->takeItem(row);
		}
	}
	void ComponentSelectDialogBase::on_removeButton_clicked()
	{
		QList<QListWidgetItem*> itemList = _ui->selectedList->selectedItems();
		const int n = itemList.size();
		for (int i = 0; i < n; ++i)
		{
			QListWidgetItem* selectitem = itemList.at(i);
			const int row = _ui->selectedList->row(selectitem);
			QListWidgetItem* availitem = new QListWidgetItem(selectitem->icon(), selectitem->text());
			const int id = selectitem->data(Qt::UserRole).toInt();
			availitem->setData(Qt::UserRole, id);
			_ui->availableList->addItem(availitem);

			_ui->selectedList->takeItem(row);
		}

	}
	QMap<int, QString> ComponentSelectDialogBase::getSelectedItemIDNames()
	{
		QMap<int, QString> id_names;
		const int n = _ui->selectedList->count();
		for (int i = 0; i < n; ++i)
		{
			QListWidgetItem*item = _ui->selectedList->item(i);
			const int id = item->data(Qt::UserRole).toInt();
			const QString text = item->text();
			assert(id > 0);
			id_names[id] = text;
		}
		return id_names;
	}
	void ComponentSelectDialogBase::setCustomLayout(QBoxLayout* l)
	{
		_ui->customLayout->addLayout(l);
	}

}