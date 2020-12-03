#include "dialogConfigOceanCurrent.h"
#include "ui_dialogConfigOceanCurrent.h"


namespace PluginShip
{
	ConfigOceanCurrentDialog::ConfigOceanCurrentDialog(GUI::MainWindow *m)
	{
		_ui = new Ui::ConfigOceanCurrentDialog;
		_ui->setupUi(this);

	}

	ConfigOceanCurrentDialog::~ConfigOceanCurrentDialog()
	{

	}
}