#include "MaterialManager.h"
#include <QDomElement>
#include <QDomNodeList>
#include "GDMLIsotope.h"
#include "GDMLElement.h"
#include "GDMLMaterial.h"
#include "DataManager.h"
#include "DefineManager.h"

namespace Geant4 
{
	void Geant4::MaterialManager::appendMaterial(GDMLMaterial* mat)
	{
		_matlist.append(mat);
	}

	void MaterialManager::appendElement(GDMLElement* ele)
	{
		_elelist.append(ele);
	}

	void MaterialManager::appendIsotope(GDMLIsotope* iso)
	{
		_isotopelist.append(iso);
	}

	int MaterialManager::getElementCount()
	{
		return _elelist.size();
	}

	int MaterialManager::getMaterialCount()
	{
		return _matlist.size();
	}

	int MaterialManager::getIsotopeCount()
	{
		return _isotopelist.size();
	}

	QList<GDMLElement*> MaterialManager::getElementList()
	{
		return _elelist;
	}

	GDMLElement* MaterialManager::getElementByIndex(int index)
	{
		if (index < 0 || index >= _elelist.size())
			return nullptr;

		return _elelist.at(index);
	}

	QList <GDMLMaterial*> MaterialManager::getMaterialList()
	{
		return _matlist;
	}

	GDMLMaterial* MaterialManager::getMaterialByIndex(int index)
	{
		if (index < 0 || index >= _matlist.size())
			return nullptr;

		return _matlist.at(index);
	}
	
	QList <GDMLIsotope*> MaterialManager::getIsotopeList()
	{
		return _isotopelist;
	}

	Geant4::GDMLIsotope* MaterialManager::getIsotopeByIndex(int index)
	{
		if (index < 0 || index >= _isotopelist.size())
			return nullptr;

		return _isotopelist.at(index);
	}

	/*void MaterialManager::readGDML(QDomNodeList &nodes)
	{
		if (nodes.size() != 1) return;
		QDomElement masele = nodes.at(0).toElement();

		QDomNodeList delist = masele.elementsByTagName("define");
		DataManager::getInstance()->getDefineManager()->readGDML(delist);

		QDomNodeList isolist = masele.elementsByTagName("isotope");
		int n = isolist.size();
		for (int i = 0; i < n; ++i)
		{
			QDomElement isoele = isolist.at(i).toElement();
			auto isp = new GDMLIsotope;
			isp->readGDML(isoele);
			_isotopelist.append(isp);
		}

		QDomNodeList eleList = masele.elementsByTagName("element");
		n = eleList.size();
		for (int i=0;i<n; ++i)
		{
			QDomElement ele = eleList.at(i).toElement();
			auto e = new GDMLElement;
			e->readGDML(ele);
			_elelist.append(e);
		}

		QDomNodeList maList = masele.elementsByTagName("material");
		n = maList.size();
		for (int i = 0; i < n; ++i)
		{
			QDomElement ele = maList.at(i).toElement();
			auto e = new GDMLMaterial;
			e->readGDML(ele);
			_matlist.append(e);
		}
		
		
	}*/

	void MaterialManager::readGDML(QDomNode &node)
	{
		QDomElement masele = node.toElement();
		QDomNodeList delist = masele.elementsByTagName("define");
		if (delist.size()>0)
			DataManager::getInstance()->getDefineManager()->readGDML(delist);

		QDomNodeList isolist = masele.elementsByTagName("isotope");
		int n = isolist.size();
		for (int i = 0; i < n; ++i)
		{
			QDomElement isoele = isolist.at(i).toElement();
			auto isp = new GDMLIsotope;
			isp->readGDML(isoele);
			_isotopelist.append(isp);
		}

		QDomNodeList eleList = masele.elementsByTagName("element");
		n = eleList.size();
		for (int i = 0; i < n; ++i)
		{
			QDomElement ele = eleList.at(i).toElement();
			auto e = new GDMLElement;
			e->readGDML(ele);
			_elelist.append(e);
		}

		QDomNodeList maList = masele.elementsByTagName("material");
		n = maList.size();
		for (int i = 0; i < n; ++i)
		{
			QDomElement ele = maList.at(i).toElement();
			auto e = new GDMLMaterial;
			e->readGDML(ele);
			_matlist.append(e);
		}

	}



	void MaterialManager::writeGDML(QDomDocument& doc, QDomElement& root)
	{
		QDomElement materials = doc.createElement("materials");
		if (_elelist.size()>0)
		{
			this->writeGDMLElement(doc, materials);
		}
		if (_isotopelist.size()>0)
		{
			this->writeGDMLIsotope(doc, materials);
		}
		if (_matlist.size()>0)
		{
			this->writeGDMLMaterial(doc, materials);
		}

		root.appendChild(materials);

	}


	void MaterialManager::writeGDMLElement(QDomDocument& doc, QDomElement & materials)
	{
		for (int i = 0; i < _elelist.size(); i++)
		{
			GDMLElement* ele = _elelist[i];
			ele->writeGDML(doc, materials);
			
		}

	}
	void MaterialManager::writeGDMLIsotope(QDomDocument& doc, QDomElement & materials)
	{
		for (int i = 0; i < _isotopelist.size(); i++)
		{
			GDMLIsotope* iso = _isotopelist[i];
			iso->writeGDML(doc, materials);
		}

	}
	void MaterialManager::writeGDMLMaterial(QDomDocument& doc, QDomElement & materials)
	{
		for (int i = 0; i < _matlist.size(); i++)
		{
			GDMLMaterial* onematerial = _matlist[i];
			QDomElement material = doc.createElement("material");

			material.setAttribute("name", onematerial->getName());
			material.setAttribute("formula", onematerial->getFormula());
			material.setAttribute("Z", onematerial->getZ());
			material.setAttribute("state", onematerial->getState());
			QDomElement atom = doc.createElement("atom");
			atom.setAttribute("value", onematerial->getValue());
			material.appendChild(atom);

			onematerial->writeGDML(doc, material);
			materials.appendChild(material);
		}
	}

	void MaterialManager::clear()
	{
		for (auto a : _isotopelist) delete a;
		for (auto a : _elelist) delete a;
		for (auto a : _matlist) delete a;
		_isotopelist.clear();
		_elelist.clear();
		_matlist.clear();
	}

}

