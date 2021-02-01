#ifndef _PARASPIN_H_
#define _PARASPIN_H_

#include "SelfDefObjectAPI.h"
#include "SelfDefParaWidgetBase.h"
#include <QSpinBox>

namespace GUI
{
	class MainWindow;
}
namespace DataProperty
{
	class ParameterBase;
	class ParameterInt;
}

namespace SelfDefObj
{
	class SELFDEFINEOBJAPI ParaSpin : public QSpinBox, public SelfDefParaWidgetBase
	{
		Q_OBJECT
	public:
		ParaSpin(GUI::MainWindow* m, DataProperty::ParameterBase* data);
		~ParaSpin() = default;

		void focusInEvent(QFocusEvent *event) override;
		void focusOutEvent(QFocusEvent *event) override;

	signals:
		void lockGraphFocus(bool b);
		void dataChanged(DataProperty::ParameterBase* p);

	private slots:
		void valueChanged();

	private:
		void wheelEvent(QWheelEvent *event) override;
		QString generateCode() override;

	private:
		DataProperty::ParameterInt* _data{};

	};
}


#endif