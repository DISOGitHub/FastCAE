#include "geometryParaExtrusion.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDomAttr>
#include <QDebug>
#include "geometrySet.h"
#include "geometryData.h"

namespace Geometry
{
	GeometryParaExtrusion::GeometryParaExtrusion()
	{
		_type = GeometryParaMakeExtrusion;
	}
	
	void GeometryParaExtrusion::setShapeHash(QMultiHash<Geometry::GeometrySet*, int> shapehash)
	{
		_shapeHash = shapehash;
	}

	QMultiHash<Geometry::GeometrySet*, int> GeometryParaExtrusion::getShapeHash()
	{
		return _shapeHash;
	}

	void GeometryParaExtrusion::setDistance(double d1)
	{
		_d = d1;
	}

	double GeometryParaExtrusion::getDistance()
	{
		return _d;
	}

	void GeometryParaExtrusion::setDirection(double* dir)
	{
		for (int i = 0; i < 3; ++i)
		{
			_dir[i] = dir[i];
		}
	}

	void GeometryParaExtrusion::getDirection(double* dir)
	{
		for (int i = 0; i < 3; ++i)
		{
			dir[i] = _dir[i];
		}
	}

	void GeometryParaExtrusion::setReverse(bool s)
	{
		_reverse = s;

	}

	bool GeometryParaExtrusion::getReverse()
	{
		return _reverse;
	}

	void GeometryParaExtrusion::setSolid(bool s)
	{
		_issolid = s;

	}

	bool GeometryParaExtrusion::getSloid()
	{
		return _issolid;
	}

	QDomElement& GeometryParaExtrusion::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{
		QDomElement element = doc->createElement("Parameter");  //创建子节点
		QDomAttr typeattr = doc->createAttribute("Type");
		typeattr.setValue(this->typeToString());
		element.setAttributeNode(typeattr);

		QDomAttr rad1attr = doc->createAttribute("Distance");
		rad1attr.setValue(QString::number(_d));
		element.setAttributeNode(rad1attr);

		if (_shapeHash.size() == 0) return element;
		QList<Geometry::GeometrySet*> setList = _shapeHash.keys();
		QList<int> setidList;
		QString setidStr{};
		for (int i = 0; i < setList.size(); ++i)
		{
			setidStr.append(QString::number((setList[i]->getID())));
			if (i != (setList.size() - 1)) setidStr.append(",");

		}
		QDomElement startpointEle = doc->createElement("EdgeSetIDList");
		QDomText startpointText = doc->createTextNode(setidStr);
		startpointEle.appendChild(startpointText);
		element.appendChild(startpointEle);

		QList<int> indexList = _shapeHash.values();
		QString indexListStr{};
		for (int i = 0; i < indexList.size(); ++i)
		{
			indexListStr.append(QString::number(indexList[i]));
			if (i != (indexList.size() - 1)) indexListStr.append(",");

		}
		QDomElement indexlistEle = doc->createElement("IndexList");
		QDomText indexText = doc->createTextNode(indexListStr);
		indexlistEle.appendChild(indexText);
		element.appendChild(indexlistEle);

		QDomElement dirEle = doc->createElement("Direction");
		QString dirstr = QString("%1,%2,%3").arg(_dir[0]).arg(_dir[1]).arg(_dir[2]);
		QDomText dirText = doc->createTextNode(dirstr);
		dirEle.appendChild(dirText);
		element.appendChild(dirEle);

		QDomElement reverse1Ele = doc->createElement("Reverse");
		QString reverse1Str = QString("%1").arg(_reverse);
		QDomText reverse1Text = doc->createTextNode(reverse1Str);
		reverse1Ele.appendChild(reverse1Text);
		element.appendChild(reverse1Ele);

		QDomElement solidEle = doc->createElement("Solid");
		QString solidStr = QString("%1").arg(_issolid);
		QDomText solidText = doc->createTextNode(solidStr);
		solidEle.appendChild(solidText);
		element.appendChild(solidEle);

		parent->appendChild(element);
		return element;
	}

	void GeometryParaExtrusion::readDataFromProjectFile(QDomElement* e)
	{
 		_name = e->attribute("Name");
 		_d = e->attribute("Distance").toDouble();

		QDomNodeList setIdList = e->elementsByTagName("EdgeSetIDList");
		if (setIdList.size() != 1) return;
		QDomElement coorele = setIdList.at(0).toElement();
		QString coorstr = coorele.text();
		QStringList coorsl = coorstr.split(",");
		if (coorsl.size() < 1) return;
		QList<int> setidList;
		for (int i = 0; i < coorsl.size(); ++i)
		{
			setidList.append(coorsl.at(i).toInt());
		}

		QDomNodeList startList = e->elementsByTagName("IndexList");
		if (startList.size() != 1) return;
		QDomElement startele = startList.at(0).toElement();
		QString startstr = startele.text();
		QStringList startsl = startstr.split(",");
		if (startsl.size() < 1) return;
		QList<int> indexList;
		for (int i = 0; i < startsl.size(); ++i)
		{
			indexList.append(startsl.at(i).toInt());
		}

		Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
		for (int i = 0; i < setidList.size(); ++i)
		{
			Geometry::GeometrySet*set = data->getGeometrySetByID(setidList[i]);
			_shapeHash.insert(set, indexList[i]);
		}

		QDomNodeList paraList = e->elementsByTagName("Direction");
		if (paraList.size() != 1) return;
		QDomElement paraele = paraList.at(0).toElement();
		QString parastr = paraele.text();
		QStringList parasl = parastr.split(",");
		if (parasl.size() != 3) return;
		for (int i = 0; i < 3; ++i)
		{
			_dir[i] = parasl.at(i).toDouble();
		}

		QDomNodeList r1List = e->elementsByTagName("Reverse");
		QString r1 = r1List.at(0).toElement().text();
		if (r1 == "1") _reverse = true;
		else if (r1 == "0") _reverse = false;

		QDomNodeList solidList = e->elementsByTagName("Solid");
		QString solidsr = solidList.at(0).toElement().text();
		if (solidsr == "1") _issolid = true;
		else if (solidsr == "0") _issolid = false;

	}

	

}