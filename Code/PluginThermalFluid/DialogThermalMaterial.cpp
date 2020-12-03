#include "DialogThermalMaterial.h"
#include "ui_DialogThermalMaterial.h"
#include "TFMaterial.h"
#include "ThermalData.h"
#include "DataProperty/PropertyDouble.h"
#include "TFCouplingTree.h"
#include <qtreewidget.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qevent.h>

namespace ThermalFluid
{
	ThermalMaterialDialog::ThermalMaterialDialog(GUI::MainWindow* mw, TFMaterial* tm, TFCouplingTree* tree) :
		QFDialog(mw), _tree(tree), _currentTm(tm)
	{
		_ui = new Ui::tmDialog;
		_ui->setupUi(this);

		_ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
		_ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
		_ui->buttonBox->addButton(QDialogButtonBox::Apply)->setText(tr("Apply"));

		connect(_ui->buttonBox, SIGNAL(accepted()), this, SLOT(tmBox_OkBt_ClickedSlot()));
		connect(_ui->buttonBox, SIGNAL(rejected()), this, SLOT(tmBox_CancelBt_ClickedSlot()));
		connect(_ui->buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()), this, SLOT(tmBox_ApplyBt_ClickedSlot()));
	}

	ThermalMaterialDialog::~ThermalMaterialDialog()
	{
		delete _ui;
		_ui = nullptr;
	}

	void ThermalMaterialDialog::iniCurrentItemDlg()
	{
		setWindowTitle(tr("Edit This ThermalMaterial"));
		_isNew = false;
		_ui->labTmDisplayID->setText(QString::number(_currentTm->getID()));
		_ui->lineEditTm_Name->setText(_currentTm->getName());
		_ui->lineEditTm_RHO->setText(QString::number(_currentTm->getRho()));
		_ui->lineEditTm_CPP->setText(QString::number(_currentTm->getCpp()));
		_ui->lineEditTm_KTHERM->setText(QString::number(_currentTm->getKtherm()));
		_ui->lineEditTm_E->setText(QString::number(_currentTm->getEmiss()));
		_ui->lineEditTm_ABSORP->setText(QString::number(_currentTm->getAbsorp()));
		_ui->lineEditTm_SPECULARITY->setText(QString::number(_currentTm->getSpecularity()));
		_ui->lineEditTm_REMISS->setText(QString::number(_currentTm->getRemiss()));
		_ui->lineEditTm_RABSOR->setText(QString::number(_currentTm->getRabsor()));
		_ui->lineEditTm_RSOLSPE->setText(QString::number(_currentTm->getRsolspe()));
	}

	void ThermalMaterialDialog::iniEmptyDlg()
	{
		setWindowTitle(tr("Init This ThermalMaterial"));
		_isNew = true;
		_ui->labTmDisplayID->setText(QString::number(_currentTm->getID()));
		_ui->lineEditTm_Name->clear();
		_ui->lineEditTm_RHO->clear();
		_ui->lineEditTm_CPP->clear();
		_ui->lineEditTm_KTHERM->clear();
		_ui->lineEditTm_E->clear();
		_ui->lineEditTm_ABSORP->clear();
		_ui->lineEditTm_SPECULARITY->clear();
		_ui->lineEditTm_REMISS->clear();
		_ui->lineEditTm_RABSOR->clear();
		_ui->lineEditTm_RSOLSPE->clear();
	}

	void ThermalMaterialDialog::tmBox_OkBt_ClickedSlot()
	{
		if (!_hasSave)
		{
			tmBox_ApplyBt_ClickedSlot();
			if (_hasSave)	accept();
		}
		else accept();
	}

	void ThermalMaterialDialog::tmBox_ApplyBt_ClickedSlot()
	{
		if (!tmDlgInformation())    return;
		setCurrentTmData();

		if (_isNew)
		{
			QTreeWidgetItem* aItem = new QTreeWidgetItem(_tree->_tMaterialRoot, ThermalMaterialChild);
			aItem->setText(0, QString("%1").arg(_currentTm->getName()));
			aItem->setData(0, Qt::UserRole, _tree->_thermalData->getMaterialListCount() - 1);
			_tree->_tMaterialRoot->child(_tree->_thermalData->getMaterialListCount() - 1)->setSelected(true);
			_tree->_currentItem->setSelected(false);
		}
		else
			_tree->_currentItem->setText(0, QString("%1").arg(_currentTm->getName()));
		emit _tree->disPlayProp(_currentTm);
		_hasSave = true;
		_isNew = false;
	}

	void ThermalMaterialDialog::tmBox_CancelBt_ClickedSlot()
	{
		if (_isNew)
			_tree->_thermalData->removeMaterial(_currentTm);
		reject();
	}

	bool ThermalMaterialDialog::tmDlgInformation()
	{
		if (_ui->lineEditTm_Name->text().isEmpty())
		{
			QMessageBox::information(this, tr("infomation"), tr("Please Enter Name!"));
			return false;
		}
		if (_ui->lineEditTm_RHO->text().isEmpty())
		{
			QMessageBox::information(this, tr("infomation"), tr("Please Enter RHO!"));
			return false;
		}
		if (_ui->lineEditTm_CPP->text().isEmpty())
		{
			QMessageBox::information(this, tr("infomation"), tr("Please Enter CPP!"));
			return false;
		}
		if (_ui->lineEditTm_KTHERM->text().isEmpty())
		{
			QMessageBox::information(this, tr("infomation"), tr("Please Enter KTHERM!"));
			return false;
		}
		if (_ui->lineEditTm_E->text().isEmpty())
		{
			QMessageBox::information(this, tr("infomation"), tr("Please Enter E!"));
			return false;
		}
		if (_ui->lineEditTm_ABSORP->text().isEmpty())
		{
			QMessageBox::information(this, tr("infomation"), tr("Please Enter ABSORP!"));
			return false;
		}
		if (_ui->lineEditTm_SPECULARITY->text().isEmpty())
		{
			QMessageBox::information(this, tr("infomation"), tr("Please Enter SPECULARITY!"));
			return false;
		}
		if (_ui->lineEditTm_REMISS->text().isEmpty())
		{
			QMessageBox::information(this, tr("infomation"), tr("Please Enter REMISS!"));
			return false;
		}
		if (_ui->lineEditTm_RABSOR->text().isEmpty())
		{
			QMessageBox::information(this, tr("infomation"), tr("Please Enter RABSOR!"));
			return false;
		}
		if (_ui->lineEditTm_RSOLSPE->text().isEmpty())
		{
			QMessageBox::information(this, tr("infomation"), tr("Please Enter RSOLSPE!"));
			return false;
		}
		return true;
	}

	void ThermalMaterialDialog::setCurrentTmData()
	{
		QString rho = _ui->lineEditTm_RHO->text();
		QString cpp = _ui->lineEditTm_CPP->text();
		QString ktherm = _ui->lineEditTm_KTHERM->text();
		QString e = _ui->lineEditTm_E->text();
		QString absorp = _ui->lineEditTm_ABSORP->text();
		QString specularity = _ui->lineEditTm_SPECULARITY->text();
		QString remiss = _ui->lineEditTm_REMISS->text();
		QString rabsor = _ui->lineEditTm_RABSOR->text();
		QString rsolspe = _ui->lineEditTm_RSOLSPE->text();

		_currentTm->setName(_ui->lineEditTm_Name->text());
		_currentTm->setRho(rho.toDouble());
		_currentTm->setCpp(cpp.toDouble());
		_currentTm->setKtherm(ktherm.toDouble());
		_currentTm->setEmiss(e.toDouble());
		_currentTm->setAbsorp(absorp.toDouble());
		_currentTm->setSpecularity(specularity.toDouble());
		_currentTm->setRemiss(remiss.toDouble());
		_currentTm->setRabsor(rabsor.toDouble());
		_currentTm->setRsolspe(rsolspe.toDouble());

		DataProperty::PropertyDouble* pDouble;
		pDouble = dynamic_cast<DataProperty::PropertyDouble*>(_currentTm->getPropertyByName(QObject::tr("RHO")));
		pDouble->setValue(rho.toDouble());
		pDouble = dynamic_cast<DataProperty::PropertyDouble*>(_currentTm->getPropertyByName(QObject::tr("CPP")));
		pDouble->setValue(cpp.toDouble());
		pDouble = dynamic_cast<DataProperty::PropertyDouble*>(_currentTm->getPropertyByName(QObject::tr("KTHERM")));
		pDouble->setValue(ktherm.toDouble());
		pDouble = dynamic_cast<DataProperty::PropertyDouble*>(_currentTm->getPropertyByName(QObject::tr("E")));
		pDouble->setValue(e.toDouble());
		pDouble = dynamic_cast<DataProperty::PropertyDouble*>(_currentTm->getPropertyByName(QObject::tr("ABSORP")));
		pDouble->setValue(absorp.toDouble());
		pDouble = dynamic_cast<DataProperty::PropertyDouble*>(_currentTm->getPropertyByName(QObject::tr("SPECULARITY")));
		pDouble->setValue(specularity.toDouble());
		pDouble = dynamic_cast<DataProperty::PropertyDouble*>(_currentTm->getPropertyByName(QObject::tr("REMISS")));
		pDouble->setValue(remiss.toDouble());
		pDouble = dynamic_cast<DataProperty::PropertyDouble*>(_currentTm->getPropertyByName(QObject::tr("RABSOR")));
		pDouble->setValue(rabsor.toDouble());
		pDouble = dynamic_cast<DataProperty::PropertyDouble*>(_currentTm->getPropertyByName(QObject::tr("RSOLSPE")));
		pDouble->setValue(rsolspe.toDouble());
	}

	void ThermalMaterialDialog::keyPressEvent(QKeyEvent *event)
	{
		switch (event->key())
		{
		case  Qt::Key_Escape:
			tmBox_CancelBt_ClickedSlot();
			break;
		default:
			QWidget::keyPressEvent(event);
			break;
		}
	}
}