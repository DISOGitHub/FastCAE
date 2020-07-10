#include "geometryParaSphere.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDomAttr>

namespace Geometry
{
	GeometryParaSphere::GeometryParaSphere()
	{
		_type = GeometryParaCreateSphere;
	}

	void GeometryParaSphere::setName(QString name)
	{
		_name = name;
	}

	QString GeometryParaSphere::getName()
	{
		return _name;
	}
	void GeometryParaSphere::setRadius(double r)
	{
		_radius = r;
	}

	double GeometryParaSphere::getRadius()
	{
		return _radius;
	}

	void GeometryParaSphere::setLocation(double* loc)
	{
		for (int i = 0; i < 3; ++i)
			_location[i] = loc[i];
	}

	void GeometryParaSphere::getLocation(double* loc)
	{
		for (int i = 0; i < 3; ++i)
			loc[i] = _location[i];
	}

	QDomElement& GeometryParaSphere::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
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

		parent->appendChild(element);
		return element;
	}

	void GeometryParaSphere::readDataFromProjectFile(QDomElement* e)
	{
		_name = e->attribute("Name");
		QDomNodeList radiusList = e->elementsByTagName("Radius");
		_radius = radiusList.at(0).toElement().text().toDouble();
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

	}



}