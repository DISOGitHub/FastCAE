#include "BCBase.h"
#include "meshData/meshSingleton.h"
#include "meshData/meshSet.h"
#include "geometry/GeoComponent.h"
#include "geometry/geometryData.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDomAttr>
#include <QObject>
#include <QDebug>

namespace BCBase
{
	BCBase::BCBase()
	{
//		setID(++BCID);
		_mesh = MeshData::MeshData::getInstance();
		this->setModuleType(DataProperty::Module_BC);
	}
	void BCBase::bingdingComponentID(int id)
	{
		QString qType{};
		auto set = _mesh->getMeshSetByID(id);
		if (set)
		{			
			_component = set;
			qType = MeshData::MeshSet::setTypeToString(set->getSetType());
			appendProperty("MeshSetID", id);
			appendProperty("MeshSetType", qType);
		}
		else
		{
			auto gc = Geometry::GeometryData::getInstance()->getGeoComponentByID(id);
			if (!gc)	return;
			_component = gc;
			qType = Geometry::GeoComponent::gcTypeToString(gc->getGCType());
			appendProperty("GeoComponentID", id);
			appendProperty("GeoComponentType", qType);
		}
		_ComponentID = id;
	}
	int BCBase::getComponentID()
	{
		return _ComponentID;
	}
	DataProperty::ComponentBase* BCBase::getComponent()
	{
		return _component;
	}
	QString BCBase::getComponentName()
	{
		if (_component == nullptr)
			return "FFFFFF";
		else
			return _component->getName();
	}
	void BCBase::setBCType(BCType t)
	{
		_BCtype = t;
		appendProperty(QObject::tr("BCType"), BCTypeToString(t));
	}
	BCType BCBase::getBCType()
	{
		return _BCtype;
	}
	QDomElement& BCBase::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{
		QString st = BCTypeToString(_BCtype);
		parent->setAttribute("BCType", st);
		QDomElement cpIDEle = doc->createElement("BCComponentInfo");
		cpIDEle.setAttribute("ComponentID", _ComponentID);
		parent->appendChild(cpIDEle);
		return *parent;
	}
	void BCBase::readDataFromProjectFile(QDomElement* bcele)
	{
		QString stype = bcele->attribute("BCType");
		BCType type = StringToBCType(stype);
		setBCType(type);
		QDomElement cpInfoEle = bcele->elementsByTagName("BCComponentInfo").at(0).toElement();
		QString sID = cpInfoEle.attribute("ComponentID");
		bingdingComponentID(sID.toInt());
	}
	void BCBase::copy(DataBase* d)
	{
		BCBase* data = (BCBase*)d;
		this->setBCType(data->getBCType());
		this->bingdingComponentID(data->getComponentID());
        DataBase::copy(d);
		
	}

	void BCBase::setComponent(DataProperty::ComponentBase* cp)
	{
		_component = cp;
	}
}
