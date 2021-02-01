/**********************
数据类型为string的属性类
**********************/
#ifndef PROPERTYSTRING_H
#define PROPERTYSTRING_H

#include "DataPropertyAPI.h"
#include "PropertyBase.h"
#include <QString>

namespace DataProperty
{
	class DATAPROPERTYAPI PropertyString : public PropertyBase
	{
	public:
		//构造函数
		PropertyString();
		//构造函数
		PropertyString(QString name, QString val);
		~PropertyString() = default;
		//设置值
		void setValue(QString v);
		//获取值
		QString getValue();
		//类型转换
		QVariant getVariant() override;

	private:
		QString _value;
	};
}


#endif