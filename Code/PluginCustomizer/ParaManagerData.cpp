#include "ParaManagerData.h"
#include "ParaLinkageData.h"
#include "DataProperty/ParameterBase.h"
#include "DataProperty/ParameterInt.h"
#include "DataProperty/ParameterBool.h"
#include "DataProperty/ParameterDouble.h"
#include "DataProperty/ParameterString.h" 
#include "DataProperty/ParameterSelectable.h"
#include "DataProperty/ParameterTable.h"
#include "DataProperty/DataBase.h"
#include "DataProperty/ParameterGroup.h"
#include "DataProperty/ParameterPath.h"
#include "IOXml.h"
#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDebug>

namespace FastCAEDesigner{
	ParaManagerData* ParaManagerData::_instance = nullptr;

	ParaManagerData::ParaManagerData()
	{
		
	}


	ParaManagerData::~ParaManagerData()
	{
// 		clearQMap();
// 		_paraLinkageDataListDict.clear();
	}

	ParaManagerData* ParaManagerData::getInstance()
	{
		if (_instance == nullptr)
			_instance = new ParaManagerData();

		return _instance;
	}

	void ParaManagerData::setParameterLinkageList(int type, ParaLinkageData* data)
	{
		QList<ParaLinkageData*> list = _paraLinkageDataListDict[type]; 

		if (list.contains(data))
			list.removeOne(data);

		list.append(data);

		_paraLinkageDataListDict.insert(type, list);
	}

	ParaLinkageData* ParaManagerData::getParameterLinkageData(int type, int index)
	{
		QList<ParaLinkageData*> list = _paraLinkageDataListDict[type];
		return list.at(index);
	}

	QList<ParaLinkageData*> ParaManagerData::getParameterLinkageList(int type)
	{
		return _paraLinkageDataListDict[type];
	}

	void ParaManagerData::removeParameterLinkageDataFromList(int type, int index)
	{
		QList<ParaLinkageData*> list = _paraLinkageDataListDict[type];
		ParaLinkageData* data = list.at(index);
		if (data == nullptr)
			return;
		
		list.removeAt(index);
		delete data;
		data = nullptr;		

		_paraLinkageDataListDict.insert(type, list);
	}

	void ParaManagerData::removeParameterLinkageDataFromList(ParaLinkageData* data)
	{
		QList<ParaLinkageData*> list = _paraLinkageDataListDict[data->getParameterType()];
		list.removeOne(data);

		_paraLinkageDataListDict.insert(data->getParameterType(), list);
	}

	QString ParaManagerData::getFileName()
	{
		QString path = qApp->applicationDirPath();
		QDir dir(path);
		dir.cdUp();
		QString destPath = dir.path() + "/ConfigFiles" + "/ObserverConfig.config";

		return destPath;
	}

	//Ð´xml
	void ParaManagerData::writeObserverConfig()
	{
		if (!_writeConfig)
			return;
		
		QString destPath = getFileName();
		qDebug() << destPath;
		QFile observer(destPath);
		if (!observer.open(QIODevice::WriteOnly | QIODevice::Truncate))
		{
			qDebug() << "Wirte ObserverConfig file!";
			_paraLinkageDataListDict.clear();
			return;
		}
		
		QDomDocument doc;
		QDomProcessingInstruction ins;
		ins = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
		doc.appendChild(ins);
		QDomElement root = doc.createElement("ObserverConfig");

// 		QMap<QString, QList<ParaLinkageData*>>::const_iterator i;
// 		for (i = _paraLinkageDataListDict.constBegin(); i != _paraLinkageDataListDict.constBegin();++i)
// 		{
// 			writexml(doc, root, i.key(), i.value());
// 		}
// 		
		QMap<int, QList<ParaLinkageData*>>::iterator iter = _paraLinkageDataListDict.begin();
		while (iter != _paraLinkageDataListDict.end())
		{
			writexml(doc, root, iter.key(), iter.value());

			iter++;
		}


		doc.appendChild(root);

		QTextStream out_stream(&observer);
		doc.save(out_stream, 4); //Ëõ½ø1¸ñ
		observer.close();

		_paraLinkageDataListDict.clear();

		_readConfig = true;
		_writeConfig = false;
	}

