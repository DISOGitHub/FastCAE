#include "SketchPointWidget.h"
#include "ui_SketchPointWidget.h"
#include "mainWindow/mainWindow.h"
#include "geometry/geometryData.h"
#include <gp_Ax3.hxx>
#include "GeometryCommand/GeoSketchCreater.h"

namespace GeometryWidget
{
	SketchPointWidget::SketchPointWidget(GUI::MainWindow* m, MainWidget::PreWindow* pre, Command::SketchCreater* c)
		:GeoDialogBase(m,pre), _creater(c)
	{
		_ui = new Ui::SketchPointWidget;
		_ui->setupUi(this);
		this->setWindowFlags(windowFlags()&~Qt::WindowCloseButtonHint);
		_geoData = Geometry::GeometryData::getInstance();
	}

	SketchPointWidget::~SketchPointWidget()
	{
		if (_ui != nullptr) delete _ui;
	}

	void SketchPointWidget::on_addButton_clicked()
	{
		bool ok = false;
		QString text = _ui->lineEditX->text();
		double x = text.toDouble(&ok);
		if (!ok) return;
		text = _ui->lineEditY->text();
		double y = text.toDouble(&ok);
		if (!ok) return;

		gp_Ax3* sp = _geoData->getSketchPlane();
		gp_Pnt o = sp->Location();
		gp_Dir xa = sp->XDirection();
		gp_Dir ya = sp->YDirection();

		gp_Pnt pt;
		pt.SetX(o.X() + x*xa.X() + y*ya.X());
		pt.SetY(o.Y() + x*xa.Y() + y*ya.Y());
		pt.SetZ(o.Z() + x*xa.Z() + y*ya.Z());

		_creater->appendPoint(pt);

	}

}