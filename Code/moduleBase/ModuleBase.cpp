#include "ModuleBase.h"
#include "mainWindow/mainWindow.h"

namespace ModuleBase
{
	CModuleBase::CModuleBase(GUI::MainWindow* mainwindow, ModuleType type) : _moduleType(type), _mainWindow(mainwindow)
	{
	}
	CModuleBase::~CModuleBase()
	{

	}
	const ModuleType CModuleBase::getModuleType()
	{
		return _moduleType;
	}
	void CModuleBase::reTranslate()
	{

	}
}