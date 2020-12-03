#ifndef MESHVIEWPROVIDER_H_
#define MESHYVIEWPROVIDER_H_

#include "mainWidgetsAPI.h"
#include <QObject>
#include <QMultiHash>
#include <QHash>
#include <QList>
#include "moduleBase/ModuleType.h"
#include <vtkPolyData.h>
#include <vtkActor.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyDataMapper.h>
#include <vtkLookupTable.h>
#include "meshData/meshSingleton.h"
#include "meshData/meshKernal.h"
#include "meshData/meshSet.h"

class TopoDS_Shape;
class TopoDS_Edge;
class vtkActor;

namespace GUI
{
	class MainWindow;
}

namespace MeshData
{
	class MeshData;
}


namespace MainWidget
{
	class PreWindow;
	enum DisplayModel
	{
		Node,
		WireFrame,
		Surface,
		SurfaceWithEdge,
	};
	class MAINWIDGETSAPI MeshViewProvider: public QObject
	{
		Q_OBJECT
	public:
		MeshViewProvider(GUI::MainWindow* mainwindow, PreWindow* preWin);
		~MeshViewProvider();
		//更新网格渲染
		void updateDisplayModel();
		void updateMeshActor();
		void updateKernalActor();
		void updateMeshDispaly(int index, bool display);
		void removeMeshActor(const int index);
		void setDisplay(QString m);

		vtkActor* getActorByKernal(MeshData::MeshKernal* k);

	public:
		QMultiHash<vtkDataSet*, int>* _selectItems{};
	private:
		void init();
		void removeMeshActors();
		void updateKernalShowIds(MeshData::MeshSet* set, MeshData::MeshKernal* k);
		void updateDisplayKernal(MeshData::MeshKernal* k);
		void updateMappingItems(QMultiHash<vtkDataSet*, int>* oldItems);
		void updateBoxMeshMappingItems(QMultiHash<vtkDataSet*, int>* oldItems, vtkDataSet * kernaldataSet);
		void updateMeshNodeMappingItems(QMultiHash<vtkDataSet*, int>* oldItems, vtkDataSet * dataSet);
		void updateMeshCellMappingItems(QMultiHash<vtkDataSet*, int>* oldItems, vtkDataSet * dataSet);
		//
		void fillKernalCellIds(MeshData::MeshKernal* k);
		void fillKernalPointIds(MeshData::MeshKernal* k);
		void updateCellMappingPointIds(MeshData::MeshKernal* k, QHash<int, bool> _cellIdIsExistHash, int cellId, bool isShow);
		void findCellMappingKCell(vtkDataSet * dataSet, vtkIdList *cellPtIds, vtkDataSet * kernalDataSet, int &findCellId);
	public slots:
	    void setMeshSelectMode(int mode);
		//网格的高亮显示
		void clearHighLight();
		void highLighDataSet(vtkDataSet* dataset);
		void highLighSet(QMultiHash<vtkDataSet*, int>* items);
		void highLighMeshSet(MeshData::MeshSet* set); // 组件meshSet的高亮显示
		void highLighKernel(MeshData::MeshKernal* k); // 网格模型kernel的高亮显示
		//
		void highLightActorDispalyPoint(bool on);
		void updateGraphOption();
		void updateMeshSetDisplay();
		//meshSet的显示和隐藏
		void updateMeshSetVisibily(MeshData::MeshSet* set);
		void removeSetData(const int index);
	private:
		PreWindow* _preWindow{};
		GUI::MainWindow* _mainWindow{};
		//
		DisplayModel _displayModel{SurfaceWithEdge};
		ModuleBase::SelectModel _selectModel{ ModuleBase::None };
		//
		QList<vtkActor*> _meshActors{};
		MeshData::MeshData* _meshData{};
		//新增集合
		QHash<MeshData::MeshKernal*, vtkDataSet*> _kernalDataSetHash{};
		QHash<vtkDataSetMapper *, MeshData::MeshKernal*> _mapperKernalHash{};
		QHash<vtkDataSetMapper *, vtkDataSet*> _mapperDataSetHash{};
		QHash<vtkActor *, vtkDataSetMapper *> _actorMapperHash{};
		//
		QHash<MeshData::MeshKernal*, QList<int>*> _kernalCellIdsHash{};
		QHash<MeshData::MeshKernal*, QList<int>*> _kernalPointIdsHash{};
	protected:
		vtkSmartPointer<vtkActor> _highLightActor{};
		vtkSmartPointer<vtkDataSetMapper> _highLightMapper{};
		vtkSmartPointer<vtkPolyData> _emptyDataset{};
		//
		vtkSmartPointer<vtkActor> _boxActor{};
		vtkSmartPointer<vtkPolyDataMapper> _boxMapper{};
	};

}

#endif
