#include "Common.h"

namespace Geant4
{
	QString GetIconStringByDefineType(DataType type)
	{
		QString icon;
		switch (type)
		{
		case PositionType:
			icon = "://QUI/icon/iso.png"; break;
		case RotationType:
			icon = "://QUI/icon/angle.png"; break;
		case ConstantType:
			icon = "://QUI/icon/family.png";  break;
		case ExpressionType:
			icon = "://QUI/icon/curve.png"; break;
		case QuantityType:
			icon = "://QUI/icon/curve.png";break;
		default: break;
		}
		return icon;
	}

	QString GetIconStringBySolidType(SolidType type)
	{
		QString icon;
		switch (type)
		{
		case SolidBox:
			icon = "://QUI/icon/surfaceWithEdge.png"; break;
		case SolidSphere:
			icon = "://QUI/geometry/sphere.png"; break;
		case SolidSubtractionBoolean:
			icon = "://QUI/geometry/cut.png"; break;
		case SolidTube:
			icon = "://QUI/geometry/cylinder.png"; break;
		default: break;
		}

		return icon;
	}

}



