#include "PropertyPoint.h"

namespace DataProperty
{
	PropertyPoint::PropertyPoint() : PropertyBase(Prop_Point)
	{
	}
	PropertyPoint::PropertyPoint(QString name, double x, double y, double z) : PropertyBase(Prop_Point)
	{
		_name = name;
		_coordinate[0] = x;
		_coordinate[1] = y;
		_coordinate[2] = z;
	}
	PropertyPoint::PropertyPoint(QString name, double* p) : PropertyBase(Prop_Point)
	{
		_name = name;
		_coordinate[0] = p[0];
		_coordinate[1] = p[1];
		_coordinate[2] = p[2];
	}
	void PropertyPoint::setValue(double x, double y, double z)
	{
		_coordinate[0] = x;
		_coordinate[1] = y;
		_coordinate[2] = z;
	}
	void PropertyPoint::setValue(double *p)
	{
		_coordinate[0] = p[0];
		_coordinate[1] = p[1];
		_coordinate[2] = p[2];
	}
	void PropertyPoint::getValue(double* c)
	{
		c[0] = _coordinate[0];
		c[1] = _coordinate[1];
		c[2] = _coordinate[2];
	}


}