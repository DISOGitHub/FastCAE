#include "dialogTube.h"
#include "ui_dialogTube.h"
#include "SolidTube.h"

namespace Geant4 {
	DialogTube::DialogTube(SolidShapeBase* data) :m_tubeData(dynamic_cast<SolidTube*>(data))
	{
		m_ui = new Ui::DialogTube;
		m_ui->setupUi(this);

		Qt::WindowFlags flags = Qt::Dialog;
		flags |= Qt::WindowCloseButtonHint;
		setWindowFlags(flags);

		this->init();
	}

	DialogTube::~DialogTube()
	{
		delete m_ui;
	}

	void DialogTube::init()
	{
		QString name = m_tubeData->getName();
		QString lunit = m_tubeData->getParameterUnit();
		QString aunit = m_tubeData->getParameterAunit();

		m_ui->Name->setText(name);
		m_ui->lunitComboBox->setCurrentText(lunit);
		m_ui->aunitComboBox->setCurrentText(aunit);

		QStringList list;
		m_tubeData->getValue(list);

		m_ui->Rmin->setText(list.at(0));
		m_ui->Rmax->setText(list.at(1));
		m_ui->Z->setText(list.at(2));
		m_ui->Startphi->setText(list.at(3));
		m_ui->Deltaphi->setText(list.at(4));

	}

	void DialogTube::closeEvent(QCloseEvent *)
	{

	}

	void DialogTube::reject()
	{
		this->close();
	}

	void DialogTube::accept()
	{
		QString lunit = m_ui->lunitComboBox->currentText();
		m_tubeData->setParameterUnit(lunit);
		QString aunit = m_ui->aunitComboBox->currentText();
		m_tubeData->setParameterAunit(aunit);

		QString rmin = m_ui->Rmin->text();
		QString rmax = m_ui->Rmax->text();
		QString z = m_ui->Z->text();
		QString startphi = m_ui->Startphi->text();
		QString deltaphi = m_ui->Deltaphi->text();
	
		m_tubeData->setRmin(rmin);
		m_tubeData->setRmax(rmax);
		m_tubeData->setZValue(z);
		m_tubeData->setStartphi(startphi);
		m_tubeData->setDeltaphi(deltaphi);
		
		
		QDialog::accept();
		this->close();
	}

}