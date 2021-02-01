#include "BCPressure.h"
#include "DataProperty/ParameterDouble.h"
#include <QObject>

namespace BCBase
{
	BCPressure::BCPressure() : BCVectorBase()
	{
		_valueX->setDescribe(QObject::tr("Pressure X"));
		_valueY->setDescribe(QObject::tr("Pressure Y"));
		_valueZ->setDescribe(QObject::tr("Pressure Z"));
	}


}