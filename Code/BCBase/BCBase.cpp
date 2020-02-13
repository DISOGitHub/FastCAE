#include "BCBase.h"
#include "meshData/meshSingleton.h"
#include "meshData/meshSet.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDomAttr>
#include <QObject>

namespace BCBase
{
	BCBase::BCBase()
	{
		_mesh = MeshData::MeshData::getInstance();
		this->setModuleType(DataProperty::Module_BC);
	}
	void BCBase::setMeshSetID(int id)
	{
		_meshSetID = id;
		_set = _mesh->getMeshSetByID(id);
		appendProperty(QObject::tr("Set"), id);
	}
	int BCBase::getMeshSetID()
	{
		return _meshSetID;
	}
	MeshData::MeshSet* BCBase::getMeshSet()
	{
		return _set;
	}
	QString BCBase::getMeshSetName()
	{
		if (_set == nullptr)
			return "FFFFFF";
		else
			return _set->getName();
	}
	void BCBase::setType(BCType t)
	{
		_type = t;
		appendProperty(QObject::tr("Type"), BCTypeToString(t));
	}
	BCType BCBase::getType()
	{
		return _type;
	}
	QDomElement& BCBase::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{
//		QDomElement bcele = doc->createElement("BC");
		QDomAttr setidattr = doc->createAttribute("SetID");
		setidattr.setValue(QString("%1").arg(_meshSetID));
		QDomAttr typeattr = doc->createAttribute("Type");
		QString st = BCTypeToString(_type);
		typeattr.setValue(st);

		parent->setAttributeNode(setidattr);
		parent->setAttributeNode(typeattr);

//		parent->appendChild(bcele);
		return *parent;
	}
	void BCBase::readDataFromProjectFile(QDomElement* bcele)
	{
		QString sid = bcele->attribute("SetID");
		this->setMeshSetID(sid.toInt());
		QString stype = bcele->attribute("Type");
		BCType type = StringToBCType(stype);
		this->setType(type);
	}
	void BCBase::copy(DataBase* d)
	{
		BCBase* data = (BCBase*)d;
		this->setType(data->getType());
		this->setMeshSetID(data->getMeshSetID());
		DataBase::copy(d);
	}
	
}