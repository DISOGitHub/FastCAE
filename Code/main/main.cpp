#include <QApplication>
#include <QtCore>
#include <QtGui>
#include <QDesktopWidget>
#include <QString>
#include <QSplashScreen>
#include <QFile>
#include <QObject>
#include "mainWindow/mainWindow.h"
#include "XBeautyUI.h"
#include <QMessageBox>
#include <QCoreApplication>
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/ConfigDataReader.h"
#include "ConfigOptions/GlobalConfig.h"
#include "settings/busAPI.h"
#include "python/PyAgent.h"

#ifdef Q_OS_WIN
#include "License/confirmation.h"
#ifdef _DEBUG
#include <crtdbg.h>
#endif
#endif

#define CHECKINGMEMORY 0



int main(int argc, char* argv[])
{
	QString releaseVersion = "2.1.2049";

	{
		bool nogui = false;
		bool design = false;
		bool script = false;
		QString scriptFile;
		for (int i = 1; i < argc; ++i)
		{
			QString arguement = QString(argv[i]).toLower();
			if (arguement.contains("-nogui")) nogui = true;
//			if (arguement.contains("-design")) design = true;
			if (arguement.contains("-i"))
			{
				script = true;
				char* c = argv[++i];
				std::string sf = c;
				scriptFile = QString::fromLocal8Bit(sf.c_str());
			}

			qDebug() << "arguements  " << arguement;
		}

		QApplication app(argc, argv);

//		Setting::BusAPI::instance()->setDesignModel(design);

		GUI::MainWindow mainwindow;

		/******************************启动单一实例************************************/
		if (design)
		{
#ifdef Q_OS_WIN
			HANDLE hMutex = ::CreateMutexA(0, true, "FastCAE");
			if (hMutex)
			{
				if (GetLastError() == ERROR_ALREADY_EXISTS)
				{
					QMessageBox::warning(NULL, "Warning", "Already have an instance !");
					return 0;
				}
			}
#endif
		}

		/******************************************************************************/
		QString hello = ConfigOption::ConfigOption::getInstance()->getGlobalConfig()->getWelcome();
		XBeautyUI::instance()->setWelcomePictureRemainTime(1500);
		XBeautyUI::instance()->setQssFilePath(":/Beauty/QUI/beauty/qianfan.qss");
		XBeautyUI::instance()->autoSetStyle();

		QString helloPicName = qApp->applicationDirPath() + "/../ConfigFiles/Icon/" + hello;
		int remineTime = XBeautyUI::instance()->welcomePictureRemainTime();
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

		if (!nogui)
		{
			//**************欢迎界面********************
#ifndef _DEBUG
			QSplashScreen* splash = new QSplashScreen;
			QFile helloPic(helloPicName);
			if (helloPic.exists())
			{
				splash->setPixmap(helloPicName);
				splash->setDisabled(true);
				splash->show();
				QElapsedTimer t;
				t.start();
				while (t.elapsed() < remineTime) QCoreApplication::processEvents();
				splash->finish(&mainwindow);
				delete splash;
		}
#endif
			//*****************************************
			mainwindow.show();
			mainwindow.showMaximized();
			emit mainwindow.sendInfoToStatesBar(QString("Version: %1").arg(releaseVersion));
	}
// 		if (design)
// 			mainwindow.changeToDesignModel();
		if (script)
			Py::PythonAagent::getInstance()->execScript(scriptFile);

		if (nogui) return 0;

		app.exec();
	}


#ifdef Q_OS_WIN
#ifdef _DEBUG

	if (CHECKINGMEMORY)
	{
		printf("check memory leak ...\n");
		_CrtDumpMemoryLeaks();
		printf("check over.\n");
	}
	
#endif
#endif

	return 0;

	
}