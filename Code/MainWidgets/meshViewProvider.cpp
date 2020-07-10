#include "meshViewProvider.h"

#include "mainWindow/mainWindow.h"
#include "MainWidgets/preWindow.h"

#include "settings/busAPI.h"
#include "settings/GraphOption.h"

#include <vtkRenderer.h>
#include <vtkSmoothPolyDataFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkRenderWindow.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkCoordinate.h>
#include <vtkAppendFilter.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkExtractSelection.h>
#include <vtkExtractSelection.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkUnstructuredGrid.h>
#include <vtkLookupTable.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>
#include <vtkPolygon.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkIdList.h>
#include <TopoDS.hxx>
#include <TopExp_Explorer.hxx>
#include <IVtkOCC_Shape.hxx>
#include <IVtkTools_ShapeDataSource.hxx>
#include <IVtkTools_DisplayModeFilter.hxx>

#include <QDebug>
#include <assert.h>

namespace MainWidget
{
	MeshViewProvider::MeshViewProvider(GUI::MainWindow* mainwindow, PreWindow* renderWin) :
		_mainWindow(mainwindow), _preWindow(renderWin)
	{
		_meshData = MeshData::MeshData::getInstance();
		connect(_preWindow, SIGNAL(removeSetDataSig(int)), this, SLOT(removeSetData(int)));
		//网格相关的高亮信号槽连接
		connect(_preWindow, SIGNAL(highLightActorDispalyPoint(bool)), this, SLOT(highLightActorDispalyPoint(bool)));
		//
		connect(_preWindow, SIGNAL(clearMeshSetHighLight()), this, SLOT(clearHighLight()));
		connect(_preWindow, SIGNAL(clearAllHighLight()), this, SLOT(clearHighLight()));
		connect(_preWindow, SIGNAL(highLighMeshSet(MeshData::MeshSet*)), this, SLOT(highLighMeshSet(MeshData::MeshSet*)));
		connect(_preWindow, SIGNAL(highLighKernel(MeshData::MeshKernal*)), this, SLOT(highLighKernel(MeshData::MeshKernal*)));
		connect(_preWindow, SIGNAL(highLighDataSet(vtkDataSet*)), this, SLOT(highLighDataSet(vtkDataSet*)));
		connect(_mainWindow, SIGNAL(updateMeshSetVisibleSig(MeshData::MeshSet*)), this, SLOT(updateMeshSetVisibily(MeshData::MeshSet*)));
		init();
	}

	MeshViewProvider::~MeshViewProvider()
	{
		removeMeshActors();
	}

	void MeshViewProvider::init()
	{
		//
		_selectItems = new QMultiHash<vtkDataSet*, int>;
		//高亮显示对象的初始化
		_highLightMapper = vtkSmartPointer<vtkDataSetMapper>::New();
		_highLightActor = vtkSmartPointer<vtkActor>::New();
		_emptyDataset = vtkSmartPointer<vtkPolyData>::New();

		_highLightActor->SetMapper(_highLightMapper);
		_highLightActor->SetPickable(false);
		_highLightMapper->ScalarVisibilityOff();
		_highLightMapper->SetInputData(_emptyDataset);
		_highLightActor->GetProperty()->SetOpacity(0.8);
		_highLightMapper->Update();
		_preWindow->AppendActor(_highLightActor, ModuleBase::ActorType::D3);
		//高亮显示对象的初始化
		_boxMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		_boxMapper->ScalarVisibilityOff();
		_boxMapper->SetInputData(_emptyDataset);
		_boxMapper->Update();
		_boxActor = vtkSmartPointer<vtkActor>::New();
		_boxActor->SetMapper(_boxMapper);
		_boxActor->SetPickable(false);
		_boxActor->GetProperty()->SetRepresentationToWireframe();
		_boxActor->GetProperty()->EdgeVisibilityOn();
		_boxActor->GetProperty()->SetLineWidth(1);
		_preWindow->AppendActor(_boxActor, ModuleBase::ActorType::D3);
		//
		updateGraphOption();//高亮对象的颜色
	}

