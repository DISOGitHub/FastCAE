#include "DialogSolidMesh.h"
#include "ui_DialogSolidMesh.h"
#include "python/PyAgent.h"
#include "geometry/geometrySet.h"
#include "DialogLocalSetting.h"
//#include "DialogPhysicalsSetting.h"
#include "geometry/geometryData.h"
#include "GmshSettingData.h"
#include "LocalField.h"
#include "meshData/meshKernal.h"
#include "meshData/meshSingleton.h"
#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include <QDebug>

namespace Gmsh
{
	SolidMeshDialog::SolidMeshDialog(GUI::MainWindow* m, MainWidget::PreWindow* pre, int index)
		: GmshDialogBase(m, pre)
	{
		_ui = new Ui::SolidMeshDialog;
		_ui->setupUi(this);
		_geoData = Geometry::GeometryData::getInstance();

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

	SolidMeshDialog::~SolidMeshDialog()
	{
		if (_ui != nullptr) delete _ui;
		//if (_settingData != nullptr) delete _settingData;
	}

	void SolidMeshDialog::closeEvent(QCloseEvent *e)
	{
		QDialog::closeEvent(e);
		delete this;
	}

	void SolidMeshDialog::reject()
	{
		QDialog::reject();
		this->close();
	}

	void SolidMeshDialog::init()
	{
		if (_settingData == nullptr)
			return;

		highLightSolid();

		_ui->OrderComboBox->setCurrentIndex(_settingData->getElementOrder() - 1);
		//_ui->MethodComboBox->setCurrentIndex(_settingData->getMethod());
		_ui->MinSizeDoubleSpinBox->setValue(_settingData->getMinSize());
		_ui->MaxSizeDoubleSpinBox->setValue(_settingData->getMaxSize());
		_ui->SizeFacDoubleSpinBox->setValue(_settingData->getSizeFactor());
		_ui->gridCoplanarCheckBox->setChecked(_settingData->getGridCoplanar());
// 		_localPoints = _settingData->getSizeAtPoints();
// 		_localFields = _settingData->getSizeFields();

		if (_settingData->getElementType() == "Hex")
			_ui->HexRadioButton->setChecked(true);

		int method = _settingData->getMethod();
		int methodIndex = 0;
		switch (method)
		{
		case 2: methodIndex = 1; break;
		case 4: methodIndex = 2; break;
		case 5: methodIndex = 3; break;
		case 6: methodIndex = 4; break;
		case 7: methodIndex = 5; break;
		case 9: methodIndex = 6; break;
		default:break;
		}

		_ui->MethodComboBox->setCurrentIndex(methodIndex);
		
		//_localDensities = *_settingData->getLocalDesities();
		initLocals();

	}

	void SolidMeshDialog::highLightSolid()
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
// 				int num = set->getGeoMemberCount(4);
// 				
// 				for (int j = 0; j < num;j++)
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
// 				int num = set->getGeoMemberCount(4);
// 
// 				for (int j = 0; j < num; j++)
// 				{
// 					emit highLightGeometrySolidSig(set, j, true);
// 				}
// 			}
		}
		else
		{
			QMultiHash<int, int> solidHash = _settingData->getSolidHash();

			for (QMultiHash<int, int>::iterator iter = solidHash.begin(); iter != solidHash.end(); ++iter)
			{
				auto id = iter.key();
				Geometry::GeometrySet* set = _geoData->getGeometrySetByID(id);
				if (set == nullptr)continue;

				int index = iter.value();
				_solidHash.insert(set, index);
			//	emit highLightGeometrySolidSig(set, index, true);
			}
		}
				
	}

	void SolidMeshDialog::initLocals()
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
			else if (ld->_type == LocalType::SolidField)
			{
				SolidFields* sf = new SolidFields;
				sf->copy(ld);
				_localDensities.append(sf);
			}
			else if (ld->_type == LocalType::FrustumField)
			{
				Frustum* fs = new Frustum;
				fs->copy(ld);
				_localDensities.append(fs);
			}
		}
	}


	void SolidMeshDialog::on_geoSelectSurface_clicked()
	{
		_ui->selectall->setChecked(false);
		_ui->selectvisible->setChecked(false);

		emit setSelectMode((int)ModuleBase::GeometryBody);
		_selectBody = true;
	}

	void SolidMeshDialog::shapeSlected(Geometry::GeometrySet* set, int index)
	{
		if (!_selectBody) return;
		if (_solidHash.contains(set, index))
		{
			_solidHash.remove(set, index);
			emit highLightGeometrySolidSig(set, index,false);
		}
		else
		{
			_solidHash.insert(set,index);
			emit highLightGeometrySolidSig(set,index,true);

		}
		QString text = QString(tr("Selected Solid(%1)").arg(_solidHash.size()));
		_ui->planelabel->setText(text);
	}

	void SolidMeshDialog::on_localButton_clicked()
	{
		emit setSelectMode((int)ModuleBase::None);
		_selectBody = false;
		auto d = new LocalSettingDialog(this,_mainWindow,  _preWindow);
		emit showDialog(d);

	}

