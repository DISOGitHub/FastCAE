#ifndef _GEOMETRYPARAMAKEFILLGAP_H_
#define _GEOMETRYPARAMAKEFILLGAP_H_

#include "geometryModelParaBase.h"
#include <QString>

namespace Geometry
{
	class GeometrySet;
	class GEOMETRYAPI GeometryParaMakeFillGap : public GeometryModelParaBase
	{
	public:
		GeometryParaMakeFillGap();
		~GeometryParaMakeFillGap() = default;

		void setInputShapes(QList<QPair<Geometry::GeometrySet*, int>> s);
		QList<QPair<Geometry::GeometrySet*, int>> getShapes();

		void setType(int b);
		int getType();
		///����д�빤���ļ�
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		///�ӹ����ļ���������
		virtual void readDataFromProjectFile(QDomElement* e) override;


	private:
		int _filltype;

		QList<QPair<Geometry::GeometrySet*, int>> _shapelist;
	};
}


#endif