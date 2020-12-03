#include "ParametersLinkage.h"
#include "ui_ParametersLinkage.h"
#include "DataProperty/ParameterBase.h"
#include "DataProperty/ParameterInt.h"
#include "DataProperty/ParameterBool.h"
#include "DataProperty/ParameterDouble.h"
#include "DataProperty/ParameterString.h" 
#include "DataProperty/ParameterSelectable.h"
#include "DataProperty/ParameterTable.h"
#include "DataProperty/DataBase.h"
#include "DataProperty/ParameterGroup.h"
#include "DataProperty/ParameterPath.h"
#include "DataManager.h"
#include "EditorTableValue.h"
#include "EditorPathValue.h"
#include "ParaLinkageData.h"
#include <QSignalMapper>
#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QTimer>

namespace FastCAEDesigner{
	ParametersLinkage::ParametersLinkage(ParaLinkageData* linkData, int type,QWidget *parent) :
		QDialog(parent),
		ui(new Ui::ParametersLinkage),
		_paraLinkData(linkData),
		_type(type)
	{
		ui->setupUi(this);
		init();
	}

	ParametersLinkage::~ParametersLinkage()
	{
		//clearAllParameterList();
		_activeList.clear();
		_drivenList.clear();
		_drivenGroupList.clear();
		_allParameterList.clear();
		_remainParameterList.clear();
		_allParameterGroupList.clear();
		delete ui;
	}

	void ParametersLinkage::init()
	{
//		qDebug() << _type;

		ui->parameterTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		ui->parameterTableWidget->setWindowFlags(Qt::FramelessWindowHint);
		ui->parameterTableWidget->setShowGrid(false);
		ui->parameterTableWidget->verticalHeader()->setVisible(false);

		ui->lastStepQPB->setVisible(false);
		ui->lastStepQPB->setText(tr("Last Step"));
		ui->nextStepQPB->setText(tr("Next Step"));

	//	initTreeComboBox();
		setLinkageVisable(true);
		setParameterTableVisable(false);

		updateDataToUi();

		connect(ui->lastStepQPB, SIGNAL(clicked()), this, SLOT(onLastStepQPBClicked()));
		connect(ui->nextStepQPB, SIGNAL(clicked()), this, SLOT(onNextStepQPBClicked()));

	}

	void ParametersLinkage::initActiveTableWidget()
	{
		ui->parameterType->setText(tr("Active"));
		ui->parameterTableWidget->reset();
		ui->parameterTableWidget->clear();
		ui->parameterTableWidget->setRowCount(_allParameterList.count());
		ui->parameterTableWidget->setColumnCount(4);

		QStringList head;
		head.append(tr("group name"));
		head.append(tr("parameter name"));
		head.append(tr("parameter type"));
		head.append(tr("value"));

		ui->parameterTableWidget->setHorizontalHeaderLabels(head);

		for (int i = 0; i < _allParameterList.count(); i++)
		{
			insertActiveTableWidget(i, _allParameterList.at(i));
		}

		//createTableWidget(_allParameterList.count(), 4);
	}

	void ParametersLinkage::initDrivenTableWidget()
	{
		ui->parameterType->setText(tr("Driven"));
		ui->parameterTableWidget->reset();
		ui->parameterTableWidget->clear();
		ui->parameterTableWidget->setRowCount(_remainParameterList.count());
		ui->parameterTableWidget->setColumnCount(6);

		QStringList head;
		head.append(tr("group name"));
		head.append(tr("parameter name"));
		head.append(tr("parameter type"));
		head.append(tr("value"));
		head.append(tr("editable or not"));
		head.append(tr("visible or not"));

		ui->parameterTableWidget->setHorizontalHeaderLabels(head);

		for (int i = 0; i < _remainParameterList.count(); i++)
		{
			insertDrivenTableWidget(i, _remainParameterList.at(i));
		}


		//refreshTableWidget(_remainParameterList.count(), 6, _remainParameterList);
	}

	void ParametersLinkage::initDrivenGroupTableWidget()
	{
		ui->parameterType->setText(tr("Driven Group"));

		ui->parameterTableWidget->reset();
		ui->parameterTableWidget->clear();
		ui->parameterTableWidget->setRowCount(_allParameterGroupList.count());
		ui->parameterTableWidget->setColumnCount(2);

		QStringList head;
		head.append(tr("group name"));
		head.append(tr("visible or not"));

		ui->parameterTableWidget->setHorizontalHeaderLabels(head);

		for (int i = 0; i < _allParameterGroupList.count(); i++)
		{
			insertDrivenGroupTableWidget(i, _allParameterGroupList.at(i));
		}

	}

