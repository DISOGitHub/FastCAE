#include "ParaWidgetFactory.h"
#include "DataProperty/ParameterBase.h"
#include "ParaDoubleSpin.h"
#include "ParaSpin.h"
#include "ParaCheck.h"
#include "ParaCombox.h"
#include "ParaLineEdit.h"
#include "ParaPath.h"
#include "ParaTable.h"

namespace SelfDefObj
{
	QWidget* ParaWidgetFactory::generateWidgetByPara(GUI::MainWindow* mainWindow,  DataProperty::ParameterBase* p)
	{
		DataProperty::ParaType type = p->getParaType();
		QWidget* w = nullptr;
		switch (type)
		{
		case DataProperty::Para_Int: w = new SelfDefObj::ParaSpin(mainWindow, p); break;
		case DataProperty::Para_Double: w = new SelfDefObj::ParaDoubleSpin(mainWindow, p); break;
		case DataProperty::Para_Color:break;
		case DataProperty::Para_String: w = new SelfDefObj::ParaLineEdit(mainWindow, p); break;
		case DataProperty::Para_Bool: w = new SelfDefObj::ParaCheck(mainWindow, p); break;
		case DataProperty::Para_Selectable: w = new SelfDefObj::ParaCombox(mainWindow, p); break;
		case DataProperty::Para_Path: w = new SelfDefObj::ParaPath(mainWindow, p);  break;
		case DataProperty::Para_Table: w = new SelfDefObj::ParaTable(mainWindow, p); break;
		default:
			break;
		}
		if (w != nullptr)
		{
			bool editable = p->isEditable();
			w->setEnabled(editable);
		}
		return w;
	}
}