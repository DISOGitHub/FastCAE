#ifndef GEOMETRYPARAMAKEFILLHOLE_H_
#define GEOMETRYPARAMAKEFILLHOLE_H_

#include "geometryModelParaBase.h"
#include <QList>
#include <QString>

namespace Geometry
{
	class GeometrySet;

	class GEOMETRYAPI GeometryParaMakeFillHole : public GeometryModelParaBase
	{
	public:
		GeometryParaMakeFillHole();
		~GeometryParaMakeFillHole() = default;
		
		void setName(QString name);
		QString getName();



		void setInputSet(GeometrySet* s);

		GeometrySet* getGeometrySet();

		void setFaceList(QList<int> edgeList);
		QList<int> getFaceList(); 
		
		
		///数据写入工程文件
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		///从工程文件读入数据
		virtual void readDataFromProjectFile(QDomElement* e) override;

	private:
//		QString _name{};
		GeometrySet* _geoSet{};
		QList<int> _faceIndexList{};

	};

}
#endif