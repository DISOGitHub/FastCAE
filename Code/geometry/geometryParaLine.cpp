#include "geometryParaLine.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDomAttr>
#include <QDebug>
namespace Geometry
{
	GeometryParaLine::GeometryParaLine()
	{
		_type = GeometryParaCreateLine;
	}

	void GeometryParaLine::setName(QString name)
	{
		_name = name;
	}

	QString GeometryParaLine::getName()
	{
		return _name;
	}

	void GeometryParaLine::setStartPoint(double* startpt)
	{
		for (int i = 0; i < 3; ++i)
			_startpoint[i] = startpt[i];
	}

	void GeometryParaLine::getStartPoint(double* startpt)
	{
		for (int i = 0; i < 3; ++i)
			startpt[i]=_startpoint[i];
	}

	void GeometryParaLine::setOptionIndex(int index)
	{
		_index = index;
	}

	int GeometryParaLine::getOptionIndex()
	{
		return _index;
	}

	void GeometryParaLine::setCoor(double* coor)
	{
		
		for (int i = 0; i < 3; i++)
		{
			_coor[i] = coor[i];
		}
	}

	void GeometryParaLine::getCoor(double* coor)
	{
		for (int i = 0; i < 3; i++)
		{
			coor[i] = _coor[i];
		}
	}

	void GeometryParaLine::setLength(double l)
	{
		_length = l;
	}

	double GeometryParaLine::getLength()
	{
			return _length;
	}

	void GeometryParaLine::setDirection(double* dir)
	{
		for (int i = 0; i < 3; ++i)
		{
			_direction[i] = dir[i];
		}
	}

	void GeometryParaLine::getDirection(double* dir)
	{
		for (int i = 0; i < 3; ++i)
		{
			dir[i] = _direction[i];
		}

	}

	void GeometryParaLine::setReverse(bool b)
	{
		_reverse = b;
	}

	bool GeometryParaLine::getReverse()
	{
		return _reverse;
	}

	QDomElement& GeometryParaLine::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{
		QDomElement element = doc->createElement("Parameter");  //创建子节点
		QDomAttr typeattr = doc->createAttribute("Type");
		typeattr.setValue(this->typeToString());
		element.setAttributeNode(typeattr);

		QDomAttr nameattr = doc->createAttribute("Name");
		nameattr.setValue(_name);
		element.setAttributeNode(nameattr);

		QDomElement startpointEle = doc->createElement("StartPoint");
		QString startpointstr = QString("%1,%2,%3").arg(_startpoint[0]).arg(_startpoint[1]).arg(_startpoint[2]);
		QDomText startpointText = doc->createTextNode(startpointstr);
		startpointEle.appendChild(startpointText);
		element.appendChild(startpointEle);

		QDomElement indexEle = doc->createElement("OptionIndex");
		QString indexStr = QString("%1").arg(_index);
		QDomText indexText = doc->createTextNode(indexStr);
		indexEle.appendChild(indexText);
		element.appendChild(indexEle);


		QDomElement coorEle = doc->createElement("Coor");
		QString coorstr = QString("%1,%2,%3").arg(_coor[0]).arg(_coor[1]).arg(_coor[2]);
		QDomText coorText = doc->createTextNode(coorstr);
		coorEle.appendChild(coorText);
		element.appendChild(coorEle);

		QDomElement lenEle = doc->createElement("Length");
		QString lenStr = QString("%1").arg(_length);
		QDomText lenText = doc->createTextNode(lenStr);
		lenEle.appendChild(lenText);
		element.appendChild(lenEle);

		QDomElement dirEle = doc->createElement("Direction");
		QString dirstr = QString("%1,%2,%3").arg(_direction[0]).arg(_direction[1]).arg(_direction[2]);
		QDomText dirText = doc->createTextNode(dirstr);
		dirEle.appendChild(dirText);
		element.appendChild(dirEle);

		QDomElement reverseEle = doc->createElement("Reverse");
		QString reverseStr = QString("%1").arg(_reverse);
		QDomText reverseText = doc->createTextNode(reverseStr);
		reverseEle.appendChild(reverseText);
		element.appendChild(reverseEle);

		parent->appendChild(element);
		return element;
	}

	void GeometryParaLine::readDataFromProjectFile(QDomElement* e)
	{
		_name = e->attribute("Name");
		QDomNodeList indexList = e->elementsByTagName("OptionIndex");
		_index = indexList.at(0).toElement().text().toDouble();

		QDomNodeList lenList = e->elementsByTagName("Length");
		_length = lenList.at(0).toElement().text().toDouble();

		QDomNodeList rList = e->elementsByTagName("Reverse");
		QString r = rList.at(0).toElement().text();
		if (r == "1") _reverse = true;
		else if (r == "0") _reverse = false;

		QDomNodeList coorList = e->elementsByTagName("Coor");
		if (coorList.size() != 1) return;
		QDomElement coorele = coorList.at(0).toElement();
		QString coorstr = coorele.text();
		QStringList coorsl = coorstr.split(",");
		if (coorsl.size() != 3) return;
		for (int i = 0; i < 3; ++i)
		{
			_coor[i] = coorsl.at(i).toDouble();
		}

		QDomNodeList startList = e->elementsByTagName("StartPoint");
		if (startList.size() != 1) return;
		QDomElement startele = startList.at(0).toElement();
		QString startstr = startele.text();
		QStringList startsl = startstr.split(",");
		if (startsl.size() != 3) return;
		for (int i = 0; i < 3; ++i)
		{
			_startpoint[i] = startsl.at(i).toDouble();
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