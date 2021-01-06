#ifndef _PYAGENT_H_
#define _PYAGENT_H_

#include "pythonAPI.h"
#include <QObject>
#include <QStringList>

namespace GUI
{
	class MainWindow;
}

namespace Py
{
	class PyInterpreter;
	class ScriptReader;
	class RecordThread;

	class PYTHONAPI PythonAagent : public QObject
	{
		Q_OBJECT
	public:
		static PythonAagent* getInstance();
		void initialize(GUI::MainWindow* m);
		void finalize(); 
		void submit(QString code, bool save = true);
		//后台执行，不在界面显示，也不保存
		void backstageExec(QString code);
		void submit(QStringList codes, bool save = true);
		void saveScript(QString fileName);
		bool execScript(QString fileName);
		void appCodeList(QString code);
		void lock();
		void unLock();
		bool isLocked();
		void appendOn();
		void appendOff();
		void execMessWinCode(QString code);
		QStringList getcodelist();
		void setNoGUI(bool nogui);



	signals:
		void printInfo(int type, QString m);
		void closeMainWindow();

	private:
		PythonAagent();
		~PythonAagent() = default; 
		void connectSignals();

	private slots:
		void readerFinished();

	private:
		static PythonAagent* _instance;
		PyInterpreter* _interpreter{};
		RecordThread* _recordScript{};
		GUI::MainWindow* _mainWindow{};
		ScriptReader* _reader{};
		bool _islock{ false };
		bool _append{ true };

		bool _noGUI{ false };
	
	};
}



#endif