#ifndef DIALOGSOLIDMESH_H_
#define DIALOGSOLIDMESH_H_

#include "GeometryWidgets/geoDialogBase.h"
#include <QList>

namespace Ui
{
	class SolidMeshDialog;
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
	class SolidMeshDialog : public GeometryWidget::GeoDialogBase
	{
		Q_OBJECT
	public:
		SolidMeshDialog(GUI::MainWindow* m, MainWidget::PreWindow* pre);
		~SolidMeshDialog();

	private slots:
		void on_geoSelectSurface_clicked();
		void selectActorShape(vtkActor*, int, Geometry::GeometrySet*) override;

	private:
		void closeEvent(QCloseEvent *) override;
		void accept() override;
		void reject() override;

	private:
		Ui::SolidMeshDialog* _ui{};
		Py::PythonAagent* _pyAgent{};

		QList<Geometry::GeometrySet*> _geosetList{};
	};
}


#endif