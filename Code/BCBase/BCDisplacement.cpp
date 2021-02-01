#include "BCDisplacement.h"
#include "DataProperty/ParameterDouble.h"
#include <QObject>

namespace BCBase
{
	BCDisplacement::BCDisplacement() : BCVectorBase()
	{
		_valueX->setDescribe(QObject::tr("Displacement X"));
		_valueY->setDescribe(QObject::tr("Displacement Y"));
		_valueZ->setDescribe(QObject::tr("Displacement Z"));
	}
}