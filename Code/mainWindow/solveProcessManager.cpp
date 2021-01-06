#include "solveProcessManager.h"
#include "SolverControl/SolverControlerBase.h"


namespace GUI
{
	SolveProcessManager::~SolveProcessManager()
	{
		QList<SolverControl::SolverControlBase*> ss = _solvingProcess.values();
		for (auto s : ss)
			s->stopSolver();
		_solvingProcess.clear();
	}


	bool SolveProcessManager::isSolving(int id)
	{
		return _solvingProcess.contains(id);
	}

	void SolveProcessManager::insertProcess(int id, SolverControl::SolverControlBase* p)
	{
		connect(p, SIGNAL(removeSolver(int)), this, SLOT(removeSolve(int)));
		bool ok = connect(p, SIGNAL(processFinish(int)), this, SLOT(solveFinished(int)));
		if (ok)
			_solvingProcess.insert(id, p);
	}
	


	void SolveProcessManager::solveFinished(int id)
	{
		auto p = _solvingProcess.value(id);
		_solvingProcess.remove(id);
		if (p == nullptr) return;
		p->stopSolver();
	}

	void SolveProcessManager::removeSolve(int id)
	{
		_solvingProcess.remove(id);
	}

}