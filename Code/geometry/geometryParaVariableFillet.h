#ifndef _GEOMETRYPARAVARIABLEFILLET_H_
#define _GEOMETRYPARAVARIABLEFILLET_H_

#include "geometryModelParaBase.h"
#include <QMap>

class GeometrySet;
class vtkActor;
class TopoDS_Shape;

namespace Geometry
{
	class GeometrySet;

	class GEOMETRYAPI GeometryParaVariableFillet : public GeometryModelParaBase
	{
	public:

		GeometryParaVariableFillet();
		~GeometryParaVariableFillet() = default;

		void setInputSet(GeometrySet* s);
		GeometrySet* getGeometrySet();

	/*	void setEdgeSet(GeometrySet* s);
		GeometrySet* getEdgeSet();

		void setEdgeIndex(int e);
		int getEdgeIndex();*/
		void setEdgePair(QPair<Geometry::GeometrySet*, int> edgepair);
		QPair<Geometry::GeometrySet*, int> getEdgePair();

		void setBasicRadius(double b);
		double getBasicRadius();

		void setRadiuMap(QMap<double, double> s);
		QMap<double, double> getRadiuMap();


		///数据写入工程文件
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		///从工程文件读入数据
		virtual void readDataFromProjectFile(QDomElement* e) override;

	private:
		/*int _edgeindex{};
		GeometrySet* _edgeset{};*/
		GeometrySet* _inputSet{};
		QPair<Geometry::GeometrySet*, int> _edgpair{};
		double _basicradius;
		QMap<double, double> _radiusmap{};

		
	};
}


#endif