	void ParaManagerData::writexml(QDomDocument doc, QDomElement root, int type, QList<ParaLinkageData*> list)
	{
		QString stype = QString("Type%1").arg(type);
		QDomElement paraList = IoXml::getInstance()->CreateElementAttribute(doc, "ObserverList", "Type", stype);
		for (int i = 0; i < list.count(); i++)
		{
			ParaLinkageData* data = list.at(i);
			if (data == nullptr)
				continue;
			
			QDomElement paraName = IoXml::getInstance()->CreateElementAttribute(doc, "Observer", "name", data->getParameterName());

// 			writeActiveData(doc, paraName, data);
// 			writeFollowData(doc, paraName, data);

			writeActieParamters(doc, paraName, data);
			writeFollowParamters(doc, paraName, data);
			
			paraList.appendChild(paraName);
		}
		root.appendChild(paraList);


	}

	void ParaManagerData::writeActiveData(QDomDocument doc, QDomElement root, ParaLinkageData* data)
	{
		QDomElement active = IoXml::getInstance()->CreateElement(doc, "Active");

		QList<DataProperty::ParameterBase*> list = data->getActiveList();
		for (int i = 0; i < list.count(); i++)
		{
			QStringList var;
			QStringList val;
			var.append("GroupName");
			var.append("Describe");
			var.append("Type");
			var.append("Value");

			DataProperty::ParameterBase* base = list.at(i);
			val.append(base->getGroupName());

			QString dsc{};
			if ((base->getGroupName()).isEmpty())
				dsc = base->getDescribe();
			else
				dsc = base->getGroupName() + "/" + base->getDescribe();
			val.append(dsc);
						
			val.append(base->ParaTypeToString(base->getParaType()));
			//qDebug() << base->ParaTypeToString(base->getParaType());

			QString value = getDataValue(base);
			val.append(value);

			if (base->getParaType() == DataProperty::Para_Selectable)
			{
				var.append("Range");
				val.append((dynamic_cast<DataProperty::ParameterSelectable*>(base)->getOption()).join(":"));
			}
			else if (base->getParaType() == DataProperty::Para_Path)
			{
				var.append("Pathtype");
				if (dynamic_cast<DataProperty::ParameterPath*>(base)->getType() == DataProperty::Path) val.append("Path");
				if (dynamic_cast<DataProperty::ParameterPath*>(base)->getType() == DataProperty::File) val.append("File");
				if (dynamic_cast<DataProperty::ParameterPath*>(base)->getType() == DataProperty::FileList) val.append("FileList");
			}

			QMap<QString,QString> map = insertValueToMap(var, val);

			QDomElement para = IoXml::getInstance()->CreateElement(doc, "Parameter", map);
			active.appendChild(para);
		}

		root.appendChild(active);
	}

	void ParaManagerData::writeFollowData(QDomDocument doc, QDomElement root, ParaLinkageData* data)
	{
		QDomElement follow = IoXml::getInstance()->CreateElement(doc, "Follow");

		writeDrivenData(doc, follow, data);
		writeDrivenGroupData(doc, follow, data);

		root.appendChild(follow);

	}

