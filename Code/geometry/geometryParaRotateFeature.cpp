#include "geometryParaRotateFeature.h"
#include <QDomElement>
#include <QDomDocument>
#include "geometrySet.h"
#include "geometryData.h"
namespace Geometry
{
	GeometryParaRotateFeature::GeometryParaRotateFeature()
	{
		_type = GeometryParaMakeRotateFeature;
	}

	QDomElement& GeometryParaRotateFeature::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{
		QDomElement element = doc->createElement("Parameter");  //创建子节点
		QDomAttr typeattr = doc->createAttribute("Type");
		typeattr.setValue(this->typeToString());
		element.setAttributeNode(typeattr);
		
		if (_originSet != nullptr)
		{
			QDomElement orisetEle = doc->createElement("Oriset");
			QDomText orisetText = doc->createTextNode(QString::number(_originSet->getID()));
			orisetEle.appendChild(orisetText);
			element.appendChild(orisetEle);
		}

		QDomElement methodEle = doc->createElement("Method");
		QString methodStr = QString("%1").arg(_method);
		QDomText methodText = doc->createTextNode(methodStr);
		methodEle.appendChild(methodText);
		element.appendChild(methodEle);

		QDomElement angleEle = doc->createElement("Angle");
		QString angleStr = QString("%1").arg(_angle);
		QDomText angleText = doc->createTextNode(angleStr);
		angleEle.appendChild(angleText);
		element.appendChild(angleEle);

		QDomElement saveoriEle = doc->createElement("SaveOrigion");
		QString saveoriStr = QString("%1").arg(_saveOrigin);
		QDomText saveoriText = doc->createTextNode(saveoriStr);
		saveoriEle.appendChild(saveoriText);
		element.appendChild(saveoriEle);

		QDomElement coorEle = doc->createElement("BasePoint");
		QString coorstr = QString("%1,%2,%3").arg(_basicPoint[0]).arg(_basicPoint[1]).arg(_basicPoint[2]);
		QDomText coorText = doc->createTextNode(coorstr);
		coorEle.appendChild(coorText);
		element.appendChild(coorEle);

		QStringList solidStrList{};
		if (_solidHash.size() > 0)
		{
			QMultiHash<Geometry::GeometrySet*, int>::iterator it = _solidHash.begin();
			for (; it != _solidHash.end(); it++)
			{
				solidStrList << QString("%1:%2)").arg(it.key()->getID()).arg(it.value());
			}
			QDomElement startpointEle = doc->createElement("BodyIDList");
			QDomText startpointText = doc->createTextNode(solidStrList.join(","));
			startpointEle.appendChild(startpointText);
			element.appendChild(startpointEle);
		}
		QDomElement edgeEle = doc->createElement("Edge");
		if (_edge.first != nullptr)
		{
			QString edgeStr = QString("%1,%2").arg(_edge.first->getID()).arg(_edge.second);
			QDomText edgeText = doc->createTextNode(edgeStr);
			edgeEle.appendChild(edgeText);
			element.appendChild(edgeEle);
		}
		QDomElement randomdirEle = doc->createElement("Vector");
		QString randomdirstr = QString("%1,%2,%3").arg(_vector[0]).arg(_vector[1]).arg(_vector[2]);
		QDomText randomdirText = doc->createTextNode(randomdirstr);
		randomdirEle.appendChild(randomdirText);
		element.appendChild(randomdirEle);

		QDomElement reverseEle = doc->createElement("Reverse");
		QString rStr = QString("%1").arg(_reverse);
		QDomText planeindexText = doc->createTextNode(rStr);
		reverseEle.appendChild(planeindexText);
		element.appendChild(reverseEle);

		parent->appendChild(element);
		return element;

	}

	void GeometryParaRotateFeature::readDataFromProjectFile(QDomElement* e)
	{

		Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
		QDomNodeList setIdList = e->elementsByTagName("BodyIDList");
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
			_solidHash.insert(set, solidindex);
		}

		QDomNodeList rList = e->elementsByTagName("SaveOrigion");
		QString r = rList.at(0).toElement().text();
		if (r == "1") _saveOrigin = true;
		else if (r == "0") _saveOrigin = false;

