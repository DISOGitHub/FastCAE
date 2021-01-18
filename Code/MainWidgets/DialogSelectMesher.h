#ifndef _DIALOGSELECTMESHER_H_
#define _DIALOGSELECTMESHER_H_

#include "mainWidgetsAPI.h"
#include "SelfDefObject/QFDialog.h"

namespace Ui
{
	class MesherDialog;
}

namespace GUI
{
	class MainWindow;
}

namespace DataProperty
{
	class DataBase;
}

namespace MainWidget
{
	class MAINWIDGETSAPI MesherDialog : public QFDialog
	{

		Q_OBJECT
	public:
		MesherDialog(GUI::MainWindow* mainwindow);
		~MesherDialog();

		bool isSingleMesher();
		DataProperty::DataBase* getMesherInfo();

	private:
		Ui::MesherDialog* _ui{};
		bool _isSingleMesher{ false };

	};
}


#endif