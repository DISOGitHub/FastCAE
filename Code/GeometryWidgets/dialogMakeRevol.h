#ifndef  _DIALOGMAKEREVOL_H_
#define _DIALOGMAKEREVOL_H_

#include "moduleBase/ModuleType.h"
#include "geoDialogBase.h"
#include <QMultiHash>
#include "geoPointWidget.h"


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
		void shapeSlected(Geometry::GeometrySet* set, int index) override;
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

	private:
		Ui::CreateRevol* _ui{};
		GeoPointWidget*  _baseWidget{};

		bool _selectAxisEdge{ false };
		bool _selectEdge{ false };

		QMultiHash<Geometry::GeometrySet*, int> _shapeHash{};
		int _axisIndex{ -1 };
		Geometry::GeometrySet* _axisSet{};


	};



}
















































#endif 