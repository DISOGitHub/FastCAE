#include "BCType.h"

namespace BCBase
{
	QString BCTypeToString(BCType t)
	{
		QString s = "None";
		switch (t)
		{
		case BCBase::Pressure:
			s = "Pressure"; break;
		case BCBase::Velocity:
			s = "Velocity"; break;
		case BCBase::Temperature:
			s = "Temperature"; break;
		case BCBase::FixSupport:
			s = "FixSupport"; break;
		case BCBase::Displacement:
			s = "Displacement"; break;
		case BCBase::AngleVelocity:
			s = "AngleVelocity"; break;
		case BCBase::Rotation:
			s = "Rotation"; break;
		case BCBase::Acceleration:
			s = "Acceleration"; break;
		case BCBase::AngleAcceleration:
			s = "AngleAcceleration"; break;
		case BCBase::Inlet:
			s = "Inlet"; break;
		case BCBase::Outlet:
			s = "Outlet"; break;
		case BCBase::Symmetry:
			s = "Symmetry"; break;
		case BCBase::Wall:
			s = "Wall"; break;
		case  BCBase::UserDef:
			s = "UserDefine"; break;
		default:
			break;
		}

		return s;
	}

	BCType StringToBCType(QString s)
	{
		BCType t = None;

		if (s == "Pressure") t = Pressure;
		else if (s == "Temperature") t = Temperature;
		else if (s == "Velocity") t = Velocity;
		else if (s == "FixSupport") t = FixSupport;
		else if (s == "Displacement") t = Displacement;
		else if (s == "AngleVelocity") t = AngleVelocity;
		else if (s == "Rotation") t = Rotation;
		else if (s == "Acceleration") t = Acceleration;
		else if (s == "AngleAcceleration") t = AngleAcceleration;
		else if (s == "Inlet") t = Inlet;
		else if (s == "Outlet") t = Outlet;
		else if (s == "Symmetry") t = Symmetry;
		else if (s == "Wall") t = Wall;
		else if (s == "UserDefine") t = UserDef;
		else t = UserDef;
		return t;
	}

	QString BCBASEAPI BCIconByType(BCType t)
	{
		QString s;
		switch (t)
		{
		case BCBase::Pressure:
			s = "://QUI/icon/press.png"; break;
		case BCBase::Velocity:
			s = "://QUI/icon/speed.png"; break;
		case BCBase::Temperature:
			s = "://QUI/icon/tempure.png"; break;
		case BCBase::FixSupport:
			s = "://QUI/icon/fix.png"; break;
		case BCBase::Displacement:
			s = "Displacement"; break;
		case BCBase::AngleVelocity:
			s = "AngleVelocity"; break;
		case BCBase::Rotation:
			s = "://QUI/icon/angle.png"; break;
		case BCBase::Acceleration:
			s = "://QUI/icon/acce.png"; break;
		case BCBase::AngleAcceleration:
			s = "AngleAcceleration"; break;
		case BCBase::Inlet:
			s = "://QUI/icon/inlet.png"; break;
		case BCBase::Outlet:
			s = "://QUI/icon/outlet.png"; break;
		case BCBase::Symmetry:
			s = "://QUI/icon/symmetry.png"; break;
		case BCBase::Wall:
			s = "://QUI/icon/wall.png"; break;
		case  BCBase::UserDef:
//			s = "UserDefine"; break;
		default:
			break;
		}

		return s;
	}


}