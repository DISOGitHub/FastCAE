#include "Post3DInterface.h"
#include "ui_Post3DInterface.h"
#include "Post3DWidget.h"


namespace Post
{
	Post3DWindowInterface::Post3DWindowInterface(GUI::MainWindow* m, int proid) :
		PostWindowBase(m, proid, ModuleBase::Post3D, D3), _ui(new Ui::Post3DInterface)
	{
		_ui->setupUi(this);
		_post3DWindow = new Post3DWindow(m, proid);
		_ui->graphLayout->addWidget(_post3DWindow);

		appendGraphWindow(_post3DWindow);
		setPropWidget(_post3DWindow->getPropWidget());
		setTreeWidget(_post3DWindow->getTreeWidget());
		_toolBarList = _post3DWindow->getToolBars();
	}
	Post3DWindowInterface::~Post3DWindowInterface()
	{
// 		if (_post3DWindow != nullptr)
// 			delete _post3DWindow;
	}
	void Post3DWindowInterface::init()
	{

	}
	Post3DWindow* Post3DWindowInterface::getWindow()
	{
		return _post3DWindow;
	}
	void Post3DWindowInterface::reTranslate()
	{
		_post3DWindow->reTranslate();
	}

	void Post3DWindowInterface::saveImage(QString fileName, int width, int heigh, bool showDlg)
	{
		_post3DWindow->saveImage(fileName, width, heigh, showDlg);
	}

}