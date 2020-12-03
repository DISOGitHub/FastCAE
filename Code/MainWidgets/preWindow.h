#ifndef PREWINDOW_H
#define PREWINDOW_H

#include "mainWidgetsAPI.h"
#include "moduleBase/graph3DWindow.h"
#include "moduleBase/ModuleType.h"

namespace MeshData
{
	class MeshData;
}
namespace Geometry
{
	class GeometryData;
	class GeometrySet;
	class GeometryDatum;
	class GeoComponent;
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
	class MeshViewProvider;

	class MAINWIDGETSAPI PreWindow :public ModuleBase::Graph3DWindow
	{
		Q_OBJECT
	public:
		PreWindow(GUI::MainWindow* mw, int id = -1, ModuleBase::GraphWindowType type = ModuleBase::GraphWindowType::PreWindows);
		~PreWindow();
		
		Geometry::GeometrySet* getSelectedGeoSet();
		//获取选择的几何
		QMultiHash<Geometry::GeometrySet*, int>* getGeoSelectItems();
		//设置已经选择的几何
		ModuleBase::SelectModel getSelectModel();
		QMultiHash<vtkDataSet*, int>* getSelectItems();

		MeshViewProvider* getMeshViewProvider();
	signals:
		//关闭
		void closed();
		//显示几何形状
		void showGeoSet(Geometry::GeometrySet* set, bool render = true);
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
		//新增接口
		void geoShapeSelected(Geometry::GeometrySet*shape, int index);//被选中接口
		void highLightGeometrySet(Geometry::GeometrySet* s, bool on);//高亮显示主体
		void highLightGeometryPoint(Geometry::GeometrySet* s, int id, bool on);//高亮显示点
		void highLightGeometryEdge(Geometry::GeometrySet* s, int id, bool on);//高亮显示边
		void highLightGeometryFace(Geometry::GeometrySet* s, int id, bool on);//高亮显示面
		void highLightGeometrySolid(Geometry::GeometrySet* s, int id, bool on);//高亮显示实体
		void clearGeometryHighLight();//清空所有高亮对象
		void removeSetDataSig(const int index);
		//更新网格渲染元素
		void updateMeshActorSig();
		void highLighKernel(MeshData::MeshKernal* k);
		void highLighMeshSet(MeshData::MeshSet* set);
		void setMeshSelectMode(int model);
		void highLighDataSet(vtkDataSet* dataset);
		void clearMeshSetHighLight();		
		
		void highLightGeoComponentSig(Geometry::GeoComponent* aGC);
		
	public slots:
	    //设置选择模式
	    void setSelectModel(int mode) override;
		void setGeoSelectItems(QMultiHash<Geometry::GeometrySet*, int> items);
		//设置草图类型
		void setSketchType(ModuleBase::SketchType t);
		//选择网格类型
		void setDisplay(QString m) override;
		//更新网格渲染
		void updateMeshActor();
		//
		void highLighSet(QMultiHash<vtkDataSet*, int>* items);
		//更新几何渲染
		void updateGeometryActor();
		//高亮一个几何组件;
		void highLightGeoComponentSlot(Geometry::GeoComponent*);
		
	private slots:
		void updateGraphOption() override;
		//几何
		void updateGeoDispaly(int index, bool display);
		void removeGemoActor(const int index);
		//网格
		void updateMeshDispaly(int index, bool display);
		void removeMeshActor(const int index);
		//草图
		void startSketch(bool start, double* loc, double* dir);

	private:
		//QList<vtkActor*> _meshActors{};
		//QList<vtkActor*> _geometryActors{};
		ModuleBase::SelectModel _selectModel;
		QMultiHash<vtkDataSet*, int>* _selectItems{};
		//
		MeshData::MeshData* _meshData{};
		Geometry::GeometryData* _geometryData{};

		ModelData::ModelDataSingleton* _modelData{};
		int _selectedGeoIndex{ -1 };

		GeometryViewProvider* _geoProvider{};
		MeshViewProvider * _meshProvider{};
		SketchViewProvider* _sketchProvider{};

	};
}

#endif
