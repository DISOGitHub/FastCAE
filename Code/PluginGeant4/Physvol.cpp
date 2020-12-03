#include "Physvol.h"
#include <QDomElement>
#include <QDomNodeList>
#include "Value3D.h"
#include "DataManager.h"
#include "DefineManager.h"
#include "StructureManager.h"
#include "Volume.h"
#include "PreDefineBase.h"
#include <QDebug>

namespace Geant4
{
	void Physvol::setName(const QString& name)
	{
		_name = name;
		if (name.isEmpty()) return;
		this->appendProperty("Name", name);
	}

	void Physvol::readGDML(QDomElement& e)
	{
		QString name = e.attribute("name");
		this->setName(name);

		QDomNodeList nodes = e.elementsByTagName("volumeref");
		if (nodes.size() == 1)
		{
			_volRef = nodes.at(0).toElement().attribute("ref");
			this->appendProperty("VolumeRef", _volRef);
			qDebug() << _volRef;
		}
		nodes = e.elementsByTagName("positionref");
		if (nodes.size() == 1)
		{
			_posRef = nodes.at(0).toElement().attribute("ref");
			this->appendProperty("PositionRef", _posRef);
//			qDebug() << _posRef;
		}
			
		nodes = e.elementsByTagName("rotationref");
		if (nodes.size() == 1)
		{
			_rotRef = nodes.at(0).toElement().attribute("ref");
			this->appendProperty("RotationRef", _rotRef);
//			qDebug() << _rotRef;
		}
			
		nodes = e.elementsByTagName("position");
		if (nodes.size() == 1)
		{
			QDomElement ele = nodes.at(0).toElement();
			_pos = new Value3D;
			_pos->setName(ele.attribute("name"));
			_pos->setUnit(ele.attribute("unit"));
			double x = ele.attribute("x").toDouble();
			double y = ele.attribute("y").toDouble();
			double z = ele.attribute("z").toDouble();
			_pos->setValue(x, y, z);
			this->appendProperty("Position", x, y, z);
		}
		nodes = e.elementsByTagName("rotation");
		if (nodes.size() == 1)
		{
			QDomElement ele = nodes.at(0).toElement();
			_rot = new Value3D;
			_rot->setName(ele.attribute("name"));
			_rot->setUnit(ele.attribute("unit"));
			double x = ele.attribute("x").toDouble();
			double y = ele.attribute("y").toDouble();
			double z = ele.attribute("z").toDouble();
			_rot->setValue(x, y, z);
			this->appendProperty("Rotation", x, y, z);
		}

	}



	QString Physvol::getVolRef()
	{
		return _volRef;
	}

	QString Physvol::getSolidRef()
	{
		auto manager = DataManager::getInstance()->getStructManager();
		Volume* v = manager->getVolumeByName(_volRef);
		if (v == nullptr) return QString();
		return v->getSolidRef();

	}

	bool Physvol::getPosition(double* d)
	{
		if (_pos != nullptr)
		{
			_pos->getValue(d);
			return true;
		}
		else
		{
			auto ma = DataManager::getInstance()->getDefineManager();
			auto def = ma->getPreDefineByName(_posRef);
			if (def == nullptr) return false;
			def->getValue(d);
			return true;
		}
		return false;
	}

	bool Physvol::getRotation(double* d)
	{
		if (_rot != nullptr)
		{
			_rot->getValue(d);
			return true;
		}
		else
		{
//TODO			return DataManager::getInstance()->getDefineManager()->getRotation(_posRef, d);
		}

		return false;
	}

	int Physvol::getType()
	{
		auto sm = DataManager::getInstance()->getStructManager();
		return  sm->getType(_volRef);
	}

	void Physvol::connectToGeometrySet(Geometry::GeometrySet * s)
	{
		_connectedShape = s;
	}

	Geometry::GeometrySet * Physvol::getConnectedGeoSet()
	{
		return _connectedShape;
	}

	void Physvol::writeGDML(QDomDocument& doc, QDomElement& volele)
	{
		QDomElement phyele = doc.createElement("physvol");
		phyele.setAttribute("name", _name);

		QDomElement volref = doc.createElement("volumeref");
		volref.setAttribute("ref", _volRef);
		phyele.appendChild(volref);

		if (!_posRef.isEmpty())
		{
			QDomElement posref = doc.createElement("positionref");
			posref.setAttribute("ref", _posRef);
			phyele.appendChild(posref);
		}
		else
		{
			//TODO
		}

		if (!_rotRef.isEmpty())
		{
			QDomElement rotref = doc.createElement("rotationref");
			rotref.setAttribute("ref", _rotRef);
			phyele.appendChild(rotref);
		}
		else
		{
			//TODO
		}

		volele.appendChild(phyele);
	}
}