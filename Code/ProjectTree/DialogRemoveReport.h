#ifndef _REMOVEREPORTDIALOG_H_
#define _REMOVEREPORTDIALOG_H_

#include "SelfDefObject/QFDialog.h"

namespace GUI
{
	class MainWindow;
}
namespace Ui
{
	class RemoveReportDialog;
}

namespace ProjectTree
{
	class RemoveReportDialog : public QFDialog
	{
		Q_OBJECT
	public:
		RemoveReportDialog(GUI::MainWindow* m);
		~RemoveReportDialog();

		bool isRemoveCompletely();

	private:
		Ui::RemoveReportDialog* _ui{};

	};
}


#endif
