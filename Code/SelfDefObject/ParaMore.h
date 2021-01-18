#ifndef _PARAMORE_H_
#define _PARAMORE_H_

#include "SelfDefObjectAPI.h"
#include <QWidget>

namespace GUI
{
	class MainWindow;
}
namespace DataProperty
{
	class ParameterBase;
}
namespace Ui
{
	class ParaMore;
}

namespace SelfDefObj
{

	class SELFDEFINEOBJAPI ParaMore : public QWidget 
	{
		Q_OBJECT
	public:
		ParaMore(GUI::MainWindow* m, DataProperty::ParameterBase* data);
		~ParaMore();

	signals:
		void lockGraphFocus(bool b);

	protected:
		void focusInEvent(QFocusEvent *e);
		void focusOutEvent(QFocusEvent *e);
		virtual void updateLineEdit() = 0;
		void setText(QString text);

	protected slots:
		virtual void on_moreButton_clicked();

	protected:
		Ui::ParaMore* _ui{};
		DataProperty::ParameterBase* _data{};

	};

}


#endif