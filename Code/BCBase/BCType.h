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
		Pressure,           //压力
		Velocity,           //速度
		Temperature,        //温度

		//Struct
		FixSupport,         //固定支撑
		Displacement,       //位移
		AngleVelocity,      //角速度
		Rotation,           //转角
		Acceleration,       //加速度
		AngleAcceleration,  //角加速度
		
		//Flow
		Inlet,              //入口
		Outlet,             //出口
		Symmetry,           //对称面
		Wall,               //壁面

		End,

		UserDef,

	};

	extern QString BCBASEAPI BCTypeToString(BCType t);

	extern QString BCBASEAPI BCIconByType(BCType t);

	extern BCType BCBASEAPI StringToBCType(QString s);

}


#endif