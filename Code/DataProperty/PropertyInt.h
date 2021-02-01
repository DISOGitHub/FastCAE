/**********************
数据类型为int的属性类
**********************/
#ifndef PROPERTYINT_H
#define PROPERTYINT_H

#include "DataPropertyAPI.h"
#include "PropertyBase.h"

namespace DataProperty
{
	class DATAPROPERTYAPI PropertyInt : public PropertyBase
	{
	public:
		//构造函数
		PropertyInt();
		//构造函数
		PropertyInt(QString name, int val);
		~PropertyInt() = default;
		//设置值
		void setValue(int v);
		//获取值
		int getValue();
		//类型转换
		QVariant getVariant() override;

	private:
		int _value;
	};
}


#endif