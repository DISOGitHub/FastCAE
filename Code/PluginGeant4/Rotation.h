#ifndef _ROTATION_H
#define _ROTATION_H

#include "PreDefineBase.h"

namespace Geant4 
{

	class Value3D;

	class Rotation: public PreDefineBase
	{
	public:
		Rotation();
		~Rotation();

		void setUnit(QString u);
		QString getUnit();

		void setValue(double* v);
		void getValue(double* v) override;

		void readGDML(QDomElement & ele) override;
		void writeGDML(QDomDocument& doc, QDomElement& ele) override;

		void getValue(QStringList& list);
		void setValue(QString x, QString y, QString z);

	private:
		Value3D* _value{};
		
	};
}
#endif // !_ROTATION_H



