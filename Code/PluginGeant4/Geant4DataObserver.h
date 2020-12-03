#pragma once
#include <QComboBox>
#include <QLineEdit>
namespace Geant4
{
	class Geant4DataObserver : public QObject
	{
		Q_OBJECT
	public:
		Geant4DataObserver(QWidget * wgt, QString sign, QString belong = QString());
		~Geant4DataObserver();
	};
}

