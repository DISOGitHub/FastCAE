/**********************
数据类型为color的属性类
**********************/
#ifndef PROPERTYCOLOR_H
#define PROPERTYCOLOR_H

#include "DataPropertyAPI.h"
#include "PropertyBase.h"
#include <QColor>

namespace DataProperty
{
	class DATAPROPERTYAPI PropertyColor : public PropertyBase
	{ 
	public:
		PropertyColor();
		PropertyColor(QString name, QColor color);
		~PropertyColor() = default;

		void setColor(QColor color);
		QColor getColor();
		QVariant getVariant() override;

	private:
		QColor _color;
	};
}


#endif