#include "DialogThermalCoupling.h"
#include "ui_DialogThermalCoupling.h"
#include "ThermalCoupling.h"
#include "DataProperty/PropertyString.h"
#include "DataProperty/PropertyDouble.h"
#include "TFCouplingTree.h"
#include "ThermalData.h"
#include <qtreewidget.h>
#include <qmessagebox.h>
#include <qevent.h>
#include <qpushbutton.h>

namespace ThermalFluid
{
	ThermalCouplingDialog::ThermalCouplingDialog(GUI::MainWindow* mw, ThermalCoupling* _currentTc, TFCouplingTree* tree) :
		QFDialog(mw), _tree(tree), _currentTc(_currentTc)
	{
		_ui = new Ui::tcDialog;
		_ui->setupUi(this);
		_ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
		_ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
		_ui->buttonBox->addButton(QDialogButtonBox::Apply)->setText(tr("Apply"));

		connect(_ui->buttonBox, SIGNAL(accepted()), this, SLOT(tcBox_OkBt_ClickedSlot()));
		connect(_ui->buttonBox, SIGNAL(rejected()), this, SLOT(tcBox_CancelBt_ClickedSlot()));
		connect(_ui->buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()), this, SLOT(tcBox_ApplyBt_ClickedSlot()));
	}

	ThermalCouplingDialog::~ThermalCouplingDialog()
	{
		delete _ui;
		_ui = nullptr;
	}

	void ThermalCouplingDialog::iniCurrentItemDlg()
	{
		setWindowTitle(tr("Edit This ThermalCoupling"));
		_isNew = false;
		_ui->labTcDisplayID->setText(QString::number(_currentTc->getID()));
		_ui->lineEditTc_Name->setText(_currentTc->getName());
		_ui->lineEditGroup1->setText(_currentTc->getGroupOne());
		_ui->lineEditGroup2->setText(_currentTc->getGroupTwo());
		_ui->lineEditTc_XValue->setText(QString::number(_currentTc->getXValue()));
		_ui->lineEditTc_Coe->setText(QString::number(_currentTc->getCoe()));

		DataProperty::PropertyString* pStr = dynamic_cast<DataProperty::PropertyString*>(_currentTc->getPropertyByName(QObject::tr("tcType")));
		int tcCount = _ui->comboBox->count();
		for (int i = 0; i < tcCount; i++)
		{
			if (pStr->getValue() == _ui->comboBox->itemText(i))
			{
				_ui->comboBox->setCurrentIndex(i);
				break;
			}
		}
	}

	void ThermalCouplingDialog::iniEmptyDlg()
	{
		setWindowTitle(tr("Init This ThermalCoupling"));
		_isNew = true;
		_ui->labTcDisplayID->setText(QString::number(_currentTc->getID()));
		_ui->lineEditTc_Name->clear();
		_ui->lineEditGroup1->clear();
		_ui->lineEditGroup2->clear();
		_ui->lineEditTc_XValue->clear();
		_ui->lineEditTc_Coe->clear();
		_ui->comboBox->setCurrentIndex(-1);
	}

	void ThermalCouplingDialog::tcBox_OkBt_ClickedSlot()
	{		
		if (!_hasSave)
		{
			tcBox_ApplyBt_ClickedSlot();
			if (_hasSave)	accept();
		}
		else accept();
	}

	void ThermalCouplingDialog::tcBox_ApplyBt_ClickedSlot()
	{
		if (!tcDlgInformation())    return;
		setCurrentTcData();

		if (_isNew)
		{
			QTreeWidgetItem* aItem = new QTreeWidgetItem(_tree->_tCouplingRoot, ThermalCouplingChild);
			aItem->setText(0, QString("%1").arg(_currentTc->getName()));
			aItem->setData(0, Qt::UserRole, _tree->_thermalData->getThermalCouplingListCount() - 1);
			_tree->_tCouplingRoot->child(_tree->_thermalData->getThermalCouplingListCount() - 1)->setSelected(true);
			_tree->_currentItem->setSelected(false);
		}
		else
			_tree->_currentItem->setText(0, QString("%1").arg(_currentTc->getName()));
		emit _tree->disPlayProp(_currentTc);
		_hasSave = true;
		_isNew = false;
	}

	void ThermalCouplingDialog::tcBox_CancelBt_ClickedSlot()
	{
		if (_isNew)
			_tree->_thermalData->removeThermalCoupling(_currentTc);
		reject();
	}

	void ThermalCouplingDialog::setCurrentTcData()
	{
		QString group1 = _ui->lineEditGroup1->text();
		QString group2 = _ui->lineEditGroup2->text();
		QString type = _ui->comboBox->currentText();
		QString xValue = _ui->lineEditTc_XValue->text();
		QString coe = _ui->lineEditTc_Coe->text();

		_currentTc->setName(_ui->lineEditTc_Name->text());
		_currentTc->setGroupOne(group1);
		_currentTc->setGroupTwo(group2);
		_currentTc->setType(_currentTc->stringToCouplingType(type));
		_currentTc->setXValue(xValue.toDouble());
		_currentTc->setCoe(coe.toDouble());

		DataProperty::PropertyString* pStr;
		pStr = dynamic_cast<DataProperty::PropertyString*>(_currentTc->getPropertyByName(QObject::tr("GroupOne")));
		pStr->setValue(group1);
		pStr = dynamic_cast<DataProperty::PropertyString*>(_currentTc->getPropertyByName(QObject::tr("GroupTwo")));
		pStr->setValue(group2);
		pStr = dynamic_cast<DataProperty::PropertyString*>(_currentTc->getPropertyByName(QObject::tr("tcType")));
		pStr->setValue(type);

		DataProperty::PropertyDouble* pDouble;
		pDouble = dynamic_cast<DataProperty::PropertyDouble*>(_currentTc->getPropertyByName(QObject::tr("Computing coefficients")));
		pDouble->setValue(xValue.toDouble());
		pDouble = dynamic_cast<DataProperty::PropertyDouble*>(_currentTc->getPropertyByName(QObject::tr("coefficient of heat transfer")));
		pDouble->setValue(coe.toDouble());
	}

	void ThermalCouplingDialog::keyPressEvent(QKeyEvent *event)
	{
		switch (event->key())
		{
		case  Qt::Key_Escape:
			tcBox_CancelBt_ClickedSlot();
			break;
		default:
			QWidget::keyPressEvent(event);
			break;
		}
	}

	bool ThermalCouplingDialog::tcDlgInformation()
	{
		if (_ui->lineEditTc_Name->text().isEmpty())
		{
			QMessageBox::information(this, tr("infomation"), tr("Please Enter Name!"));
			return false;
		}
		if (_ui->lineEditGroup1->text().isEmpty())
		{
			QMessageBox::information(this, tr("infomation"), tr("Please Enter Group1!"));
			return false;
		}
		if (_ui->lineEditGroup2->text().isEmpty())
		{
			QMessageBox::information(this, tr("infomation"), tr("Please Enter Group2!"));
			return false;
		}
		if (_ui->comboBox->currentText().isEmpty())
		{
			QMessageBox::information(this, tr("infomation"), tr("Please Enter Type!"));
			return false;
		}
		if (_ui->lineEditTc_XValue->text().isEmpty())
		{
			QMessageBox::information(this, tr("infomation"), tr("Please Enter xValue!"));
			return false;
		}
		if (_ui->lineEditTc_Coe->text().isEmpty())
		{
			QMessageBox::information(this, tr("infomation"), tr("Please Enter Coe!"));
			return false;
		}
		return true;
	}
}


