#ifndef _MESHERCONTROLER_H_
#define _MESHERCONTROLER_H_

#include "moduleBase/processBar.h"
#include "solverControlAPI.h"
#include <QProcess>

namespace GUI
{
	class MainWindow;
}
namespace ConfigOption
{
	class MesherInfo;
}

namespace SolverControl
{
	class SOLVERCONTROLAPI MesherControlerBase : public ModuleBase::ProcessBar
	{
		Q_OBJECT

	public:
		MesherControlerBase(GUI::MainWindow* mw, QString mesher);
		~MesherControlerBase();

		void start();
	
	signals:
		void addProcessBar(QWidget* w);
		void sendMessage(QString);
		void importMesh(QStringList);

	private:
		bool copyTemplate();

	private slots:
		void processFinished(int, QProcess::ExitStatus);
		void readSolverOutput();

	private:
		ConfigOption::MesherInfo* _mesher{};
		QString _solverPath{};
		QString _tempPath{};
		QProcess _process;
		QString _outputBuffer{};

	};

}



#endif // !_MESHERCONTROLER_H_
