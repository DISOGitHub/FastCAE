#ifndef _COMMONFUNCTIONS_H_
#define _COMMONFUNCTIONS_H_

#include "moduleBase/moduleBaseAPI.h"
#include <QString>

extern "C"
{
	//清空并移除文件夹
	bool MODULEBASEAPI RemoveDir(QString fullpath);
}


#endif