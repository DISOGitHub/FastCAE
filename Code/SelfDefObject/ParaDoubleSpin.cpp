#include "ParaDoubleSpin.h"
#include "DataProperty/ParameterDouble.h"
#include "mainWindow/mainWindow.h"
#include <QWheelEvent>

namespace SelfDefObj
{
	ParaDoubleSpin::ParaDoubleSpin(GUI::MainWindow* m, DataProperty::ParameterBase* data)
		: SelfDefParaWidgetBase(data)
	{
		connect(this, SIGNAL(lockGraphFocus(bool)), m, SIGNAL(enableGraphWindowKeyBoard(bool)));
		connect(this, SIGNAL(valueChanged(double)), this, SLOT(valueChanged()));

		_data = (DataProperty::ParameterDouble*)(data);

		double d = _data->getValue();
		double range[2] = { 0.0, 0.0 };
		double acc = _data->getAccuracy();
		double dacc = 1.0;
		for (int i = 0; i < acc; ++i)
			dacc /= 10.0;
		_data->getRange(range);
		this->setRange(range[0], range[1]);
		this->setDecimals(acc);
		this->setValue(d);
		this->setSingleStep(dacc);
		QString unit = _data->getUnit();
		if (!unit.isEmpty())
			this->setSuffix(" " + unit);

		_inited = true;
	}

	ParaDoubleSpin::~ParaDoubleSpin()
	{

	}
	void ParaDoubleSpin::focusInEvent(QFocusEvent *event)
	{
		emit lockGraphFocus(false);
		QDoubleSpinBox::focusInEvent(event);
	}
	void ParaDoubleSpin::focusOutEvent(QFocusEvent *event)
	{
		double v = this->value();
		_data->setValue(v);
		emit lockGraphFocus(true);
		QDoubleSpinBox::focusOutEvent(event);
	}
	void ParaDoubleSpin::valueChanged()
	{
// 		double v = this->value();
// 		_data->setValue(v);
		submitCode();
		emit dataChanged(_data);
	}

	void ParaDoubleSpin::wheelEvent(QWheelEvent *event)
	{
		event->ignore();
	}

	QString ParaDoubleSpin::generateCode()
	{
		QString v = QString::number(this->value());
		QString bc = SelfDefParaWidgetBase::generateCode();
		bc.replace("%Value%", v);
		return bc;
	}
}  