	/*
	设置网格选择模式
	*/
	void MeshViewProvider::setMeshSelectMode(int mode)
	{
		clearHighLight();
		ModuleBase::SelectModel m = (ModuleBase::SelectModel) mode;
		_selectModel = m;
		int n = 0;
		for (int i = 0; i < n; ++i) _meshActors.at(i)->PickableOff();
		switch (m)
		{
		case ModuleBase::None:
			clearHighLight();
			break;
		case ModuleBase::MeshNode:
		case ModuleBase::BoxMeshNode:
			//
			_highLightActor->GetProperty()->SetRepresentationToPoints();
			_highLightActor->GetProperty()->SetPointSize(5);
			//
			n = _meshActors.size();
			for (int i = 0; i < n; ++i) _meshActors.at(i)->PickableOn();
			break;
		case ModuleBase::MeshCell:
		case ModuleBase::BoxMeshCell:
			//
			_highLightActor->GetProperty()->SetRepresentationToSurface();
			//
			n = _meshActors.size();
			for (int i = 0; i < n; ++i) _meshActors.at(i)->PickableOn();
			break;
		default:
			break;
		}
	}

	/*
	更新网格所有kernal数据对象以actor渲染方式显示
	*/
	void MeshViewProvider::updateMeshActor()
	{
		//清空所有集合和模型数据
		removeMeshActors();
		//重新更新显示
		updateKernalActor();
	}

	/*
	更新显示所有的kernal模型数据
	*/
	void MeshViewProvider::updateKernalActor()
	{
		const int n = _meshData->getKernalCount();
		for (int i = 0; i < n; ++i)
		{
			MeshData::MeshKernal* k = _meshData->getKernalAt(i);
			vtkDataSet * dataSet = k->getMeshData();
			bool visible = k->isVisible();
			vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
			vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
			mapper->SetInputData(dataSet);
			mapper->ScalarVisibilityOff();
			mapper->Update();
			actor->SetMapper(mapper);
			actor->SetVisibility(visible);
			_preWindow->AppendActor(actor, ModuleBase::ActorType::D3);
			//
			_meshActors.append(actor);
			_kernalDataSetHash.insert(k, dataSet);
			_mapperKernalHash.insert(mapper, k);
			_mapperDataSetHash.insert(mapper, mapper->GetInput());
			_actorMapperHash.insert(actor, mapper);
			//
			fillKernalCellIds(k);
			fillKernalPointIds(k);
		}
		updateDisplayModel();
		updateMeshSetDisplay();
		_preWindow->reRender();
	}

	/*
	填充实体的单元序号
	*/
	void MeshViewProvider::fillKernalCellIds(MeshData::MeshKernal* k)
	{
		vtkDataSet * dataSet = k->getMeshData();
		QList<int> *cellIds = new QList<int>;
		for (vtkIdType i = 0; i < dataSet->GetNumberOfCells(); i++)
		{
			cellIds->append((int)i);
		}
		_kernalCellIdsHash.insert(k, cellIds);
	}

	/*
	填充实体的点序号
	*/
	void MeshViewProvider::fillKernalPointIds(MeshData::MeshKernal* k)
	{
		vtkDataSet * dataSet = k->getMeshData();
		QList<int> *ptIds = new QList<int>;
		for (vtkIdType i = 0; i < dataSet->GetNumberOfPoints(); i++)
		{
			ptIds->append((int)i);
		}
		_kernalPointIdsHash.insert(k, ptIds);
	}

	/*
	更新设置kernal指定序号网格的显示隐藏状态
	*/
	void MeshViewProvider::updateMeshDispaly(int index, bool display)
	{
		clearHighLight();
		assert(index >= 0 && index < _meshActors.size());
		vtkActor* ac = _meshActors.at(index);
		if (ac == nullptr) return;
		ac->SetVisibility(display);
		_preWindow->reRender();
		_preWindow->resetCamera();
	}

	/*
	删除所有kernal网格actor
	*/
	void MeshViewProvider::removeMeshActors()
	{
		const int n = _meshActors.size();
		for (int i = 0; i < n; ++i)
		{
			vtkActor* actor = _meshActors.at(i);
			_preWindow->RemoveActor(actor);
		}
		_meshActors.clear();
		_mapperKernalHash.clear();
		_kernalDataSetHash.clear();
		_mapperDataSetHash.clear();
		clearHighLight();
	}

