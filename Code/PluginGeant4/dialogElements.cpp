#include "ui_dialogElements.h"
#include "dialogElements.h"
#include "MaterialManager.h"
#include "DataManager.h"
#include "Value3D.h"
#include <QTableWidgetItem>
#include "GDMLElement.h"
#include "GDMLIsotope.h"
#include <qdebug.h>
namespace Geant4
{

	CreateElementsDialog::CreateElementsDialog(GDMLElement*ele)
	{
		_oneelement = ele;
		_ui = new Ui::DialogElement;
		_ui->setupUi(this);

		Qt::WindowFlags flags = Qt::Dialog;
		flags |= Qt::WindowCloseButtonHint;
		setWindowFlags(flags);
		setWindowTitle(QString(tr("Element Edit")));
		m_tableWidget = _ui->tableWidget;
		this->init();

	}

	void CreateElementsDialog::init()
	{
		_ui->namelineEdit->setText(_oneelement->getName());
		_ui->namelineEdit->setEnabled(false);
		_ui->namelineEdit_2->setText(_oneelement->getFormula());
		_ui->namelineEdit_3->setText(_oneelement->getElementZ());
		_ui->namelineEdit_4->setText(_oneelement->getType());
		_ui->namelineEdit_5->setText(QString::number(_oneelement->getValue()));
		_ui->namelineEdit_6->setText(_oneelement->getUnit());

		m_tableWidget->verticalHeader()->setVisible(false);
		m_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		int n = _oneelement->getEleFracList().size();
		if (n > 0) 
		{
			m_tableWidget->setRowCount(n);
			fillTableWidget(n);
		}
	}

	void CreateElementsDialog::closeEvent(QCloseEvent *)
	{
		
	}

	void CreateElementsDialog::reject()
	{
		this->close();
	}

	void CreateElementsDialog::accept()
	{
		_oneelement->clearElement();
		this->updateOneElement();
		QDialog::accept();
		this->close();
	}

	void CreateElementsDialog::updateOneElement()
	{
		QString formula = _ui->namelineEdit_2->text();
		QString Z = _ui->namelineEdit_3->text();
		QString type = _ui->namelineEdit_4->text();
		double value = _ui->namelineEdit_5->text().toDouble();
		QString unit = _ui->namelineEdit_6->text();
		_oneelement->setFormula(formula);
		_oneelement->setElementZ(Z);
		_oneelement->setType(type);
		_oneelement->setValue(value);
		_oneelement->setUnit(unit);

		int frarow = m_tableWidget->rowCount();
		for (int i = 0; i < frarow; i++)
		{
			Fraction fra;
			QString strN = m_tableWidget->item(i, 0)->text();
			QString strRef = m_tableWidget->item(i, 1)->text();
			fra.n = strN.toDouble();
			fra.ref = strRef;
			_oneelement->appendEleFracList(fra);
		}

	}

	void CreateElementsDialog::fillTableWidget(int n)
	{
		
		for (int i = 0; i < n; i++)
		{
			Fraction fra = _oneelement->getEleFracList().at(i);

			QTableWidgetItem* nItem = new QTableWidgetItem;
			nItem->setText(QString::number(fra.n));
			m_tableWidget->setItem(i, 0, nItem);

			QTableWidgetItem* refItem = new QTableWidgetItem;
			refItem->setText(fra.ref);
			m_tableWidget->setItem(i, 1, refItem);
		}

	}

}