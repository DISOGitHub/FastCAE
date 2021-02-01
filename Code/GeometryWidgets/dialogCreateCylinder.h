#ifndef _CREATECYLINDER_H_
#define _CREATECYLINDER_H_

#include "geometryWidgetsAPI.h"
#include "geoDialogBase.h"


namespace Ui
{
	class CreateCylinder;
}

namespace GeometryWidget
{

	class GeoPointWidget;

	class GEOMETRYWIDGETSAPI CreateCylinderDialog : public GeoDialogBase
	{
		Q_OBJECT
	public:
		CreateCylinderDialog(GUI::MainWindow* m, MainWidget::PreWindow* p);
		CreateCylinderDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set);
		~CreateCylinderDialog();


	private:
		void init();
//		void closeEvent(QCloseEvent *);
		void reject() override;
		void accept() override;

	private slots:
	    void on_radioButtonUser();


	private:
		Ui::CreateCylinder* _ui{};
		GeoPointWidget* _pw{};

	};


}


#endif