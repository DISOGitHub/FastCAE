#include "CreateSolidDialogFactory.h"
#include "dialogBox.h"
#include "dialogSphere.h"
#include "dialogBoolean.h"
#include "dialogTube.h"

namespace Geant4 {
	QDialog* CreateSolidDialogFactory::editSolidDialog(SolidShapeBase* data)
	{
		SolidType type = data->getType();
		QDialog* dlg = nullptr;
		switch (type)
		{
		case SolidType::SolidBox:
			dlg = new DialogBox(data);break;
		case SolidType::SolidSphere:
			dlg = new DialogSphere(data);break;
		case SolidType::SolidTube:
			dlg = new DialogTube(data); break;
		case SolidType::SolidUnionBoolean:
		case SolidType::SolidSubtractionBoolean:
		case SolidType::SolidIntersectionBoolean:
			dlg = new DialogBoolean(data); break;
		default:
			break;
		}
		return dlg;
	}
}