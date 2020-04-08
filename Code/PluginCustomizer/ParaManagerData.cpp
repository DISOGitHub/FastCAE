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
		_paraLinkageDataListDict.clear();
	}

	ParaManagerData* ParaManagerData::getInstance()
	{
		if (_instance == nullptr)
			_instance = new ParaManagerData();

		return _instance;
	}

	void ParaManagerData::setParameterLinkageList(QString name, ParaLinkageData* data)
	{
		QList<ParaLinkageData*> list = _paraLinkageDataListDict[name]; 
		list.append(data);

		_paraLinkageDataListDict.insert(name, list);
	}

	ParaLinkageData* ParaManagerData::getParameterLinkageData(QString name, int index)
	{
		QList<ParaLinkageData*> list = _paraLinkageDataListDict[name];
		return list.at(index);
	}

	QList<ParaLinkageData*> ParaManagerData::getParameterLinkageList(QString name)
	{
		return _paraLinkageDataListDict[name];
	}

	void ParaManagerData::readObserverConfig()
	{

	}

	void ParaManagerData::writeObserverConfig()
	{
		QString path = qApp->applicationDirPath();
		QDir dir(path);
		dir.cdUp();
		QString destPath = dir.path() + "/ConfigFiles" +"/ObserverConfig.config";
		qDebug() << destPath;
		QFile observer(destPath);
		if (!observer.open(QIODevice::WriteOnly | QIODevice::Truncate))
		{
			qDebug() << "Wirte ObserverConfig file!";
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
		QMap<QString, QList<ParaLinkageData*>>::iterator iter = _paraLinkageDataListDict.begin();
		while (iter != _paraLinkageDataListDict.end())
		{
			writexml(doc, root, iter.key(), iter.value());

			iter++;
		}


		doc.appendChild(root);

		QTextStream out_stream(&observer);
		doc.save(out_stream, 4); //Ëõ½ø1¸ñ
		observer.close();
	}

	void ParaManagerData::removeParameterLinkageDataFromList(QString name, int index)
	{
		QList<ParaLinkageData*> list = _paraLinkageDataListDict[name];
		list.removeAt(index);

		_paraLinkageDataListDict.insert(name, list);
	}

	void ParaManagerData::removeParameterLinkageDataFromList(ParaLinkageData* data)
	{
		QList<ParaLinkageData*> list = _paraLinkageDataListDict[data->getParameterType()];
		list.removeOne(data);

		_paraLinkageDataListDict.insert(data->getParameterType(), list);
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
		bool b{false};
		if (s == "true")
			b = true;

		return b;
	}

	void ParaManagerData::writexml(QDomDocument doc, QDomElement root, QString name, QList<ParaLinkageData*> list)
	{
		QDomElement paraList = IoXml::getInstance()->CreateElementAttribute(doc, "ObserverList", "Type", name);
		for (int i = 0; i < list.count(); i++)
		{
			ParaLinkageData* data = list.at(i);
			if (data == nullptr)
				continue;
			
			QDomElement paraName = IoXml::getInstance()->CreateElementAttribute(doc, "Observer", "name", data->getParameterName());

			writeActiveData(doc, paraName, data);
			writeFollowData(doc, paraName, data);
			
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
			var.append("Describle");
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
			qDebug() << base->ParaTypeToString(base->getParaType());

			QString value = getDataValue(base);
			val.append(value);

			if (base->getParaType() == DataProperty::Para_Selectable)
			{
				var.append("Range");
				val.append((((DataProperty::ParameterSelectable*)base)->getOption()).join(":"));
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
		for (int i = 0; i < list.count(); i++)
		{
			QStringList var;
			QStringList val;
			var.append("Describle");
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
				QString index = QString("%1").arg(((DataProperty::ParameterSelectable*)base)->getCurrentIndex());
				val.append(index);

				QMap<QString, QString> map = insertValueToMap(var, val);
				QDomElement para = IoXml::getInstance()->CreateElement(doc, "Parameter", map);

				QString option = (((DataProperty::ParameterSelectable*)base)->getOption()).join(";");
				QDomElement SelOption = IoXml::getInstance()->CreateElement(doc, "Option", option);

				para.appendChild(SelOption);
				driven.appendChild(para);
			}
			else if (base->getParaType() == DataProperty::Para_Table)
			{
				QMap<QString, QString> map = insertValueToMap(var, val);
				QDomElement para = IoXml::getInstance()->CreateElement(doc, "Parameter", map);

				QString title = ((DataProperty::ParameterTable*)base)->getTitle().join(";");
				QDomElement tabTitle = IoXml::getInstance()->CreateElement(doc, "Title", title);

				int r = ((DataProperty::ParameterTable*)base)->getRowCount();
				int c = ((DataProperty::ParameterTable*)base)->getColumnCount();
				QString number = QString("%1,%2").arg(r).arg(c);
				QDomElement tabNumber = IoXml::getInstance()->CreateElement(doc, "Number", number);

				para.appendChild(tabTitle);
				para.appendChild(tabNumber);

				for (int k = 0; k < r; k++)
				{
					QStringList dataList;
					for (int j = 0; j < c; j++)
					{
						QString d = QString("%1").arg(((DataProperty::ParameterTable*)base)->getValue(k, j));
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
				val.append(((DataProperty::ParameterPath*)base)->getSuffix());

				QMap<QString, QString> map = insertValueToMap(var, val);
				QDomElement para = IoXml::getInstance()->CreateElement(doc, "Parameter", map);

				QString path{};
				QString file{};
				DataProperty::PathType pathType = ((DataProperty::ParameterPath*)base)->getType();
				if (pathType == DataProperty::Path) 
				{
					path = "Path";
					file = ((DataProperty::ParameterPath*)base)->getPath();
				}
				else if (pathType == DataProperty::File) 
				{ 
					path = "File";
					file = ((DataProperty::ParameterPath*)base)->getFile();
				}
				else if (pathType == DataProperty::FileList) 
				{ 
					path = "FileList";
					file = (((DataProperty::ParameterPath*)base)->getFileList()).join(";");
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
				val.append(((DataProperty::ParameterInt*)base)->valueToString());

				QMap<QString, QString> map = insertValueToMap(var, val);
				QDomElement para = IoXml::getInstance()->CreateElement(doc, "Parameter", map);

				int range[2];
				((DataProperty::ParameterInt*)base)->getRange(range);
				QString ran = QString("%1,%2").arg(range[0]).arg(range[1]);
				QDomElement paraRange = IoXml::getInstance()->CreateElement(doc, "Range", ran);

				para.appendChild(paraRange);
				driven.appendChild(para);
			}
			else if (base->getParaType() == DataProperty::Para_Double)
			{
				var.append("Value");
				val.append(((DataProperty::ParameterDouble*)base)->valueToString());

				QMap<QString, QString> map = insertValueToMap(var, val);
				QDomElement para = IoXml::getInstance()->CreateElement(doc, "Parameter", map);

				double range[2];
				((DataProperty::ParameterDouble*)base)->getRange(range);
				QString ran = QString("%1,%2").arg(range[0]).arg(range[1]);
				QDomElement paraRange = IoXml::getInstance()->CreateElement(doc, "Range", ran);

				QString accu = QString("%1").arg(((DataProperty::ParameterDouble*)base)->getAccuracy());
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
			var.append("Describle");
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
}