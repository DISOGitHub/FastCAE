/*******************************
求解控制器基类
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
		///启动求解
		void startSolver();
		//设置求解名称（描述）
		void setSolverDescription(QString des);
		//结束求解
		void stopSolver();
		//启动MPI命令
		void startMPI(int nprocess);
		//设置启动参数
		void setSolveArgs(QString arg);
		//清爽启动
		void startSolverClear();


	signals:
		void solverStarted(QWidget* processBar);
		void sendMessage(QString mess);
		void processFinish(int pid);
		void openRealTime(Post::RealTimeWindowBase* w, int proID);
		void removeSolver(int pid);

	protected slots:
		virtual void readSolverOutput();
		//程序运行完成
		void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
		//强行终止求解
		void stopSolver(QWidget* w);

	protected:
		//写出文件
		bool preProcess();
		//结果文件转换
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
