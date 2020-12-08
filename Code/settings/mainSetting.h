/**************************
窗口设置项 读写ini文件
libaojun
2017.08.28
****************************/

#ifndef _MainSetting_H_
#define _MainSetting_H_

#include <QString>
#include <QStringList>
#include <QVector>
#include <QIcon>
#include <QMap>

class QSettings;

namespace Setting
{
class MainSetting
{

public:
	MainSetting() = default;
	~MainSetting() = default;

	void writeINI(QSettings* seeting);
	void readINI(QSettings* seeting);

	void setWorkingDir(const QString &dir);
	QString getWorkingDir() const;
	void setLanguage(const QString &lang);
	QString getLanguage();
	//添加最新打开文件
	void appendRecentFile(QString f);
	QStringList getRencentFile();
	//插件
	void setPlugins(QStringList ps);
	QStringList getPlugins();

	void isShowUserGuidance(bool s);
	bool isShowUserGuidance();

	void setLicensePath(const QString & path);
	QString getLicensePath() const;

private:
	QString _workingDir;
	QString _language{"English"};
	QStringList _recentFiles{};
	QString _licensePath;
	QStringList _plugins{};
	bool _showUserGuidance{ true };

};

}


#endif