	void ParaManagerData::writeDrivenData(QDomDocument doc, QDomElement driven, ParaLinkageData* data)
	{
		QList<DataProperty::ParameterBase*> list = data->getDrivenList();
		//qDebug() << data->getDrivenList().size();
		for (int i = 0; i < list.count(); i++)
		{
			QStringList var;
			QStringList val;
			var.append("Describe");
			var.append("Type");
			var.append("Visible");
			var.append("Editable");

			DataProperty::ParameterBase* base = list.at(i);

			QString dsc{};
			if ((base->getGroupName()).isEmpty())
				dsc = base->getDescribe();
			else
				dsc = base->getGroupName() + "/" + base->getDescribe();
			val.append(dsc);

			val.append(base->ParaTypeToString(base->getParaType()));

			val.append(dataBoolToQString(base->isVisible()));

			val.append(dataBoolToQString(base->isEditable()));

			if (base->getParaType() == DataProperty::Para_Selectable)
			{
				var.append("CurrentIndex");
				QString index = QString("%1").arg(dynamic_cast<DataProperty::ParameterSelectable*>(base)->getCurrentIndex());
				val.append(index);

				QMap<QString, QString> map = insertValueToMap(var, val);
				QDomElement para = IoXml::getInstance()->CreateElement(doc, "Parameter", map);

				QString option = (dynamic_cast<DataProperty::ParameterSelectable*>(base)->getOption()).join(";");
				QDomElement SelOption = IoXml::getInstance()->CreateElement(doc, "Option", option);

				para.appendChild(SelOption);
				driven.appendChild(para);
			}
			else if (base->getParaType() == DataProperty::Para_Table)
			{
				QMap<QString, QString> map = insertValueToMap(var, val);
				QDomElement para = IoXml::getInstance()->CreateElement(doc, "Parameter", map);

				QString title = dynamic_cast<DataProperty::ParameterTable*>(base)->getTitle().join(";");
				QDomElement tabTitle = IoXml::getInstance()->CreateElement(doc, "Title", title);

				int r = dynamic_cast<DataProperty::ParameterTable*>(base)->getRowCount();
				int c = dynamic_cast<DataProperty::ParameterTable*>(base)->getColumnCount();
				QString number = QString("%1,%2").arg(r).arg(c);
				QDomElement tabNumber = IoXml::getInstance()->CreateElement(doc, "Number", number);

				para.appendChild(tabTitle);
				para.appendChild(tabNumber);

				for (int k = 0; k < r; k++)
				{
					QStringList dataList;
					for (int j = 0; j < c; j++)
					{
						QString d = QString("%1").arg(dynamic_cast<DataProperty::ParameterTable*>(base)->getValue(k, j));
						dataList.append(d);
					}
					QString d = dataList.join(",");
					QDomElement tabData = IoXml::getInstance()->CreateElement(doc, "Data", d);

					para.appendChild(tabData);
				}

				driven.appendChild(para);
			}
			else if (base->getParaType() == DataProperty::Para_Path)
			{
				var.append("Suffix");
				val.append(dynamic_cast<DataProperty::ParameterPath*>(base)->getSuffix());

				QMap<QString, QString> map = insertValueToMap(var, val);
				QDomElement para = IoXml::getInstance()->CreateElement(doc, "Parameter", map);

				QString path{};
				QString file{};
				DataProperty::PathType pathType = dynamic_cast<DataProperty::ParameterPath*>(base)->getType();
				if (pathType == DataProperty::Path) 
				{
					path = "Path";
					file = dynamic_cast<DataProperty::ParameterPath*>(base)->getPath();
				}
				else if (pathType == DataProperty::File) 
				{ 
					path = "File";
					file = dynamic_cast<DataProperty::ParameterPath*>(base)->getFile();
				}
				else if (pathType == DataProperty::FileList) 
				{ 
					path = "FileList";
					file = (dynamic_cast<DataProperty::ParameterPath*>(base)->getFileList()).join(";");
				}

				QDomElement paraType = IoXml::getInstance()->CreateElement(doc, "Type", path);
				QDomElement paraData = IoXml::getInstance()->CreateElement(doc, "Data", file);

				para.appendChild(paraType);
				para.appendChild(paraData);
				driven.appendChild(para);
			}
			else if (base->getParaType() == DataProperty::Para_Int)
			{
				var.append("Value");
				val.append(dynamic_cast<DataProperty::ParameterInt*>(base)->valueToString());

				QMap<QString, QString> map = insertValueToMap(var, val);
				QDomElement para = IoXml::getInstance()->CreateElement(doc, "Parameter", map);

				int range[2];
				dynamic_cast<DataProperty::ParameterInt*>(base)->getRange(range);
				QString ran = QString("%1,%2").arg(range[0]).arg(range[1]);
				QDomElement paraRange = IoXml::getInstance()->CreateElement(doc, "Range", ran);

				para.appendChild(paraRange);
				driven.appendChild(para);
			}
			else if (base->getParaType() == DataProperty::Para_Double)
			{
				var.append("Value");
				val.append(dynamic_cast<DataProperty::ParameterDouble*>(base)->valueToString());

				QMap<QString, QString> map = insertValueToMap(var, val);
				QDomElement para = IoXml::getInstance()->CreateElement(doc, "Parameter", map);

				double range[2];
				dynamic_cast<DataProperty::ParameterDouble*>(base)->getRange(range);
				QString ran = QString("%1,%2").arg(range[0]).arg(range[1]);
				QDomElement paraRange = IoXml::getInstance()->CreateElement(doc, "Range", ran);

				QString accu = QString("%1").arg(dynamic_cast<DataProperty::ParameterDouble*>(base)->getAccuracy());
				QDomElement paraAccuracy = IoXml::getInstance()->CreateElement(doc, "Accuracy", accu);

				para.appendChild(paraRange);
				para.appendChild(paraAccuracy);
				driven.appendChild(para);
			}
			else
			{
				var.append("Value");
				val.append(base->valueToString());

				QMap<QString, QString> map = insertValueToMap(var, val);
				QDomElement para = IoXml::getInstance()->CreateElement(doc, "Parameter", map);
				driven.appendChild(para);
			}

			
		}
	}

