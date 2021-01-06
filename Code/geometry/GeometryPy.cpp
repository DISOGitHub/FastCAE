#include "GeometryPy.h"
#include "geometryData.h"
#include "geometrySet.h"
#include <string>
#include <QDebug>

int GEOMETRYAPI getGeometrySetCount()
{
	Geometry::GeometryData* geodata = Geometry::GeometryData::getInstance();
	int c = geodata->getGeometrySetCount();
	return c;
}

bool GEOMETRYAPI isEmpty()
{
	Geometry::GeometryData* geodata = Geometry::GeometryData::getInstance();
	bool em = geodata->isEmpty();
	return em;
}

GEOMETRYAPI	char* getGeometrySetNameByIndex(int index)
{
	QString name{};
	Geometry::GeometryData* geodata = Geometry::GeometryData::getInstance();
	Geometry::GeometrySet* set = geodata->getGeometrySetAt(index);
	if (set != nullptr)  name = set->getName();
	//qDebug() << name;
	std::string ba = name.toStdString();
	const char* ch = ba.c_str();
	char dest[128] = { 0 };
	strcpy(dest, ch);
	return dest;
}

GEOMETRYAPI	char* getGeometrySetNameByID(int id)
{
	QString name{};
	Geometry::GeometryData* geodata = Geometry::GeometryData::getInstance();
	Geometry::GeometrySet* set = geodata->getGeometrySetByID(id);
	if (set != nullptr)  name = set->getName();
	qDebug() << name;
	std::string ba = name.toStdString();
	const char* ch = ba.c_str();
	char dest[128] = { 0 };
	strcpy(dest, ch);
	return dest;
}

int GEOMETRYAPI getIndexOfGeoometrySet(char* n)
{
	QString name(n);
	Geometry::GeometryData* geodata = Geometry::GeometryData::getInstance();
	const int nc = geodata->getGeometrySetCount();
	for (int i = 0; i < nc; ++i)
	{
		Geometry::GeometrySet* set = geodata->getGeometrySetAt(i);
		QString cname = set->getName();
		if (cname == name) return i;
	}
	return  -1;
}

int GEOMETRYAPI getIDOfGeoometrySet(char* n)
{
	QString name(n);
	Geometry::GeometryData* geodata = Geometry::GeometryData::getInstance();
	const int nc = geodata->getGeometrySetCount();
	for (int i = 0; i < nc; ++i)
	{
		Geometry::GeometrySet* set = geodata->getGeometrySetAt(i);
		QString cname = set->getName();
		if (cname == name) return set->getID();
	}
	return  -1;
}

int GEOMETRYAPI getIDByIndex(int index)
{
	int id = -1;
	Geometry::GeometryData* geodata = Geometry::GeometryData::getInstance();
	auto set = geodata->getGeometrySetAt(index);
	if (set != nullptr) id = set->getID();
	return id;
}

int GEOMETRYAPI getIndexByID(int id)
{
	Geometry::GeometryData* geodata = Geometry::GeometryData::getInstance();
	const int n = geodata->getGeometrySetCount();
	for (int i = 0; i < n; ++i)
	{
		auto set = geodata->getGeometrySetAt(i);
		if (id == set->getID()) return i;
	}
	return -1;
}

void GEOMETRYAPI removeGeometrySetByIndex(int index)
{
	int n = getGeometrySetCount();
	if (index < 0 || index >= n) return;

	Geometry::GeometryData* geodata = Geometry::GeometryData::getInstance();
	geodata->removeGeometrySet(index);
	
}

void GEOMETRYAPI removeGeometrySetByID(int id)
{
	Geometry::GeometryData* geodata = Geometry::GeometryData::getInstance();
	int index = getIndexByID(id);
	if (index < 0) return;
	geodata->removeGeometrySet(index);
}

void GEOMETRYAPI clear()
{
	Geometry::GeometryData* geodata = Geometry::GeometryData::getInstance();
	geodata->clear();
}

