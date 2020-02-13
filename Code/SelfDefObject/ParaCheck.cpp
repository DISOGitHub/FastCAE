#include "ParaCheck.h"
#include "DataProperty/ParameterBool.h"
#include "mainWindow/mainWindow.h"
#include "MainWidgets/PropertyTable.h"
#include <QDebug>

namespace SelfDefObj
{
	ParaCheck::ParaCheck(GUI::MainWindow* m, DataProperty::ParameterBase* data) :SelfDefParaWidgetBase(data)
//		: ParameterWidgetBase(m, data)
	{
		connect(this, SIGNAL(lockGraphFocus(bool)), m, SIGNAL(enableGraphWindowKeyBoard(bool)));
		connect(this, SIGNAL(stateChanged(int)), this, SLOT(stateChanged()));
		
		blockSignals(true);
		_data = (DataProperty::ParameterBool*)data;
		bool b = _data->getValue();
		this->setChecked(b);
		blockSignals(false);

		_inited = true;
	}

	void ParaCheck::focusInEvent(QFocusEvent *e)
	{
		emit lockGraphFocus(false);
		QCheckBox::focusInEvent(e);
	}

	void ParaCheck::focusOutEvent(QFocusEvent *e)
	{
		bool b = this->isChecked();
		_data->setValue(b);
		emit lockGraphFocus(true);
		QCheckBox::focusOutEvent(e);
	}

	void ParaCheck::stateChanged()
	{
//		bool b = this->isChecked();
		submitCode();
// 		_data->setValue(b);
// 		qDebug() << _data->genAbsoluteName();
		emit dataChanged(_data);
	}

	QString ParaCheck::generateCode()
	{
		QString value = "false";
		if (this->isChecked())
			value = "true";
		QString code = SelfDefParaWidgetBase::generateCode();
		code.replace("%Value%", value);
		return code;
	}

}