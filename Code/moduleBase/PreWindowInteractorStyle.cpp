#include "PreWindowInteractorStyle.h"
#include "mainWindow/mainWindow.h"
#include "DataProperty/DataBase.h"
#include "meshData/meshSingleton.h"
#include "meshData/meshKernal.h"
#include "settings/busAPI.h"
#include "settings/GraphOption.h"
#include <vtkProperty.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkPropPicker.h>
#include <vtkPointPicker.h>
#include <vtkCellPicker.h>
#include <vtkActor.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkExtractSelection.h>
#include <vtkMapper.h>
#include <vtkDataSet.h>
#include <vtkDataSetMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkRenderer.h>
#include <vtkAreaPicker.h>
#include <vtkExtractGeometry.h>
#include <vtkPlanes.h>
#include <vtkCoordinate.h>
#include <QKeyEvent>
#include <QCoreApplication>
#include <vtkCell.h>
#include <vtkCoordinate.h>
#include <QDebug>

namespace ModuleBase
{
	PropPickerInteractionStyle::PropPickerInteractionStyle()
	{
		_actor = nullptr;
		_property = vtkProperty::New();
//		_selectItemIDs = vtkSmartPointer<vtkIdTypeArray>::New();
//		_selectItemIDs->SetNumberOfComponents(1);
		_meshData = MeshData::MeshData::getInstance();
		_coordinate = vtkCoordinate::New();
	}
	PropPickerInteractionStyle* PropPickerInteractionStyle::New()
	{
		return new PropPickerInteractionStyle;
	}
	PropPickerInteractionStyle::~PropPickerInteractionStyle()
	{
		if (_dataProp != nullptr) delete _dataProp;
		_property->Delete();
		_coordinate->Delete();
	}
	void PropPickerInteractionStyle::connectToMainWindow(GUI::MainWindow* mw, QWidget* p)
	{
		_mainWindow = mw;
		_parent = p;
		connect(this, SIGNAL(clearAllHighLight()), mw, SIGNAL(clearAllHighLight()));
		connect(mw, SIGNAL(selectModelChangedSig(int)), this, SLOT(setSelectModel(int)));
//		connect(this, SIGNAL(dispalyInfo(DataProperty::PropertyList*)), mw, SIGNAL(updateProperty(DataProperty::PropertyList*)));
	}
	void PropPickerInteractionStyle::setSelectModel(int m)
	{
		_selectModel = (SelectModel)m;
		this->CurrentMode = 0;
		if (_actor != nullptr)
			_actor->GetProperty()->DeepCopy(_property);
		_actor = nullptr;
// 		if (_selectModel == None)
// 		{
// 			_selectItemIDs->SetNumberOfValues(0);
// 		}
		_selectItems.clear();
		emit grabKeyBoard(false);
		switch (_selectModel)
		{
		case ModuleBase::MeshNode:
		case ModuleBase::MeshCell:
		case ModuleBase::GeometryWinPoint:
		case ModuleBase::GeometryWinCurve:
		case ModuleBase::GeometryWinSurface:
		case ModuleBase::GeometryWinBody:
			emit grabKeyBoard(true);
			break;
		case ModuleBase::GeometryPoint:
		case ModuleBase::GeometryCurve:
		case ModuleBase::GeometrySurface:
		case ModuleBase::GeometryBody:
			break;
		case ModuleBase::BoxMeshNode:
		case ModuleBase::BoxMeshCell:
		case ModuleBase::DrawSketch:
			this->CurrentMode = 1;
			break;
	/*	default: break;*/
		}
	}
	void PropPickerInteractionStyle::keyEvent(int type, QKeyEvent* e)
	{
		if (type == 0) //press
		{
			if (e->key() == Qt::Key_Control)

			{
				_ctrlPressed = true;
				qDebug() << "ctrl press";
			}
			else if (e->key() == Qt::Key_Up)
				OnKeyBoardUp();
			else if (e->key() == Qt::Key_Down)
				OnKeyBoardDown();
		}
		else if (type == 1) //release
		{
			if (e->key() == Qt::Key_Control)
			{
				_ctrlPressed = false;
				qDebug() << "ctrl release";
			}
		}
	}
	void PropPickerInteractionStyle::OnLeftButtonDown()
	{
		int* startPos = this->GetInteractor()->GetEventPosition();
		_startPos[0] = startPos[0]; _startPos[1] = startPos[1];
//		qDebug() << "start  " << _startPos[0] << "   " << _startPos[1];
		_selected = false;
		
		
		switch (_selectModel)
		{
		case ModuleBase::None: 
			
			break;
		case ModuleBase::GeometryBody:
		case ModuleBase::GeometryCurve:
		case ModuleBase::GeometrySurface:
		case ModuleBase::GeometryPoint:
		case ModuleBase::GeometryWinPoint:
		case ModuleBase::GeometryWinCurve:
		case ModuleBase::GeometryWinSurface:
		case ModuleBase::GeometryWinBody:
			clickSelectGeometry();
			break;
		case ModuleBase::MeshNode:
			clickSelectMeshNode();
			break;
		case ModuleBase::MeshCell:
			clickSelectMeshCell();
			break;
 		case ModuleBase::BoxMeshNode:
// 			boxSelectMeshNode();
// 			break;
 		case  ModuleBase::BoxMeshCell:
		
// 			boxSelectMeshCell();
 			break;
		default:
			break;
		}

		vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
	}
	void PropPickerInteractionStyle::clickSelectGeometry()
 	{
		_tempActorContainer.clear();
		_currentTempIndex = 0;

		if (_preGeoSeltctActor != nullptr)
		{
			emit selectGeometry(_preGeoSeltctActor,_ctrlPressed);
			_tempActorContainer.append(_preGeoSeltctActor);
		}

		int* clickPos = this->GetInteractor()->GetEventPosition();

		vtkSmartPointer<vtkPropPicker> picker = vtkSmartPointer<vtkPropPicker>::New();
		int p = picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());

