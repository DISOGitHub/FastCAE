#include "BCFactory.h"
#include <QString>
#include "BCBase/BCBase.h"
#include "BCBase/BCUserDef.h"
#include "BCBase/BCDisplacement.h"
#include "BCBase/BCPressure.h"
#include "BCBase/BCTemperature.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/BCConfig.h"


namespace ParaClassFactory
{
	BCBase::BCBase* BCFactory::createBCByText(QString text)
	{
		QString t = text.toLower();
		BCBase::BCBase* bc = nullptr;

		if (t == "pressure") 
			bc = new BCBase::BCPressure;
		else if (t == "temperature")
			bc = new BCBase::BCTemperature;
		else if (t == "displacement")
			bc = new BCBase::BCDisplacement;
		else bc = new BCBase::BCBase;
		return bc;
	}

	BCBase::BCBase* BCFactory::createBCByType(BCBase::BCType type, QString name, ProjectTreeType t)
	{
		BCBase::BCBase* bc = nullptr;
		if (type == BCBase::UserDef)
		{
			bc = new BCBase::BCUserDef;
			BCBase::BCUserDef* templa = ConfigOption::ConfigOption::getInstance()->getBCConfig()->getBCByName(name, t);
			if (templa != nullptr)
				bc->copy(templa);
			else
			{
				delete bc;
				bc = nullptr;
			}
		}
		else
		{
			switch (type)
			{
			case BCBase::Pressure:
				bc = new BCBase::BCPressure;
				break;
			case BCBase::Velocity:
				bc = new BCBase::BCVectorBase;
				bc->setType(BCBase::Velocity);
				(dynamic_cast<BCBase::BCVectorBase*>(bc))->setVariableName("VelocityX", "VelocityY", "VelocityZ");
				break;
			case BCBase::Temperature:
				bc = new BCBase::BCTemperature;
				break;
			case BCBase::Displacement:
				bc = new BCBase::BCDisplacement;
				break;
			case BCBase::FixSupport:
				bc = new BCBase::BCBase;
				bc->setType(BCBase::FixSupport);
				break;
			case BCBase::Inlet:
				bc = new BCBase::BCBase;
				bc->setType(BCBase::Inlet);
				break;
			case BCBase::Outlet:
				bc = new BCBase::BCBase;
				bc->setType(BCBase::Outlet);
				break;
			case BCBase::Symmetry:
				bc = new BCBase::BCBase;
				bc->setType(BCBase::Symmetry);
				break;
			case  BCBase::Wall:
				bc = new BCBase::BCBase;
				bc->setType(BCBase::Wall);
				break;;
			case  BCBase::Rotation:
				bc = new BCBase::BCScalarBase;
				bc->setType(BCBase::Rotation);
				(dynamic_cast<BCBase::BCScalarBase*>(bc))->setVariableName("Rotation");
				break;
			case  BCBase::AngleVelocity:
				bc = new BCBase::BCScalarBase;
				bc->setType(BCBase::AngleVelocity);
				(dynamic_cast<BCBase::BCScalarBase*>(bc))->setVariableName("AngleVelocity");
				break;
			case  BCBase::AngleAcceleration:
				bc = new BCBase::BCScalarBase;
				bc->setType(BCBase::AngleAcceleration);
				(dynamic_cast<BCBase::BCScalarBase*>(bc))->setVariableName("AngleAcceleration");
				break;
			case  BCBase::Acceleration:
				bc = new BCBase::BCVectorBase;
				bc->setType(BCBase::Acceleration);
				(dynamic_cast<BCBase::BCVectorBase*>(bc))->setVariableName("AccelerationX", "AccelerationY", "AccelerationZ");
				break;
			default:
//				bc = new BCBase::BCBase;
				break;
			}
		}

		return bc;
	}


}