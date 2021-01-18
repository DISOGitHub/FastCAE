/**********************************
进度框
***********************************/
#ifndef _PROCESSWINDOWBASE_H_
#define _PROCESSWINDOWBASE_H_

#include "moduleBaseAPI.h"
#include "dockWidgetBase.h"
#include <QList>

namespace GUI
{
class MainWindow;
}

namespace Ui
{
class ProcessWindow;
}

namespace ModuleBase
{

//	class ProcessBar;

class MODULEBASEAPI ProcessWindowBase : public DockWidgetBase //public QDockWidget, public CModuleBase
{
	Q_OBJECT
public:
	ProcessWindowBase(GUI::MainWindow* mainwindow, QWidget* parent = 0);
	virtual ~ProcessWindowBase();
	//翻译
	virtual void reTranslate() override;
	
private slots:
    //添加进程
	void addProcess(QWidget* w);
	//移除进程
	void removeWidget(QWidget* w);

protected:

protected:
	Ui::ProcessWindow* _ui{};
	QList<QWidget*> _processList{};

};


}


#endif
 
