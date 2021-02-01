#ifndef _GEOMETRYPARASWEEP_H_
#define _GEOMETRYPARASWEEP_H_

#include "geometryModelParaBase.h"
#include <QString>
#include <QMultiHash>

class GeometrySet;
class vtkActor;
class TopoDS_Shape;

namespace Geometry
{
	class GeometrySet;

	class GEOMETRYAPI GeometryParaSweep : public GeometryModelParaBase
	{
	public:

		GeometryParaSweep();
		~GeometryParaSweep() = default;

		void setShapeHash(QMultiHash<Geometry::GeometrySet*, int> shapehash);
		QMultiHash<Geometry::GeometrySet*, int> getShapeHash();

		void setPath(QPair<Geometry::GeometrySet*, int> path);
		QPair<Geometry::GeometrySet*, int> getPath();

		void setSolid(bool s);
		bool getSloid();
		///数据写入工程文件
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		///从工程文件读入数据
		virtual void readDataFromProjectFile(QDomElement* e) override;

	private:
		QString _name{};

		QMultiHash<Geometry::GeometrySet*, int> _shapeHash;
		QPair<Geometry::GeometrySet*, int> _path;
		bool _issolid{ false };
	};
}


#endif