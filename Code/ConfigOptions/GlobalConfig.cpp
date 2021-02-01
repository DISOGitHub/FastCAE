#include "GlobalConfig.h"

namespace ConfigOption
{
	GlobalConfig::GlobalConfig(GlobalConfig* old)
	{
		_softName = old->getSoftName();
		_chineseName = old->getChineseName();
		_corporation = old->getCorporation();
		_website = old->getWebsite();
		_logo = old->getLogo();
		_welcome = old->getWelcome();
		_version = old->getVersion();
		_email = old->getEMail();
		_material = old->isMaterialEnabled();
		_userManual = old->GetUserManual();//Added xvdongming
	}

	void GlobalConfig::setSoftName(QString name)
	{
		_softName = name;
	}
	QString GlobalConfig::getSoftName()
	{
		return _softName;
	}
	void GlobalConfig::setChineseName(QString n)
	{
		_chineseName = n;
	}
	QString GlobalConfig::getChineseName()
	{
		return _chineseName;
	}
	void GlobalConfig::setCorporation(QString c)
	{
		_corporation = c;
	}
	QString GlobalConfig::getCorporation()
	{
		return _corporation;
	}
	void GlobalConfig::setWebsite(QString c)
	{
		_website = c;
	}
	QString GlobalConfig::getWebsite()
	{
		return _website;
	}
	void GlobalConfig::setLogo(QString logo)
	{
		_logo = logo;
	}
	QString GlobalConfig::getLogo()
	{
		return _logo;
	}
	void GlobalConfig::setWelcome(QString w)
	{
		_welcome = w;
	}
	QString GlobalConfig::getWelcome()
	{
		return _welcome;
	}
	void GlobalConfig::setVersion(QString v)
	{
		_version = v;
	}
	QString GlobalConfig::getVersion()
	{
		return _version;
	}
	void GlobalConfig::setEMail(QString m)
	{
		_email = m;
	}
	QString GlobalConfig::getEMail()
	{
		return _email;
	}
	void GlobalConfig::enableMaterial(bool m)
	{
		_material = m;
	}
	bool GlobalConfig::isMaterialEnabled()
	{
		return _material;
	}

	QString GlobalConfig::GetUserManual()
	{
		return _userManual;
	}

	void GlobalConfig::SetUserManual(QString userManual)
	{
		_userManual = userManual;
	}

	// 	void GlobalConfig::setUseRibbon(const QString& str)
	// 	{
	// 		_useRibbon = str;
	// 	}

	// 	const QString GlobalConfig::getUseRibbon() const
	// 	{
	// 		return _useRibbon; 
	// 	}

	void GlobalConfig::clearData()
	{
		_softName.clear();
		_chineseName.clear();
		_corporation.clear();
		_website.clear();
		_logo.clear();
		_welcome.clear();
		_version.clear();
		_email.clear();
		_userManual.clear();//Added xvdongming
		_material = false;
	}

}
