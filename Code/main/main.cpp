#include <QApplication>
#include <QFile>
#include "mainWindow/mainWindow.h"
#include "XBeautyUI.h"
#include "CommandLine.h"

#ifdef Q_OS_WIN
#include "License/confirmation.h"
#ifdef _DEBUG
#include <crtdbg.h>
#endif
#endif

#define CHECKINGMEMORY 0

#define FASTCAE_VERSION "2.5beta"

int main(int argc, char* argv[])
{
	CommandPara para(argc, argv);
	if (para.isHelp()) return 1;

	QApplication app(argc, argv);

	GUI::MainWindow mainwindow;
	

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

	app.exec();

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

	return 1;

	
}
