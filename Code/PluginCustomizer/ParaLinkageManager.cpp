#include "ParaLinkageManager.h"
#include "ui_ParaLinkageManager.h"
#include "EditorDescripttionSetup.h"
#include "ParametersLinkage.h"
#include "ParaLinkageData.h"
#include "ParaManagerData.h"
#include "DataProperty/ParameterBase.h"
#include "DataProperty/ParameterGroup.h"
#include "DataProperty/ParameterTable.h"
#include <QListWidgetItem>
#include <QTableWidgetItem>
#include <QDebug>


namespace FastCAEDesigner{
	ParaLinkageManager::ParaLinkageManager(QString name,QWidget *parent) :
		QDialog(parent),
		ui(new Ui::ParaLinkageManager),
		_caseName(name)
	{
		ui->setupUi(this);

		init();
		initParameterTableWidget();

		connect(ui->CreateQPB, SIGNAL(clicked()), this, SLOT(onCreateQPBClicked()));
		connect(ui->EditQPB, SIGNAL(clicked()), this, SLOT(onEditQPBClicked()));
		connect(ui->DeleteQPB, SIGNAL(clicked()), this, SLOT(onDeleteQPBClicked()));
		connect(ui->linkageListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onItemClicked(QListWidgetItem*)));
		//connect(ui->linkageListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(onItemClicked(int)));
	}

	ParaLinkageManager::~ParaLinkageManager()
	{
		delete ui;
	}

	void ParaLinkageManager::init()
	{
	//	_paraDataDict.clear();
		ui->linkageListWidget->clear();

		_paraLinkageDataList = ParaManagerData::getInstance()->getParameterLinkageList(_caseName);
		
		for (int i = 0; i < _paraLinkageDataList.count();i++)
		{
			qDebug() << _paraLinkageDataList.at(i)->getParameterName();
			QListWidgetItem* item = new QListWidgetItem;
			item->setText(_paraLinkageDataList.at(i)->getParameterName());
			ui->linkageListWidget->addItem(item);

			//_paraDataDict.insert(item, _paraLinkageDataList.at(i));
		}

	}

	void ParaLinkageManager::onCreateQPBClicked()
	{
		ParaLinkageData* data = new ParaLinkageData();
		ParametersLinkage dlg(data,_caseName);
		int r = dlg.exec();

		if (r == QDialog::Accepted)
		{
			ParaManagerData::getInstance()->setParameterLinkageList(_caseName, data/*dlg.getParameterLinkageData()*/);
			init();
		}
	}

	void ParaLinkageManager::onDeleteQPBClicked()
	{
		int index = ui->linkageListWidget->currentRow();
		QListWidgetItem* item = ui->linkageListWidget->takeItem(index);
		ParaManagerData::getInstance()->removeParameterLinkageDataFromList(_caseName, index);
		//_paraDataDict.remove(item);
	}

	void ParaLinkageManager::onEditQPBClicked()
	{
		int index = ui->linkageListWidget->currentRow();
		ParaLinkageData* data = _paraLinkageDataList.at(index);
		ParametersLinkage dlg(data, _caseName);
		int r = dlg.exec();

		if (r == QDialog::Accepted)
		{
			ParaManagerData::getInstance()->removeParameterLinkageDataFromList(_caseName, index);
			ParaManagerData::getInstance()->setParameterLinkageList(_caseName, data);
			init();
		}
	}

	void ParaLinkageManager::onItemClicked(QListWidgetItem* item)
	{
		if (item == nullptr)
			return;

		int index = ui->linkageListWidget->row(item);
		ParaLinkageData* data = _paraLinkageDataList.at(index);

		if (data == nullptr)
			return;
		
		showActiveList(data->getActiveList());
		showDrivenList(data->getDrivenList());
		showDrivenGroupList(data->getDrivenGroupList());
	}

