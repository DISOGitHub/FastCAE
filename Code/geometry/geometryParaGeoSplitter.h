#ifndef _GEOMETRYPARAGEOSPLITTER_H_
#define _GEOMETRYPARAGEOSPLITTER_H_

#include "geometryModelParaBase.h"
#include <QString>

namespace Geometry
{
	class GeometrySet;
	class GEOMETRYAPI GeometryParaGeoSplitter : public GeometryModelParaBase
	{
	public:
		GeometryParaGeoSplitter();
		~GeometryParaGeoSplitter() = default;

		void setName(QString name);
		QString getName();

		void setFaceIndex(int i);
		int getFaceIndex();

		void setFaceBody(Geometry::GeometrySet* facebody);
		Geometry::GeometrySet* getFaceBody();

		void setOriSet(Geometry::GeometrySet* s);
		Geometry::GeometrySet* getOriSet();

		///数据写入工程文件
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		///从工程文件读入数据
		virtual void readDataFromProjectFile(QDomElement* e) override;

	private:
		Geometry::GeometrySet* _oriset{};
		QString _name{};
		int _faceindex{};
		Geometry::GeometrySet* _facebody{};

	};
}


#endif