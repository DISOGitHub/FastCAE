#include "ParaLineEdit.h"
#include "DataProperty/ParameterString.h"
#include "mainWindow/mainWindow.h"

namespace SelfDefObj
{

	ParaLineEdit::ParaLineEdit(GUI::MainWindow* m, DataProperty::ParameterBase* data)
		: SelfDefParaWidgetBase(data)
	{
		connect(this, SIGNAL(lockGraphFocus(bool)), m, SIGNAL(enableGraphWindowKeyBoard(bool)));
		connect(this, SIGNAL(textChanged(QString)), this, SLOT(textChanged()));
		_data = (DataProperty::ParameterString*)data;
		
		QString t = _data->getValue();
		this->setText(t);

		_inited = true;
	}

	void ParaLineEdit::focusInEvent(QFocusEvent *e)
	{
		emit lockGraphFocus(false);
		QLineEdit::focusInEvent(e);
	}

	void ParaLineEdit::focusOutEvent(QFocusEvent *e)
	{
// 		QString t = this->text();
// 		_data->setValue(t);
// 		emit lockGraphFocus(true);
		QLineEdit::focusOutEvent(e);
	}

	void ParaLineEdit::textChanged()
	{
// 		QString t = this->text();
// 		_data->setValue(t);
		submitCode();
		emit dataChanged(_data);
	}

	QString ParaLineEdit::generateCode()
	{
		QString t = this->text();
		QString code = SelfDefParaWidgetBase::generateCode();
		code.replace("%Value%", t);
		return code;
	}

}