	/*
	删除指定的kernal网格模型actor
	*/
	void MeshViewProvider::removeMeshActor(const int index)
	{
		assert(index >= 0 && index < _meshActors.size());
		vtkActor* actor = _meshActors.at(index);
		_preWindow->RemoveActor(actor);
		//
		_meshActors.removeAt(index);
		//
		_kernalDataSetHash.remove(_mapperKernalHash.value(_actorMapperHash.value(actor)));
		_mapperKernalHash.remove(_actorMapperHash.value(actor));
		_mapperDataSetHash.remove(_actorMapperHash.value(actor));
		_actorMapperHash.remove(actor);
		//
		_preWindow->reRender();
	}

	void MeshViewProvider::removeSetData(const int index)
	{
		assert(index >= 0 && index < _meshData->getMeshSetCount());
		MeshData::MeshSet* set = _meshData->getMeshSetAt(index);
		set->isVisible(true);
		updateMeshSetVisibily(set);
	}

	/*
	设置显示模式
	*/
	void MeshViewProvider::setDisplay(QString m)
	{
		if (m.toLower() == "node") _displayModel = Node;
		else if (m.toLower() == "wireframe") _displayModel = WireFrame;
		else if (m.toLower() == "surface") _displayModel = Surface;
		else if (m.toLower() == "surfacewithedge") _displayModel = SurfaceWithEdge;
		updateDisplayModel();
	}

	/*
	更新当前kernal网格模型显示方式
	*/
	void MeshViewProvider::updateDisplayModel()
	{
		const int n = _meshActors.size();
		QColor c;
		float size;
		Setting::GraphOption* goptions = Setting::BusAPI::instance()->getGraphOption();
		for (int i = 0; i < n; ++i)
		{
			MeshData::MeshKernal* k = _mapperKernalHash.value(_actorMapperHash.value(_meshActors.at(i)));
			updateDisplayKernal(k);
			auto prop = _meshActors.at(i)->GetProperty();
			switch (_displayModel)
			{
			case MainWidget::Node:
				prop->SetRepresentationToPoints();
				c = goptions->getMeshNodeColor();
				size = goptions->getMeshNodeSize();
				prop->SetDiffuseColor(c.redF(), c.greenF(), c.blueF());
				prop->SetPointSize(size);
				prop->EdgeVisibilityOff();
				break;
			case MainWidget::WireFrame:
				prop->SetRepresentationToWireframe();
				prop->EdgeVisibilityOn();
				c = goptions->getMeshEdgeColor();
				size = goptions->getMeshEdgeWidth();
				prop->SetDiffuseColor(c.redF(), c.greenF(), c.blueF());
				prop->SetEdgeColor(c.redF(), c.greenF(), c.blueF());
				prop->SetLineWidth(size);
				break;
			case MainWidget::Surface:
				prop->SetRepresentationToSurface();
				prop->EdgeVisibilityOff();
				c = goptions->getMeshFaceColor();
				prop->SetDiffuseColor(c.redF(), c.greenF(), c.blueF());
				prop->SetEdgeColor(c.redF(), c.greenF(), c.blueF());
				prop->SetLineWidth(size);
				break;
			case MainWidget::SurfaceWithEdge:
				c = goptions->getMeshFaceColor();
				prop->SetDiffuseColor(c.redF(), c.greenF(), c.blueF());
				c = goptions->getMeshEdgeColor();
				prop->SetRepresentationToSurface();
				prop->SetEdgeColor(c.redF(), c.greenF(), c.blueF());
				size = goptions->getMeshEdgeWidth();
				prop->SetLineWidth(size);
				prop->EdgeVisibilityOn();
				break;
			default:
				break;
			}
		}
		_preWindow->reRender();
	}

	void MeshViewProvider::updateMeshSetDisplay()
	{
		const int n = _meshData->getMeshSetCount();
		for (int i = 0; i < n; ++i)
		{
			MeshData::MeshSet* set = _meshData->getMeshSetAt(i);
			if (!set->isVisible())
			{
				updateMeshSetVisibily(set);
			}
		}
	}

