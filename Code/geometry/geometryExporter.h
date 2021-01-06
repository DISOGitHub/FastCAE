#ifndef GEOMETRYEXPORTER_H_
#define GEOMETRYEXPORTER_H_

#include "geometryAPI.h"
#include <QString>

namespace Geometry
{
	class GeometryData;

	class GEOMETRYAPI GeometryExporter
	{
	public:
		GeometryExporter(QString fileName);
		~GeometryExporter() = default;

		bool write();

	private:
		bool writeBrep(const char* file);
		bool writeIgs(const char* file);
		bool writeStp(const char* file);

	private:
		QString _fileName{};
		GeometryData* _geoData{};

	};
}


#endif