#include "SolverControlerBase.h"
#include "mainWindow/mainWindow.h"
#include "moduleBase/processBar.h"
#include "PostWidgets/RealTimeWindowBase.h"
#include "ModelData/modelDataSingleton.h"
#include "ModelData/modelDataBaseExtend.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/SolverConfig.h"
#include "ConfigOptions/SolverInfo.h"
#include "IO/SolverIO.h"
#include <QMessageBox>
#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>
#include "python/PyAgent.h"

namespace SolverControl
{

	SolverControlBase::SolverControlBase(GUI::MainWindow* mainwindow, ConfigOption::SolverInfo* solver, ModelData::ModelDataBase* m, bool sonly)
		:_mainWindow(mainwindow), _solver(solver), _model(m), _solveOnly(sonly)
	{
		connect(&_process, SIGNAL(readyReadStandardOutput()), this, SLOT(readSolverOutput()));
		connect(this, SIGNAL(sendMessage(QString)), _mainWindow, SIGNAL(printMessageToMessageWindow(QString)));
		connect(this, SIGNAL(solverStarted(QWidget*)), _mainWindow, SIGNAL(addProcessBarSig(QWidget*)));
		connect(_mainWindow, SIGNAL(stopSolve(QWidget*)), this, SLOT(stopSolver(QWidget*)));
		connect(&_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
		connect(this, SIGNAL(processFinish(int)), _mainWindow, SIGNAL(processFinished(int)));
		connect(this, SIGNAL(openRealTime(Post::RealTimeWindowBase*, int)), _mainWindow, SIGNAL(openRealTimeWindowSig(Post::RealTimeWindowBase*, int)));
	
	}
	SolverControlBase::~SolverControlBase()
	{
		if (_processBar != nullptr) delete _processBar;
	}

	void SolverControlBase::setSolverDescription(QString des)
	{
		_description = des;
	}

	void SolverControlBase::startSolver()
	{
		QString solverPath = _solver->getExePath();
		QFileInfo info(solverPath);
		int id = _model->getID();
		
		if ((!info.exists()) || (!info.isFile()))
		{
			QMessageBox::warning(nullptr, tr("Warning"), tr("Solver Path Error! Solve Path : %1").arg(solverPath));
			emit processFinish(id);
			return;
		}
		bool ok = preProcess();
		if (!ok)
		{
			QMessageBox::warning(nullptr, tr("Warning"), tr("Input file write failed !"));
			emit processFinish(id);
			return;
		}

		QString oldDir = "";
		
		QString startPath = _model->getPath();

		if (_solver->getType() == ConfigOption::SelfDevelop)
		{
			QDir dir(startPath);
			if (dir.exists() && (!startPath.isEmpty()))
			{
				oldDir = QDir::currentPath();
				QDir::setCurrent(startPath);
			}

		}
		
		_processBar = new ModuleBase::ProcessBar(_mainWindow, _description, false);

		QString args = _solver->getParameter();
		QRegExp regExp("%.*%");
		regExp.setMinimal(true);
		int pos = regExp.indexIn(args);
		QStringList variables = regExp.capturedTexts();
		for (int i = 0; i < variables.size(); ++i)
		{
			QString variable = variables.at(i);
			QString va = variable.remove("%");
			if (va.toLower() == "modelpath")
				args.replace(variables.at(i), startPath);
//			qDebug() << args;
		}

// 		QString startProcess = solverPath + " " + args;
// 		if (solverPath.contains(" "))
// 			startProcess = QString("\"%1\"").arg(startProcess);
		
//		qDebug() << startProcess;
// 		if (!_args.isEmpty())
// 			startProcess = startProcess + " " + _args;
//		qDebug() << startProcess;

		_process.start(solverPath,QStringList(args));

		emit solverStarted(_processBar);

		ModelData::ModelDataBaseExtend* extend = dynamic_cast<ModelData::ModelDataBaseExtend*>(_model);
		if (extend != nullptr && !_solveOnly)
		{
			QList<ConfigOption::PostCurve*> curves = extend->getMonitorCurves();
			if (curves.size() > 0)
			{
				int id = _model->getID();
				Post::RealTimeWindowBase* w = new Post::RealTimeWindowBase(_mainWindow, id);
				emit openRealTime(w, id);
			}
			
		}
		if (_solver->getType() == ConfigOption::SelfDevelop)
		{
			if (!oldDir.isEmpty())
				QDir::setCurrent(oldDir);
		}
	
	}
	void SolverControlBase::readSolverOutput()
	{
		_outputBuffer = QString::fromLocal8Bit(_process.readAll());
		QString process = _solver->getProcessKeyWord()+"(\\d+)(?:\\s*)";
//		qDebug() << process;
		QRegExp rx(process);
		int pos = _outputBuffer.indexOf(rx);
		if (pos > -1 && _processBar != nullptr)
		{
			QString s = rx.cap(1);
			_processBar->setProcess(s.toInt());
		}

		emit sendMessage(_outputBuffer);
	}
	void SolverControlBase::stopSolver(QWidget* w)
	{
		if (w != _processBar) return;

		int id = -1;
		if(_model != nullptr)
			id = _model->getID();
		emit removeSolver(id);
		if (!_processFinished)
		{
			_process.kill();
		}
		_processBar = nullptr;
//		this->deleteLater();
	}

	void SolverControlBase::stopSolver()
	{
		if (!_processFinished)
		{
			_process.kill();
		}
	}

	void SolverControlBase::startMPI(int nprocess)
	{
		QString solverPath = _solver->getExePath();
		QFileInfo info(solverPath);
		if ((!info.exists()) || (!info.isFile()))
		{
			QMessageBox::warning(nullptr, tr("Warning"), tr("Solver Path Error! Solve Path : %1").arg(solverPath));
			return;
		}
		bool ok = preProcess();
		if (!ok)
		{
			QMessageBox::warning(nullptr, tr("Warning"), tr("Input file write failed !"));
			return;
		}

		QString oldDir = "";

		QString startPath = _model->getPath();

		if (_solver->getType() == ConfigOption::SelfDevelop)
		{
			QDir dir(startPath);
			if (dir.exists() && (!startPath.isEmpty()))
			{
				oldDir = QDir::currentPath();
				QDir::setCurrent(startPath);
			}

		}

		_processBar = new ModuleBase::ProcessBar(_mainWindow, _description, false);

		QString args = _solver->getParameter();
		QRegExp regExp("%.*%");
		regExp.setMinimal(true);
		int pos = regExp.indexIn(args);
		QStringList variables = regExp.capturedTexts();
		for (int i = 0; i < variables.size(); ++i)
		{
			QString variable = variables.at(i);
			QString va = variable.remove("%");
			if (va.toLower() == "modelpath")
				args.replace(variables.at(i), startPath);
			//			qDebug() << args;
		}

		// 		QString startProcess = solverPath + " " + args;
		// 		if (solverPath.contains(" "))
		// 			startProcess = QString("\"%1\"").arg(startProcess);

		//		qDebug() << startProcess;
		// 		if (!_args.isEmpty())
		// 			startProcess = startProcess + " " + _args;
		//		qDebug() << startProcess;

		QString c = QString("mpiexec -n %1 %2").arg(nprocess).arg(solverPath);

		_process.start(c);

		emit solverStarted(_processBar);

		ModelData::ModelDataBaseExtend* extend = dynamic_cast<ModelData::ModelDataBaseExtend*>(_model);
		if (extend != nullptr && !_solveOnly)
		{
			QList<ConfigOption::PostCurve*> curves = extend->getMonitorCurves();
			if (curves.size() > 0)
			{
				int id = _model->getID();
				Post::RealTimeWindowBase* w = new Post::RealTimeWindowBase(_mainWindow, id);
				emit openRealTime(w, id);
			}

		}
		if (_solver->getType() == ConfigOption::SelfDevelop)
		{
			if (!oldDir.isEmpty())
				QDir::setCurrent(oldDir);
		}
	}

	void SolverControlBase::setSolveArgs(QString arg)
	{
		_args = arg;
	}

	void SolverControlBase::startSolverClear()
	{
		_solveOnly = true;
		_processBar = new ModuleBase::ProcessBar(_mainWindow, _description, false);
		QString solverPath = _solver->getExePath();

		_process.start(solverPath, QStringList(_args));

		emit solverStarted(_processBar);

	}

	void SolverControlBase::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
	{
		Q_UNUSED(exitCode);

		_processFinished = true;

		int id = -1;
		if(_model != nullptr)
			id = _model->getID();
		emit processFinish(id);
		

		switch (exitStatus)
		{
		case QProcess::NormalExit:
			emit sendMessage("************************************");
			emit sendMessage("***** Solving process finished *****");
			emit sendMessage("************************************");
			
			break;
		case QProcess::CrashExit:
			emit sendMessage("************************************");
			emit sendMessage("***** Solving process Crashed ******");
			emit sendMessage("************************************");
			break;
		default:
			emit sendMessage("***********************************");
			emit sendMessage("***** Solving process Stopped *****");
			emit sendMessage("***********************************");
			break;
		}
		if (_processBar != nullptr)
		{
			if (_processBar->isBusy())
				_processBar->setProcessRange(0, 100);
			_processBar->setProcess(100);
		}
		postPorocess();
		Py::PythonAagent::getInstance()->unLock();
	}

	bool SolverControlBase::preProcess()
	{
		if (_solveOnly) return true;
		bool istemp = _solver->isWriteTemplate();
		if (istemp)
		{
			QString te = _solver->getTemplate();
			QString path = _model->getPath();
			return IO::SolverIO::replaceTemplate(te, path, _model);
		}
		else
		{
			QString format = _solver->getInputFormat();
			return IO::SolverIO::writeInpFile(format, _model);
		}
		return false;
	}

	bool SolverControlBase::postPorocess()
	{
		if (_solveOnly) return true;
		QString  trans = _solver->getTransfer();
		QString path = _model->getPath();
		return IO::SolverIO::transformFile(trans, path);
	}


}