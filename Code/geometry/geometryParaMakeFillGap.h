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
		///数据写入工程文件
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		///从工程文件读入数据
		virtual void readDataFromProjectFile(QDomElement* e) override;


	private:
		int _filltype;

		QList<QPair<Geometry::GeometrySet*, int>> _shapelist;
	};
}


#endif