#ifndef _COMMONFUNCTIONS_H_
#define _COMMONFUNCTIONS_H_

#include "moduleBase/moduleBaseAPI.h"
#include <QString>

extern "C"
{
	//��ղ��Ƴ��ļ���
	bool MODULEBASEAPI RemoveDir(QString fullpath);
	//��������ת��Ϊ�ַ�����acc-С�����λ��
	QString MODULEBASEAPI doubleToString(double v, int acc);
}


#endif