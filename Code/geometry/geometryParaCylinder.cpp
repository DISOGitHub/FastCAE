#include "geometryParaCylinder.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDomAttr>
#include <QDebug>
namespace Geometry
{
	GeometryParaCylinder::GeometryParaCylinder()
	{
		_type = GeometryParaCreateCylinder;
	}

	void GeometryParaCylinder::setName(QString name)
	{
		_name = name;
	}

	QString GeometryParaCylinder::getName()
	{
		return _name;
		
	}

	void GeometryParaCylinder::setLocation(double* loc)
	{
		for (int i = 0; i < 3; ++i)
			_location[i] = loc[i];
	}

	void GeometryParaCylinder::getLocation(double* loc)
	{
		for (int i = 0; i < 3; ++i)
			loc[i] = _location[i];
	}

	void GeometryParaCylinder::setRadius(double* r)
	{
		_radius = *r;
	}

	void GeometryParaCylinder::getRadius(double* r)
	{
		*r = _radius;
	}

	void GeometryParaCylinder::setLength(double* l)
	{
		_length = *l;
	}

	void GeometryParaCylinder::getLength(double* l)
	{
		*l = _length;
	}

	void GeometryParaCylinder::setDirection(double* dir)
	{
		for (int i = 0; i < 3;++i)
		{
			_direction[i] = dir[i];
		}
	}

	void GeometryParaCylinder::getDirection(double* dir)
	{
		for (int i = 0; i < 3;++i)
		{
			dir[i] = _direction[i];
		}

	}

	QDomElement& GeometryParaCylinder::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{
		QDomElement element = doc->createElement("Parameter");  //创建子节点
		QDomAttr typeattr = doc->createAttribute("Type");
		typeattr.setValue(this->typeToString());
		element.setAttributeNode(typeattr);

		QDomAttr nameattr = doc->createAttribute("Name");
		nameattr.setValue(_name);
		element.setAttributeNode(nameattr);

		QDomElement locationEle = doc->createElement("Location");
		QString locstr = QString("%1,%2,%3").arg(_location[0]).arg(_location[1]).arg(_location[2]);
		QDomText locText = doc->createTextNode(locstr);
		locationEle.appendChild(locText);
		element.appendChild(locationEle);

		QDomElement radiusEle = doc->createElement("Radius");
		QString radiusStr = QString("%1").arg(_radius);
		QDomText rText = doc->createTextNode(radiusStr);
		radiusEle.appendChild(rText);
		element.appendChild(radiusEle);

		QDomElement lenEle = doc->createElement("Length");
		QString lenStr = QString("%1").arg(_length);
		QDomText lenText = doc->createTextNode(lenStr);
		lenEle.appendChild(lenText);
		element.appendChild(lenEle);

		QDomElement directionEle = doc->createElement("Direction");
		QString directiontr = QString("%1,%2,%3").arg(_direction[0]).arg(_direction[1]).arg(_direction[2]);
		QDomText dirText = doc->createTextNode(directiontr);
		directionEle.appendChild(dirText);
		element.appendChild(directionEle);

		parent->appendChild(element);
		return element; 
	}

	void GeometryParaCylinder::readDataFromProjectFile(QDomElement* e)
	{
		_name = e->attribute("Name");
		QDomNodeList radiusList = e->elementsByTagName("Radius");
		_radius= radiusList.at(0).toElement().text().toDouble();

		QDomNodeList lengthList = e->elementsByTagName("Length");
		_length = lengthList.at(0).toElement().text().toDouble();

		QDomNodeList locList = e->elementsByTagName("Location");
		if (locList.size() != 1) return;
		QDomElement locele = locList.at(0).toElement();
		QString locstr = locele.text();
		QStringList locsl = locstr.split(",");
		if (locsl.size() != 3) return;
		for (int i = 0; i < 3; ++i)
		{
			_location[i] = locsl.at(i).toDouble();
 		}
		
		QDomNodeList paraList = e->elementsByTagName("Direction");
		if (paraList.size() != 1) return;
		QDomElement paraele = paraList.at(0).toElement();
		QString parastr = paraele.text();
		QStringList parasl = parastr.split(",");
		if (parasl.size() != 3) return;
		for (int i = 0; i < 3; ++i)
		{
			_direction[i] = parasl.at(i).toDouble();
		}



	}

	

}