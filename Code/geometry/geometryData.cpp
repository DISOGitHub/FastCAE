#include "geometryData.h"
#include "geometryReader.h"
#include <gp_Ax3.hxx>
#include <assert.h>
#include <QCryptographicHash>
#include "geometrySet.h"
#include <QDomDocument>
#include <QDomElement>
#include <QDebug>
#include <QDataStream>
#include "geometryDatum.h"


namespace Geometry
{
	GeometryData* GeometryData::_instance = nullptr;

	GeometryData* GeometryData::getInstance()
	{
		if (_instance == nullptr)
		{
			_instance = new GeometryData;
		}
		return _instance;
	}
	GeometryData::~GeometryData()
	{

	}
	void GeometryData::appendGeometrySet(GeometrySet* set)
	{
		_geometryList.append(set);
	}
	void GeometryData::appendGeometryDatum(GeometryDatum* plane)
	{
		_geomtretryDatumList.append(plane);
	}

	QList<GeometryDatum*> GeometryData::getGeometryDatum()
	{
		return _geomtretryDatumList;
	}

	int GeometryData::getGeometrySetCount()
	{
		return _geometryList.size();
	}
	GeometrySet* GeometryData::getGeometrySetAt(const int index)
	{
		if(index >= 0 && index < _geometryList.size())
			return _geometryList.at(index);
		return nullptr;
	}

	int GeometryData::getIndexByGeoometrySet(GeometrySet* s)
	{
		const int n = _geometryList.size();
		for (int i = 0; i < n; ++i)
		{
			auto c = _geometryList.at(i);
			if (c == s) return i;
		}
		return  -1;
	}

	void GeometryData::removeGeometrySet(const int index)
	{
		assert(index >= 0 && index < _geometryList.size());
		GeometrySet* set = _geometryList.at(index);
		delete set;
		_geometryList.removeAt(index);
	}

	void GeometryData::removeTopGeometrySet(GeometrySet* set)
	{
		_geometryList.removeOne(set);
//		delete set;
	}

	bool GeometryData::hasGeometrySet(GeometrySet* set)
	{
		bool ishas{false};
		if (_geometryList.contains(set)) ishas = true;
		return ishas;
	}

// 	void GeometryData::removeDatumPlaneByIndex(int index)
// 	{
// 		_geomtretryDatumList.removeAt(index);
// 	}

	void GeometryData::removeGeometryDatum(GeometryDatum* plane)
	{
		if (_geomtretryDatumList.contains(plane))
			_geomtretryDatumList.removeOne(plane);
	}

	void GeometryData::setVisable(int index, bool visable)
	{
		assert(index >= 0 && index < _geometryList.size());
		_geometryList[index]->setVisible(visable);
	}
	void GeometryData::clear()
	{
		const int n = this->getGeometrySetCount();
		for (int i = 0; i < n; ++i)
		{
			GeometrySet* set = _geometryList.at(i);
			delete set;
		}
		_geometryList.clear();
		
		const int nd = _geomtretryDatumList.size();
		for (int i = 0; i < nd; ++i)
		{
			auto set = _geomtretryDatumList.at(i);
			delete set;
		}
		_geomtretryDatumList.clear();

		if (_sketchPlan != nullptr)
		{
			delete _sketchPlan;
			_sketchPlan = nullptr;
		}
		GeometrySet::resetMaxID();
	}
	QString GeometryData::getMD5()
	{
		const int n = _geometryList.size();
		if (n < 1) return"";

		QDataStream stream;
		for (int i = 0; i < n; ++i)
		{
			GeometrySet* set = _geometryList[i];
			stream << set->getID() << set->getName() << set->getShape()<<set->getStlDataSet();
		}
		char* s;
		stream >> s;
		QString md5 = QCryptographicHash::hash(s, QCryptographicHash::Md5);
		return md5;
	}
	QDomElement& GeometryData::writeToProjectFile(QDomDocument* doc, QDomElement* element, bool isdiso)
	{
		QDomElement georoot = doc->createElement("Geometry");
		element->appendChild(georoot);
		const int n = _geometryList.size();
		for (int i = 0; i < n; ++i)
		{
			GeometrySet* set = _geometryList.at(i);
			set->writeToProjectFile(doc, &georoot,isdiso);
		}
		return georoot;
	}
	void GeometryData::readFromProjectFile(QDomNodeList* nodelist, bool isdiso)
	{
		QDomElement georoot = nodelist->at(0).toElement();
		QDomNodeList geolist = georoot.elementsByTagName("GeoSet");
		const int geoCount = geolist.size();
		for (int i = 0; i < geoCount; ++i)
		{
 			QDomElement geoset = geolist.at(i).toElement();
// 			if (!isdiso)
// 			{
// 				QDomNodeList setList = geoset.elementsByTagName("Path");
// 				if (setList.size() != 1) continue;
// 				QDomElement pathele = setList.at(0).toElement();
// 				QString fpath = pathele.text();
// 				GeometryReader reader(fpath);
// 				if (!reader.read()) continue;
// 			}
// 			else
// 			{
				GeometrySet* s = new GeometrySet;
				_geometryList.append(s);
//			}
			s->readDataFromProjectFile(&geoset,isdiso);
		}
	}
	GeometrySet* GeometryData::getGeometrySetByID(const int id)
	{
		const int n = this->getGeometrySetCount();
		for (int i = 0; i < n; ++i)
		{
			GeometrySet* set = _geometryList.at(i);
			GeometrySet* res = set->getSetByID(id);
			if (res != nullptr)
				return res;
		}
		return nullptr;
	}

	GeometryDatum* GeometryData::getDatumPlaneByIndex(const int index)
	{
		GeometryDatum* d = nullptr;
		if (index >=0 && index < _geomtretryDatumList.size())
			d = _geomtretryDatumList.at(index);
		return d;
	}

	bool GeometryData::isEmpty()
	{
		int n = _geometryList.size();
		bool b = n < 1;
		return b;
	}

	void GeometryData::setSketchPlane(double* loc, double* dir)
	{
		if (_sketchPlan == nullptr)
			_sketchPlan = new gp_Ax3;
		_sketchPlan->SetAxis(gp_Ax1(gp_Pnt(loc[0], loc[1], loc[2]), gp_Dir(1.00001,0.00001,0))); //向量修改有崩溃风险  OCC bug
 		_sketchPlan->SetDirection(gp_Dir(dir[0], dir[1], dir[2]));
	}

	gp_Ax3* GeometryData::getSketchPlane()
	{
		return _sketchPlan;
	}

	void GeometryData::replaceSet(GeometrySet* newset, GeometrySet* oldset)
	{
		const int index = _geometryList.indexOf(oldset);
		_geometryList.replace(index, newset);
	}

	void GeometryData::sort()
	{
		qSort(_geometryList.begin(), _geometryList.end(), compareSet);
		qSort(_geomtretryDatumList.begin(), _geomtretryDatumList.end(), compareSet);
	}

	bool compareSet(GeometrySet* s1, GeometrySet* s2)
	{
		int id1 = s1->getID();
		int id2 = s2->getID();
		return id1 < id2;
	}

}