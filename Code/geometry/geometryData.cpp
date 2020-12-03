#include "geometryData.h"
#include <gp_Ax3.hxx>
#include <assert.h>
#include <QCryptographicHash>
#include "geometrySet.h"
#include <QDomDocument>
#include <QDomElement>
#include <QDataStream>
#include "geometryDatum.h"
#include "GeoComponent.h"

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

		const int nc = _geoCpList.size();
		for (int i = 0; i < nc; ++i)
		{
			auto gc = _geoCpList.at(i);
			delete gc;
			gc = nullptr;
		}
		_geoCpList.clear();
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
			stream << set->getID() << set->getName() << set->getShape()/*<<set->getStlDataSet()*/;
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

		int n = _geometryList.size();
		if (n > 0)
		{
			QDomElement geoSetsRoot = doc->createElement("GeoSets");
			georoot.appendChild(geoSetsRoot);
			for (auto geoSet : _geometryList)
				geoSet->writeToProjectFile(doc, &geoSetsRoot, isdiso);
		}

		n = _geomtretryDatumList.size();
		if (n > 0)
		{
			QDomElement geoDatumsRoot = doc->createElement("GeoDatums");
			georoot.appendChild(geoDatumsRoot);
			for (auto datum : _geomtretryDatumList)
				datum->writeToProjectFile(doc, &geoDatumsRoot, isdiso);
		}

		n = _geoCpList.size();
		if (n > 0)
		{
			QDomElement gcsRoot = doc->createElement("GeoComponents");
			georoot.appendChild(gcsRoot);
			for (auto geoComponent : _geoCpList)
				geoComponent->writeToProjectFile(doc, &gcsRoot);
		}
		return georoot;
	}
	void GeometryData::readFromProjectFile(QDomNodeList* nodelist, bool isdiso)
	{
		QDomElement georoot = nodelist->at(0).toElement();
		QDomElement geoSetRoot = georoot.elementsByTagName("GeoSets").at(0).toElement();
		QDomElement datumRoot = georoot.elementsByTagName("GeoDatums").at(0).toElement();
		QDomElement gcRoot = georoot.elementsByTagName("GeoComponents").at(0).toElement();

		QDomNodeList geoSetList = geoSetRoot.elementsByTagName("GeoSet");
		QDomNodeList datumList = datumRoot.elementsByTagName("GeoDatum");
		QDomNodeList gcList = gcRoot.elementsByTagName("GeoComponent");

		const int geoCount = geoSetList.size();
		for (int i = 0; i < geoCount; ++i)
		{
 			QDomElement geoset = geoSetList.at(i).toElement();
			GeometrySet* s = new GeometrySet;
			_geometryList.append(s);
			s->readDataFromProjectFile(&geoset,isdiso);
		}

		const int nd = datumList.size();
		for (int i = 0; i < nd; ++i)
		{
			QDomElement geoDat = datumList.at(i).toElement();
			GeometryDatum* s = new GeometryDatum;
			_geomtretryDatumList.append(s);
			s->readDataFromProjectFile(&geoDat, isdiso);
		}

		const int gcCount = gcList.size();
		for (int i = 0; i < gcCount; ++i)
		{
			QDomElement gcEle = gcList.at(i).toElement();
			QString gcName = gcEle.attribute("gcName");
			GeoComponentType gcType = GeoComponent::stringTogcType(gcEle.attribute("gcType"));
			GeoComponent* gc = new GeoComponent(gcName, gcType);
			gc->readDataFromProjectFile(&gcEle);
			_geoCpList.append(gc);
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

	GeometryDatum* GeometryData::getDatumByIndex(const int index)
	{
		GeometryDatum* d = nullptr;
		if (index >=0 && index < _geomtretryDatumList.size())
			d = _geomtretryDatumList.at(index);
		return d;
	}

	GeoComponent* GeometryData::getGeoComponentByIndex(int index)
	{
		if (index >= 0 && index < _geoCpList.size())
			return _geoCpList.at(index);
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
		qSort(_geoCpList.begin(), _geoCpList.end(), compareSet);
	}

	void GeometryData::appendGeoComponent(GeoComponent* gc)
	{
		if (!gc)	return;
		_geoCpList.append(gc);
	}

	QList<GeoComponent*>& GeometryData::getGeoComponentList()
	{
		return _geoCpList;
	}

	GeoComponent* GeometryData::getGeoComponentByID(int ID)
	{
		for (auto aGc : _geoCpList)
			if (ID == aGc->getID())	return aGc;
	}

	bool GeometryData::removeGeoComponentByIndex(int index)
	{
		if (_geoCpList.size() < index + 1)	return false;
		GeoComponent* gc = _geoCpList.at(index);
		if (!_geoCpList.removeOne(gc))	return false;
		delete gc;
		gc = nullptr;
		return true;
	}

	/*void GeometryData::generateMeshAndWriteToXML(QDomDocument& doc, QDomElement& parent)
	{
		QMultiHash<int, int> Surface, Body;
		for (GeoComponent* gc : _geoCpList)
		{
			QMultiHash<Geometry::GeometrySet*, int> items = gc->getSelectedItems();
			QHashIterator<GeometrySet*, int> it(items);
			while (it.hasNext())
			{
				it.next();
				int ID = it.key()->getID();
				int index = it.value();
				if (gc->getGCType() == GeoComponentType::Surface)
					Surface.insert(ID, index);
				else if (gc->getGCType() == GeoComponentType::Body)
					Body.insert(ID, index);
			}
		}

		Gmsh::GMshPara* para = new Gmsh::GMshPara;
		para->_solidHash = Body;
		para->_surfaceHash = Surface;
		para->_elementType = "Tet";
		para->_method = 1;
		para->_dim = 3;
		para->_elementOrder = 1;
		para->_sizeFactor = 1.00;
		para->_maxSize = 100.00;
		para->_minSize = 0.00;
		para->_isGridCoplanar = true;
		para->_geoclean = true;

		Setting::BusAPI::instance();

		Gmsh::GmshModule* gModule = Gmsh::GmshModule::getInstance(mw);
		Gmsh::GmshThread* thread = gModule->iniGmshThread(para);
		thread->setSolveFileInfo(&doc, &parent, path);
		auto processBar = new ModuleBase::ProcessBar(mw, QObject::tr("Gmsh Working..."));
		Gmsh::GmshThreadManager* manager = gModule->getGmshThreadManager();
		manager->insertThread(processBar, thread);
	}*/

	bool compareSet(DataProperty::DataBase* s1, DataProperty::DataBase* s2)
	{
		int id1 = s1->getID();
		int id2 = s2->getID();
		return id1 < id2;
	}

}