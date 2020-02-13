#ifndef DIALOG_CREATECHAMFER_H_
#define DIALOG_CREATECHAMFER_H_

#include "geoDialogBase.h"
#include <QMultiHash>

class vtkActor;
class TopoDS_Shape;

namespace Ui
{
	class CreateChamferDialog;
}

namespace Geometry
{
	class GeometrySet;
}

namespace GeometryWidget
{
	class  GEOMETRYWIDGETSAPI CreateChamferDialog : public GeoDialogBase
	{
		Q_OBJECT
	public:
		CreateChamferDialog(GUI::MainWindow* m, MainWidget::PreWindow* pre);
		CreateChamferDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set);
		~CreateChamferDialog();

	private slots:
	    void on_geoSelectCurve_clicked();
		void selectActorShape(vtkActor* ac, int shape, Geometry::GeometrySet* set) override;
		void on_sectionComboxChanged(int index);

	private:
		void init();
		void closeEvent(QCloseEvent *) override;
		void reject() override;
		void accept() override;


	private:
		Ui::CreateChamferDialog* _ui{};

		QList<vtkActor*> _actors{};
		QMultiHash<Geometry::GeometrySet*, int> _shapeHash{};
	};
	
}

#endif