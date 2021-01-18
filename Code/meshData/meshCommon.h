#ifndef _MESHCOMMON_H_
#define _MESHCOMMON_H_

#include "meshDataAPI.h"
#include <QList>

extern "C"
{
	int MESHDATAAPI VTKCellTypeToDim(int ctype);

}


#endif