	/*
	MeshSet的数据的显示与隐藏
	*/
	void MeshViewProvider::updateMeshSetVisibily(MeshData::MeshSet* set)
	{
		clearHighLight();
		if (set == nullptr)
		{
			return;
		}
		QList<int > kernalIds = set->getKernals();
		for (int i = 0; i < kernalIds.size(); i++)
		{
			MeshData::MeshKernal* k = _meshData->getKernalByID(kernalIds[i]);
			updateKernalShowIds(set, k);
			updateDisplayKernal(k);
		}
	}

	/*
	更新当前显示或隐藏meshSet后的kernalCellIds的数据
	*/
	void MeshViewProvider::updateKernalShowIds(MeshData::MeshSet* set, MeshData::MeshKernal* k)
	{
		//
		MeshData::SetType type = set->getSetType();
		vtkDataSet * dataSet = k->getMeshData();
		if (type == MeshData::Element)
		{
			QList<int> *kernalCellIds = _kernalCellIdsHash.value(k);
			//
			QList<int> cellIdList = set->getKernalMembers(k->getID());
			QHash<int, bool> _cellIdIsExistHash;
			for (int i = 0; i < cellIdList.size(); i++)
			{
				_cellIdIsExistHash.insert(cellIdList[i], true);
			}
			for (int i = 0; i < cellIdList.size(); i++)
			{
				if (set->isVisible())
				{
					if (kernalCellIds->contains(cellIdList.at(i)))
					{
						continue;
					}
					kernalCellIds->append(cellIdList.at(i));
				}
				else {
						kernalCellIds->removeOne(cellIdList.at(i));
				}
				updateCellMappingPointIds(k, _cellIdIsExistHash, cellIdList[i], set->isVisible());
			}
		}
		else if (type == MeshData::Node)
		{
			QList<int> *kernalPtIds = _kernalPointIdsHash.value(k);
			//
			QList<int> ptIdList = set->getKernalMembers(k->getID());
			for (int i = 0; i < ptIdList.size(); i++)
			{
				if (set->isVisible())
				{
					if (kernalPtIds->contains(ptIdList[i]))
					{
						continue;
					}
					kernalPtIds->append(ptIdList[i]);
				}
				else {
					kernalPtIds->removeOne(ptIdList[i]);
				}
			}
		}
		
	}

	void MeshViewProvider::updateCellMappingPointIds(MeshData::MeshKernal* k, QHash<int, bool> _cellIdIsExistHash, int cellId, bool isShow)
	{
		vtkDataSet * dataSet = k->getMeshData();
		QList<int> *kernalPtIds = _kernalPointIdsHash.value(k);
		//
		vtkSmartPointer<vtkIdList> ptIds = vtkSmartPointer<vtkIdList>::New();
		dataSet->GetCellPoints((vtkIdType)cellId, ptIds);
		for (vtkIdType j = 0; j < ptIds->GetNumberOfIds(); j++)
		{
			vtkIdType ptId = ptIds->GetId(j);
			bool isfind = true;
			vtkSmartPointer<vtkIdList> cellIds = vtkSmartPointer<vtkIdList>::New();
			dataSet->GetPointCells(ptId, cellIds);
			for (vtkIdType k = 0; k < cellIds->GetNumberOfIds(); k++)
			{
				bool isExist = _cellIdIsExistHash.value((int)cellIds->GetId(k));
				if (isExist == true)
				{
					continue;
				}
				else {
					isfind = false;
					break;
				}
			}
			if(isfind == false)
			{
				continue;
			}
			if (isfind == true)
			{
				if (isShow)
				{
					if (kernalPtIds->contains((int)ptId))
					{
						continue;
					}
					kernalPtIds->append((int)ptId);
				}
				else {
					kernalPtIds->removeOne((int)ptId);
				}
			}
		}
	}

