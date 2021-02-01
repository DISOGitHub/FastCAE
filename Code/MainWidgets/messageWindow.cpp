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
//		this->repaintTitleBar();
//		this->titleBarWidget()->setVisible(false);

		QWidget* lTitleBar = this->titleBarWidget();
		QWidget* lEmptyWidget = new QWidget();
		this->setTitleBarWidget(lEmptyWidget);
		delete lTitleBar;

	}
	MessageWindow::~MessageWindow()
	{
//		delete _ui;
	}

}