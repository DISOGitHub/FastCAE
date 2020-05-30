#include "MesherControlerBase.h"
#include "mainWindow/mainWindow.h"
#include <QString>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/SolverConfig.h"
#include "ConfigOptions/MesherInfo.h"
#include "IO/TemplateReplacer.h"
#include "python/PyAgent.h"
#include <QDebug>

namespace SolverControl
{

	MesherControlerBase::MesherControlerBase(GUI::MainWindow* mw, QString mesher)
		: ModuleBase::ProcessBar(mw, tr("Meshing..."))
	{
		_mesher = ConfigOption::ConfigOption::getInstance()->getSolverOption()->getMesherByName(mesher);
		connect(this, SIGNAL(addProcessBar(QWidget*)), mw, SIGNAL(addProcessBarSig(QWidget*)));
		connect(this, SIGNAL(sendMessage(QString)), mw, SIGNAL(printMessageToMessageWindow(QString)));
		connect(this, SIGNAL(importMesh(QStringList)), mw, SIGNAL(importMeshByNamesSig(QStringList)));
		connect(&_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
		connect(&_process, SIGNAL(readyReadStandardOutput()), this, SLOT(readSolverOutput()));

		emit addProcessBar(this);
		const QString appPath = qApp->applicationDirPath();
		_solverPath = appPath + "/../Solver/";
		_tempPath = appPath + "/../temp/";
	}

	MesherControlerBase::~MesherControlerBase()
	{
		_process.kill();
	}

	void MesherControlerBase::start()
	{
		if (copyTemplate())
		{
			IO::TempalteReplacer t(_mesher);
			t.appendFile(_tempPath + _mesher->getTemplate());
			t.replace();
		}
		QString oldDir = QDir::currentPath();
		QDir::setCurrent(_tempPath);
		qDebug() << _tempPath;
		QString cmd = _solverPath + _mesher->getMesherName();
		qDebug() << cmd;
		_process.start(cmd);

		QDir::setCurrent(oldDir);

	}

	bool MesherControlerBase::copyTemplate()
	{
		QString t = _mesher->getTemplate();
		QString outf = _mesher->getOutput();
		QString tempalteOri = _solverPath + t;
		QString tempalteDes = _tempPath + t;
		QString outputfile = _tempPath + outf;
		QFile tfile(tempalteOri);
		if (!tfile.exists()) return false;
		
		QFile dfile(tempalteDes);
		if (dfile.exists())
			dfile.remove();

		tfile.copy(tempalteDes);
		
		QFile ofile(outputfile);
		if (ofile.exists())
			ofile.remove();

		return true;
	}

	void MesherControlerBase::processFinished(int, QProcess::ExitStatus)
	{
		
		QStringList flies;
		flies.append(_tempPath + _mesher->getOutput());
		emit importMesh(flies);
		this->setProcess(100);
		Py::PythonAagent::getInstance()->unLock();
		
	}

	void MesherControlerBase::readSolverOutput()
	{
		_outputBuffer = QString::fromLocal8Bit(_process.readAll());
		emit sendMessage(_outputBuffer);
	}

}