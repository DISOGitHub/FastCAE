#ifndef _PARAWIDGETFACTORY_H_
#define _PARAWIDGETFACTORY_H_

#include "SelfDefObjectAPI.h"

class QWidget;
namespace DataProperty
{
	class ParameterBase;
}

namespace GUI
{
	class MainWindow;
}

namespace SelfDefObj
{
	class SELFDEFINEOBJAPI ParaWidgetFactory
	{
	public:
		ParaWidgetFactory() = default;
		~ParaWidgetFactory() = default;

		static QWidget* generateWidgetByPara(GUI::MainWindow* m, DataProperty::ParameterBase* p);
	};
}



#endif