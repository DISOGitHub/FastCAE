#ifndef _PARATABLE_H_
#define _PARATABLE_H_

#include "ParaMore.h"

namespace GUI
{
	class MainWindow;
}
namespace DataProperty
{
	class ParameterBase;
	class ParameterTable;
}

namespace SelfDefObj
{
	class SELFDEFINEOBJAPI ParaTable : public ParaMore
	{
		Q_OBJECT
	public:
		ParaTable(GUI::MainWindow* m, DataProperty::ParameterBase* data);
		~ParaTable() = default;

	signals:
		void dataChanged(DataProperty::ParameterBase* p);

	protected slots:
		void on_moreButton_clicked() override;

	protected:
		void updateLineEdit() override;

	private:
		DataProperty::ParameterTable* _data{};
		GUI::MainWindow* _mainWindow{};
	};

}

#endif