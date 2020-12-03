#include "ui_dialogMaterial.h"
#include "dialogMaterial.h"
#include "MaterialManager.h"
#include "DataManager.h"
#include "Value3D.h"
#include <QTableWidgetItem>
#include "GDMLMaterial.h"
#include <QDebug>
namespace Geant4
{

	CreateMaterialDialog::CreateMaterialDialog(GDMLMaterial* material):
		m_Material(DataManager::getInstance()->getMaterialManager())
	{
		_material = material;

		_ui = new Ui::DialogMaterial;
		_ui->setupUi(this);
		Qt::WindowFlags flags = Qt::Dialog;
		flags |= Qt::WindowCloseButtonHint;
		setWindowFlags(flags);
		setWindowTitle(QString(tr("Material Edit")));

		 fra_tableWidget = _ui->tableWidget;
		 DTP_tableWidget = _ui->tableWidget_2;
		Com_tablewidege = _ui->tableWidget_3;
		this->init();
		
	}

	void CreateMaterialDialog::init()
	{

		_ui->namelineEdit->setText(_material->getName());
		_ui->namelineEdit->setEnabled(false);
		_ui->namelineEdit_2->setText(_material->getFormula());
		_ui->namelineEdit_3->setText(_material->getZ());
		_ui->namelineEdit_4->setText(_material->getState());
		_ui->namelineEdit_5->setText(QString::number(_material->getValue()));


		DTP_tableWidget->verticalHeader()->setVisible(false);
		DTP_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		fra_tableWidget->verticalHeader()->setVisible(false);
		fra_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		Com_tablewidege->verticalHeader()->setVisible(false);
		Com_tablewidege->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

		int nN =_material->getMaterialTypeCount();
		if (nN > 0) 
		{
			DTP_tableWidget->setRowCount(nN);
			fillDTPTableWidget(nN);
		}
		int nfrac = _material->getFractionCount();
		if (nfrac > 0)
		{
			fra_tableWidget->setRowCount(nfrac);
			fillFraTableWidget(nfrac);
		}
		int comsize = _material->getCompositeCount();
		if (comsize > 0)
		{
			Com_tablewidege->setRowCount(comsize);
			fillComTableWidege(comsize);

		}
	}

	void CreateMaterialDialog::closeEvent(QCloseEvent *)
	{

	}

	void CreateMaterialDialog::reject()
	{
		this->close();
	}

	void CreateMaterialDialog::accept()
	{
		_material->clearMaterial();
		this->updateOneMaterial();
		QDialog::accept();
		this->close();
	}

	void CreateMaterialDialog::updateOneMaterial()
	{
		QString formula = _ui->namelineEdit_2->text();
		QString Z = _ui->namelineEdit_3->text();
		QString state = _ui->namelineEdit_4->text();
		double value = _ui->namelineEdit_5->text().toDouble();
		_material->setFormula(formula);
		_material->setZ(Z);
		_material->setState(state);
		_material->setValue(value);

		int frarow = fra_tableWidget->rowCount();
		for (int i = 0; i < frarow; i++)
		{
	
			Fraction fra;
			QString strN = fra_tableWidget->item(i, 0)->text();
			QString strRef = fra_tableWidget->item(i, 1)->text();
			fra.n = strN.toDouble();
			fra.ref = strRef;
			_material->appendFraction(fra);
		}

		int DTProw = DTP_tableWidget->rowCount();
		for (int i = 0; i < DTProw; i++)
		{
			MaterialType dtp;
			QString type = DTP_tableWidget->item(i, 0)->text();
			QString valuestr = DTP_tableWidget->item(i, 1)->text();
			QString unit = DTP_tableWidget->item(i, 2)->text();
			dtp.type = type;
			dtp.value = valuestr.toDouble();
			dtp.unit = unit;
			_material->appendMaterialType(dtp);
			
		}
		int comrow = Com_tablewidege->rowCount();
		for (int i = 0; i < comrow; i++)
		{
			Composite sit;
			QString strN = Com_tablewidege->item(i, 0)->text();
			QString strRef = Com_tablewidege->item(i, 1)->text();
			sit.n = strN.toDouble();
			sit.ref = strRef;
			_material->appendComposite(sit);

		}
	}

	void CreateMaterialDialog::fillFraTableWidget(int n)
	{
		for (int i = 0; i < n; i++)
		{
			Fraction onefra = _material->getFractionByIndex(i);
			QString ref = onefra.ref;
			QString nvalue = QString::number(onefra.n);

			QTableWidgetItem* nItem = new QTableWidgetItem;
			nItem->setText(nvalue);
			fra_tableWidget->setItem(i, 0, nItem);
			QTableWidgetItem* refItem = new QTableWidgetItem;
			refItem->setText(ref);
			fra_tableWidget->setItem(i, 1, refItem);
			
		}
	}

	void CreateMaterialDialog::fillComTableWidege(int n)
	{
		for (int i = 0; i < n; i++)
		{
			
			Composite onecom = _material->getCompositeByIndex(i);
			QString ref = onecom.ref;
			QString nvalue = QString::number(onecom.n);

			QTableWidgetItem* nItem = new QTableWidgetItem;
			nItem->setText(nvalue);
			Com_tablewidege->setItem(i, 0, nItem);
			QTableWidgetItem* refItem = new QTableWidgetItem;
			refItem->setText(ref);
			Com_tablewidege->setItem(i, 1, refItem);
			
		}
	}

	void CreateMaterialDialog::fillDTPTableWidget(int n)
	{
		
		for (int i = 0; i < n; i++)
		{
			MaterialType oneDTP = _material->getMaterialTypeByIndex(i);
			QString type = oneDTP.type;
			QString unit = oneDTP.unit;
			QString value = QString::number(oneDTP.value);
			
			QTableWidgetItem* TypeItem = new QTableWidgetItem;
			TypeItem->setText(type);
			DTP_tableWidget->setItem(i, 0, TypeItem);
			QTableWidgetItem* UnitItem = new QTableWidgetItem;
			UnitItem->setText(unit);
			DTP_tableWidget->setItem(i, 2, UnitItem);
			QTableWidgetItem* ValueItem = new QTableWidgetItem;
			ValueItem->setText(value);
			DTP_tableWidget->setItem(i, 1, ValueItem);
		}

	}

}