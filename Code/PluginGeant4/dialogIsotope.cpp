#include "ui_dialogIsotope.h"
#include "dialogIsotope.h"
#include "MaterialManager.h"
#include "DataManager.h"
#include <QTableWidgetItem>
#include "GDMLElement.h"
#include "GDMLIsotope.h"
#include <qdebug.h>
namespace Geant4
{

	CreateIsotopeDialog::CreateIsotopeDialog(GDMLIsotope*ele)
	{
		_oneIsotope = ele;
		_ui = new Ui::DialogIsotope;
		_ui->setupUi(this);

		Qt::WindowFlags flags = Qt::Dialog;
		flags |= Qt::WindowCloseButtonHint;
		setWindowFlags(flags);
		setWindowTitle(QString(tr("Isotope Edit")));
		this->init();
	}


	void CreateIsotopeDialog::init()
	{
		_ui->namelineEdit->setText(_oneIsotope->getName());
		_ui->namelineEdit->setEnabled(false);
		_ui->namelineEdit_2->setText(_oneIsotope->getIsotopeN());
		_ui->namelineEdit_3->setText(_oneIsotope->getIsotopeZ());
		_ui->namelineEdit_4->setText(_oneIsotope->getType());
		_ui->namelineEdit_5->setText(QString::number(_oneIsotope->getValue()));
		_ui->namelineEdit_6->setText(_oneIsotope->getUnit());

	}

	void CreateIsotopeDialog::closeEvent(QCloseEvent *)
	{
		
	}

	void CreateIsotopeDialog::reject()
	{
		this->close();
	}

	void CreateIsotopeDialog::accept()
	{
		
		this->updateOneIsotope();
		QDialog::accept();
		this->close();
	}

	void CreateIsotopeDialog::updateOneIsotope()
	{
		QString N = _ui->namelineEdit_2->text();
		QString Z = _ui->namelineEdit_3->text();
		QString type = _ui->namelineEdit_4->text();
		double value = _ui->namelineEdit_5->text().toDouble();
		QString unit = _ui->namelineEdit_6->text();
		_oneIsotope->setIsotopeZ(Z);
		_oneIsotope->setIsotopeN(N);
		_oneIsotope->setType(type);
		_oneIsotope->setValue(value);
		_oneIsotope->setUnit(unit);
	}

}