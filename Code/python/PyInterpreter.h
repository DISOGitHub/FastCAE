#ifndef _PYINTERPRETER_H_
#define _PYINTERPRETER_H_

#include <QObject>
#include <QStringList>

namespace GUI
{
	class MainWindow;
}

namespace Py
{
	class PythonAagent;

	class PyInterpreter : public QObject
	{
		Q_OBJECT
	public:
		PyInterpreter() = default;
		~PyInterpreter() = default;

		bool init(PythonAagent* agent);
		int execCode(QString code,bool save = true);
		void execFile(QString file);
		int getCodeCount();
		QString getCodeAt(int i);
		void codeListAppend(QString qs);
		QStringList getCode();

	private:
		QStringList _codelist{};
		PythonAagent* _agent{};
	};

}


#endif