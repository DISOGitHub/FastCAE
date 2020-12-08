#ifndef PROPERTYDOUBLE_H
#define PROPERTYDOUBLE_H

#include "DataPropertyAPI.h"
#include "PropertyBase.h"

namespace DataProperty
{
	class DATAPROPERTYAPI PropertyDouble : public PropertyBase
	{
	public:
		//���캯��
		PropertyDouble();
		//���캯��
		PropertyDouble(QString name, double val);
		~PropertyDouble() = default;
		//����ֵ
		void setValue(double v);
		//��ȡֵ
		double getValue();
		//����ת��
		QVariant getVariant() override;

	private:
		double _value;
	};
}

#endif
