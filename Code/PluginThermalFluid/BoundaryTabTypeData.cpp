#include "BoundaryTabTypeData.h"
#include <QDomDocument>
#include <qDebug>

namespace ThermalFluid{
	BoundaryTabTypeData::BoundaryTabTypeData()
	{

	}

	BoundaryTabTypeData::~BoundaryTabTypeData()
	{
		clearYXList();
	}

// 	void BoundaryTabTypeData::setID(int id)
// 	{
// 		_id = id;
// 	}
// 
// 	int BoundaryTabTypeData::getID()
// 	{
// 		return _id;
// 	}

	void BoundaryTabTypeData::setType(TabType t)
	{
		_type = t;
	}

	TabType BoundaryTabTypeData::getType()
	{
		return _type;
	}

	void BoundaryTabTypeData::appendYX(double y, double x)
	{
		double* v = new double[2];
		v[0] = y;
		v[1] = x;

		_yx.append(v);
	}

	int BoundaryTabTypeData::getYXCount()
	{
		return _yx.count();
	}

	double* BoundaryTabTypeData::getYXValue(int index)
	{		
		if (index < 0 || index >= _yx.count())
			return 0;
		
		return _yx.at(index);
	}

	TabType BoundaryTabTypeData::stringToTabType(QString s)
	{
		s = s.toUpper();
		if (s == "QNODE")
			return TabType::TQNODE;
		else if (s == "TEMP")
			return TabType::TEMP;
		else if (s == "ROTAT")
			return TabType::ROTAT;
		else
			return TabType::TNONE;
	}

	QString BoundaryTabTypeData::tabTypeToString(TabType t)
	{
		QString s{};
		if (t == TabType::TEMP)
			s = "TEMP";
		else if (t == TabType::TQNODE)
			s = "QNODE";
		else if (t == TabType::ROTAT)
			s = "ROTAT";
		else
			s = "NULL";

		return s;
	}

	void BoundaryTabTypeData::clearYXList()
	{
		for (int i = 0; i < _yx.size(); i++)
		{
			double* yx = _yx.at(i);
			delete yx;
		}

		_yx.clear();
	}

	void BoundaryTabTypeData::writeBoundaryTabTypeData(QDomDocument* doc, QDomElement* bttd)
	{
		QDomElement aBttd = doc->createElement("aBoundaryTabTypeData");
		aBttd.setAttribute("id", _id);
		aBttd.setAttribute("type", tabTypeToString(_type));
		for (int i = 0; i < _yx.size(); i++)
		{
			QDomElement aYx = doc->createElement("yx");
			double *ptr = _yx[i];
			QString strYx = QString("%1;%2").arg(ptr[0]).arg(ptr[1]);
			aYx.appendChild(doc->createTextNode(strYx));
			aBttd.appendChild(aYx);
		}
		bttd->appendChild(aBttd);
	}

	void BoundaryTabTypeData::readBoundaryTabTypeData(QDomElement* aBttd)
	{
		if (!aBttd->isNull())
		{
			_id = aBttd->attribute("id").toInt();
			_type = stringToTabType(aBttd->attribute("type"));

			QDomNodeList yxList = aBttd->childNodes();
			for (int i = 0; i < yxList.count(); i++)
			{
				QStringList strYxList = yxList.at(i).toElement().text().split(';');
				appendYX(strYxList.at(0).toDouble(), strYxList.at(1).toDouble());
			}
		}
	}

}