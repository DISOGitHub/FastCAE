#include "preWindow.h"
// #include "meshData/meshSingleton.h"
// #include "meshData/meshKernal.h"
#include "geometry/geometryData.h"
//#include "geometry/geometrySet.h"
#include "geometry/GeoComponent.h"
#include "ModelData/modelDataSingleton.h"
#include "mainWindow/mainWindow.h"
// #include "settings/busAPI.h"
// #include "settings/GraphOption.h"
#include "geometryViewProvider.h"
#include "meshViewProvider.h"
#include "sketchViewProvider.h"
#include "moduleBase/PreWindowInteractorStyle.h"
// #include <vtkActor.h>
// #include <vtkDataSetMapper.h>
// #include <vtkPolyDataMapper.h>
// #include <vtkRenderer.h>
// #include <TopoDS.hxx>
// #include <TopExp_Explorer.hxx>
// #include <IVtkOCC_Shape.hxx>
#include <IVtkTools_ShapeDataSource.hxx>
#include <vtkCamera.h>
#include <assert.h>
// #include <vtkPolyDataNormals.h>
// #include <vtkRenderWindow.h>
#include <QDebug>

namespace MainWidget
{
	PreWindow::PreWindow(GUI::MainWindow* mw, int id, ModuleBase::GraphWindowType t) 
		: ModuleBase::Graph3DWindow(mw, id, t,true)
	{
		_meshData = MeshData::MeshData::getInstance();
		_geometryData = Geometry::GeometryData::getInstance();
		_modelData = ModelData::ModelDataSingleton::getinstance();
		_geoProvider = new GeometryViewProvider(mw, this);
		_meshProvider = new MeshViewProvider(mw, this);
		_sketchProvider = new SketchViewProvider(mw, this);
		this->setWindowTitle(tr("Pre-Window"));
		connect(this, SIGNAL(closed()), mw, SIGNAL(closePreWindowSig()));
		connect(mw, SIGNAL(clearAllHighLight()), this, SIGNAL(clearAllHighLight()));
		//几何
		connect(mw, SIGNAL(updateGeoDispalyStateSig(int, bool)), this, SLOT(updateGeoDispaly(int, bool)));
		connect(mw, SIGNAL(removeGeometryActorSig(int)), this, SLOT(removeGemoActor(int)));
		connect(mw, SIGNAL(clearHighLightSig()), this, SIGNAL(clearGeometryHighLight()));
		connect(mw, SIGNAL(highLightGeometrySetSig(Geometry::GeometrySet*, bool)), this, SIGNAL(highLightGeometrySet(Geometry::GeometrySet*, bool)));
		//草图
		connect(mw, SIGNAL(startSketchSig(bool, double*, double*)), this, SLOT(startSketch(bool, double*, double*)));
		//网格
		connect(this, SIGNAL(updateMeshActorSig()), this, SLOT(updateMeshActor()));
		connect(this, SIGNAL(highLighSet(QMultiHash<vtkDataSet*, int>*)), this, SLOT(highLighSet(QMultiHash<vtkDataSet*, int>*)));
		connect(mw, SIGNAL(updateMeshDispalyStateSig(int, bool)), this, SLOT(updateMeshDispaly(int, bool)));
		connect(mw, SIGNAL(removeMeshActorSig(int)), this, SLOT(removeMeshActor(int)));
		connect(mw, SIGNAL(removeSetDataSig(int)), this, SIGNAL(removeSetDataSig(int)));
		connect(mw, SIGNAL(clearHighLightSig()), this, SIGNAL(clearMeshSetHighLight()));
		//
		connect(mw, SIGNAL(selectModelChangedSig(int)), this, SLOT(setSelectModel(int)));
		connect(mw, SIGNAL(displayModeChangedSig(QString)), this, SLOT(setDisplay(QString)));
		connect(mw, SIGNAL(updateGraphOptionsSig()), this, SLOT(updateGraphOption()));
		//网格相关的高亮信号槽连接
		connect(mw, SIGNAL(highLightSetSig(MeshData::MeshSet*)), this, SIGNAL(highLighMeshSet(MeshData::MeshSet*)));
		connect(mw, SIGNAL(highLightGeoComponentSig(Geometry::GeoComponent*)), this, SLOT(highLightGeoComponentSlot(Geometry::GeoComponent*)));
		connect(mw, SIGNAL(highLightKernelSig(MeshData::MeshKernal*)), this, SIGNAL(highLighKernel(MeshData::MeshKernal*)));
		connect(mw, SIGNAL(highLightDataSetSig(vtkDataSet*)), this, SIGNAL(highLighDataSet(vtkDataSet*)));
	}

