#ifndef PROPERTYPOINT_H
#define PROPERTYPOINT_H

#include "DataPropertyAPI.h"
#include "PropertyBase.h"

namespace DataProperty
{
	class DATAPROPERTYAPI PropertyPoint : public PropertyBase
	{
	public:
		PropertyPoint();
		PropertyPoint(QString name, double* p);
		PropertyPoint(QString name, double x,double y, double z);
		~PropertyPoint() = default;

		void setValue(double *c);
		void setValue(double x, double y, double z);
		void getValue(double* c);

	private:
		double _coordinate[3];
	};
}

#endif