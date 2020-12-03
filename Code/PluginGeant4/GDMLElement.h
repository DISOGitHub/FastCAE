#ifndef _GDMLELEMENT_H_
#define _GDMLELEMENT_H_

#include "DataProperty/DataBase.h"
#include <QString>
#include "GDMLMaterial.h"

namespace  Geant4
{
	class Value;

	class GDMLElement : public DataProperty::DataBase
	{
		Q_OBJECT
	public:
		GDMLElement();
		~GDMLElement();
		
		void setName(const QString& name)override;

		void setFormula(QString formula);
		QString getFormula();

		void setElementZ(QString Z);
		QString getElementZ();

		void setType(QString type);
		QString getType();

		void setUnit(QString unit);
		QString getUnit();

		void setValue(double v);
		double getValue();

		QList<Fraction> getEleFracList();
		void appendEleFracList(Fraction frac);

		void clearElement();

		void writeGDML(QDomDocument& doc, QDomElement& materils);
		void readGDML(QDomElement& ele);

	private:
		QString _formula{};
		QString  _ElementZ{};
		QString _type{};

		Value* _value{};
		QList<Fraction> _elefractionlist{};

	};
}


#endif