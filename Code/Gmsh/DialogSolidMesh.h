#ifndef DIALOGSOLIDMESH_H_
#define DIALOGSOLIDMESH_H_

#include "GmshDialogBase.h"
/*#include "GmshModuleAPI.h"*/
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

namespace DataProperty
{
	class DataBase;
}

namespace MeshData{
	class MeshKernal;
	class MeshData;
}

class vtkActor;


namespace Gmsh
{
	class GmshSettingData;

	class GMSHAPI SolidMeshDialog : public GmshDialogBase
	{
		Q_OBJECT
	public:
		SolidMeshDialog(GUI::MainWindow* m, MainWidget::PreWindow* pre, int index = -1);
		~SolidMeshDialog();


	private slots:
		void on_geoSelectSurface_clicked();
//		void selectActorShape(vtkActor*, int, Geometry::GeometrySet*) override;
		void shapeSlected(Geometry::GeometrySet* set, int index);
		void on_localButton_clicked();
		//void on_physicalsPButton_clicked();
		void on_selectall_clicked();
		void on_selectvisible_clicked();

	private:
		void closeEvent(QCloseEvent *) override;
		void accept() override;
		void reject() override;
		void init();
		void highLightSolid();
		void initLocals();

	private:
		Ui::SolidMeshDialog* _ui{};
		QMultiHash<Geometry::GeometrySet*, int> _solidHash{};

		Geometry::GeometryData* _geoData;

		bool _selectBody{ false };

		GmshSettingData* _settingData{};
		MeshData::MeshData* _meshData{};
		MeshData::MeshKernal* _kernalData{};
	};
}


#endif