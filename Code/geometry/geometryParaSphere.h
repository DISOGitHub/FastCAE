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


		///数据写入工程文件
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		///从工程文件读入数据
		virtual void readDataFromProjectFile(QDomElement* e) override;

	private:
		QString _name{};
		double _location[3];
		double _radius{0};

	};

}


#endif