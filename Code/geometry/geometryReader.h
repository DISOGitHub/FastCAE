#ifndef GEOMETRYREADER_H
#define GEOMETRYREADER_H

#include "geometryAPI.h"
#include <QString>
#include <QList>

// class TopTools_HSequenceOfShape;
// class TopoDS_Face;

namespace Geometry
{
	class GeometrySet;
	class GeometryData;

	class GEOMETRYAPI GeometryReader
	{
	public:
		GeometryReader(const QString &filename);
		~GeometryReader() = default;
		bool read();
		QList<GeometrySet*> getResult();


	private: 
// 		void buildMesh(TopTools_HSequenceOfShape* shape,GeometrySet* set);
// 		void buildSurface(TopoDS_Face face, GeometrySet* set);
		bool readStp(const char* filename);
		bool readIgs(const char* filename);
		bool readBrep(const char* filename);

	private:
		QString _filename{};
		GeometryData* _geoData{};

		QList<GeometrySet*> _result{};
	};
}


#endif
