#ifndef _GOLBALCONFIG_H_
#define _GOLBALCONFIG_H_

#include "ConfigOptionsAPI.h"
#include <QString>

namespace ConfigOption
{

	class CONFIGOPTIONSAPI GlobalConfig
	{
	public:
		GlobalConfig() = default;
		~GlobalConfig() = default;
		GlobalConfig(GlobalConfig* old);

		void setSoftName(QString name);
		QString getSoftName();
		void setChineseName(QString name);
		QString getChineseName();
		void setCorporation(QString c);
		QString getCorporation();
		void setWebsite(QString w);
		QString getWebsite();
		void setLogo(QString logo);
		QString getLogo();
		void setWelcome(QString w);
		QString getWelcome();
		void enableMaterial(bool m);
		void setVersion(QString v);
		QString getVersion();
		void setEMail(QString m);
		QString getEMail();
		bool isMaterialEnabled();
		void clearData();

		QString GetUserManual(); //Added xvdongming 2019-11-22
		void SetUserManual(QString userManual);//Added xvdongming 2019-11-22

	private:
		QString _softName{};
		QString _chineseName{};
		QString _corporation{};
		QString _website{};
		QString _logo{};
		QString _welcome{};
		QString _version{};
		QString _email{};
		QString _userManual{};//Added xvdongming 2019-11-22
		bool _material{false};
	};

}


#endif
