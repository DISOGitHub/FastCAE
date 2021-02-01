#ifndef DIALOGCREATEBOX_H_
#define DIALOGCREATEBOX_H_

#include "geometryWidgetsAPI.h"
#include "geoDialogBase.h"

namespace Ui
{
	class CreateBox;
}

namespace GeometryWidget
{
	class GeoPointWidget;

	class GEOMETRYWIDGETSAPI CreateBoxDialog : public GeoDialogBase
	{
		Q_OBJECT
	public:
		CreateBoxDialog(GUI::MainWindow* m, MainWidget::PreWindow* p);
		CreateBoxDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set);
		~CreateBoxDialog();
	
	private:
		void init();
//		void closeEvent(QCloseEvent *);
		void reject() override;
		void accept() override;

		bool getPara(double* p);

	private:
		Ui::CreateBox* _ui{};
		GeoPointWidget* _pw{};
	
	};
}



#endif