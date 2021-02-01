/**********************************************************************************

功能：Xml文件节点生成类，根据设定参数生成制定格式的节点并返回。
建立：xvdongming
日期：2019-11
修改和维护：xvdongming

**********************************************************************************/

#ifndef IOXML_H
#define IOXML_H

#include <QObject>
#include <QDomElement>
#include <QMap>

namespace FastCAEDesigner
{
	class IoXml : public QObject
	{
		Q_OBJECT
	private:
		IoXml(QObject *parent = 0);

	public:
		~IoXml();

	public:
		static IoXml* getInstance();

	private:
		static IoXml* _instance;

	public:
		
		//建立xml节点 -- 带一个属性的节点属性值为bool类型
		QDomElement CreateElementAttribute(QDomDocument &doc, QString name, QString attrKey, bool attrValue);
		//建立xml节点 -- 带一个属性的节点
		QDomElement CreateElementAttribute(QDomDocument &doc, QString name, QString attrKey, QString attrValue);
		//建立xml节点 -- 带两个属性的节点
		QDomElement CreateElementAttribute(QDomDocument &doc, QString name, QString attr1Key, QString attr1Value, QString attr2Key, QString attr2Value);
		//建立xml节点 -- 带多个属性的节点
		QDomElement CreateElement(QDomDocument &doc, QString name, QMap<QString, QString> keyAndValueList);
		
		//建立xml节点 -- 只有一个名称的空节点
		QDomElement CreateElement(QDomDocument &doc, QString name);
		//建立xml节点 -- 带一个数值的节点
		QDomElement CreateElement(QDomDocument &doc, QString name, QString value);
		
	};
}

#endif // IOXML_H