	void ParaManagerData::writeDrivenGroupData(QDomDocument doc, QDomElement root, ParaLinkageData* data)
	{		
		QList<DataProperty::ParameterGroup*> list = data->getDrivenGroupList();
		for (int i = 0; i < list.count(); i++)
		{
			QStringList var;
			QStringList val;
			var.append("Describe");
			var.append("Visible");

			DataProperty::ParameterGroup* group = list.at(i);

			val.append(group->getDescribe());
			val.append(dataBoolToQString(group->isVisible()));

			QMap<QString, QString> map = insertValueToMap(var, val);
			QDomElement para = IoXml::getInstance()->CreateElement(doc, "ParameterGroup", map);

			root.appendChild(para);
		}
	}

	QString ParaManagerData::getDataValue(DataProperty::ParameterBase* data)
	{
		QString value{};
		if (data->getParaType() == DataProperty::Para_Table)
		{
			QStringList valueList;

			int rows = ((DataProperty::ParameterTable*)data)->getRowCount();
			int columns = ((DataProperty::ParameterTable*)data)->getColumnCount();
			QString rc = QString("%1X%2").arg(rows).arg(columns);
			valueList.append(rc);

			QString title = (((DataProperty::ParameterTable*)data)->getTitle()).join("-");
			valueList.append(title);

			QStringList dataList;
			for (int i = 0; i < rows;i++)
			{
				for (int j = 0; j < columns;j++)
				{
					double v = ((DataProperty::ParameterTable*)data)->getValue(i, j);
					QString vs = QString("%1").arg(v);
					dataList.append(vs);
				}
			}
			QString dataValue = dataList.join("-");
			valueList.append(dataValue);

			value = valueList.join(":");
		}
		else
			value = data->valueToString();
		
		return value;
	}

	QMap<QString, QString> ParaManagerData::insertValueToMap(QStringList var, QStringList val)
	{
		QMap<QString, QString> map;

		for (int i = 0; i < var.count(); i++)
		{
			map.insert(var.at(i), val.at(i));
		}
		return map;
	}

	void ParaManagerData::writeActieParamters(QDomDocument doc, QDomElement root, ParaLinkageData* data)
	{
		QDomElement active = IoXml::getInstance()->CreateElement(doc, "Active");

		QList<DataProperty::ParameterBase*> list = data->getActiveList();

		for (int i = 0; i < list.size(); i++)
		{
			QDomElement act = doc.createElement("Parameter");
			auto base = list.at(i);

			QString dsc{};
			if ((base->getGroupName()).isEmpty())
				dsc = base->getDescribe();
			else
				dsc = base->getGroupName() + "/" + base->getDescribe();
			base->setDescribe(dsc);

			base->writeParameter(&doc, &act);
			active.appendChild(act);
		}

		root.appendChild(active);
	}

