#ifndef _COMPONENTBASE_H_
#define _COMPONENTBASE_H_

#include "DataPropertyAPI.h"
#include "DataBase.h"

namespace DataProperty
{
	enum ComponentType
	{
		MESH = 1,
		GEOMETRY
	};

	class DATAPROPERTYAPI ComponentBase : public DataBase
	{
	public:
		ComponentBase(ComponentType type);
		virtual ~ComponentBase();
		//获取组件类型
		ComponentType getComponentType();
		//设置组件ID
		void setID(int);
		//获取最大ID
		static int getMaxID();
		//重置最大ID
		static void resetMaxID();
 		//根据ID获取组件类型，只有一个ID的情况下，而没有对象
 		static ComponentType getComponentTypeByID(int ID);
		//枚举转字符串
		static QString componentTypeToString(ComponentType cpType);
		//字符串转枚举
		static ComponentType stringToComponentType(QString sType);

	private:
		static QHash<int, ComponentType> IDType;
		static int currMaxID;
		ComponentType _type;
	};
}

#endif