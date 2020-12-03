#pragma once

#include <QDialog>
#include "Geant4DataModel.h"

namespace Ui {
class DialogAddVolumeInterface;
}

namespace Geant4 {
	class SolidShapeBase;
	class SolidTube;

	class DialogAddVolumeInterface : public QDialog
	{
		Q_OBJECT

	public:
		DialogAddVolumeInterface(QWidget * parent = nullptr, QString anaName = QString(), QVector<_G4VolumeInterface> volumeInterfaces = QVector<_G4VolumeInterface>());
		~DialogAddVolumeInterface();

		void setUsedVolumeInterface(QVector<_G4VolumeInterface> volumeInterfaces);

		QString getSelectedVolumeInterface();

		QString getListA();

		QString getListB();
	
	private:
		void init();
		void closeEvent(QCloseEvent *) override;
		void reject() override;
		void accept() override;

	private:
		Ui::DialogAddVolumeInterface *m_ui;
		QVector<_G4VolumeInterface> m_volumeInterfaces;
	};
}
