#include "geometryParaFace.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDomAttr>
#include <QDebug>
#include "geometrySet.h"
#include "geometryData.h"

namespace Geometry
{
	GeometryParaFace::GeometryParaFace()
	{
		_type = GeometryParaCreateFace;
	}

	void GeometryParaFace::setName(QString name)
	{
		_name = name;
	}

	QString GeometryParaFace::getName()
	{
		return _name;
	}

	void GeometryParaFace::setShapeHash(QMultiHash<Geometry::GeometrySet*, int> shapehash)
	{
		_shapeHash = shapehash;
	}

	QMultiHash<Geometry::GeometrySet*, int> GeometryParaFace::getShapeHash()
	{
		return _shapeHash;
	}

	QDomElement& GeometryParaFace::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{
		QDomElement element = doc->createElement("Parameter");  //创建子节点
		QDomAttr typeattr = doc->createAttribute("Type");
		typeattr.setValue(this->typeToString());
		element.setAttributeNode(typeattr);

		QDomAttr nameattr = doc->createAttribute("Name");
		nameattr.setValue(_name);
		element.setAttributeNode(nameattr);

		QStringList solidStrList{};
		if (_shapeHash.size() > 0)
		{
			QMultiHash<Geometry::GeometrySet*, int>::iterator it = _shapeHash.begin();
			for (; it != _shapeHash.end(); it++)
			{
				solidStrList << QString("%1:%2").arg(it.key()->getID()).arg(it.value());
			}
			QDomElement startpointEle = doc->createElement("EdgeList");
			QDomText startpointText = doc->createTextNode(solidStrList.join(","));
			startpointEle.appendChild(startpointText);
			element.appendChild(startpointEle);
		}

		parent->appendChild(element);
		return element;
	}

	void GeometryParaFace::readDataFromProjectFile(QDomElement* e)
	{
		_name = e->attribute("Name");
		
		Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
		QDomNodeList setIdList = e->elementsByTagName("EdgeList");
		if (setIdList.size() != 1) return;
		QDomElement coorele = setIdList.at(0).toElement();
		QString coorstr = coorele.text();
		QStringList coorsl = coorstr.split(",");
		if (coorsl.size() < 1) return;
		for (int i = 0; i < coorsl.size(); ++i)
		{
			QStringList solidstr = coorsl[i].split(":");
			int setid = solidstr.at(0).toInt();
			int solidindex = solidstr.at(1).toInt();
			Geometry::GeometrySet *set = data->getGeometrySetByID(setid);
			_shapeHash.insert(set, solidindex);
		}


	}

	

}