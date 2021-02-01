#ifndef _GEOMETRYPARAMAKEFEATURE_H_
#define _GEOMETRYPARAMAKEFEATURE_H_

#include "geometryModelParaBase.h"
#include <QString>
#include <QMultiHash>
namespace Geometry
{
	class GeometrySet;
	class GEOMETRYAPI GeometryParaMirrorFeature : public GeometryModelParaBase
	{
	public:
		GeometryParaMirrorFeature();
		~GeometryParaMirrorFeature() = default;
 
 		void setCurrentIndex(int index);
 		int getCurrentIndex();

		void setFaceIndex(int i);
		int getFaceIndex();

		void setFaceBody(Geometry::GeometrySet* facebody);
		Geometry::GeometrySet* getFaceBody();

		void appendBody(Geometry::GeometrySet* set, int bodyindex);
		QMultiHash<Geometry::GeometrySet*, int> getBodys();
		

		void setSaveOrigion(bool s);
		bool getSaveOrigion();

		void setPlaneIndex(int i);
		int getPlaneIndex();

		void setDirection(double* dir);
		void getDirection(double* dir);

		void setbasepoint(double* basept);
		void getbasepoint(double* basept);

		void setOriSet(Geometry::GeometrySet* s);
		Geometry::GeometrySet* getOriSet();

		///数据写入工程文件
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		///从工程文件读入数据
		virtual void readDataFromProjectFile(QDomElement* e) override;


	private:
		int _typeindex{};
		QMultiHash<Geometry::GeometrySet*, int>_solidHash{};
		Geometry::GeometrySet* _oriset{};

		int _faceindex{};
		Geometry::GeometrySet* _facebody{};
		int _palneindex{};
		bool _saveorigion{};
		double _basepoint[3]{};
		double _randomdir[3]{};
	};
}


#endif