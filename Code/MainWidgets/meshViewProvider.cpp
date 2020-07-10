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
		//������صĸ����źŲ�����
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
		//������ʾ����ĳ�ʼ��
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
		//������ʾ����ĳ�ʼ��
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
		updateGraphOption();//�����������ɫ
	}

	/*
	��������ѡ��ģʽ
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
	������������kernal���ݶ�����actor��Ⱦ��ʽ��ʾ
	*/
	void MeshViewProvider::updateMeshActor()
	{
		//������м��Ϻ�ģ������
		removeMeshActors();
		//���¸�����ʾ
		updateKernalActor();
	}

	/*
	������ʾ���е�kernalģ������
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
	���ʵ��ĵ�Ԫ���
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
	���ʵ��ĵ����
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
	��������kernalָ������������ʾ����״̬
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
	ɾ������kernal����actor
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
	ɾ��ָ����kernal����ģ��actor
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
	������ʾģʽ
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
	���µ�ǰkernal����ģ����ʾ��ʽ
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
	MeshSet�����ݵ���ʾ������
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
	���µ�ǰ��ʾ������meshSet���kernalCellIds������
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
	������ʾ����set���ʵ��ģ������
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

	/****************************************������ص�**************************************************/
	/*
	���ÿյ�polydata���͵�����ʵ����ո�����ʾ
	*/
	void MeshViewProvider::clearHighLight()
	{
		_highLightMapper->SetInputData(_emptyDataset);
		_highLightMapper->Update();
		_preWindow->reRender();
	}
	/*
	������ʾMeshSet���͵�����
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
	������ʾkernal���͵�����
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
	���Ϊtrue���������Ϊ����ʾ��ʽ���Ϊfalse��Ϊ����ʾ��ʽ
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
	���¸�����ʾ�������ɫ����������
	*/
	void MeshViewProvider::updateGraphOption()
	{
		Setting::GraphOption* option = Setting::BusAPI::instance()->getGraphOption();
		QColor hicolor = option->getHighLightColor();
		_highLightActor->GetProperty()->SetDiffuseColor(hicolor.redF(), hicolor.greenF(), hicolor.blueF());
		_boxActor->GetProperty()->SetDiffuseColor(hicolor.redF(), hicolor.greenF(), hicolor.blueF());
	}

	/*
	����vtkDataSet���͵����ݵĸ�����ʾ
	*/
	void MeshViewProvider::highLighDataSet(vtkDataSet* dataset)
	{
		_highLightMapper->SetInputData(dataset);
		_highLightMapper->Update();
		_preWindow->reRender();
	}

	/*
	����ʰȡ�㣬��Ԫ�ĸ�����ʾ
	*/
	void MeshViewProvider::highLighSet(QMultiHash<vtkDataSet*, int>* items)
	{
		if (items == nullptr || items->isEmpty())
		{
			clearHighLight();
			return;
		}
		//�ҵ�ӳ���ϵ��������selectItems
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
	����ӳ����Items
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
			//����ʵ��ʰȡ����dataSet�����Ҷ�Ӧ��mapper
			vtkDataSetMapper * mapper = _mapperDataSetHash.key(dataSet);
			//����mapper�Ҷ�Ӧ��kernal
			MeshData::MeshKernal* k = _mapperKernalHash.value(mapper);
			if (k == nullptr)
			{
				continue;
			}
			//��ȡ��ǰkernal��dataSet����
			vtkDataSet * kernalDataSet = k->getMeshData();
			if (_selectModel == ModuleBase::MeshCell)
			{
				//�����µ�Items������kernalDataset���ݣ���Ԫ�����Ÿı�
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
				//�����µ�Items������kernalDataset���ݣ���Ԫ�����Ÿı�
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