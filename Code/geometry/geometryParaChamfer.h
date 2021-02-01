#ifndef _GEOMETRYPARACHAMFER_H_
#define _GEOMETRYPARACHAMFER_H_

#include "geometryModelParaBase.h"
#include <QString>
#include <QMultiHash>

class GeometrySet;
class vtkActor;
class TopoDS_Shape;

namespace Geometry
{
	class GeometrySet;

	class GEOMETRYAPI GeometryParaChamfer : public GeometryModelParaBase
	{
	public:

		GeometryParaChamfer();
		~GeometryParaChamfer() = default;

		void setInputSet(GeometrySet* s);
		GeometrySet* getGeometrySet();

		void setCombIndex(bool i);
		bool getCombIndex();

		void setEdgeIndexList(QList<int> s);
		QList<int> getEdgeIndexList();

		void setDistance1(double d1);
		void setDistance2(double d2);
		double getDistance1();
		double getDistance2();

		///数据写入工程文件
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		///从工程文件读入数据
		virtual void readDataFromProjectFile(QDomElement* e) override;

	private:
		//QString _name{};

//		QMultiHash<Geometry::GeometrySet*, int> _shapeHash;
	
		GeometrySet* _inputSet{};
		QList<int> _edgeindexlist;

		bool _combindex0;
		double _d1;
		double _d2;
		
	};
}


#endif