	void ParametersLinkage::insertActiveTableWidget(int row, DataProperty::ParameterBase* model)
	{
		if (model == nullptr)
			return;

		//	QWidget* paraGroupName = getGroupNameWidget(row, model);
		QTableWidgetItem* paraGroupName = new QTableWidgetItem();
		QTableWidgetItem* paraName = new QTableWidgetItem();
		QTableWidgetItem* paraType = new QTableWidgetItem();

		if (getParameterState(_activeList, model))
			paraGroupName->setCheckState(Qt::Checked);
		else
			paraGroupName->setCheckState(Qt::Unchecked);

		if (model->getGroupName().isEmpty())
			paraGroupName->setText("0");
		else
			paraGroupName->setText(model->getGroupName());

		paraName->setText(model->getDescribe());
		paraType->setText(model->ParaTypeToString(model->getParaType()));

		QWidget* paraValue = getParameterValue(row, model);

		paraGroupName->setFlags(paraGroupName->flags() & (~Qt::ItemIsEditable));
		paraName->setFlags(paraName->flags() & (~Qt::ItemIsEditable));
		paraType->setFlags(paraType->flags() & (~Qt::ItemIsEditable));

		//ui->parameterTableWidget->setCellWidget(row, 0, paraGroupName);
		ui->parameterTableWidget->setItem(row, 0, paraGroupName);
		ui->parameterTableWidget->setItem(row, 1, paraName);
		ui->parameterTableWidget->setItem(row, 2, paraType);
		ui->parameterTableWidget->setCellWidget(row, 3, paraValue);

	}

	void ParametersLinkage::insertDrivenTableWidget(int row, DataProperty::ParameterBase* model)
	{
		if (model == nullptr)
			return;

		//	QWidget* paraGroupName = getGroupNameWidget(row, model);
		QTableWidgetItem* paraGroupName = new QTableWidgetItem();
		QTableWidgetItem* paraName = new QTableWidgetItem();
		QTableWidgetItem* paraType = new QTableWidgetItem();
		QComboBox* paraEdit = new QComboBox();
		QComboBox* paraVisiable = new QComboBox();

		if (getParameterState(_drivenList, model))
			paraGroupName->setCheckState(Qt::Checked);
		else
			paraGroupName->setCheckState(Qt::Unchecked);

		if (model->getGroupName().isEmpty())
			paraGroupName->setText("0");
		else
			paraGroupName->setText(model->getGroupName());

		paraName->setText(model->getDescribe());
		paraType->setText(model->ParaTypeToString(model->getParaType()));

		QWidget* paraValue = getParameterValue(row, model);

		paraEdit->insertItem(0, "true");
		paraEdit->insertItem(1, "false");
		paraVisiable->insertItem(0, "true");
		paraVisiable->insertItem(1, "false");

		if (model->isEditable())
			paraEdit->setCurrentIndex(0);
		else
			paraEdit->setCurrentIndex(1);

		if (model->isVisible())
			paraVisiable->setCurrentIndex(0);
		else
			paraVisiable->setCurrentIndex(1);

		paraGroupName->setFlags(paraGroupName->flags() & (~Qt::ItemIsEditable));
		paraName->setFlags(paraName->flags() & (~Qt::ItemIsEditable));
		paraType->setFlags(paraType->flags() & (~Qt::ItemIsEditable));

		//ui->parameterTableWidget->setCellWidget(row, 0, paraGroupName);
		ui->parameterTableWidget->setItem(row, 0, paraGroupName);
		ui->parameterTableWidget->setItem(row, 1, paraName);
		ui->parameterTableWidget->setItem(row, 2, paraType);
		ui->parameterTableWidget->setCellWidget(row, 3, paraValue);
		ui->parameterTableWidget->setCellWidget(row, 4, paraEdit);
		ui->parameterTableWidget->setCellWidget(row, 5, paraVisiable);

	}

