#include "Python.h"
#include "PyModuleManager.h"
#include "PyAgent.h"
#include "moduleBase/messageWindowBase.h"
#include <QDebug>


namespace Py
{
	PyModuleManager::PyModuleManager()
	{
		
	}

	PyModuleManager::~PyModuleManager()
	{

	}

	void PyModuleManager::init(PythonAagent* agent)
	{
		_agent = agent;
		this->import("os");
		this->import("sys");
	}

	void PyModuleManager::import(QString module)
	{
		if (_moduleHash.contains(module))
			return;
		QByteArray byarray = module.toLatin1();
		auto m = PyImport_ImportModule(byarray.data());
		QString simp = QString("Py Module import %1").arg(module);
		qDebug() << simp;
		if (m == nullptr)
		{
			QString error = QString("Error: Py Module import %1").arg(module);
			emit _agent->printInfo((int)ModuleBase::Warning_Message, error);
		}
		else
			_moduleHash.insert(module, m);
	}

	void PyModuleManager::execFunctionVoid(QString module, QString function, QList<QPair<DataProperty::ParaType, QVariant>> args)
	{
		execFunction(module, function, args);
	}

	int PyModuleManager::execFunctionInt(QString module, QString function, QList<QPair<DataProperty::ParaType, QVariant>> args)
	{
		void* re = execFunction(module, function, args);
		if(re == nullptr)
		{
			QString error = QString("Error return Value %1").arg(function);
			emit _agent->printInfo((int)ModuleBase::Error_Message, error);
			qDebug() << error;
			return -1;
		}
		PyObject* preturn = static_cast<PyObject*>(re);
		int va = 0;
		PyArg_Parse(preturn, "i", &va);
		return va;
	}

	double PyModuleManager::execFunctionDouble(QString module, QString function, QList<QPair<DataProperty::ParaType, QVariant>> args)
	{
		void* re = execFunction(module, function, args);
		if (re == nullptr)
		{
			QString error = QString("Error return Value %1").arg(function);
			emit _agent->printInfo((int)ModuleBase::Error_Message, error);
			qDebug() << error;
			return -1;
		}
		PyObject* preturn = static_cast<PyObject*>(re);
		double va = 0;
		PyArg_Parse(preturn, "f", &va);
		return va;
	}

	QString PyModuleManager::execFunctionString(QString module, QString function, QList<QPair<DataProperty::ParaType, QVariant>> args)
	{
		void* re = execFunction(module, function, args);
		if (re == nullptr)
		{
			QString error = QString("Error return Value %1").arg(function);
			emit _agent->printInfo((int)ModuleBase::Error_Message, error);
			qDebug() << error;
			return -1;
		}
		PyObject* preturn = static_cast<PyObject*>(re);
		std::string s;
		PyArg_Parse(preturn, "s", &s);
		return QString::fromStdString(s);
	}

	void* PyModuleManager::execFunction(QString &module, QString &function, QList<QPair<DataProperty::ParaType, QVariant>> &args)
	{
		void* mod = _moduleHash.value(module);
		if (mod == nullptr)
		{
			QString error = QString("No Module Named %1").arg(module);
			emit _agent->printInfo((int)ModuleBase::Error_Message, error);
			qDebug() << error;
			return nullptr;
		}
		PyObject* pmodule = static_cast<PyObject*>(mod);
		QByteArray funArray = function.toLatin1();
		//function
		PyObject* pFun = PyObject_GetAttrString(pmodule, funArray.data());
		if (pFun == nullptr)
		{
			QString error = QString("No Function Named %1").arg(function);
			emit _agent->printInfo((int)ModuleBase::Error_Message, error);
			qDebug() << error;
			return nullptr;
		}
		//argments
		PyObject* pArgements = nullptr;
		const int nargs = args.size();
		if (args.size() == 0)
			pArgements = Py_BuildValue("()"); // 无参函数
		else
			pArgements = PyTuple_New(nargs);

		for (int i =0, j =0;i<nargs; ++i)
		{
			QPair<DataProperty::ParaType, QVariant> arg = args.at(i);
			PyObject* pyarg = nullptr;
			QVariant va = arg.second;
			switch (arg.first)
			{
			case DataProperty::Para_Int:
				pyarg = Py_BuildValue("i", va.toInt()); break;
			case DataProperty::Para_Double:
				pyarg = Py_BuildValue("f", va.toDouble()); break;
			case DataProperty::Para_String:
			{
				QByteArray array = va.toString().toLatin1();
				pyarg = Py_BuildValue("s", array.data()); break;
			}
			default:
				break;
			}
			if (pyarg != nullptr)
			{
				PyTuple_SetItem(pArgements, j, pyarg);
				j++;
			}
		}
		PyObject* pReturn = PyEval_CallObject(pFun, pArgements);
		return pReturn;
	}

}
