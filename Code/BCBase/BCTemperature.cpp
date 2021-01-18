#include "BCTemperature.h"
#include "DataProperty/ParameterDouble.h"
#include <QObject>

namespace BCBase
{
	BCTemperature::BCTemperature() : BCScalarBase(BCType::Temperature)
	{
		_value->setDescribe(QObject::tr("Temperature"));
	}


}
