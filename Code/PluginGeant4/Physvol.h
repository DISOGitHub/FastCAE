#ifndef _PHYSVOL_H_
#define _PHYSVOL_H_

#include <QString>
#include "DataProperty/DataBase.h"

class QDomElement;
class QDomDocument;

namespace Geometry
{
	class GeometrySet;
}

namespace Geant4
{
	class Value3D;

	class Physvol : public DataProperty::DataBase
	{
	public:
		Physvol() = default;
		~Physvol() = default;

		void setName(const QString& name)override;

		QString getVolRef();
		QString getSolidRef();
		bool getPosition(double* d);
		bool getRotation(double* d);

		int getType();

		void connectToGeometrySet(Geometry::GeometrySet*);
		Geometry::GeometrySet* getConnectedGeoSet();


		void readGDML(QDomElement& e);
		void writeGDML(QDomDocument& doc, QDomElement& ele);

	private:
		QString _volRef{};
		QString _posRef{};
		QString _rotRef{};
		Value3D* _pos{};
		Value3D* _rot{};

		Geometry::GeometrySet* _connectedShape{};
	};
}

#endif