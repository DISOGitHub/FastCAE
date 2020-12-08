#include "geometryDatum.h"
#include <QDomDocument>
#include <QDomElement>
#include <QCoreApplication>

namespace Geometry
{
	GeometryDatum::GeometryDatum()
		: GeometrySet(DATUM,true)
	{
	}
	GeometryDatum::~GeometryDatum()
	{

	}


	void GeometryDatum::setDatumType(DatumType t)
	{
		_type = t;
	}

	DatumType GeometryDatum::getDatumType()
	{
		return _type;
	}

	QDomElement& GeometryDatum::writeToProjectFile(QDomDocument* doc, QDomElement* ele, bool isDiso /* = false */)
	{
		QDomElement element = doc->createElement("GeoDatum");  //创建子节点
		QDomAttr idattr = doc->createAttribute("ID");
		idattr.setValue(QString::number(_id));
		element.setAttributeNode(idattr);
		QDomAttr visible = doc->createAttribute("Visible");
		visible.setValue("True");
		if (!_visible) visible.setValue("False");
		element.setAttributeNode(visible);
		QString stype = this->datumTypeToString(_type);
		element.setAttribute("Type", stype);
		QDomElement nameele = doc->createElement("Name");
		QDomText nameText = doc->createTextNode(_name);
		nameele.appendChild(nameText);
		element.appendChild(nameele);

		ele->appendChild(element);  //子节点挂载

		if (isDiso)
		{
			QString exelPath = QCoreApplication::applicationDirPath();
			const QString tempPath = exelPath + "/../tempIO/" + QString("datum_%1.brep").arg(_id);
			bool ok = this->writeBrep(tempPath);
		}
		return element;
	}

	

	void GeometryDatum::readDataFromProjectFile(QDomElement* element, bool isDiso /* = false */)
	{
		int id = element->attribute("ID").toInt();
		this->setID(id);
		bool visible = true;
		QString vis = element->attribute("Visible");
		if (vis.toLower() == "false") visible = false;
		this->setVisible(visible);
		QString stype = element->attribute("Type");
		_type = datumTypeFromString(stype);
		QDomNodeList nameNode = element->elementsByTagName("Name");

		if (nameNode.size() != 1) return;
		QString name = nameNode.at(0).toElement().text();
		//		qDebug() << name;
		this->setName(name);

		if (isDiso)
		{
			QString exelPath = QCoreApplication::applicationDirPath();
			const QString tempPath = exelPath + "/../tempIO/" + QString("datum_%1.brep").arg(_id);
			this->readBrep(tempPath);
		}

	}

	QString GeometryDatum::datumTypeToString(DatumType t)
	{
		QString stype = "None";
		switch (t)
		{
		case Geometry::DatumNone: break;
		case Geometry::DatumPoint:
			stype = "DatumPoint"; break;
		case Geometry::DatumAxis:
			stype = "DatumAxis"; break;
		case Geometry::DatumPlane:
			stype = "DatumPlane"; break;
		default: break;
		}
		return stype;
	}

	Geometry::DatumType GeometryDatum::datumTypeFromString(QString s)
	{
		DatumType t= DatumNone;
		s = s.toLower();
		if (s == "datumpoint")
			t = DatumPoint;
		else if (s == "datumaxis")
			t = DatumAxis;
		else if (s == "datumplane")
			t = DatumPlane;
		return t;
		
	}

}
