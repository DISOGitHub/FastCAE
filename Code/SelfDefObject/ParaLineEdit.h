#ifndef _PARALINEEDIT_H_
#define _PARALINEEDIT_H_

#include "SelfDefObjectAPI.h"
#include "SelfDefParaWidgetBase.h"
#include <QLineEdit>

namespace GUI
{
	class MainWindow;
}
namespace DataProperty
{
	class ParameterBase;
	class ParameterString;
}

namespace SelfDefObj
{
	class SELFDEFINEOBJAPI ParaLineEdit : public QLineEdit, public SelfDefParaWidgetBase
	{
		Q_OBJECT
	public:
		ParaLineEdit(GUI::MainWindow* m, DataProperty::ParameterBase* data);
		~ParaLineEdit() = default;

		void focusInEvent(QFocusEvent *e) override;
		void focusOutEvent(QFocusEvent *e) override;

	signals:
		void lockGraphFocus(bool);
		void dataChanged(DataProperty::ParameterBase* p);

	private slots:
		void textChanged();

	private:
		QString generateCode() override;

	private:
		DataProperty::ParameterString* _data{};

	};
}


#endif