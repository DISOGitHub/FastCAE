#ifndef SOLVEPROCESSMANAGER_H_
#define SOLVEPROCESSMANAGER_H_

#include <QHash>
#include <QObject>

namespace SolverControl
{
	class SolverControlBase;
}

namespace  GUI
{
	class SolveProcessManager : public QObject
	{
		Q_OBJECT
	public:
		SolveProcessManager() = default;
		~SolveProcessManager();

		bool isSolving(int id);
		void insertProcess(int id, SolverControl::SolverControlBase* p);

	private slots:
	   void solveFinished(int id);

	private:
		QHash<int, SolverControl::SolverControlBase*> _solvingProcess{};
	};

}



#endif