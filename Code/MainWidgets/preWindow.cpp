#include "preWindow.h"
#include "meshData/meshSingleton.h"
#include "meshData/meshKernal.h"
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include "ModelData/modelDataSingleton.h"
#include "mainWindow/mainWindow.h"
#include "settings/busAPI.h"
#include "settings/GraphOption.h"
#include "geometryViewProvider.h"
#include "sketchViewProvider.h"
#include "moduleBase/PreWindowInteractorStyle.h"
#include <vtkActor.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <TopoDS.hxx>
#include <TopExp_Explorer.hxx>
#include <IVtkOCC_Shape.hxx>
#include <IVtkTools_ShapeDataSource.hxx>
#include <IVtkTools_DisplayModeFilter.hxx>
#include <vtkSmoothPolyDataFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkRenderWindow.h>
#include <QDebug>
#include <assert.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkCoordinate.h>

namespace MainWidget
{
	PreWindow::PreWindow(GUI::MainWindow* mw, int id, ModuleBase::GraphWindowType t) 
		: ModuleBase::Graph3DWindow(mw, id, t,true)
	{
		_meshData = MeshData::MeshData::getInstance();
		_geometryData = Geometry::GeometryData::getInstance();
		_modelData = ModelData::ModelDataSingleton::getinstance();
		_geoProvider = new GeometryViewProvider(mw, this);
		_sketchProvider = new SketchViewProvider(mw, this);
		this->setWindowTitle(tr("Pre-Window"));
		connect(mw, SIGNAL(updateGeoDispalyStateSig(int, bool)), this, SLOT(updateGeoDispaly(int, bool)));
		connect(mw, SIGNAL(removeGeometryActorSig(int)), this, SLOT(removeGemoActor(int)));
		connect(mw, SIGNAL(updateMeshDispalyStateSig(int, bool)), this, SLOT(updateMeshDispaly(int, bool)));
		connect(mw, SIGNAL(removeMeshActorSig(int)), this, SLOT(removeMeshActor(int)));
		connect(this, SIGNAL(closed()), mw, SIGNAL(closePreWindowSig()));
		connect(mw, SIGNAL(startSketchSig(bool, double*, double*)), this, SLOT(startSketch(bool, double*, double*)));
		connect(this, SIGNAL(RestoreGeoColorSig()), _geoProvider, SLOT(RestoreGeoColor()));
		connect(this, SIGNAL(updateMeshActorSig()), this, SLOT(updateMeshActor()));
		connect(mw, SIGNAL(clearAllHighLight()), this, SLOT(clearAllHighLight()));
		updateMeshActor();
		//updateGeometryActor();
	}
	PreWindow::~PreWindow()
	{
		removeMeshActors();
//		removeGeometryActors();
// 		_meshData->clear();
// 		_geometryData->clear();
// 		_modelData->clear();
		if (_geoProvider != nullptr) delete _geoProvider;
		if (_sketchProvider != nullptr) delete _sketchProvider;
		emit closed();
	}
	void PreWindow::removeMeshActors()
	{
		const int n = _meshActors.size();
		for (int i = 0; i < n; ++i)
		{
			vtkActor* actor = _meshActors.at(i);
			this->RemoveActor(actor);
		}
		_meshActors.clear();
	}
// 	void PreWindow::removeGeometryActors()
// 	{
// 		const int n = _geometryActors.size();
// 		for (int i = 0; i < n; ++i)
// 		{
// 			vtkActor* actor = _geometryActors.at(i);
// 			this->RemoveActor(actor);
// 		}
// 		_geometryActors.clear();
// 	}
	void PreWindow::updateMeshActor()
	{
		removeMeshActors();
		const int n = _meshData->getKernalCount();
		for (int i = 0; i < n; ++i)
		{
			MeshData::MeshKernal* k = _meshData->getKernalAt(i);
			bool visible = k->isVisible();
			vtkDataSet* dataset = k->getMeshData();
			vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
			vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
			mapper->SetInputData(dataset);
			mapper->ScalarVisibilityOff();
			mapper->Update();
			actor->SetMapper(mapper);
			actor->SetVisibility(visible);
			this->AppendActor(actor, ModuleBase::ActorType::D3);
			_meshActors.append(actor);
			updateDisplayModel();
		}
	}
	void PreWindow::updateGeometryActor()
	{
		_geoProvider->updateGeoActors();

	}
	void PreWindow::updateGeoDispaly(int index, bool display)
	{
		Geometry::GeometrySet* s = _geometryData->getGeometrySetAt(index);
		if (s == nullptr) return;
		_geoProvider->updateDiaplayStates(s, display);
	}
	void PreWindow::removeGemoActor(const int index)
	{
		Geometry::GeometrySet* set = _geometryData->getGeometrySetAt(index);
		if (set == nullptr) return;
		_geoProvider->removeActors(set);
		_geometryData->removeTopGeometrySet(set);
		delete set;

	}
	void PreWindow::updateMeshDispaly(int index, bool display)
	{
		assert(index >= 0 && index < _meshActors.size());
		vtkActor* ac = _meshActors.at(index);
		if (ac == nullptr) return;
		ac->SetVisibility(display);
		_renderWindow->Render();
//		resetCamera();
	}
	void PreWindow::removeMeshActor(const int index)
	{
		assert(index >= 0 && index < _meshActors.size());
		vtkActor* actor = _meshActors.at(index);
		RemoveActor(actor);
		_meshActors.removeAt(index);
		_renderWindow->Render();
//		resetCamera();
	}
	