	/*
	更新显示隐藏set后的实体模型数据
	*/
	void MeshViewProvider::updateDisplayKernal(MeshData::MeshKernal* k)
	{
		vtkSmartPointer<vtkAppendFilter> appendFliter = vtkSmartPointer<vtkAppendFilter>::New();
		vtkDataSet* dataset = k->getMeshData();
		vtkSmartPointer<vtkIdTypeArray> idArray = vtkSmartPointer<vtkIdTypeArray>::New();
		//
		vtkSmartPointer<vtkSelectionNode> selectionNode = vtkSmartPointer<vtkSelectionNode>::New();
		if (_displayModel == Node)
		{
			QList<int> *ptIds = _kernalPointIdsHash.value(k);
			for (int id : *ptIds) idArray->InsertNextValue(id);
			selectionNode->SetFieldType(vtkSelectionNode::POINT);
		}
		else {
			QList<int> *cellIds = _kernalCellIdsHash.value(k);
			for (int id : *cellIds) idArray->InsertNextValue(id);
			selectionNode->SetFieldType(vtkSelectionNode::CELL);
		}
		selectionNode->SetContentType(vtkSelectionNode::INDICES);
		selectionNode->SetSelectionList(idArray);

		vtkSmartPointer<vtkSelection> selection = vtkSmartPointer<vtkSelection>::New();
		selection->AddNode(selectionNode);

		vtkSmartPointer<vtkExtractSelection> extractionSelection = vtkSmartPointer<vtkExtractSelection>::New();
		extractionSelection->SetInputData(0, dataset);
		extractionSelection->SetInputData(1, selection);
		extractionSelection->Update();
		//
		vtkDataSetMapper * mapper = _mapperKernalHash.key(k);
		mapper->SetInputConnection(extractionSelection->GetOutputPort());
		mapper->Update();
		_mapperKernalHash.insert(mapper, k);
		_mapperDataSetHash.insert(mapper, mapper->GetInput());
		_preWindow->reRender();
		_actorMapperHash.key(mapper)->SetVisibility(k->isVisible());
	}

	/****************************************高亮相关的**************************************************/
	/*
	以置空的polydata类型的数据实现清空高亮显示
	*/
	void MeshViewProvider::clearHighLight()
	{
		_highLightMapper->SetInputData(_emptyDataset);
		_highLightMapper->Update();
		_preWindow->reRender();
	}
	/*
	高亮显示MeshSet类型的数据
	*/
	void MeshViewProvider::highLighMeshSet(MeshData::MeshSet* set)
	{
		if (set == nullptr || !set->isVisible())
		{
			clearHighLight();
			return;
		}
		MeshData::SetType settype = set->getSetType();
		_highLightActor->GetProperty()->SetRepresentationToSurface();
		_highLightActor->SetPickable(false);
		if (settype == MeshData::Node)
		{
			_highLightActor->GetProperty()->SetRepresentationToPoints();
			_highLightActor->GetProperty()->SetPointSize(5);
		}
		vtkDataSet* dataset = set->getDisplayDataSet();
		_highLightMapper->SetInputData(dataset);
		_highLightMapper->Update();
		_preWindow->reRender();
	}

	/*
	高亮显示kernal类型的数据
	*/
	void MeshViewProvider::highLighKernel(MeshData::MeshKernal* k)
	{
		if (k == nullptr || !k->isVisible())
		{
			clearHighLight();
			return;
		}
		_highLightActor->GetProperty()->SetRepresentationToSurface();
		 vtkDataSetMapper * mapper = _mapperKernalHash.key(k);
		 _highLightMapper->SetInputData(mapper->GetInput());
		_highLightMapper->Update();
		_preWindow->reRender();
	}

	/*
	如果为true则高亮对象为点显示方式如果为false则为面显示方式
	*/
	void MeshViewProvider::highLightActorDispalyPoint(bool on)
	{
		if (on)
		{
			_highLightActor->GetProperty()->SetRepresentationToPoints();
			_highLightActor->GetProperty()->SetPointSize(5);
		}
		else
		{
			_highLightActor->GetProperty()->SetRepresentationToSurface();
		}
	}
	/*
	更新高亮显示对象的颜色参数的设置
	*/
	void MeshViewProvider::updateGraphOption()
	{
		Setting::GraphOption* option = Setting::BusAPI::instance()->getGraphOption();
		QColor hicolor = option->getHighLightColor();
		_highLightActor->GetProperty()->SetDiffuseColor(hicolor.redF(), hicolor.greenF(), hicolor.blueF());
		_boxActor->GetProperty()->SetDiffuseColor(hicolor.redF(), hicolor.greenF(), hicolor.blueF());
	}

