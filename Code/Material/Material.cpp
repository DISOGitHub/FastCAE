#include "Material.h"
#include <QDomDocument>
#include <QDomElement>
#include <QDomAttr>

namespace Material
{
	int Material::maxID = 0;
	Material::Material(bool IDplus) : _needID(IDplus)
	{
		if (IDplus)
		{
			maxID++;
			_id = maxID;
			this->appendProperty("ID", _id);
		}
		this->setModuleType(DataProperty::Module_Material);
	}

	void Material::setType(QString type)
	{
		if (type.simplified().isEmpty())
			return;
		_type = type;
		appendProperty("Type", type);
	}
	 
	QString Material::getType()
	{
		return _type;
	}

	void Material::readDataFromProjectFile(QDomElement* e)
	{ 
		if (_needID)
		{
			_id = e->attribute("ID").toInt();
			if (maxID < _id)
				maxID = _id;
			this->appendProperty("ID", _id);
		}
		this->setName(e->attribute("Name"));
		this->setType(e->attribute("Type"));

		DataProperty::DataBase::readDataFromProjectFile(e);
		this->generateParaInfo();
	}

	QDomElement& Material::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{
		QDomElement maele = doc->createElement("Material");
		maele.setAttribute("Name", _name);
		maele.setAttribute("Type", _type);
		if (_needID)
			maele.setAttribute("ID", _id);
		DataProperty::DataBase::writeParameters(doc, &maele);
		parent->appendChild(maele);
		return maele;
	}

	void Material::copy(DataBase* data)
	{
		Material* m = dynamic_cast<Material*>(data);
		if (m == nullptr) return;
		_type = m->getType();
		DataProperty::DataBase::copy(data);
		this->generateParaInfo();
	}

	int Material::getMaxID()
	{
		return maxID;
	}

	void Material::enableID(bool on)
	{
		_needID = on;
	}

}