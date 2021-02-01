
/************************
属性基类，在属性框中显示
所有数据类的基类
************************/
#ifndef PROPERTYBASE_H
#define PROPERTYBASE_H

#include "DataPropertyAPI.h"
#include <QString>
#include <QVariant>

namespace DataProperty
{
	enum PropertyType
	{
		Prop_Blank = 0,
		Prop_INT,
		Prop_Double,
		Prop_Color,
		Prop_String,
		Prop_Bool,
		Prop_Point,
		Prop_IDList,
	};

	class DATAPROPERTYAPI PropertyBase
	{
	public:
		//构造函数
		PropertyBase(PropertyType type);
		virtual ~PropertyBase() = default;

		//设置类型
		void setType(PropertyType tp);
		//获取类型
		PropertyType getType();
		//设置名称
		void setName(const QString &name);
		//获取名称
		QString getName();
	
		bool isEditable();
		void setEditable(bool editable);
		//类型转换
		virtual QVariant getVariant();

	protected:
		PropertyType _type{ Prop_Blank };
		QString _name{};
		bool _editable{ false };


	};

}


#endif