		QDomNodeList reverseList = e->elementsByTagName("Reverse");
		QString sr = reverseList.at(0).toElement().text();
		if (sr == "1") _reverse = true;
		else if (sr == "0") _reverse = false;

		QDomNodeList nodeidList = e->elementsByTagName("Oriset");
		if (nodeidList.size() < 1) return;
		QDomElement idele = nodeidList.at(0).toElement();
		QString sid = idele.text();
		int orisetid = sid.toInt();
		_originSet = _geoData->getGeometrySetByID(orisetid);

		QDomNodeList methodList = e->elementsByTagName("Method");
		_method = methodList.at(0).toElement().text().toDouble();

		QDomNodeList angleList = e->elementsByTagName("Angle");
		_angle = angleList.at(0).toElement().text().toDouble();

		QDomNodeList pList = e->elementsByTagName("BasePoint");
		if (pList.size() != 1) return;
		QDomElement pele = pList.at(0).toElement();
		QString pstr = pele.text();
		QStringList psl = pstr.split(",");
		if (psl.size() != 3) return;
		for (int i = 0; i < 3; ++i)
		{
			_basicPoint[i] = psl.at(i).toDouble();
		}

		QDomNodeList dirList = e->elementsByTagName("Vector");
		if (dirList.size() != 1) return;
		QDomElement direle = dirList.at(0).toElement();
		QString dirstr = direle.text();
		QStringList dirsl = dirstr.split(",");
		if (dirsl.size() != 3) return;
		for (int i = 0; i < 3; ++i)
		{
			_vector[i] = dirsl.at(i).toDouble();
		}

		QDomNodeList edgeList = e->elementsByTagName("Edge");
		if (edgeList.size() != 1) return;
		QDomElement edgeele = edgeList.at(0).toElement();
		QString edgestr = edgeele.text();
		QStringList edgesl = edgestr.split(",");
		_edge.first = data->getGeometrySetByID((edgesl[0]).toInt());
		_edge.second = edgesl[1].toInt();

	}

	void GeometryParaRotateFeature::setOriginObject(GeometrySet* set)
	{
		_originSet = set;
	}

	GeometrySet* GeometryParaRotateFeature::getOriginObject()
	{
		return _originSet;
	}

	void GeometryParaRotateFeature::appendBody(Geometry::GeometrySet* set, int bodyindex)
	{
		_solidHash.insert(set, bodyindex);
	}

	QMultiHash<Geometry::GeometrySet*, int> GeometryParaRotateFeature::getBodys()
	{
		return _solidHash;
	}

	void GeometryParaRotateFeature::setBasicPoint(double* p)
	{
		for (int i = 0; i < 3; ++i)
			_basicPoint[i] = p[i];
	}

	void GeometryParaRotateFeature::getBasicPoint(double* p)
	{
		for (int i = 0; i < 3; ++i)
			p[i] = _basicPoint[i];
	}

	void GeometryParaRotateFeature::setAngle(double a)
	{
		_angle = a;
	}

	double GeometryParaRotateFeature::getAngle()
	{
		return _angle;
	}

	void GeometryParaRotateFeature::isSaveOrigin(bool s)
	{
		_saveOrigin = s;
	}

	bool GeometryParaRotateFeature::isSaveOrigin()
	{
		return _saveOrigin;
	}

	void GeometryParaRotateFeature::setMethod(int m)
	{
		_method = m;
	}

	int GeometryParaRotateFeature::getMethod()
	{
		return _method;
	}

	void GeometryParaRotateFeature::setEdge(GeometrySet* body, int index)
	{
		_edge.first = body;
		_edge.second = index;
	}

	void GeometryParaRotateFeature::setVector(double* vec)
	{
		for (int i = 0; i < 3; ++i)
			_vector[i] = vec[i];
	}

	void GeometryParaRotateFeature::getVector(double* v)
	{
		for (int i = 0; i < 3; ++i)
			v[i] = _vector[i];
	}

	void GeometryParaRotateFeature::isReverse(bool r)
	{
		_reverse = r;
	}

	bool GeometryParaRotateFeature::isReverse()
	{
		return _reverse;
	}

	QPair<GeometrySet*, int> GeometryParaRotateFeature::getEdge()
	{
		return _edge;
	}



}