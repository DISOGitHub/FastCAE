#include "MesherInfo.h"
#include <QDomElement>
#include <QDomAttr>
#include "python/PyAgent.h"
#include <QDebug>

namespace ConfigOption
{
	MesherInfo::MesherInfo()
	{
		this->setModuleType(DataProperty::Module_Mesher);
	}

	QString MesherInfo::getMesherName()
	{
		return _mesherName;
	}

	void MesherInfo::readParameters(QDomElement* ele)
	{
		_mesherName = ele->attribute("Name");
		this->appendProperty("Mesher",_mesherName);
		_template = ele->attribute("Template");
		_output = ele->attribute("Output");
		DataBase::readParameters(ele);
	}

	QString MesherInfo::getTemplate()
	{
		return _template;
	}
	
	QString MesherInfo::getOutput()
	{
		return _output;
	}

	void MesherInfo::writeParameters(QDomDocument* doc, QDomElement* parent)
	{
		QDomElement mesherEle = doc->createElement("Mesher");
		mesherEle.setAttribute("Name", _mesherName);
		mesherEle.setAttribute("Template", _template);
		mesherEle.setAttribute("Output", _output);

		DataBase::writeParameters(doc, &mesherEle);
		parent->appendChild(mesherEle);
	}

}
