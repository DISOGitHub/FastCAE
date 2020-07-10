#ifndef DIALOGSOLIDMESH_H_
#define DIALOGSOLIDMESH_H_

#include "GmshDialogBase.h"
#include <QMultiHash>

namespace Ui
{
	class SolidMeshDialog;
}

namespace Geometry
{
	class GeometrySet;
	class GeometryData;
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
//		void selectActorShape(vtkActor*, int, Geometry::GeometrySet*) override;
		void shapeSlected(Geometry::GeometrySet* set, int index);
		void on_localButton_clicked();
		void addLocalPoints();
		void addLocalFields();
		//void on_physicalsPButton_clicked();
		void on_selectall_clicked();
		void on_selectvisible_clicked();

	private:
		void closeEvent(QCloseEvent *) override;
		void accept() override;
		void reject() override;

	private:
		Ui::SolidMeshDialog* _ui{};
		QMultiHash<Geometry::GeometrySet*, int> _solidHash{};

		Geometry::GeometryData* _geoData;

		bool _selectBody{ false };
	};
}


#endif