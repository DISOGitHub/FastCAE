#ifndef  DIALOG_CREATEFILET_H_
#define DIALOG_CREATEFILET_H_

#include "geoDialogBase.h"
#include <QMultiHash>

class vtkActor;
class TopoDS_Shape;

namespace Ui
{
	class CreateFilterDialog;
}

namespace Geometry
{
	class GeometrySet;
}

namespace GeometryWidget
{
	class  GEOMETRYWIDGETSAPI CreateFiletDialog : public GeoDialogBase
	{
		Q_OBJECT
	public:
		CreateFiletDialog(GUI::MainWindow* m, MainWidget::PreWindow* pre);
		CreateFiletDialog(GUI::MainWindow* m, MainWidget::PreWindow* pre, Geometry::GeometrySet* set);
		~CreateFiletDialog();

	private slots:
		void on_geoSelectCurve_clicked();
		void selectActorShape(vtkActor* ac, int shape, Geometry::GeometrySet* set) override;

	private:
		void closeEvent(QCloseEvent *) override;
		void reject() override;
		void accept() override;

	private:
		Ui::CreateFilterDialog* _ui{};

		QList<vtkActor*> _actors{};
		QMultiHash<Geometry::GeometrySet*, int> _shapeHash{};
	};

}

#endif