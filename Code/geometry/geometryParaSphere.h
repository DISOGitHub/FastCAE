#ifndef GEOMETRYPARASPHERE_H_
#define GEOMETRYPARASPHERE_H_

#include "geometryModelParaBase.h"
#include <QList>
#include <QString>

namespace Geometry
{
	class GeometrySet;

	class GEOMETRYAPI GeometryParaSphere : public GeometryModelParaBase
	{
	public:
		GeometryParaSphere();
		~GeometryParaSphere() = default;

		void setName(QString name);
		QString getName();

		void setLocation(double* loc);
		void getLocation(double* loc);

		void setRadius(double r);
		double getRadius();


		///����д�빤���ļ�
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		///�ӹ����ļ���������
		virtual void readDataFromProjectFile(QDomElement* e) override;

	private:
		QString _name{};
		double _location[3];
		double _radius{0};

	};

}


#endif