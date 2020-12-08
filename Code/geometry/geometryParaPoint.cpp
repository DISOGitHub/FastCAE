#include "geometryParaPoint.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDomAttr>

namespace Geometry
{
	GeometryParaPoint::GeometryParaPoint()
	{
		_type = GeometryParaCreatePoint;
	}

	void GeometryParaPoint::setName(QString name)
	{
		_name = name;
	}

	QString GeometryParaPoint::getName()
	{
		return _name;
	}

	void GeometryParaPoint::setPara(double* para)
	{
		for (int i = 0; i < 3; ++i)
			_para[i] = para[i];
	}

	void GeometryParaPoint::getPara(double* para)
	{
		for (int i = 0; i < 3; ++i)
		{
			para[i] = _para[i];
		}
	}

	void GeometryParaPoint::setCorner(double* corner)
	{
		for (int i = 0; i < 3; ++i)
			_corner[i] = corner[i];
	}

	void GeometryParaPoint::getCorner(double* corner)
	{
		for (int i = 0; i < 3; ++i)
		{
			corner[i] = _corner[i];
		}
	}

	QDomElement& GeometryParaPoint::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{
		QDomElement element = doc->createElement("Parameter");  //创建子节点
		QDomAttr typeattr = doc->createAttribute("Type");
		typeattr.setValue(this->typeToString());
		element.setAttributeNode(typeattr);

		QDomAttr nameattr = doc->createAttribute("Name");
		nameattr.setValue(_name);
		element.setAttributeNode(nameattr);

		QDomElement cornerEle = doc->createElement("Corner");
		QString corstr = QString("%1,%2,%3").arg(_corner[0]).arg(_corner[1]).arg(_corner[2]);
		QDomText corText = doc->createTextNode(corstr);
		cornerEle.appendChild(corText);
		element.appendChild(cornerEle);

		QDomElement paraEle = doc->createElement("Para");
		QString parastr = QString("%1,%2,%3").arg(_para[0]).arg(_para[1]).arg(_para[2]);
		QDomText paraText = doc->createTextNode(parastr);
		paraEle.appendChild(paraText);
		element.appendChild(paraEle);

		parent->appendChild(element);
		return element;
	}

	void GeometryParaPoint::readDataFromProjectFile(QDomElement* e)
	{
		_name = e->attribute("Name");
		QDomNodeList cList = e->elementsByTagName("Corner");
		if (cList.size() != 1) return;
		QDomElement cele = cList.at(0).toElement();
		QString cstr =cele.text();
		QStringList csl = cstr.split(",");
		if (csl.size() != 3) return;
		for (int i = 0; i < 3; ++i)
		{
			_corner[i] = csl.at(i).toDouble();
		}

		QDomNodeList pList = e->elementsByTagName("Para");
		if (pList.size() != 1) return;
		QDomElement pele = pList.at(0).toElement();
		QString pstr = pele.text();
		QStringList psl = pstr.split(",");
		if (psl.size() != 3) return;
		for (int i = 0; i < 3; ++i)
		{
			_para[i] = psl.at(i).toDouble();
		}
	}

}