// 	void ParaLinkageManager::onItemClicked(int index)
// 	{
// 		ParaLinkageData* data = _paraLinkageDataList.at(index);
// 
// 		if (data == nullptr)
// 			return;
// 
// 		showActiveList(data->getActiveList());
// 		showDrivenList(data->getDrivenList());
// 		showDrivenGroupList(data->getDrivenGroupList());
// 	}


	void ParaLinkageManager::showActiveList(QList<DataProperty::ParameterBase*> list)
	{
		ui->activeTableWidget->reset();
		ui->activeTableWidget->clear();
		ui->activeTableWidget->setRowCount(list.count());
		ui->activeTableWidget->setColumnCount(4);

		QStringList head;
		head.append(tr("group name"));
		head.append(tr("parameter name"));
		head.append(tr("parameter type"));
		head.append(tr("value"));

		ui->activeTableWidget->setHorizontalHeaderLabels(head);

		for (int i = 0; i < list.count(); i++)
			insertActiveDataToTable(i, list.at(i));
	}

	void ParaLinkageManager::showDrivenList(QList<DataProperty::ParameterBase*> list)
	{
		ui->drivenTableWidget->reset();
		ui->drivenTableWidget->clear();
		ui->drivenTableWidget->setRowCount(list.count());
		ui->drivenTableWidget->setColumnCount(6);

		QStringList head;
		head.append(tr("group name"));
		head.append(tr("parameter name"));
		head.append(tr("parameter type"));
		head.append(tr("value"));
		head.append(tr("editable or not"));
		head.append(tr("visible or not"));

		ui->drivenTableWidget->setHorizontalHeaderLabels(head);

		for (int i = 0; i < list.count(); i++)
			insertDrivenDataToTable(i, list.at(i));
	}

	void ParaLinkageManager::showDrivenGroupList(QList<DataProperty::ParameterGroup*> list)
	{
		ui->drivenGroupTableWidget->reset();
		ui->drivenGroupTableWidget->clear();
		ui->drivenGroupTableWidget->setRowCount(list.count());
		ui->drivenGroupTableWidget->setColumnCount(2);

		QStringList head;
		head.append(tr("group name"));
		head.append(tr("visible or not"));

		ui->drivenGroupTableWidget->setHorizontalHeaderLabels(head);

		for (int i = 0; i < list.count(); i++)
			insertDrivenGroupDataToTable(i, list.at(i));
	}

	void ParaLinkageManager::insertActiveDataToTable(int row, DataProperty::ParameterBase* model)
	{
		QTableWidgetItem* groupName = new QTableWidgetItem;
		QTableWidgetItem* name = new QTableWidgetItem;
		QTableWidgetItem* type = new QTableWidgetItem;
		QTableWidgetItem* value = new QTableWidgetItem;

		if (model->getGroupName().isEmpty())
			groupName->setText("0");
		else
			groupName->setText(model->getGroupName());

		name->setText(model->getDescribe());
		type->setText(model->ParaTypeToString(model->getParaType()));

		if (model->getParaType() == DataProperty::Para_Table)
		{
			int rows = ((DataProperty::ParameterTable*)model)->getRowCount();
			int columns = ((DataProperty::ParameterTable*)model)->getColumnCount();
			QString s = QString("%1X%2").arg(rows).arg(columns);
			value->setText(s);
		}
		else
			value->setText(model->valueToString());

		ui->activeTableWidget->setItem(row, 0, groupName);
		ui->activeTableWidget->setItem(row, 1, name);
		ui->activeTableWidget->setItem(row, 2, type);
		ui->activeTableWidget->setItem(row, 3, value);
		
	}

	void ParaLinkageManager::insertDrivenDataToTable(int row, DataProperty::ParameterBase* model)
	{
		QTableWidgetItem* groupName = new QTableWidgetItem;
		QTableWidgetItem* name = new QTableWidgetItem;
		QTableWidgetItem* type = new QTableWidgetItem;
		QTableWidgetItem* value = new QTableWidgetItem;
		QTableWidgetItem* editable = new QTableWidgetItem;
		QTableWidgetItem* visiable = new QTableWidgetItem;

		if (model->getGroupName().isEmpty())
			groupName->setText("0");
		else
			groupName->setText(model->getGroupName());

		name->setText(model->getDescribe());
		type->setText(model->ParaTypeToString(model->getParaType()));

		if (model->getParaType() == DataProperty::Para_Table)
		{
			int rows = ((DataProperty::ParameterTable*)model)->getRowCount();
			int columns = ((DataProperty::ParameterTable*)model)->getColumnCount();
			QString s = QString("%1X%2").arg(rows).arg(columns);
			value->setText(s);
		}
		else
			value->setText(model->valueToString());

		if (model->isEditable())
			editable->setText("true");
		else
			editable->setText("false");

		if (model->isVisible())
			visiable->setText("true");
		else
			visiable->setText("false");

		ui->drivenTableWidget->setItem(row, 0, groupName);
		ui->drivenTableWidget->setItem(row, 1, name);
		ui->drivenTableWidget->setItem(row, 2, type);
		ui->drivenTableWidget->setItem(row, 3, value);
		ui->drivenTableWidget->setItem(row, 4, editable);
		ui->drivenTableWidget->setItem(row, 5, visiable);
	}

	void ParaLinkageManager::insertDrivenGroupDataToTable(int row, DataProperty::ParameterGroup* model)
	{
		QTableWidgetItem* groupName = new QTableWidgetItem;
		QTableWidgetItem* visiable = new QTableWidgetItem;

		groupName->setText(model->getDescribe());

		if (model->isVisible())
			visiable->setText("true");
		else
			visiable->setText("false");

		ui->drivenGroupTableWidget->setItem(row, 0, groupName);
		ui->drivenGroupTableWidget->setItem(row, 1, visiable);
	}

	void ParaLinkageManager::initParameterTableWidget()
	{
		ui->activeTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		ui->activeTableWidget->setWindowFlags(Qt::FramelessWindowHint);
		ui->activeTableWidget->setShowGrid(false);
		ui->activeTableWidget->verticalHeader()->setVisible(false);

		ui->drivenTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		ui->drivenTableWidget->setWindowFlags(Qt::FramelessWindowHint);
		ui->drivenTableWidget->setShowGrid(false);
		ui->drivenTableWidget->verticalHeader()->setVisible(false);

		ui->drivenGroupTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		ui->drivenGroupTableWidget->setWindowFlags(Qt::FramelessWindowHint);
		ui->drivenGroupTableWidget->setShowGrid(false);
		ui->drivenGroupTableWidget->verticalHeader()->setVisible(false);
	}
}

