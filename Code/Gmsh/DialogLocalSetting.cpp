#include "DialogLocalSetting.h"
#include "GmshDialogBase.h"
#include "ui_DialogLocalSetting.h"
#include "GeometryWidgets/geoPointWidget.h"
#include <QHeaderView>

namespace Gmsh
{
	LocalSettingDialog::LocalSettingDialog(GmshDialogBase* parent, GUI::MainWindow* m, MainWidget::PreWindow* p)
		:QDialog(parent), _points(parent->getLocalPoints())
	{
		_ui = new Ui::LocalSettingDialog;
		_ui->setupUi(this);
		_tab = _ui->tableWidget;
		_pointWidget = new GeometryWidget::GeoPointWidget(m, p);
		_ui->pointLayout->addWidget(_pointWidget);
		_tab->horizontalHeader()->setStretchLastSection(true);
		connect(_tab, SIGNAL(cellClicked(int, int)), this, SLOT(deleteItem(int, int)));
	}

	LocalSettingDialog::~LocalSettingDialog()
	{
		if (_ui != nullptr) delete _ui;
	}

	void LocalSettingDialog::on_PointAddButton_clicked()
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

	void LocalSettingDialog::on_closeButton_clicked()
	{
		this->close();
	}

	void LocalSettingDialog::deleteItem(int row, int col)
	{
		if (col != 0) return;
		int n = _points->size();
		if (row >= 0 && row < n)
		{
			_points->removeAt(row);
			updateTab();
		}
	}

	void LocalSettingDialog::showEvent(QShowEvent * e)
	{
		QDialog::showEvent(e);
		updateTab();
	}

	void LocalSettingDialog::updateTab()
	{
		int wid = _tab->width();
		_tab->setColumnWidth(0, 20);
		_tab->setColumnWidth(1, wid/2-12);
		_tab->setColumnWidth(2, wid/2-12);
//		_tab->clear();
		const int n = _points->size();
		_tab->setRowCount(n);
		for (int i = 0; i < n; ++i)
		{
			LocalPoint* p = _points->at(i);
			QLabel *label = new QLabel("");
			label->setPixmap(QPixmap("://QUI/geometry/remove.png"));
			_tab->setCellWidget(i, 0, label);
			QString sp = QString("(%1, %2, %3)").arg(p->x).arg(p->y).arg(p->z);
			QString sv = QString::number(p->value);
			QTableWidgetItem* item1 = new QTableWidgetItem(sp);
			_tab->setItem(i, 1, item1);
			QTableWidgetItem* item2 = new QTableWidgetItem(sv);
			_tab->setItem(i, 2, item2);
			item1->setFlags(item1->flags() & (~Qt::ItemIsEditable));
			item2->setFlags(item2->flags() & (~Qt::ItemIsEditable));
		}
	
	}



}



