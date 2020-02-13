#ifndef  _DIALOGMAKEREVOL_H_
#define _DIALOGMAKEREVOL_H_

#include "moduleBase/ModuleType.h"
#include "geoDialogBase.h"
#include <QMultiHash>
#include "geoPointWidget.h"


class vtkActor;
class TopoDS_Shape;


namespace Ui
{
	class CreateRevol;
}
namespace Geometry
{
	class GeometrySet;
}
namespace GeometryWidget
{
	class GEOMETRYWIDGETSAPI CreateRevolDialog : public GeoDialogBase
	{
		Q_OBJECT
	public:
		CreateRevolDialog(GUI::MainWindow* m, MainWidget::PreWindow* p);
		CreateRevolDialog(GUI::MainWindow* m, MainWidget::PreWindow* pre, Geometry::GeometrySet* set);
		~CreateRevolDialog();

		
	private slots:

		void selectActorShape(vtkActor* ac, int shape, Geometry::GeometrySet* set) override;
		void on_geoSelectCurve_clicked();
		void on_geoSelectCurve_1_clicked();
		void on_radioButtonUser();
		void pointWidgetClicked(GeoPointWidget* w);
		void on_TypeChanged(int index);
	private:
		void init();
		void closeEvent(QCloseEvent *);
		void accept() override;
		void reject() override;
	//	bool getVector(double* vec);


	private:
		Ui::CreateRevol* _ui{};
		GeoPointWidget*  _baseWidget{};

		bool _selectAxisEdge{ false };
		bool _selectEdge{ false };

		QMultiHash<Geometry::GeometrySet*, int> _shapeHash{};
		QList<vtkActor*> _edgeActors{};

		vtkActor* _axisActor{};
		int _axisIndex{ -1 };
		Geometry::GeometrySet* _axisSet{};




	};



}
















































#endif 