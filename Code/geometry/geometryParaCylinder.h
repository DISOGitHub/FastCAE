#ifndef _GEOMETRYPARACYLINDER_H_
#define _GEOMETRYPARACYLINDER_H_

#include "geometryModelParaBase.h"
#include <QString>

namespace Geometry
{
	class GEOMETRYAPI GeometryParaCylinder : public GeometryModelParaBase
	{
	public:
		GeometryParaCylinder();
		~GeometryParaCylinder() = default;

		void setName(QString name);
		QString getName();

		void setLocation(double* loc);
		void getLocation(double* loc);

		void setRadius(double* r);
		void getRadius(double* r);

		void setLength(double* l);
		void getLength(double* l);

		void setDirection(double* dir);
		void getDirection(double* dir);
		///����д�빤���ļ�
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		///�ӹ����ļ���������
		virtual void readDataFromProjectFile(QDomElement* e) override;

	private:
		QString _name{};
		double _location[3];
		double _radius;
		double _length;
		double _direction[3];
	};
}


#endif