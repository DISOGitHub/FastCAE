#ifndef _SOLVERCONFIG_H_
#define _SOLVERCONFIG_H_

#include "ConfigOptionsAPI.h"
#include <QList>
#include <QString>

namespace ConfigOption
{
	class SolverInfo;
	class MesherInfo;

	class CONFIGOPTIONSAPI SolverOption
	{
	public:
		SolverOption() = default;
		~SolverOption() = default;
		
		void clearData();
		void appendSolver(SolverInfo* s);
		void appendMesher(MesherInfo* m);
		SolverInfo* getSolverAt(int index);
		MesherInfo* getMesherAt(int index);
		MesherInfo* getMesherByName(QString name);
		int getSolverCount();
		int getMesherCount();
		void removeSolver(int index);
		bool read(QString f);
		void write(QString f);

	private:
		QList<SolverInfo*> _solverlist{};
		QList<MesherInfo*> _mesherList{};

	};
}


#endif