#include "geometryParaFillet.h"
#include "geometrySet.h"
#include "geometryData.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDomAttr>

namespace Geometry
{
	GeometryParaFillet::GeometryParaFillet()
	{
		_type = GeometryParaCreateFillet;
	}

	void GeometryParaFillet::setGeometrySet(GeometrySet* s)
	{
		_geoSet = s;
	}

	GeometrySet* GeometryParaFillet::getGeometrySet()
	{
		return _geoSet;
	}

	void GeometryParaFillet::setEdgeList(QList<int> edgeList)
	{
		_edgeIndexList = edgeList;
	}

	QList<int> GeometryParaFillet::getEdgeList()
	{
		return _edgeIndexList;
	}

	void GeometryParaFillet::setRadius(double r)
	{
		_radius = r;
	}

	double GeometryParaFillet::getRadius()
	{
		return _radius;
	}

	QDomElement& GeometryParaFillet::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{
		QDomElement element = doc->createElement("Parameter");  //创建子节点
		QDomAttr typeattr = doc->createAttribute("Type");
		typeattr.setValue(this->typeToString());
		element.setAttributeNode(typeattr);

		QDomAttr radattr = doc->createAttribute("Radius");
		radattr.setValue(QString::number(_radius));
		element.setAttributeNode(radattr);

		if (_geoSet != nullptr)
		{
			QDomElement idEle = doc->createElement("SubID");
			QDomText idText = doc->createTextNode(QString::number(_geoSet->getID()));
			idEle.appendChild(idText);
			element.appendChild(idEle);
		}
		
		if (_edgeIndexList.size() == 0) return element;
		QDomElement edgeEle = doc->createElement("Edges");
		QStringList edgeIndexs;
		for (int i = 0; i < _edgeIndexList.size(); ++i)
			edgeIndexs.append(QString::number(_edgeIndexList.at(i)));
		QDomText edgeText = doc->createTextNode(edgeIndexs.join(","));
		edgeEle.appendChild(edgeText);
		element.appendChild(edgeEle);
		
		parent->appendChild(element);
		return element;
	}

	void GeometryParaFillet::readDataFromProjectFile(QDomElement* e)
	{
		QString rs = e->attribute("Radius");
		_radius = rs.toDouble();
		QDomNodeList nodeidList = e->elementsByTagName("SubID");
		if (nodeidList.size() < 1) return;
		QDomElement idele = nodeidList.at(0).toElement();
		QString sid = idele.text();
		int id = sid.toInt();
		_geoSet = _geoData->getGeometrySetByID(id);

		QDomNodeList edgeIndexList = e->elementsByTagName("Edges");
		if (edgeIndexList.size() < 1) return;
		QDomElement edgeele = edgeIndexList.at(0).toElement();
		QString sIndex = edgeele.text();
		QStringList sindexs = sIndex.split(",");
		for (int i = 0; i < sindexs.size(); ++i)
		{
			int inedx = sindexs.at(i).toDouble();
			_edgeIndexList.append(inedx);
		}
	}


}