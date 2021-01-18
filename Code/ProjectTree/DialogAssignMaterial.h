#ifndef _DIALOGASSIGNMATERIAL_H_
#define _DIALOGASSIGNMATERIAL_H_

#include "moduleBase/componentDialogBase.h"

namespace GUI
{
	class MainWindow;
}
namespace ModelData
{
	class ModelDataBaseExtend;
}
class QHBoxLayout;
class QLabel;
class QComboBox;

namespace ProjectTree
{
	class AssignMaterialDialog: public ModuleBase::ComponentSelectDialogBase
	{
	public:
		AssignMaterialDialog(GUI::MainWindow* mainwindow, ModelData::ModelDataBaseExtend* data);
		~AssignMaterialDialog();
	
	private:
		void init() override;
		void accept() override;


	private:
		ModelData::ModelDataBaseExtend* _data{};
		QHBoxLayout* _layout{};
		QLabel* _label{};
		QComboBox* _combobox{};
	};

}


#endif