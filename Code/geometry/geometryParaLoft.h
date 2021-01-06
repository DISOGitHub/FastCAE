#ifndef _GEOMETRYPARALOFT_H_
#define _GEOMETRYPARALOFT_H_

#include "geometryModelParaBase.h"
#include <QString>
#include <QMultiHash>

class GeometrySet;
class vtkActor;
class TopoDS_Shape;

namespace Geometry
{
	class GeometrySet;

	class GEOMETRYAPI GeometryParaLoft : public GeometryModelParaBase
	{
	public:

		GeometryParaLoft();
		~GeometryParaLoft() = default;


		void setShapeHash(QList<QMultiHash<Geometry::GeometrySet*, int>> shapehash);
		QList<QMultiHash<Geometry::GeometrySet*, int>> getShapeHash();

		void setName(QString s);
		QString getName();

		void setSolid(bool s);
		bool getSloid();
		///����д�빤���ļ�
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		///�ӹ����ļ���������
		virtual void readDataFromProjectFile(QDomElement* e) override;

	private:
		QString _name{};

		QList<QMultiHash<Geometry::GeometrySet*, int>> _shapeHash;
		bool _issolid{ false };
	};
}


#endif