#include "geometryParaMakeMatrix.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDomAttr>
#include "geometrySet.h"
#include "geometryData.h"
#include <QDebug>
namespace Geometry
{
	GeometryParaMatrix::GeometryParaMatrix()
	{
		_type = GeometryParaMakeMatrix;
	}

	void GeometryParaMatrix::appendBody(Geometry::GeometrySet* set, int bodyindex)
	{
		_solidHash.insert(set, bodyindex);
	}

	QMultiHash<Geometry::GeometrySet*, int> GeometryParaMatrix::getBodys()
	{
		return _solidHash;
	}

	void GeometryParaMatrix::setCurrentIndex(int index)
	{
		_optionindex = index;
	}
	
	int GeometryParaMatrix::getCurrentIndex()
	{
		return _optionindex;
	}

	void GeometryParaMatrix::setDirection1(double* dir)
	{
		for (int i = 0; i < 3; ++i)
		{
			_dir1[i] = dir[i];
		}
	}

	void GeometryParaMatrix::getDirection1(double* dir)
	{
		for (int i = 0; i < 3; ++i)
		{
			dir[i] = _dir1[i];
		}
	}

	void GeometryParaMatrix::setReverse1(bool s)
	{
		_reverse1 = s;
	}

	bool GeometryParaMatrix::getReverse1()
	{
		return _reverse1;
	}

	void GeometryParaMatrix::setDistance1(double dis)
	{
		_dis1 = dis;
	}

	double GeometryParaMatrix::getDistance1()
	{
		return _dis1;
	}

	void GeometryParaMatrix::setCount1(int n)
	{
		_count1 = n;
	}

	int GeometryParaMatrix::getCount1()
	{
		return _count1;
	}

	void GeometryParaMatrix::setShowDir2(bool s)
	{
		_showdir2 = s;
	}

	bool GeometryParaMatrix::showtDir2()
	{
		return _showdir2;
	}

	void GeometryParaMatrix::setDirection2(double* dir)
	{
		for (int i = 0; i < 3; ++i)
		{
			_dir2[i] = dir[i];
		}
	}

	void GeometryParaMatrix::getDirection2(double* dir)
	{
		for (int i = 0; i < 3; ++i)
		{
			dir[i] = _dir2[i];
		}
	}

	void GeometryParaMatrix::setReverse2(bool s)
	{
		_reverse2 = s;

	}

	bool GeometryParaMatrix::getReverse2()
	{
		return _reverse2;
	}

	void GeometryParaMatrix::setDistance2(double dis)
	{
		_dis2 = dis;
	}

	double GeometryParaMatrix::getDistance2()
		 
	{
		return _dis2;
	}

	void GeometryParaMatrix::setCount2(int n)
	{
		_count2 = n;
	}

	int GeometryParaMatrix::getCount2()
	{
		return _count2;
	}

	void GeometryParaMatrix::setAxis(double* axis)
	{
		for (int i = 0; i < 3; ++i)
		{
			_axis[i] = axis[i];
		}
	}

	void GeometryParaMatrix::getAxis(double* axis)
	{
		for (int i = 0; i < 3; ++i)
		{
			axis[i] = _axis[i];
		}
	}

	void GeometryParaMatrix::setAxisDir(double* dir)
	{
		for (int i = 0; i < 3; ++i)
		{
			_axisdir[i] = dir[i];
		}
	}

	void GeometryParaMatrix::getAxisDir(double* dir)
	{
		for (int i = 0; i < 3; ++i)
		{
			dir[i] = _axisdir[i];
		}
	}

	void GeometryParaMatrix::setAxisRev(bool s)
	{
		_axisreverse = s;

	}

	bool GeometryParaMatrix::getAxisRev()
	{
		return _axisreverse;
	}

	void GeometryParaMatrix::setAxisCount(int n)
	{
		_axisacount = n;
	}

	int GeometryParaMatrix::getAxisCount()
	{
		return _axisacount;
	}

	void GeometryParaMatrix::setAngle(double angle)
	{
		_axisangle = angle;
	}

	double GeometryParaMatrix::getAngle()
	{
		return _axisangle;
	}


	void GeometryParaMatrix::setOriSet(Geometry::GeometrySet* s)
	{
		_oriset = s;
	}
	
	Geometry::GeometrySet* GeometryParaMatrix::getOriSet()
	{
		return _oriset;
	}

