/************************
输出信息框
************************/
#ifndef _MESSAGEWINDOW_H_
#define _MESSAGEWINDOW_H_

#include "mainWidgetsAPI.h"
#include "moduleBase/messageWindowBase.h"

namespace GUI
{
	class MainWindow;
}

namespace MainWidget
{

class MAINWIDGETSAPI MessageWindow : public ModuleBase::MessageWindowBase
{
	Q_OBJECT
public:
	MessageWindow(GUI::MainWindow* m);
	~MessageWindow();


};



}


#endif