#ifndef DIALOGINPUTDATA_H_
#define  DIALOGINPUTDATA_H_

#include "HullPluginAPI.h"
#include "GeometryWidgets/geoDialogBase.h"

namespace Ui
{
	class InputDataDialog;
}
namespace PluginShip
{

	class HULLPLUGINAPI InputDataDialog : public QDialog
	{
		Q_OBJECT
	public:
		InputDataDialog();
		~InputDataDialog();
	signals:
		void sendAxis(double, double);
	private:
		void accept() override;
	
 
	private:
		Ui::InputDataDialog* _ui{};
		
	};
}



#endif