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

		if (_oriset != nullptr)
		{
			QDomElement orisetEle = doc->createElement("Oriset");
			QDomText orisetText = doc->createTextNode(QString::number(_oriset->getID()));
			orisetEle.appendChild(orisetText);
			element.appendChild(orisetEle);
		}

		parent->appendChild(element);
		return element;
	}

	void GeometryParaGeoSplitter::readDataFromProjectFile(QDomElement* e)
	{
		_name = e->attribute("Name");

		Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
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

	}

}