#include "DialogSurfaceMesh.h"
#include "ui_DialogSurfaceMesh.h"
#include "DialogLocalSetting.h"
#include "settings/busAPI.h"
#include "settings/GraphOption.h"
// #include <vtkActor.h>
// #include <vtkProperty.h>
#include "python/PyAgent.h"
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include "GmshSettingData.h"
#include "meshData/meshKernal.h"
#include "meshData/meshSingleton.h"
#include "LocalField.h"
//#include "DialogPhysicalsSetting.h"
#include <QMessageBox>

namespace Gmsh
{
	SurfaceMeshDialog::SurfaceMeshDialog(GUI::MainWindow* m, MainWidget::PreWindow* pre, int index)
		: GmshDialogBase(m, pre)
	{
		_ui = new Ui::SurfaceMeshDialog;
		_ui->setupUi(this);


		if (index >= 0)
		{
			_meshData = MeshData::MeshData::getInstance();
			_kernalData = _meshData->getKernalAt(index);
			if (_kernalData == nullptr) return;
			emit highLightMeshKernal(_kernalData);
			_settingData = dynamic_cast<GmshSettingData*>(_kernalData->getGmshSetting());
			init();
		}
	}
	SurfaceMeshDialog::~SurfaceMeshDialog()
	{
		if (_ui != nullptr) delete _ui;
		//if (_settingData != nullptr)delete _settingData;
	}

	void SurfaceMeshDialog::on_geoSelectSurface_clicked()
	{
		_ui->selectall->setChecked(false);
		_ui->selectvisible->setChecked(false);

		emit setSelectMode((int)ModuleBase::GeometrySurface);
		_selectFace = true;
		
	}

	void SurfaceMeshDialog::shapeSlected(Geometry::GeometrySet* st,int index)
	{
		if (!_selectFace)  return;
		bool high = false;
		if (_geoHash.contains(st, index))
		{
			_geoHash.remove(st, index);
		}
		else
		{
			_geoHash.insert(st, index);
			high = true;
		}
		emit highLightGeometryFaceSig(st, index, high);
// 		if (_actorList.contains(ac))
// 		{
// 			_actorList.removeOne(ac);
// 			QColor color = Setting::BusAPI::instance()->getGraphOption()->getGeometrySurfaceColor();
// 			ac->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
// 			_geoHash.remove(st, index);
// 		}
// 		else
// 		{
// 			_actorList.append(ac);
// 			QColor color = Setting::BusAPI::instance()->getGraphOption()->getHighLightColor();
// 			ac->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
// 			_geoHash.insert(st, index);
// 		}
		QString text = QString(tr("Selected Surface(%1)").arg(_geoHash.size()));
		_ui->planelabel->setText(text);
	}

	void SurfaceMeshDialog::on_localButton_clicked()
	{
		emit setSelectMode((int)ModuleBase::GeometrySurface);
		_selectFace = false;
		auto d = new LocalSettingDialog(this,_mainWindow, _preWindow);
		emit showDialog(d);
	}

// 	void SurfaceMeshDialog::on_physicalsPButton_clicked()
// 	{
// 		emit setSelectMode((int)ModuleBase::None);
// 		_selectFace = false;
// 		auto d = new DialogPhysicalsSetting(_mainWindow, _preWindow,this);
// 		emit showDialog(d);
// 	}

	void SurfaceMeshDialog::on_selectall_clicked()
	{
		emit clearGeometryHighLightSig();
		emit setSelectMode((int)ModuleBase::None);
		_geoHash.clear();
		_selectFace = false;
		int num = 0;

		if (_ui->selectall->isChecked())
		{
			_ui->selectvisible->setChecked(false);
			_ui->geoSelectSurface->setEnabled(false);
			int n = _geoData->getGeometrySetCount();
			for (int i = 0; i < n; i++)
			{
				Geometry::GeometrySet* set = _geoData->getGeometrySetAt(i);
				if (set == nullptr)
					continue;

				num += set->getGeoMemberCount(3);
			}
		}
		else
		{
			_ui->geoSelectSurface->setEnabled(true);
		}

		QString text = QString(tr("Selected Surface(%1)").arg(num));
		_ui->planelabel->setText(text);
	}

