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
#include <vtkPolyData.h>
#include <vtkInteractorStyleRubberBand2D.h>
#include <QKeyEvent>
#include <QCoreApplication>
#include <vtkCell.h>
#include <vtkCoordinate.h>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QList>
#include <vtkDataSet.h>
#include "geometry/geometryData.h"

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
		//
		this->SelectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
		this->SelectedActor = vtkSmartPointer<vtkActor>::New();
		this->SelectedActor->SetMapper(SelectedMapper);
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

	/*
	相关的信号槽连接
	*/
	void PropPickerInteractionStyle::connectToMainWindow(GUI::MainWindow* mw, QWidget* p)
	{
		_mainWindow = mw;
		_parent = p;
		connect(this, SIGNAL(clearAllHighLight()), mw, SIGNAL(clearAllHighLight()));
		connect(mw, SIGNAL(selectModelChangedSig(int)), this, SLOT(setSelectModel(int)));
		//		connect(this, SIGNAL(dispalyInfo(DataProperty::PropertyList*)), mw, SIGNAL(updateProperty(DataProperty::PropertyList*)));
	}

	/*
	设置当前拾取模型选择模式
	*/
	void PropPickerInteractionStyle::setSelectModel(int m)
	{
		_selectModel = (SelectModel)m;
		this->CurrentMode = 0;
		if (_actor != nullptr)
			_actor->GetProperty()->DeepCopy(_property);
		_actor = nullptr;
		_preGeoSeltctActor = nullptr;
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
		}
	}

	/*
	按键触发
	*/
	void PropPickerInteractionStyle::keyEvent(int type, QKeyEvent* e)
	{
		if (type == 0) //press
		{
			if (e->key() == Qt::Key_Control)

			{
				_ctrlPressed = true;
			}
			else if (e->key() == Qt::Key_Up)
				OnKeyBoardUp();
			else if (e->key() == Qt::Key_Down)
				OnKeyBoardDown();
			else if (e->key() == Qt::Key_Alt)
				_altPressed = true;
		}
		else if (type == 1) //release
		{
			if (e->key() == Qt::Key_Control)
			{
				_ctrlPressed = false;
			}
			else if (e->key() == Qt::Key_Alt)
				_altPressed = false;
		}
	}

	/*
	鼠标左键按下触发事件
	*/
	void PropPickerInteractionStyle::OnLeftButtonDown()
	{
		_leftButtonDown = true;
		_mouseMoved = false;
		vtkInteractorStyleRubberBandPick::OnLeftButtonDown();
		int* startPos = this->GetInteractor()->GetEventPosition();
		_startPos[0] = startPos[0]; _startPos[1] = startPos[1];
		//
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
			//boxSelectMeshNode();
			break;
		case  ModuleBase::BoxMeshCell:
			//boxSelectMeshCell();
			break;
		default:
			break;
		}
	}

	/*
	鼠标左键抬起触发事件
	*/
	void PropPickerInteractionStyle::OnLeftButtonUp()
	{
		vtkInteractorStyleRubberBandPick::OnLeftButtonUp();
		if (_selectModel == None && !_mouseMoved)
			emit this->clearAllHighLight();

		if ((_selectModel != BoxMeshCell) && (_selectModel != BoxMeshNode) && (_selectModel != DrawSketch)) return;
		if (this->CurrentMode == 0) return;
		//		_selectItemIDs->SetNumberOfValues(0);
		_selectItems.clear();
		int* endPos = this->GetInteractor()->GetEventPosition();
		_endPos[0] = endPos[0]; _endPos[1] = endPos[1];
		//		qDebug() << "end  " << _endPos[0] << "   " << _endPos[1];
		if (_selectModel != DrawSketch)
		{
			vtkActor* ac = nullptr;
			vtkAreaPicker* areaPicker = dynamic_cast<vtkAreaPicker*>(this->GetInteractor()->GetPicker());
			ac = areaPicker->GetActor();
			if (ac == nullptr) return;
		}
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
		_mouseMoved = false;
		_leftButtonDown = false;
	}

	/*
	鼠标移动
	*/
	void PropPickerInteractionStyle::OnMouseMove()
	{
		_mouseMoved = true;
		vtkInteractorStyleRubberBandPick::OnMouseMove();
		if ((_selectModel == BoxMeshCell) || (_selectModel == BoxMeshNode) || (_selectModel == DrawSketch) /*&& _leftButtonDown*/)
		{
			int* endPos = this->GetInteractor()->GetEventPosition();
			_endPos[0] = endPos[0]; _endPos[1] = endPos[1];
			//
			if (_selectModel == DrawSketch)
			{
				int* endPos = this->GetInteractor()->GetEventPosition();
				_coordinate->SetCoordinateSystemToDisplay();
				_coordinate->SetValue(endPos[0], endPos[1], 0);
				double* d = _coordinate->GetComputedWorldValue(_renderer);
				emit mouseMovePoint(d);
			}
		}
		else if (_selectModel == GeometryCurve || _selectModel == GeometrySurface || _selectModel == GeometryPoint || _selectModel == GeometryBody ||
			_selectModel == GeometryWinCurve || _selectModel == GeometryWinSurface || _selectModel == GeometryWinPoint || _selectModel == GeometryWinBody)
		{
			clickSelectGeometry(true);
		}
	}

	/*
	几何相关的鼠标选取高亮触发函数
	*/
	void PropPickerInteractionStyle::clickSelectGeometry(bool preSelect)
	{
		if (!preSelect)
		{
			emit selectGeometry(_ctrlPressed);
			return;
		}

		int* clickPos = this->GetInteractor()->GetEventPosition();
		int success = -1, index = -1; vtkActor* actor = nullptr;

		if (_selectModel == GeometryPoint || _selectModel == GeometryWinPoint)
		{
			vtkSmartPointer<vtkPointPicker> picker = vtkSmartPointer<vtkPointPicker>::New();
			success = picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());
			if (0 != success)
			{
				index = picker->GetPointId();
				actor = picker->GetActor();
			}
		}
		else
		{
			vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
			if (_selectModel == GeometryCurve || _selectModel == GeometryWinCurve)
				picker->SetTolerance(0.0025);
			success = picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());
			if (0 != success)
			{
				index = picker->GetCellId();
				actor = picker->GetActor();
			}
		}
		emit preSelectGeometry(actor, index);
	}

	/*
	网格模型选取高亮点触发函数
	*/
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

	/*
	网格相关的拾取高亮单元的触发函数
	*/
	void PropPickerInteractionStyle::clickSelectMeshCell()
	{
		emit higtLightActorDisplayPoint(false);
		int* clickPos = this->GetInteractor()->GetEventPosition();
		vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
		int ok = picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());
		int id = picker->GetCellId();
		if (picker->GetCellId() >= 0)
		{
			if (!_ctrlPressed)
				_selectItems.clear();
			int id = picker->GetCellId();
			vtkDataSet* dataset = picker->GetActor()->GetMapper()->GetInput();
			_selectItems.insert(dataset, id);
			emit highLight(&_selectItems);
		}
	}

	/*
	网格显示点的相关信息
	*/
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

	/*
	网格相关的框选点的选取高亮触发函数
	*/
	void PropPickerInteractionStyle::boxSelectMeshNode()
	{
		emit higtLightActorDisplayPoint(true);
		_selectItems.clear();
		// Forward events
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

	/*
	网格模型相关的单元框选取高亮触发函数
	*/
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

	/*
	网格框选获取当前框选的范围区域值
	*/
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

	/*
	根据框选的范围区域值计算出当前实体下的选取到的所有单元或点的集合
	*/
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
				double coor[3];
				dataSet->GetPoint(i, coor);
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

	/*
	判断当前点是否在框选的范围区域值内
	*/
	bool PropPickerInteractionStyle::isPointInRange(int* p, int* range)
	{
		bool isx = p[0] >= range[0] && p[0] <= range[1] ? true : false;
		bool isy = p[1] >= range[2] && p[1] <= range[3] ? true : false;

		return isx && isy;

	}

	/*
	鼠标中间滚轮按下事件
	*/
	void PropPickerInteractionStyle::OnMiddleButtonDown()
	{
		_mouseMoved = false;
		_leftButtonDown = false;
		vtkInteractorStyleTrackballCamera::OnMiddleButtonDown();
		if ((_selectModel != BoxMeshCell) && (_selectModel != BoxMeshNode)) return;

		if (this->CurrentMode == 0)
			this->CurrentMode = 1;
		else
			this->CurrentMode = 0;
	}

	/*
	鼠标中间滚轮抬起事件
	*/
	void PropPickerInteractionStyle::OnMiddleButtonUp()
	{
		_mouseMoved = false;
		_leftButtonDown = false;
		vtkInteractorStyleTrackballCamera::OnMiddleButtonUp();
		if (_selectModel == DrawSketch)
		{
			emit mouseMiddleUp();
			return;
		}

		if ((_selectModel != BoxMeshCell) && (_selectModel != BoxMeshNode)) return;
		this->CurrentMode = 1;
	}

	/*
	鼠标中间滚轮向前事件
	*/
	void PropPickerInteractionStyle::OnMouseWheelForward()
	{

		vtkInteractorStyleRubberBandPick::OnMouseWheelForward();
		emit mouseWhellMove();
	}

	/*
	鼠标中间滚轮向后事件
	*/
	void PropPickerInteractionStyle::OnMouseWheelBackward()
	{
		vtkInteractorStyleRubberBandPick::OnMouseWheelBackward();
		emit mouseWhellMove();
	}

	/*
	鼠标右键按下触发事件
	*/
	void PropPickerInteractionStyle::OnRightButtonDown()
	{
		//判断是否选中几何，bool
		//bool isSelectBody = false;
		//if (_preGeoSeltctActor != nullptr) return;
		Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
		if (data->getGeometrySetCount() > 0)
		{
			emit rightDownMenu();
		}


	}

	/*
	鼠标右键抬起触发事件
	*/
	void PropPickerInteractionStyle::OnRightButtonUp()
	{
		_mouseMoved = false;
		_leftButtonDown = false;
		vtkInteractorStyleRubberBandPick::OnRightButtonUp();
		emit mouseRightUp();
	}

	/*
	键盘上的向上箭头按键触发事件
	*/
	void PropPickerInteractionStyle::OnKeyBoardUp()
	{
		/*switch (_selectModel)
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
		QList<double*> pickeds = _actorPickedPoints.values(ac);
		for (auto picked : pickeds)
		emit selectGeometry(ac, picked, _ctrlPressed);
		return;
		}
		}*/
	}

	/*
	键盘上的向下箭头按键事件
	*/
	void PropPickerInteractionStyle::OnKeyBoardDown()
	{
		/*switch (_selectModel)
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
		QList<double*> setPickeds = _actorPickedPoints.values(ac);
		for (auto picked:setPickeds)
		emit selectGeometry(ac, picked, _ctrlPressed);
		return;
		}
		}*/
	}

	/*
	设置渲染窗口
	*/
	void PropPickerInteractionStyle::setRenderWindow(vtkRenderWindow* w)
	{
		_renderWindow = w;
	}

	/*
	设置渲染器
	*/
	void PropPickerInteractionStyle::setRender(vtkRenderer* r)
	{
		_renderer = r;
	}

	/*
	根据颜色值的比较来判断当前actor是否已经高亮（未使用）
	*/
	bool PropPickerInteractionStyle::isActorHightLighted(vtkActor* ac)
	{
		if (ac == nullptr) return false;
		double* c = ac->GetProperty()->GetColor();
		QColor hc = Setting::BusAPI::instance()->getGraphOption()->getHighLightColor();
		double r = hc.redF();
		double g = hc.greenF();
		double b = hc.blueF();

		bool same = true;
		double absu = fabs(r - c[0]);
		if (absu > 0.000001) same = false;
		absu = fabs(g - c[1]);
		if (absu > 0.000001) same = false;
		absu = fabs(b - c[2]);
		if (absu > 0.000001) same = false;
		return same;
	}
}