	void ParametersLinkage::insertDrivenGroupTableWidget(int row, DataProperty::ParameterGroup* model)
	{
		QTableWidgetItem* groupName = new QTableWidgetItem();
		QComboBox* groupVisible = new QComboBox();

		if (getParameterGroupState(_drivenGroupList,model))
		//if (_drivenGroupList.contains(model))
			groupName->setCheckState(Qt::Checked);
		else
			groupName->setCheckState(Qt::Unchecked);

		//	groupName->setCheckState(Qt::Unchecked);
		groupName->setText(model->getDescribe());

		groupVisible->insertItem(0, "true");
		groupVisible->insertItem(1, "false");

		if (model->isVisible())
			groupVisible->setCurrentIndex(0);
		else
			groupVisible->setCurrentIndex(1);

		groupName->setFlags(groupName->flags() & (~Qt::ItemIsEditable));

		ui->parameterTableWidget->setItem(row, 0, groupName);
		ui->parameterTableWidget->setCellWidget(row, 1, groupVisible);
	}

	QWidget* ParametersLinkage::getGroupNameWidget(int row, DataProperty::ParameterBase* model)
	{
		QSignalMapper* checkMapper = new QSignalMapper(this);
		QHBoxLayout *h_box_layout = new QHBoxLayout();
		QWidget *widget = new QWidget();
		QCheckBox* checkGroupName = new QCheckBox();
		QLabel* groupName = new QLabel();

		if (model->getGroupName().isEmpty())
			groupName->setText("0");
		else
			groupName->setText(model->getGroupName());

		if (_page == 1)
		{
			if (_activeList.contains(model))
				checkGroupName->setChecked(true);
		}
		else if (_page == 2)
		{
			if (_drivenList.contains(model))
				checkGroupName->setChecked(true);
		}

		connect(checkGroupName, SIGNAL(clicked()), checkMapper, SLOT(map()));
		checkMapper->setMapping(checkGroupName, row);
		connect(checkMapper, SIGNAL(mapped(int)), this, SLOT(setParameterList(int)));

		// 		connect(checkGroupName, SIGNAL(clicked()), checkMapper, SLOT(map()));
		// 		checkMapper->setMapping(checkGroupName, model);
		// 		connect(checkMapper, SIGNAL(mapped(DataProperty::ParameterBase*)), this, SLOT(setParameterList(DataProperty::ParameterBase*)));

		h_box_layout->addWidget(checkGroupName);
		h_box_layout->addWidget(groupName);
		h_box_layout->setContentsMargins(0, 0, 0, 0);
		h_box_layout->setSpacing(1);
		widget->setLayout(h_box_layout);

		return widget;
	}

	QWidget* ParametersLinkage::getParameterValue(int row, DataProperty::ParameterBase* model)
	{
		QSignalMapper* signalMapper = new QSignalMapper(this);
		QHBoxLayout* h_box_layout = new QHBoxLayout();
		QWidget* widget = new QWidget();
		// 		QCheckBox* checkvalue = new QCheckBox();
		// 		QLabel* groupName = new QLabel();

		if (model->getParaType() == DataProperty::Para_Path)
		{
			QPushButton* paraQPB = new QPushButton();
			QLabel* paraQLabel = new QLabel();

			paraQPB->setText(tr("Value"));
			paraQLabel->setText(model->valueToString());

			connect(paraQPB, SIGNAL(clicked()), signalMapper, SLOT(map()));
			signalMapper->setMapping(paraQPB, row);
			connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(setParameterValue(int)));

			h_box_layout->addWidget(paraQLabel);
			h_box_layout->addWidget(paraQPB);
			h_box_layout->setStretch(0, 2);
			h_box_layout->setStretch(1, 1);
			h_box_layout->setContentsMargins(0, 0, 0, 0);
			h_box_layout->setSpacing(1);
			widget->setLayout(h_box_layout);
		}
		else if (model->getParaType() == DataProperty::Para_Table)
		{
			QPushButton* paraQPB = new QPushButton();
			QLabel* paraQLabel = new QLabel();
			QString value{};

			int r = ((DataProperty::ParameterTable*)model)->getRowCount();
			int c = ((DataProperty::ParameterTable*)model)->getColumnCount();

			value = QString("%1X%2").arg(r).arg(c);

			paraQPB->setText(tr("Value"));
			paraQLabel->setText(value);

			connect(paraQPB, SIGNAL(clicked()), signalMapper, SLOT(map()));
			signalMapper->setMapping(paraQPB, row);
			connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(setParameterValue(int)));

