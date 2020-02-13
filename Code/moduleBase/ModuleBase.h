/****************************
模板基类
libaojun
************************** */
#ifndef _MODULEBASE_H_
#define _MODULEBASE_H_

#include "ModuleType.h"
#include "moduleBaseAPI.h"

class QTreeWidgetItem;

namespace GUI
{
	class MainWindow;
}

namespace ModuleBase
{
class MODULEBASEAPI CModuleBase 
{
//	Q_OBJECT
public:
	CModuleBase(GUI::MainWindow* mainwindow, ModuleType type = Undefined);
	/*子类必须重写析构函数，防止内存泄漏 */
	virtual ~CModuleBase() ;
	const ModuleType getModuleType();
	virtual void reTranslate();

protected:
	ModuleType _moduleType{ Undefined };
	GUI::MainWindow* _mainWindow{};

private:

};


}


#endif