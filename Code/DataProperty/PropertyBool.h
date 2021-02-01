/**********************
数据类型为bool的属性类
**********************/
#ifndef PROPERTYBOOL_H
#define PROPERTYBOOL_H

#include "DataPropertyAPI.h"
#include "PropertyBase.h"

namespace DataProperty
{
	class DATAPROPERTYAPI PropertyBool : public PropertyBase
	{
	public:
		//构造函数
		PropertyBool();
		//构造函数
		PropertyBool(QString name, bool val);
		~PropertyBool() = default;
		//设置值
		void setValue(bool v);
		//获取值
		bool getValue();
		//类型转换
		QVariant getVariant() override;

	private:
		bool _value;
	};
}

#endif