#include "geometryDatum.h"

namespace Geometry
{
	GeometryDatum::GeometryDatum()
		: GeometrySet(DATUM,false)
	{
	}
	

	void GeometryDatum::setDatumType(DatumType t)
	{
		_type = t;
	}

	DatumType GeometryDatum::getDatumType()
	{
		return _type;
	}

	GeometryDatum::~GeometryDatum()
	{

	}


}
