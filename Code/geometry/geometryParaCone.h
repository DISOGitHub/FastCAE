#ifndef _GEOMETRYPARACONE_H_
#define _GEOMETRYPARACONE_H_

#include "geometryModelParaBase.h"
#include <QString>

namespace Geometry
{
	class GEOMETRYAPI GeometryParaCone : public GeometryModelParaBase
	{
	public:
		GeometryParaCone();
		~GeometryParaCone() = default;

		void setName(QString name);
		QString getName();

		void setLocation(double* loc);
		void getLocation(double* loc);

		void setRadius1(double* r);
		void getRadius1(double* r);

		void setRadius2(double* r);
		void getRadius2(double* r);

		void setLength(double* l);
		void getLength(double* l);

		void setDirection(double* dir);
		void getDirection(double* dir);
		///数据写入工程文件
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		///从工程文件读入数据
		virtual void readDataFromProjectFile(QDomElement* e) override;

	private:
		QString _name{};
		double _location[3];
		double _radius1;
		double _radius2;
		double _length;
		double _direction[3];
	};
}


#endif