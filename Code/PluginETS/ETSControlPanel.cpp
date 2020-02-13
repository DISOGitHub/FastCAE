#include "ETSControlPanel.h"
#include <ui_ETSControlPanel.h>

namespace ETS
{
	ETSControlPanel::ETSControlPanel(GUI::MainWindow* m)
		:ModuleBase::DockWidgetBase(m,ModuleBase::ProjectWindows)
	{
		_ui = new Ui::ETSControlPanel;
		_ui->setupUi(this);
		this->repaintTitleBar();
	}

	ETSControlPanel::~ETSControlPanel()
	{
		if (_ui != nullptr) delete _ui;
	}

}


 