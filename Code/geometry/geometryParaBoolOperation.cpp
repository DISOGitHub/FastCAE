#include "geometryParaBoolOperation.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDomAttr>
#include "geometrySet.h"
#include "geometryData.h"
#include <QDebug>
namespace Geometry
{
	GeometryParaBoolOperation::GeometryParaBoolOperation()
	{
		_type = GeometryParaMakeBoolOperation;
	}

	void GeometryParaBoolOperation::setInputBody1(QPair <Geometry::GeometrySet*, int> body1)
	{
		_body1 = body1;
	}


	QPair <Geometry::GeometrySet*, int> GeometryParaBoolOperation::getBody1()
	{
		return _body1;
	}

	void GeometryParaBoolOperation::setInputBody2(QPair <Geometry::GeometrySet*, int> body2)
	{
		_body2 = body2;
	}

	QPair <Geometry::GeometrySet*, int> GeometryParaBoolOperation::getBody2()
	{
		return _body2;
	}

	void GeometryParaBoolOperation::setType(BoolType b)
	{
		_booltype = b;
	}

	BoolType GeometryParaBoolOperation::getType()
	{
		return _booltype;
	}

	QDomElement& GeometryParaBoolOperation::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{
		
		QDomElement element = doc->createElement("Parameter");  //创建子节点
		QDomAttr typeattr = doc->createAttribute("Type");
		typeattr.setValue(this->typeToString());
		element.setAttributeNode(typeattr);

		QDomAttr booltypeattr = doc->createAttribute("BoolType");
		booltypeattr.setValue(BoolTypeToString(_booltype));
		element.setAttributeNode(booltypeattr);

		if (_body1.first != nullptr)
		{
			QDomElement body1Ele = doc->createElement("Body1");
			QDomText body1Text = doc->createTextNode(QString::number(_body1.first->getID()));
			body1Ele.appendChild(body1Text);
			element.appendChild(body1Ele);

			QDomElement bodyindex1Ele = doc->createElement("BodyIndex1");
			QDomText bodyindex1Text = doc->createTextNode(QString::number(_body1.second));
			bodyindex1Ele.appendChild(bodyindex1Text);
			element.appendChild(bodyindex1Ele);

		}
		
		if (_body2.first != nullptr)
		{
			QDomElement body2Ele = doc->createElement("Body2");
			QDomText body2Text = doc->createTextNode(QString::number(_body2.first->getID()));
			body2Ele.appendChild(body2Text);
			element.appendChild(body2Ele);

			QDomElement bodyindex2Ele = doc->createElement("BodyIndex2");
			QDomText bodyindex2Text = doc->createTextNode(QString::number(_body1.second));
			bodyindex2Ele.appendChild(bodyindex2Text);
			element.appendChild(bodyindex2Ele);
		}

		parent->appendChild(element);
		return element;
	}

	void GeometryParaBoolOperation::readDataFromProjectFile(QDomElement* e)
	{
		_booltype = StringToBoolType(e->attribute("BoolType"));
		
		QDomNodeList nodeidList = e->elementsByTagName("Body1");
		if (nodeidList.size() < 1) return;
		QDomElement idele = nodeidList.at(0).toElement();
		QString sid = idele.text();
		int id = sid.toInt();
		_body1.first = _geoData->getGeometrySetByID(id);

		QDomNodeList indexList1 = e->elementsByTagName("BodyIndex1");
		if (indexList1.size() < 1) return;
		QDomElement indexele1 = indexList1.at(0).toElement();
		QString indexid1 = indexele1.text();
		_body1.second = indexid1.toInt();
		
		QDomNodeList node2idList = e->elementsByTagName("Body2");
		if (node2idList.size() < 1) return;
		QDomElement id2ele = node2idList.at(0).toElement();
		QString s2id = id2ele.text();
		int id2 = s2id.toInt();
		_body2.first = _geoData->getGeometrySetByID(id2);

		QDomNodeList indexList2 = e->elementsByTagName("BodyIndex2");
		if (indexList2.size() < 1) return;
		QDomElement indexele2 = indexList2.at(0).toElement();
		QString indexid2 = indexele2.text();
		_body2.second = indexid2.toInt();

	}

	QString GeometryParaBoolOperation::BoolTypeToString(BoolType b)
	{
	
		QString text;
		switch (_booltype)
		{
		case BoolCut:
			text = "Cut";
			break;
		case BoolFause:
			text = "Fause";
			break;
		case BoolCommon:
			text = "Common";
			break;
		default:
			text = "Unmaned";
			break;
		}
		return text;
	}

	BoolType GeometryParaBoolOperation::StringToBoolType(QString s)
	{
		BoolType text;
		if (s == "Cut")text = BoolCut;
		else if (s == "Fause") text = BoolFause;
		else if (s == "Common") text = BoolCommon;
		else if(s=="Unmaned")text = BoolNone;
		return text;
	}

}