/*********************
属性列表
需要在属性框显示的数据类都需要继承此类
**********************/
#ifndef PROPERTYLIST_H
#define PROPERTYLIST_H

#include "DataPropertyAPI.h"
#include <QList>
#include <QString>
#include <QColor>
//#include "PropertyPoint.h"

namespace DataProperty
{
	class PropertyBase;

	class DATAPROPERTYAPI PropertyList
	{
	public:
		PropertyList() = default;
		~PropertyList();
		//获取属性个数
		int getPropertyCount();
		//获取第index个属性
		PropertyBase* getPropertyAt(const int index);
		//通过名字获取属性
		PropertyBase* getPropertyByName(const QString &name);
		//拷贝propList的内容
		void copy(PropertyList* propList);
		//通过不同的方法添加属性 name为属性名称，第二参数为属性的值
		void appendProperty(QString name, int value);
		void appendProperty(QString name, double value);
		void appendProperty(QString name, QString string);
		void appendProperty(QString name, QColor color);
		void appendProperty(QString name, bool value);
		void appendProperty(QString name, double *c);
		void appendProperty(QString name, double x, double y, double z);
		//徐文强,2020/6/19 11:52添加
		bool removeProperty(QString name);

	protected:
		bool appendProperty(PropertyBase* p);
		void clearPropertyList();

    private:
		QList<PropertyBase*> _propertyList;
	};
}


#endif
