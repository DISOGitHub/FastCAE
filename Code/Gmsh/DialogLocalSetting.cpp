#include "DialogLocalSetting.h"
#include "GmshDialogBase.h"
#include "ui_DialogLocalSetting.h"
#include "GeometryWidgets/geoPointWidget.h"
#include "LocalField.h"
#include <QHeaderView>
#include <math.h>

namespace Gmsh
{
	LocalSettingDialog::LocalSettingDialog(GmshDialogBase* parent, GUI::MainWindow* m, MainWidget::PreWindow* p)
		:QDialog(parent), _points(parent->getLocalPoints()), _fields(parent->getLocalFields())
	{
		_ui = new Ui::LocalSettingDialog;
		_ui->setupUi(this);
		_tab = _ui->tableWidget;
		_pointWidget = new GeometryWidget::GeoPointWidget(m, p);
		_ui->pointLayout->addWidget(_pointWidget);
		_tab->horizontalHeader()->setStretchLastSection(true);
		
// 		_bGroup = new QButtonGroup(this);
// 		_bGroup->addButton(_ui->xAxis);
// 		_bGroup->addButton(_ui->yAxis);
// 		_bGroup->addButton(_ui->zAxis);
// 		_bGroup->addButton(_ui->custom);


		updatePointsInterface();

		connect(_tab, SIGNAL(cellClicked(int, int)), this, SLOT(deleteItem(int, int)));
		connect(_ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateInterface(int)));
		connect(_ui->custom, SIGNAL(toggled(bool)), this, SLOT(onCustomChecked()));
	}

	LocalSettingDialog::~LocalSettingDialog()
	{
		if (_ui != nullptr) delete _ui;
	}

	void LocalSettingDialog::on_PointAddButton_clicked()
	{
		switch (_localType)
		{
		case LocalType::Points:
			addPointsData();
			break;
		case LocalType::FieldsBox:
			addBoxData();
			break;
		case LocalType::FieldsBall:
			addBallData();
			break;
		case LocalType::FieldsCylinder:
			addCylinderData();
			break;
		default:
			break;
		}
	}

	void LocalSettingDialog::on_closeButton_clicked()
	{
		this->close();
	}

	void LocalSettingDialog::deleteItem(int row, int col)
	{
		if (col != 0) return;
		int n = _points->size();
		int fn = _fields->size();

		if (row >= 0 && row < n)
		{
			_points->removeAt(row);
			updateTab();
		}
		else if (row >= n && row < (n+fn))
		{
			_fields->removeAt(row-n);
			updateTab();
		}
	}

	void LocalSettingDialog::updateInterface(int type)
	{
// 		if (type == 0)
// 			_localType = LocalType::Points;
// 		else if (type == 1)
// 			_localType = LocalType::FieldsBox;
// 		else if (type == 2)
// 			_localType = LocalType::FieldsBox;

		switch (type)
		{
		case 0:
			_localType = LocalType::Points;
			updatePointsInterface();
			break;
		case 1:
			_localType = LocalType::FieldsBox;
			updateBoxInterface();
			break;
		case 2:
			_localType = LocalType::FieldsBall;
			updateBallInterface();
			break;
		case 3:
			_localType = LocalType::FieldsCylinder;
			updateCylinderInterface();
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
		_ui->label->setText("Value:");
		_ui->backgroundField->setVisible(false);
		clearInterfaceData();
		updateValueInterface(1);
		cylinderSelectVisiable();
		cylinderAxisVisiable();
	}

	void LocalSettingDialog::updateBoxInterface()
	{
		_ui->label->setText("Length:");
		_ui->label_2->setText("Width:");
		_ui->label_3->setText("Height:");
		_ui->label_4->setText("VIn:");
		_ui->label_5->setText("VOut:");
		_ui->label_6->setText("Thinckness:");
		_ui->backgroundField->setVisible(true);
		
		clearInterfaceData();
		updateValueInterface(6);
		cylinderSelectVisiable();
		cylinderAxisVisiable();
	}

	void LocalSettingDialog::updateBallInterface()
	{
		_ui->label->setText("Radius:");
		_ui->label_2->setText("VIn:");
		_ui->label_3->setText("VOut:");
		_ui->label_4->setText("Thinckness:");
		_ui->backgroundField->setVisible(true);
		
		clearInterfaceData();
		updateValueInterface(4);
		cylinderSelectVisiable();
		cylinderAxisVisiable();
	}

	void LocalSettingDialog::updateCylinderInterface()
	{
		_ui->label->setText("Radius:");
		_ui->label_2->setText("Length:");
		_ui->label_3->setText("VIn:");
		_ui->label_4->setText("VOut:");
		_ui->backgroundField->setVisible(true);

		clearInterfaceData();
		updateValueInterface(4);
		cylinderSelectVisiable(true);
		cylinderAxisVisiable();
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
			break;
		default:
			break;
		}
	}

	void LocalSettingDialog::addPointsData()
	{
		double coor[3] = { 0 };
		_pointWidget->getCoordinate(coor);
		double v = _ui->doubleSpinBox->value();
		LocalPoint* p = new LocalPoint;
		p->x = coor[0];  p->y = coor[1]; p->z = coor[2];
		p->value = v;
		_points->append(p);
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

		Box* box = new Box;
		box->_xMin = coor[0]; box->_yMin = coor[1]; box->_zMin = coor[2];
		box->_xMax = box->_xMin + l; box->_yMax = box->_yMin + w; box->_zMax = box->_zMin + h;
		box->_vIn = vi; box->_vOut = vo; box->_thickness = tn;
		box->_backgroundField = _ui->backgroundField->isChecked();

		_fields->append(box);
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

		Ball* ball = new Ball;
		ball->_xCenter = coor[0]; ball->_yCenter = coor[1]; ball->_zCenter = coor[2];
		ball->_radius = r; ball->_vIn = vi; ball->_vOut = vo; ball->_thickness = tn;
		ball->_backgroundField = _ui->backgroundField->isChecked();

		_fields->append(ball);
		updateTab();

	}

	void LocalSettingDialog::addCylinderData()
	{
		double coor[3] = { 0 };
		double offset[3] = { 0 };
		_pointWidget->getCoordinate(coor);
		double r = _ui->doubleSpinBox->value();
		double lt = _ui->doubleSpinBox_2->value();
		double vi = _ui->doubleSpinBox_3->value();
		double vo = _ui->doubleSpinBox_4->value();
		double xv = _ui->xDoubleSpinBox->value();
		double yv = _ui->yDoubleSpinBox->value();
		double zv = _ui->zDoubleSpinBox->value();


		Cylinder* cd = new Cylinder;

		cd->_xCenter = coor[0]; cd->_yCenter = coor[1]; cd->_zCenter = coor[2];
		cd->_radius = r; cd->_vIn = vi; cd->_vOut = vo; cd->_length = lt;
		cd->_backgroundField = _ui->backgroundField->isChecked();

		if (_ui->xAxis->isChecked())
		{
			cd->_xAxis = lt / 2;
			offset[0] = lt / 2;
		}
		else if (_ui->yAxis->isChecked()) 
		{
			cd->_yAxis = lt/2;
			offset[1] = lt / 2;
		}
		else if (_ui->zAxis->isChecked())
		{
			cd->_zAxis = lt / 2;
			offset[2] = lt / 2;
		}
		else if (_ui->custom->isChecked())
		{
			int num = 0;
			if (xv != 0)num++;
			if (yv != 0)num++;
			if (zv != 0)num++;

			if (num > 0)
			{
				double val = lt / 2 / sqrt(num);
				cd->_xAxis = xv * val;
				cd->_yAxis = yv * val;
				cd->_zAxis = zv * val;
			}

			double xyz = (xv*xv) + (yv*yv) + (zv*zv);
			if (xyz > 0)
			{
				double ofs = lt / 2 / sqrt(xyz);

				offset[0] = ofs*xv;
				offset[1] = ofs*yv;
				offset[2] = ofs*zv;
			}
		}

		cd->_offset[0] = offset[0];
		cd->_offset[1] = offset[1];
		cd->_offset[2] = offset[2];


		_fields->append(cd);
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
		const int n = _points->size();
		const int f = _fields->size();
		_tab->setRowCount(n + f);
		updatePoints(n);
		updateFields(f, n);
// 		for (int i = 0; i < n; ++i)
// 		{
// 			LocalPoint* p = _points->at(i);
// 			QLabel *label = new QLabel("");
// 			label->setPixmap(QPixmap("://QUI/geometry/remove.png"));
// 			_tab->setCellWidget(i, 0, label);
// 			QString sp = QString("(%1, %2, %3)").arg(p->x).arg(p->y).arg(p->z);
// 			QString sv = QString::number(p->value);
// 			QTableWidgetItem* item1 = new QTableWidgetItem(sp);
// 			_tab->setItem(i, 1, item1);
// 			QTableWidgetItem* item2 = new QTableWidgetItem(sv);
// 			_tab->setItem(i, 2, item2);
// 			item1->setFlags(item1->flags() & (~Qt::ItemIsEditable));
// 			item2->setFlags(item2->flags() & (~Qt::ItemIsEditable));
// 		}
	
	}


	void LocalSettingDialog::updatePoints(int n)
	{
		for (int i = 0; i < n; ++i)
		{
			LocalPoint* p = _points->at(i);
			QLabel *label = new QLabel("");
			label->setPixmap(QPixmap("://QUI/geometry/remove.png"));
			_tab->setCellWidget(i, 0, label);
			QString sp = QString("p:(%1, %2, %3)").arg(p->x).arg(p->y).arg(p->z);
			QString sv = QString("Val:%1").arg(p->value);
			QTableWidgetItem* item1 = new QTableWidgetItem("Point");
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

	void LocalSettingDialog::updateFields(int n, int index)
	{
		for (int i = 0; i < n; ++i)
		{
			LocalField* p = _fields->at(i);
			QLabel *label = new QLabel("");
			label->setPixmap(QPixmap("://QUI/geometry/remove.png"));
			_tab->setCellWidget(i+index, 0, label);
			
			QString sp, sv,sn;
// 			switch (p->_type)
// 			{
// 			case FieldType::BoxField:
// 				Box* b = dynamic_cast<Box*>(p);
// 				sn = "Box";
// 				sp = QString("P:(%1, %2, %3) L:%4 W:%5 H:%6").arg(b->_xMin).arg(b->_yMin).arg(b->_zMin)
// 					.arg(b->_xMax - b->_xMin).arg(b->_yMax - b->_yMin).arg(b->_zMax - b->_zMin);
// 				sv = QString("VIn:%1 VOut:%2 Tck:%3").arg(b->_vIn).arg(b->_vOut).arg(b->_thickness);
// 				break;
// 			case FieldType::BallField:
// 				Ball* l = dynamic_cast<Ball*>(p);
// 				sn = "Ball";
// 				sp = QString("P:(%1, %2, %3) R:%4").arg(l->_xCenter).arg(l->_yCenter).arg(l->_zCenter).arg(l->_radius);
// 				sv = QString("VIn:%1 VOut:%2 Tck:%3").arg(l->_vIn).arg(l->_vOut).arg(l->_thickness);
// 				break;
// 			default:
//				break;
// 			}
			QStringList list = p->getInformation();
			if (list.size() != 3)
				continue;

			sn = list.at(0);
			sp = list.at(1);
			sv = list.at(2);

			QTableWidgetItem* item1 = new QTableWidgetItem(sn);
			_tab->setItem(i+index, 1, item1);
			QTableWidgetItem* item2 = new QTableWidgetItem(sp);
			_tab->setItem(i+index, 2, item2);
			QTableWidgetItem* item3 = new QTableWidgetItem(sv);
			_tab->setItem(i+index, 3, item3);
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

}



