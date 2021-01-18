#ifndef PROPERTYDOUBLE_H
#define PROPERTYDOUBLE_H

#include "DataPropertyAPI.h"
#include "PropertyBase.h"

namespace DataProperty
{
	class DATAPROPERTYAPI PropertyDouble : public PropertyBase
	{
	public:
		//构造函数
		PropertyDouble();
		//构造函数
		PropertyDouble(QString name, double val);
		~PropertyDouble() = default;
		//设置值
		void setValue(double v);
		//获取值
		double getValue();
		//类型转化
		QVariant getVariant() override;

	private:
		double _value;
	};
}

#endif
