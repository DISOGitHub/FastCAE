
#include "geometryParaMakeFillHole.h"
#include "geometrySet.h"
#include "geometryData.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDomAttr>

namespace Geometry
{
	GeometryParaMakeFillHole::GeometryParaMakeFillHole()
	{
		_type = GeometryParaFillHole;
	}

	void GeometryParaMakeFillHole::setName(QString name)
	{
		_name = name;
	}

	QString GeometryParaMakeFillHole::getName()
	{
		return _name;
	}

	void GeometryParaMakeFillHole::setInputSet(GeometrySet* s)
	{
		_geoSet = s;
	}

	GeometrySet* GeometryParaMakeFillHole::getGeometrySet()
	{
		return _geoSet;
	}
	
	void GeometryParaMakeFillHole::setFaceList(QList<int> edgeList)
	{
		_faceIndexList = edgeList;
	}
	
	QList<int> GeometryParaMakeFillHole::getFaceList()
	{
		return _faceIndexList;
	}


	QDomElement& GeometryParaMakeFillHole::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{
		QDomElement element = doc->createElement("Parameter");  //创建子节点
		QDomAttr typeattr = doc->createAttribute("Type");
		typeattr.setValue(this->typeToString());
		element.setAttributeNode(typeattr);

		if (_geoSet != nullptr)
		{
			QDomElement idEle = doc->createElement("SubID");
			QDomText idText = doc->createTextNode(QString::number(_geoSet->getID()));
			idEle.appendChild(idText);
			element.appendChild(idEle);
		}
		if (_faceIndexList.size() == 0) return element;
		QDomElement edgeEle = doc->createElement("Faces");
		QStringList edgeIndexs;
		for (int i = 0; i < _faceIndexList.size(); ++i)
			edgeIndexs.append(QString::number(_faceIndexList.at(i)));
		QDomText edgeText = doc->createTextNode(edgeIndexs.join(","));
		edgeEle.appendChild(edgeText);
		element.appendChild(edgeEle);
		
		parent->appendChild(element);
		return element;
	}

	void GeometryParaMakeFillHole::readDataFromProjectFile(QDomElement* e)
	{

		QDomNodeList nodeidList = e->elementsByTagName("SubID");
		if (nodeidList.size() < 1) return;
		QDomElement idele = nodeidList.at(0).toElement();
		QString sid = idele.text();
		int id = sid.toInt();
		_geoSet = _geoData->getGeometrySetByID(id);
		
		QDomNodeList edgeIndexList = e->elementsByTagName("Faces");
		if (edgeIndexList.size() < 1) return;
		QDomElement edgeele = edgeIndexList.at(0).toElement();
		QString sIndex = edgeele.text();
		QStringList sindexs = sIndex.split(",");
		for (int i = 0; i < sindexs.size(); ++i)
		{
			int inedx = sindexs.at(i).toDouble();
			_faceIndexList.append(inedx);
		}
	
	}


}
