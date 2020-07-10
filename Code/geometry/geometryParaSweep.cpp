#include "geometryParaSweep.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDomAttr>
#include <QDebug>
#include "geometrySet.h"
#include "geometryData.h"

namespace Geometry
{
	GeometryParaSweep::GeometryParaSweep()
	{
		_type = GeometryParaMakeSweep;
	}

	void GeometryParaSweep::setShapeHash(QMultiHash<Geometry::GeometrySet*, int> shapehash)
	{
		_shapeHash = shapehash;
	}

	QMultiHash<Geometry::GeometrySet*, int> GeometryParaSweep::getShapeHash()
	{
		return _shapeHash;
	}

	void GeometryParaSweep::setPath(QPair<Geometry::GeometrySet*, int> path)
	{
		_path = path;
	}

	QPair<Geometry::GeometrySet*, int> GeometryParaSweep::getPath()
	{
		return _path;
	}

	void GeometryParaSweep::setSolid(bool s)
	{
		_issolid = s;

	}

	bool GeometryParaSweep::getSloid()
	{
		return _issolid;
	}

	QDomElement& GeometryParaSweep::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{
		QDomElement element = doc->createElement("Parameter");  //创建子节点
		QDomAttr typeattr = doc->createAttribute("Type");
		typeattr.setValue(this->typeToString());
		element.setAttributeNode(typeattr);

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

		QDomElement edgeEle = doc->createElement("Path");
		if (_path.first!=nullptr)
		{
			QString edgeStr = QString("%1,%2").arg(_path.first->getID()).arg(_path.second);
			QDomText edgeText = doc->createTextNode(edgeStr);
			edgeEle.appendChild(edgeText);
			element.appendChild(edgeEle);
		}

		parent->appendChild(element);
		return element;
	}

	void GeometryParaSweep::readDataFromProjectFile(QDomElement* e)
	{
 		_name = e->attribute("Name");

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

		QDomNodeList edgeList = e->elementsByTagName("Path");
		if (edgeList.size() != 1) return;
		QDomElement edgeele = edgeList.at(0).toElement();
		QString edgestr = edgeele.text();
		QStringList edgesl = edgestr.split(",");
		_path.first = data->getGeometrySetByID((edgesl[0]).toInt());
		_path.second = edgesl[1].toInt();
 		

	}

	

}