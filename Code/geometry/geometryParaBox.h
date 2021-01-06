#ifndef _GEOMETRYPARABOX_H_
#define _GEOMETRYPARABOX_H_

#include "geometryModelParaBase.h"
#include <QString>

namespace Geometry
{
	class GEOMETRYAPI GeometryParaBox : public GeometryModelParaBase
	{
	public:
		GeometryParaBox();
		~GeometryParaBox() = default;

		void setName(QString name);
		QString getName();

		void setLocation(double* loc);
		void getLocation(double* loc);

		void setGeoPara(double* para);
		void getGeoPara(double* para);

		///����д�빤���ļ�
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		///�ӹ����ļ���������
		virtual void readDataFromProjectFile(QDomElement* e) override;

	private:

		QString _name{};
		double _location[3];
		double _geoPara[3];
	};
}




#endif