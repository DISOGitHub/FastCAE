#include "geometryParaMakeFillGap.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDomAttr>
#include "geometrySet.h"
#include "geometryData.h"
#include <QDebug>
#include <QPair>
namespace Geometry
{
	GeometryParaMakeFillGap::GeometryParaMakeFillGap()
	{
		_type = GeometryParaFillGap;
	}

	void GeometryParaMakeFillGap::setInputShapes(QList<QPair<Geometry::GeometrySet*, int>> s)
	{
		_shapelist = s;
	}

	QList<QPair<Geometry::GeometrySet*, int>> GeometryParaMakeFillGap::getShapes()
	{
		return  _shapelist;
	}

	void GeometryParaMakeFillGap::setType(int b)
	{
		_filltype = b;
	}
	int GeometryParaMakeFillGap::getType()
	{
		return _filltype;
	}
	QDomElement& GeometryParaMakeFillGap::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{
		
		QDomElement element = doc->createElement("Parameter");  //创建子节点
		QDomAttr typeattr = doc->createAttribute("Type");
		typeattr.setValue(this->typeToString());
		element.setAttributeNode(typeattr);

		QDomAttr booltypeattr = doc->createAttribute("FillType");
		booltypeattr.setValue(QString::number(_filltype));
		element.setAttributeNode(booltypeattr);

		if (_shapelist.size() == 2)
		{
			QDomElement body1Ele = doc->createElement("Shape1");
			QDomText body1Text = doc->createTextNode(QString::number(_shapelist.front().first->getID()));
			body1Ele.appendChild(body1Text);
			element.appendChild(body1Ele);

			QDomElement bodyindex1Ele = doc->createElement("ShapeIndex1");
			QDomText bodyindex1Text = doc->createTextNode(QString::number(_shapelist.front().second));
			bodyindex1Ele.appendChild(bodyindex1Text);
			element.appendChild(bodyindex1Ele);

			QDomElement body2Ele = doc->createElement("Shape2");
			QDomText body2Text = doc->createTextNode(QString::number(_shapelist.back().first->getID()));
			body2Ele.appendChild(body2Text);
			element.appendChild(body2Ele);

			QDomElement bodyindex2Ele = doc->createElement("ShapeIndex2");
			QDomText bodyindex2Text = doc->createTextNode(QString::number(_shapelist.back().second));
			bodyindex2Ele.appendChild(bodyindex2Text);
			element.appendChild(bodyindex2Ele);
		}

		parent->appendChild(element);
		return element;
	}

	void GeometryParaMakeFillGap::readDataFromProjectFile(QDomElement* e)
	{
		_shapelist.clear();
		_filltype = e->attribute("FillType").toInt();
		
		QDomNodeList nodeidList = e->elementsByTagName("Shape1");
		if (nodeidList.size() < 1) return;
		QDomElement idele = nodeidList.at(0).toElement();
		QString sid = idele.text();
		int id = sid.toInt();
		Geometry::GeometrySet* set1= _geoData->getGeometrySetByID(id);
		if (set1 == nullptr) return;
		QDomNodeList indexList1 = e->elementsByTagName("ShapeIndex1");
		if (indexList1.size() < 1) return;
		QDomElement indexele1 = indexList1.at(0).toElement();
		QString indexid1 = indexele1.text();
		int index1 = indexid1.toInt();
		_shapelist.push_back(QPair<Geometry::GeometrySet*, int>(set1, index1));
		
		QDomNodeList node2idList = e->elementsByTagName("Shape2");
		if (node2idList.size() < 1) return;
		QDomElement id2ele = node2idList.at(0).toElement();
		QString s2id = id2ele.text();
		int id2 = s2id.toInt();
		Geometry::GeometrySet* set2 = _geoData->getGeometrySetByID(id2);
		if (set2 == nullptr) return;

		QDomNodeList indexList2 = e->elementsByTagName("ShapeIndex2");
		if (indexList2.size() < 1) return;
		QDomElement indexele2 = indexList2.at(0).toElement();
		QString indexid2 = indexele2.text();
		int index2 = indexid2.toInt();
		_shapelist.push_back(QPair<Geometry::GeometrySet*, int>(set2, index2));
	}

	

}