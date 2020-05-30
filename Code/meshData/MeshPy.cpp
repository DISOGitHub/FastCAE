#include "MeshPy.h"
#include "meshKernal.h"
#include "meshSingleton.h"
#include "meshSet.h"

MeshData::MeshData *md = MeshData::MeshData::getInstance();

MESHDATAAPI int getKernalCountPy()
{
	return md->getKernalCount();
}

MESHDATAAPI const char* getKernalNameByIndexPy(const int index)
{
	MeshData::MeshKernal *mk = md->getKernalAt(index);
	if (!mk)	return "";
	std::string cppStr = mk->getName().toStdString();
	const char* cStr = cppStr.c_str();
	char name[128] = { 0 };
	strcpy(name, cStr);
	return name;
}

MESHDATAAPI const char* getKernalNameByKidPy(const int kid)
{
	MeshData::MeshKernal *mk = md->getKernalByID(kid);
	if (!mk)	return "";
	std::string cppStr = mk->getName().toStdString();
	const char* cStr = cppStr.c_str();
	char name[128] = { 0 };
	strcpy(name, cStr);
	return name;
}

MESHDATAAPI void removeKernalByIndexPy(const int index)
{
	md->removeKernalAt(index);
}

MESHDATAAPI void removeKernalByKidPy(const int kid)
{
	md->removeKernalByID(kid);
}

MESHDATAAPI int getSetCountPy()
{
	return md->getMeshSetCount();
}

MESHDATAAPI const char* getSetNameByIndexPy(const int index)
{
	MeshData::MeshSet *ms = md->getMeshSetAt(index);
	if (!ms)	return "";
	std::string cppStr = ms->getName().toStdString();
	const char* cStr = cppStr.c_str();
	char name[128] = { 0 };
	strcpy(name, cStr);
	return name;
}

MESHDATAAPI const char* getSetNameBySidPy(const int sid)
{
	MeshData::MeshSet *ms = md->getMeshSetByID(sid);
	if (!ms)	return "";
	std::string cppStr = ms->getName().toStdString();
	const char* cStr = cppStr.c_str();
	char name[128] = { 0 };
	strcpy(name, cStr);
	return name;
}

MESHDATAAPI void removeSetByIndexPy(const int index)
{
	md->removeMeshSetAt(index);
}

MESHDATAAPI void clearPy()
{
	md->clear();
}

MESHDATAAPI void generateDisplayDataSetPy()
{
	md->generateDisplayDataSet();
}
