#ifndef _PARADOUBLESPIN_H_
#define _PARADOUBLESPIN_H_

#include "SelfDefObjectAPI.h"
#include "SelfDefParaWidgetBase.h"
#include <QDoubleSpinBox>

namespace GUI
{
	class MainWindow;
}
namespace DataProperty
{
	class ParameterBase;
	class ParameterDouble;
}

namespace SelfDefObj
{
	class SELFDEFINEOBJAPI ParaDoubleSpin : public QDoubleSpinBox, public SelfDefParaWidgetBase
	{
		Q_OBJECT

	public:
		ParaDoubleSpin(GUI::MainWindow* m, DataProperty::ParameterBase* data);
		~ParaDoubleSpin();
		
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
		DataProperty::ParameterDouble* _data{};

	
	};
}


#endif
