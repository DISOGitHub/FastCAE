#ifndef _VOLUME_H_
#define _VOLUME_H_

#include <QList>
#include <QString>
#include "DataProperty/DataBase.h"

class QDomElement;

namespace Geant4
{
	class Physvol;
	 
	class Volume : public DataProperty::DataBase
	{
	public:
		Volume() = default;
		~Volume() = default;

		QString getName();

		void setMaterialRef(QString m);
		QString getMaterialRef();

		void setSolidRef(QString ref);
		QString getSolidRef();

		int getPhysvolCount();
		Physvol* getPhysvolAt(int index);

		void readGDML(QDomElement& e);
		void writeGDML(QDomDocument& doc, QDomElement& ele);

	private:
		QString  _name{};
		QString _materialRef{};
		QString _solidRef{};

		QList<Physvol*> _physvols{};
	};
}



#endif