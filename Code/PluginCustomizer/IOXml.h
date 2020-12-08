/**********************************************************************************

���ܣ�Xml�ļ��ڵ������࣬�����趨���������ƶ���ʽ�Ľڵ㲢���ء�
������xvdongming
���ڣ�2019-11
�޸ĺ�ά����xvdongming

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
		
		//����xml�ڵ� -- ��һ�����ԵĽڵ�����ֵΪbool����
		QDomElement CreateElementAttribute(QDomDocument &doc, QString name, QString attrKey, bool attrValue);
		//����xml�ڵ� -- ��һ�����ԵĽڵ�
		QDomElement CreateElementAttribute(QDomDocument &doc, QString name, QString attrKey, QString attrValue);
		//����xml�ڵ� -- ���������ԵĽڵ�
		QDomElement CreateElementAttribute(QDomDocument &doc, QString name, QString attr1Key, QString attr1Value, QString attr2Key, QString attr2Value);
		//����xml�ڵ� -- ��������ԵĽڵ�
		QDomElement CreateElement(QDomDocument &doc, QString name, QMap<QString, QString> keyAndValueList);
		
		//����xml�ڵ� -- ֻ��һ�����ƵĿսڵ�
		QDomElement CreateElement(QDomDocument &doc, QString name);
		//����xml�ڵ� -- ��һ����ֵ�Ľڵ�
		QDomElement CreateElement(QDomDocument &doc, QString name, QString value);
		
	};
}

#endif // IOXML_H