	/*
	对于vtkDataSet类型的数据的高亮显示
	*/
	void MeshViewProvider::highLighDataSet(vtkDataSet* dataset)
	{
		_highLightMapper->SetInputData(dataset);
		_highLightMapper->Update();
		_preWindow->reRender();
	}

	/*
	对于拾取点，单元的高亮显示
	*/
	void MeshViewProvider::highLighSet(QMultiHash<vtkDataSet*, int>* items)
	{
		if (items == nullptr || items->isEmpty())
		{
			clearHighLight();
			return;
		}
		//找到映射关系所构建的selectItems
		_selectItems->clear();
		findMappingItems(items, _selectItems);
		if (_selectItems->size() == 0)
		{
			return;
		}
		vtkSmartPointer<vtkAppendFilter> appendFilter = vtkSmartPointer<vtkAppendFilter>::New();
		QList<vtkDataSet*> datasetList = _selectItems->uniqueKeys();
		for (vtkDataSet* dateset : datasetList)
		{
			QList<int> ids = _selectItems->values(dateset);
			vtkSmartPointer<vtkIdTypeArray> idarray = vtkSmartPointer<vtkIdTypeArray>::New();
			for (int id : ids)
				idarray->InsertNextValue(id);

			vtkSmartPointer<vtkSelectionNode> selectionNode = vtkSmartPointer<vtkSelectionNode>::New();
			if (_selectModel == ModuleBase::MeshCell || _selectModel == ModuleBase::BoxMeshCell)
				selectionNode->SetFieldType(vtkSelectionNode::CELL);
			else if (_selectModel == ModuleBase::MeshNode || _selectModel == ModuleBase::BoxMeshNode)
				selectionNode->SetFieldType(vtkSelectionNode::POINT);
			selectionNode->SetContentType(vtkSelectionNode::INDICES);
			selectionNode->SetSelectionList(idarray);

			vtkSmartPointer<vtkSelection> selection = vtkSmartPointer<vtkSelection>::New();
			selection->AddNode(selectionNode);

			vtkSmartPointer<vtkExtractSelection> extractionSelection = vtkSmartPointer<vtkExtractSelection>::New();
			extractionSelection->SetInputData(0, dateset);
			extractionSelection->SetInputData(1, selection);
			extractionSelection->Update();
			appendFilter->AddInputData(extractionSelection->GetOutput());
		}
		appendFilter->Update();
		_highLightMapper->SetInputData(appendFilter->GetOutput());
		_highLightMapper->Update();
		_preWindow->reRender();
	}

