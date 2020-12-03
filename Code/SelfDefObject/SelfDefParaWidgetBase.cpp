#include "SelfDefParaWidgetBase.h"
#include "python/PyAgent.h"
#include "DataProperty/ParameterBase.h"
#include <QDebug>
#include "ConfigOptions/SolverConfig.h"
#include "ConfigOptions/ConfigOptions.h"
namespace SelfDefObj
{

	SelfDefParaWidgetBase::SelfDefParaWidgetBase(DataProperty::ParameterBase* d)
	{
		_data = d;
		_pyagent = Py::PythonAagent::getInstance();
		genBasicCode();
	}

	void SelfDefParaWidgetBase::submitCode()
	{
		if (!_inited) return;
		QString code = generateCode();
		if (code.isEmpty()) return;
		if (code.contains("%")) return;
		_pyagent->submit(code);
	}

	void SelfDefParaWidgetBase::genBasicCode()
	{
		DataProperty::ParaType pt = _data->getParaType();
		QString stype = DataProperty::ParameterBase::ParaTypeToString(pt);

		QString code;
		DataProperty::ModuleType t = _data->getModuleType();
		int id = _data->getDataID();
		int index = _data->getDataIndex();
		QString name = _data->genAbsoluteName();

		switch (t)
		{
		case DataProperty::Module_Model:
			code = QString("Case.setValue(%1,\"%2\",\"%3\",\"%Value%\")").arg(id).arg(name).arg(stype); break;
		case DataProperty::Module_BC:
			code = QString("Case.setBCValue(%1,%2,\"%3\",\"%4\",\"%Value%\")").arg(id).arg(index).arg(name).arg(stype); break;
		case DataProperty::Module_Material:
			code = QString("Material.setValue(%1,\"%2\",\"%3\",\"%Value%\")").arg(id).arg(name).arg(stype); break;
		case DataProperty::Module_Mesher:
			code = QString("Mesher.setValue(%1,\"%2\",\"%3\",\"%Value%\")").arg(index).arg(name).arg(stype); break;

		default:
			break;
		}
		_basicCode = code;
	}

	QString SelfDefParaWidgetBase::generateCode()
	{
// 		QString c = _data->valueToString();
// 		QString code = _basicCode;
// 		code.replace("%Value%", c);
// 		return code;
		return _basicCode;
	}

}