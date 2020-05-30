#ifndef DIALOGDURFACEMESH_H__
#define DIALOGDURFACEMESH_H__

#include "GmshDialogBase.h"
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
	class SurfaceMeshDialog : public GmshDialogBase
	{
		Q_OBJECT
	public:
		SurfaceMeshDialog(GUI::MainWindow* m, MainWidget::PreWindow* pre);
		~SurfaceMeshDialog();

	private slots:
	    void on_geoSelectSurface_clicked();
		void selectActorShape(vtkActor*, int, Geometry::GeometrySet*) override;
		void on_localButton_clicked();

	private:
		void closeEvent(QCloseEvent *) override;
		void accept() override;
		void reject() override;

	private:
		Ui::SurfaceMeshDialog* _ui{};

		QList<vtkActor*> _actorList{};
		QMultiHash<Geometry::GeometrySet*, int> _geoHash{};
		bool _selectFace{ false };
	};

}


#endif