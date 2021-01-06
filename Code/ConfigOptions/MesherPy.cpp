#include "MesherPy.h"
#include <QString>
#include "ConfigOptions.h"
#include "ConfigOptions/MeshConfig.h"
#include "ConfigOptions/SolverConfig.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/MesherInfo.h"
#include"DataProperty/ParameterBase.h"
#include "python/PyAgent.h"
#include <QDebug>
namespace ConfigOption{


	void MesherPy::setValue(int index, char* name, char* stype, char* svalue)
	{

		QString sname(name);
		QString type(stype);
		QString value(svalue);
		SolverOption* solvers = ConfigOption::getInstance()->getSolverOption();
		if (solvers == nullptr) return;
		MesherInfo* info = solvers->getMesherAt(index);
		if (info == nullptr) return;
		DataProperty::ParameterBase* para= info->getParameterByName(sname);
		if (para == nullptr) return;
		DataProperty::ParaType t = DataProperty::ParameterBase::StringToParaType(type);
		if (t != para->getParaType()) return;
		para->setValueFromString(value);
		Py::PythonAagent::getInstance()->unLock();


		
		

	}
}

void CONFIGOPTIONSAPI setValue(int index, char* name, char* stype, char* svalue)
{
	ConfigOption::MesherPy::setValue(index, name, stype, svalue);
}
