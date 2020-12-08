#include "DataBase.h"
#include "ParameterGroup.h"
#include <QObject>
#include <QDomElement>
#include <QDomNodeList>
#include <QDebug>
#include "python/PyAgent.h"

namespace DataProperty
{
	void DataBase::setID(int id)
	{
		_id = id;
		appendProperty(QObject::tr("ID"), _id);
	}
	int DataBase::getID()
	{
		return _id;
	}
	void DataBase::setName(const QString& name)
	{
		if (name.simplified().isEmpty()) return;
		_name = name;
		appendProperty(QObject::tr("Name"), _name);
	}
	QString DataBase::getName()
	{
 		return _name;
	}
	QDomElement& DataBase::writeToProjectFile(QDomDocument* doc, QDomElement* ele)
	{
//		Q_UNUSED(doc);
		this->writeParameters(doc, ele);
		return *ele;
	}
// 	void DataBase::writeToProjectFile1(QDomDocument* doc, QDomElement* parent)
// 	{
// 
// 	}
	void DataBase::readDataFromProjectFile(QDomElement* e)
	{
//		Q_UNUSED(e);
		this->readParameters(e);
	}
	void DataBase::copy(DataBase* data)
	{
		if (data == nullptr) return;

		ParameterList::copy(data);
		PropertyList::copy(data);
		_buttons = data->getButtonList();

		const int n = data->getParameterGroupCount();
		for (int i = 0; i < n; ++i)
		{
			ParameterGroup* group = data->getParameterGroupAt(i);
			ParameterGroup* g = new ParameterGroup;
			g->copy(group);
			this->appendParameterGroup(g);
		}

		if (data->getID() != -1)
			this->setID(data->getID());
		if (data->getName() != "FFFFFF" && !data->getName().isEmpty())
			this->setName(data->getName());

//		this->generateInfo();
		
	}

	void DataBase::appendParameterGroup(ParameterGroup* g)
	{
		_paraGroupList.append(g);
		const int pcount = g->getParameterCount();
		for (int i = 0; i < pcount; ++i)
		{
			ParameterBase* pb = g->getParameterAt(i);
			pb->setDataID(_id);
			pb->setModuleType(_moduleType);
			pb->setDataIndex(_index);
		}
	}

	ParameterGroup* DataBase::getParameterGroupAt(const int index)
	{
		ParameterGroup* g = nullptr;
		if (index >=0 && index<= _paraGroupList.size())
		{
			g = _paraGroupList.at(index);
		}
		return g;
	}

	int DataBase::getParameterGroupCount()
	{
		return _paraGroupList.size();
	}

	int DataBase::getVisiableParameterGroupCount()
	{
		int c = 0;
		for (auto g : _paraGroupList)
		{
			if (g->isVisible())
				c++;
		}
		return c;
	}

	void DataBase::readParameters(QDomElement* ele)
	{
		QDomNodeList grouplist = ele->elementsByTagName("ParameterGroup");
		const int n = grouplist.size();
		for (int i = 0; i < n; ++i)
		{
			QDomElement gele = grouplist.at(i).toElement();
			QString des = gele.attribute("Describe");
			ParameterGroup* g = new ParameterGroup;
			g->setDescribe(des);
			g->readParameters(&gele);
			this->appendParameterGroup(g);
		}
		for (int i = 0; i < n; ++i)
		{
			ele->removeChild(grouplist.at(0));
		}

		ParameterList::readParameters(ele);

		QDomNodeList buttonList = ele->elementsByTagName("Button");
		const int nb = buttonList.size();
		for (int i = 0; i < nb; ++i)
		{
			QDomElement buele = buttonList.at(i).toElement();
			ButtonInfo binfo;
			binfo.text = buele.attribute("Text");
			binfo.chinese = buele.attribute("Chinese");
			binfo.command = buele.attribute("Command");
			_buttons.append(binfo);
		}
	}

	void DataBase::writeParameters(QDomDocument* doc, QDomElement* parent)
	{
		const int ngroup = _paraGroupList.size();
		for (int i = 0; i < ngroup; ++i)
		{
			ParameterGroup* g = _paraGroupList.at(i);
			g->writeParameters(doc, parent);
		}
		ParameterList::writeParameters(doc, parent);
		
		const int nbutton = _buttons.size();
		for (int i = 0; i < nbutton; ++i)
		{
			QDomElement bele = doc->createElement("Button");
			ButtonInfo info = _buttons.at(i);
			bele.setAttribute("Text", info.text);
			bele.setAttribute("Chinese", info.chinese);
			bele.setAttribute("Command", info.command);
			parent->appendChild(bele);
		}
	}

