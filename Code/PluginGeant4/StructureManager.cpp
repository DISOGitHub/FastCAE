#include "StructureManager.h"
#include "Assembly.h"
#include "Volume.h"
#include <QDomNodeList>
#include <QDomElement>

namespace  Geant4
{

	void StructureManager::appenValume(Volume* v)
	{
		_volumeList.append(v);
	}

	void StructureManager::appendAssembly(Assembly* a)
	{
		_assemblyList.append(a);
	}

	int StructureManager::getType(QString name)
	{
		for (auto ass : _assemblyList)
		{
			if (ass->getName() == name)
				return 2;
		}

		for (auto vol : _volumeList)
		{
			if (vol->getName() == name)
				return 1;
		}
		return 0;
	}

	int StructureManager::getVolumeCount()
	{
		return _volumeList.size();
	}

	Volume* StructureManager::getVolumeAt(int index)
	{
		return _volumeList.at(index);
	}

    Volume* StructureManager::getVolumeByName(QString name)
	{
		for (auto v : _volumeList)
		{
			if (v->getName() == name)
				return v;
		}
		return nullptr;
	}

	Physvol* StructureManager::getPhysvolAt(int vindex, int pIndex)
	{
		auto vol = this->getVolumeAt(vindex);
		return vol->getPhysvolAt(pIndex);
	}

	QList<Physvol*> StructureManager::getAllPhysvol()
	{
		QList<Physvol*> phyList;
		for (auto v :_volumeList)
		{
			const int n = v->getPhysvolCount();
			for (int i = 0; i < n; ++i)
			{
				auto p = v->getPhysvolAt(i);
				phyList.append(p);
			}
		}
		return phyList;
	}

/*
	bool StructureManager::readGDML(QDomNodeList & nodelist)
	{
		if (nodelist.size() != 1)  return false;
		QDomElement domele = nodelist.at(0).toElement();

		QDomNodeList volList = domele.elementsByTagName("volume");
		int n = volList.size();
		for (int i = 0; i < n; ++i)
		{
			QDomElement ele = volList.at(i).toElement();
			Volume* v = new Volume;
			this->appenValume(v);
			v->readGDML(ele);
		}

		QDomNodeList assList = domele.elementsByTagName("assembly");
		n = assList.size();
		for (int i = 0; i < n; ++i)
		{
			QDomElement ele = assList.at(i).toElement();
			Assembly* a = new Assembly;
			this->appendAssembly(a);
			a->readGDML(domele);
		}

		return true;
	}*/

	bool StructureManager::readGDML(QDomNode & node)
	{
		QDomElement domele = node.toElement();

		QDomNodeList volList = domele.elementsByTagName("volume");
		int n = volList.size();
		for (int i = 0; i < n; ++i)
		{
			QDomElement ele = volList.at(i).toElement();
			Volume* v = new Volume;
			this->appenValume(v);
			v->readGDML(ele);
		}

		QDomNodeList assList = domele.elementsByTagName("assembly");
		n = assList.size();
		for (int i = 0; i < n; ++i)
		{
			QDomElement ele = assList.at(i).toElement();
			Assembly* a = new Assembly;
			this->appendAssembly(a);
			a->readGDML(domele);
		}

		return true;
	}

	void StructureManager::clear()
	{
		for (auto d : _volumeList)
			delete d;
		for (auto a : _assemblyList)
			delete a;
		_volumeList.clear();
		_assemblyList.clear();
	}

	void StructureManager::writeGDML(QDomDocument& doc, QDomElement& root)
	{
		QDomElement stele = doc.createElement("structure");
		for (auto vol : _volumeList)
			vol->writeGDML(doc, stele);
		root.appendChild(stele);
	}

	

}