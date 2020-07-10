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
//#include "DialogPhysicalsSetting.h"
#include <QMessageBox>

namespace Gmsh
{
	SurfaceMeshDialog::SurfaceMeshDialog(GUI::MainWindow* m, MainWidget::PreWindow* pre)
		: GmshDialogBase(m, pre)
	{
		_ui = new Ui::SurfaceMeshDialog;
		_ui->setupUi(this);
	
	}
	SurfaceMeshDialog::~SurfaceMeshDialog()
	{
		if (_ui != nullptr) delete _ui;
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
// 				int sn = set->getGeoMemberCount(3);
// 				for (int j = 0; j < sn; j++)
// 				{
// 					//emit highLightGeometryFaceSig(set, j, true);
// 					num++;
// 				}
			}
		}
		else
		{
			_ui->geoSelectSurface->setEnabled(true);
			//emit clearGeometryHighLightSig();
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
// 				int sn = set->getGeoMemberCount(3);
// 				for (int j = 0; j < sn; j++)
// 				{
// 					//emit highLightGeometryFaceSig(set, j, true);
// 					num++;
// 				}
			}
		}
		else
		{
			_ui->geoSelectSurface->setEnabled(true);
			//emit clearGeometryHighLightSig();
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

		this->appendPointSizeFiled();
		this->appendSizeFields();
//		this->appendPhysicals();
		_pyAgent->submit("gmsher.startGenerationThread()");
// 		_pyAgent->submit("gmsh.model.mesh.generate(2)");
// 		_pyAgent->submit("Mesher.TransFormMesh(2)");
		
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

}