	void SurfaceMeshDialog::on_selectvisible_clicked()
	{
		emit clearGeometryHighLightSig();
		emit setSelectMode((int)ModuleBase::None);
		_geoHash.clear();
		_selectFace = false;
		int num = 0;

		if (_ui->selectvisible->isChecked())
		{
			_ui->selectall->setChecked(false);
			_ui->geoSelectSurface->setEnabled(false);
			int n = _geoData->getGeometrySetCount();
			for (int i = 0; i < n; i++)
			{
				Geometry::GeometrySet* set = _geoData->getGeometrySetAt(i);
				if (set == nullptr)
					continue;

				if (!set->isVisible())
					continue;

				num += set->getGeoMemberCount(3);
			}
		}
		else
		{
			_ui->geoSelectSurface->setEnabled(true);
		}


		QString text = QString(tr("Selected Surface(%1)").arg(num));
		_ui->planelabel->setText(text);
	}

	void SurfaceMeshDialog::accept()
	{
		if (_geoHash.isEmpty() && !_ui->selectall->isChecked() && !_ui->selectvisible->isChecked())
		{
			QMessageBox::warning(this, tr("Warning"), tr("No object has been selected !"));
			return;
		}

		_pyAgent->submit(QString("gmsher = Mesher.Gmsher()"));

		_pyAgent->submit(QString("gmsher.setDim(2)"));

		QList<Geometry::GeometrySet*> setList = _geoHash.uniqueKeys();
		for (auto set : setList)
		{
			const int id = set->getID();
			QList<int> indexList = _geoHash.values(set);
			for (int index : indexList)
			{
				QString code = QString("gmsher.appendSurface(%1,%2)").arg(id).arg(index);
				_pyAgent->submit(code);
			}
		}
//		_pyAgent->submit("Mesher.mergeGmsh()");
		if (_ui->selectall->isChecked())
			_pyAgent->submit(QString("gmsher.selectedAll()"));
		if (_ui->selectvisible->isChecked())
			_pyAgent->submit(QString("gmsher.selectedVisible()"));

		QString eleType = "Tri";
		if (_ui->QuadRadioButton->isChecked()) eleType = "Quad";
		_pyAgent->submit(QString("gmsher.setElementType(\"%1\")").arg(eleType));

		const int orderindex = _ui->OrderComboBox->currentIndex();
		_pyAgent->submit(QString("gmsher.setElementOrder(%1)").arg(orderindex + 1)); //�ͽ׸߽׵�Ԫ

		const int methodIndex = _ui->MethodComboBox->currentIndex();
		int method = 2;
		switch (methodIndex)
		{
		case 1: method = 1; break;
		case 2: method = 5; break;
		case 3: method = 6; break;
		case 4: method = 8; break;
		case 5: method = 9; break;
		default: break;
		}
		_pyAgent->submit(QString("gmsher.setMethod(%1)").arg(method));
		;
		const double factor = _ui->SizeFacDoubleSpinBox->value();
		const double minsize = _ui->MinSizeDoubleSpinBox->value();
		const double maxsize = _ui->MaxSizeDoubleSpinBox->value();
		_pyAgent->submit(QString("gmsher.setSizeFactor(%1)").arg(factor));
		_pyAgent->submit(QString("gmsher.setMinSize(%1)").arg(minsize));
		_pyAgent->submit(QString("gmsher.setMaxSize(%1)").arg(maxsize));

		const int smooth = _ui->smoothingSpinBox->value();
		_pyAgent->submit(QString("gmsher.setSmoothIteration(%1)").arg(smooth));
//		_pyAgent->submit(QString("gmsh.option.setNumber(\"Mesh.SmoothNormals\", 1)"));

// 		_pyAgent->submit(QString("boxsize = gmsh.option.getNumber(\"General.BoundingBoxSize\")"));
// 		_pyAgent->submit(QString("gmsh.option.setNumber(\"Geometry.Tolerance\", boxsize/100)"));


		if (_ui->geoCleanCheckBox->isChecked()) //geoclean = 1;
			_pyAgent->submit(QString("gmsher.cleanGeo()"));

		if (_ui->gridCoplanarCheckBox->isChecked())
			_pyAgent->submit(QString("gmsher.setGridCoplanar()"));

		this->appendLocalDesities();
//		this->appendPhysicals();
		if (_settingData != nullptr)
			_pyAgent->submit(QString("gmsher.setMeshID(%1)").arg(_settingData->getMeshID()));

		//specifiedDimensionScript();

		_pyAgent->submit("gmsher.startGenerationThread()");
// 		_pyAgent->submit("gmsh.model.mesh.generate(2)");
// 		_pyAgent->submit("Mesher.TransFormMesh(2)");
		
		if (_settingData != nullptr)
		{
			delete _settingData;
		}

		if (_kernalData != nullptr)
		{
			_meshData->removeKernalByID(_kernalData->getID());
		}
		QDialog::accept();
		this->close();
	}

