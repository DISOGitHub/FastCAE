#include "ParaWidgetBase.h"
#include "mainWindow/mainWindow.h"

namespace SelfDefObj
{
	ParaWidgetBase::ParaWidgetBase(GUI::MainWindow* m, DataProperty::DataBase *data, bool isNew /* = false */, QWidget* parent /* = 0 */, Qt::WindowFlags f /* = 0 */)
		:SelfDefWidget(data, isNew, parent, f)
	{
		connect(this, SIGNAL(lockGraphFocus(bool)), m, SIGNAL(enableGraphWindowKeyBoard(bool)));
		emit lockGraphFocus(false);
		this->setFocusPolicy(Qt::ClickFocus);
	}
	ParaWidgetBase::~ParaWidgetBase()
	{
		apply();
//		emit lockGraphFocus(true);
	}

	void ParaWidgetBase::focusInEvent(QFocusEvent *e)
	{
		emit lockGraphFocus(false);
		QWidget::focusInEvent(e);
	}
	
	void ParaWidgetBase::focusOutEvent(QFocusEvent *e)
	{
//		emit lockGraphFocus(true);
		this-> apply();
		QWidget::focusOutEvent(e);
	}
	void ParaWidgetBase::apply()
	{

	}

}