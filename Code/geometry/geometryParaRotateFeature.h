#ifndef _GEOMETRYPARAROTATEFEATURE_H_
#define _GEOMETRYPARAROTATEFEATURE_H_

#include "geometryModelParaBase.h"
#include <QPair>
#include <QMultiHash>
namespace Geometry
{
	class GeometrySet;

	class GEOMETRYAPI GeometryParaRotateFeature : public GeometryModelParaBase
	{
	public:
		GeometryParaRotateFeature();
		~GeometryParaRotateFeature() = default;
		
		void setOriginObject(GeometrySet* set);
		GeometrySet* getOriginObject();

		void appendBody(Geometry::GeometrySet* set, int bodyindex);
		QMultiHash<Geometry::GeometrySet*, int> getBodys();


		void setBasicPoint(double* p);
		void getBasicPoint(double* p);
		
		void setAngle(double a);
		double getAngle();
		
		void isSaveOrigin(bool s);
		bool isSaveOrigin();

		void setMethod(int m);
		int getMethod();

		void setEdge(GeometrySet* body, int index);
		QPair<GeometrySet*, int> getEdge();

		void setVector(double* vec);
		void getVector(double* v);

		void isReverse(bool r);
		bool isReverse();

		///数据写入工程文件
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		///从工程文件读入数据
		virtual void readDataFromProjectFile(QDomElement* e) override;

	private:

		QMultiHash<Geometry::GeometrySet*, int>_solidHash{};
		GeometrySet* _originSet{};
		double _basicPoint[3];
		double _angle{ 0.00 };
		bool _saveOrigin{ false };
		int _method{ 0 };
		QPair<GeometrySet*, int> _edge{};
		double _vector[3];
		bool _reverse{ false };


	};
}


#endif