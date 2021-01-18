#ifndef  DIALOG_MAKEREMOVESURFACE_H_
#define DIALOG_MAKEREMOVESURFACE_H_

#include "geoDialogBase.h"
#include <QMultiHash>

//class vtkActor;
class TopoDS_Shape;

namespace Ui
{
	class DialogRemoveSurface;
}

namespace Geometry
{
	class GeometrySet;
}

namespace GeometryWidget
{
	class  GEOMETRYWIDGETSAPI MakeRemoveSurfaceDialog : public GeoDialogBase
	{
		Q_OBJECT
	public:
		MakeRemoveSurfaceDialog(GUI::MainWindow* m, MainWidget::PreWindow* pre);
		MakeRemoveSurfaceDialog(GUI::MainWindow* m, MainWidget::PreWindow* pre, Geometry::GeometrySet* set);
		~MakeRemoveSurfaceDialog();

	private slots:
		void on_geoSelectSurface_1_clicked();
		void shapeSlected(Geometry::GeometrySet* set, int shape) override;

	private:
//		void closeEvent(QCloseEvent *) override;
		void reject() override;
		void accept() override;

	private:
		Ui::DialogRemoveSurface* _ui{};

		QMultiHash<Geometry::GeometrySet*, int> _shapeHash{};
	};

}

#endif