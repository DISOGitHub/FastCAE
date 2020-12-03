#include "dialogAddVolumeInterface.h"
#include "ui_dialogAddVolumeInterface.h"
#include "Geant4DataModel.h"
#include "DataManager.h"
#include "SolidsManager.h"
#include "SolidShapeBase.h"
#include <QMessageBox>

namespace Geant4 {

	DialogAddVolumeInterface::DialogAddVolumeInterface(QWidget * parent /*= nullptr*/, QString anaName /*= QString()*/, QVector<_G4VolumeInterface> volumeInterfaces /*= QVector<_G4VolumeInterface>()*/)
	{
		m_ui = new Ui::DialogAddVolumeInterface();
		m_ui->setupUi(this);
		auto info = DataManager::getInstance()->getGeant4DataModel()->getAnalysisItems().value(anaName);
		auto solids = DataManager::getInstance()->getSolidsManager();
		int solidNum = solids->getSolidCount();

		for (int iCnt = 0; iCnt < solidNum; ++iCnt)
		{
			auto solidName = solids->getSolidAt(iCnt)->getName();
			m_ui->listA->addItem(solidName);
		}
		connect(m_ui->listA, &QListWidget::itemPressed, [=]() {

			m_ui->listB->clear();
			auto Aname = m_ui->listA->currentItem()->text();
			for (int iCnt = 0; iCnt < solidNum; ++iCnt)
			{
				auto solidName = solids->getSolidAt(iCnt)->getName();
				if (solidName == Aname)
				{
					continue;
				}
				bool alreadyHave = false;
				for (auto one : m_volumeInterfaces)
				{
					if (one.isEqual(Aname, solidName))
					{
						alreadyHave = true;
						break;
					}
				}
				if (!alreadyHave)
				{
					m_ui->listB->addItem(solidName);
				}
			}
		});
	}

	DialogAddVolumeInterface::~DialogAddVolumeInterface()
	{
		delete m_ui;
	}

	void DialogAddVolumeInterface::setUsedVolumeInterface(QVector<_G4VolumeInterface> volumeInterfaces)
	{
		m_volumeInterfaces = volumeInterfaces;
	}

	QString DialogAddVolumeInterface::getSelectedVolumeInterface()
	{
		return "";
	}

	QString DialogAddVolumeInterface::getListA()
	{
		return m_ui->listA->selectedItems().at(0)->text();
	}

	QString DialogAddVolumeInterface::getListB()
	{
		return m_ui->listB->selectedItems().at(0)->text();
	}

	void DialogAddVolumeInterface::init()
	{

	}

	void DialogAddVolumeInterface::closeEvent(QCloseEvent *)
	{

	}

	void DialogAddVolumeInterface::reject()
	{
		QDialog::reject();
	}

	void DialogAddVolumeInterface::accept()
	{
		if (m_ui->listA->selectedItems().isEmpty())
		{
			QMessageBox::information(this, "Warning", "You must chosen one in objectA...");
			return;
		}
		if (m_ui->listB->selectedItems().isEmpty())
		{
			QMessageBox::information(this, "Warning", "You must chosen one in objectB...");
			return;
		}
		QDialog::accept();
	}
}