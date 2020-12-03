#include "dialogAddVolume.h"
#include "ui_dialogAddVolume.h"
#include "DataManager.h"
#include "Geant4DataModel.h"
#include "SolidsManager.h"
#include "SolidShapeBase.h"

namespace Geant4 {

	DialogAddVolume::DialogAddVolume(QWidget * parent /*= nullptr*/, QString anaName /*= QString()*/)
	{
		m_ui = new Ui::DialogAddVolume();
		m_ui->setupUi(this);

		auto info = DataManager::getInstance()->getGeant4DataModel()->getAnalysisItems().value(anaName);
		auto solids = DataManager::getInstance()->getSolidsManager();
		int solidNum = solids->getSolidCount();

		for (int iCnt = 0; iCnt < solidNum; ++iCnt)
		{
			auto solidName = solids->getSolidAt(iCnt)->getName();
			if (!info.volumes.contains(solidName))
			{
				m_ui->list->addItem(solidName);
			}
		}
	}

	DialogAddVolume::~DialogAddVolume()
	{
		delete m_ui;
	}

	QString DialogAddVolume::getSelectedVolumeName()
	{
		return m_selectedVolumeName;
	}

	void DialogAddVolume::closeEvent(QCloseEvent *)
	{
		
	}

	void DialogAddVolume::reject()
	{
		QDialog::reject();
	}

	void DialogAddVolume::accept()
	{
		m_selectedVolumeName = m_ui->list->currentItem()->text();
		QDialog::accept();
	}

}