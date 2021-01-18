#ifndef  DIALOG_MAKEFILLHOLE_H_
#define DIALOG_MAKEFILLHOLE_H_

#include "geoDialogBase.h"
#include <QMultiHash>

//class vtkActor;
class TopoDS_Shape;

namespace Ui
{
	class FillHoleDialog;
}

namespace Geometry
{
	class GeometrySet;
}

namespace GeometryWidget
{
	class  GEOMETRYWIDGETSAPI MakeFillHoleDialog : public GeoDialogBase
	{
		Q_OBJECT
	public:
		MakeFillHoleDialog(GUI::MainWindow* m, MainWidget::PreWindow* pre);
		MakeFillHoleDialog(GUI::MainWindow* m, MainWidget::PreWindow* pre, Geometry::GeometrySet* set);
		~MakeFillHoleDialog();

	private slots:
		void on_geoSelectSurface_1_clicked();
		void shapeSlected(Geometry::GeometrySet* set, int shape) override;

	private:
//		void closeEvent(QCloseEvent *) override;
		void reject() override;
		void accept() override;

	private:
		Ui::FillHoleDialog* _ui{};

		QMultiHash<Geometry::GeometrySet*, int> _shapeHash{};
	};

}

#endif