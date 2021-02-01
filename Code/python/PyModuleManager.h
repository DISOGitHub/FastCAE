#ifndef __PYMODLULEMANANGER_H__
#define __PYMODLULEMANANGER_H__

#include <QHash>
#include <QString>
#include <QPair>
#include <QVariant>
#include "pythonAPI.h"
#include "DataProperty/ParameterBase.h"

//class PyObject;

namespace Py
{
	class PythonAagent;

	class PYTHONAPI PyModuleManager
	{
	public:
		PyModuleManager();
		~PyModuleManager();

		void init(PythonAagent* agent);

		void import(QString module);

		void execFunctionVoid(QString module, QString function, QList<QPair<DataProperty::ParaType, QVariant>> args);

		int execFunctionInt(QString module, QString function, QList<QPair<DataProperty::ParaType, QVariant>> args);

		double execFunctionDouble(QString module, QString function, QList<QPair<DataProperty::ParaType, QVariant>> args);

		QString execFunctionString(QString module, QString function, QList<QPair<DataProperty::ParaType, QVariant>> args);

	private:
		void* execFunction(QString &module, QString &function, QList<QPair<DataProperty::ParaType, QVariant>> &args);

	private:
		QHash<QString, void*> _moduleHash{};
		PythonAagent* _agent{};
	};
}


#endif