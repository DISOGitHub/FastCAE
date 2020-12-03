#ifndef _GDMLISOTOPE_H_
#define _GDMLISOTOPE_H_

#include "DataProperty/DataBase.h"
#include <QString>

class QDomElement;

namespace  Geant4
{
	class Value;

	class GDMLIsotope : public DataProperty::DataBase
	{
		Q_OBJECT
	public:
		GDMLIsotope();
		~GDMLIsotope();
		
		void setName(const QString& name) override;

		void setIsotopeN(QString N);
		QString getIsotopeN();

		void setIsotopeZ(QString Z);
		QString getIsotopeZ();

		void setType(QString type);
		QString getType();

		void setUnit(QString unit);
		QString getUnit();

		void setValue(double v);
		double getValue();

		void writeGDML(QDomDocument& doc, QDomElement& materils);
		void readGDML(QDomElement & ele);

	private:
		QString _IsotopeN{};
		QString  _IsotopeZ{};
		QString _type{};

		Value*  _value{};

	};
}


#endif