#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QProcess>
#include "mainWindow/mainWindow.h"
#include "XBeautyUI.h"
#include "CommandLine.h"
#include "ConfigOptions/ConfigDataReader.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/GlobalConfig.h"
#include "settings/busAPI.h"

#ifdef Q_OS_WIN
#include "License/confirmation.h"
#ifdef _DEBUG
#include <crtdbg.h>
#endif
#endif

#define CHECKINGMEMORY 0

#define FASTCAE_VERSION "2.5.0"

int main(int argc, char* argv[])
{
	CommandPara para(argc, argv);
	if (para.isHelp()) return 1;

	QApplication app(argc, argv);

// 	QString path = qApp->applicationDirPath();
// 	ConfigOption::ConfigDataReader reader(path + "/../ConfigFiles/", ConfigOption::ConfigOption::getInstance());
// 	reader.read();
// 	QString qUseRibbon = ConfigOption::ConfigOption::getInstance()->getGlobalConfig()->getUseRibbon();
// 	bool bUseRibbon = qUseRibbon == "yes" ? true : false;

	bool isRibbon = Setting::BusAPI::instance()->isUseRibbon();

	GUI::MainWindow mainwindow(isRibbon);

	/******************************************************************************/
	XBeautyUI::instance()->setQssFilePath(":/Beauty/QUI/beauty/qianfan.qss");
	XBeautyUI::instance()->autoSetStyle();
	QString qssFileName = XBeautyUI::instance()->qssFilePath();

	//**************加载qss****************** 
	QFile qssFile(qssFileName);
	if (qssFile.exists())
	{
		qssFile.open(QIODevice::ReadOnly);
		QString style = qssFile.readAll();
		qApp->setStyleSheet(style);
		qssFile.close();
	}
	//*****************************************

	//***************正版验证******************
#ifdef Q_OS_WIN
	// 	Confirmation confirm;
	// 	confirm.setParent(&mainwindow);
	// 	if (!confirm.licenseCheck())
	// 	{
	// 		return 0;
	// 	}
#endif
	//*****************************************

	if (para.exec(&mainwindow))
		emit mainwindow.sendInfoToStatesBar(QString("Version: %1").arg(FASTCAE_VERSION));
	else return 1;

	int e = app.exec();

#ifdef Q_OS_WIN
#ifdef _DEBUG

	// 	if (CHECKINGMEMORY)
	// 	{
	// 		printf("check memory leak ...\n");
	// 		_CrtDumpMemoryLeaks();
	// 		printf("check over.\n");
	// 	}
	// 	
#endif
#endif

	if (e == -1000)
	{
		QProcess::startDetached(qApp->applicationFilePath(), QStringList());
		return 0;
	}

	return e;


}
