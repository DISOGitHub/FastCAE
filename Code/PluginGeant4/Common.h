#ifndef _COMMON_H_
#define _COMMON_H_

#include <QString>
#include "PreDefineBase.h"
#include "SolidShapeBase.h"

namespace Geant4
{
	QString GetIconStringByDefineType(DataType type);
	QString GetIconStringBySolidType(SolidType type);
}


#endif