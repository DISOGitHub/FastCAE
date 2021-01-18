#ifndef AIRCRAFTMANAGER_H_
#define AIRCRAFTMANAGER_H_

#include <QObject>


namespace Aircraft
{


	class AircraftManager : public QObject
	{
		Q_OBJECT
	public:
		AircraftManager() = default;
		~AircraftManager() = default;

		
		void clear();

	private:
		
		/*QList<GDMLIsotope*> _isotopelist{};
		QList<GDMLElement*> _elelist{};
		QList<GDMLMaterial*> _matlist{};*/
	};
}



#endif
