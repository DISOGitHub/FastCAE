#include "ScriptReader.h"
#include "PyAgent.h"
#include "moduleBase/messageWindowBase.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QRegExp>

namespace Py
{


	ScriptReader::ScriptReader(QString filename, PythonAagent* agent)
	{
		_fileName = filename;
		_agent = agent;
	}
	void ScriptReader::run()
	{
		QFile file(_fileName);
		if (!file.open(QIODevice::Text | QIODevice::ReadOnly))
		{
			emit _agent->printInfo(ModuleBase::Error_Message, tr("Script open failed"));
			return;
		}
		QTextStream stream(&file);
		while (!stream.atEnd())
		{
			while (_locker);
			QString line = stream.readLine().simplified();
			
			qDebug() << "read: " << line;
			if (line.isEmpty()) continue;
			pause();
			_agent->submit(line, true);

			if (isPythonObj(line) && !line.toLower().contains("thread"))
				restart();

		}
		file.close();
		//		_interpreter->execFile(filename);
		emit _agent->printInfo(ModuleBase::Normal_Message, tr("Script Executed %1").arg(_fileName));
	}

	void ScriptReader::pause()
	{
	//	_mutex.lock();
		_locker = true;
		qDebug() << "lock";
	}
	void ScriptReader::restart()
	{
// 		_mutex.tryLock();
// 		_mutex.unlock();
		_locker = false;
		qDebug() << "unlock";
	}

	bool ScriptReader::isPythonObj(QString line)
	{
		if (line.contains("=")) return true;
		QStringList cs = line.split(".");
		if (cs.at(0).toLower() == cs.at(0))
			return true;
		return false;
	}

}