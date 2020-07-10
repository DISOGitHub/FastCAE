#include "DialogSolidMesh.h"
#include "ui_DialogSolidMesh.h"
#include "python/PyAgent.h"
#include "geometry/geometrySet.h"
#include "DialogLocalSetting.h"
//#include "DialogPhysicalsSetting.h"
#include "geometry/geometryData.h"
#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include <QDebug>

namespace Gmsh
{
	SolidMeshDialog::SolidMeshDialog(GUI::MainWindow* m, MainWidget::PreWindow* pre)
		: GmshDialogBase(m, pre)
	{
		_ui = new Ui::SolidMeshDialog;
		_ui->setupUi(this);
		_geoData = Geometry::GeometryData::getInstance();
	}

	SolidMeshDialog::~SolidMeshDialog()
	{
		if (_ui != nullptr) delete _ui;
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

// 		QMenu* menu = new QMenu(this);
// 		QAction* points = new QAction(tr("Points"), this);
// 		QAction* fields = new QAction(tr("Fields"), this);
// 
// 		menu->addAction(points);
// 		menu->addAction(fields);
// 
// 		connect(points, SIGNAL(triggered()), this, SLOT(addLocalPoints()));
// 		connect(fields, SIGNAL(triggered()), this, SLOT(addLocalFields()));
// 
// 
// 		menu->exec(QCursor::pos());

	}

	void SolidMeshDialog::addLocalPoints()
	{
		emit setSelectMode((int)ModuleBase::None);
		_selectBody = false;
		auto d = new LocalSettingDialog(this, _mainWindow, _preWindow);
		emit showDialog(d);
	}

	void SolidMeshDialog::addLocalFields()
	{
// 		emit setSelectMode((int)ModuleBase::None);
// 		_selectBody = false;
// 		auto d = new DialogLocalFields(this, _mainWindow, _preWindow);
// 		emit showDialog(d);
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
				//emit highLightGeometrySetSig(set, true);
// 				_solidHash.insert(set, 0);
// 				emit highLightGeometrySolidSig(set, 0, true);

				// 				int sn = set->getSubSetCount();
				// 				if (sn == 0)
				// 					_solidHash.insert(set, 0);
				// 				for (int j = 0; j < sn;j++)
				// 				{
				// 					Geometry::GeometrySet* st = _geoData->getGeometrySetAt(i);
				// 					if (st == nullptr)
				// 						continue;
				// 					
				// 				}
			}
		}
		else
		{
			//num = 0;
			_ui->geoSelectSurface->setEnabled(true);
		//	emit clearGeometryHighLightSig();

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
				//emit highLightGeometrySetSig(set, true);
				// 				int sn = set->getSubSetCount();
				// 				if (sn == 0)
				// 					_solidHash.insert(set, 0);
				// 				for (int j = 0; j < sn;j++)
				// 				{
				// 					Geometry::GeometrySet* st = _geoData->getGeometrySetAt(i);
				// 					if (st == nullptr)
				// 						continue;
				// 					
				// 				}
			}
		}
		else
		{
			//num = 0;
			_ui->geoSelectSurface->setEnabled(true);
			//emit clearGeometryHighLightSig();
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
		
		this->appendPointSizeFiled();
		this->appendSizeFields();
		//this->appendPhysicals();
		_pyAgent->submit("gmsher.startGenerationThread()");

		QDialog::accept();
		this->close();
	}



}