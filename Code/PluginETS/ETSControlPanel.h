#ifndef ETSCONTROLPANEL_H_
#define ETSCONTROLPANEL_H_

#include "moduleBase/dockWidgetBase.h"

namespace Ui
{
	class ETSControlPanel;
}

namespace ETS
{
	class ETSControlPanel : public ModuleBase::DockWidgetBase
	{
		Q_OBJECT
	public:
		ETSControlPanel(GUI::MainWindow*);
		~ETSControlPanel();


	private:
		Ui::ETSControlPanel* _ui{};
		
	};
}


#endif