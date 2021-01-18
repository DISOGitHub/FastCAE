#include "TreeConfigReader.h"
#include "ProjectTreeConfig.h"
#include "ConfigOptions/ConfigOptions.h"
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include "ProjectTreeInfo.h"
#include "TreeItemData.h"
#include "DataProperty/modelTreeItemType.h"
#include <QDebug>

namespace ConfigOption
{
	TreeConfigReader::TreeConfigReader(const QString &fileName, ProjectTreeConfig* treeconfig) /*: IOBase(fileName)*/
	{
//		_info = ConfigOption::getInstance()->getProjectTreeConfig();
		_info = treeconfig;
		_file.setFileName(fileName);
	}
	TreeConfigReader::~TreeConfigReader()
	{
		if (_doc != nullptr)
		{
			delete _doc;
		}
	}
	bool TreeConfigReader::read()
	{
		if (!_file.open(QFile::ReadOnly | QFile::Text)) return false;
		_doc = new QDomDocument();
		if (!_doc->setContent(&_file))
		{
			_file.close();
			return false;
		}
		
		QDomNodeList treelist = _doc->elementsByTagName("Tree");
//		qDebug() << treelist.size();
		for (int i = 0; i < treelist.count(); ++i)
		{
			QDomElement ele = treelist.at(i).toElement();
			readTree(&ele);
		}
		return true;
	}
	void TreeConfigReader::readTree(QDomElement* element)
	{
		if (element == nullptr) return;
		QString typestring = element->attribute("Type");
		ProjectTreeType type = getTreeTypeByString(typestring);
		QString name = element->attribute("Name");
//		QString outFile = element->attribute("OutputFile");
		QString chinese = element->attribute("Chinese");
//		qDebug() << outFile;
		ProjectTreeInfo* pt = new ProjectTreeInfo();
		pt->setName(name);
		pt->setType(type);
		pt->setChinese(chinese);
//		pt->setOutputFileName(outFile);
		QDomNodeList disnodelist = element->elementsByTagName("Disable");
		QStringList disitems;
		for (int i = 0; i < disnodelist.size();++i)
		{
			QDomElement e = disnodelist.at(i).toElement();
			QString s = e.text();
			disitems.append(s.split(";"));
		}
		qDebug() << disitems;
		pt->setDisableItems(disitems);
		QDomNodeList children = element->elementsByTagName("ChildNode");
		for (int i = 0; i < children.size(); ++i)
		{
			QDomElement ele = children.at(i).toElement();
			readChildNode(pt, &ele);
		}
		_info->appendTree(type, pt);
	}
	void TreeConfigReader::readChildNode(ProjectTreeInfo* tree, QDomElement* ele)
	{
		if (ele == nullptr) return;
		TreeItem* item = new TreeItem;
		QString text = ele->attribute("Text");
		QString type = ele->attribute("Type");
		QString icon = ele->attribute("Icon");
		QString chinese = ele->attribute("Chinese");
		QString sid = ele->attribute("DataID");
		const int id = sid.toInt();

		item->setText(text);
		item->setType(type);
		item->setIcon(icon);
		item->setChinese(chinese);
		item->setDataID(id);

		QDomNodeList nodelist = ele->elementsByTagName("Parent");
		if (nodelist.size() > 0)
		{
			QString p = nodelist.at(0).toElement().text();
			item->SetParent(p);
		}
		nodelist = ele->elementsByTagName("ContextMenu");
		const int n = nodelist.size();
		for (int i = 0; i < n; ++i)
		{
			QDomElement conele = nodelist.at(i).toElement();
			QDomNodeList conList = conele.elementsByTagName("Menu");
			for (int i = 0; i < conList.size(); ++i)
			{
				QDomElement menuele = conList.at(i).toElement();
				QString text = menuele.text();
				qDebug() <<item<< text;
				item->appendContextMenu(text);
			}
		}
		tree->appendItem(item);
	}

}