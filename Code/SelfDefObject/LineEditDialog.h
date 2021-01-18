#ifndef _LINEWDITDIALOG_H_
#define _LINEWDITDIALOG_H_

#include "QFDialog.h"
#include <QString>

namespace GUI
{
	class MainWindow;
}
namespace Ui
{
	class LineEditDialog;
}

namespace SelfDefObj
{
	class SELFDEFINEOBJAPI LineEditDialog : public QFDialog
	{
		Q_OBJECT
	public:
		LineEditDialog(GUI::MainWindow* m, QString &text);
		~LineEditDialog();

		void setDescribe(QString d);

	private:
		void accept() override;

	private:
		QString& _text;
		Ui::LineEditDialog* _ui{};
	};

}



#endif // !_LINEWDITDIALOG_H_
