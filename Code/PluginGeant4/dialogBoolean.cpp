#include "dialogBoolean.h"
#include "ui_dialogBoolean.h"
#include "SolidBoolean.h"
#include "SolidShapeBase.h"

namespace Geant4 {
	DialogBoolean::DialogBoolean(SolidShapeBase* data) :m_booleanData(dynamic_cast<SolidBoolean*>(data))
	{
		m_ui = new Ui::DialogBoolean;
		m_ui->setupUi(this);

		Qt::WindowFlags flags = Qt::Dialog;
		flags |= Qt::WindowCloseButtonHint;
		setWindowFlags(flags);

		QString tile = SolidShapeBase::TypeToString(data->getType());
		this->setWindowTitle(tile);		

		this->init();
	}

	DialogBoolean::~DialogBoolean()
	{
		delete m_ui;
	}

	void DialogBoolean::init()
	{
		QString name = m_booleanData->getName();
		QString first = m_booleanData->getFirst();
		QString second = m_booleanData->getSecond();
		m_ui->Name->setText(name);
		m_ui->First->setText(first);
		m_ui->Second->setText(second);

		displayPositionData();
		displayRotationData();
	}

	void DialogBoolean::closeEvent(QCloseEvent *)
	{

	}

	void DialogBoolean::reject()
	{
		this->close();
	}

	void DialogBoolean::accept()
	{
		QString first = m_ui->First->text();
		QString second = m_ui->Second->text();
		QStringList poslist;
		poslist.append(m_ui->positionRef->text());
		poslist.append(m_ui->positionName->text());
		poslist.append(m_ui->positionUnit->currentText());
		poslist.append(m_ui->positionX->text());
		poslist.append(m_ui->positionY->text());
		poslist.append(m_ui->positionZ->text());
		QStringList rotlist;
		rotlist.append(m_ui->rotationRef->text());
		rotlist.append(m_ui->rotationName->text());
		rotlist.append(m_ui->rotationUnit->currentText());
		rotlist.append(m_ui->rotationX->text());
		rotlist.append(m_ui->rotationY->text());
		rotlist.append(m_ui->rotationZ->text());

		m_booleanData->setFirst(first);
		m_booleanData->setSecond(second);
		m_booleanData->setPositionValue(poslist);
		m_booleanData->setRotationValue(rotlist);

		QDialog::accept();
		this->close();
	}

	void DialogBoolean::displayPositionData()
	{
		QStringList poslist;
		m_booleanData->getPositionValue(poslist);
		QString posref = poslist.at(0);
		if (!posref.isEmpty())
			m_ui->positionRef->setText(posref);
		else
		{
			m_ui->positionName->setText(poslist.at(1));
			m_ui->positionUnit->setCurrentText(poslist.at(2));
			m_ui->positionX->setText(poslist.at(3));
			m_ui->positionY->setText(poslist.at(4));
			m_ui->positionZ->setText(poslist.at(5));
		}
	}

	void DialogBoolean::displayRotationData()
	{
		QStringList rotlist;
		m_booleanData->getRotationValue(rotlist);
		QString rotref = rotlist.at(0);
		if (!rotref.isEmpty())
			m_ui->rotationRef->setText(rotref);
		else
		{
			m_ui->rotationName->setText(rotlist.at(1));
			m_ui->rotationUnit->setCurrentText(rotlist.at(2));
			m_ui->rotationX->setText(rotlist.at(3));
			m_ui->rotationY->setText(rotlist.at(4));
			m_ui->rotationZ->setText(rotlist.at(5));
		}
	}

}