#include "SolidsManager.h"
#include <QDomNodeList>
#include <QDomElement>
#include "SolidBox.h"
#include "SolidSphere.h"
#include "SolidBoolean.h"
#include "SolidTube.h"

namespace Geant4 
{
	void SolidsManager::appendSolid(SolidShapeBase * s)
	{
		m_geometry_list.append(s);
	}


	int SolidsManager::getSolidCount()
	{
		return m_geometry_list.size();
	}

	SolidShapeBase* SolidsManager::getSolidAt(int index)
	{
		if (index >= 0 && index < m_geometry_list.size())
			return m_geometry_list.at(index);
		return nullptr;
	}

	Geant4::SolidShapeBase* SolidsManager::getSolidByName(QString name)
	{
		name = name.toLower();
		for (auto s : m_geometry_list)
		{
			if (s->getName().toLower() == name)
				return s;
		}
		return nullptr;
	}

	void SolidsManager::deleteSolidByName(QString name)
	{
		name = name.toLower();
		for (auto s : m_geometry_list)
		{
			if (s->getName().toLower() == name)
			{
				m_geometry_list.removeOne(s);
				return;
			}
		}
	}

	bool SolidsManager::isSolid(QString name)
	{
		name = name.toLower();
		for (auto s : m_geometry_list)
		{
			if (s->getName().toLower() == name)
				return true;
		}
		return false;
	}

	void SolidsManager::clear()
	{
		for (auto s : m_geometry_list)
			delete s;
		m_geometry_list.clear();
	}

/*
	bool SolidsManager::readGDML(QDomNodeList & nodes)
	{
		const int nc = nodes.count();
		if (nc != 1)return false;
		QDomElement solidele = nodes.at(0).toElement();
		QDomNodeList solidList = solidele.childNodes();
		const int nsolid = solidList.size();
		for (int i = 0; i < nsolid; ++i)
		{
			SolidShapeBase* solid = nullptr;
			QDomElement sele = solidList.at(i).toElement();
			QString name = sele.tagName();
			if ("box" == name)
				solid = new SolidBox;
			else if ("sphere" == name)
				solid = new SolidSphere;

			if (nullptr != solid)
			{this
				this->appendSolid(solid);
				solid->readGDML(sele);
				solid->generateShape();
			}
		}
		return true;
	}*/

	bool SolidsManager::readGDML(QDomNode & node)
	{
		QDomElement solidele = node.toElement();
		QDomNodeList solidList = solidele.childNodes();
		const int nsolid = solidList.size();
		for (int i = 0; i < nsolid; ++i)
		{
			SolidShapeBase* solid = nullptr;
			QDomElement sele = solidList.at(i).toElement();
			QString name = sele.tagName();
			if ("box" == name)
				solid = new SolidBox;
			else if ("sphere" == name)
				solid = new SolidSphere;
			else if ("tube" == name)
				solid = new SolidTube;
			else if (("subtraction" == name)||("union" == name)||("intersection" == name))
				solid = new SolidBoolean();

			if (nullptr != solid)
			{
				this->appendSolid(solid);
				solid->readGDML(sele);
				solid->generateShape();
			}
		}
		return true;
	}

	void SolidsManager::writeGDML(QDomDocument& doc, QDomElement& ele)
	{
		QDomElement solids = doc.createElement("solids");
		for (auto solid : m_geometry_list)
		{
			if(solid == nullptr) continue;
			solid->writeGDML(doc, solids);
		}
		ele.appendChild(solids);
	}

}