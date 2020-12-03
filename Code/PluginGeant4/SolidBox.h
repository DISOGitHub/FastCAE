#ifndef _BOXDATA_H
#define _BOXDATA_H

#include "SolidShapeBase.h"


namespace Geant4 
{
	class Value;
	class Value3D;

	class SolidBox : public SolidShapeBase
	{
	public:
		SolidBox();
		~SolidBox();

		void setXValue(QString s);
		void setYValue(QString s);
		void setZValue(QString s);
		Value* getXValue();
		Value* getYValue();
		Value* getZValue();

		void generateShape() override;
		
		void setParameterUnit(QString unit) override;
		QString getParameterUnit() override;
		void setShapeParameter(double* p) override;
		void getShapeParameter(double* p)override;

		void readGDML(QDomElement& ele) override;
		void writeGDML(QDomDocument& doc, QDomElement& ele) override;

	private:
		Value3D* _parameter;
		Value* m_xValue;
		Value* m_yValue;
		Value* m_zValue;
		QString m_unit;
	};
}

#endif // _BOXDATA_H