	PreWindow::~PreWindow()
	{
		if (_geoProvider != nullptr) delete _geoProvider;
		if (_sketchProvider != nullptr) delete _sketchProvider;
		if (_meshProvider != nullptr) delete _meshProvider;
		emit closed();
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
	
	Geometry::GeometrySet* PreWindow::getSelectedGeoSet()
	{
		Geometry::GeometrySet* set = _geometryData->getGeometrySetAt(_selectedGeoIndex);
		return set;
	}

	/*void PreWindow::setGeometryDisplay(QString m)
	{
		if (m.toLower() == "point") _displayGeometryModel =GeometryPoint;
		else if (m.toLower() == "curve") _displayGeometryModel = GeometryCurve;
		else if (m.toLower() == "surface") _displayGeometryModel = GeometrySurface;
		updateDisplayGeometryModel();
	}*/

	void PreWindow::setDisplay(QString m)
	{
		_meshProvider->setDisplay(m);
	}

	void PreWindow::updateMeshActor()
	{
		_meshProvider->updateMeshActor();
	}

	void PreWindow::updateMeshDispaly(int index, bool display)
	{
		_meshProvider->updateMeshDispaly(index, display);
	}

	void PreWindow::removeMeshActor(const int index)
	{
		_meshProvider->removeMeshActor(index);
	}

	void PreWindow::setSelectModel(int mode)
	{
		_selectModel = (ModuleBase::SelectModel) mode;
		_meshProvider->setMeshSelectMode(mode);
	}

	ModuleBase::SelectModel PreWindow::getSelectModel()
	{
		return _selectModel;
	}

	void PreWindow::highLighSet(QMultiHash<vtkDataSet*, int>* items)
	{
		_meshProvider->highLighSet(items);
	}

	QMultiHash<vtkDataSet*, int>* PreWindow::getSelectItems()
	{
		return  _meshProvider->_selectItems;
	}

	MainWidget::MeshViewProvider* PreWindow::getMeshViewProvider()
	{
		return  _meshProvider;
	}

	void PreWindow::updateGraphOption()
	{
		_meshProvider->updateDisplayModel();
		_meshProvider->updateGraphOption();
		_geoProvider->updateGraphOption();
// 		QColor c(0,0,0) /*= Setting::BusAPI::instance()->getGraphOption()->getGeometryColor()*/;
// 		const int n = _geometryActors.size();
// 		for (int i = 0; i < n; ++i)
// 		{
// 			vtkActor* actor = _geometryActors.at(i);
// 			actor->GetProperty()->SetDiffuseColor(c.redF(), c.greenF(), c.blueF());
// 		}
//		ModuleBase::Graph3DWindow::updateGraphOption();
//		_renderWindow->Render();
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

	QMultiHash<Geometry::GeometrySet*, int>* PreWindow::getGeoSelectItems()
	{
		return _geoProvider->getGeoSelectItems();
	}

	void PreWindow::setGeoSelectItems(QMultiHash<Geometry::GeometrySet*, int> items)
	{
		_geoProvider->setGeoSelectItems(items);
	}

	void PreWindow::highLightGeoComponentSlot(Geometry::GeoComponent* aGC)
	{
		auto selectedItems = aGC->getSelectedItems();
		QMutableHashIterator<Geometry::GeometrySet*, int> iterator(selectedItems);
		while (iterator.hasNext())
		{
			iterator.next();
			switch (aGC->getGCType())
			{
			case Geometry::GeoComponentType::Node: 
				emit highLightGeometryPoint(iterator.key(), iterator.value(), true); 
				break;
			case Geometry::GeoComponentType::Line: 
				emit highLightGeometryEdge(iterator.key(), iterator.value(), true); 
				break;
			case Geometry::GeoComponentType::Surface: 
				emit highLightGeometryFace(iterator.key(), iterator.value(), true); 
				break;
			case Geometry::GeoComponentType::Body: 
				emit highLightGeometrySolid(iterator.key(), iterator.value(), true); 
				break;
			}
		}
	}
}
