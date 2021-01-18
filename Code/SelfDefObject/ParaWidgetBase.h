#ifndef _PARAWIDGETBASE_H_
#define _PARAWIDGETBASE_H_

#include <QWidget>
#include "SelfDefWidget.h"

namespace GUI
{
	class MainWindow;
}

namespace SelfDefObj
{
	class SELFDEFINEOBJAPI ParaWidgetBase : public SelfDefWidget
	{
		Q_OBJECT
	public:
		ParaWidgetBase(GUI::MainWindow* m, DataProperty::DataBase *data, bool isNew = false, QWidget* parent = 0, Qt::WindowFlags f = 0);
		~ParaWidgetBase();

	signals:
		void lockGraphFocus(bool b);

	protected:
		void focusInEvent(QFocusEvent *e) override;
		void focusOutEvent(QFocusEvent *e) override;
		
		virtual void apply() override;


	};
}


#endif