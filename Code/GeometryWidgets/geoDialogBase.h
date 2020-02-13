#ifndef GEODIALOGBASE_H_
#define GEODIALOGBASE_H_

#include "geometryWidgetsAPI.h"
#include "SelfDefObject/QFDialog.h"
#include "moduleBase/ModuleType.h"

class QDialogButtonBox;

namespace MainWidget
{
	class PreWindow;
}
namespace GUI
{
	class MainWindow;
}
namespace Geometry
{
	class GeometryData;
	class GeometrySet;
}

namespace Py
{
	class PythonAagent;
}

class vtkActor;

namespace GeometryWidget
{
	class GEOMETRYWIDGETSAPI GeoDialogBase :public QFDialog
	{
		Q_OBJECT
	public:
		GeoDialogBase(GUI::MainWindow* m, MainWidget::PreWindow* p , bool keySig = true);
		~GeoDialogBase() = default;

		bool isEdit();
	signals:
		void updateGraphOptions();
		void setSelectMode(int);
		void highLightGeometrySet(Geometry::GeometrySet* set, bool on);
		void highLightGeometryPoint(Geometry::GeometrySet* set, int index, QList<vtkActor*>*);
		void highLightGeometryEdge(Geometry::GeometrySet* set, int index, QList<vtkActor*>*);
		void highLightGeometryFace(Geometry::GeometrySet* set, int index, QList<vtkActor*>*);
		void showGeometry(Geometry::GeometrySet* set);
		void hideGeometry(Geometry::GeometrySet* set);
		void updateGeoTree();

	protected:
		void translateButtonBox(QDialogButtonBox *b);

	private slots:
	    virtual void selectActorShape(vtkActor*, int, Geometry::GeometrySet*);

	protected:
		Geometry::GeometryData* _geoData{};
		MainWidget::PreWindow* _preWindow{};

		//�༭ģʽ
		bool _isEdit{ false };
		Geometry::GeometrySet* _editSet{};

		Py::PythonAagent* _pyAgent{};
	};
}


#endif