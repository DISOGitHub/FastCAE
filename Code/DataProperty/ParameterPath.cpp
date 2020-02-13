#include "ParameterPath.h"
#include <QDomDocument>
#include <QDomAttr>
#include <QDomElement>
#include <QDomText>
#include <QDomNodeList>

namespace DataProperty
{
	ParameterPath::ParameterPath()
		: ParameterBase(Para_Path)
	{
	}
	void ParameterPath::setType(PathType t)
	{
		_type = t;
	}
	PathType ParameterPath::getType()
	{
		return _type;
	}
	void ParameterPath::setSuffix(QString s)
	{
		_suffix = s;
	}
	QString ParameterPath::getSuffix()
	{
		return _suffix;
	}
	void ParameterPath::setFile(QString f)
	{
		_file = f;
	}
	QString ParameterPath::getFile()
	{
		return _file;
	}
	void ParameterPath::setFileList(QStringList sl)
	{
		_files = sl;
	}
	QStringList ParameterPath::getFileList()
	{
		return _files;
	}
	void ParameterPath::setPath(QString s)
	{
		_path = s;
	}

	QString ParameterPath::getPath()
	{
		return _path;
	}
	void ParameterPath::writeParameter(QDomDocument* doc, QDomElement* parent)
	{
		ParameterBase::writeParameter(doc, parent);
		QDomAttr suffixAttr = doc->createAttribute("Suffix");
		suffixAttr.setValue(_suffix);
		parent->setAttributeNode(suffixAttr);

		QString stype;
		switch (_type)
		{
		case File: stype = "File"; break;
		case Path: stype = "Path"; break;;
		case FileList: stype = "FileList"; break;
		default: break;
		}
		QString svalue;
		switch (_type)
		{
		case File: 
			svalue = _file;
			break;
		case Path: 
			svalue = _path;
			break;;
		case FileList:
			for (int i = 0; i < _files.size(); ++i)
			{
				QString s = _files.at(i);
				svalue.append(s);
				svalue.append(";");
			}
			svalue.resize(svalue.size() - 1);
			break;
		default: break;
		}

		QDomElement typeele = doc->createElement("Type");
		QDomText domtexttype = doc->createTextNode(stype);
		typeele.appendChild(domtexttype);
		parent->appendChild(typeele);

		QDomElement valueele = doc->createElement("Text");
		QDomText domvalue = doc->createTextNode(svalue);
		valueele.appendChild(domvalue);
		parent->appendChild(valueele);
	}

	void ParameterPath::readParameter(QDomElement* e)
	{
		ParameterBase::readParameter(e);
		_suffix = e->attribute("Suffix");

		QDomNodeList typelist = e->elementsByTagName("Type");
		if (typelist.size() != 1) return;
		QString stype = typelist.at(0).toElement().text().toLower();

		if (stype == "file") _type = File;
		else if (stype == "path" || stype == "dictionary") _type = Path;
		else if (stype == "filelist" ) _type = FileList;

		QString text;
		QDomNodeList textlist = e->elementsByTagName("Text");
		if (textlist.size() == 1)
		text = textlist.at(0).toElement().text();
		textlist = e->elementsByTagName("Data");
		if (textlist.size() == 1)
		text = textlist.at(0).toElement().text();
		if (text.isEmpty()) return;

		switch (_type)
		{
		case File: _file = text; break;
		case Path: _path = text; break;
		case FileList: _files = text.split(";"); break;
		default: break;
		}
	}
	void ParameterPath::copy(ParameterBase* ori)
	{
		ParameterBase::copy(ori);
		ParameterPath* p = (ParameterPath*)ori;

		_type = p->getType();
		_suffix = p->getSuffix();
		_file = p->getFile();
		_path = p->getPath();
		_files = p->getFileList();
	}

	QString ParameterPath::valueToString()
	{
		QString t;
		switch (_type)
		{
		case DataProperty::None:
			break;
		case DataProperty::Path:
			t = _path; break;
		case DataProperty::File:
			t = _file; break;
		case DataProperty::FileList:
			t = _files.join(","); break;
		default:
			break;
		}
		return t;
	}

	void ParameterPath::setValueFromString(QString v)
	{
		switch (_type)
		{
		case DataProperty::None:
			break;
		case DataProperty::Path:
			_path = v;  break;
		case DataProperty::File:
			_file = v; break;
		case DataProperty::FileList:
			_files = v.split(","); break;
		default:
			break;
		}
	}
}
