#include "geometryParaVariableFillet.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDomAttr>
#include <QDebug>
#include "geometrySet.h"
#include "geometryData.h"

namespace Geometry
{
	GeometryParaVariableFillet::GeometryParaVariableFillet()
	{
		_type = GeometryParaMakeVariableFillet;
	}
	void GeometryParaVariableFillet::setInputSet(GeometrySet* s)
	{
		_inputSet = s;
	}

	Geometry::GeometrySet* GeometryParaVariableFillet::getGeometrySet()
	{
		return _inputSet;
	}

	void GeometryParaVariableFillet::setEdgePair(QPair<Geometry::GeometrySet*, int> edgepair)
	{
		_edgpair = edgepair;
	}

	QPair<Geometry::GeometrySet*, int> GeometryParaVariableFillet::getEdgePair()
	{
		return _edgpair;
	}


	void GeometryParaVariableFillet::setBasicRadius(double b)
	{
		_basicradius = b;
	}

	double GeometryParaVariableFillet::getBasicRadius()
	{
		return _basicradius;
	}

	void GeometryParaVariableFillet::setRadiuMap(QMap<double, double> s)
	{
		_radiusmap = s;
	}

	QMap<double, double> GeometryParaVariableFillet::getRadiuMap()
	{
		return _radiusmap;
	}

	QDomElement& GeometryParaVariableFillet::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{
		QDomElement element = doc->createElement("Parameter");  //创建子节点

		QDomAttr typeattr = doc->createAttribute("Type");
		typeattr.setValue(this->typeToString());
		element.setAttributeNode(typeattr);

		QDomAttr rad1attr = doc->createAttribute("EdgeIndex");
		rad1attr.setValue(QString::number(_edgpair.second));
		element.setAttributeNode(rad1attr);
		if (_edgpair.first != nullptr)
		{
			QDomAttr rad2attr = doc->createAttribute("EdgeSet");
			rad2attr.setValue(QString::number(_edgpair.first->getID()));
			element.setAttributeNode(rad2attr);
		}

		if (_inputSet != nullptr)
		{
			QDomAttr idEle = doc->createAttribute("SubID");
			idEle.setValue(QString::number(_inputSet->getID()));
			element.setAttributeNode(idEle);
		}
		QDomAttr _combindex0Ele = doc->createAttribute("BasicRadius");
		_combindex0Ele.setValue(QString::number(_basicradius));
		element.setAttributeNode(_combindex0Ele);

		if (_radiusmap.size() > 0)
		{
			QList<double> locList = _radiusmap.keys();
			QList<double> radiusList = _radiusmap.values();
			QString locStr{};
			for (int i = 0; i < locList.size(); ++i)
			{
				locStr.append(QString::number((locList[i])));
				if (i != (locList.size() - 1)) locStr.append(",");
			}

			QString radListStr{};
			for (int i = 0; i < radiusList.size(); ++i)
			{
				radListStr.append(QString::number(radiusList[i]));
				if (i != (radiusList.size() - 1)) radListStr.append(",");
			}

			QDomElement loclistEle = doc->createElement("LocList");
			QDomText loclistText = doc->createTextNode(locStr);
			loclistEle.appendChild(loclistText);
			element.appendChild(loclistEle);

			QDomElement radlistEle = doc->createElement("RadList");
			QDomText radlistText = doc->createTextNode(radListStr);
			radlistEle.appendChild(radlistText);
			element.appendChild(radlistEle);
		}

		parent->appendChild(element);
		return element;
	}

	void GeometryParaVariableFillet::readDataFromProjectFile(QDomElement* e)
	{
		_edgpair.second= e->attribute("EdgeIndex").toInt();
		int edgesetid = e->attribute("EdgeSet").toInt();
		_edgpair.first = Geometry::GeometryData::getInstance()->getGeometrySetByID(edgesetid);
		int subid = e->attribute("SubID").toInt();
		_inputSet = Geometry::GeometryData::getInstance()->getGeometrySetByID(subid);
		_basicradius = e->attribute("BasicRadius").toDouble();

		QDomNodeList setIdList = e->elementsByTagName("LocList");
		if (setIdList.size() != 1) return;
		QDomElement coorele = setIdList.at(0).toElement();
		QString coorstr = coorele.text();
		QStringList coorsl = coorstr.split(",");
		if (coorsl.size() < 1) return;
		QList<double> setidList;
		for (int i = 0; i < coorsl.size(); ++i)
		{
			setidList.append(coorsl.at(i).toDouble());
		}

		QDomNodeList startList = e->elementsByTagName("RadList");
		if (startList.size() != 1) return;
		QDomElement startele = startList.at(0).toElement();
		QString startstr = startele.text();
		QStringList startsl = startstr.split(",");
		if (startsl.size() < 1) return;
		QList<double> indexList;
		for (int i = 0; i < startsl.size(); ++i)
		{
			indexList.append(startsl.at(i).toDouble());
		}
		for (int i = 0; i < setidList.size(); ++i)
		{
			_radiusmap.insert(setidList[i], indexList[i]);
		}

	}

	

}