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
		
	private:
		DatumType _type{ DatumNone };
	};

}

#endif
