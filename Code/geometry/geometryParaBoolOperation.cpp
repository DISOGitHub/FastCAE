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

	void GeometryParaBoolOperation::setInputBody1(GeometrySet* body1)
	{
		_body1 = body1;
	}

	Geometry::GeometrySet* GeometryParaBoolOperation::getBody1()
	{
		return _body1;
	}

	void GeometryParaBoolOperation::setInputBody2(GeometrySet* body2)
	{
		_body2 = body2;
	}

	Geometry::GeometrySet* GeometryParaBoolOperation::getBody2()
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

		if (_body1 != nullptr)
		{
			QDomElement body1Ele = doc->createElement("Body1");
			QDomText body1Text = doc->createTextNode(QString::number(_body1->getID()));
			body1Ele.appendChild(body1Text);
			element.appendChild(body1Ele);
		}
		
		if (_body2 != nullptr)
		{
			QDomElement body2Ele = doc->createElement("Body2");
			QDomText body2Text = doc->createTextNode(QString::number(_body2->getID()));
			body2Ele.appendChild(body2Text);
			element.appendChild(body2Ele);
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
		_body1 = _geoData->getGeometrySetByID(id);
		
		QDomNodeList node2idList = e->elementsByTagName("Body2");
		if (node2idList.size() < 1) return;
		QDomElement id2ele = node2idList.at(0).toElement();
		QString s2id = id2ele.text();
		int id2 = s2id.toInt();
		_body2 = _geoData->getGeometrySetByID(id2);
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