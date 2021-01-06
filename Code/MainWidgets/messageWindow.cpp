#include "messageWindow.h"
//#include <ui_MessageWindow.h>

namespace MainWidget
{

	MessageWindow::MessageWindow(GUI::MainWindow *m) 
		: ModuleBase::MessageWindowBase(m)
	{
// 		_ui = new Ui::MessageWindow();
// 		_ui->setupUi(this);
// 		//connect(parent, SIGNAL(sendInfoToStatesBar(QString)), this, SLOT(ReceiveEvtInfo(QString)));
// 		RegistConnet();
		this->repaintTitleBar();
	}
	MessageWindow::~MessageWindow()
	{
//		delete _ui;
	}

}