			h_box_layout->addWidget(paraQLabel);
			h_box_layout->addWidget(paraQPB);
			h_box_layout->setStretch(0, 2);
			h_box_layout->setStretch(1, 1);
			h_box_layout->setContentsMargins(0, 0, 0, 0);
			h_box_layout->setSpacing(1);
			widget->setLayout(h_box_layout);
		}
		else if (model->getParaType() == DataProperty::Para_Selectable)
		{
			int index = model->getDataID();
			QComboBox* valueQCB = new QComboBox();
			//DataProperty::ParameterSelectable*para = (DataProperty::ParameterSelectable*)model;

			//qDebug() << ((DataProperty::ParameterSelectable*)model)->getChinese();
			QStringList valueList = ((DataProperty::ParameterSelectable*)model)->getOption();
			//qDebug() << valueList;

			valueQCB->addItems(valueList);
			valueQCB->setCurrentIndex(((DataProperty::ParameterSelectable*)model)->getCurrentIndex());

			// 			for (int i = 0; i < valueList.count(); i++)
			// 			{
			// 				valueQCB->setItemText(i, valueList.at(i));
			// 			}

			widget = valueQCB;
		}
		else if (model->getParaType() == DataProperty::Para_Bool)
		{
			QComboBox* valueQCB = new QComboBox();

			valueQCB->insertItem(0, "true");
			valueQCB->insertItem(1, "false");

			if (model->valueToString() == "true")
				valueQCB->setCurrentIndex(0);
			else
				valueQCB->setCurrentIndex(1);

			widget = valueQCB;
		}
		else
		{
			QLineEdit* valueQLineEdit = new QLineEdit();
			valueQLineEdit->setText(model->valueToString());

			widget = valueQLineEdit;
		}


		// 		h_box_layout->addWidget(checkGroupName);
		// 		h_box_layout->addWidget(groupName);
		// 		h_box_layout->setContentsMargins(0, 0, 0, 0);
		// 		h_box_layout->setSpacing(1);
		// 		widget->setLayout(h_box_layout);

		return widget;
	}

	void ParametersLinkage::onLastStepQPBClicked()
	{
		switch (_page)
		{
		case 1:
			_page--;
			_allParameterList.clear();
			ui->lastStepQPB->setVisible(false);
			setLinkageVisable(true);
			setParameterTableVisable(false);
			break;
		case 2:
			_page--;
			//_activeList.clear();
			_remainParameterList.clear();
			initActiveTableWidget();
			break;
		case 3:
			_page--;
		//	_drivenList.clear();
			ui->nextStepQPB->setText(tr("Next Step"));
			initDrivenTableWidget();
			break;
		default:
			break;
		}
	}

	void ParametersLinkage::onNextStepQPBClicked()
	{
		switch (_page)
		{
		case 0:
			completeNameAndTypeConfiguration();
			break;
		case 1:
			completeActiveConfiguration();
			break;
			//qDebug() << _page;
		case 2:
			completeDrivenConfiguration();
			break;
			//qDebug() << _page;
		default:
			completeDrivenGrouopConfiguration();
			break;
		}
	}

	void ParametersLinkage::completeNameAndTypeConfiguration()
	{
		_name = ui->nameLineEdit->text().trimmed();
		//QString type = ui->treeQCB->currentText().trimmed();

		if (_name.isEmpty())
		{
			ui->errorText->setText(tr("The name of ParametersLinkage is empty."));
			ui->errorText->show();
			QTimer::singleShot(3000, this, SLOT(OnTimeout()));
			return;
		}
		if (_usedNameList.contains(_name))
		{
			ui->errorText->setText(tr("The name of ParametersLinkage is already existed."));
			ui->errorText->show();
			QTimer::singleShot(3000, this, SLOT(OnTimeout()));
			return;
		}

		_page++;
		ui->lastStepQPB->setVisible(true);

		setLinkageVisable(false);
		setParameterTableVisable(true);

		setAllParameterList();
		setAllGroupList();

		initActiveTableWidget();
		//qDebug() << _page;
	}

	void ParametersLinkage::completeActiveConfiguration()
	{
		setActiveList();
		setAllGroupList();
		if (_activeList.count() == 0)
		{
			ui->errorText->setText(tr("At least one active parameter must be configured."));
			ui->errorText->show();
			QTimer::singleShot(3000, this, SLOT(OnTimeout()));
			return;
		}
		_page++;
		getDrivenParameterList();
		getDrivenParameterGroupList();
		initDrivenTableWidget();
	}

	void ParametersLinkage::completeDrivenConfiguration()
	{
		setDrivenList();
		_page++;
		ui->nextStepQPB->setText(tr("Finish"));
		initDrivenGroupTableWidget();
	}

	void ParametersLinkage::completeDrivenGrouopConfiguration()
	{
		setDrivenGroupList();

		if ((_drivenList.count() == 0) && (_drivenGroupList.count() == 0))
		{
			ui->errorText->setText(tr("At least one follower parameter or follower parameters group must be configured."));
			ui->errorText->show();
			QTimer::singleShot(3000, this, SLOT(OnTimeout()));
			return;
		}
		updateUiToData();

		this->accept();
		close();
	}

	void ParametersLinkage::setLinkageVisable(bool b)
	{
		ui->paraLinkageName->setVisible(b);
		ui->nameLineEdit->setVisible(b);
		ui->associationTree->setVisible(false);
		ui->treeQCB->setVisible(false);
	}

	void ParametersLinkage::setParameterTableVisable(bool b)
	{
		ui->lastStepQPB->setVisible(b);
		ui->parameterType->setVisible(b);
		ui->parameterTableWidget->setVisible(b);
	}

	void ParametersLinkage::setAllParameterList()
	{
		_allParameterList.clear();
		
		QList<DataProperty::ParameterBase*> list = DataManager::getInstance()->getAllParameterList(_type);

		//	qDebug() << list.count();
		//	qDebug() << groupList.count();

		for (int i = 0; i < list.count(); i++)
		{
			copyData(list.at(i));
			// 			DataProperty::ParameterBase* data = new DataProperty::ParameterBase;
			// 			data->copy(list.at(i));
			// 			_allParameterList.append(data);
		}

	}

	void ParametersLinkage::setAllGroupList()
	{
		_allParameterGroupList.clear();
		QList<DataProperty::ParameterGroup*> groupList = DataManager::getInstance()->getAllParameterGroupList(_type);

		for (int i = 0; i < groupList.count(); i++)
		{
			DataProperty::ParameterGroup* data = new DataProperty::ParameterGroup;
			data->copy(groupList.at(i));
			_allParameterGroupList.append(data);
		}
	}

	void ParametersLinkage::copyData(DataProperty::ParameterBase* base)
	{
		if (base->getParaType() == DataProperty::Para_Int)
		{
			DataProperty::ParameterInt* data = new DataProperty::ParameterInt;
			data->setGroupName(base->getGroupName());
			data->copy(base);
			_allParameterList.append(data);
		}
		else if (base->getParaType() == DataProperty::Para_Double)
		{
			DataProperty::ParameterDouble* data = new DataProperty::ParameterDouble;
			data->setGroupName(base->getGroupName());
			data->copy(base);
			_allParameterList.append(data);
		}
		else if (base->getParaType() == DataProperty::Para_Bool)
		{
			DataProperty::ParameterBool* data = new DataProperty::ParameterBool;
			data->setGroupName(base->getGroupName());
			data->copy(base);
			_allParameterList.append(data);
		}
		else if (base->getParaType() == DataProperty::Para_Path)
		{
			DataProperty::ParameterPath* data = new DataProperty::ParameterPath;
			data->setGroupName(base->getGroupName());
			data->copy(base);
			_allParameterList.append(data);
		}
		else if (base->getParaType() == DataProperty::Para_Table)
		{
			DataProperty::ParameterTable* data = new DataProperty::ParameterTable;
			data->setGroupName(base->getGroupName());
			data->copy(base);
			_allParameterList.append(data);
		}
		else if (base->getParaType() == DataProperty::Para_Selectable)
		{
			DataProperty::ParameterSelectable* data = new DataProperty::ParameterSelectable;
			data->setGroupName(base->getGroupName());
			data->copy(base);
			_allParameterList.append(data);
		}
		else if (base->getParaType() == DataProperty::Para_String)
		{
			DataProperty::ParameterString* data = new DataProperty::ParameterString;
			data->setGroupName(base->getGroupName());
			data->copy(base);
			_allParameterList.append(data);
		}

	}

	bool ParametersLinkage::getParameterState(QList<DataProperty::ParameterBase*> list, DataProperty::ParameterBase* data)
	{
		if (list.contains(data))
			return true;

		for (int i = 0; i < list.count(); i++)
		{
			if (data->getParaType() == list.at(i)->getParaType())
			{
				if (QString::compare(data->getDescribe(), list.at(i)->getDescribe()) == 0)
				{
					if (QString::compare(data->getGroupName(), list.at(i)->getGroupName()) == 0)
					{
						repalceDataValue(data, list.at(i));
						return true;
					}
				}
			}
		}

		return false;
	}

	void ParametersLinkage::repalceDataValue(DataProperty::ParameterBase* o, DataProperty::ParameterBase* n)
	{

		if (o->getParaType() == DataProperty::Para_Table)
		{
			dynamic_cast<DataProperty::ParameterTable*>(o)->setData(dynamic_cast<DataProperty::ParameterTable*>(n)->getData());
		}
		else if (o->getParaType() == DataProperty::Para_Selectable)
		{
			dynamic_cast<DataProperty::ParameterSelectable*>(o)->setCurrentIndex(dynamic_cast<DataProperty::ParameterSelectable*>(n)->getCurrentIndex());
		}
		else
			o->setValueFromString(n->valueToString());

		if (_page == 2)
		{
			o->setEditable(n->isEditable());
			o->setVisible(n->isVisible());
		}
	}

	bool ParametersLinkage::getParameterGroupState(QList<DataProperty::ParameterGroup*> list, DataProperty::ParameterGroup* data)
	{
		if (list.contains(data))
			return true;

		for (int i = 0; i < list.count(); i++)
		{
			if (QString::compare(data->getDescribe(), list.at(i)->getDescribe()) == 0)
			{
				data->setVisible(list.at(i)->isVisible());
				return true;
			}
				
		}

		return false;
	}

	void ParametersLinkage::getDrivenParameterList()
	{
		for (int i = 0; i < _allParameterList.count(); i++)
		{
			if (!_activeList.contains(_allParameterList.at(i)))
				_remainParameterList.append(_allParameterList.at(i));
		}

		//return _remainParameterList;
	}

	void ParametersLinkage::getDrivenParameterGroupList()
	{
		for (int i = 0; i < _allParameterGroupList.size(); i++)
		{
			DataProperty::ParameterGroup* group = _allParameterGroupList.at(i);
			for (int j = 0; j < group->getParameterCount();j++)
			{
				if (getParameterState(_activeList, group->getParameterAt(j)))
					_allParameterGroupList.removeOne(group);
			}
		}
	}

	void ParametersLinkage::setActiveList()
	{
		_activeList.clear();
		for (int i = 0; i < _allParameterList.count(); i++)
		{
			if (ui->parameterTableWidget->item(i, 0)->checkState() == Qt::Checked)
			{
				setActiveParameterValue(i, _allParameterList.at(i));
				_activeList.append(_allParameterList.at(i));
			}
		}
	}

	void ParametersLinkage::setDrivenList()
	{
		_drivenList.clear();
		for (int i = 0; i < _remainParameterList.count(); i++)
		{
			if (ui->parameterTableWidget->item(i, 0)->checkState() == Qt::Checked)
			{
				setDrivenParameterValue(i, _remainParameterList.at(i));
				setDrivenParameterEditAndVisible(i, _remainParameterList.at(i));
				_drivenList.append(_remainParameterList.at(i));
			}
		}
	}

	void ParametersLinkage::setDrivenGroupList()
	{
		_drivenGroupList.clear();

		for (int i = 0; i < _allParameterGroupList.count(); i++)
		{
			if (ui->parameterTableWidget->item(i, 0)->checkState() == Qt::Checked)
			{
				QWidget* visible = ui->parameterTableWidget->cellWidget(i, 1);
				QComboBox* comboBoxVisible = (QComboBox*)visible;

				if (comboBoxVisible->currentText() == "true")
					_allParameterGroupList.at(i)->setVisible(true);
				else
					_allParameterGroupList.at(i)->setVisible(false);

				_drivenGroupList.append(_allParameterGroupList.at(i));
			}
		}
	}

	void ParametersLinkage::setActiveParameterValue(int row, DataProperty::ParameterBase* model)
	{
		if ((model->getParaType() == DataProperty::Para_Path) || (model->getParaType() == DataProperty::Para_Table));
		else if (model->getParaType() == DataProperty::Para_Selectable)
		{
			QWidget* widget = ui->parameterTableWidget->cellWidget(row, 3);
			QComboBox* comboBox = (QComboBox*)widget;

			/*if (model->valueToString() != comboBox->currentText())*/
			_allParameterList.at(row)->setValueFromString(comboBox->currentText());
			((DataProperty::ParameterSelectable*)_allParameterList.at(row))->setCurrentIndex(comboBox->currentIndex());
		}
		else if (model->getParaType() == DataProperty::Para_Bool)
		{
			QWidget* widget = ui->parameterTableWidget->cellWidget(row, 3);
			QComboBox* comboBox = (QComboBox*)widget;
//			qDebug() << comboBox->currentText();
			/*if (model->valueToString() != comboBox->currentText())*/
			_allParameterList.at(row)->setValueFromString(comboBox->currentText());
		}
		else
		{
			QWidget* widget = ui->parameterTableWidget->cellWidget(row, 3);
			QLineEdit* lineEdit = (QLineEdit*)widget;

			/*if (model->valueToString() != lineEdit->text())*/
			_allParameterList.at(row)->setValueFromString(lineEdit->text());
		}
	}

	void ParametersLinkage::setDrivenParameterValue(int row, DataProperty::ParameterBase* model)
	{
		if ((model->getParaType() == DataProperty::Para_Path) || (model->getParaType() == DataProperty::Para_Table));
		else if (model->getParaType() == DataProperty::Para_Selectable)
		{
			QWidget* widget = ui->parameterTableWidget->cellWidget(row, 3);
			QComboBox* comboBox = (QComboBox*)widget;

			/*if (model->valueToString() != comboBox->currentText())*/
			_remainParameterList.at(row)->setValueFromString(comboBox->currentText());
			((DataProperty::ParameterSelectable*)_remainParameterList.at(row))->setCurrentIndex(comboBox->currentIndex());
		}
		else if (model->getParaType() == DataProperty::Para_Bool)
		{
			QWidget* widget = ui->parameterTableWidget->cellWidget(row, 3);
			QComboBox* comboBox = (QComboBox*)widget;
//			qDebug() << comboBox->currentText();
			/*if (model->valueToString() != comboBox->currentText())*/
			_remainParameterList.at(row)->setValueFromString(comboBox->currentText());
		}
		else
		{
			QWidget* widget = ui->parameterTableWidget->cellWidget(row, 3);
			QLineEdit* lineEdit = (QLineEdit*)widget;

			/*if (model->valueToString() != lineEdit->text())*/
			_remainParameterList.at(row)->setValueFromString(lineEdit->text());
		}
	}

	void ParametersLinkage::setDrivenParameterEditAndVisible(int row, DataProperty::ParameterBase* model)
	{
		QWidget* edit = ui->parameterTableWidget->cellWidget(row, 4);
		QComboBox* comboBoxEdit = (QComboBox*)edit;

		QWidget* visible = ui->parameterTableWidget->cellWidget(row, 5);
		QComboBox* comboBoxVisible = (QComboBox*)visible;

		if (comboBoxEdit->currentText() == "true")
			_remainParameterList.at(row)->setEditable(true);
		else
			_remainParameterList.at(row)->setEditable(false);

		if (comboBoxVisible->currentText() == "true")
			_remainParameterList.at(row)->setVisible(true);
		else
			_remainParameterList.at(row)->setVisible(false);
	}

	void ParametersLinkage::updateUiToData()
	{
		_paraLinkData->setParameterName(_name);
		_paraLinkData->setParameterType(_type);
		_paraLinkData->setActiveList(_activeList);
		_paraLinkData->setDrivenList(_drivenList);
		_paraLinkData->setDrivenGroupList(_drivenGroupList);

//		qDebug() << _paraLinkData->getParameterName();
	}

	void ParametersLinkage::updateDataToUi()
	{
		if (_paraLinkData == nullptr)
			return;

		_name = _paraLinkData->getParameterName();
		ui->nameLineEdit->setText(_name);
		//	_type = _paraLinkData->getParameterType();
		_activeList = _paraLinkData->getActiveList();
		_drivenList = _paraLinkData->getDrivenList();
		_drivenGroupList = _paraLinkData->getDrivenGroupList();
	}


	void ParametersLinkage::OnTimeout()
	{
		ui->errorText->setText("");
		ui->errorText->hide();
	}

	ParaLinkageData* ParametersLinkage::getParameterLinkageData()
	{
		return _paraLinkData;
	}

	void ParametersLinkage::clearAllParameterList()
	{
		for (int i = 0; i < _allParameterList.count(); i++)
		{
			DataProperty::ParameterBase* data = _allParameterList.at(i);
			if (data == nullptr)
				continue;

			delete data;
			data = nullptr;
		}
		for (int i = 0; i < _allParameterGroupList.count(); i++)
		{
			DataProperty::ParameterGroup* data = _allParameterGroupList.at(i);
			if (data == nullptr)
				continue;

			delete data;
			data = nullptr;
		}
	}

	void ParametersLinkage::setUsedNameList(QList<QString> list)
	{
		_usedNameList = list;
	}

	//20200326 xuxinwei 创建参数信息列表
