/**********************************************************************************

���ܣ�Xml�ļ��ڵ������࣬�����趨���������ƶ���ʽ�Ľڵ㲢���ء�
������xvdongming
���ڣ�2019-11
�޸ĺ�ά����xvdongming

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

	//����
	IoXml* IoXml::getInstance()
	{
		if (_instance == nullptr)
			_instance = new IoXml;
		return _instance;
	}


	/*************************************************************************************

	���ܣ��������������ԵĽڵ�
	������1��doc        ��xml�ļ��ĵ�ָ��
	      2��name       ���ڵ�����
		  3��attr1Key   ����1�����Եļ�ֵ
		  4��attr1Value ����1�����Ե���ֵ
		  5��attr2Key   ����2�����Եļ�ֵ
		  6��attr3Value ����2�����Ե���ֵ
	����ֵ��xml�ڵ�
	���ڣ�2019-11-15
	�༭��xvdongming
	��ע��                  <SoftName name="fastcae666" Chinese="����"/>

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

	���ܣ�������һ�����ԵĽڵ�
	������
		1��doc        ��xml�ļ��ĵ�ָ��
		2��name       ���ڵ�����
		3��attr1Key   ����1�����Եļ�ֵ
		4��attr1Value ����1�����Ե���ֵ
	����ֵ��xml�ڵ�
	���ڣ�2019-11-15
	�༭��xvdongming
	��ע��                           <CreateGeo Enable="true"/>

	**************************************************************************************/
	QDomElement IoXml::CreateElementAttribute(QDomDocument &doc, QString name, QString attrKey, QString attrValue)
	{
		QDomElement element = doc.createElement(name);
		element.setAttribute(attrKey, attrValue);
		return element;
	}

	/*************************************************************************************

	���ܣ�������һ�����ԵĽڵ�
	������
	1��doc        ��xml�ļ��ĵ�ָ��
	2��name       ���ڵ�����
	3��value      ����ֵ
	����ֵ��xml�ڵ�
	���ڣ�2019-11-15
	�༭��xvdongming
	��ע��            	<Corporation>ǧ���Ƽ�2222</Corporation>
	**************************************************************************************/
	QDomElement IoXml::CreateElement(QDomDocument &doc, QString name, QString value)
	{
		QDomElement element = doc.createElement(name);
		QDomText text = doc.createTextNode(value);
		element.appendChild(text);
		return element;
	}

	/*************************************************************************************

	���ܣ�������һ�����ԵĽڵ�
	������
	1��doc        ��xml�ļ��ĵ�ָ��
	2��name       ���ڵ�����
	����ֵ��xml�ڵ�
	���ڣ�2019-11-15
	�༭��xvdongming
	��ע��            	<Corporation>ǧ���Ƽ�2222</Corporation>
	**************************************************************************************/
	QDomElement IoXml::CreateElement(QDomDocument &doc, QString name)
	{
		QDomElement element = doc.createElement(name);
		return element;
	}

	/*************************************************************************************

	���ܣ�������������ԵĽڵ�
	������
	1��doc             ��xml�ļ��ĵ�ָ��
	2��name            ���ڵ�����
	3��keyAndValueList �������б�
	����ֵ��xml�ڵ�
	���ڣ�2019-11-15
	�༭��xvdongming
	��ע��            	<SoftName a1="a1" a2="a2" a3=""a3/>
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

	���ܣ�������һ�����ԵĽڵ�
	������
	1��doc        ��xml�ļ��ĵ�ָ��
	2��name       ���ڵ�����
	3��attr1Key   ����1�����Եļ�ֵ
	4��attr1Value ����1�����Ե���ֵ��bool����
	����ֵ��xml�ڵ�
	���ڣ�2019-11-21
	�༭��xvxinwei
                   
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