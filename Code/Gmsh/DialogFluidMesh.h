#ifndef DIALOGFLUIDMESH_H
#define DIALOGFLUIDMESH_H

#include "GmshDialogBase.h"
#include <QDialog>

namespace Ui {
class DialogFluidMesh;
}

namespace GUI{
	class MainWindow;
}

namespace MainWidget{
	class PreWindow;
}

namespace Geometry{
	class GeometrySet;
	class GeometryData;
}

namespace GeometryWidget{
	class GeoPointWidget;
}

namespace Gmsh{
	class DialogFluidMesh : public GmshDialogBase
	{
		Q_OBJECT

	public:
		DialogFluidMesh(GUI::MainWindow* m, MainWidget::PreWindow* pre, int index = -1);
		~DialogFluidMesh();

	private slots:
		void on_geoSelectSurface_clicked();
		void shapeSlected(Geometry::GeometrySet* set, int index);
		void pointWidgetClicked(GeometryWidget::GeoPointWidget*);

	private:
		void closeEvent(QCloseEvent *) override;
		void accept() override;
		void reject() override;
		bool fluidFieldIsSrue();

	private:
		Ui::DialogFluidMesh* _ui;
		QMultiHash<Geometry::GeometrySet*, int> _solidHash{};
		Geometry::GeometryData* _geoData;
		bool _selectBody{ false };
		GeometryWidget::GeoPointWidget* _minPoint{};
		GeometryWidget::GeoPointWidget* _maxPoint{};
	};
}
#endif // DIALOGFLUIDMESH_H
