#include "DefineManager.h"
#include "PreDefineBase.h"
#include "Position.h"
#include "Rotation.h"
#include "Constant.h"
#include "Quantity.h"
#include "Expression.h"
#include <QDomNodeList>
#include <QDomElement>
#include <QDebug>


namespace Geant4 
{

	int DefineManager::getDefineCount()
	{
		return _defineList.size();
	}

	PreDefineBase* DefineManager::getPreDefineAt(int index)
	{
		return _defineList.at(index);
	}

	PreDefineBase* DefineManager::getPreDefineByName(QString name)
	{
		for (auto m : _defineList)
		{
			if (m->getName() == name)
				return m;
		}
		return nullptr;
	}

	void DefineManager::clear()
	{
		for (auto p : _defineList)
			delete p;
		_defineList.clear();
	}

	bool DefineManager::readGDML(QDomNodeList & nodes)
	{
	//	if (nodes.size() != 1)return false;
		QDomElement rele = nodes.at(0).toElement();

		QDomNodeList defNodes = rele.childNodes();
		const int n = defNodes.size();
		for (int i = 0; i < n; ++i)
		{
			PreDefineBase* pdef = nullptr;
			QDomElement ele = defNodes.at(i).toElement();
			QString name = ele.tagName().toLower();
			qDebug() << name;
			if (name == "position")
				pdef = new Position;
			else if (name == "rotation")
				pdef = new Rotation;
			else if (name == "constant")
				pdef = new Constant;
			else if (name == "quantity")
				pdef = new Quantity;
			else if (name == "expression")
				pdef = new Expression;
			
			if (nullptr != pdef)
			{
				_defineList.append(pdef);
				pdef->readGDML(ele);
			}
		}

		return true;

	}

	bool DefineManager::readGDML(QDomNode & node)
	{
		QDomElement rele = node.toElement();

		QDomNodeList defNodes = rele.childNodes();
		const int n = defNodes.size();
		for (int i = 0; i < n; ++i)
		{
			PreDefineBase* pdef = nullptr;
			QDomElement ele = defNodes.at(i).toElement();
			QString name = ele.tagName().toLower();
			qDebug() << name;
			if (name == "position")
				pdef = new Position;
			else if (name == "rotation")
				pdef = new Rotation;
			else if (name == "constant")
				pdef = new Constant;
			else if (name == "quantity")
				pdef = new Quantity;
			else if (name == "expression")
				pdef = new Expression;

			if (nullptr != pdef)
			{
				_defineList.append(pdef);
				pdef->readGDML(ele);
			}
		}
		return true;
	}

	void DefineManager::writeGDML(QDomDocument& doc, QDomElement& ele)
	{
		QDomElement define = doc.createElement("define");
		for (auto pdef : _defineList)
		{
			if (pdef == nullptr)continue;
			pdef->writeGDML(doc, define);
		}
		ele.appendChild(define);
	}

}