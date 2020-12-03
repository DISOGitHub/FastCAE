#include "dialogSphere.h"
#include "ui_dialogSphere.h"
#include "SolidSphere.h"
#include "Value.h"

namespace Geant4 {
	DialogSphere::DialogSphere(SolidShapeBase* data):m_sphereData(dynamic_cast<SolidSphere*>(data))
	{
		m_ui = new Ui::DialogSphere;
		m_ui->setupUi(this);

		Qt::WindowFlags flags = Qt::Dialog;
		flags |= Qt::WindowCloseButtonHint;
		setWindowFlags(flags);

		this->init();
	}

	DialogSphere::~DialogSphere()
	{
		delete m_ui;
	}

	void DialogSphere::init()
	{
		QString name = m_sphereData->getName();
		QString lunit = m_sphereData->getParameterUnit();
		QString aunit = m_sphereData->getParameterAunit();
		
		m_ui->Name->setText(name);
		m_ui->lunitComboBox->setCurrentText(lunit);
		m_ui->aunitComboBox->setCurrentText(aunit);

		Value* rmin = m_sphereData->getRmin();
		Value* rmax = m_sphereData->getRmax();
		Value* startphi = m_sphereData->getStartphi();
		Value* deltaphi = m_sphereData->getDeltaphi();
		Value* starttheta = m_sphereData->getStarttheta();
		Value* dletatheta = m_sphereData->getDeltatheta();

		QString min = rmin->getReference();
		if (min.isEmpty())
			m_ui->Rmin->setText(QString::number(rmin->getValue()));
		else
			m_ui->Rmin->setText(min);

		QString max = rmax->getReference();
		if (max.isEmpty())
			m_ui->Rmax->setText(QString::number(rmin->getValue()));
		else
			m_ui->Rmax->setText(max);

		QString sp = startphi->getReference();
		if (sp.isEmpty())
			m_ui->Startphi->setText(QString::number(rmin->getValue()));
		else
			m_ui->Startphi->setText(sp);

		QString dp = deltaphi->getReference();
		if (dp.isEmpty())
			m_ui->Deltaphi->setText(QString::number(rmin->getValue()));
		else
			m_ui->Deltaphi->setText(dp);

		QString st = starttheta->getReference();
		if (st.isEmpty())
			m_ui->Starttheta->setText(QString::number(rmin->getValue()));
		else
			m_ui->Starttheta->setText(st);

		QString dt = dletatheta->getReference();
		if (dt.isEmpty())
			m_ui->Deltatheta->setText(QString::number(rmin->getValue()));
		else
			m_ui->Deltatheta->setText(dt);
	}

	void DialogSphere::closeEvent(QCloseEvent *)
	{

	}

	void DialogSphere::reject()
	{
		this->close();
	}

	void DialogSphere::accept()
	{
		QString lunit = m_ui->lunitComboBox->currentText();
		m_sphereData->setParameterUnit(lunit);
		QString aunit = m_ui->aunitComboBox->currentText();
		m_sphereData->setParameterAunit(aunit);
		
		QString rmin = m_ui->Rmin->text();
		QString rmax = m_ui->Rmax->text();
		QString startphi = m_ui->Startphi->text();
		QString deltaphi = m_ui->Deltaphi->text();
		QString starttheta = m_ui->Starttheta->text();
		QString dletatheta = m_ui->Deltatheta->text();

		m_sphereData->setRmin(rmin);
		m_sphereData->setRmax(rmax);
		m_sphereData->setStartphi(startphi);
		m_sphereData->setDeltaphi(deltaphi);
		m_sphereData->setStarttheta(starttheta);
		m_sphereData->setDeltatheta(dletatheta);

		QDialog::accept();
		this->close();
	}

}