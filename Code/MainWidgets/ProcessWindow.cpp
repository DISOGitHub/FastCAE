#include "ProcessWindow.h"
#include "mainWindow/mainWindow.h"

namespace MainWidget
{

	ProcessWindow::ProcessWindow(GUI::MainWindow* mainwindow, QWidget* parent /* = 0 */) :
		ProcessWindowBase(mainwindow)
	{
		this->repaintTitleBar();
	}
	ProcessWindow::~ProcessWindow()
	{

	}
}