	/*
	查找映射后的Items
	*/
	void MeshViewProvider::findMappingItems(QMultiHash<vtkDataSet*, int>* oldItems, QMultiHash<vtkDataSet*, int>* newItems)
	{
		QList<vtkDataSet*> vtkDataSets = oldItems->uniqueKeys();
		for (int i = 0; i < vtkDataSets.size(); i++)
		{
			vtkDataSet * dataSet = vtkDataSets[i];
			if (dataSet == nullptr)
			{
				continue;
			}
			if (_selectModel == ModuleBase::BoxMeshCell || _selectModel == ModuleBase::BoxMeshNode)
			{
				MeshData::MeshKernal* k = _kernalDataSetHash.key(dataSet);
				//
				QList<int> *cellIds = _kernalCellIdsHash.value(k);
				//
				vtkDataSet* mapperDataSet = _mapperDataSetHash.value(_mapperKernalHash.key(k));
				//
				QList<int> ids = oldItems->values(dataSet);
				for (int j = 0; j < ids.size(); j++)
				{
					int id = ids[j];
					if (_selectModel == ModuleBase::BoxMeshCell)
					{
						if (cellIds->contains(id))
						{
							newItems->insert(dataSet, id);
						}
					}
					if (_selectModel == ModuleBase::BoxMeshNode)
					{
						double pt[3];
						dataSet->GetPoint(id, pt);
						int mPtId = (int)mapperDataSet->FindPoint(pt);
						if (mPtId < 0)
						{
							continue;
						}
						newItems->insert(dataSet, id);
					}
				}
				continue;
			}
			//根据实际拾取到的dataSet数据找对应的mapper
			vtkDataSetMapper * mapper = _mapperDataSetHash.key(dataSet);
			//根据mapper找对应的kernal
			MeshData::MeshKernal* k = _mapperKernalHash.value(mapper);
			if (k == nullptr)
			{
				continue;
			}
			//获取当前kernal的dataSet数据
			vtkDataSet * kernalDataSet = k->getMeshData();
			if (_selectModel == ModuleBase::MeshCell)
			{
				//构建新的Items，换成kernalDataset数据，单元或点序号改变
				QList<int> ids = oldItems->values(dataSet);
				for (int j = 0; j < ids.size(); j++)
				{
					int id = ids[j];
					vtkSmartPointer<vtkIdList> ptIds = vtkSmartPointer<vtkIdList>::New();
					dataSet->GetCellPoints((vtkIdType)id, ptIds);
					int kId;
					findCellMappingKCell(dataSet, ptIds, kernalDataSet, kId);
					if (kId == -1)
					{
						continue;
					}
					newItems->insert(kernalDataSet, kId);
				}
			}
			if (_selectModel == ModuleBase::MeshNode)
			{
				//构建新的Items，换成kernalDataset数据，单元或点序号改变
				QList<int> ids = oldItems->values(dataSet);
				for (int j = 0; j < ids.size(); j++)
				{
					vtkIdType id = (vtkIdType)ids[j];
					double pt[3];
					dataSet->GetPoint(id, pt);
					int kPtId = (int)kernalDataSet->FindPoint(pt);
					if (kPtId < 0)
					{
						continue;
					}
					newItems->insert(kernalDataSet, kPtId);
				}
			}
		}
	}

	void MeshViewProvider::findCellMappingKCell(vtkDataSet * dataSet, vtkIdList *cellPtIds, vtkDataSet * kernalDataSet, int &findCellId)
	{
		vtkIdType cellPtId = cellPtIds->GetId(0);
		double pt[3];
		dataSet->GetPoint(cellPtId, pt);
		vtkIdType id = kernalDataSet->FindPoint(pt);
		vtkSmartPointer<vtkIdList> cellIds = vtkSmartPointer<vtkIdList>::New();
		kernalDataSet->GetPointCells(id, cellIds);
		QList<int> currCellIdList;
		for (vtkIdType i = 0; i < cellIds->GetNumberOfIds(); i++)
		{
			int cellId = (int)cellIds->GetId(i);
			currCellIdList.append(cellId);
		}
		currCellIdList = currCellIdList;
		for (vtkIdType i = 1; i < cellPtIds->GetNumberOfIds(); i++)
		{
			QList<int> cellIdList;
			vtkIdType cellPtId = cellPtIds->GetId(i);
			double pt[3];
			dataSet->GetPoint(cellPtId,pt);
			vtkIdType id = kernalDataSet->FindPoint(pt);
			vtkSmartPointer<vtkIdList> cellIds = vtkSmartPointer<vtkIdList>::New();
			kernalDataSet->GetPointCells(id, cellIds);
			for (vtkIdType j = 0; j < cellIds->GetNumberOfIds(); j++)
			{
				vtkIdType cellId = (vtkIdType)cellIds->GetId(j);
				getCellIdList(cellId, currCellIdList, cellIdList);
			}
			cellIdList = cellIdList;
			currCellIdList = cellIdList;
		}
		if (currCellIdList.size() == 0)
		{
			findCellId = -1;
		}
		else {
			findCellId = currCellIdList[0];
		}
	}

	void MeshViewProvider::getCellIdList(vtkIdType cellId, QList<int> &orignalCellIdList, QList<int>& cellIdList)
	{
		if (orignalCellIdList.contains((int)cellId))
		{
			cellIdList.append((int)cellId);
		}
	}

}