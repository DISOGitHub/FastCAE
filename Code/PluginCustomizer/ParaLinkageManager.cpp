#include "ParaLinkageManager.h"
#include "ui_ParaLinkageManager.h"
#include "EditorDescripttionSetup.h"
#include "ParametersLinkage.h"
#include "ParaLinkageData.h"
#include "ParaManagerData.h"
#include "DataProperty/ParameterBase.h"
#include "DataProperty/ParameterGroup.h"
#include "DataProperty/ParameterTable.h"
#include "DataProperty/ParameterSelectable.h"
#include <QListWidgetItem>
#include <QTableWidgetItem>
#include <QDebug>


namespace FastCAEDesigner{
	ParaLinkageManager::ParaLinkageManager(int type,QWidget *parent) :
		QDialog(parent),
		ui(new Ui::ParaLinkageManager),
		_type(type)
	{
		ui->setupUi(this);

// 		ui->activeTableWidget->setEnabled(false);
// 		ui->drivenTableWidget->setEnabled(false);
// 		ui->drivenGroupTableWidget->setEnabled(false);

		ParaManagerData::getInstance()->readObserverConfig();

		init();
		initParameterTableWidget();
		initTableWidgetTitle();

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
		_usedNameList.clear();

		_paraLinkageDataList = ParaManagerData::getInstance()->getParameterLinkageList(_type);
		
		for (int i = 0; i < _paraLinkageDataList.count();i++)
		{
			QString name = _paraLinkageDataList.at(i)->getParameterName();
//			qDebug() << name;
			QListWidgetItem* item = new QListWidgetItem;
			item->setText(name);
			ui->linkageListWidget->addItem(item);

			_usedNameList.append(name);

			//_paraDataDict.insert(item, _paraLinkageDataList.at(i));
		}

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

	void ParaLinkageManager::initTableWidgetTitle()
	{
		ui->activeTableWidget->setColumnCount(4);
		QStringList activehead;
		activehead.append(tr("group name"));
		activehead.append(tr("parameter name"));
		activehead.append(tr("parameter type"));
		activehead.append(tr("value"));
		ui->activeTableWidget->setHorizontalHeaderLabels(activehead);

		ui->drivenTableWidget->setColumnCount(6);
		QStringList drivenhead;
		drivenhead.append(tr("group name"));
		drivenhead.append(tr("parameter name"));
		drivenhead.append(tr("parameter type"));
		drivenhead.append(tr("value"));
		drivenhead.append(tr("editable or not"));
		drivenhead.append(tr("visible or not"));
		ui->drivenTableWidget->setHorizontalHeaderLabels(drivenhead);

		ui->drivenGroupTableWidget->setColumnCount(2);
		QStringList grouphead;
		grouphead.append(tr("group name"));
		grouphead.append(tr("visible or not"));
		ui->drivenGroupTableWidget->setHorizontalHeaderLabels(grouphead);
	}

	void ParaLinkageManager::refreshParameterLinkage(ParaLinkageData* data)
	{
		clearTableWidget();

		showActiveList(data->getActiveList());
		showDrivenList(data->getDrivenList());
		showDrivenGroupList(data->getDrivenGroupList());
	}

	void ParaLinkageManager::onCreateQPBClicked()
	{
		ParaLinkageData* data = new ParaLinkageData();
		ParametersLinkage dlg(data,_type);
		dlg.setUsedNameList(_usedNameList);
		int r = dlg.exec();

		if (r == QDialog::Accepted)
		{
			ParaManagerData::getInstance()->setParameterLinkageList(_type, data/*dlg.getParameterLinkageData()*/);
			init();
			refreshParameterLinkage(data);
		}
	}

	void ParaLinkageManager::onDeleteQPBClicked()
	{
		int index = ui->linkageListWidget->currentRow();
		if (index < 0) return;
		QListWidgetItem* item = ui->linkageListWidget->takeItem(index);
		ParaManagerData::getInstance()->removeParameterLinkageDataFromList(_type, index);
		//_paraDataDict.remove(item);
		init();
		clearTableWidget();

	}

	void ParaLinkageManager::onEditQPBClicked()
	{
		int index = ui->linkageListWidget->currentRow();
		if (index < 0) return;
		
		ParaLinkageData* data = _paraLinkageDataList.at(index);
		if (data == nullptr) return;
		_usedNameList.removeOne(data->getParameterName());

		ParametersLinkage dlg(data, _type);
		dlg.setUsedNameList(_usedNameList);
		int r = dlg.exec();

		if (r == QDialog::Accepted)
		{
			//ParaManagerData::getInstance()->removeParameterLinkageDataFromList(_type, index);
			ParaManagerData::getInstance()->setParameterLinkageList(_type, data);
			init();
			refreshParameterLinkage(data);
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
		
		refreshParameterLinkage(data);
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
	//	ui->activeTableWidget->clear();
		ui->activeTableWidget->setRowCount(list.count());
// 		ui->activeTableWidget->setColumnCount(4);
// 
// 		QStringList head;
// 		head.append(tr("group name"));
// 		head.append(tr("parameter name"));
// 		head.append(tr("parameter type"));
// 		head.append(tr("value"));
// 
// 		ui->activeTableWidget->setHorizontalHeaderLabels(head);

		for (int i = 0; i < list.count(); i++)
			insertActiveDataToTable(i, list.at(i));
	}

	void ParaLinkageManager::showDrivenList(QList<DataProperty::ParameterBase*> list)
	{
		ui->drivenTableWidget->reset();
		//ui->drivenTableWidget->clear();
		ui->drivenTableWidget->setRowCount(list.count());
// 		ui->drivenTableWidget->setColumnCount(6);
// 
// 		QStringList head;
// 		head.append(tr("group name"));
// 		head.append(tr("parameter name"));
// 		head.append(tr("parameter type"));
// 		head.append(tr("value"));
// 		head.append(tr("editable or not"));
// 		head.append(tr("visible or not"));
// 
// 		ui->drivenTableWidget->setHorizontalHeaderLabels(head);

		for (int i = 0; i < list.count(); i++)
			insertDrivenDataToTable(i, list.at(i));
	}

	void ParaLinkageManager::showDrivenGroupList(QList<DataProperty::ParameterGroup*> list)
	{
		ui->drivenGroupTableWidget->reset();
		//ui->drivenGroupTableWidget->clear();
		ui->drivenGroupTableWidget->setRowCount(list.count());
// 		ui->drivenGroupTableWidget->setColumnCount(2);
// 
// 		QStringList head;
// 		head.append(tr("group name"));
// 		head.append(tr("visible or not"));
// 
// 		ui->drivenGroupTableWidget->setHorizontalHeaderLabels(head);

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

// 		if (model->getParaType() == DataProperty::Para_Selectable)
// 		{
// 			DataProperty::parameterSelectable*
// 		}

		if (model->getParaType() == DataProperty::Para_Table)
		{
			int rows = ((DataProperty::ParameterTable*)model)->getRowCount();
			int columns = ((DataProperty::ParameterTable*)model)->getColumnCount();
			QString s = QString("%1X%2").arg(rows).arg(columns);
			value->setText(s);
		}
		else
			value->setText(model->valueToString());

		groupName->setFlags(groupName->flags() & (~Qt::ItemIsEditable));
		name->setFlags(name->flags() & (~Qt::ItemIsEditable));
		type->setFlags(type->flags() & (~Qt::ItemIsEditable));
		value->setFlags(value->flags() & (~Qt::ItemIsEditable));

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

		groupName->setFlags(groupName->flags() & (~Qt::ItemIsEditable));
		name->setFlags(name->flags() & (~Qt::ItemIsEditable));
		type->setFlags(type->flags() & (~Qt::ItemIsEditable));
		value->setFlags(value->flags() & (~Qt::ItemIsEditable));
		editable->setFlags(editable->flags() & (~Qt::ItemIsEditable));
		visiable->setFlags(visiable->flags() & (~Qt::ItemIsEditable));

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
//		qDebug() << model->getDescribe();
		groupName->setText(model->getDescribe());

		if (model->isVisible())
			visiable->setText("true");
		else
			visiable->setText("false");

		groupName->setFlags(groupName->flags() & (~Qt::ItemIsEditable)); 
		visiable->setFlags(visiable->flags() & (~Qt::ItemIsEditable));

		ui->drivenGroupTableWidget->setItem(row, 0, groupName);
		ui->drivenGroupTableWidget->setItem(row, 1, visiable);
	}


	void ParaLinkageManager::clearTableWidget()
	{
		ui->activeTableWidget->clearContents();
		ui->drivenTableWidget->clearContents();
		ui->drivenGroupTableWidget->clearContents();
	}

}

