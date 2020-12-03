#ifndef _GMSHPLUGIN_H_
#define _GMSHPLUGIN_H_

#include "GmshModuleAPI.h"
#include <QObject>
#include "moduleBase/messageWindowBase.h"

class QDialog;

namespace GUI
{
	class MainWindow;
}

namespace MainWidget
{
	class PreWindow;
}

namespace Py
{
	class PythonAagent;
}

namespace Gmsh
{
	class GmshThread;
	class GmshThreadManager;
	class GMshPara;

	class GMSHAPI GmshModule : public QObject
	{
		Q_OBJECT

	public:
	   static GmshModule* getInstance(GUI::MainWindow* m);

	   //执行
	   void exec(int commandType, MainWidget::PreWindow* pre);
	   //结束
	   void finalize();
	   //初始化一个GmshThread
	   GmshThread* iniGmshThread(GMshPara*);
	   //获取线程管理者
	   GmshThreadManager* getGmshThreadManager();
	   
	signals:
		void showDialog(QDialog* d);
		void updateMeshTree();
		void updateActions();
		void generateSig(GMshPara* para);
		void printMessageToMessageWindow(ModuleBase::Message message);
		void updateSetTree();

	private slots:
		void preWindowOpened(MainWidget::PreWindow*);
		void preWindowClosed();
		void generateSlot(GMshPara* para);
		void editMesh(int dim, int index);

	private:
		GmshModule(GUI::MainWindow* m);
		~GmshModule() = default;

	private:
		GUI::MainWindow* _mainwindow{};
		MainWidget::PreWindow* _preWindow{};
		Py::PythonAagent* _pyAgent{};
		GmshThreadManager* _threadManager{};

		static GmshModule* _instance;
	};
}



#endif