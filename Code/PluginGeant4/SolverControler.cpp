#include "SolverControler.h"
#include "mainWindow/mainWindow.h"
#include "mainWindow/signalHandler.h"
#include "ConfigOptions/SolverConfig.h"
#include "ConfigOptions/SolverInfo.h"
#include "GenerateMacFile.h"
#include "SolverControl/SolverControlerBase.h"
#include "mainWindow/solveProcessManager.h"
#include "DataManager.h"
#include "PreWindowManager.h"
#include <QFileInfo>
#include <QApplication>
#include <QDebug>
#include <QList>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkLine.h>

namespace Geant4
{
	Geant4SolverControler::Geant4SolverControler(GUI::MainWindow* mainwin, ConfigOption::SolverInfo* sol)
		:_mainWindow(mainwin),_solver(sol)
	{
		_solverManager = _mainWindow->getSignalHandler()->getSolveProcessManager();
	}

	Geant4SolverControler::~Geant4SolverControler()
	{

	}

	void Geant4SolverControler::writeMacFile()
	{		
		QString macfile = qApp->applicationDirPath()+"/../temp/MyMac.mac";
		GenerateMacFile m;
		m.write(macfile);
	}

	void Geant4SolverControler::startSolve()
	{
		//如果结果文件存在，先删除结果文件 
		QString exePath = _solver->getExePath();
		QFileInfo inf(exePath);
		QString dir = inf.absolutePath();
		const QString file = dir + "/Trajectory.txt";

		QFile resFile(file);
		if (resFile.exists())
			resFile.remove();

		//启动求解过程，生成新的结果文件
		QString macfile = qApp->applicationDirPath() + "/../temp/MyMac.mac";
		auto solvercon = new SolverControl::SolverControlBase(_mainWindow,_solver,nullptr);
		solvercon->setSolveArgs(macfile);
		solvercon->setSolverDescription("Solving Geant4...");
		_solverManager->insertProcess(-1, solvercon);
		solvercon->startSolverClear();
		connect(solvercon, SIGNAL(processFinish(int)), this, SLOT(solvingFinised()));
	}

	void Geant4SolverControler::solvingFinised()
	{
		//求解过程结束，读取新的结果文件
		QString exePath = _solver->getExePath();
		QFileInfo inf(exePath);
		QString dir = inf.absolutePath();
		const QString file = dir + "/Trajectory.txt";
		
		QFile resFile(file);
		if (!resFile.exists()) return;//结果文件不存在，不读取

		//读取结果文件
		readResultFile(&resFile);
	}

	void Geant4SolverControler::readResultFile(QFile* file)
	{
		if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) return;
		QString line;
		int type = 0;
		QList<Point> points;

		while (!file->atEnd())
		{
			line = file->readLine().simplified();
			qDebug() << line;
			QStringList sl = line.split(" ");
			if (sl.size() == 3)
			{
				Point p;
				p.X = sl.at(0).toDouble();
				p.Y = sl.at(1).toDouble();
				p.Z = sl.at(2).toDouble();
				points.append(p);
			}
			else if (sl.size() == 2)
			{
				addTrack(type, points, sl);
				continue;
			}
		}
		file->close();
		addTrack(type, points, QStringList());
//		DataManager::getInstance()->getPreWindowManager()->updataTrackProperty();
	}

	void Geant4SolverControler::addTrack(int &type, QList<Point>& points, QStringList newLine)
	{
		//处理原先的点
		const int npoint = points.size();
		if (npoint > 1 && type < 4 && type > 0)
		{
			vtkPoints* pts = vtkPoints::New();
			for (int i = 0; i < npoint; ++i)
			{
				Point p = points.at(i);
				pts->InsertNextPoint(p.X, p.Y, p.Z);
			}
			vtkCellArray* cells = vtkCellArray::New();
			for (int i =1;i<npoint; ++i)
			{
				auto line = vtkLine::New();
				line->GetPointIds()->SetId(0, i - 1);
				line->GetPointIds()->SetId(1, i);
				cells->InsertNextCell(line);
			}
			vtkPolyData* polydata = vtkPolyData::New();
			polydata->SetPoints(pts);
			polydata->SetPolys(cells);
			DataManager::getInstance()->getPreWindowManager()->addTrack(type, polydata);
		}

		//将类型与点集合清空
		type = 0; points.clear();
		if (newLine.size() == 2)
		{
			QString stype = newLine[1].toLower().simplified();
			if ("e+" == stype || "e-" == stype)
				type = 2;
			else if ("gamma" == stype)
				type = 3;
			else if ("proton" == stype)
				type = 1;
		}
	}

}


