#ifndef _DOCKWIDGETBASE_H_
#define _DOCKWIDGETBASE_H_

#include "moduleBaseAPI.h"
#include "ModuleBase.h"
#include <QDockWidget>

namespace GUI
{
	class MainWindow;
}

namespace ModuleBase
{
	class MODULEBASEAPI DockWidgetBase : public QDockWidget, public CModuleBase
	{
		Q_OBJECT
	public:
		DockWidgetBase(GUI::MainWindow* mainwindow, ModuleType type);
		~DockWidgetBase();
		
		void reTranslate() override;
		void repaintTitleBar();
	private:
		QWidget* _titleWidget{};
	};


}

#endif