	ParameterBase* DataBase::getParameterByName(QString name)
	{
		if (name.contains("/"))
		{
			QStringList namepart = name.split("/");
			if (namepart.size() != 2) return nullptr;
			auto g = getParameterGroupByName(namepart[0]);
			if (g != nullptr)
				return g->getParameterByName(namepart[1]);
			else 
				return nullptr;
		}
		else
			return ParameterList::getParameterByName(name);
	}

	ParameterGroup* DataBase::getParameterGroupByName(QString name)
	{
		ParameterGroup* g = nullptr;
		const int n = _paraGroupList.size();
		for (int i = 0; i < n; ++i)
		{
			auto gg = _paraGroupList.at(i);
			if (gg->getDescribe() == name)
			{
				g = gg;
				break;
			}
		}
		return g;
	}
	void DataBase::dataToStream(QDataStream* datas)
	{
		*datas << _id << _name;
		int nparaGroupList = _paraGroupList.count();
		for (int i = 0; i < nparaGroupList; i++){
			_paraGroupList[i]->dataToStream(datas);
		}
		ParameterList::dataToStream(datas);
	}

	bool DataBase::isContainsButton()
	{
		bool is = false;
		if (_buttons.size() > 0)
			is = true;
		return is;
	}

	QStringList DataBase::getButtonText()
	{
		QStringList butText;
		const int n = _buttons.size();
		for (int i = 0; i < n; ++i)
		{
			ButtonInfo tep = _buttons.at(i);
			butText.append(tep.text);
		}
		return butText;
	}
	
	QStringList DataBase::getButtonChinese()
	{
		QStringList butText;
		const int n = _buttons.size();
		for (int i = 0; i < n; ++i)
		{
			ButtonInfo tep = _buttons.at(i);
			butText.append(tep.chinese);
		}
		return butText;
	}

	void DataBase::onButtonClicked(QString b)
	{
		QString command;
		const int n = _buttons.size();
		for (int i = 0; i < n; ++i)
		{
			ButtonInfo tep = _buttons.at(i);
			if (tep.text == b || tep.chinese == b)
			{
				command = tep.command;
				break;
			}
		}
		qDebug() << command;
		Py::PythonAagent::getInstance()->submit(command);
	}

	QList<ButtonInfo> DataBase::getButtonList()
	{
		return _buttons;
	}
	void DataBase::appendParameter(ParameterBase* p)
	{
		p->setModuleType(_moduleType);
		p->setDataID(_id);
		p->setDataIndex(_index);

		ParameterList::appendParameter(p);
	}
	ParameterBase* DataBase::appendParameter(ParaType type)
	{
		ParameterBase* p = ParameterList::appendParameter(type);
		p->setModuleType(_moduleType);
		p->setDataID(_id);
		p->setDataIndex(_index);
		return p;
	}

	void DataBase::removeParameter(ParameterBase* p)
	{
		for (auto g : _paraGroupList)
		{
			g->removeParameter(p);
		}
		ParameterList::removeParameter(p);
	}

	void DataBase::generateParaInfo()
	{
		if (_moduleType == Module_None) return;

		const int nGroup = _paraGroupList.size();
		for (int j = 0; j < nGroup; ++j)
		{
			ParameterGroup* g = _paraGroupList.at(j);
			const int pcount = g->getParameterCount();
			for (int i = 0; i < pcount; ++i)
			{
				ParameterBase* pb = g->getParameterAt(i);
				pb->setDataID(_id);
				pb->setModuleType(_moduleType);
				pb->setDataIndex(_index);
			}
		}

		const int pcount = this->getParameterCount();
		for (int i = 0; i < pcount; ++i)
		{
			ParameterBase* pb = this->getParameterAt(i);
			pb->setDataID(_id);
			pb->setModuleType(_moduleType);
			pb->setDataIndex(_index);
		}
	}

	void DataBase::setModuleType(ModuleType t)
	{
		_moduleType = t;
	}
	
	ModuleType DataBase::getModuleType()
	{
		return _moduleType;
	}

	void DataBase::setIndex(int index)
	{
		_index = index;
	}

	int DataBase::getIndex()
	{
		return _index;
	}

	void DataBase::removeParameterGroup(ParameterGroup* g)
	{
		if (_paraGroupList.contains(g))
		{
			_paraGroupList.removeOne(g);
			delete g;
		}
	}

	void DataBase::removeParameterGroupAt(int i)
	{
		if (i < 0 || i >= _paraGroupList.size()) return;
		ParameterGroup* g = _paraGroupList.at(i);
		this->removeParameterGroup(g);
	}

}