#include "RecordScript.h"
#include "PyAgent.h"
#include <qfile.h>
#include <QTimer>
#include <QTextStream>
#include <qreadwritelock.h>
#include <QDir>
#include <QCoreApplication>

namespace Py
{
	unsigned int LastSize = 0;

	RecordThread::RecordThread(QObject *parent) :
		_file(new QFile(qApp->applicationDirPath() + "/../temp/RecordScript.py")),
		_IO(new QTextStream(_file))
	{
		QDir dir(qApp->applicationDirPath() + "/../temp/");
		if (!dir.exists())
			dir.mkdir(qApp->applicationDirPath() + "/../temp/");
	}

	RecordThread::~RecordThread()
	{
		
		delete _IO;
		_file->remove();
		_file->deleteLater();
	}


	void RecordThread::pause()
	{
		_states = -1;
	}

	void RecordThread::stop()
	{
		_states = 1;
	}

	void RecordThread::reStart()
	{
		LastSize = 0;
		_states = 0;
	}


	void RecordThread::run()
	{
		while (true)
		{
			if (_states == 1) return;
			this->msleep(500);
			writeRecordScriptFile();
		}
	}

	void RecordThread::writeRecordScriptFile()
	{
		if (_states != 0) return;

		QStringList codelist = PythonAagent::getInstance()->getcodelist();
		unsigned int CurrentSize = codelist.size();
		if (CurrentSize <= LastSize) return;
		
		bool ok = false;
		if (LastSize > 0)
			ok = _file->open(QIODevice::WriteOnly | QIODevice::Append);
		else
			ok = _file->open(QIODevice::WriteOnly);

		if (!ok) return;

		for (int i = LastSize; i < CurrentSize; i++)
			*_IO << codelist.at(i) << endl;

		LastSize = CurrentSize;
		_file->close();
	}
}