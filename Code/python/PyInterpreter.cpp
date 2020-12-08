#include <Python.h>
#include "PyInterpreter.h"
#include "PyAgent.h"
#include <QCoreApplication>
#include "moduleBase/messageWindowBase.h"
#include <assert.h>
#include <QDebug>
#include <QDir>
#include <QByteArray>
#include <string>
#include <QReadWriteLock>

namespace Py
{

	bool PyInterpreter::init(PythonAagent* agent)
	{
		if (!Py_IsInitialized())
			return false;
		_agent = agent;
		PyErr_PrintEx(1);
		this->execCode("import sys", false);
		this->execCode("import os", false);
		QString path = QDir::cleanPath(qApp->applicationDirPath());
		QString qs = QString("sys.path.append(\"%1\")").arg(path);
		this->execCode(qs, false);

		QDir dir(path);
		QStringList suffix; suffix << "*.py";
		dir.setNameFilters(suffix);
		QList<QFileInfo> files = dir.entryInfoList(suffix);
		for (int i = 0; i < files.size(); ++i)
		{
			QFileInfo fileinfo = files.at(i);
			QString name = fileinfo.baseName();
			QString command = QString("import %1").arg(name);
			this->execCode(command, false);
		}

		return true;
	}

	int PyInterpreter::execCode(QString code, bool save)
	{
		QReadWriteLock lock;
		lock.lockForRead();
		std::string s = code.toStdString();
		const char* c = s.c_str();
		qDebug() << "exec: " << code;
				
		int ok = PyRun_SimpleStringFlags(c,NULL);
		
		if (ok == -1)
		{
			QString error = QString(tr("Exception occurred at: \"%1\"")).arg(code);
			_agent->printInfo((int)ModuleBase::Warning_Message, error);
		}
	
		if (save)
			_codelist.append(code);
		lock.unlock();
		return ok;
	}

	void PyInterpreter::execFile(QString file)
	{
		QByteArray la = file.toLocal8Bit();
		char* c = la.data();
		FILE * fp = nullptr;
		fp = fopen(c, "r");
		if (fp != nullptr)
			PyRun_SimpleFile(fp,c);		
	}

	int PyInterpreter::getCodeCount()
	{
		return _codelist.size();
	}

	QString PyInterpreter::getCodeAt(int i)
	{
		QString c;
		if (i >= 0 && i < _codelist.size())
			c = _codelist.at(i);
		return c;
	}

	void PyInterpreter::codeListAppend(QString qs)
	{
		_codelist.append(qs);
	}

	QStringList PyInterpreter::getCode()
	{
		return _codelist;
	}
}