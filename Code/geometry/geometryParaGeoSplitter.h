#ifndef _GEOMETRYPARAGEOSPLITTER_H_
#define _GEOMETRYPARAGEOSPLITTER_H_

#include "geometryModelParaBase.h"
#include <QString>
#include <QMultiHash>
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

		void appendBody(Geometry::GeometrySet* set, int bodyindex);
		QMultiHash<Geometry::GeometrySet*, int> getBodys();

		void setFaceIndex(int i);
		int getFaceIndex();

		void setFaceBody(Geometry::GeometrySet* facebody);
		Geometry::GeometrySet* getFaceBody();

		void setOriSet(Geometry::GeometrySet* s);
		Geometry::GeometrySet* getOriSet();

		///����д�빤���ļ�
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		///�ӹ����ļ��������� 
		virtual void readDataFromProjectFile(QDomElement* e) override;

	private:
		QMultiHash<Geometry::GeometrySet*, int>_solidHash{};
		Geometry::GeometrySet* _oriset{};
		QString _name{};
		int _faceindex{};
		Geometry::GeometrySet* _facebody{};

	};
}


#endif