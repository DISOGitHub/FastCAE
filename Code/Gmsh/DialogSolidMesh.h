#ifndef DIALOGSOLIDMESH_H_
#define DIALOGSOLIDMESH_H_

#include "GmshDialogBase.h"
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
	class SolidMeshDialog : public GmshDialogBase
	{
		Q_OBJECT
	public:
		SolidMeshDialog(GUI::MainWindow* m, MainWidget::PreWindow* pre);
		~SolidMeshDialog();


	private slots:
		void on_geoSelectSurface_clicked();
		void selectActorShape(vtkActor*, int, Geometry::GeometrySet*) override;
		void on_localButton_clicked();

	private:
		void closeEvent(QCloseEvent *) override;
		void accept() override;
		void reject() override;

	private:
		Ui::SolidMeshDialog* _ui{};
		QList<Geometry::GeometrySet*> _geosetList{};

		bool _selectBody{ false };
	};
}


#endif