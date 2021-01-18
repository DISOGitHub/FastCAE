#ifndef _PARACOMBOX_H_
#define _PARACOMBOX_H_

#include "SelfDefObjectAPI.h"
#include "SelfDefParaWidgetBase.h"
#include <QComboBox>

namespace GUI
{
	class MainWindow;
}

namespace DataProperty
{
	class ParameterBase;
	class ParameterSelectable;
}

namespace SelfDefObj
{
	class SELFDEFINEOBJAPI ParaCombox : public QComboBox, public SelfDefParaWidgetBase
	{
		Q_OBJECT
	public:
		ParaCombox(GUI::MainWindow* m, DataProperty::ParameterBase* data);
		~ParaCombox() = default;

		void focusInEvent(QFocusEvent *e) override;
		void focusOutEvent(QFocusEvent *e) override;

	signals:
		void lockGraphFocus(bool b);
		void dataChanged(DataProperty::ParameterBase* p);

	private slots:
		void valueChanged();

	private:
		QString generateCode() override;

	private:
		DataProperty::ParameterSelectable* _data{};

	};

}

#endif