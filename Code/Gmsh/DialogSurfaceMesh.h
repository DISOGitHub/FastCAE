#ifndef DIALOGDURFACEMESH_H__
#define DIALOGDURFACEMESH_H__

#include "GmshDialogBase.h"
/*#include "GmshModuleAPI.h"*/
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

namespace DataProperty
{
	class DataBase;
}

namespace MeshData{
	class MeshKernal;
	class MeshData;
}

namespace Py
{
	class PythonAagent;
}

//class vtkActor;

namespace Gmsh
{
	class GmshSettingData;

	class GMSHAPI SurfaceMeshDialog : public GmshDialogBase
	{
		Q_OBJECT
	public:
		SurfaceMeshDialog(GUI::MainWindow* m, MainWidget::PreWindow* pre, int index = -1);
		~SurfaceMeshDialog();

	private slots:
	    void on_geoSelectSurface_clicked();
//		void selectActorShape(vtkActor*, int, Geometry::GeometrySet*) override;
		void shapeSlected(Geometry::GeometrySet* set, int index) override;
		void on_localButton_clicked();
		//void on_physicalsPButton_clicked();
		void on_selectall_clicked();
		void on_selectvisible_clicked();

	private:
		void closeEvent(QCloseEvent *) override;
		void accept() override;
		void reject() override;
		void init();
		void highLightSurface();
		void initLocals();
		

	private:
		Ui::SurfaceMeshDialog* _ui{};

//		QList<vtkActor*> _actorList{};
		QMultiHash<Geometry::GeometrySet*, int> _geoHash{};
		bool _selectFace{ false };

		GmshSettingData* _settingData{};
		MeshData::MeshData* _meshData{};
		MeshData::MeshKernal* _kernalData{};
	};

}


#endif