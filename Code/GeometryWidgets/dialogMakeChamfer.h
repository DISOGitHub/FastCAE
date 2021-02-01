#ifndef DIALOG_CREATECHAMFER_H_
#define DIALOG_CREATECHAMFER_H_

#include "geoDialogBase.h"
#include <QMultiHash>

//class vtkActor;
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
		void shapeSlected(Geometry::GeometrySet* set, int index) override;
		void on_sectionComboxChanged(int index);

	private:
		void init();
//		void closeEvent(QCloseEvent *) override;
		void reject() override;
		void accept() override;


	private:
		Ui::CreateChamferDialog* _ui{};
		QMultiHash<Geometry::GeometrySet*, int> _shapeHash{};
	};
	
}

#endif