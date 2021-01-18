#ifndef _DIALOGUSERGUIDENCE_H__
#define _DIALOGUSERGUIDENCE_H__

#include "UserGuidenceAPI.h"
#include <QDialog>
#include <QList>

class QAction;

namespace GUI
{
	class MainWindow;
}
namespace Ui
{
	class UserGuidenceDialog;
}


namespace Guidence
{

	class USERGUIDENCEAPI UserGuidenceDialog : public QDialog
	{
		Q_OBJECT
	public:
		UserGuidenceDialog(GUI::MainWindow* mw, QAction* ac);
		~UserGuidenceDialog();

	private slots:
		void on_backButton_clicked();
		void on_closeButton_clicked();
		void on_documentButton_clicked();
		void on_exampleButton_clicked();

	private:
		void init();
		void closeEvent(QCloseEvent * e) override;
		void showEvent(QShowEvent * e) override;

	private:
		Ui::UserGuidenceDialog* _ui{};
		QAction* _action{};

		QList<QWidget*> _examples{};
	};



}



#endif