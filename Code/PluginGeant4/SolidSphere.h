#ifndef _SOLIDSPHERE_H
#define _SOLIDSPHERE_H

#include "SolidShapeBase.h"

namespace Geant4 {
	class Value;

	class SolidSphere:public SolidShapeBase
	{
	public:
		SolidSphere();
		~SolidSphere();		

		void setRmin(QString s);
		void setRmax(QString s);
		void setStartphi(QString s);
		void setDeltaphi(QString s);
		void setStarttheta(QString s);
		void setDeltatheta(QString s);
		Value* getRmin();
		Value* getRmax();
		Value* getStartphi();
		Value* getDeltaphi();
		Value* getStarttheta();
		Value* getDeltatheta();

		void generateShape() override;

		void setParameterUnit(QString unit) override;
		QString getParameterUnit() override;
		void setParameterAunit(QString unit);
		QString getParameterAunit();
		void getShapeParameter(double* p)override;

		void readGDML(QDomElement& ele) override;
		void writeGDML(QDomDocument& doc, QDomElement& ele) override;

	
	private:
		Value* m_rmin;
		Value* m_rmax;
		Value* m_startphi;
		Value* m_deltaphi;
		Value* m_starttheta;
		Value* m_deltatheta;
		QString m_lunit;
		QString m_aunit;
	};
}
#endif

