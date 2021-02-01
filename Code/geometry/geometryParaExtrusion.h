#ifndef _GEOMETRYPARAEXTRUSION_H_
#define _GEOMETRYPARAEXTRUSION_H_

#include "geometryModelParaBase.h"
#include <QString>
#include <QMultiHash>

class GeometrySet;
class vtkActor;
class TopoDS_Shape;

namespace Geometry
{
	class GeometrySet;

	class GEOMETRYAPI GeometryParaExtrusion : public GeometryModelParaBase
	{
	public:

		GeometryParaExtrusion();
		~GeometryParaExtrusion() = default;


		void setShapeHash(QMultiHash<Geometry::GeometrySet*, int> shapehash);
		QMultiHash<Geometry::GeometrySet*, int> getShapeHash();

		void setDistance(double d1);
		double getDistance();

		void setDirection(double* dir);
		void getDirection(double* dir);


		void setReverse(bool s);
		bool getReverse();

		void setSolid(bool s);
		bool getSloid();
		///数据写入工程文件
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		///从工程文件读入数据
		virtual void readDataFromProjectFile(QDomElement* e) override;

	private:
		//QString _name{};

		QMultiHash<Geometry::GeometrySet*, int> _shapeHash;
		double _d;
		double _dir[3]{};
		bool _reverse{ false };
		bool _issolid{ false };
	};
}


#endif