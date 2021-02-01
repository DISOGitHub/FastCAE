#ifndef _DIALOGCREATEMATERIAL_H_
#define _DIALOGCREATEMATERIAL_H_

#include "SelfDefObject/QFDialog.h"

namespace GUI
{
	class MainWindow;
}
namespace Ui
{
	class DialogCreateMaterial;
}

namespace MainWidget
{
	class CreateMaterialDialog : public QFDialog
	{
		Q_OBJECT
	public:
		CreateMaterialDialog(GUI::MainWindow*  mainwindow);
		~CreateMaterialDialog();

		void init();
		
	private:
		void accept() override;
	
	private:
		Ui::DialogCreateMaterial* _ui{};
	};
}

#endif
