#ifndef _POSITION_H
#define _POSITION_H

#include "PreDefineBase.h"

namespace Geant4 
{
	class Value3D;
	class Value;

	class Position : public PreDefineBase
	{
	public:
		Position();
		~Position();

		void readGDML(QDomElement & ele) override;
		void writeGDML(QDomDocument& doc, QDomElement& ele) override;
		void setUnit(QString u);
		QString getUnit();

		void setValue(double* v);
		void getValue(double* v) override;
		
		void getValue(QStringList& list);
		void setValue(QString x, QString y, QString z);

	private:
		Value3D* _value{};

	
	};
}
#endif // _POSITION_H
