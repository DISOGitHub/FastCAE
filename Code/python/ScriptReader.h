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
		ScriptReader(QString filename, PythonAagent* agent);
		~ScriptReader();

		void pause();
		void restart();

	private:
		void run() override;
		bool isPythonObj(QString line);

	private:
		QString _fileName{};
		PythonAagent* _agent{};
	//	QMutex _mutex{};
		bool _locker{ false };
	};
}


#endif