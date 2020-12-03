#include "Volume.h"
#include <QDomElement>
#include "Physvol.h"
#include <QDebug>
namespace Geant4
{

	QString Volume::getName()
	{
		return _name;
	}

	void Volume::setMaterialRef(QString m)
	{
		_materialRef = m;
	}

	QString Volume::getMaterialRef()
	{
		return _materialRef;
	}



	void Volume::setSolidRef(QString ref)
	{
		_solidRef = ref;
	}

	QString Volume::getSolidRef()
	{
		return _solidRef;
	}

	int Volume::getPhysvolCount()
	{
		return _physvols.size();
	}

	Physvol* Volume::getPhysvolAt(int index)
	{
		return _physvols.at(index);
	}

	void Volume::readGDML(QDomElement& e)
	{
		_name = e.attribute("name");
		this->appendProperty("Name", _name);
		QDomNodeList malist = e.elementsByTagName("materialref");
		if (malist.size() != 1)return;
		_materialRef = malist.at(0).toElement().attribute("ref");
		this->appendProperty("MaterialRef", _materialRef);
		QDomNodeList solidList =  e.elementsByTagName("solidref");
		if (solidList.size() != 1) return;
		_solidRef = solidList.at(0).toElement().attribute("ref");
		this->appendProperty("SolidRef", _solidRef);
		QDomNodeList phylist = e.elementsByTagName("physvol");
		const int n = phylist.size();
		this->appendProperty("PhysvolCount", n);
		for (int i = 0; i < n; ++i)
		{
			QDomElement e = phylist.at(i).toElement();
			Physvol* p = new Physvol;
			p->readGDML(e);
			_physvols.append(p);
		}

	}

	void Volume::writeGDML(QDomDocument& doc, QDomElement& parent)
	{
		QDomElement volele = doc.createElement("volume");
		volele.setAttribute("name", _name);
		QDomElement mf = doc.createElement("materialref");
		mf.setAttribute("ref", _materialRef);
		QDomElement sf = doc.createElement("solidref");
		sf.setAttribute("ref",_solidRef);
		volele.appendChild(mf);
		volele.appendChild(sf);

		for (auto phy : _physvols)
			phy->writeGDML(doc, volele);
		
		parent.appendChild(volele);


	}

}



