#include "geometryParaLoft.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDomAttr>
#include <QDebug>
#include "geometrySet.h"
#include "geometryData.h"

namespace Geometry
{
	GeometryParaLoft::GeometryParaLoft()
	{
		_type = GeometryParaMakeLoft;
	}
	
	void GeometryParaLoft::setShapeHash(QList<QMultiHash<Geometry::GeometrySet*, int>> shapehash)
	{
		_shapeHash = shapehash;
	}

	void GeometryParaLoft::setName(QString s)
	{
		_name = s;
	}

	QString GeometryParaLoft::getName()
	{
		return _name;
	}

	QList<QMultiHash<Geometry::GeometrySet*, int>> GeometryParaLoft::getShapeHash()
	{
		return _shapeHash;
	}

	void GeometryParaLoft::setSolid(bool s)
	{
		_issolid = s;

	}
	bool GeometryParaLoft::getSloid()
	{
		return _issolid;
	}

	QDomElement& GeometryParaLoft::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{
		QDomElement element = doc->createElement("Parameter");  //创建子节点
		QDomAttr typeattr = doc->createAttribute("Type");
		typeattr.setValue(this->typeToString());
		element.setAttributeNode(typeattr);
		if (_shapeHash.size()>0)
		{
			QString secstr{};
			for (int i = 0; i < _shapeHash.size(); i++)
			{
				QList<QString*> strlist{};
				QList<Geometry::GeometrySet*> sets = _shapeHash[i].uniqueKeys();
				QString sec{};

				for (int j = 0; j < sets.size(); ++j)
				{
					QString* temp = new QString();
					auto s = sets.at(j);
					int id = s->getID();
					*temp = QString::number(id) + ":";
					//(*temp).append(":");
					QList<int> indexs = _shapeHash[i].values(s);
					int nmu = indexs.size();
					for (int k = 0; k< indexs.size(); k++)
					{
						(*temp) += QString::number(indexs[k]);
						if (k < indexs.size() - 1)
						{
							(*temp).append(",");
						}
						else if (k == (indexs.size() - 1))
						{
							(*temp).append(";");
						}
					}

					strlist << temp;
				}
				for (int i = 0; i < strlist.size(); i++)
				{
					sec += strlist[i];
				}
	
				secstr=secstr+sec+" ";
			}
			QDomElement startpointEle = doc->createElement("SectionList");
			QDomText startpointText = doc->createTextNode(secstr);
			startpointEle.appendChild(startpointText);
			element.appendChild(startpointEle);

		}
		QDomElement solidEle = doc->createElement("Solid");
		QString solidStr = QString("%1").arg(_issolid);
		QDomText solidText = doc->createTextNode(solidStr);
		solidEle.appendChild(solidText);
		element.appendChild(solidEle);

		parent->appendChild(element);
		return element;
	}

	void GeometryParaLoft::readDataFromProjectFile(QDomElement* e)
	{
		_name = e->attribute("Name");
		QDomNodeList setIdList = e->elementsByTagName("SectionList");
		if (setIdList.size() != 1) return;
		QDomElement coorele = setIdList.at(0).toElement();
		QString coorstr = coorele.text();

		QStringList selist = coorstr.simplified().split(" ");
		for (QString var : selist)
		{
			QMultiHash<Geometry::GeometrySet*, int> temphash{};
			var.replace(";", " ");
			QStringList varlist = var.simplified().split(" ");
			for (QString var : varlist)
			{

				var.replace(":", " ");
				QStringList hash = var.simplified().split(" ");
				QStringList indexlist = hash[1].simplified().split(",");
				for (QString var : indexlist)
				{
					Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
					Geometry::GeometrySet* geoset = data->getGeometrySetByID(hash[0].toInt());
					temphash.insert(geoset, var.toInt());
				}

			}
			_shapeHash.push_back(temphash);

		}
			QDomNodeList solidList = e->elementsByTagName("Solid");
			QString solidsr = solidList.at(0).toElement().text();
			if (solidsr == "1") _issolid = true;
			else if (solidsr == "0") _issolid = false;
	}
	

}