#include "MaterialPy.h"
#include "Material.h"
#include "MaterialSingletion.h"
#include "python/PyAgent.h"
#include <QString>

namespace Material
{
	void MaterialPy::setValue(int ID, char* variable, char* type, char* value)
	{
		QString svariable(variable);
		QString sType(type);
		QString sValue(value);
		Material* ma = MaterialSingleton::getInstance()->getMaterialByID(ID);
		if (ma == nullptr) return;
		DataProperty::ParameterBase* p = ma->getParameterByName(svariable);
		if (p == nullptr) return;
		DataProperty::ParaType t = DataProperty::ParameterBase::StringToParaType(sType);
		if (t != p->getParaType()) return;
		p->setValueFromString(sValue);
		Py::PythonAagent::getInstance()->unLock();
	}


}

void MATERIALAPI setValue(int ID, char* variable, char* stype, char* svalue)
{
	Material::MaterialPy::setValue(ID, variable, stype, svalue);
}