// 	void SolidMeshDialog::on_physicalsPButton_clicked()
// 	{
// 		emit setSelectMode((int)ModuleBase::None);
// 		_selectBody = false;
// 		auto d = new DialogPhysicalsSetting(_mainWindow,_preWindow,this);
// 		emit showDialog(d);
// 	}

	void SolidMeshDialog::on_selectall_clicked()
	{
		//_solidHash.clear();
		emit clearGeometryHighLightSig();
		emit setSelectMode((int)ModuleBase::None);
		_solidHash.clear();
		_selectBody = false;
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

				num += set->getGeoMemberCount(4);
			}
		}
		else
		{
			_ui->geoSelectSurface->setEnabled(true);
		}

		QString text = QString(tr("Selected Solid(%1)").arg(num));
		_ui->planelabel->setText(text);
	}

	void SolidMeshDialog::on_selectvisible_clicked()
	{
		emit clearGeometryHighLightSig();
		emit setSelectMode((int)ModuleBase::None);
		_solidHash.clear();
		_selectBody = false;
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

				num += set->getGeoMemberCount(4);
			}
		}
		else
		{
			_ui->geoSelectSurface->setEnabled(true);
		}


		QString text = QString(tr("Selected Solid(%1)").arg(num));
		_ui->planelabel->setText(text);
	}

	void SolidMeshDialog::accept()
	{
		if (_solidHash.isEmpty() && !_ui->selectall->isChecked() && !_ui->selectvisible->isChecked())
		{
			QMessageBox::warning(this, tr("Warning"), tr("No object has been selected !"));
			return;
		}
		
		_pyAgent->submit(QString("gmsher = Mesher.Gmsher()"));

		_pyAgent->submit(QString("gmsher.setDim(3)"));

		for(QMultiHash<Geometry::GeometrySet*, int>::iterator iter = _solidHash.begin(); iter != _solidHash.end(); ++iter)		{			auto set = iter.key();			int id = set->getID();			int index = iter.value();			QString code = QString("gmsher.appendSolid(%1,%2)").arg(id).arg(index);
			_pyAgent->submit(code);		}
		if (_ui->selectall->isChecked())
			_pyAgent->submit(QString("gmsher.selectedAll()"));
		if (_ui->selectvisible->isChecked())
			_pyAgent->submit(QString("gmsher.selectedVisible()"));

//		_pyAgent->submit("Mesher.initGmsh()");

		QString eleType("Tet");
		if (_ui->HexRadioButton->isChecked()) eleType = "Hex";     //六面体 
		_pyAgent->submit(QString("gmsher.setElementType(\"%1\")").arg(eleType));

		const int orderindex = _ui->OrderComboBox->currentIndex();
		_pyAgent->submit(QString("gmsher.setElementOrder(%1)").arg(orderindex + 1)); //低阶高阶单元

		const int methodIndex = _ui->MethodComboBox->currentIndex();
		int method = 1;
		switch (methodIndex)
		{
		case 1: method = 2; break;
		case 2: method = 4; break;
		case 3: method = 5; break;
		case 4: method = 6; break;
		case 5: method = 7; break;
		case 6: method = 9; break;
		default: break;
		}

		_pyAgent->submit(QString("gmsher.setMethod(%1)").arg(method));

		const double factor = _ui->SizeFacDoubleSpinBox->value();
		const double minsize = _ui->MinSizeDoubleSpinBox->value();
		const double maxsize = _ui->MaxSizeDoubleSpinBox->value();
		_pyAgent->submit(QString("gmsher.setSizeFactor(%1)").arg(factor));
		_pyAgent->submit(QString("gmsher.setMinSize(%1)").arg(minsize));
		_pyAgent->submit(QString("gmsher.setMaxSize(%1)").arg(maxsize));

		if (_ui->geoCleanCheckBox->isChecked())
			_pyAgent->submit(QString("gmsher.cleanGeo()"));
	
		if (_ui->gridCoplanarCheckBox->isChecked())
			_pyAgent->submit(QString("gmsher.setGridCoplanar()"));
		
		this->appendLocalDesities();
		//this->appendPhysicals();
		if (_settingData != nullptr)
			_pyAgent->submit(QString("gmsher.setMeshID(%1)").arg(_settingData->getMeshID()));

	//	specifiedDimensionScript();

		_pyAgent->submit("gmsher.startGenerationThread()");

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



}