#include "dialogBox.h"
#include "ui_dialogBox.h"
#include "DataManager.h"
#include "SolidsManager.h"
#include "SolidShapeBase.h"
#include "SolidBox.h"
#include "Value.h"


namespace Geant4 
{
	DialogBox::DialogBox(SolidShapeBase* data) : m_boxData(dynamic_cast<SolidBox*>(data))
	{
		m_ui = new Ui::DialogBox;
		m_ui->setupUi(this);
		
		Qt::WindowFlags flags = Qt::Dialog;
		flags |= Qt::WindowCloseButtonHint;
		setWindowFlags(flags);
			
		this->init();
	}

	DialogBox::~DialogBox()
	{
		delete m_ui;
	}

	void DialogBox::init()
	{
		m_ui->name->setText(m_boxData->getName());
		QString unit = m_boxData->getParameterUnit();
		m_ui->unitComboBox->setCurrentText(unit);

		Value* xvalue = m_boxData->getXValue();
		Value* yvalue = m_boxData->getYValue();
		Value* zvalue = m_boxData->getZValue();

		QString x = xvalue->getReference();
		if (x.isEmpty())
			m_ui->xValue->setText(QString::number(xvalue->getValue()));
		else
			m_ui->xValue->setText(x);

		QString y = yvalue->getReference();
		if (y.isEmpty())
			m_ui->yValue->setText(QString::number(yvalue->getValue()));
		else
			m_ui->yValue->setText(y);

		QString z = zvalue->getReference();
		if (z.isEmpty())
			m_ui->zValue->setText(QString::number(zvalue->getValue()));
		else
			m_ui->zValue->setText(z);
	}

	void DialogBox::closeEvent(QCloseEvent *)
	{

	}

	void DialogBox::reject()
	{
		this->close();
	}

	void DialogBox::accept()
	{
		QString unit = m_ui->unitComboBox->currentText();
		m_boxData->setParameterUnit(unit);
		
		QString x = m_ui->xValue->text();
		QString y = m_ui->yValue->text();
		QString z = m_ui->zValue->text();
		
		m_boxData->setXValue(x);
		m_boxData->setYValue(y);
		m_boxData->setZValue(z);

		QDialog::accept();
		this->close();
	}



}