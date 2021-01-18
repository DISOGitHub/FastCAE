#ifndef _COMMONFUNCTIONS_H_
#define _COMMONFUNCTIONS_H_

#include "moduleBase/moduleBaseAPI.h"
#include <QString>

extern "C"
{
	//清空并移除文件夹
	bool MODULEBASEAPI RemoveDir(QString fullpath);
	//将浮点数转化为字符串，acc-小数点后位数
	QString MODULEBASEAPI doubleToString(double v, int acc);
}


#endif