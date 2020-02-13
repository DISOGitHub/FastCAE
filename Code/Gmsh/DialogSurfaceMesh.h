#ifndef DIALOGDURFACEMESH_H__
#define DIALOGDURFACEMESH_H__

#include "GeometryWidgets/geoDialogBase.h"
#include <QList>
#include <QMultiHash>

namespace Ui
{
	class SurfaceMeshDialog;
}

namespace Geometry
{
	class GeometrySet;
}

namespace Py
{
	class PythonAagent;
}

class vtkActor;

namespace Gmsh
{
	class SurfaceMeshDialog : public GeometryWidget::GeoDialogBase
	{
		Q_OBJECT
	public:
		SurfaceMeshDialog(GUI::MainWindow* m, MainWidget::PreWindow* pre);
		~SurfaceMeshDialog();

	private slots:
	    void on_geoSelectSurface_clicked();
		void selectActorShape(vtkActor*, int, Geometry::GeometrySet*) override;

	private:
		void closeEvent(QCloseEvent *) override;
		void accept() override;
		void reject() override;

	private:
		Ui::SurfaceMeshDialog* _ui{};
		Py::PythonAagent* _pyAgent{};

		QList<vtkActor*> _actorList{};
		QMultiHash<Geometry::GeometrySet*, int> _geoHash{};
	};

}


#endif