	void ParaManagerData::writeFollowParamters(QDomDocument doc, QDomElement root, ParaLinkageData* data)
	{

		QDomElement follow = IoXml::getInstance()->CreateElement(doc, "Follow");

		QList<DataProperty::ParameterBase*> list = data->getDrivenList();

		for (int i = 0; i < list.size(); i++)
		{
			QDomElement driven = doc.createElement("Parameter");
			auto base = list.at(i);

			QString dsc{};
			if ((base->getGroupName()).isEmpty())
				dsc = base->getDescribe();
			else
				dsc = base->getGroupName() + "/" + base->getDescribe();
			base->setDescribe(dsc);

			base->writeParameter(&doc, &driven);
			follow.appendChild(driven);
		}

		QList<DataProperty::ParameterGroup*> grouplist = data->getDrivenGroupList();

		for (int i = 0; i < grouplist.size(); i++)
		{
			auto base = grouplist.at(i);
			base->writeParameters(&doc, &follow);
		}

		root.appendChild(follow);
	}

	//¶ÁÈ¡xml
	void ParaManagerData::readObserverConfig()
	{
		//clearQMap();
	//	_paraLinkageDataListDict.clear();

		if (!_readConfig)
			return;

		QString destPath = getFileName();
		qDebug() << destPath;
		QFile observer(destPath);

		if (!observer.exists())
		{
			_writeConfig = true;
			return;

		}

		if (!observer.open(QIODevice::ReadOnly))
		{
			qDebug() << "read ObserverConfig file!";
			return;
		}

		QDomDocument doc;
		if (!doc.setContent(&observer))
		{
			observer.close();
			qDebug() << "read ObserverConfig file!";
			return;
		}

		QDomNodeList obslist = doc.elementsByTagName("ObserverList");
		for (int i = 0; i < obslist.size(); ++i)
		{
			QDomElement obslistele = obslist.at(i).toElement();
			QString stype = obslistele.attribute("Type");
			ProjectTreeType type = getTreeTypeByString(stype);
			if (type == UnDefined) continue;
			//			this->readBCType(&bclistele, type);
			QList<ParaLinkageData*> list = readObserverList(&obslistele, type);
			_paraLinkageDataListDict.insert(type, list);
		}

		observer.close();

		_writeConfig = true;
		_readConfig = false;
	}

	QList<ParaLinkageData*> ParaManagerData::readObserverList(QDomElement* ele, ProjectTreeType type)
	{
		QList<ParaLinkageData*> list;
		if (ele == nullptr) return list;
		QDomNodeList observerlist = ele->elementsByTagName("Observer");
		for (int i = 0; i < observerlist.size(); ++i)
		{
			ParaLinkageData* data = new ParaLinkageData;
			QDomElement obserele = observerlist.at(i).toElement();
			data->setParameterName(obserele.attribute("name"));
			readObserver(&obserele, type, data);
			list.append(data);
		}

		return list;
	}

	void ParaManagerData::readObserver(QDomElement* ele, ProjectTreeType type, ParaLinkageData* data)
	{
		if (ele == nullptr) return;

		QList<DataProperty::ParameterBase*> activeList;
		QList<DataProperty::ParameterBase*> drivenList;
		QList<DataProperty::ParameterGroup*> drivenGroupList;

		//1
		QDomNodeList activelist = ele->elementsByTagName("Active");
		if (activelist.size() != 1) return;
		QDomElement domelement = activelist.at(0).toElement();
		QDomNodeList activeParaList = domelement.elementsByTagName("Parameter");
		int npara = activeParaList.size();
		for (int i = 0; i < npara; ++i)
		{
			QDomElement paraele = activeParaList.at(i).toElement();
			QString stype = paraele.attribute("Type");
			DataProperty::ParameterBase* p = DataProperty::ParameterList::createParameterByType(stype);
			if (p == nullptr) continue;
			p->readParameter(&paraele);
			//setActiveData(&paraele, p);
			QString dsc = p->getDescribe();
			QStringList dscList = dsc.split("/");
			if (dscList.size() > 1)
			{
				p->setGroupName(dscList.at(0));
				p->setDescribe(dscList.at(1));
			}

			activeList.append(p);
		}

		//2
		QDomNodeList followlist = ele->elementsByTagName("Follow");
		if (followlist.size() != 1) return;
		domelement = followlist.at(0).toElement();

		QDomNodeList followgroupList = domelement.elementsByTagName("ParameterGroup");
		npara = followgroupList.size();
		for (int i = 0; i < npara; ++i)
		{
			QDomElement paragroupele = followgroupList.at(i).toElement();
			DataProperty::ParameterGroup* g = new DataProperty::ParameterGroup;
			g->readParameters(&paragroupele);

			drivenGroupList.append(g);
		}
		QDomElement groupParameter = followgroupList.at(0).toElement();
		QDomNodeList groupList = groupParameter.elementsByTagName("Parameter");
		npara = groupList.size();
		for (int i = 0; i < npara; ++i)
		{
			groupParameter.removeChild(groupList.at(i));
		}


		QDomNodeList followParaList = domelement.elementsByTagName("Parameter");
		npara = followParaList.size();
		for (int i = 0; i < npara; ++i)
		{
			QDomElement paraele = followParaList.at(i).toElement();
			QString stype = paraele.attribute("Type");
			DataProperty::ParameterBase* p = DataProperty::ParameterList::createParameterByType(stype);
			if (p == nullptr) continue;
			p->readParameter(&paraele);
			//setDrivenData(&paraele, p);

			QString dsc = p->getDescribe();
			QStringList dscList = dsc.split("/");
			if (dscList.size() > 1)
			{
				p->setGroupName(dscList.at(0));
				p->setDescribe(dscList.at(1));
			}

			drivenList.append(p);
			
		}


		//qDebug() << drivenList.size();
		data->setActiveList(activeList);
		data->setDrivenList(drivenList);
		data->setDrivenGroupList(drivenGroupList);
		//qDebug() << data->getDrivenList().size();
		
		

	}

