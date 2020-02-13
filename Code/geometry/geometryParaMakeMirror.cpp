#include "geometryParaMakeMirror.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDomAttr>
#include "geometrySet.h"
#include "geometryData.h"
#include <QDebug>
namespace Geometry
{
	GeometryParaMirrorFeature::GeometryParaMirrorFeature()
	{
		_type = GeometryParaMakeMirrorFeature;
	}



	void GeometryParaMirrorFeature::setCurrentIndex(int index)
	{
		_typeindex = index;
	}
	
	int GeometryParaMirrorFeature::getCurrentIndex()
	{
		return _typeindex;
	}

	void GeometryParaMirrorFeature::setFaceIndex(int i)
	{
		_faceindex = i;
	}

	int GeometryParaMirrorFeature::getFaceIndex()
	{
		return _faceindex;
	}

	void GeometryParaMirrorFeature::setFaceBody(Geometry::GeometrySet* facebody)
	{
		_facebody = facebody;
	}

	Geometry::GeometrySet* GeometryParaMirrorFeature::getFaceBody()
	{
		return _facebody;
	}

	void GeometryParaMirrorFeature::setBodyList(QList<Geometry::GeometrySet*> bodylist)
	{
		_bodylist = bodylist;
	}
			
	QList<Geometry::GeometrySet*> GeometryParaMirrorFeature::getBodyList()
	{
		return _bodylist;
	}
     
	void GeometryParaMirrorFeature::setSaveOrigion(bool s)
	{
		_saveorigion = s;
	}

	bool GeometryParaMirrorFeature::getSaveOrigion()
	{
		return _saveorigion;
	}

	void GeometryParaMirrorFeature::setPlaneIndex(int i)
	{
		_palneindex = i;
	}

	int GeometryParaMirrorFeature::getPlaneIndex()
	{
		return _palneindex;
	}

	

	void GeometryParaMirrorFeature::setDirection(double* dir)
	{

		for (int i = 0; i < 3; ++i)
		{
			_randomdir[i] = dir[i];
		}
	}

	void GeometryParaMirrorFeature::getDirection(double* dir)
	{
		for (int i = 0; i < 3; ++i)
		{
			dir[i] = _randomdir[i];
		}

	}

	void GeometryParaMirrorFeature::setbasepoint(double* basept)
	{
		for (int i = 0; i < 3; ++i)
		{
			_basepoint[i] = basept[i];
		}
	}

	void GeometryParaMirrorFeature::getbasepoint(double* basept)
	{
		for (int i = 0; i < 3; ++i)
		{
			basept[i] = _basepoint[i];
		}
	}

	void GeometryParaMirrorFeature::setOriSet(Geometry::GeometrySet* s)
	{
		_oriset = s;
	}

	Geometry::GeometrySet* GeometryParaMirrorFeature::getOriSet()
	{
		return _oriset;
	}

	QDomElement& GeometryParaMirrorFeature::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{
		QDomElement element = doc->createElement("Parameter");  //创建子节点
		QDomAttr typeattr = doc->createAttribute("Type");
		typeattr.setValue(this->typeToString());
		element.setAttributeNode(typeattr);

		QDomElement indexEle = doc->createElement("TypeIndex");
		QString indexStr = QString("%1").arg(_typeindex);
		QDomText indexText = doc->createTextNode(indexStr);
		indexEle.appendChild(indexText);
		element.appendChild(indexEle);

		if (_bodylist.size() > 0)
		{
			QString setidStr{};
			for (int i = 0; i < _bodylist.size(); ++i)
			{
				setidStr.append(QString::number((_bodylist[i]->getID())));
				if (i != (_bodylist.size() - 1)) setidStr.append(",");

			}
			QDomElement startpointEle = doc->createElement("BodyIDList");
			QDomText startpointText = doc->createTextNode(setidStr);
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

		QDomElement saveoriEle = doc->createElement("SaveOrigion");
		QString saveoriStr = QString("%1").arg(_saveorigion);
		QDomText saveoriText = doc->createTextNode(saveoriStr);
		saveoriEle.appendChild(saveoriText);
		element.appendChild(saveoriEle);
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

	void GeometryParaMirrorFeature::readDataFromProjectFile(QDomElement* e)
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
			Geometry::GeometrySet*set = data->getGeometrySetByID((coorsl[i]).toInt());
			_bodylist.append(set);
		}
		QDomNodeList rList = e->elementsByTagName("SaveOrigion");
		QString r = rList.at(0).toElement().text();
		if (r == "1") _saveorigion = true;
		else if (r == "0") _saveorigion = false;


		QDomNodeList typeindexList = e->elementsByTagName("TypeIndex");
		_typeindex = typeindexList.at(0).toElement().text().toInt();

		QDomNodeList faceindexList = e->elementsByTagName("FaceIndex");
		_faceindex = faceindexList.at(0).toElement().text().toInt();

		QDomNodeList planeindexList = e->elementsByTagName("PlaneIndex");
		_palneindex = planeindexList.at(0).toElement().text().toInt();


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