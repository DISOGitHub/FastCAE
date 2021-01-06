#ifndef GEOMETRYPARAFILLET_H_
#define GEOMETRYPARAFILLET_H_

#include "geometryModelParaBase.h"
#include <QList>
#include <QString>

namespace Geometry
{
	class GeometrySet;

	class GEOMETRYAPI GeometryParaFillet : public GeometryModelParaBase
	{
	public:
		GeometryParaFillet();
		~GeometryParaFillet() = default;

		void setName(QString name);
		QString getName();
		void setGeometrySet(GeometrySet* s);
		GeometrySet* getGeometrySet();
		void setEdgeList(QList<int> edgeList);
		QList<int> getEdgeList();
		void setRadius(double r);
		double getRadius();

		///����д�빤���ļ�
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		///�ӹ����ļ���������
		virtual void readDataFromProjectFile(QDomElement* e) override;

	private:
//		QString _name{};
		GeometrySet* _geoSet{};
		QList<int> _edgeIndexList{};
		double _radius{ 0.0 };

	};

}


#endif