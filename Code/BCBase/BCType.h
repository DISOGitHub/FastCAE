#ifndef _BCTYPE_H_
#define _BCTYPE_H_

#include <QString>
#include "BCBaseAPI.h"

namespace BCBase
{
	enum BCType
	{
		None = 0,
		
		//common
		Pressure,           //ѹ��
		Velocity,           //�ٶ�
		Temperature,        //�¶�

		//Struct
		FixSupport,         //�̶�֧��
		Displacement,       //λ��
		AngleVelocity,      //���ٶ�
		Rotation,           //ת��
		Acceleration,       //���ٶ�
		AngleAcceleration,  //�Ǽ��ٶ�
		
		//Flow
		Inlet,              //���
		Outlet,             //����
		Symmetry,           //�Գ���
		Wall,               //����

		End,

		UserDef,

	};

	extern QString BCBASEAPI BCTypeToString(BCType t);

	extern QString BCBASEAPI BCIconByType(BCType t);

	extern BCType BCBASEAPI StringToBCType(QString s);

}


#endif