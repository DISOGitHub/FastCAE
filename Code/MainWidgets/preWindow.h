#ifndef PREWINDOW_H
#define PREWINDOW_H

#include "mainWidgetsAPI.h"
#include "moduleBase/graph3DWindow.h"
#include "moduleBase/ModuleType.h"
#include <QList>

namespace MeshData
{
	class MeshData;
}
namespace Geometry
{
	class GeometryData;
	class GeometrySet;
	class GeometryDatum;
}

namespace ModelData
{
	class ModelDataSingleton;
}

namespace GUI
{
	class MainWindow;
}

class vtkActor;
class TopoDS_Shape;

namespace MainWidget
{
	class GeometryViewProvider;
	class SketchViewProvider;

	enum DisplayModel
	{
		Node,
		WireFrame,
		Surface,
		SurfaceWithEdge,
	};
	

	class MAINWIDGETSAPI PreWindow :public ModuleBase::Graph3DWindow
	{
		Q_OBJECT
	public:
		PreWindow(GUI::MainWindow* mw, int id = -1, ModuleBase::GraphWindowType type = ModuleBase::GraphWindowType::PreWindows);
		~PreWindow();
		
		Geometry::GeometrySet* getSelectedGeoSet();
		//获取选择的几何
		QMultiHash<Geometry::GeometrySet*, int> getGeoSelectItems();
		//设置已经选择的几何
		void setGeoSelectItems(QMultiHash<Geometry::GeometrySet*, int> items);
	
	
	signals:
		//关闭
		void closed();
		//显示几何形状
		void showGeoSet(Geometry::GeometrySet* set);
		//移除几何形状的显示
		void removeGemoActors(Geometry::GeometrySet* set);
		//显示基准
		void showDatum(Geometry::GeometryDatum*);
		//移除几何基准显示
		void removeGeoDatumActors(Geometry::GeometryDatum*);
		//设置选择模式
		void setGeoSelectMode(int);
		//选择的几何元素
		void selectGeoActorShape(vtkActor* ac, int shape, Geometry::GeometrySet* set);
		//更新网格渲染元素
		void updateMeshActorSig();
		

	public slots:
	    //设置选择模式
	    void setSelectModel(int mode) override;
		//设置草图类型
		void setSketchType(ModuleBase::SketchType t);
		//更新网格渲染
		void updateMeshActor();
		//更新几何渲染
		void updateGeometryActor();
		//清除所有高亮对象
		void clearAllHighLight();

	private:
		void removeMeshActors();
		void updateDisplayModel();

	private slots:
		void updateGeoDispaly(int index, bool display);
		void updateMeshDispaly(int index, bool display);
		void removeGemoActor(const int index);
		void removeMeshActor(const int index);
		//选择Geometry
		void setDisplay(QString m) override;

		void updateGraphOption() override;
		void startSketch(bool start, double* loc, double* dir);

	private:
		QList<vtkActor*> _meshActors{};
		//QList<vtkActor*> _geometryActors{};
		MeshData::MeshData* _meshData{};
		Geometry::GeometryData* _geometryData{};

		ModelData::ModelDataSingleton* _modelData{};
		int _selectedGeoIndex{ -1 };
		DisplayModel _displayModel{ SurfaceWithEdge };

		GeometryViewProvider* _geoProvider{};
		SketchViewProvider* _sketchProvider{};

	};
}

#endif
