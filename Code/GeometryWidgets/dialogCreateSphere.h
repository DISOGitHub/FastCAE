#ifndef DIALOGCREATESPHERE_H_
#define DIALOGCREATESPHERE_H_

#include "geometryWidgetsAPI.h"
#include "geoDialogBase.h"

namespace Ui
{
	class CreateSphere;
}

namespace GeometryWidget
{
	class GeoPointWidget;

	class GEOMETRYWIDGETSAPI CreateSphereDialog :public GeoDialogBase
	{
		Q_OBJECT
	public:
		CreateSphereDialog(GUI::MainWindow* m, MainWidget::PreWindow* p);
		CreateSphereDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set);
		~CreateSphereDialog();
 
	private:
		void init();
//		void closeEvent(QCloseEvent *);
		void reject() override;
		void accept() override;

	private:
		Ui::CreateSphere* _ui{};
		GeoPointWidget* _pw{};
	};
}




#endif