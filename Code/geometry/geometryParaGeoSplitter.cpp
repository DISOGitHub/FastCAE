#include "geometryParaGeoSplitter.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDomAttr>
#include "geometrySet.h"
#include "geometryData.h"
#include <QDebug>
namespace Geometry
{
	GeometryParaGeoSplitter::GeometryParaGeoSplitter()
	{
		_type = GeometryParaSplitter;
	}

	void GeometryParaGeoSplitter::setName(QString name)
	{
		_name = name;
	}

	QString GeometryParaGeoSplitter::getName()
	{
		return _name;
	}

	void GeometryParaGeoSplitter::setCurrentIndex(int index)
	{
		_typeindex = index;
	}

	int GeometryParaGeoSplitter::getCurrentIndex()
	{
		return _typeindex;
	}

	void GeometryParaGeoSplitter::setPlaneIndex(int i)
	{
		_palneindex = i;
	}

	int GeometryParaGeoSplitter::getPlaneIndex()
	{
		return _palneindex;
	}

	void GeometryParaGeoSplitter::setDirection(double* dir)
	{

		for (int i = 0; i < 3; ++i)
		{
			_randomdir[i] = dir[i];
		}
	}

	void GeometryParaGeoSplitter::getDirection(double* dir)
	{
		for (int i = 0; i < 3; ++i)
		{
			dir[i] = _randomdir[i];
		}

	}

	void GeometryParaGeoSplitter::setbasepoint(double* basept)
	{
		for (int i = 0; i < 3; ++i)
		{
			_basepoint[i] = basept[i];
		}
	}

	void GeometryParaGeoSplitter::getbasepoint(double* basept)
	{
		for (int i = 0; i < 3; ++i)
		{
			basept[i] = _basepoint[i];
		}
	}

	void GeometryParaGeoSplitter::appendBody(Geometry::GeometrySet* set, int bodyindex)
	{
		_solidHash.insert(set, bodyindex);
	}

	QMultiHash<Geometry::GeometrySet*, int> GeometryParaGeoSplitter::getBodys()
	{
		return _solidHash;
	}
	void GeometryParaGeoSplitter::setFaceIndex(int i)
	{
		_faceindex = i;
	}

	int GeometryParaGeoSplitter::getFaceIndex()
	{
		return _faceindex;
	}

	void GeometryParaGeoSplitter::setFaceBody(Geometry::GeometrySet* facebody)
	{
		_facebody = facebody;
	}

	Geometry::GeometrySet* GeometryParaGeoSplitter::getFaceBody()
	{
		return _facebody;
	}

	void GeometryParaGeoSplitter::setOriSet(Geometry::GeometrySet* s)
	{
		_oriset = s;
	}

	Geometry::GeometrySet* GeometryParaGeoSplitter::getOriSet()
	{
		return _oriset;
	}

	QDomElement& GeometryParaGeoSplitter::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{
		QDomElement element = doc->createElement("Parameter");  //创建子节点
		QDomAttr typeattr = doc->createAttribute("Type");
		typeattr.setValue(this->typeToString());
		element.setAttributeNode(typeattr);

		QDomAttr nameattr = doc->createAttribute("Name");
		nameattr.setValue(_name);
		element.setAttributeNode(nameattr);

		QDomElement indexEle = doc->createElement("TypeIndex");
		QString indexStr = QString("%1").arg(_typeindex);
		QDomText indexText = doc->createTextNode(indexStr);
		indexEle.appendChild(indexText);
		element.appendChild(indexEle);

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

		QDomElement faceindexEle = doc->createElement("FaceIndex");
		QString faceindexStr = QString("%1").arg(_faceindex);
		QDomText faceindexText = doc->createTextNode(faceindexStr);
		faceindexEle.appendChild(faceindexText);
		element.appendChild(faceindexEle);

		if (_facebody != nullptr)
		{
			QDomElement facebodyEle = doc->createElement("FaceBody");
			QDomText facebodyText = doc->createTextNode(QString::number(_facebody->getID()));
			facebodyEle.appendChild(facebodyText);
			element.appendChild(facebodyEle);
		}

		QDomElement planeindexEle = doc->createElement("PlaneIndex");
		QString planeindexStr = QString("%1").arg(_palneindex);
		QDomText planeindexText = doc->createTextNode(planeindexStr);
		planeindexEle.appendChild(planeindexText);
		element.appendChild(planeindexEle);

		if (_oriset != nullptr)
		{
			QDomElement orisetEle = doc->createElement("Oriset");
			QDomText orisetText = doc->createTextNode(QString::number(_oriset->getID()));
			orisetEle.appendChild(orisetText);
			element.appendChild(orisetEle);
		}
		QDomElement coorEle = doc->createElement("BasePoint");
		QString coorstr = QString("%1,%2,%3").arg(_basepoint[0]).arg(_basepoint[1]).arg(_basepoint[2]);
		QDomText coorText = doc->createTextNode(coorstr);
		coorEle.appendChild(coorText);
		element.appendChild(coorEle);

		QDomElement randomdirEle = doc->createElement("RandorPoint");
		QString randomdirstr = QString("%1,%2,%3").arg(_randomdir[0]).arg(_randomdir[1]).arg(_randomdir[2]);
		QDomText randomdirText = doc->createTextNode(randomdirstr);
		randomdirEle.appendChild(randomdirText);
		element.appendChild(randomdirEle);

		parent->appendChild(element);
		return element;
	}

	void GeometryParaGeoSplitter::readDataFromProjectFile(QDomElement* e)
	{
		_name = e->attribute("Name");
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
		QDomNodeList typeindexList = e->elementsByTagName("TypeIndex");
		_typeindex = typeindexList.at(0).toElement().text().toInt();
		QDomNodeList planeindexList = e->elementsByTagName("PlaneIndex");
		_palneindex = planeindexList.at(0).toElement().text().toInt();

		QDomNodeList faceindexList = e->elementsByTagName("FaceIndex");
		_faceindex = faceindexList.at(0).toElement().text().toInt();
		QDomNodeList nodeidList = e->elementsByTagName("Oriset");
		if (nodeidList.size() < 1) return;
		QDomElement idele = nodeidList.at(0).toElement();
		QString sid = idele.text();
		int orisetid = sid.toInt();
		_oriset = _geoData->getGeometrySetByID(orisetid);
		
		QDomNodeList facebodyidList = e->elementsByTagName("FaceBody");
		if (facebodyidList.size() < 1) return;
		QDomElement facebodyidele = facebodyidList.at(0).toElement();
		QString facebodyid = facebodyidele.text();
		int faceid = facebodyid.toInt();
		_facebody = _geoData->getGeometrySetByID(faceid);

		QDomNodeList cList = e->elementsByTagName("BasePoint");
		if (cList.size() != 1) return;
		QDomElement cele = cList.at(0).toElement();
		QString cstr = cele.text();
		QStringList csl = cstr.split(",");
		if (csl.size() != 3) return;
		for (int i = 0; i < 3; ++i)
		{
			_basepoint[i] = csl.at(i).toDouble();
		}

		QDomNodeList pList = e->elementsByTagName("RandorPoint");
		if (pList.size() != 1) return;
		QDomElement pele = pList.at(0).toElement();
		QString pstr = pele.text();
		QStringList psl = pstr.split(",");
		if (psl.size() != 3) return;
		for (int i = 0; i < 3; ++i)
		{
			_randomdir[i] = psl.at(i).toDouble();
		}
	}

}