	void ParaManagerData::setActiveData(QDomElement* ele, DataProperty::ParameterBase* p)
	{
		QString gpn = ele->attribute("GroupName");
		p->setGroupName(gpn);
		QString des = ele->attribute("Describe");
		QStringList deslist = des.split("/");
		if (deslist.count() > 1)
			p->setDescribe(deslist.at(1));
		else
			p->setDescribe(des);

		if (p->getParaType() == DataProperty::Para_Int)
		{
			p->setParaType(DataProperty::Para_Int);
			QString val = ele->attribute("Value");
			p->setValueFromString(val);
		}
		else if (p->getParaType() == DataProperty::Para_Double)
		{
			p->setParaType(DataProperty::Para_Double);
			QString val = ele->attribute("Value");
			p->setValueFromString(val);
		}
		else if (p->getParaType() == DataProperty::Para_String)
		{
			p->setParaType(DataProperty::Para_String);
			QString val = ele->attribute("Value");
			p->setValueFromString(val);
		}
		else if (p->getParaType() == DataProperty::Para_Selectable)
		{
			p->setParaType(DataProperty::Para_Selectable);
			QString val = ele->attribute("Value");
			//p->setValueFromString(val);
			QString ran = ele->attribute("Range");
			QStringList range = ran.split(":");
			dynamic_cast<DataProperty::ParameterSelectable*>(p)->setOption(range);
			for (int i = 0; i < range.size(); i++)
			{
				if (QString::compare(val, range.at(i)) == 0)
					dynamic_cast<DataProperty::ParameterSelectable*>(p)->setCurrentIndex(i);
			}
		}
		else if (p->getParaType() == DataProperty::Para_Path)
		{
			p->setParaType(DataProperty::Para_Path);
			QString stype = ele->attribute("Pathtype").toLower();
			if (stype == "path") dynamic_cast<DataProperty::ParameterPath*>(p)->setType(DataProperty::Path);
			if (stype == "file") dynamic_cast<DataProperty::ParameterPath*>(p)->setType(DataProperty::File);
			if (stype == "filelist") dynamic_cast<DataProperty::ParameterPath*>(p)->setType(DataProperty::FileList);

			QString val = ele->attribute("Value");
			p->setValueFromString(val);
		}
		else if (p->getParaType() == DataProperty::Para_Table)
		{
			p->setParaType(DataProperty::Para_Table);
			QString val = ele->attribute("Value");
			QStringList valuelist = val.split(":");

			QString rc = valuelist.at(0);
			QString title = valuelist.at(1);
			QString tabv = valuelist.at(2);

			QStringList rclist = rc.split("X");
			int r = rclist.at(0).toInt();
			int c = rclist.at(1).toInt();

			dynamic_cast<DataProperty::ParameterTable*>(p)->setRowCount(r);
			dynamic_cast<DataProperty::ParameterTable*>(p)->setColumnCount(c);

			QStringList titlelist = title.split("-");
			dynamic_cast<DataProperty::ParameterTable*>(p)->setTitle(titlelist);

			QStringList tabvlist = tabv.split("-");
			int v = 0;
			for (int i = 0; i < r;i++)
			{
				for (int j = 0; j < c;j++)
				{
					double d = tabvlist.at(v).toDouble();
					dynamic_cast<DataProperty::ParameterTable*>(p)->setValue(i, j, d);
					v++;
				}
			}

		}
		else if (p->getParaType() == DataProperty::Para_Bool)
		{
			p->setParaType(DataProperty::Para_Bool);
			QString val = ele->attribute("Value");
			p->setValueFromString(val);
		}

	}

