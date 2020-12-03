#ifndef _SOLIDBOOLEAN_H
#define _SOLIDBOOLEAN_H

#include "SolidShapeBase.h"

namespace Geant4 {
	class Position;
	class Rotation;

	class SolidBoolean:public SolidShapeBase
	{
	public:
		SolidBoolean();
		~SolidBoolean();

		void setFirst(QString s);
		QString getFirst();
		void setSecond(QString s);
		QString getSecond();
		void setPositionValue(QStringList list);
		void getPositionValue(QStringList& list);
		void setRotationValue(QStringList list);
		void getRotationValue(QStringList& list);

		void generateShape() override;

		void readGDML(QDomElement& ele) override;
		void writeGDML(QDomDocument& doc, QDomElement& ele) override;

	private:
		QString m_first;
		QString m_second;
		QString m_positionref;
		QString m_rotationref;
		Position* m_position = nullptr;
		Rotation* m_rotation = nullptr;
	};
}
#endif // _SOLIDBOOLEAN_H

