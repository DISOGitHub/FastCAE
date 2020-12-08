#include "DialogLocalSetting.h"
#include "GmshDialogBase.h"
#include "ui_DialogLocalSetting.h"
#include "GeometryWidgets/geoPointWidget.h"
#include "LocalField.h"
#include "mainWindow/mainWindow.h"
#include "MainWidgets/preWindow.h"
#include "geometry/geometrySet.h"
#include "geometry/geometryData.h"
#include <QHeaderView>
#include <math.h>

namespace Gmsh
{
	LocalSettingDialog::LocalSettingDialog(GmshDialogBase* parent, GUI::MainWindow* m, MainWidget::PreWindow* p)
		:QDialog(parent), _locals(parent->getLocalDesities())
	{
		_ui = new Ui::LocalSettingDialog;
		_ui->setupUi(this);
		_tab = _ui->tableWidget;
		_pointWidget = new GeometryWidget::GeoPointWidget(m, p);
		_ui->pointLayout->addWidget(_pointWidget);
		_tab->horizontalHeader()->setStretchLastSection(true);
		_ui->comboBox->setMaxCount(5);
		
		updatePointsInterface();

		connect(_tab, SIGNAL(cellClicked(int, int)), this, SLOT(changeData(int, int)));
		connect(_ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateInterface(int)));
		connect(_ui->custom, SIGNAL(toggled(bool)), this, SLOT(onCustomChecked()));
		connect(this, SIGNAL(setSelectMode(int)), m, SIGNAL(selectModelChangedSig(int)));
		connect(p, SIGNAL(geoShapeSelected(Geometry::GeometrySet*, int)), this, SLOT(shapeSlected(Geometry::GeometrySet*, int)));
		connect(this, SIGNAL(highLightGeometrySolidSig(Geometry::GeometrySet*, int, bool)), p, SIGNAL(highLightGeometrySolid(Geometry::GeometrySet*, int, bool)));
		connect(this, SIGNAL(clearGeometryHighLightSig()), p, SIGNAL(clearGeometryHighLight()));
	}

	LocalSettingDialog::~LocalSettingDialog()
	{
		if (_ui != nullptr) delete _ui;
	}

	void LocalSettingDialog::on_PointAddButton_clicked()
	{
		switch (_localType)
		{
		case LocalSettingType::Points:
			addPointsData();
			break;
		case LocalSettingType::FieldsBox:
			addBoxData();
			break;
		case LocalSettingType::FieldsBall:
			addBallData();
			break;
		case LocalSettingType::FieldsCylinder:
			addCylinderData();
			break;
		case LocalSettingType::FieldsSolid:
			addSolidFieldsData();
			break;
		case LocalSettingType::FieldsFrustum:
			addFrustumFieldData();
			break;
		default:
			break;
		}
	}

	void LocalSettingDialog::on_closeButton_clicked()
	{
		this->close();
	}

	void LocalSettingDialog::on_geoSelectSurface_clicked()
	{
		emit setSelectMode((int)ModuleBase::GeometryBody);
		_selectSolid = true;
	}

	void LocalSettingDialog::changeData(int row, int col)
	{
		int n = _locals->size();
		if (row < 0 || row >= n)
			return;

		if (col == 0)
			deleteItem(row);
		else
			operateLocalData(row);
	}

	void LocalSettingDialog::shapeSlected(Geometry::GeometrySet* set, int index)
	{
		if (!_selectSolid) return;
		if (_solidHash.contains(set, index))
		{
			_solidHash.remove(set, index);
			emit highLightGeometrySolidSig(set, index, false);
		}
		else
		{
			_solidHash.insert(set, index);
			emit highLightGeometrySolidSig(set, index, true);

		}
		QString text = QString(tr("Selected Solid(%1)").arg(_solidHash.size()));
		_ui->panel->setText(text);
	}


	void LocalSettingDialog::deleteItem(int row)
	{
		int n = _locals->size();

		if (row >= 0 && row < n)
		{
			_locals->removeAt(row);
			updateTab();
		}
	}

	void LocalSettingDialog::updateInterface(int type)
	{
		_ui->PointAddButton->setText(tr("Add"));
		_currentLocal = nullptr;
		switch (type)
		{
		case 0:
			_localType = LocalSettingType::Points;
			updatePointsInterface();
			break;
		case 1:
			_localType = LocalSettingType::FieldsBox;
			updateBoxInterface();
			break;
		case 2:
			_localType = LocalSettingType::FieldsBall;
			updateBallInterface();
			break;
		case 3:
			_localType = LocalSettingType::FieldsCylinder;
			updateCylinderInterface();
			break;
		case 4:
			_localType = LocalSettingType::FieldsSolid;
			updateSolidFieldInterface();
			break;
		case 5:
			_localType = LocalSettingType::FieldsFrustum;
			updateFrustumFieldInterface();
			break;
		default:
			break;
		}

	}

	void LocalSettingDialog::onCustomChecked()
	{
		bool b = _ui->custom->isChecked();
		_ui->xLabel->setVisible(b);
		_ui->xDoubleSpinBox->setVisible(b);
		_ui->yLabel->setVisible(b);
		_ui->yDoubleSpinBox->setVisible(b);
		_ui->zLabel->setVisible(b);
		_ui->zDoubleSpinBox->setVisible(b);
	}

	void LocalSettingDialog::showEvent(QShowEvent * e)
	{
		QDialog::showEvent(e);
		updateTab();
	}

	void LocalSettingDialog::updatePointsInterface()
	{
		_ui->panel->setVisible(false);
		_ui->geoSelectSurface->setVisible(false);
		_pointWidget->setVisible(true);

		_ui->label->setText(tr("Value:"));
		_ui->backgroundField->setVisible(false);

		clearInterfaceData();
		updateValueInterface(1);
		cylinderSelectVisiable();
		cylinderAxisVisiable();

		if (_currentLocal != nullptr)
		{
			LocalPoint* p = dynamic_cast<LocalPoint*>(_currentLocal);
			if (p == nullptr)
				return;
			_ui->doubleSpinBox->setValue(p->_value);
			double coor[3] = { 0 };
			coor[0] = p->_x; coor[1] = p->_y; coor[2] = p->_z;
			_pointWidget->setCoordinate(coor);
		}
	}

	void LocalSettingDialog::updateBoxInterface()
	{
		_ui->panel->setVisible(false);
		_ui->geoSelectSurface->setVisible(false);
		_pointWidget->setVisible(true);

		_ui->label->setText(tr("Length:"));
		_ui->label_2->setText(tr("Width:"));
		_ui->label_3->setText(tr("Height:"));
		_ui->label_4->setText(tr("VIn:"));
		_ui->label_5->setText(tr("VOut:"));
		_ui->label_6->setText(tr("Thinckness:"));
		_ui->backgroundField->setVisible(true);
		
		clearInterfaceData();
		updateValueInterface(6);
		cylinderSelectVisiable();
		cylinderAxisVisiable();

		if (_currentLocal != nullptr)
		{
			Box* b = dynamic_cast<Box*>(_currentLocal);
			if (b == nullptr)
				return;
			double coor[3] = { 0 };
			coor[0] = b->_x; coor[1] = b->_y; coor[2] = b->_z;
			_pointWidget->setCoordinate(coor);
			_ui->doubleSpinBox->setValue(b->_length);
			_ui->doubleSpinBox_2->setValue(b->_width);
			_ui->doubleSpinBox_3->setValue(b->_height);
			_ui->doubleSpinBox_4->setValue(b->_vIn);
			_ui->doubleSpinBox_5->setValue(b->_vOut);
			_ui->doubleSpinBox_6->setValue(b->_thickness);
			_ui->backgroundField->setChecked(b->_backgroundField);
		}
	}

	void LocalSettingDialog::updateBallInterface()
	{
		_ui->panel->setVisible(false);
		_ui->geoSelectSurface->setVisible(false);
		_pointWidget->setVisible(true);

		_ui->label->setText(tr("Radius:"));
		_ui->label_2->setText(tr("VIn:"));
		_ui->label_3->setText(tr("VOut:"));
		_ui->label_4->setText(tr("Thinckness:"));
		_ui->backgroundField->setVisible(true);
		
		clearInterfaceData();
		updateValueInterface(4);
		cylinderSelectVisiable();
		cylinderAxisVisiable();

		if (_currentLocal != nullptr)
		{
			Ball* b = dynamic_cast<Ball*>(_currentLocal);
			if (b == nullptr)
				return;
			double coor[3] = { 0 };
			coor[0] = b->_x; coor[1] = b->_y; coor[2] = b->_z;
			_pointWidget->setCoordinate(coor);
			_ui->doubleSpinBox->setValue(b->_radius);
			_ui->doubleSpinBox_2->setValue(b->_vIn);
			_ui->doubleSpinBox_3->setValue(b->_vOut);
			_ui->doubleSpinBox_4->setValue(b->_thickness);
			_ui->backgroundField->setChecked(b->_backgroundField);
		
		}
	}

	void LocalSettingDialog::updateCylinderInterface()
	{
		_ui->panel->setVisible(false);
		_ui->geoSelectSurface->setVisible(false);
		_pointWidget->setVisible(true);
		_ui->label->setText(tr("Radius:"));
		_ui->label_2->setText(tr("Length:"));
		_ui->label_3->setText(tr("VIn:"));
		_ui->label_4->setText(tr("VOut:"));
		_ui->backgroundField->setVisible(true);

		clearInterfaceData();
		updateValueInterface(4);
		cylinderSelectVisiable(true);
		cylinderAxisVisiable();

		if (_currentLocal != nullptr)
		{
			Cylinder* c = dynamic_cast<Cylinder*>(_currentLocal);
			if (c == nullptr)
				return;
			double coor[3] = { 0 };
			coor[0] = c->_x; coor[1] = c->_y; coor[2] = c->_z;
			_pointWidget->setCoordinate(coor);
			_ui->doubleSpinBox->setValue(c->_radius);
			_ui->doubleSpinBox_2->setValue(c->_length);
			_ui->doubleSpinBox_3->setValue(c->_vIn);
			_ui->doubleSpinBox_4->setValue(c->_vOut);
			_ui->backgroundField->setChecked(c->_backgroundField);
			
			updateCylinderAxisData(c->_xAxis,c->_yAxis,c->_zAxis);
		}
	}

	void LocalSettingDialog::updateCylinderAxisData(double xa, double ya, double za)
	{
		int num = 0;
		int t = 0;
		if (xa != 0)
		{
			num++;
			t = 1;
		}
		if (ya != 0)
		{
			num++;
			t = 2;
		}
		if (za != 0)
		{
			num++;
			t = 3;
		}

		if (num == 1)
		{
			switch (t)
			{
			case 1:
				_ui->xAxis->setChecked(true);
				break;
			case 2:
				_ui->yAxis->setChecked(true);
				break;
			case 3:
				_ui->zAxis->setChecked(true);
				break;
			}
		}
		else /*if (num > 1)*/
		{
			_ui->custom->setChecked(true);
			cylinderAxisVisiable(true);
			_ui->xDoubleSpinBox->setValue(xa);
			_ui->yDoubleSpinBox->setValue(ya);
			_ui->zDoubleSpinBox->setValue(za);
		}
	}

	void LocalSettingDialog::updateValueInterface(int n)
	{
		showValueInterface(n);
		hideValueInterface(n);
	}

	void LocalSettingDialog::showValueInterface(int n)
	{
		switch (n)
		{
		case 7:
			_ui->label_7->setVisible(true);
			_ui->doubleSpinBox_7->setVisible(true);
		case 6:
			_ui->label_6->setVisible(true);
			_ui->doubleSpinBox_6->setVisible(true);
		case 5:
			_ui->label_5->setVisible(true);
			_ui->doubleSpinBox_5->setVisible(true);
		case 4:
			_ui->label_4->setVisible(true);
			_ui->doubleSpinBox_4->setVisible(true);
		case 3:
			_ui->label_3->setVisible(true);
			_ui->doubleSpinBox_3->setVisible(true);
		case 2:
			_ui->label_2->setVisible(true);
			_ui->doubleSpinBox_2->setVisible(true);
		case 1:
			_ui->label->setVisible(true);
			_ui->doubleSpinBox->setVisible(true);
			break;
		default:
			break;
		}
	}

	void LocalSettingDialog::hideValueInterface(int n)
	{
		switch (n)
		{
		case 0:
			_ui->label->setVisible(false);
			_ui->doubleSpinBox->setVisible(false);
		case 1:
			_ui->label_2->setVisible(false);
			_ui->doubleSpinBox_2->setVisible(false);
		case 2:
			_ui->label_3->setVisible(false);
			_ui->doubleSpinBox_3->setVisible(false);			
		case 3:
			_ui->label_4->setVisible(false);
			_ui->doubleSpinBox_4->setVisible(false);
		case 4:
			_ui->label_5->setVisible(false);
			_ui->doubleSpinBox_5->setVisible(false);
		case 5:
			_ui->label_6->setVisible(false);
			_ui->doubleSpinBox_6->setVisible(false);
		case 6:
			_ui->label_7->setVisible(false);
			_ui->doubleSpinBox_7->setVisible(false);
			break;
		default:
			break;
		}
	}

	void LocalSettingDialog::updateSolidFieldInterface()
	{
	//	_ui->pointLayout->removeWidget(_pointWidget);
		_ui->panel->setVisible(true);
		_ui->geoSelectSurface->setVisible(true);
		_pointWidget->setVisible(false);
		_ui->label->setText(tr("VIn:"));
		_ui->label_2->setText(tr("VOut:"));
		_ui->label_3->setText(tr("Thinckness:"));
		_ui->backgroundField->setVisible(true);
		_solidHash.clear();

		clearInterfaceData();
		updateValueInterface(3);

		if (_currentLocal != nullptr)
		{
			SolidFields* b = dynamic_cast<SolidFields*>(_currentLocal);
			if (b == nullptr)
				return;
			_ui->doubleSpinBox->setValue(b->_vIn);
			_ui->doubleSpinBox_2->setValue(b->_vOut);
			_ui->doubleSpinBox_3->setValue(b->_thickness);
			_ui->backgroundField->setChecked(b->_backgroundField);

			for (QMultiHash<int, int>::iterator iter = b->_solidHash.begin(); iter != b->_solidHash.end(); ++iter)			{				auto id = iter.key();				Geometry::GeometrySet* set = Geometry::GeometryData::getInstance()->getGeometrySetByID(id);				if (set ==nullptr)continue;				int index = iter.value();				_solidHash.insert(set, index);				emit highLightGeometrySolidSig(set, index, true);			}
		}

	}

	void LocalSettingDialog::updateFrustumFieldInterface()
	{
		_ui->panel->setVisible(false);
		_ui->geoSelectSurface->setVisible(false);
		_pointWidget->setVisible(true);
		_ui->label->setText(tr("RadiusIn 1:"));
		_ui->label_2->setText(tr("RadiusOut 1:"));
		_ui->label_3->setText(tr("RadiusIn 2:"));
		_ui->label_4->setText(tr("RadiusOut 2:"));
		_ui->label_5->setText(tr("Length:"));
		_ui->label_6->setText(tr("VIn:"));
		_ui->label_7->setText(tr("VOut:"));
		_ui->backgroundField->setVisible(true);

		clearInterfaceData();
		updateValueInterface(7);
		cylinderSelectVisiable(true);
		cylinderAxisVisiable();

		if (_currentLocal != nullptr)
		{
			Frustum* f = dynamic_cast<Frustum*>(_currentLocal);
			if (f == nullptr)
				return;
			double coor[3] = { 0 };
			coor[0] = f->_x; coor[1] = f->_y; coor[2] = f->_z;
			_pointWidget->setCoordinate(coor);
			_ui->doubleSpinBox->setValue(f->_iRadius);
			_ui->doubleSpinBox_2->setValue(f->_oRadius);
			_ui->doubleSpinBox_3->setValue(f->_iTopRadius);
			_ui->doubleSpinBox_4->setValue(f->_oTopRadius);
			_ui->doubleSpinBox_5->setValue(f->_length);
			_ui->doubleSpinBox_6->setValue(f->_vIn);
			_ui->doubleSpinBox_7->setValue(f->_vOut);
			_ui->backgroundField->setChecked(f->_backgroundField);

			updateCylinderAxisData(f->_xAxis, f->_yAxis, f->_zAxis);
		}
	}

	void LocalSettingDialog::addPointsData()
	{
		double coor[3] = { 0 };
		_pointWidget->getCoordinate(coor);
		double v = _ui->doubleSpinBox->value();
		if (_currentLocal == nullptr)
		{
			LocalPoint* p = new LocalPoint;
			p->_type = LocalType::PointSize;
			p->_x = coor[0];  p->_y = coor[1]; p->_z = coor[2];
			p->_value = v;
			_locals->append(p);
		}
		else
		{
			LocalPoint*p = dynamic_cast<LocalPoint*>(_currentLocal);
			p->_x = coor[0];  p->_y = coor[1]; p->_z = coor[2];
			p->_value = v;
		}
		updateTab();
	}

	void LocalSettingDialog::addBoxData()
	{
		double coor[3] = { 0 };
		_pointWidget->getCoordinate(coor);
		double l = _ui->doubleSpinBox->value();
		double w = _ui->doubleSpinBox_2->value();
		double h = _ui->doubleSpinBox_3->value();
		double vi = _ui->doubleSpinBox_4->value();
		double vo = _ui->doubleSpinBox_5->value();
		double tn = _ui->doubleSpinBox_6->value();

		if (_currentLocal == nullptr)
		{
			Box* box = new Box;
			box->_type = LocalType::BoxField;
			// 		box->_xMin = coor[0]; box->_yMin = coor[1]; box->_zMin = coor[2];
			// 		box->_xMax = box->_xMin + l; box->_yMax = box->_yMin + w; box->_zMax = box->_zMin + h;
			box->_x = coor[0]; box->_y = coor[1]; box->_z = coor[2];
			box->_length = l; box->_width = w; box->_height = h;
			box->_vIn = vi; box->_vOut = vo; box->_thickness = tn;
			box->_backgroundField = _ui->backgroundField->isChecked();
			_locals->append(box);
		}
		else
		{
			Box* box = dynamic_cast<Box*>(_currentLocal);
			box->_x = coor[0]; box->_y = coor[1]; box->_z = coor[2];
			box->_length = l; box->_width = w; box->_height = h;
			box->_vIn = vi; box->_vOut = vo; box->_thickness = tn;
			box->_backgroundField = _ui->backgroundField->isChecked();
		}

		updateTab();

	}

	void LocalSettingDialog::addBallData()
	{
		double coor[3] = { 0 };
		_pointWidget->getCoordinate(coor);
		double r = _ui->doubleSpinBox->value();
		double vi = _ui->doubleSpinBox_2->value();
		double vo = _ui->doubleSpinBox_3->value();
		double tn = _ui->doubleSpinBox_4->value();

		if (_currentLocal == nullptr)
		{
			Ball* ball = new Ball;
			ball->_type = LocalType::BallField;
			/*ball->_xCenter = coor[0]; ball->_yCenter = coor[1]; ball->_zCenter = coor[2];*/
			ball->_x = coor[0]; ball->_y = coor[1]; ball->_z = coor[2];
			ball->_radius = r; ball->_vIn = vi; ball->_vOut = vo; ball->_thickness = tn;
			ball->_backgroundField = _ui->backgroundField->isChecked();
			_locals->append(ball);
		}
		else
		{
			Ball* ball = dynamic_cast<Ball*>(_currentLocal);
			ball->_x = coor[0]; ball->_y = coor[1]; ball->_z = coor[2];
			ball->_radius = r; ball->_vIn = vi; ball->_vOut = vo; ball->_thickness = tn;
			ball->_backgroundField = _ui->backgroundField->isChecked();
		}

		updateTab();

	}

	void LocalSettingDialog::addCylinderData()
	{
		double coor[3] = { 0 };
		_pointWidget->getCoordinate(coor);
		double r = _ui->doubleSpinBox->value();
		double lt = _ui->doubleSpinBox_2->value();
		double vi = _ui->doubleSpinBox_3->value();
		double vo = _ui->doubleSpinBox_4->value();
		double xv = _ui->xDoubleSpinBox->value();
		double yv = _ui->yDoubleSpinBox->value();
		double zv = _ui->zDoubleSpinBox->value();

		if (_currentLocal == nullptr)
		{
			Cylinder* cd = new Cylinder;
			cd->_type = LocalType::CylinderField;
			/*cd->_xCenter = coor[0]; cd->_yCenter = coor[1]; cd->_zCenter = coor[2];*/
			cd->_x = coor[0]; cd->_y = coor[1]; cd->_z = coor[2];
			cd->_radius = r; cd->_vIn = vi; cd->_vOut = vo; cd->_length = lt;
			cd->_backgroundField = _ui->backgroundField->isChecked();

			if (_ui->xAxis->isChecked())
			{
				cd->_xAxis = 1; cd->_yAxis = 0; cd->_zAxis = 0;
			}
			else if (_ui->yAxis->isChecked())
			{
				cd->_xAxis = 0; cd->_yAxis = 1; cd->_zAxis = 0;
			}
			else if (_ui->zAxis->isChecked())
			{
				cd->_xAxis = 0; cd->_yAxis = 0; cd->_zAxis = 1;
			}
			else if (_ui->custom->isChecked())
			{
				cd->_xAxis = xv; cd->_yAxis = yv; cd->_zAxis = zv;
			}

			_locals->append(cd);
		}
		else
		{
			Cylinder* cd = dynamic_cast<Cylinder*>(_currentLocal);
			cd->_x = coor[0]; cd->_y = coor[1]; cd->_z = coor[2];
			cd->_radius = r; cd->_vIn = vi; cd->_vOut = vo; cd->_length = lt;
			cd->_backgroundField = _ui->backgroundField->isChecked();

			if (_ui->xAxis->isChecked())
			{
				cd->_xAxis = 1; cd->_yAxis = 0; cd->_zAxis = 0;
			}
			else if (_ui->yAxis->isChecked())
			{
				cd->_xAxis = 0; cd->_yAxis = 1; cd->_zAxis = 0;
			}
			else if (_ui->zAxis->isChecked())
			{
				cd->_xAxis = 0; cd->_yAxis = 0; cd->_zAxis = 1;
			}
			else if (_ui->custom->isChecked())
			{
				cd->_xAxis = xv; cd->_yAxis = yv; cd->_zAxis = zv;
			}

		}
		
		updateTab();
	}

	void LocalSettingDialog::addSolidFieldsData()
	{
		double vin = _ui->doubleSpinBox->value();
		double vout = _ui->doubleSpinBox_2->value();
		double thick = _ui->doubleSpinBox_3->value();

		if (_solidHash.size() <= 0)return;

		QMultiHash<int, int> solid;
		for (QMultiHash<Geometry::GeometrySet*, int>::iterator iter = _solidHash.begin(); iter != _solidHash.end(); ++iter)		{			auto set = iter.key();			int id = set->getID();			int index = iter.value();			solid.insert(id, index);		}
		if (_currentLocal == nullptr)
		{
			SolidFields* sf = new SolidFields;
			sf->_type = LocalType::SolidField;
			sf->_vIn = vin;
			sf->_vOut = vout;
			sf->_solidHash = solid;
			sf->_thickness = thick;
			//sf->setIndex(_locals->size() + 1);
			sf->_backgroundField = _ui->backgroundField->isChecked();
			_locals->append(sf);
		}
		else
		{
			SolidFields* sf = dynamic_cast<SolidFields*>(_currentLocal);
			sf->_vIn = vin;
			sf->_vOut = vout;
			sf->_solidHash = solid;
			sf->_thickness = thick;
			//sf->setIndex(_locals->size() + 1);
			sf->_backgroundField = _ui->backgroundField->isChecked();
		}

		emit clearGeometryHighLightSig();
		updateTab();
	}

	void LocalSettingDialog::addFrustumFieldData()
	{
		double coor[3] = { 0 };
		_pointWidget->getCoordinate(coor);
		double iradius = _ui->doubleSpinBox->value();
		double oradius = _ui->doubleSpinBox_2->value();
		double itradius = _ui->doubleSpinBox_3->value();
		double otradius = _ui->doubleSpinBox_4->value();
		double length = _ui->doubleSpinBox_5->value();
		double vin = _ui->doubleSpinBox_6->value();
		double vout = _ui->doubleSpinBox_7->value();
		double xv = _ui->xDoubleSpinBox->value();
		double yv = _ui->yDoubleSpinBox->value();
		double zv = _ui->zDoubleSpinBox->value();

		if (_currentLocal == nullptr)
		{
			Frustum* ft = new Frustum;
			ft->_type = LocalType::FrustumField;
			ft->_x = coor[0]; ft->_y = coor[1]; ft->_z = coor[2];
			ft->_iRadius = iradius; ft->_oRadius = otradius; ft->_iTopRadius = itradius; ft->_oTopRadius = otradius;
			ft->_length = length; ft->_vIn = vin; ft->_vOut = vout;
			ft->_backgroundField = _ui->backgroundField->isChecked();
			if (_ui->xAxis->isChecked())
			{
				ft->_xAxis = 1; ft->_yAxis = 0; ft->_zAxis = 0;
			}
			else if (_ui->yAxis->isChecked())
			{
				ft->_xAxis = 0; ft->_yAxis = 1; ft->_zAxis = 0;
			}
			else if (_ui->zAxis->isChecked())
			{
				ft->_xAxis = 0; ft->_yAxis = 0; ft->_zAxis = 1;
			}
			else if (_ui->custom->isChecked())
			{
				ft->_xAxis = xv; ft->_yAxis = yv; ft->_zAxis = zv;
			}
			_locals->append(ft);
		}
		else
		{
			Frustum* ft = dynamic_cast<Frustum*>(_currentLocal);
			ft->_x = coor[0]; ft->_y = coor[1]; ft->_z = coor[2];
			ft->_iRadius = iradius; ft->_oRadius = otradius; ft->_iTopRadius = itradius; ft->_oTopRadius = otradius;
			ft->_length = length; ft->_vIn = vin; ft->_vOut = vout;
			ft->_backgroundField = _ui->backgroundField->isChecked();
			if (_ui->xAxis->isChecked())
			{
				ft->_xAxis = 1; ft->_yAxis = 0; ft->_zAxis = 0;
			}
			else if (_ui->yAxis->isChecked())
			{
				ft->_xAxis = 0; ft->_yAxis = 1; ft->_zAxis = 0;
			}
			else if (_ui->zAxis->isChecked())
			{
				ft->_xAxis = 0; ft->_yAxis = 0; ft->_zAxis = 1;
			}
			else if (_ui->custom->isChecked())
			{
				ft->_xAxis = xv; ft->_yAxis = yv; ft->_zAxis = zv;
			}
		}
		
		updateTab();
	}

	void LocalSettingDialog::updateTab()
	{
		int wid = _tab->width();
		_tab->setColumnWidth(0, 20);
		_tab->setColumnWidth(1, wid/3-12);
		_tab->setColumnWidth(2, wid/3-12);
		_tab->setColumnWidth(3, wid / 3 - 12);
//		_tab->clear();
		const int n = _locals->size();
		_tab->setRowCount(n);
		updateLocals(n);
	
	}

	void LocalSettingDialog::updateLocals(int n)
	{
		for (int i = 0; i < n; ++i)
		{
			LocalDensity* p = _locals->at(i);
			if (p == nullptr)continue;
			QLabel *label = new QLabel("");
			label->setPixmap(QPixmap("://QUI/geometry/remove.png"));
			_tab->setCellWidget(i, 0, label);

			QString sp, sv, sn;

			QStringList list = p->getInformation();
			if (list.size() != 3)
				continue;

			sn = list.at(0);
			sp = list.at(1);
			sv = list.at(2);

			QTableWidgetItem* item1 = new QTableWidgetItem(sn);
			_tab->setItem(i, 1, item1);
			QTableWidgetItem* item2 = new QTableWidgetItem(sp);
			_tab->setItem(i, 2, item2);
			QTableWidgetItem* item3 = new QTableWidgetItem(sv);
			_tab->setItem(i, 3, item3);
			item1->setFlags(item1->flags() & (~Qt::ItemIsEditable));
			item2->setFlags(item2->flags() & (~Qt::ItemIsEditable));
			item3->setFlags(item3->flags() & (~Qt::ItemIsEditable));
		}
	}

	void LocalSettingDialog::clearInterfaceData()
	{
		_ui->doubleSpinBox->setValue(0);
		_ui->doubleSpinBox_2->setValue(0);
		_ui->doubleSpinBox_3->setValue(0);
		_ui->doubleSpinBox_4->setValue(0);
		_ui->doubleSpinBox_5->setValue(0);
		_ui->doubleSpinBox_6->setValue(0);
		_ui->backgroundField->setChecked(false);
		_ui->xDoubleSpinBox->setValue(0);
		_ui->yDoubleSpinBox->setValue(0);
		_ui->zDoubleSpinBox->setValue(0);
		_ui->yAxis->setChecked(true);
	}

	void LocalSettingDialog::cylinderSelectVisiable(bool b)
	{
		_ui->xAxis->setVisible(b);
		_ui->yAxis->setVisible(b);
		_ui->zAxis->setVisible(b);
		_ui->custom->setVisible(b);
	}

	void LocalSettingDialog::cylinderAxisVisiable(bool b)
	{
		_ui->xLabel->setVisible(b);
		_ui->xDoubleSpinBox->setVisible(b);
		_ui->yLabel->setVisible(b);
		_ui->yDoubleSpinBox->setVisible(b);
		_ui->zLabel->setVisible(b);
		_ui->zDoubleSpinBox->setVisible(b);
	}

	void LocalSettingDialog::operateLocalData(int row)
	{
		_currentLocal = _locals->at(row);
		_ui->PointAddButton->setText(tr("Modify"));
		_ui->comboBox->blockSignals(true);
		switch (_currentLocal->_type)
		{
		case LocalType::PointSize:
			_localType = LocalSettingType::Points;
			_ui->comboBox->setCurrentIndex(0);
			updatePointsInterface();
			break;
		case LocalType::BoxField:
			_localType = LocalSettingType::FieldsBox;
			_ui->comboBox->setCurrentIndex(1);
			updateBoxInterface();
			break;
		case LocalType::BallField:
			_localType = LocalSettingType::FieldsBall;
			_ui->comboBox->setCurrentIndex(2);
			updateBallInterface();
			break;
		case LocalType::CylinderField:
			_localType = LocalSettingType::FieldsCylinder;
			_ui->comboBox->setCurrentIndex(3);
			updateCylinderInterface();
			break;
		case LocalType::SolidField:
			_localType = LocalSettingType::FieldsSolid;
			_ui->comboBox->setCurrentIndex(4);
			updateSolidFieldInterface();
			break;
		case LocalType::FrustumField:
			_localType = LocalSettingType::FieldsFrustum;
			_ui->comboBox->setCurrentIndex(5);
			updateFrustumFieldInterface();
			break;
		default:
			break;
		}
		_ui->comboBox->blockSignals(false);

	}

}



