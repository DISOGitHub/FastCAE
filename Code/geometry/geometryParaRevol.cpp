#include "geometryParaRevol.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDomAttr>
#include <QDebug>
#include "geometrySet.h"
#include "geometryData.h"

namespace Geometry
{
	GeometryParaRevol::GeometryParaRevol()
	{
		_type = GeometryParaMakeRevol;
	}

	void GeometryParaRevol::setShapeHash(QMultiHash<Geometry::GeometrySet*, int> shapehash)
	{
		_shapeHash = shapehash;
	}

	QMultiHash<Geometry::GeometrySet*, int> GeometryParaRevol::getShapeHash()
	{
		return _shapeHash;
	}

	void GeometryParaRevol::setAxisEdge(QPair<Geometry::GeometrySet*, int> axisedge)
	{
		_aixsetedge = axisedge;
	}

	QPair<Geometry::GeometrySet*, int> GeometryParaRevol::getAxisEdge()
	{
		return _aixsetedge;
	}

	void GeometryParaRevol::setDistance(double d1)
	{
		_d = d1;
	}

	double GeometryParaRevol::getDistance()
	{
		return _d;
	}

	void GeometryParaRevol::setBasciPoint(double* dir)
	{
		for (int i = 0; i < 3; ++i)
		{
			_basept[i] = dir[i];
		}
	}

	void GeometryParaRevol::getBasicPoint(double* dir)
	{
		for (int i = 0; i < 3; ++i)
		{
			dir[i] = _basept[i];
		}
	}

	void GeometryParaRevol::setDegree(double d)
	{
		_degree = d;
	}

	double GeometryParaRevol::getDegree()
	{
		return _degree;
	}

	void GeometryParaRevol::setReverse(bool s)
	{
		_reverse = s;

	}

	bool GeometryParaRevol::getReverse()
	{
		return _reverse;
	}
	
	void GeometryParaRevol::setMethod(int s)
	{
		_method = s;
	}

	int GeometryParaRevol::getMethod()
	{
		return _method;
	}

	void GeometryParaRevol::setCoor(double* coor)
	{
		for (int i = 0; i < 3; ++i)
		{
			_coor[i] = coor[i];
		}
	}

	void GeometryParaRevol::getCoor(double* coor)
	{
		for (int i = 0; i < 3; ++i)
		{
			coor[i] = _coor[i];
		}
	}

	void GeometryParaRevol::setSolid(bool s)
	{
		_solid = s;

	}

	bool GeometryParaRevol::getSolid()
	{
		return _solid;
	}

	QDomElement& GeometryParaRevol::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{
		QDomElement element = doc->createElement("Parameter");  //创建子节点
		QDomAttr typeattr = doc->createAttribute("Type");
		typeattr.setValue(this->typeToString());
		element.setAttributeNode(typeattr);

		QDomAttr rad1attr = doc->createAttribute("Distance");
		rad1attr.setValue(QString::number(_d));
		element.setAttributeNode(rad1attr);


		QDomAttr degereeattr = doc->createAttribute("Degree");
		degereeattr.setValue(QString::number(_degree));
		element.setAttributeNode(degereeattr);



		if (_shapeHash.size() > 0)
		{
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
		}

		QDomElement baseptEle = doc->createElement("BasicPoint");
		QString baseptstr = QString("%1,%2,%3").arg(_basept[0]).arg(_basept[1]).arg(_basept[2]);
		QDomText baseptText = doc->createTextNode(baseptstr);
		baseptEle.appendChild(baseptText);
		element.appendChild(baseptEle);

		QDomElement methodEle = doc->createElement("Method");
		QString methodStr = QString("%1").arg(_method);
		QDomText methodText = doc->createTextNode(methodStr);
		methodEle.appendChild(methodText);
		element.appendChild(methodEle);

		if (_method == 1)
		{
			QDomElement coorEle = doc->createElement("Coordinate");
			QString coorstr = QString("%1,%2,%3").arg(_coor[0]).arg(_coor[1]).arg(_coor[2]);
			QDomText coorText = doc->createTextNode(coorstr);
			coorEle.appendChild(coorText);
			element.appendChild(coorEle);
		}
		else
		{

			QDomElement orisetEle = doc->createElement("AxisSet");
			QDomText orisetText = doc->createTextNode(QString::number(_aixsetedge.first->getID()));
			orisetEle.appendChild(orisetText);
			element.appendChild(orisetEle);

			QDomElement axisedgeEle = doc->createElement("AxisEdgeIndex");
			QDomText axisedgeText = doc->createTextNode(QString::number(_aixsetedge.second));
			axisedgeEle.appendChild(axisedgeText);
			element.appendChild(axisedgeEle);
		}
		QDomElement reverse1Ele = doc->createElement("Reverse");
		QString reverse1Str = QString("%1").arg(_reverse);
		QDomText reverse1Text = doc->createTextNode(reverse1Str);
		reverse1Ele.appendChild(reverse1Text);
		element.appendChild(reverse1Ele);

		QDomElement solidEle = doc->createElement("Solid");
		QString solidStr = QString("%1").arg(_solid);
		QDomText solidText = doc->createTextNode(solidStr);
		solidEle.appendChild(solidText);
		element.appendChild(solidEle);

		parent->appendChild(element);
		return element;
	}

	void GeometryParaRevol::readDataFromProjectFile(QDomElement* e)
	{
		_name = e->attribute("Name");
 		_d = e->attribute("Distance").toDouble();
		_degree = e->attribute("Degree").toDouble();
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
		QDomNodeList typeindexList = e->elementsByTagName("Method");
		_method = typeindexList.at(0).toElement().text().toInt();

		QDomNodeList dir1List = e->elementsByTagName("BasicPoint");
		if (dir1List.size() != 1) return;
		QDomElement dir1ele = dir1List.at(0).toElement();
		QString dir1str = dir1ele.text();
		QStringList dir1sl = dir1str.split(",");
		if (dir1sl.size() != 3) return;
		for (int i = 0; i < 3; ++i)
		{
			_basept[i] = dir1sl.at(i).toDouble();
		}
		if (_method==0)
		{
			QDomNodeList nodeidList = e->elementsByTagName("AxisSet");
			if (nodeidList.size() < 1) return;
			QDomElement idele = nodeidList.at(0).toElement();
			QString sid = idele.text();
			int orisetid = sid.toInt();
			_aixsetedge.first = _geoData->getGeometrySetByID(orisetid);
			_aixsetedge.second = e->attribute("AxisEdgeIndex").toInt();

		}
		else
		{
			QDomNodeList cList = e->elementsByTagName("Coordinate");
			if (cList.size() != 1) return;
			QDomElement cele = cList.at(0).toElement();
			QString cstr = cele.text();
			QStringList csl = cstr.split(",");
			if (csl.size() != 3) return;
			for (int i = 0; i < 3; ++i)
			{
				_coor[i] = csl.at(i).toDouble();
			}
		}
		QDomNodeList axisrList = e->elementsByTagName("Reverse");
		QString axisr = axisrList.at(0).toElement().text();
		if (axisr == "1") _reverse = true;
		else if (axisr == "0") _reverse = false;

		QDomNodeList solidList = e->elementsByTagName("Solid");
		QString solidsr = solidList.at(0).toElement().text();
		if (solidsr == "1") _solid = true;
		else if (solidsr == "0") _solid = false;

	}

}