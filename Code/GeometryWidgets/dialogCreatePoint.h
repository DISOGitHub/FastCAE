#ifndef DIALOGCREATEPOINT_H_
#define DIALOGCREATEPOINT_H_

#include "geoDialogBase.h"

namespace Ui
{
	class CreatePoint;
}

namespace GeometryWidget
{
	class GeoPointWidget;

	class GEOMETRYWIDGETSAPI CreatePointDialog : public GeoDialogBase
	{
		Q_OBJECT
	public:
		CreatePointDialog(GUI::MainWindow* m, MainWidget::PreWindow* p);
		CreatePointDialog(GUI::MainWindow* m, MainWidget::PreWindow* p,Geometry::GeometrySet* set);
		~CreatePointDialog();

	private:
		void init();

//		void closeEvent(QCloseEvent *);
		void reject() override;
		void accept() override;

		bool getPara(double* p);

	private:
		Ui::CreatePoint* _ui{};
		GeoPointWidget* _pw{};
	};


}



#endif