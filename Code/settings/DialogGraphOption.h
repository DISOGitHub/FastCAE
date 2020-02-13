#ifndef DIALOGGRAPHOPTION_H_
#define DIALOGGRAPHOPTION_H_

//#include "SelfDefObject/QFDialog.h"
#include <QDialog>

namespace GUI
{
	class MainWindow;
}
namespace Ui
{
	class GraphOptionDialog;
}


namespace Setting
{
	class GraphOption;

	class GraphOptionDialog : public QDialog
	{
		Q_OBJECT

	public:
		GraphOptionDialog(GUI::MainWindow* mainwindow, GraphOption* option);
		~GraphOptionDialog();

	signals:
		void updateGraph();

		private slots:
		void on_out_OkButton_clicked();
		void on_out_ApplyButton_clicked();
		void on_out_CancelButton_clicked();

	private:
		void accept() override;
		void init();

	private:
		Ui::GraphOptionDialog* _ui{};
		GUI::MainWindow* _mainWindow{};
		GraphOption* _graphOption{};
	};


}




#endif
