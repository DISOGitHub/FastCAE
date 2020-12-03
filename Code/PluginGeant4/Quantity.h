#ifndef _QUANTITY_H
#define _QUANTITY_H

#include "PreDefineBase.h"

namespace Geant4 
{
	class Value;

	class Quantity : public PreDefineBase
	{
	public:
		Quantity();
		~Quantity();

		void readGDML(QDomElement & ele) override;
		void writeGDML(QDomDocument& doc, QDomElement& ele) override;

		void setValue(double v);
		void setValue(QString s);
		void getValue(double* v) override;
		QString getValueString();

		void setUnit(QString u);
		QString getUnit();

		void setType(QString t);
		QString getType();

	private:
		Value* _value{};
	};
}
#endif // _QUANTITY_H