// 	void ParametersLinkage::createTableWidget(int row, int column)
// 	{
// 		ui->parameterTableWidget->reset();
// 		ui->parameterTableWidget->clear();
// 		ui->parameterTableWidget->setRowCount(row);
// 		ui->parameterTableWidget->setColumnCount(column);
// 
// 		QStringList head;
// 		head.append(tr("group name"));
// 		head.append(tr("parameter name"));
// 		head.append(tr("parameter type"));
// 		head.append(tr("value"));
// 
// 		ui->parameterTableWidget->setHorizontalHeaderLabels(head);
// 
// 		for (int i = 0; i < row; i++)
// 		{
// 			insertActiveTableWidget(i, _allParameterList.at(i));
// 		}
// 
// 	}

// 	void ParametersLinkage::refreshTableWidget(int row, int column, QList<DataProperty::ParameterBase *> list)
// 	{
// 		ui->parameterTableWidget->reset();
// 		ui->parameterTableWidget->clear();
// 		ui->parameterTableWidget->setRowCount(row);
// 		ui->parameterTableWidget->setColumnCount(column);
// 
// 		QStringList head;
// 		head.append(tr("group name"));
// 		head.append(tr("parameter name"));
// 		head.append(tr("parameter type"));
// 		head.append(tr("value"));
// 		head.append(tr("editable or not"));
// 		head.append(tr("visible or not"));
// 
// 		ui->parameterTableWidget->setHorizontalHeaderLabels(head);
// 
// 		for (int i = 0; i < row; i++)
// 		{
// 			insertDrivenTableWidget(i, _remainParameterList.at(i));
// 		}
// 	}

