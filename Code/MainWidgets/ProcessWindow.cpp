#include "ProcessWindow.h"
#include "mainWindow/mainWindow.h"

namespace MainWidget
{

	ProcessWindow::ProcessWindow(GUI::MainWindow* mainwindow, QWidget* parent /* = 0 */) :
		ProcessWindowBase(mainwindow)
	{
//		this->repaintTitleBar();
//		this->titleBarWidget()->setVisible(false);

		QWidget* lTitleBar = this->titleBarWidget();
		QWidget* lEmptyWidget = new QWidget();
		this->setTitleBarWidget(lEmptyWidget);
		delete lTitleBar;

	}
	ProcessWindow::~ProcessWindow()
	{

	}
}