	Geometry::GeometrySet* PreWindow::getSelectedGeoSet()
	{
		Geometry::GeometrySet* set = _geometryData->getGeometrySetAt(_selectedGeoIndex);
		return set;
	}
	void PreWindow::setDisplay(QString m)
	{
		if (m.toLower() == "node") _displayModel = Node;
		else if (m.toLower() == "wireframe") _displayModel = WireFrame;
		else if (m.toLower() == "surface") _displayModel = Surface;
		else if (m.toLower() == "surfacewithedge") _displayModel = SurfaceWithEdge;
		updateDisplayModel();
	}

	/*void PreWindow::setGeometryDisplay(QString m)
	{
		if (m.toLower() == "point") _displayGeometryModel =GeometryPoint;
		else if (m.toLower() == "curve") _displayGeometryModel = GeometryCurve;
		else if (m.toLower() == "surface") _displayGeometryModel = GeometrySurface;
		updateDisplayGeometryModel();
	}*/

	void PreWindow::updateDisplayModel()
	{
		const int n = _meshActors.size();
		QColor c;
		float size;
		Setting::GraphOption* goptions = Setting::BusAPI::instance()->getGraphOption();

		for (int i = 0; i < n; ++i)
		{
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
				c = Setting::BusAPI::instance()->getGraphOption()->getMeshFaceColor();
				prop->SetDiffuseColor(c.redF(), c.greenF(), c.blueF());
				prop->SetRepresentationToSurface();
				prop->EdgeVisibilityOff();
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
		this->_renderWindow->Render();
	}

/*
	void PreWindow::updateDisplayGeometryModel()
	{
		//QList<vtkActor*> ac;
		int setcount= _geometryData->getGeometrySetCount();
		for (int i = 0; i < setcount;++i)
		{
			Geometry::GeometrySet*set = _geometryData->getGeometrySetByID(i);

			switch (_displayModel)
			{
				case DisplayGeometryModel::GeometryPoint:
							
					//emit _mainWindow->highLightGeometryPointSig(set, id, ac);
					break;
				case DisplayGeometryModel::GeometryCurve:
					//emit _mainwindow->highLightGeometryEdgeSig(set, id, ac);
					break;
				case DisplayGeometryModel::GeometrySurface:
					//emit _mainwindow->highLightGeometryFaceSig(set, id, ac);
					break;

				default:
					break;
			}

		}
		this->_renderWindow->Render();
	}*/
	void PreWindow::setSelectModel(int mode)
	{
		ModuleBase::SelectModel m = (ModuleBase::SelectModel) mode;
		int n = 0;
// 		n = _geometryActors.size();
// 		for (int i = 0; i < n; ++i) _geometryActors.at(i)->PickableOff();
		n = _meshActors.size();
		for (int i = 0; i < n; ++i) _meshActors.at(i)->PickableOff();
		switch (m)
		{
		case ModuleBase::None:
			break;
		case ModuleBase::GeometryBody:
// 			n = _geometryActors.size();
// 			for (int i = 0; i < n; ++i) _geometryActors.at(i)->PickableOn();
			break;
		case ModuleBase::MeshNode:
			n = _meshActors.size();
			for (int i = 0; i < n; ++i) _meshActors.at(i)->PickableOn();
			break;
		case ModuleBase::MeshCell:
			n = _meshActors.size();
			for (int i = 0; i < n; ++i) _meshActors.at(i)->PickableOn();
			break;
		default:
			break;
		}
	}
	void PreWindow::updateGraphOption()
	{
		updateDisplayModel();
		_geoProvider->updateGraphOption();
// 		QColor c(0,0,0) /*= Setting::BusAPI::instance()->getGraphOption()->getGeometryColor()*/;
// 		const int n = _geometryActors.size();
// 		for (int i = 0; i < n; ++i)
// 		{
// 			vtkActor* actor = _geometryActors.at(i);
// 			actor->GetProperty()->SetDiffuseColor(c.redF(), c.greenF(), c.blueF());
// 		}
		ModuleBase::Graph3DWindow::updateGraphOption();
		_renderWindow->Render();
	}

	void PreWindow::startSketch(bool start, double* loc, double* dir)
	{
		if (start)
		{
			vtkCamera* camera = _render->GetActiveCamera();
			gp_Ax3* ax3 = Geometry::GeometryData::getInstance()->getSketchPlane();
			gp_Dir y = ax3->YDirection();
			camera->SetViewUp(y.X(), y.Y(), y.Z());
			camera->SetPosition(dir[0], dir[1], dir[2]);
			camera->SetFocalPoint(0, 0, 0);
			_interactionStyle->setSelectModel((int)ModuleBase::DrawSketch);
			this->resetCamera();
		}
		else
		{
			_interactionStyle->setSelectModel((int)ModuleBase::None);
			_sketchProvider->setSketchType(ModuleBase::SketchNone);
		}
		_sketchProvider->showSketchPlane(start);
		this->resetCamera();
	}

	void PreWindow::setSketchType(ModuleBase::SketchType t)
	{
		_sketchProvider->setSketchType(t);
	}

	void PreWindow::clearAllHighLight()
	{
		emit RestoreGeoColorSig();
		clearHighLight();
	}

	QMultiHash<Geometry::GeometrySet*, int> PreWindow::getGeoSelectItems()
	{
		return _geoProvider->getGeoSelectItems();
	}



}
