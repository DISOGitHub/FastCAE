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

	   //Ö´ÐÐ
	   void exec(int commandType, MainWidget::PreWindow* pre);
	   //½áÊø
	   void finalize();

	signals:
		void showDialog(QDialog* d);
		void updateMeshTree();
		void updateActions();
		void generateSig(GMshPara* para);
		void printMessageToMessageWindow(ModuleBase::Message message);

	private slots:
		void preWindowOpened(MainWidget::PreWindow*);
		void preWindowClosed();
		void generateSlot(GMshPara* para);

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