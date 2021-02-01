#ifndef _GEOMETRYPARAREVOL_H_
#define _GEOMETRYPARAREVOL_H_

#include "geometryModelParaBase.h"
#include <QString>
#include <QMultiHash>

class GeometrySet;
class vtkActor;
class TopoDS_Shape;

namespace Geometry
{
	class GeometrySet;

	class GEOMETRYAPI GeometryParaRevol : public GeometryModelParaBase
	{
	public:

		GeometryParaRevol();
		~GeometryParaRevol() = default;


		void setShapeHash(QMultiHash<Geometry::GeometrySet*, int> shapehash);
		QMultiHash<Geometry::GeometrySet*, int> getShapeHash();

		void setAxisEdge(QPair<Geometry::GeometrySet*, int> axisedge);
		QPair<Geometry::GeometrySet*, int> getAxisEdge();
		
		void setDistance(double d1);
		double getDistance();

		void setBasciPoint(double* dir);
		void getBasicPoint(double* dir);

		void setDegree(double d);
		double getDegree();

		void setReverse(bool s);
		bool getReverse();

		void setMethod(int s);
		int getMethod();

		void setCoor(double* coor);
		void getCoor(double* coor);

		void setSolid(bool s);
		bool getSolid();



		///数据写入工程文件
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		///从工程文件读入数据
		virtual void readDataFromProjectFile(QDomElement* e) override;

	private:
		//QString _name{};

		QMultiHash<Geometry::GeometrySet*, int> _shapeHash{};
		QPair<Geometry::GeometrySet*, int> _aixsetedge{};
		double _d{};
		double _basept[3]{};
		bool _reverse{ false };
		double _degree{};
		int _method{};
		double _coor[3]{};
		bool _solid{ true };
	
	};
}


#endif