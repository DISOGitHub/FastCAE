#include "CustomParameterModel.h"
#include "EditorDescripttionSetup.h"
#include <QProcess>
#include <QCoreApplication>
#include <QDebug>

#pragma execution_character_set("utf-8")


namespace FastCAEDesigner
{
	CustomParameterModel::CustomParameterModel(QString nameEng, QString nameChn, QString iconName, int type, QObject *parent)
		:ModelBase(nameEng, nameChn, iconName, type, parent)
	{

	}

	CustomParameterModel::~CustomParameterModel()
	{

	}

	int CustomParameterModel::ShowEditor(QTreeWidgetItem* treeItem, QWidget* parent)
	{
// 		QString designer = qApp->applicationDirPath()+"/../bin/designer.exe";
// 		QProcess *process = new QProcess(parent);
// 		process->start(designer);
// 		if (!process->waitForStarted())
// 			return -1;
// 		process->waitForFinished();
// 
// 		qDebug() << process->readAll();
// 		qDebug() << process->exitCode();//ÍË³öÂë
// 		qDebug() << process->exitStatus();//ÍË³ö×´Ì¬

		FastCAEDesigner::EditorDescripttionSetup dlg(treeItem, this, parent);
		return dlg.exec();
	}

	
}