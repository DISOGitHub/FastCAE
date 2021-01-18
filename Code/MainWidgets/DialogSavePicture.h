#ifndef DIALOGSAVEPIC_H
#define DIALOGSAVEPIC_H

#include "mainWidgetsAPI.h"
#include "SelfDefObject/QFDialog.h"

namespace GUI
{
	class MainWindow;
}

namespace Ui
{
	class SavePicDialog;
}

namespace MainWidget
{
	class MAINWIDGETSAPI SavePictureDialog : public QFDialog
	{
		Q_OBJECT
	public:
		SavePictureDialog(GUI::MainWindow* mainwindow, QWidget* w);
		~SavePictureDialog();
	
	signals:
		void saveImageSig(int, int, QString);
	
	private slots:
		void on_browseButton_clicked();

	private:
		void accept() override;
		void init();

	private:
		Ui::SavePicDialog* _ui{};
		QWidget* _widget{};
	};


}


#endif