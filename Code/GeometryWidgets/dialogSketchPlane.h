#ifndef SKETCH_PLANE_DIALOG
#define SKETCH_PLANE_DIALOG

#include "geoDialogBase.h"

namespace Ui
{
	class SketchPlaneDialog;
}

namespace Geometry
{
	class GeometrySet;
}

namespace GeometryWidget
{
	class GEOMETRYWIDGETSAPI SketchPlanDialog : public GeoDialogBase
	{
		Q_OBJECT
	public:
		SketchPlanDialog(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~SketchPlanDialog();

	signals:
		void startSketch(bool, double*, double*);

	private slots:
	     void selecPlaneClicked();
		 void on_geoSelectSurface_clicked();
		 void shapeSlected(Geometry::GeometrySet* set, int index) override;

	private:
//		void closeEvent(QCloseEvent *) override;
		void reject() override;
		void accept() override;
		bool getDirection(double* loc, double* dir);

	private:
		Ui::SketchPlaneDialog* _ui{};

		Geometry::GeometrySet* _faceBody{};
		int _faceIndex{ 0 };
		
	};
}



#endif
