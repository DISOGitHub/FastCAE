#include "DialogThermalBoundary.h"
#include "ui_DialogThermalBoundary.h"
#include "ThermalBoundary.h"
#include "DataProperty/PropertyString.h"
#include "DataProperty/PropertyDouble.h"
#include "BoundaryTabTypeData.h"
#include "TFCouplingTree.h"
#include "DialogThermalTabType.h"
#include <qtreewidget.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qevent.h>

namespace ThermalFluid
{
	ThermalBoundaryDialog::ThermalBoundaryDialog(GUI::MainWindow* mw, ThermalBoundary* tb, TFCouplingTree* tree) :
		QFDialog(mw), _tree(tree), _currentTb(tb)
	{
		_ui = new Ui::tbDialog;
		_ui->setupUi(this);
		_ttDialog = new ThermalTabTypeDialog(mw, this);
		_ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
		_ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
		_ui->buttonBox->addButton(QDialogButtonBox::Apply)->setText(tr("Apply"));
		_ui->pbGetQValue->setStyleSheet("color: black;");
		_ui->pbGetQValue->setToolTip(tr("Get QValue Must Have Type TabTypeData And XValue"));
		connect(_ui->buttonBox, SIGNAL(accepted()), this, SLOT(tbBox_OkBt_ClickedSlot()));
		connect(_ui->buttonBox, SIGNAL(rejected()), this, SLOT(tbBox_CancelBt_ClickedSlot()));
		connect(_ui->buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()), this, SLOT(tbBox_ApplyBt_ClickedSlot()));
		connect(_ui->comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(currentIndexChangedSlot(QString)));
		connect(_ui->comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(createTabWidgetSlot(QString)));
	}

	ThermalBoundaryDialog::~ThermalBoundaryDialog()
	{
		delete _ui;
		_ui = nullptr;
		delete _ttDialog;
		_ttDialog = nullptr;
	}

	void ThermalBoundaryDialog::iniCurrentItemDlg()
	{
		setWindowTitle(tr("Edit This ThermalBoundary"));
		_isNew = false;
		_ui->labTbDisplayID->setText(QString::number(_currentTb->getID()));
		_ui->lineEditTb_Name->setText(_currentTb->getName());
		_ui->lineEditTb_Group->setText(_currentTb->getGroup());

		DataProperty::PropertyString* pStr;
		pStr = dynamic_cast<DataProperty::PropertyString*>(_currentTb->getPropertyByName(tr("tbType")));
		if (!pStr)
		{
			_ui->comboBox->setCurrentIndex(-1);
			_ui->labTbDisplayUnit->setText(QString(""));
		}
		else
		{
			QString type = pStr->getValue();
			int tbCount = _ui->comboBox->count();
			for (int i = 0; i < tbCount; i++)
			{
				if (type == _ui->comboBox->itemText(i))
				{
					_originalType = type;
					_ui->comboBox->setCurrentIndex(i);
					break;
				}
			}
		}

		int XValue = _currentTb->getXValue();
		_ui->lineEditTb_XValue->setText(QString::number(XValue));
		pStr = dynamic_cast<DataProperty::PropertyString*>(_currentTb->getPropertyByName(QObject::tr("sQValue")));
		if (!pStr)
		{
			auto pDouble = dynamic_cast<DataProperty::PropertyDouble*>(_currentTb->getPropertyByName(QObject::tr("dQValue")));
			_ui->labTbDisplayQValue->setText(QString::number(pDouble->getValue()));
		}
		else
			_ui->labTbDisplayQValue->setText(pStr->getValue());
	}

	void ThermalBoundaryDialog::iniEmptyDlg()
	{
		setWindowTitle(tr("Init This ThermalBoundary"));
		_isNew = true;
		_ui->labTbDisplayID->setText(QString::number(_currentTb->getID()));
		_ui->lineEditTb_Name->clear();
		_ui->lineEditTb_Group->clear();
		_ui->labTbDisplayQValue->clear();
		_ui->labTbDisplayUnit->setText(QString(""));
		_ui->comboBox->setCurrentIndex(-1);
		_ui->lineEditTb_XValue->clear();
	}

	QString ThermalBoundaryDialog::getComboxCurrentText()
	{
		if (_ui->comboBox->currentIndex() < 0)
			return QString("");
		else
			return _ui->comboBox->currentText();
	}

	ThermalBoundary* ThermalBoundaryDialog::getCurrentTb()
	{
		return _currentTb;
	}

	ThermalData* ThermalBoundaryDialog::getThermalData()
	{
		return _tree->_thermalData;
	}

	void ThermalBoundaryDialog::tbBox_OkBt_ClickedSlot()
	{
		if (!_hasSave)
		{
			tbBox_ApplyBt_ClickedSlot();
			if (_hasSave)	accept();
		}
		else accept();
	}

	void ThermalBoundaryDialog::tbBox_ApplyBt_ClickedSlot()
	{
		if (!tbDlgInformation())    return;
		setCurrentTbData();

		if (_isNew)
		{
			QTreeWidgetItem* aItem = new QTreeWidgetItem(_tree->_tBoundaryRoot, ThermalBoundaryChild);
			aItem->setText(0, QString("%1").arg(_currentTb->getName()));
			aItem->setData(0, Qt::UserRole, _tree->_thermalData->getThermalBoundaryListCount() - 1);
			_tree->_tBoundaryRoot->child(_tree->_thermalData->getThermalBoundaryListCount() - 1)->setSelected(true);
			_tree->_currentItem->setSelected(false);
		}
		else
		{
			_tree->_currentItem->setText(0, QString("%1").arg(_currentTb->getName()));
			_originalType = _ui->comboBox->currentText();
		}
		emit _tree->disPlayProp(_currentTb);
		_hasSave = true;
		_isNew = false;
	}

	void ThermalBoundaryDialog::tbBox_CancelBt_ClickedSlot()
	{
		if (_isNew)
			_tree->_thermalData->removeThermalBoundary(_currentTb);
		reject();
	}

	void ThermalBoundaryDialog::on_pbGetQValue_clicked()
	{	
		if (_ui->comboBox->currentText().isEmpty())
		{
			QMessageBox::information(this, tr("infomation"), tr("You Cant Calculate QValue Unless You Choose The Type!"));
			return;
		}
		else
		{
			if (_ttDialog->isVisible())
			{
				if (_ttDialog->hasEmptyItem())
				{
					QMessageBox::information(this, tr("infomation"), tr("You Cant Calculate QValue Unless You Fill The TabData!"));
					return;
				}				
			}		
		}
		if (_ui->lineEditTb_XValue->text().isEmpty())
		{
			QMessageBox::information(this, tr("infomation"), tr("You Cant Calculate QValue Unless You Fill The XValue!"));
			return;
		}

		ThermalBoundary* tb = _tree->iniAThermalBoundary();
		tb->setTypeAndUnit(_ui->comboBox->currentText());
		tb->setXValue(_ui->lineEditTb_XValue->text().toDouble());

		BoundaryTabTypeData* tabData{};
		if (_ttDialog->isVisible())
			tabData = _ttDialog->iniATabTypeDataFromTableWidget();
		tb->changeQValue(tb->getType(), tb->getXValue(), tabData);

		DataProperty::PropertyString* pStr = dynamic_cast<DataProperty::PropertyString*>(tb->getPropertyByName(QObject::tr("sQValue")));
		if (pStr)
			_ui->labTbDisplayQValue->setText(pStr->getValue());
		else
		{
			auto pDouble = dynamic_cast<DataProperty::PropertyDouble*>(tb->getPropertyByName(QObject::tr("dQValue")));
			if (pDouble)
				_ui->labTbDisplayQValue->setText(QString::number(pDouble->getValue()));
			else
				_ui->labTbDisplayQValue->setText(tr("NULL"));
		}
		delete tabData;
		tabData = nullptr;
	}

	bool ThermalBoundaryDialog::tbDlgInformation()
	{
		if (_ui->lineEditTb_Name->text().isEmpty())
		{
			QMessageBox::information(this, tr("infomation"), tr("Please Enter Name!"));
			return false;
		}
		if (_ui->lineEditTb_Group->text().isEmpty())
		{
			QMessageBox::information(this, tr("infomation"), tr("Please Enter Group!"));
			return false;
		}
		if (_ui->comboBox->currentText().isEmpty())
		{
			QMessageBox::information(this, tr("infomation"), tr("Please Enter Type!"));
			return false;
		}
		if (_ui->lineEditTb_XValue->text().isEmpty())
		{
			QMessageBox::information(this, tr("infomation"), tr("Please Enter XValue!"));
			return false;
		}

		if (_ttDialog->isVisible())
		{
			if (!_ttDialog->ttDlgInformation())	
				return false;
			if (_ttDialog->hasEmptyItem())
			{
				QMessageBox::information(this, tr("infomation"), tr("Please Enter Table Data!"));
				return false;
			}			
		}
		return true;
	}

	void ThermalBoundaryDialog::setCurrentTbData()
	{
		QString group = _ui->lineEditTb_Group->text();
		QString type = _ui->comboBox->currentText();
		QString XValue = _ui->lineEditTb_XValue->text();

		_currentTb->setName(_ui->lineEditTb_Name->text());
		_currentTb->setGroup(group);
		_currentTb->setTypeAndUnit(type);
		_currentTb->setXValue(XValue.toDouble());

		BoundaryTabTypeData* tabData{};
		if (_ttDialog->isVisible())
		{
			if (type == _originalType)
				tabData = _ttDialog->saveOriTabTypeData();
			else
				tabData = _ttDialog->saveNewTabTypeData();
		}
		_currentTb->changeQValue(_currentTb->getType(), _currentTb->getXValue(), tabData);

		DataProperty::PropertyString* pStr;
		pStr = dynamic_cast<DataProperty::PropertyString*>(_currentTb->getPropertyByName(tr("GroupName")));
		pStr->setValue(group);

		pStr = dynamic_cast<DataProperty::PropertyString*>(_currentTb->getPropertyByName(tr("tbType")));
		if (!pStr)
			_currentTb->appendProperty(tr("tbType"), type);
		else
			pStr->setValue(type);

		pStr = dynamic_cast<DataProperty::PropertyString*>(_currentTb->getPropertyByName(tr("Unit")));
		if (!pStr)
			_currentTb->appendProperty(tr("Unit"), _ui->labTbDisplayUnit->text());
		else
			pStr->setValue(_ui->labTbDisplayUnit->text());

		auto pDouble = dynamic_cast<DataProperty::PropertyDouble*>(_currentTb->getPropertyByName(tr("XValue")));
		pDouble->setValue(XValue.toDouble());

		pStr = dynamic_cast<DataProperty::PropertyString*>(_currentTb->getPropertyByName(QObject::tr("sQValue")));
		if (!pStr)
		{
			pDouble = dynamic_cast<DataProperty::PropertyDouble*>(_currentTb->getPropertyByName(QObject::tr("dQValue")));
			_ui->labTbDisplayQValue->setText(QString::number(pDouble->getValue()));
		}
		else
			_ui->labTbDisplayQValue->setText(pStr->getValue());
	}

	void ThermalBoundaryDialog::keyPressEvent(QKeyEvent *event)
	{
		switch (event->key())
		{
		case  Qt::Key_Escape:
			tbBox_CancelBt_ClickedSlot();
			break;
		default:
			QWidget::keyPressEvent(event);
			break;
		}
	}

	void ThermalBoundaryDialog::currentIndexChangedSlot(QString type)
	{
		QString text;
		if (type == tr("QLENG"))
			text = "W/m";
		else if (type == tr("QVOLU"))
			text = "W/m^3";
		else if (type == tr("ILENG"))
		{
			text = tr("times W/m");
			_ttDialog->setTabDataType(type);
		}
		else if (type == tr("IVOLU"))
		{
			text = tr("times W/m^3");
			_ttDialog->setTabDataType(type);
		}
		else if (type == tr("QAREA") || type == tr("IAREA"))
		{
			text = "W/m^2";
			if (type == tr("IAREA"))
				_ttDialog->setTabDataType(type);
		}
		else if (type == tr("BSINK") || type == tr("INULL"))
		{
			text = "C";
			if (type == tr("INULL"))
				_ttDialog->setTabDataType(type);
		}
		else if (type == tr("QTOTA") || type == tr("QNULL") || type == tr("ITOTA"))
		{
			text = "W";
			if (type == tr("ITOTA"))
				_ttDialog->setTabDataType(type);
		}
		else
			text = tr("NULL");
		_ui->labTbDisplayUnit->setText(text);
	}

	void ThermalBoundaryDialog::createTabWidgetSlot(QString type)
	{
		if (type == QObject::tr("IAREA")
			|| type == QObject::tr("ILENG") || type == QObject::tr("ITOTA")
			|| type == QObject::tr("IVOLU") || type == QObject::tr("INULL"))
		{
			if (type == _originalType)
				_ttDialog->writeTbDataToTable();
			else
			{
				_ttDialog->init(type);
				_ttDialog->writeEmptyDataToTable();
			}

			setMinimumSize(500, 600);
			resize(500, 600);
			_ui->tableLayout->addWidget(_ttDialog);
			_ttDialog->setVisible(true);
		}
		else
		{
			setMinimumSize(500, 250);
			resize(500, 250);
			_ui->tableLayout->removeWidget(_ttDialog);
			_ttDialog->setVisible(false);
		}
	}
}

