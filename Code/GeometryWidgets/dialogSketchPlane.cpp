#include "dialogSketchPlane.h"
#include "ui_dialogSketchPlane.h"
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include <TopoDS_Shape.hxx>
#include <QMessageBox>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <gp_Pln.hxx>
#include "mainWindow/mainWindow.h"

namespace GeometryWidget
{
	SketchPlanDialog::SketchPlanDialog(GUI::MainWindow* m, MainWidget::PreWindow* p)
		: GeoDialogBase(m,p, false, false)
	{
		_ui = new Ui::SketchPlaneDialog;
		_ui->setupUi(this);
		this->selecPlaneClicked();
		this->translateButtonBox(_ui->buttonBox);
		connect(_ui->radioButtonUser, SIGNAL(toggled(bool)), this, SLOT(selecPlaneClicked()));
		connect(this, SIGNAL(startSketch(bool, double*, double*)), m, SIGNAL(startSketchSig(bool, double*, double*)));
	}

	SketchPlanDialog::~SketchPlanDialog()
	{
		if (_ui != nullptr) delete _ui;
		/*emit updateGraphOptions();*/
	}

	void SketchPlanDialog::selecPlaneClicked()
	{
		bool s = _ui->radioButtonUser->isChecked();
		_ui->geoSelectSurface->setEnabled(s);
	}

	void SketchPlanDialog::on_geoSelectSurface_clicked()
	{
		emit setSelectMode((int)ModuleBase::GeometrySurface);
	}

	void SketchPlanDialog::closeEvent(QCloseEvent *e)
	{
		QDialog::closeEvent(e);
		delete this;
	}

	void SketchPlanDialog::reject()
	{
		double loc[3] = { 0.0 };
		double dir[3] = { 0.0 };
		
		emit startSketch(false, loc, dir);
		QDialog::reject();
		this->close();
	}

	void SketchPlanDialog::accept()
	{
		double loc[3] = { 0.0,0.0,0.0 };
		double dir[3] = { 0.0 ,0.0,0.0};
		bool ok = this->getDirection(loc, dir);

		if (!ok)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Create failed ! "));
			return;
		}
		_geoData->setSketchPlane(loc, dir);
		emit startSketch(true, loc, dir);

		QDialog::accept();
		this->close();
	}

	
	void SketchPlanDialog::shapeSlected(Geometry::GeometrySet* set, int index)
	{
		
		if (_faceBody != nullptr)
		{
			emit highLightGeometryFaceSig(_faceBody, _faceIndex, false);
			_faceBody = nullptr; _faceIndex = -1;
		}

		_faceBody = set;
		_faceIndex = index;
		emit highLightGeometryFaceSig(_faceBody, _faceIndex, true);
	
	}

	bool SketchPlanDialog::getDirection(double* basePt, double* dir)
	{
		if (_ui->radioButtonUser->isChecked())
		{
			if (_faceBody == nullptr) return false;

			TopoDS_Shape* shape = _faceBody->getShape();
			TopExp_Explorer faceExp(*shape, TopAbs_FACE);
			for (int index = 0; index < _faceIndex && faceExp.More(); faceExp.Next(), ++index);

			const TopoDS_Shape& faceShape = faceExp.Current();
			if (faceShape.IsNull()) return false;

			const TopoDS_Face &face = TopoDS::Face(faceShape);
			if (face.IsNull()) return false;

			BRepAdaptor_Surface adapt(face);
			if (adapt.GetType() != GeomAbs_Plane) return false;
			gp_Pln plane = adapt.Plane();
			gp_Ax1 normal = plane.Axis();

			gp_Pnt loca = normal.Location();
			gp_Dir dirt = normal.Direction();

			basePt[0] = loca.X(); basePt[1] = loca.Y(); basePt[2] = loca.Z();
			dir[0] = dirt.X(); dir[1] = dirt.Y(); dir[2] = dirt.Z();
		}
		else
		{
			basePt[0] = basePt[1] = basePt[2] = 0.00;
			dir[0] = dir[1] = dir[2] = 0.00;
			if (_ui->radioButtonXOY->isChecked())
				dir[2] = 1.0;
			else if (_ui->radioButtonXOZ->isChecked())
				dir[1] = 1.0;
			else if (_ui->radioButtonYOZ->isChecked())
				dir[0] = 1.0;
		}

		if (_ui->checkBoxReverse->isChecked())
			for (int i = 0; i < 3; i++)
				dir[i] *= -1;

		double m = 0.0;
		m = dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2];
		if (m < 1e-6) return false;
		
		return true;
	}

}