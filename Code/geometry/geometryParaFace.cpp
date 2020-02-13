#include "geometryParaFace.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDomAttr>
#include <QDebug>
#include "geometrySet.h"
#include "geometryData.h"

namespace Geometry
{
	GeometryParaFace::GeometryParaFace()
	{
		_type = GeometryParaCreateFace;
	}

	void GeometryParaFace::setName(QString name)
	{
		_name = name;
	}

	QString GeometryParaFace::getName()
	{
		return _name;
	}

	

	void GeometryParaFace::setShapeHash(QMultiHash<Geometry::GeometrySet*, int> shapehash)
	{
		_shapeHash = shapehash;
	}

	QMultiHash<Geometry::GeometrySet*, int> GeometryParaFace::getShapeHash()
	{
		return _shapeHash;
	}
/*

	void GeometryParaFace::setEdges(int s)
	{
		_edges = s;
	}

	int GeometryParaFace::getEdges()
	{
		return _edges;
	}*/

	void GeometryParaFace::setActor(QList<vtkActor*> a)
	{
		_actor = a;
	}

	QList<vtkActor*> GeometryParaFace::getActor()
	{
		return _actor;
	}

	QDomElement& GeometryParaFace::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{
		QDomElement element = doc->createElement("Parameter");  //创建子节点
		QDomAttr typeattr = doc->createAttribute("Type");
		typeattr.setValue(this->typeToString());
		element.setAttributeNode(typeattr);

		QDomAttr nameattr = doc->createAttribute("Name");
		nameattr.setValue(_name);
		element.setAttributeNode(nameattr);

	/*	QDomAttr edgesizeattr = doc->createAttribute("Edges");
		edgesizeattr.setValue(QString::number(_actor.size()));
		element.setAttributeNode(edgesizeattr);
*/
		if (_shapeHash.size() == 0) return element;

		QList<Geometry::GeometrySet*> setList = _shapeHash.uniqueKeys();
		QList<int> setidList;
		QString setidStr{};
		for (int i = 0; i < setList.size();++i)
		{
			setidStr.append(QString::number((setList[i]->getID())));
			if (i!=(setList.size()-1)) setidStr.append(",");

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


		parent->appendChild(element);
		return element;
	}

	void GeometryParaFace::readDataFromProjectFile(QDomElement* e)
	{
		_name = e->attribute("Name");
		//_edges = e->attribute("Edges").toInt();
		
		QDomNodeList setIdList = e->elementsByTagName("EdgeSetIDList");
		if (setIdList.size() != 1) return;
		QDomElement coorele = setIdList.at(0).toElement();
		QString coorstr = coorele.text();
		QStringList coorsl = coorstr.split(",");
		if (coorsl.size() < 1 ) return;
		QList<int> setidList;
		for (int i = 0; i<coorsl.size(); ++i)
		{
			 setidList.append(coorsl.at(i).toInt());
		}

		QDomNodeList startList = e->elementsByTagName("IndexList");
		if (startList.size() != 1) return;
		QDomElement startele = startList.at(0).toElement();
		QString startstr = startele.text();
		QStringList startsl = startstr.split(",");
		if (startsl.size() <1) return;
		QList<int> indexList;
		for (int i = 0; i < startsl.size(); ++i)
		{
			indexList.append( startsl.at(i).toInt());
		}

		Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
		for (int i = 0; i < setidList.size();++i)
		{
			Geometry::GeometrySet*set = data->getGeometrySetByID(setidList[i]);
			_shapeHash.insert(set, indexList[i]);
		}


	}

	

}