	QDomElement& GeometryParaMatrix::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{
		QDomElement element = doc->createElement("Parameter");  //创建子节点
		QDomAttr typeattr = doc->createAttribute("Type");
		typeattr.setValue(this->typeToString());
		element.setAttributeNode(typeattr);
		
		QDomElement indexEle = doc->createElement("OptionIndex");
		QString indexStr = QString("%1").arg(_optionindex);
		QDomText indexText = doc->createTextNode(indexStr);
		indexEle.appendChild(indexText);
		element.appendChild(indexEle);
		
		QStringList solidStrList{};
		if (_solidHash.size() > 0)
		{
			QMultiHash<Geometry::GeometrySet*, int>::iterator it = _solidHash.begin();
			for (; it != _solidHash.end(); it++)
			{
				solidStrList << QString("%1:%2)").arg(it.key()->getID()).arg(it.value());
			}
			QDomElement startpointEle = doc->createElement("BodyIDList");
			QDomText startpointText = doc->createTextNode(solidStrList.join(","));
			startpointEle.appendChild(startpointText);
			element.appendChild(startpointEle);
		}

		if (_oriset != nullptr)
		{
			QDomElement orisetEle = doc->createElement("Oriset");
			QDomText orisetText = doc->createTextNode(QString::number(_oriset->getID()));
			orisetEle.appendChild(orisetText);
			element.appendChild(orisetEle);
		}

		QDomElement dir1Ele = doc->createElement("Direction1");
		QString dir1str = QString("%1,%2,%3").arg(_dir1[0]).arg(_dir1[1]).arg(_dir1[2]);
		QDomText dir1Text = doc->createTextNode(dir1str);
		dir1Ele.appendChild(dir1Text);
		element.appendChild(dir1Ele);

		QDomElement reverse1Ele = doc->createElement("Reverse1");
		QString reverse1Str = QString("%1").arg(_reverse1);
		QDomText reverse1Text = doc->createTextNode(reverse1Str);
		reverse1Ele.appendChild(reverse1Text);
		element.appendChild(reverse1Ele);

		QDomElement distance1Ele = doc->createElement("Distance1");
		QString dis1Str = QString("%1").arg(_dis1);
		QDomText dis1Text = doc->createTextNode(dis1Str);
		distance1Ele.appendChild(dis1Text);
		element.appendChild(distance1Ele);

		QDomElement count1Ele = doc->createElement("Count1");
		QString count1Str = QString("%1").arg(_count1);
		QDomText count1Text = doc->createTextNode(count1Str);
		count1Ele.appendChild(count1Text);
		element.appendChild(count1Ele);

		QDomElement showdir2Ele = doc->createElement("ShowDir2");
		QString showdir2Str = QString("%1").arg(_showdir2);
		QDomText showdir2Text = doc->createTextNode(showdir2Str);
		showdir2Ele.appendChild(showdir2Text);
		element.appendChild(showdir2Ele);

		QDomElement dir2Ele = doc->createElement("Direction2");
		QString dir2str = QString("%1,%2,%3").arg(_dir2[0]).arg(_dir2[1]).arg(_dir2[2]);
		QDomText dir2Text = doc->createTextNode(dir2str);
		dir2Ele.appendChild(dir2Text);
		element.appendChild(dir2Ele);

		QDomElement reverse2Ele = doc->createElement("Reverse2");
		QString reverse2Str = QString("%1").arg(_reverse2);
		QDomText reverse2Text = doc->createTextNode(reverse2Str);
		reverse2Ele.appendChild(reverse2Text);
		element.appendChild(reverse2Ele);

		QDomElement distance2Ele = doc->createElement("Distance2");
		QString dis2Str = QString("%1").arg(_dis2);
		QDomText dis2Text = doc->createTextNode(dis2Str);
		distance2Ele.appendChild(dis2Text);
		element.appendChild(distance2Ele);

		QDomElement count2Ele = doc->createElement("Count2");
		QString count2Str = QString("%1").arg(_count2);
		QDomText count2Text = doc->createTextNode(count2Str);
		count2Ele.appendChild(count2Text);
		element.appendChild(count2Ele);
		
		QDomElement coorEle = doc->createElement("Axis");
		QString coorstr = QString("%1,%2,%3").arg(_axis[0]).arg(_axis[1]).arg(_axis[2]);
		QDomText coorText = doc->createTextNode(coorstr);
		coorEle.appendChild(coorText);
		element.appendChild(coorEle);

		QDomElement axisdirEle = doc->createElement("AxisDir");
		QString axisdirstr = QString("%1,%2,%3").arg(_axisdir[0]).arg(_axisdir[1]).arg(_axisdir[2]);
		QDomText axisdirText = doc->createTextNode(axisdirstr);
		axisdirEle.appendChild(axisdirText);
		element.appendChild(axisdirEle);
		
		QDomElement axisreverseEle = doc->createElement("AxisReverse");
		QString axisreverseStr = QString("%1").arg(_axisreverse);
		QDomText axisreverseText = doc->createTextNode(axisreverseStr);
		axisreverseEle.appendChild(axisreverseText);
		element.appendChild(axisreverseEle);

		QDomElement axiscountEle = doc->createElement("AxisCount");
		QString axiscountStr = QString("%1").arg(_axisacount);
		QDomText axiscountText = doc->createTextNode(axiscountStr);
		axiscountEle.appendChild(axiscountText);
		element.appendChild(axiscountEle);


		QDomElement angleEle = doc->createElement("Angle");
		QString angleStr = QString("%1").arg(_axisangle);
		QDomText angleText = doc->createTextNode(angleStr);
		angleEle.appendChild(angleText);
		element.appendChild(angleEle);
		
		parent->appendChild(element);
		return element;
	}

