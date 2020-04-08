#include "mainSetting.h"
#include <QSettings>
#include <QDir>
#include <QCoreApplication>

namespace Setting
{
	void MainSetting::writeINI(QSettings* setting)
	{
		setting->setValue("/Path/WorkingDir", _workingDir);
		setting->setValue("/Language/Language", _language);
		setting->setValue("/Recent/RecentFile", _recentFiles);
		setting->setValue("/License/Path", _licensePath);
		setting->setValue("/Plugins/Name", _plugins);
		setting->setValue("/User/GUidance", _showUserGuidance);
	}
	void MainSetting::readINI(QSettings* setting)
	{
		_workingDir = setting->value("/Path/WorkingDir").toString();
		_language = setting->value("/Language/Language").toString();
		_recentFiles = setting->value("/Recent/RecentFile").toStringList();
		_licensePath = setting->value("/License/Path").toString();
		_plugins = setting->value("/Plugins/Name").toStringList();
		_showUserGuidance = setting->value("/User/GUidance").toBool();

		QDir dir(_workingDir);

		if (!dir.exists())
		{
			QString path = qApp->applicationDirPath() + "/../WorkingDir/";
			QDir dir(path);
			if (!dir.exists())
			{
				dir.mkpath(path);
			}
			_workingDir = path;
		}

		
	}
	void MainSetting::setWorkingDir(const QString &dir)
	{
		QDir d(dir);
		if (!d.exists(dir))
		{
			QString path = qApp->applicationDirPath() + "/../WorkingDir/";
			QDir dirs(path);
			if (!dirs.exists())
			{
				dirs.mkpath(path);
			}
			_workingDir = path;
		}
		else
			_workingDir = dir;
	}
	QString MainSetting::getWorkingDir() const
	{
		return _workingDir;
	}
	void MainSetting::setLanguage(const QString &lang)
	{
		_language = lang;
	}
	QString MainSetting::getLanguage()
	{
		return _language;
	}
	void MainSetting::appendRecentFile(QString f)
	{
		if (_recentFiles.contains(f))
		{
			_recentFiles.removeOne(f);
			_recentFiles.insert(0, f);
		}
		else
		{
			_recentFiles.insert(0, f);
		}
		while (_recentFiles.size() > 5)
		{
			_recentFiles.removeLast();
		}
	}
	QStringList MainSetting::getRencentFile()
	{
		return _recentFiles;
	}

	void MainSetting::isShowUserGuidance(bool s)
	{
		_showUserGuidance = s;
	}

	bool MainSetting::isShowUserGuidance()
	{
		return _showUserGuidance;
	}

	void MainSetting::setLicensePath(const QString & path)
	{
		_licensePath = path;
	}

	QString MainSetting::getLicensePath() const
	{
		return _licensePath;
	}

	void MainSetting::setPlugins(QStringList ps)
	{
		_plugins = ps;
	}

	QStringList MainSetting::getPlugins()
	{
		return _plugins;
	}

}