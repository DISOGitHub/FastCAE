#include "ObserverConfigReader.h"
#include "ConfigOptions.h"
#include "ObserverConfig.h"
#include "ParameterObserver.h"
#include "DataProperty/ParameterBase.h"
#include "DataProperty/ParameterList.h"
#include "DataProperty/ParameterGroup.h"
#include <QDomDocument>
#include <QDomElement>
#include <QDomAttr>
#include <QFile>
#include <QDebug>

namespace ConfigOption
{
	ObserverConfigReader::ObserverConfigReader(QString f,ObserverConfig* oc)
	{
		_filename = f;
		_observerConfig = oc;
	}

	bool ObserverConfigReader::read()
	{
		QFile file(_filename);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
		QDomDocument doc;
		if (!doc.setContent(&file))
		{
			file.close();
			return false;
		}
		QDomNodeList obslist = doc.elementsByTagName("ObserverList");
		for (int i = 0; i < obslist.size(); ++i)
		{
			QDomElement obslistele = obslist.at(i).toElement();
			QString stype = obslistele.attribute("Type");
			ProjectTreeType type = getTreeTypeByString(stype);
			if (type == UnDefined) continue;
//			this->readBCType(&bclistele, type);
			this->readObserverList(&obslistele, type);
		}
		file.close();
		return true;
	}

	void ObserverConfigReader::readObserverList(QDomElement* ele, ProjectTreeType type)
	{
		if (ele == nullptr) return;
		QDomNodeList observerlist = ele->elementsByTagName("Observer");
		for (int i = 0; i < observerlist.size(); ++i)
		{
			QDomElement obserele = observerlist.at(i).toElement();
			this->readObserver(&obserele, type);
		}
	}

	void ObserverConfigReader::readObserver(QDomElement* ele, ProjectTreeType type)
	{
		if (ele == nullptr) return;
		ParameterObserver* obs = new ParameterObserver;
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
			obs->appendCofigActive(p->getDescribe(), p);
		}
		QDomNodeList followlist = ele->elementsByTagName("Follow");
		if (followlist.size() != 1) return;
		domelement = followlist.at(0).toElement();
		QDomNodeList followgroupList = domelement.elementsByTagName("ParameterGroup");
		npara = followgroupList.size();

		for (int j = 0; j < npara; j++)
		{
			QDomElement paragroupele = followgroupList.at(j).toElement();
			DataProperty::ParameterGroup* g = new DataProperty::ParameterGroup;
			g->readParameters(&paragroupele);
			obs->appendConfigFollowGroup(g->getDescribe(), g);
//			domelement.removeChild(paragroupele);
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
			obs->appendConfigFollow(p->getDescribe(), p);
		}
	

		_observerConfig->appendObserver(type, obs);

	}

}
