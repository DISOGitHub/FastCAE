/**********************************
进度框
***********************************/
#ifndef _PROCESSWINDOW_H_
#define _PROCESSWINDOW_H_

#include "mainWidgetsAPI.h"
#include "moduleBase/ProcessWindowBase.h"
//#include "dockWidgetBase.h"

namespace GUI
{
class MainWindow;
}

namespace Ui
{
class ProcessWindow;
}

namespace MainWidget
{

//	class ProcessBar;

class MAINWIDGETSAPI ProcessWindow : public ModuleBase::ProcessWindowBase //public QDockWidget, public CModuleBase
{
	Q_OBJECT
public:
	ProcessWindow(GUI::MainWindow* mainwindow, QWidget* parent = 0);
	virtual ~ProcessWindow();

};


}


#endif
 