		if (p == 0) return;

		vtkActor* actor = picker->GetActor();
		if (_preGeoSeltctActor == nullptr)
			emit selectGeometry(actor,_ctrlPressed);
		//选取所有的actor
		while (p != 0)
		{
			_tempActorContainer.append(actor);
			actor->SetPickable(false);

			p = picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());
			if (p != 0)
				actor = picker->GetActor();
		}
		//还原actor选择状态
		for (vtkActor* a : _tempActorContainer)
			a->SetPickable(true);
		_preGeoSeltctActor = nullptr;

	}
	void PropPickerInteractionStyle::clickSelectMeshNode()
	{
		emit higtLightActorDisplayPoint(true);
		int* clickPos = this->GetInteractor()->GetEventPosition();
		vtkSmartPointer<vtkPointPicker> picker = vtkSmartPointer<vtkPointPicker>::New();
		picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());
		if (picker->GetPointId() >= 0)
		{
			if (!_ctrlPressed)
				_selectItems.clear();
			int id = picker->GetPointId();
			vtkDataSet* dataset = picker->GetActor()->GetMapper()->GetInput();
			_selectItems.insert(dataset, id);
			emit highLight(&_selectItems);
		}
	}
	void PropPickerInteractionStyle::clickSelectMeshCell()
	{
		emit higtLightActorDisplayPoint(false);
		int* clickPos = this->GetInteractor()->GetEventPosition();
		vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
		picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());
		int id = picker->GetCellId();
		qDebug() << id;
		if (picker->GetCellId() >= 0)
		{
			if (!_ctrlPressed) 
				_selectItems.clear();
			int id =picker->GetCellId();
			vtkDataSet* dataset = picker->GetActor()->GetMapper()->GetInput();
			_selectItems.insert(dataset, id);
			emit highLight(&_selectItems);
		}
	}

	void PropPickerInteractionStyle::boxSelectMeshNode()
	{
		emit higtLightActorDisplayPoint(true);
		_selectItems.clear();
		int range[4];
		this->getBoxRange(range);
		const int nk = _meshData->getKernalCount();
		
		
		for (int i = 0; i < nk; ++i)
		{
			if (_selected) break;
			MeshData::MeshKernal* k = _meshData->getKernalAt(i);
			if (!k->isVisible()) continue;
			this->selectMeshKernal(k, range);
		}
		emit highLight(&_selectItems);

	}

	void PropPickerInteractionStyle::boxSelectMeshCell()
	{
		emit higtLightActorDisplayPoint(false);
		_selectItems.clear();
	
		int range[4];
		this->getBoxRange(range);
		const int nk = _meshData->getKernalCount();


		for (int i = 0; i < nk; ++i)
		{
			if (_selected) break;
			MeshData::MeshKernal* k = _meshData->getKernalAt(i);
			if (!k->isVisible()) continue;
			this->selectMeshKernal(k, range);
		}
		emit highLight(&_selectItems);
	}

	void PropPickerInteractionStyle::displayNodeInfo(vtkDataSet* dataset, int id)
	{
		if (_dataProp != nullptr)
		{
			delete _dataProp;
			_dataProp = nullptr;
		}
		if (dataset == nullptr || id <= 0)
		{
			emit dispalyInfo(nullptr);
			return;
		}
		_dataProp = new DataProperty::DataBase;
		double* c = dataset->GetPoint(id);
		_dataProp->appendProperty(tr("Node ID"), id);
		_dataProp->appendProperty(tr("Coordinate"), c);
		emit dispalyInfo(_dataProp);
	}

	void PropPickerInteractionStyle::OnLeftButtonUp()
	{
		vtkInteractorStyleRubberBandPick::OnLeftButtonUp();
		if (_selectModel == None)
			emit this->clearAllHighLight();

		if ((_selectModel != BoxMeshCell) && (_selectModel != BoxMeshNode) && (_selectModel != DrawSketch)) return;
		if (this->CurrentMode == 0) return;
//		_selectItemIDs->SetNumberOfValues(0);
		_selectItems.clear();
		int* endPos = this->GetInteractor()->GetEventPosition();
		_endPos[0] = endPos[0]; _endPos[1] = endPos[1];
//		qDebug() << "end  " << _endPos[0] << "   " << _endPos[1];
		switch (_selectModel)
		{
		case ModuleBase::BoxMeshNode:
			boxSelectMeshNode();
			break;
		case ModuleBase::BoxMeshCell:
			boxSelectMeshCell();
			break;
		case  ModuleBase::DrawSketch:
			_coordinate->SetCoordinateSystemToDisplay();
			_coordinate->SetValue(endPos[0], endPos[1], 0);
			double* d = _coordinate->GetComputedWorldValue(_renderer);
			emit mouseReleasePoint(d);
			break;
		}

	}

	void PropPickerInteractionStyle::OnMouseMove()
	{
		vtkInteractorStyleRubberBandPick::OnMouseMove();
		if ((_selectModel == BoxMeshCell) || (_selectModel == BoxMeshNode) || (_selectModel == DrawSketch) )
		{
			int* endPos = this->GetInteractor()->GetEventPosition();
			_endPos[0] = endPos[0]; _endPos[1] = endPos[1];
			if (_selectModel == DrawSketch)
			{
				_coordinate->SetCoordinateSystemToDisplay();
				_coordinate->SetValue(endPos[0], endPos[1], 0);
				double* d = _coordinate->GetComputedWorldValue(_renderer);
				emit mouseMovePoint(d);
			}
		}
		else if (_selectModel == GeometryCurve || _selectModel == GeometrySurface || _selectModel == GeometryPoint || 
			     _selectModel == GeometryWinCurve || _selectModel == GeometryWinSurface || _selectModel == GeometryWinPoint)
		{
			
			QColor ncolor;
			switch (_selectModel)
			{
			case ModuleBase::GeometrySurface:
			case ModuleBase::GeometryWinSurface:
				ncolor = Setting::BusAPI::instance()->getGraphOption()->getGeometrySurfaceColor(); break;
			case ModuleBase::GeometryCurve:
			case ModuleBase::GeometryWinCurve:
				ncolor = Setting::BusAPI::instance()->getGraphOption()->getGeometryCurveColor(); break;
			case ModuleBase::GeometryPoint:
			case ModuleBase::GeometryWinPoint:
				ncolor = Setting::BusAPI::instance()->getGraphOption()->getGeometryPointColor(); break;
			default:
				break;
			}
			 

			if (isActorHightLighted(_preGeoSeltctActor)) return;

			if (_preGeoSeltctActor != nullptr)
			{
				_preGeoSeltctActor->GetProperty()->SetColor(ncolor.redF(), ncolor.greenF(), ncolor.blueF());
				if (_selectModel == GeometrySurface || _selectModel == GeometryWinSurface)
				{
					double op = Setting::BusAPI::instance()->getGraphOption()->getTransparency();
					_preGeoSeltctActor->GetProperty()->SetOpacity(1.0 - op / 100.0);
				}
				_preGeoSeltctActor = nullptr;
				_renderWindow->Render();
			}
			
			int* endPos = this->GetInteractor()->GetEventPosition();
			vtkSmartPointer<vtkPropPicker> picker = vtkSmartPointer<vtkPropPicker>::New();
			int p = picker->Pick(endPos[0], endPos[1], 0, this->GetDefaultRenderer());
			if (p == 0) return;

			vtkActor* ac = picker->GetActor();
			if (isActorHightLighted(ac)) return;

		
			_preGeoSeltctActor = ac;
			QColor highLight = Setting::BusAPI::instance()->getGraphOption()->getPreHighLightColor();
			_preGeoSeltctActor->GetProperty()->SetColor(highLight.redF(), highLight.greenF(), highLight.blueF());
			ac->GetProperty()->SetOpacity(1.0);
			_renderWindow->Render();
		}
	}

	void PropPickerInteractionStyle::getBoxRange(int* range)
	{
		// range ---- xmin xmax ymin ymax
		range[0] = _startPos[0];
		range[1] = _endPos[0];
		range[2] = _startPos[1];
		range[3] = _endPos[1];

		int temp;
		if (range[0] > range[1])
		{
			temp = range[0];
			range[0] = range[1];
			range[1] = temp;
		}
		if (range[2] > range[3])
		{
			temp = range[2];
			range[2] = range[3];
			range[3] = temp;
		}
	}

	void PropPickerInteractionStyle::selectMeshKernal(MeshData::MeshKernal* k, int *range)
	{
		
		vtkRenderer* render = this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer();
		vtkSmartPointer<vtkCoordinate> coordinate = vtkSmartPointer<vtkCoordinate>::New();
		coordinate->SetCoordinateSystemToWorld();
		int* va = coordinate->GetComputedDisplayValue(render);

		vtkDataSet* dataSet = k->getMeshData();
		if (_selectModel == BoxMeshNode)
		{
			const int npoint = dataSet->GetNumberOfPoints();
			for (int i = 0; i < npoint; ++i)
			{
				double* coor = dataSet->GetPoint(i);
				coordinate->SetValue(coor);
				int * va = coordinate->GetComputedDisplayValue(render);
				if (isPointInRange(va, range))
					_selectItems.insert(dataSet, i);
			}
// 			if (_selectItemIDs->GetNumberOfValues() > 0)
// 			{
// 				_selected = true;
// 				emit highLight(dataSet, _selectItemIDs);
// 			}

		}
		else if (_selectModel == BoxMeshCell)
		{
			const int ncell = dataSet->GetNumberOfCells();
			for (int i = 0; i < ncell; ++i)
			{
				vtkCell* cell = dataSet->GetCell(i);
				double pcenter[3] = { 0 };
				cell->GetParametricCenter(pcenter);
				int subid; double coor[3]; double w[100];
				cell->EvaluateLocation(subid, pcenter, coor, w);
				
				
				coordinate->SetValue(coor);
				int * va = coordinate->GetComputedDisplayValue(render);
				if (isPointInRange(va, range))
					_selectItems.insert(dataSet, i);
			}
// 			if (_selectItemIDs->GetNumberOfValues() > 0)
// 			{
// 				_selected = true;
// 				emit highLight(dataSet, _selectItemIDs);
// 			}
		}
	}

	bool PropPickerInteractionStyle::isPointInRange(int* p, int* range)
	{
		bool isx = p[0] >= range[0] && p[0] <= range[1] ? true : false;
		bool isy = p[1] >= range[2] && p[1] <= range[3] ? true : false;

		return isx && isy;

	}

	void PropPickerInteractionStyle::OnMiddleButtonDown()
	{
		vtkInteractorStyleTrackballCamera::OnMiddleButtonDown();
		if ((_selectModel != BoxMeshCell) && (_selectModel != BoxMeshNode)) return;

		if (this->CurrentMode == 0)
			this->CurrentMode = 1;
		else
			this->CurrentMode = 0;
	}

	void PropPickerInteractionStyle::OnMiddleButtonUp()
	{
		vtkInteractorStyleTrackballCamera::OnMiddleButtonUp();
		if (_selectModel == DrawSketch)
		{
			emit mouseMiddleUp();
			return;
		}

		if ((_selectModel != BoxMeshCell) && (_selectModel != BoxMeshNode)) return;
		this->CurrentMode = 1;
	}

	void PropPickerInteractionStyle::setRenderWindow(vtkRenderWindow* w)
	{
		_renderWindow = w;
	}

	bool PropPickerInteractionStyle::isActorHightLighted(vtkActor* ac)
	{
		if (ac == nullptr) return false;
		double* c = ac->GetProperty()->GetColor();
		QColor hc = Setting::BusAPI::instance()->getGraphOption()->getHighLightColor();
		double r = hc.redF();
		double g = hc.greenF();
		double b = hc.blueF();

		bool same = true;
		double absu = abs(r - c[0]);
		if (absu > 0.000001) same = false;
		absu = abs(g - c[1]);
		if (absu > 0.000001) same = false;
		absu = abs(b - c[2]);
		if (absu > 0.000001) same = false;
	   return same;
	}

	void PropPickerInteractionStyle::setRender(vtkRenderer* r)
	{
		_renderer = r;
	}

	void PropPickerInteractionStyle::OnMouseWheelForward()
	{
		
		vtkInteractorStyleRubberBandPick::OnMouseWheelForward();
		emit mouseWhellMove();
	}

	void PropPickerInteractionStyle::OnMouseWheelBackward()
	{
		vtkInteractorStyleRubberBandPick::OnMouseWheelBackward();
		emit mouseWhellMove();
	}

	void PropPickerInteractionStyle::OnKeyBoardUp()
	{
		switch (_selectModel)
		{
		case ModuleBase::GeometryBody:
		case ModuleBase::GeometryCurve:
		case ModuleBase::GeometrySurface:
		case ModuleBase::GeometryPoint:
		case ModuleBase::GeometryWinPoint:
		case ModuleBase::GeometryWinCurve:
		case ModuleBase::GeometryWinSurface:
		case ModuleBase::GeometryWinBody:
			const int n = _tempActorContainer.size();
			_currentTempIndex--;
			if (_currentTempIndex < 0)
				_currentTempIndex = n - 1;
			if (_currentTempIndex >= 0 && _currentTempIndex < n)
			{
				vtkActor* ac = _tempActorContainer.at(_currentTempIndex);
				emit selectGeometry(ac, _ctrlPressed);
				return;
			}
		}
	}

	void PropPickerInteractionStyle::OnKeyBoardDown()
	{
		switch (_selectModel)
		{
		case ModuleBase::GeometryBody:
		case ModuleBase::GeometryCurve:
		case ModuleBase::GeometrySurface:
		case ModuleBase::GeometryPoint:
		case ModuleBase::GeometryWinPoint:
		case ModuleBase::GeometryWinCurve:
		case ModuleBase::GeometryWinSurface:
		case ModuleBase::GeometryWinBody:
			const int n = _tempActorContainer.size();
			_currentTempIndex++;
			if (_currentTempIndex >= n)
				_currentTempIndex = 0;
			if (_currentTempIndex >= 0 && _currentTempIndex < n)
			{
				vtkActor* ac = _tempActorContainer.at(_currentTempIndex);
				emit selectGeometry(ac, _ctrlPressed);
				return;
			}
		}
	}

	void PropPickerInteractionStyle::OnRightButtonUp()
	{
		vtkInteractorStyleRubberBandPick::OnRightButtonUp();
		emit mouseRightUp();
	}

	
}
