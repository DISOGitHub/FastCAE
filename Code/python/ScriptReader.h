#ifndef SCRIPTREADER_H
#define SCRIPTREADER_H

#include <QThread>
#include <QMutex>
namespace Ui
{
	class graph_MainWindowPy;
}
namespace Py
{
	class PythonAagent;

	class ScriptReader : public QThread
	{
		Q_OBJECT
	public: 
		ScriptReader(QString fileName, PythonAagent* agent);
		~ScriptReader();

		void pause();
		void restart();
		void stop();

	private:
		void run() override;
		bool isPythonObj(QString line);

	private:
		QString _fileName{};
		PythonAagent* _agent{};
	//	QMutex _mutex{};
		bool _stop{ false };
		int _lockCount{ 0 };
	};
}


#endif