	void SurfaceMeshDialog::closeEvent(QCloseEvent *e)
	{
		QDialog::closeEvent(e);
		delete this;
	}

	void SurfaceMeshDialog::reject()
	{
		QDialog::reject();
		this->close();
	}

	void SurfaceMeshDialog::init()
	{
		if (_settingData == nullptr)
			return;

		highLightSurface();

		_ui->OrderComboBox->setCurrentIndex(_settingData->getElementOrder() - 1);
		_ui->MinSizeDoubleSpinBox->setValue(_settingData->getMinSize());
		_ui->MaxSizeDoubleSpinBox->setValue(_settingData->getMaxSize());
		_ui->SizeFacDoubleSpinBox->setValue(_settingData->getSizeFactor());
		_ui->gridCoplanarCheckBox->setChecked(_settingData->getGridCoplanar());
		_ui->geoCleanCheckBox->setChecked(_settingData->getGeoClean());
		_ui->smoothingSpinBox->setValue(_settingData->getSmoothIteration());

		if (_settingData->getElementType() == "Quad")
			_ui->QuadRadioButton->setChecked(true);

		int method = _settingData->getMethod();
		int methodIndex = 0;
		switch (method)
		{
		case 1: methodIndex = 1; break;
		case 5: methodIndex = 2; break;
		case 6: methodIndex = 3; break;
		case 8: methodIndex = 4; break;
		case 9: methodIndex = 5; break;
		default:break;
		}

		_ui->MethodComboBox->setCurrentIndex(methodIndex);

		initLocals();
	}

	void SurfaceMeshDialog::highLightSurface()
	{
		if (_settingData->getSelectAll())
		{
			_ui->selectall->setChecked(true);
// 			int n = _geoData->getGeometrySetCount();
// 			for (int i = 0; i < n; i++)
// 			{
// 				Geometry::GeometrySet* set = _geoData->getGeometrySetAt(i);
// 				if (set == nullptr)
// 					continue;
// 
// 				int num = set->getGeoMemberCount(3);
// 
// 				for (int j = 0; j < num; j++)
// 				{
// 					emit highLightGeometrySolidSig(set, j, true);
// 				}
// 			}
		}
		else if (_settingData->getSelectVisiable())
		{
			_ui->selectvisible->setChecked(true);
// 			int n = _geoData->getGeometrySetCount();
// 			for (int i = 0; i < n; i++)
// 			{
// 				Geometry::GeometrySet* set = _geoData->getGeometrySetAt(i);
// 				if (set == nullptr)
// 					continue;
// 
// 				if (!set->isVisible())
// 					continue;
// 
// 				int num = set->getGeoMemberCount(3);
// 
// 				for (int j = 0; j < num; j++)
// 				{
// 					emit highLightGeometrySolidSig(set, j, true);
// 				}
// 			}
		}
		else{
			QMultiHash<int, int> surface = _settingData->getSurfaceHash();

			QList<int> setList = surface.uniqueKeys();
			for (auto id : setList)
			{
				Geometry::GeometrySet* set = _geoData->getGeometrySetByID(id);

				QList<int> indexList = surface.values(id);
				for (int index : indexList)
				{
					_geoHash.insert(set, index);					//emit highLightGeometrySolidSig(set, index, true);
				}
			}
		}
	}

	void SurfaceMeshDialog::initLocals()
	{
		QList<LocalDensity*> locallist = _settingData->getLocalDesities();
		for (LocalDensity* ld : locallist)
		{
			if (ld->_type == LocalType::PointSize)
			{

				LocalPoint* lp = new LocalPoint;
				lp->copy(ld);
				_localDensities.append(lp);
			}
			else if (ld->_type == LocalType::BoxField)
			{
				Box* box = new Box;
				box->copy(ld);
				_localDensities.append(box);
			}
			else if (ld->_type == LocalType::BallField)
			{
				Ball* ball = new Ball;
				ball->copy(ld);
				_localDensities.append(ball);
			}
			else if (ld->_type == LocalType::CylinderField)
			{
				Cylinder* cd = new Cylinder;
				cd->copy(ld);
				_localDensities.append(cd);
			}
		}
	}

}


