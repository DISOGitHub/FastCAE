#ifndef ASSEMBLY_H_
#define ASSEMBLY_H_

#include <QList>

class QDomElement;


namespace Geant4
{
	class Physvol;

	class Assembly
	{
	public:
		Assembly() = default;
		~Assembly() = default;

		void readGDML(QDomElement& ele);
		QString getName();

	private:
		QString _name{};
		QList<Physvol*> _physvols{};
	};
}


#endif