	void GeometryParaMatrix::readDataFromProjectFile(QDomElement* e)
	{
		Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
		QDomNodeList setIdList = e->elementsByTagName("BodyIDList");
		if (setIdList.size() != 1) return;
		QDomElement coorele = setIdList.at(0).toElement();
		QString coorstr = coorele.text();
		QStringList coorsl = coorstr.split(",");
		if (coorsl.size() < 1) return;
		for (int i = 0; i < coorsl.size(); ++i)
		{
			QStringList solidstr = coorsl[i].split(":");
			int setid = solidstr.at(0).toInt();
			int solidindex = solidstr.at(1).toInt();
			Geometry::GeometrySet *set = data->getGeometrySetByID(setid);
			_solidHash.insert(set, solidindex);
		}

		QDomNodeList nodeidList = e->elementsByTagName("Oriset");
		if (nodeidList.size() < 1) return;
		QDomElement idele = nodeidList.at(0).toElement();
		QString sid = idele.text();
		int orisetid = sid.toInt();
		_oriset = _geoData->getGeometrySetByID(orisetid);


		QDomNodeList typeindexList = e->elementsByTagName("OptionIndex");
		_optionindex = typeindexList.at(0).toElement().text().toInt();

		QDomNodeList dir1List = e->elementsByTagName("Direction1");
		if (dir1List.size() != 1) return;
		QDomElement dir1ele = dir1List.at(0).toElement();
		QString dir1str = dir1ele.text();
		QStringList dir1sl = dir1str.split(",");
		if (dir1sl.size() != 3) return;
		for (int i = 0; i < 3; ++i)
		{
			_dir1[i] = dir1sl.at(i).toDouble();
		}

		QDomNodeList r1List = e->elementsByTagName("Reverse1");
		QString r1 = r1List.at(0).toElement().text();
		if (r1 == "1") _reverse1 = true;
		else if (r1 == "0") _reverse1 = false;

		QDomNodeList dis1List = e->elementsByTagName("Distance1");
		_dis1 = dis1List.at(0).toElement().text().toDouble();

		QDomNodeList count1List = e->elementsByTagName("Count1");
		_count1 = count1List.at(0).toElement().text().toInt();

		QDomNodeList showdir2List = e->elementsByTagName("ShowDir2");
		QString showdir2str = showdir2List.at(0).toElement().text();
		if (showdir2str == "1") _showdir2 = true;
		else if (showdir2str == "0") _showdir2 = false;

		QDomNodeList dir2List = e->elementsByTagName("Direction2");
		if (dir2List.size() != 1) return;
		QDomElement dir2ele = dir2List.at(0).toElement();
		QString dir2str = dir2ele.text();
		QStringList dir2sl = dir2str.split(",");
		if (dir2sl.size() != 3) return;
		for (int i = 0; i < 3; ++i)
		{
			_dir2[i] = dir2sl.at(i).toDouble();
		}

		QDomNodeList r2List = e->elementsByTagName("Reverse2");
		QString r2 = r2List.at(0).toElement().text();
		if (r2 == "1") _reverse2 = true;
		else if (r2 == "0") _reverse2 = false;

		QDomNodeList dis2List = e->elementsByTagName("Distance2");
		_dis2 = dis2List.at(0).toElement().text().toDouble();

		QDomNodeList count2List = e->elementsByTagName("Count2");
		_count2 = count2List.at(0).toElement().text().toInt();


		QDomNodeList cList = e->elementsByTagName("Axis");
		if (cList.size() != 1) return;
		QDomElement cele = cList.at(0).toElement();
		QString cstr = cele.text();
		QStringList csl = cstr.split(",");
		if (csl.size() != 3) return;
		for (int i = 0; i < 3; ++i)
		{
			_axis[i] = csl.at(i).toDouble();
		}
		
		QDomNodeList pList = e->elementsByTagName("AxisDir");
		if (pList.size() != 1) return;
		QDomElement pele = pList.at(0).toElement();
		QString pstr = pele.text();
		QStringList psl = pstr.split(",");
		if (psl.size() != 3) return;
		for (int i = 0; i < 3; ++i)
		{
			_axisdir[i] = psl.at(i).toDouble();
		}

		QDomNodeList axisrList = e->elementsByTagName("AxisReverse");
		QString axisr = axisrList.at(0).toElement().text();
		if (axisr == "1") _axisreverse = true;
		else if (axisr == "0") _axisreverse = false;

		QDomNodeList axiscountList = e->elementsByTagName("AxisCount");
		_axisacount = axiscountList.at(0).toElement().text().toInt();

		QDomNodeList angleList = e->elementsByTagName("Angle");
		_axisangle = angleList.at(0).toElement().text().toDouble();

	}

	

}