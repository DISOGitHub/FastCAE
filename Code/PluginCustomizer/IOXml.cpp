/**********************************************************************************

功能：Xml文件节点生成类，根据设定参数生成制定格式的节点并返回。
建立：xvdongming
日期：2019-11
修改和维护：xvdongming

**********************************************************************************/

#include "IOXml.h"
#include <QDomElement>
#include <QDebug>

namespace FastCAEDesigner
{

	IoXml* IoXml::_instance = nullptr;

	IoXml::IoXml(QObject *parent) : QObject(parent)
	{
	}

	IoXml::~IoXml()
	{
	}

	//单例
	IoXml* IoXml::getInstance()
	{
		if (_instance == nullptr)
			_instance = new IoXml;
		return _instance;
	}


	/*************************************************************************************

	功能：建立带两个属性的节点
	参数：1、doc        ：xml文件文档指针
	      2、name       ：节点名称
		  3、attr1Key   ：第1个属性的键值
		  4、attr1Value ：第1个属性的数值
		  5、attr2Key   ：第2个属性的键值
		  6、attr3Value ：第2个属性的数值
	返回值：xml节点
	日期：2019-11-15
	编辑：xvdongming
	备注：                  <SoftName name="fastcae666" Chinese="工程"/>

	**************************************************************************************/
	QDomElement IoXml::CreateElementAttribute(QDomDocument &doc, QString name, QString attr1Key, QString attr1Value, QString attr2Key, QString attr2Value)
	{
		QDomElement element = doc.createElement(name);
		element.setAttribute(attr1Key, attr1Value);
		QDomAttr dom = doc.createAttribute(attr2Key);
		dom.setValue(attr2Value);
		element.setAttributeNode(dom);
		return element;
	}

	/*************************************************************************************

	功能：建立带一个属性的节点
	参数：
		1、doc        ：xml文件文档指针
		2、name       ：节点名称
		3、attr1Key   ：第1个属性的键值
		4、attr1Value ：第1个属性的数值
	返回值：xml节点
	日期：2019-11-15
	编辑：xvdongming
	备注：                           <CreateGeo Enable="true"/>

	**************************************************************************************/
	QDomElement IoXml::CreateElementAttribute(QDomDocument &doc, QString name, QString attrKey, QString attrValue)
	{
		QDomElement element = doc.createElement(name);
		element.setAttribute(attrKey, attrValue);
		return element;
	}

	/*************************************************************************************

	功能：建立带一个属性的节点
	参数：
	1、doc        ：xml文件文档指针
	2、name       ：节点名称
	3、value      ：数值
	返回值：xml节点
	日期：2019-11-15
	编辑：xvdongming
	备注：            	<Corporation>千帆科技2222</Corporation>
	**************************************************************************************/
	QDomElement IoXml::CreateElement(QDomDocument &doc, QString name, QString value)
	{
		QDomElement element = doc.createElement(name);
		QDomText text = doc.createTextNode(value);
		element.appendChild(text);
		return element;
	}

	/*************************************************************************************

	功能：建立带一个属性的节点
	参数：
	1、doc        ：xml文件文档指针
	2、name       ：节点名称
	返回值：xml节点
	日期：2019-11-15
	编辑：xvdongming
	备注：            	<Corporation>千帆科技2222</Corporation>
	**************************************************************************************/
	QDomElement IoXml::CreateElement(QDomDocument &doc, QString name)
	{
		QDomElement element = doc.createElement(name);
		return element;
	}

	/*************************************************************************************

	功能：建立带多个属性的节点
	参数：
	1、doc             ：xml文件文档指针
	2、name            ：节点名称
	3、keyAndValueList ：属性列表
	返回值：xml节点
	日期：2019-11-15
	编辑：xvdongming
	备注：            	<SoftName a1="a1" a2="a2" a3=""a3/>
	**************************************************************************************/
	QDomElement IoXml::CreateElement(QDomDocument &doc, QString name, QMap<QString, QString> keyAndValueList)
	{
		QDomElement element = doc.createElement(name);
		QMap<QString, QString>::iterator it;

		for (it = keyAndValueList.begin(); it != keyAndValueList.end(); ++it)
		{
			QString keyName = it.key();
			QString keyValue = it.value();
			qDebug() << keyName << keyValue;
			QDomAttr node = doc.createAttribute(keyName);
			node.setValue(keyValue);
			element.setAttributeNode(node);
		}

		return element;
	}

	/*************************************************************************************

	功能：建立带一个属性的节点
	参数：
	1、doc        ：xml文件文档指针
	2、name       ：节点名称
	3、attr1Key   ：第1个属性的键值
	4、attr1Value ：第1个属性的数值是bool类型
	返回值：xml节点
	日期：2019-11-21
	编辑：xvxinwei
                   
	**************************************************************************************/
	QDomElement IoXml::CreateElementAttribute(QDomDocument &doc, QString name, QString attrKey, bool attrValue)
	{
		QString str_attrValue;
		if (attrValue == true)
		{
			str_attrValue = "true";
		}
		else
		{
			str_attrValue = "false";
		}
		QDomElement element = doc.createElement(name);
		element.setAttribute(attrKey, str_attrValue);
		return element;
	}

}