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
/*

		QDomAttr rad1attr = doc->createAttribute("Distance1");
		rad1attr.setValue(QString::number(_d1));
		element.setAttributeNode(rad1attr);
		
		QDomAttr rad2attr = doc->createAttribute("Distance2");
		rad2attr.setValue(QString::number(_d2));
		element.setAttributeNode(rad2attr);

		
		QDomElement idEle = doc->createElement("SubID");
		QDomText idText = doc->createTextNode(QString::number(_inputSet->getID()));
		idEle.appendChild(idText);
		element.appendChild(idEle);

		QDomElement _combindex0Ele = doc->createElement("Combindex0");
		QDomText _combindex0Text = doc->createTextNode(QString::number(_combindex0));
		_combindex0Ele.appendChild(idText);
		element.appendChild(_combindex0Ele);


		QDomElement edgeEle = doc->createElement("Edges");
		QStringList edgeIndexs;
		for (int i = 0; i < _edgeindexlist.size(); ++i)
			edgeIndexs.append(QString::number(_edgeindexlist.at(i)));
		QDomText edgeText = doc->createTextNode(edgeIndexs.join(","));
		edgeEle.appendChild(edgeText);
		element.appendChild(edgeEle);*/

		parent->appendChild(element);
		return element;
	}

	void GeometryParaSweep::readDataFromProjectFile(QDomElement* e)
	{
// 		_name = e->attribute("Name");
 		/*_d1 = e->attribute("Distance1").toDouble();
		_d2 = e->attribute("Distance2").toDouble();
		int temp = e->attribute("Combindex0").toInt();
		if (temp == 0) _combindex0 = true;
		else _combindex0 = false;
// 		
		QDomNodeList nodeidList = e->elementsByTagName("SubID");
		if (nodeidList.size() < 1) return;
		QDomElement idele = nodeidList.at(0).toElement();
		QString sid = idele.text();
		int id = sid.toInt();
		_inputSet = _geoData->getGeometrySetByID(id);

		QDomNodeList edgeIndexList = e->elementsByTagName("Edges");
		if (edgeIndexList.size() < 1) return;
		QDomElement edgeele = edgeIndexList.at(0).toElement();
		QString sIndex = edgeele.text();
		QStringList sindexs = sIndex.split(",");
		for (int i = 0; i < sindexs.size(); ++i)
		{
			int inedx = sindexs.at(i).toDouble();
			_edgeindexlist.append(inedx);
		}*/



	}

	

}