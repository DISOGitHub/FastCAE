#include "DialogRemoveReport.h"
#include "ui_DialogRemoveReport.h"

namespace ProjectTree
{
	RemoveReportDialog::RemoveReportDialog(GUI::MainWindow* m)
		:QFDialog(m)
	{
		_ui = new Ui::RemoveReportDialog;
		_ui->setupUi(this);
	}
	RemoveReportDialog::~RemoveReportDialog()
	{
		if (_ui == nullptr) delete _ui;
	}
	bool RemoveReportDialog::isRemoveCompletely()
	{
		bool ok = _ui->deleteFileCheckBox->isChecked();
		return ok;
	}


}