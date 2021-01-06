#ifndef GEOMETRYPARAPOINT_H_
#define GEOMETRYPARAPOINT_H_

#include "geometryModelParaBase.h"
#include <QList>
#include <QString>

namespace Geometry
{
	class GeometrySet;

	class GEOMETRYAPI GeometryParaPoint : public GeometryModelParaBase
	{
	public:
		GeometryParaPoint();
		~GeometryParaPoint() = default;

		void setName(QString name);
		QString getName();

		void setPara(double* para);
		void getPara(double* para);

		void setCorner(double* corner);
		void getCorner(double* corner);

		///����д�빤���ļ�
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		///�ӹ����ļ���������
		virtual void readDataFromProjectFile(QDomElement* e) override;

	private:
		QString _name{};
		double _para[3];
		double _corner[3];

	};

}


#endif