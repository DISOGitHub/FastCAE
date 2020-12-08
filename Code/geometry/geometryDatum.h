#ifndef GEOMETRYDATUMPLANE_H_
#define GEOMETRYDATUMPLANE_H_

#include "geometryAPI.h"
#include "geometrySet.h"

class TopoDS_Shape;

namespace Geometry
{
	enum DatumType
	{
		DatumNone,
		DatumPoint,
		DatumAxis,
		DatumPlane,
	};

	class GEOMETRYAPI GeometryDatum : public GeometrySet
	{
	public:
		GeometryDatum();
		~GeometryDatum();

		void setDatumType(DatumType t);
		DatumType getDatumType();

		virtual QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* ele, bool isDisp /* = false */) override;
		virtual void readDataFromProjectFile(QDomElement* e, bool isDiso /* = false */);

		static QString datumTypeToString(DatumType t);
		static DatumType datumTypeFromString(QString s);

	private:
		DatumType _type{ DatumNone };
	};

}

#endif
