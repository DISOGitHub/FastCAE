#ifndef _SOLIDTUBE_H
#define _SOLIDTUBE_H

#include "SolidShapeBase.h"

namespace Geant4 {
	class Value;

	class SolidTube:public SolidShapeBase
	{
	public:
		SolidTube();
		~SolidTube();

		void setRmin(QString s);
		void setRmax(QString s);
		void setZValue(QString s);
		void setStartphi(QString s);
		void setDeltaphi(QString s);

		void getValue(QStringList& list);

		void generateShape() override;

		void setParameterUnit(QString unit) override;
		QString getParameterUnit() override;
		void setParameterAunit(QString unit);
		QString getParameterAunit();
		void getShapeParameter(double* p)override;

		void readGDML(QDomElement& ele) override;
		void writeGDML(QDomDocument& doc, QDomElement& ele) override;

	private:
		QString m_lunit;
		QString m_aunit;
		Value* m_rmin = nullptr;
		Value* m_rmax = nullptr;
		Value* m_z = nullptr;
		Value* m_startphi = nullptr;
		Value* m_deltaphi = nullptr;

	};
}
#endif // _SOLIDTUBE_H


