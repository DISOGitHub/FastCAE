#include "BCConfigReader.h"
#include "ConfigOptions.h"
#include "BCConfig.h"
#include "BCBase/BCUserDef.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDomNodeList>
#include <QDebug>

namespace ConfigOption
{
	BCConfigReader::BCConfigReader(const QString fileName, BCConfig* bcconfig) /*:IOBase(fileName)*/
	{
//		_bcconfig = ConfigOption::getInstance()->getBCConfig();
		_file.setFileName(fileName);
		_bcconfig = bcconfig;
	}
	BCConfigReader::~BCConfigReader()
	{
		if (_doc != nullptr)
		{
			delete _doc;
		}
	}
	bool BCConfigReader::read()
	{
		if (!_file.open(QFile::ReadOnly | QFile::Text)) return false;
		_doc = new QDomDocument();
		if (!_doc->setContent(&_file))
		{
			_file.close();
			return false;
		}

		QDomNodeList bclist = _doc->elementsByTagName("BCList");
		for (int i = 0; i < bclist.size(); ++i)
		{
			QDomElement bclistele = bclist.at(i).toElement();
			QString stype = bclistele.attribute("Type");
			ProjectTreeType type = getTreeTypeByString(stype);
			if (type == UnDefined) continue;
			this->readBCType(&bclistele, type);
		}
//		qDebug() << bclist.size();
	
		return true;
	}
	void BCConfigReader::readBCType(QDomElement* ele, ProjectTreeType t)
	{
		QDomNodeList bclist = ele->elementsByTagName("BC");

		for (int i = 0; i < bclist.size(); ++i)
		{
			QDomElement ele = bclist.at(i).toElement();
			this->readBC(&ele,t);
		}
		QDomNodeList enabletypelist = ele->elementsByTagName("EnableDefaultType");
		for (int i = 0; i < enabletypelist.size(); ++i)
		{
			QDomElement eleable = enabletypelist.at(i).toElement();
			QString text = eleable.text().toLower();
			QStringList enablelist = text.split(",");
			for (int j = 0; j < enablelist.size(); ++j)
			{
				QString s = enablelist.at(j).toLower();
				if (s.isEmpty()) continue;
				_bcconfig->appendEnableType(s,t);
			}

		}

	}

	void BCConfigReader::readBC(QDomElement* ele, ProjectTreeType t)
	{
		BCBase::BCUserDef* bc = new BCBase::BCUserDef;
		bc->readDataFromProjectFile(ele);
		_bcconfig->appendBC(bc, t);
	}

}