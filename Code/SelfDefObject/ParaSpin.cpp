#include "ParaSpin.h"
#include "DataProperty/ParameterInt.h"
#include "mainWindow/mainWindow.h"
#include <QWheelEvent>

namespace SelfDefObj
{
	ParaSpin::ParaSpin(GUI::MainWindow* m, DataProperty::ParameterBase* data)
		: SelfDefParaWidgetBase(data)
	{
		connect(this, SIGNAL(lockGraphFocus(bool)), m, SIGNAL(enableGraphWindowKeyBoard(bool)));
		connect(this, SIGNAL(valueChanged(int)), this, SLOT(valueChanged()));
		_data = (DataProperty::ParameterInt*)data;
		int v = _data->getValue();
		int range[2] = { 0, 0 };
		_data->getRange(range);
		this->setRange(range[0], range[1]);
		this->setValue(v);
		this->setSingleStep(1);
		QString suffix = _data->getUnit();
		if (!suffix.isEmpty())
			this->setSuffix(" " + suffix);

		_inited = true;
	}

	void ParaSpin::focusInEvent(QFocusEvent *event)
	{
		emit lockGraphFocus(false);
		QSpinBox::focusInEvent(event);
	}

	void ParaSpin::focusOutEvent(QFocusEvent *event)
	{
// 		int v = this->value();
// 		_data->setValue(v);
// 		emit lockGraphFocus(true);
		QSpinBox::focusOutEvent(event);
	}
	void ParaSpin::valueChanged()
	{
// 		int v = this->value();
// 		_data->setValue(v);
		submitCode();
		emit dataChanged(_data);
	}

	void ParaSpin::wheelEvent(QWheelEvent *event)
	{
		event->ignore();
	}

	QString ParaSpin::generateCode()
	{
		QString code = SelfDefParaWidgetBase::generateCode();
		QString t = QString::number(this->value());
		code.replace("%Value%", t);
		return code;
	}

}