	void ParaManagerData::setDrivenData(QDomElement* ele, DataProperty::ParameterBase* p)
	{
// 		QString gpn = ele->attribute("GroupName");
// 		p->setGroupName(gpn);

		QString des = ele->attribute("Describe");
		QStringList deslist = des.split("/");
		if (deslist.count() > 1)
		{
			p->setDescribe(deslist.at(1));
			p->setGroupName(deslist.at(0));
		}
		else
			p->setDescribe(des);

		QString vis = ele->attribute("Visible");
		p->setVisible(dataQStringToBool(vis));

	//	QString edi = ele->attribute("Editable");
		

		if (p->getParaType() == DataProperty::Para_Int)
		{
			p->setParaType(DataProperty::Para_Int);
			QString val = ele->attribute("Value");
			p->setValueFromString(val);

			QDomNodeList ranlist = ele->elementsByTagName("Range");
			if (ranlist.size() == 1)
			{
				QString range = ranlist.at(0).toElement().text();
				QStringList rangeList = range.split(",");
				int ran[2] = { rangeList.at(0).toInt(), rangeList.at(1).toInt() };
				dynamic_cast<DataProperty::ParameterInt*>(p)->setRange(ran);
			}
		}
		else if (p->getParaType() == DataProperty::Para_Double)
		{
			p->setParaType(DataProperty::Para_Double);
			QString val = ele->attribute("Value");
			p->setValueFromString(val);

			QDomNodeList ranlist = ele->elementsByTagName("Range");
			if (ranlist.size() == 1)
			{
				QString range = ranlist.at(0).toElement().text();
				QStringList rangeList = range.split(",");
				double ran[2] = { rangeList.at(0).toDouble(), rangeList.at(1).toDouble() };
				dynamic_cast<DataProperty::ParameterDouble*>(p)->setRange(ran);
			}

			QDomNodeList acculist = ele->elementsByTagName("Accuracy");
			if (acculist.size() == 1)
			{
				int accu = acculist.at(0).toElement().text().toInt();
				dynamic_cast<DataProperty::ParameterDouble*>(p)->setAccuracy(accu);
			}
		}
		else if (p->getParaType() == DataProperty::Para_String)
		{
			p->setParaType(DataProperty::Para_String);
			QString val = ele->attribute("Value");
			p->setValueFromString(val);
		}
		else if (p->getParaType() == DataProperty::Para_Selectable)
		{
			p->setParaType(DataProperty::Para_Selectable);
			QString ind = ele->attribute("CurrentIndex");
			dynamic_cast<DataProperty::ParameterSelectable*>(p)->setCurrentIndex(ind.toInt());

			QDomNodeList oplist = ele->elementsByTagName("Option");
			if (oplist.size() == 1)
			{
				QString option = oplist.at(0).toElement().text();
				QStringList optionlist = option.split(";");
				dynamic_cast<DataProperty::ParameterSelectable*>(p)->setOption(optionlist);
			}

		}
		else if (p->getParaType() == DataProperty::Para_Path)
		{
			p->setParaType(DataProperty::Para_Path);
			QString suffix = ele->attribute("Suffix");
			dynamic_cast<DataProperty::ParameterPath*>(p)->setSuffix(suffix);

			QDomNodeList typelist = ele->elementsByTagName("Type");
			if (typelist.size() != 1) return;
			QString stype = typelist.at(0).toElement().text().toLower();
			if (stype == "file")
			{
				dynamic_cast<DataProperty::ParameterPath*>(p)->setType(DataProperty::File);
				QString text;
				QDomNodeList textlist = ele->elementsByTagName("Text");
				if (textlist.size() == 1)
					text = textlist.at(0).toElement().text();
				textlist = ele->elementsByTagName("Data");
				if (textlist.size() == 1)
					text = textlist.at(0).toElement().text();
				if (text.isEmpty()) return;

				dynamic_cast<DataProperty::ParameterPath*>(p)->setFile(text);
			}
			else if (stype == "path" || stype == "dictionary")
			{
				dynamic_cast<DataProperty::ParameterPath*>(p)->setType(DataProperty::Path);
				QString text;
				QDomNodeList textlist = ele->elementsByTagName("Text");
				if (textlist.size() == 1)
					text = textlist.at(0).toElement().text();
				textlist = ele->elementsByTagName("Data");
				if (textlist.size() == 1)
					text = textlist.at(0).toElement().text();
				if (text.isEmpty()) return;

				dynamic_cast<DataProperty::ParameterPath*>(p)->setPath(text);
			}
			else if (stype == "filelist")
			{
				dynamic_cast<DataProperty::ParameterPath*>(p)->setType(DataProperty::FileList);
				QString text;
				QDomNodeList textlist = ele->elementsByTagName("Text");
				if (textlist.size() == 1)
					text = textlist.at(0).toElement().text();
				textlist = ele->elementsByTagName("Data");
				if (textlist.size() == 1)
					text = textlist.at(0).toElement().text();
				if (text.isEmpty()) return;

				dynamic_cast<DataProperty::ParameterPath*>(p)->setFileList(text.split(";"));
			}

			//QDomNodeList file
		}
		else if (p->getParaType() == DataProperty::Para_Table)
		{
			p->setParaType(DataProperty::Para_Table);

			QDomNodeList titlelist = ele->elementsByTagName("Title");
			if (titlelist.size() == 1)
			{
				QString t = titlelist.at(0).toElement().text();
				dynamic_cast<DataProperty::ParameterTable*>(p)->setTitle(t.split(";"));
			}

			QDomNodeList numlist = ele->elementsByTagName("Number");
			if (numlist.size() == 1)
			{
				QStringList rclist = numlist.at(0).toElement().text().split(",");
				dynamic_cast<DataProperty::ParameterTable*>(p)->setRowCount(rclist.at(0).toInt());
				dynamic_cast<DataProperty::ParameterTable*>(p)->setColumnCount(rclist.at(1).toInt());
			}

			QDomNodeList vallist = ele->elementsByTagName("Data");
			for (int i = 0; i < vallist.size(); i++)
			{
				QString val = vallist.at(i).toElement().text();
				QStringList datalist = val.split(",");
				for (int j = 0; j < datalist.size();j++)
				{
					double d = datalist.at(j).toDouble();
					dynamic_cast<DataProperty::ParameterTable*>(p)->setValue(i, j, d);
				}
			}
		}
		else if (p->getParaType() == DataProperty::Para_Bool)
		{
			p->setParaType(DataProperty::Para_Bool);
			QString val = ele->attribute("Value");
			p->setValueFromString(val);
		}
	}


	QString ParaManagerData::dataBoolToQString(bool l)
	{
		QString s;
		if (l)
			s = "true";
		else
			s = "false";

		return s;
	}

	bool ParaManagerData::dataQStringToBool(QString s)
	{
		bool b{ false };
		if (s == "true")
			b = true;

		return b;
	}

	void ParaManagerData::clearQMap()
	{
		QList<int> trees = _paraLinkageDataListDict.keys();
		for (int i = 0; i < trees.count();i++)
		{
			QList<ParaLinkageData*> datalist =_paraLinkageDataListDict.value(trees.at(i));
			for (int j = 0; j < datalist.size();j++)
			{
				ParaLinkageData* data = datalist.at(i);
				if (data == nullptr)
					continue;

				delete data;
				data = nullptr;
			}
		}

		_paraLinkageDataListDict.clear();
	}

	void ParaManagerData::setReadFlag(bool b)
	{
		_readConfig = b;
	}
}