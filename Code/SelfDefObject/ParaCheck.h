#ifndef _PARACHECK_H_
#define _PARACHECK_H_

#include "SelfDefObjectAPI.h"
#include "SelfDefParaWidgetBase.h"
#include <QCheckBox>

namespace GUI
{
	class MainWindow;
}
namespace DataProperty
{
	class ParameterBase;
	class ParameterBool;
}


namespace SelfDefObj
{
	class SELFDEFINEOBJAPI ParaCheck : public QCheckBox, public SelfDefParaWidgetBase 
	{
		Q_OBJECT
	
	public:
		ParaCheck(GUI::MainWindow* m, DataProperty::ParameterBase* data);
		~ParaCheck() = default;

		void focusInEvent(QFocusEvent *e) override;
		void focusOutEvent(QFocusEvent *e) override;

	signals:
		void lockGraphFocus(bool b);
		void dataChanged(DataProperty::ParameterBase* p);

	private slots:
		void stateChanged();

	private:
		QString generateCode() override;

	private:
		DataProperty::ParameterBool* _data{};
	
	};
 }



#endif