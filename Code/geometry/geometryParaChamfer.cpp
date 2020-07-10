#include "geometryParaChamfer.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDomAttr>
#include <QDebug>
#include "geometrySet.h"
#include "geometryData.h"

namespace Geometry
{
	GeometryParaChamfer::GeometryParaChamfer()
	{
		_type = GeometryParaMakeChamfer;
	}

	void GeometryParaChamfer::setInputSet(GeometrySet* s)
	{
		_inputSet = s;
	}

	Geometry::GeometrySet* GeometryParaChamfer::getGeometrySet()
	{
		return _inputSet;
	}

	void GeometryParaChamfer::setCombIndex(bool i)
	{
		_combindex0 = i;
	}

	bool GeometryParaChamfer::getCombIndex()
	{
		return _combindex0;
	}

	void GeometryParaChamfer::setEdgeIndexList(QList<int> s)
	{
		_edgeindexlist = s;
	}

	QList<int> GeometryParaChamfer::getEdgeIndexList()
	{
		return _edgeindexlist;
	}

	void GeometryParaChamfer::setDistance1(double d1)
	{
		_d1 = d1;
	}

	void GeometryParaChamfer::setDistance2(double d2)
	{
		_d2 = d2;
	}

	double GeometryParaChamfer::getDistance1()
	{
		return _d1;
	}

	double GeometryParaChamfer::getDistance2()
	{
		return _d2;
	}

	QDomElement& GeometryParaChamfer::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{
		QDomElement element = doc->createElement("Parameter");  //创建子节点
		QDomAttr typeattr = doc->createAttribute("Type");
		typeattr.setValue(this->typeToString());
		element.setAttributeNode(typeattr);

		QDomAttr rad1attr = doc->createAttribute("Distance1");
		rad1attr.setValue(QString::number(_d1));
		element.setAttributeNode(rad1attr);

		QDomAttr rad2attr = doc->createAttribute("Distance2");
		rad2attr.setValue(QString::number(_d2));
		element.setAttributeNode(rad2attr);

		if (_inputSet != nullptr)
		{
			QDomElement idEle = doc->createElement("SubID");
			QDomText idText = doc->createTextNode(QString::number(_inputSet->getID()));
			idEle.appendChild(idText);
			element.appendChild(idEle);
		}
		QDomElement _combindex0Ele = doc->createElement("SectionType");
		QString secstr;
		if (_combindex0 == true) secstr = QString("Symmetrical");
		else secstr = QString("Asymmetrical");
		QDomText _combindex0Text = doc->createTextNode(secstr);
		_combindex0Ele.appendChild(_combindex0Text);
		element.appendChild(_combindex0Ele);

		if (_edgeindexlist.size()!=0)
		{
			QDomElement edgeEle = doc->createElement("Edges");
			QStringList edgeIndexs;
			for (int i = 0; i < _edgeindexlist.size(); ++i)
				edgeIndexs.append(QString::number(_edgeindexlist.at(i)));
			QDomText edgeText = doc->createTextNode(edgeIndexs.join(","));
			edgeEle.appendChild(edgeText);
			element.appendChild(edgeEle);
		}
		

		parent->appendChild(element);
		return element;
	}

	void GeometryParaChamfer::readDataFromProjectFile(QDomElement* e)
	{
 		_d1 = e->attribute("Distance1").toDouble();
		_d2 = e->attribute("Distance2").toDouble();
	
		QString temp = e->attribute("SectionType");
		if (temp == "Symmetrical") _combindex0 = true;
		else _combindex0 = false;
 		
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
		}

	}

	

}