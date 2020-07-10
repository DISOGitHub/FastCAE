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
		GeoDialogBase(GUI::MainWindow* m, MainWidget::PreWindow* p , bool keySig = true, bool restoreselectmode = true);
		~GeoDialogBase();

		bool isEdit();
	signals:
		
		void setSelectMode(int);
		//delete***********
		void updateGraphOptions();
		void highLightGeometrySet(Geometry::GeometrySet* set, bool on);
		void highLightGeometryPoint(Geometry::GeometrySet* set, int index, QList<vtkActor*>*);
		void highLightGeometryEdge(Geometry::GeometrySet* set, int index, QList<vtkActor*>*);
		void highLightGeometryFace(Geometry::GeometrySet* set, int index, QList<vtkActor*>*);
		//***********
		void showGeometry(Geometry::GeometrySet* set);
		void hideGeometry(Geometry::GeometrySet* set);
		void updateGeoTree();

		//新增接口
		void highLightGeometrySetSig(Geometry::GeometrySet*, bool);//高亮显示主体
		void highLightGeometryPointSig(Geometry::GeometrySet*, int, bool);//高亮显示点
		void highLightGeometryEdgeSig(Geometry::GeometrySet*, int, bool);//高亮显示边
		void highLightGeometryFaceSig(Geometry::GeometrySet*, int, bool );//高亮显示面
		void highLightGeometrySolidSig(Geometry::GeometrySet*, int, bool);//高亮显示实体
		void clearGeometryHighLightSig();//清空所有高亮对象

	protected:
		void translateButtonBox(QDialogButtonBox *b);

	private slots:
	//delete***********
	    virtual void selectActorShape(vtkActor*, int, Geometry::GeometrySet*);
	////////////////////////////////////
		virtual void shapeSlected(Geometry::GeometrySet* set, int index);

	protected:
		Geometry::GeometryData* _geoData{};
		MainWidget::PreWindow* _preWindow{};

		//编辑模式
		bool _isEdit{ false };
		Geometry::GeometrySet* _editSet{};

		Py::PythonAagent* _pyAgent{};

		bool _restoreSelectMode{ true };
	};
}


#endif