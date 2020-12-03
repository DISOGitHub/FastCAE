/*******************************
������������
libaojun
********************************/

#ifndef SOLVERCONTROLBASE_H_
#define SOLVERCONTROLBASE_H_

#include "solverControlAPI.h"
#include <QObject>
#include <QString>
#include <QStringList>
#include <QProcess>

namespace GUI
{
	class MainWindow;
}
namespace ModuleBase
{
	class ProcessBar;
}
namespace Post
{
	class RealTimeWindowBase;
}
namespace ConfigOption
{
	class SolverInfo;
}
namespace ModelData
{
	class ModelDataBase;
}

namespace SolverControl
{
	class SOLVERCONTROLAPI SolverControlBase : public QObject
	{
		Q_OBJECT
	public:
		SolverControlBase(GUI::MainWindow* mainwindow, ConfigOption::SolverInfo* solver, ModelData::ModelDataBase* model,bool sollveOnly = false);
		~SolverControlBase();
		///�������
		void startSolver();
		//����������ƣ�������
		void setSolverDescription(QString des);
		//�������
		void stopSolver();
		//����MPI����
		void startMPI(int nprocess);
		//������������
		void setSolveArgs(QString arg);
		//��ˬ����
		void startSolverClear();


	signals:
		void solverStarted(QWidget* processBar);
		void sendMessage(QString mess);
		void processFinish(int pid);
		void openRealTime(Post::RealTimeWindowBase* w, int proID);
		void removeSolver(int pid);

	protected slots:
		virtual void readSolverOutput();
		//�����������
		void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
		//ǿ����ֹ���
		void stopSolver(QWidget* w);

	protected:
		//д���ļ�
		bool preProcess();
		//����ļ�ת��
		bool postPorocess();

	protected:
		bool _solveOnly = false;
		QProcess _process{};
		GUI::MainWindow* _mainWindow{};
		QString _description{};
		QString _args{};
		ModuleBase::ProcessBar* _processBar{};
		QString _outputBuffer{};
		bool _processFinished{ false };

		ConfigOption::SolverInfo* _solver{};
		ModelData::ModelDataBase* _model{};
	
	};
}


#endif
