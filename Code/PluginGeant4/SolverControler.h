#ifndef __SOLVERCONTROL_H__
#define __SOLVERCONTROL_H__

#include <QObject>
#include <QList>
#include <QStringList>

struct Point
{
	double X{ 0 };
	double Y{ 0 };
	double Z{ 0 };
};

class QFile;

namespace GUI
{
	class MainWindow;
	class SolveProcessManager;
}

namespace ConfigOption
{
	class SolverInfo;
}

namespace Geant4
{
	class Geant4SolverControler : public QObject
	{
		Q_OBJECT
	public:
		Geant4SolverControler(GUI::MainWindow* mainwin, ConfigOption::SolverInfo*  sol);
		~Geant4SolverControler();

		void writeMacFile();
		void startSolve();

	private slots:
		void solvingFinised();

	private:
		void readResultFile(QFile* file);
		void addTrack(int &type, QList<Point>& points, QStringList newline);

	private:
		GUI::MainWindow* _mainWindow{};
		ConfigOption::SolverInfo* _solver{};

		GUI::SolveProcessManager* _solverManager{};

		
	};
}

#endif