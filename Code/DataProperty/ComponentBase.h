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
		//��ȡ�������
		ComponentType getComponentType();
		//�������ID
		void setID(int);
		//��ȡ���ID
		static int getMaxID();
		//�������ID
		static void resetMaxID();
 		//����ID��ȡ������ͣ�ֻ��һ��ID������£���û�ж���
 		static ComponentType getComponentTypeByID(int ID);
		//ö��ת�ַ���
		static QString componentTypeToString(ComponentType cpType);
		//�ַ���תö��
		static ComponentType stringToComponentType(QString sType);

	private:
		static QHash<int, ComponentType> IDType;
		static int currMaxID;
		ComponentType _type;
	};
}

#endif