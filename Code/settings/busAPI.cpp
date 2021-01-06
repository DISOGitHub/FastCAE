#include "busAPI.h"
#include "mainSetting.h"
#include "DialogWorkingDir.h"
// #include "DialogSolver.h"
// #include "SolverManager.h"
// #include "SolveOption.h"
#include "GraphOption.h"
//#include "DialogSolveOption.h"
#include "DialogGraphOption.h"
#include "MessageSetting.h"
#include <QSettings>
#include <QCoreApplication>
#include <QDebug>

namespace Setting
{
	BusAPI* BusAPI::_ins = nullptr;

	BusAPI* BusAPI::instance()
	{
		if (_ins == nullptr)
		{
			_ins = new BusAPI;
		}
		return _ins;
	}
	BusAPI::BusAPI()
	{
		_mainSetting = new MainSetting;
// 		_solvers = new SolverManager;
// 		_solveOption = new SolveOption;
 		_graphOption = new GraphOption;
		_messageSetting = new MessageSetting;
		readINI();
	}
	BusAPI::~BusAPI()
	{
		delete _mainSetting;
// 		delete _solvers;
// 		delete _solveOption;
		delete _graphOption;
	}
	void BusAPI::setWorkingDir()
	{
		WorkingDirDialog  dlg(_mainWindow, _mainSetting);
		dlg.exec();
	}
	QString BusAPI::getWorkingDir()
	{
		return _mainSetting->getWorkingDir();
	}

	void BusAPI::writeINI()
	{
		QString path = qApp->applicationDirPath();

		QSettings settingwriter(path + "/FastCAE.ini", QSettings::IniFormat);
		_mainSetting->writeINI(&settingwriter);
//		_solveOption->writeINI(&settingwriter);
		_graphOption->writeINI(&settingwriter);
		_messageSetting->writeINI(&settingwriter);
	}
	void BusAPI::readINI()
	{
		QString path = qApp->applicationDirPath();
		
		QSettings settingReader(path + "/FastCAE.ini", QSettings::IniFormat);
		_mainSetting->readINI(&settingReader);
//		_solveOption->readINI(&settingReader);
		_graphOption->readINI(&settingReader);
		_messageSetting->readINI(&settingReader);
	}

	void BusAPI::setLanguage(const QString lang)
	{
		_mainSetting->setLanguage(lang);
	}
	QString BusAPI::getLanguage()
	{
		return _mainSetting->getLanguage();
	}
// 	SolverManager* BusAPI::getSolverManager()
// 	{
// 		return _solvers;
// 	}
// 	void BusAPI::setSolverOptions()
// 	{
// 		SolveOptionDialog dlg(_mainWindow, _solveOption);
// 		dlg.exec();
// 	}
// 	SolveOption* BusAPI::getSolveOptions()
// 	{
// 		return _solveOption;
// 	}
	QStringList BusAPI::getRencetFiles()
	{
		return _mainSetting->getRencentFile();
	}
	void BusAPI::appendRecentFile(QString f)
	{
		_mainSetting->appendRecentFile(f);
	}
	void BusAPI::setGraphOptions()
	{
		GraphOptionDialog dlg(_mainWindow, _graphOption);
		dlg.exec();
	}
	QString BusAPI::getLicensePath() const
	{
		return _mainSetting->getLicensePath();
	}

	void BusAPI::setLicensePath(const QString & licensePath)
	{
		_mainSetting->setLicensePath(licensePath);
	}

	GUI::MainWindow* BusAPI::getMainWindow()
	{
		return _mainWindow;
	}

	GraphOption* BusAPI::getGraphOption()
	{
		return _graphOption;
	}

	void BusAPI::setMainWindow(GUI::MainWindow* mw)
	{
		_mainWindow = mw;
	}
	int BusAPI::getMessageShowMode() const
	{
		return _messageSetting->getShowMode();
	}

	void BusAPI::setMessageShowMode(int showMode)
	{
		_messageSetting->setShowMode(showMode);
	}

	bool BusAPI::getMessageShowType() const
	{
		return _messageSetting->getShowType();
	}

	void BusAPI::setMessageShowType(bool showType)
	{
		_messageSetting->setShowType(showType);
	}

	bool BusAPI::getMessageShowTime() const
	{
		return _messageSetting->getShowTime();
	}

	void BusAPI::setMessageShowTime(bool showTime)
	{
		_messageSetting->setShowTime(showTime);
	}

	int BusAPI::getMessageFontPts() const
	{
		return _messageSetting->getFontPts();
	}

	void BusAPI::setMessageFontPts(int fontPts)
	{
		_messageSetting->setFontPts(fontPts);
	}

	QColor BusAPI::getMessageBkColor() const
	{
		return _messageSetting->getBkColor();
	}

	void BusAPI::setMessageBkColor(const QColor & bkColor)
	{
		_messageSetting->setBkColor(bkColor);
	}

	void BusAPI::setPlugins(QStringList p)
	{
		_mainSetting->setPlugins(p);
	}

	QStringList BusAPI::getPlugins()
	{
		return _mainSetting->getPlugins();
	}

	void BusAPI::isEnableUserGuidence(bool s)
	{
		_mainSetting->isShowUserGuidance(s);
	}

	bool BusAPI::isEnableUserGuidence()
	{
		return _mainSetting->isShowUserGuidance();
	}

}