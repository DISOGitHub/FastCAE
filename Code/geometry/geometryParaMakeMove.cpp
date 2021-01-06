#include "geometryParaMakeMove.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDomAttr>
#include "geometrySet.h"
#include "geometryData.h"
#include <QDebug>
namespace Geometry
{
	GeometryParaMakeMove::GeometryParaMakeMove()
	{
		_type = GeometryParaMakeMoveFeature;
	}

	void GeometryParaMakeMove::setOptionIndex(int index)
	{
		_optionindex = index;
	}


	int GeometryParaMakeMove::getOptionIndex()
	{
		return _optionindex;
	}

	void GeometryParaMakeMove::appendBody(Geometry::GeometrySet* set, int bodyindex)
	{
		_solidHash.insert(set, bodyindex);
	}

	QMultiHash<Geometry::GeometrySet*, int> GeometryParaMakeMove::getBodys()
	{
		return _solidHash;
	}
	void GeometryParaMakeMove::setSaveOrigion(bool s)
	{
		_saveorigion = s;
	}

	bool GeometryParaMakeMove::getSaveOrigion()
	{
		return _saveorigion;
	}

	void GeometryParaMakeMove::setReverse(bool s)
	{
		_reverse = s;
	}

	bool GeometryParaMakeMove::getReverse()
	{
		return _reverse;
	}

	void GeometryParaMakeMove::setLength(double l)
	{
		_length = l;
	}

	double GeometryParaMakeMove::getLength()
	{
		return _length;
	}

	void GeometryParaMakeMove::setDirection(double* dir)
	{
		for (int i = 0; i < 3; ++i)
		{
			_direction[i] = dir[i];
		}
	}

	void GeometryParaMakeMove::getDirection(double* dir)
	{
		for (int i = 0; i < 3; ++i)
		{
			dir[i] = _direction[i];
		}
	}

	void GeometryParaMakeMove::setEndPoint(double* dir)
	{
		for (int i = 0; i < 3; ++i)
		{
			_endpoint[i] = dir[i];
		}
	}

	void GeometryParaMakeMove::getsetEndPoint(double* dir)
	{
		for (int i = 0; i < 3; ++i)
		{
			dir[i] = _endpoint[i];
		}
	}

	void GeometryParaMakeMove::setBasePoint(double* basept)
	{
		for (int i = 0; i < 3; ++i)
		{
			_startpoint[i] = basept[i];
		}
	}

	void GeometryParaMakeMove::getBasePoint(double* basept)
	{
		for (int i = 0; i < 3; ++i)
		{
			basept[i] = _startpoint[i];
		}
	}

	void GeometryParaMakeMove::setOriSet(Geometry::GeometrySet* s)
	{
		_oriset = s;
	}

	Geometry::GeometrySet* GeometryParaMakeMove::getOriSet()
	{
		return _oriset;
	}

	QDomElement& GeometryParaMakeMove::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{
		QDomElement element = doc->createElement("Parameter");  //创建子节点
		QDomAttr typeattr = doc->createAttribute("Type");
		typeattr.setValue(this->typeToString());
		element.setAttributeNode(typeattr);

		QDomElement indexEle = doc->createElement("OptionIndex");
		QString indexStr = QString("%1").arg(_optionindex);
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

		if (_oriset != nullptr)
		{
			QDomElement orisetEle = doc->createElement("Oriset");
			QDomText orisetText = doc->createTextNode(QString::number(_oriset->getID()));
			orisetEle.appendChild(orisetText);
			element.appendChild(orisetEle);
		}
		QDomElement coorEle = doc->createElement("StrartPoint");
		QString coorstr = QString("%1,%2,%3").arg(_startpoint[0]).arg(_startpoint[1]).arg(_startpoint[2]);
		QDomText coorText = doc->createTextNode(coorstr);
		coorEle.appendChild(coorText);
		element.appendChild(coorEle);

		QDomElement randomdirEle = doc->createElement("EndPoint");
		QString randomdirstr = QString("%1,%2,%3").arg(_endpoint[0]).arg(_endpoint[1]).arg(_endpoint[2]);
		QDomText randomdirText = doc->createTextNode(randomdirstr);
		randomdirEle.appendChild(randomdirText);
		element.appendChild(randomdirEle);

		QDomElement faceindexEle = doc->createElement("Length");
		QString lenStr = QString("%1").arg(_length);
		QDomText lenText = doc->createTextNode(lenStr);
		faceindexEle.appendChild(lenText);
		element.appendChild(faceindexEle);

		QDomElement dirEle = doc->createElement("Direction");
		QString dirstr = QString("%1,%2,%3").arg(_direction[0]).arg(_direction[1]).arg(_direction[2]);
		QDomText dirText = doc->createTextNode(dirstr);
		dirEle.appendChild(dirText);
		element.appendChild(dirEle);

		QDomElement reverseEle = doc->createElement("Reverse");
		QString rStr = QString("%1").arg(_reverse);
		QDomText planeindexText = doc->createTextNode(rStr);
		reverseEle.appendChild(planeindexText);
		element.appendChild(reverseEle);

		QDomElement saveoriEle = doc->createElement("SaveOrigion");
		QString saveoriStr = QString("%1").arg(_saveorigion);
		QDomText saveoriText = doc->createTextNode(saveoriStr);
		saveoriEle.appendChild(saveoriText);
		element.appendChild(saveoriEle);

		parent->appendChild(element);
		return element;
	}

	void GeometryParaMakeMove::readDataFromProjectFile(QDomElement* e)
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
		if (r == "1") _saveorigion = true;
		else if (r == "0") _saveorigion = false;

		QDomNodeList reverseList = e->elementsByTagName("Reverse");
		QString sr = reverseList.at(0).toElement().text();
		if (sr == "1") _reverse = true;
		else if (sr == "0") _reverse = false;


		QDomNodeList typeindexList = e->elementsByTagName("OptionIndex");
		_optionindex = typeindexList.at(0).toElement().text().toInt();

		QDomNodeList faceindexList = e->elementsByTagName("Length");
		_length = faceindexList.at(0).toElement().text().toDouble();



		QDomNodeList nodeidList = e->elementsByTagName("Oriset");
		if (nodeidList.size() < 1) return;
		QDomElement idele = nodeidList.at(0).toElement();
		QString sid = idele.text();
		int orisetid = sid.toInt();
		_oriset = _geoData->getGeometrySetByID(orisetid);

	
		QDomNodeList cList = e->elementsByTagName("StrartPoint");
		if (cList.size() != 1) return;
		QDomElement cele = cList.at(0).toElement();
		QString cstr = cele.text();
		QStringList csl = cstr.split(",");
		if (csl.size() != 3) return;
		for (int i = 0; i < 3; ++i)
		{
			_startpoint[i] = csl.at(i).toDouble();
		}
		
		QDomNodeList pList = e->elementsByTagName("EndPoint");
		if (pList.size() != 1) return;
		QDomElement pele = pList.at(0).toElement();
		QString pstr = pele.text();
		QStringList psl = pstr.split(",");
		if (psl.size() != 3) return;
		for (int i = 0; i < 3; ++i)
		{
			_endpoint[i] = psl.at(i).toDouble();
		}

		QDomNodeList dirList = e->elementsByTagName("Direction");
		if (dirList.size() != 1) return;
		QDomElement direle = dirList.at(0).toElement();
		QString dirstr = direle.text();
		QStringList dirsl = dirstr.split(",");
		if (dirsl.size() != 3) return;
		for (int i = 0; i < 3; ++i)
		{
			_direction[i] = dirsl.at(i).toDouble();
		}

	}


}