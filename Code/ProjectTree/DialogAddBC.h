#ifndef _DIALOGADDBC_H_
#define _DIALOGADDBC_H_

#include "SelfDefObject/QFDialog.h"

namespace Ui
{
	class AddBCDialog;
}
namespace GUI
{
	class MainWindow;
}
namespace ModelData
{
	class ModelDataBaseExtend;
}

namespace ProjectTree
{

	class AddBCDialog : public QFDialog
	{
		Q_OBJECT
	public:
		AddBCDialog(GUI::MainWindow* m, ModelData::ModelDataBaseExtend* data);
		~AddBCDialog();

	private:
		void init();

		virtual void initBCType();
		virtual void initComponents();
		virtual void accept() override;

	private:
		Ui::AddBCDialog* _ui{};
		ModelData::ModelDataBaseExtend* _data{};


	};

}



#endif