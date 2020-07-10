#ifndef  _DIALOGCREATEDATUMPLANE_H_
#define _DIALOGCREATEDATUMPLANE_H_

#include "moduleBase/ModuleType.h"
#include "geoDialogBase.h"
#include <QMultiHash>

//class vtkActor;
class TopoDS_Shape;

namespace Ui
{
	class CreateDatumplaneDialog;
}
namespace Geometry
{
	class GeometrySet;
}
namespace GeometryWidget
{
	class GEOMETRYWIDGETSAPI CreateDatumplaneDialog : public GeoDialogBase
	{
		Q_OBJECT
	public:
		CreateDatumplaneDialog(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~CreateDatumplaneDialog();


	private slots:
		void shapeSlected(Geometry::GeometrySet* set, int shape) override;
		void on_geoSelectSurface_clicked();
	
	private:

		void closeEvent(QCloseEvent *);
		void accept() override;
		void reject() override;
		bool getDirection(double* basePt, double* dir);

	private:
		Ui::CreateDatumplaneDialog* _ui{};

//		vtkActor* _faceActor{};
		int _faceIndex{ 0 };
		Geometry::GeometrySet* _faceBody{};

		double _distance{1.0};






	};



}
















































#endif 