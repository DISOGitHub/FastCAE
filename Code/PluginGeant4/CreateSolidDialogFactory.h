#ifndef _CREATESOLIDDIALOGFACTORY_H
#define _CREATESOLIDDIALOGFACTORY_H

#include "SolidShapeBase.h"
class QDialog;

namespace Geant4 {
	class SolidShapeBase;

	class CreateSolidDialogFactory
	{
	public:
		static QDialog* editSolidDialog(SolidShapeBase* data);
	};
}
#endif // _CREATESOLIDDIALOGFACTORY_H