// 	void ParametersLinkage::setParameterList(DataProperty::ParameterBase* model)
// 	{
// 		if (_page == 1)
// 		{
// 			_activeList.append(model);
// 		}
// 	}
// 
// 	void ParametersLinkage::setParameterList(int index)
// 	{
// 		if (_page == 1)
// 		{
// 			if (_activeList.contains(_allParameterList.at(index)))
// 				_activeList.removeOne(_allParameterList.at(index));
// 			else
// 				_activeList.append(_allParameterList.at(index));
// 			qDebug() << _allParameterList.at(index)->ParaTypeToString(_allParameterList.at(index)->getParaType());
// 		}
// 		else if (_page == 2)
// 		{
// 			if (_drivenList.contains(_remainParameterList.at(index)))
// 				_drivenList.removeOne(_remainParameterList.at(index));
// 			else
// 				_drivenList.append(_remainParameterList.at(index));
// 		}
// 	}

// 	void ParametersLinkage::updateActiveTableWidget()
// 	{
// 
// 	}
// 
// 	void ParametersLinkage::updateDrivenTableWidget()
// 	{
// 
// 	}

	void ParametersLinkage::setParameterValue(int index)
	{
		if (_page == 1)
		{
			if (_allParameterList.at(index)->getParaType() == DataProperty::Para_Table)
			{
				EditorTableValue dlg(((DataProperty::ParameterTable*)_allParameterList.at(index)));
				dlg.exec();
			}
			else if (_allParameterList.at(index)->getParaType() == DataProperty::Para_Path)
			{
				EditorPathValue dlg(((DataProperty::ParameterPath*)_allParameterList.at(index)));
				dlg.setFileSuffixEnable(false);
				dlg.exec();
			}
			
		}
		else if (_page == 2)
		{
			if (_remainParameterList.at(index)->getParaType() == DataProperty::Para_Table)
			{
				EditorTableValue dlg(((DataProperty::ParameterTable*)_remainParameterList.at(index)));
				dlg.exec();
			}
			else if (_remainParameterList.at(index)->getParaType() == DataProperty::Para_Path)
			{
				EditorPathValue dlg(((DataProperty::ParameterPath*)_remainParameterList.at(index)));
				dlg.setFileSuffixEnable(false);
				dlg.exec();
			}
		}
	}

// 	void ParametersLinkage::initTreeComboBox()
// 	{
// 		QList<QString> tree = DataManager::getInstance()->getTreeList();
// 		for (int i = 0; i < tree.count(); i++)
// 		{
// 			ui->treeQCB->insertItem(i, tree.at(i));
// 		}
// 	}


}