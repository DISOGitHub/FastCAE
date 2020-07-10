#include "dialogCreateDatumplane.h"
#include "ui_dialogCreateDatumplane.h"
#include "mainWindow/mainWindow.h"
#include "moduleBase/ModuleType.h"
#include "MainWidgets/preWindow.h"
#include "GeometryCommand/GeoCommandList.h"
#include"GeometryCommand/GeoCommandCreateDatumplane.h"
#include <QMessageBox>
#include <QDebug>
#include <QColor>
#include <TopoDS.hxx>
#include <gp_Pln.hxx>
#include <TopoDS_Shape.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRep_Tool.hxx>
#include "geometry/geometrySet.h"

namespace GeometryWidget
{
	
	CreateDatumplaneDialog::CreateDatumplaneDialog(GUI::MainWindow* m, MainWidget::PreWindow* p)
		: GeoDialogBase(m, p)
	{
		_ui = new Ui::CreateDatumplaneDialog;
		_ui->setupUi(this);
		this->translateButtonBox(_ui->buttonBox);
	}

	CreateDatumplaneDialog::~CreateDatumplaneDialog()
	{
		if (_ui != nullptr) delete _ui;

	}

	void CreateDatumplaneDialog::shapeSlected(Geometry::GeometrySet* set, int index)
	{
		if (_faceBody != nullptr)
		{
			emit highLightGeometryFaceSig(_faceBody, _faceIndex, false);
		}

		_faceIndex = index;
		_faceBody = set;

		emit highLightGeometryFaceSig(_faceBody, _faceIndex, true);
 
	}

	void CreateDatumplaneDialog::on_geoSelectSurface_clicked()
	{
		emit setSelectMode(int(ModuleBase::GeometrySurface));
	}

	void CreateDatumplaneDialog::closeEvent(QCloseEvent *e)
	{
		QDialog::closeEvent(e);
		delete this;
	}

	void CreateDatumplaneDialog::accept()
	{
		double floc[3] = { 0.0 };
		double fdir[3] = { 0.0 };

		double distance{ 0.0 };
		double dir[3] = { 0.0 };

		bool ok = getDirection(floc, fdir);
		if (ok)
		{
			QString text = _ui->lineEditDistance->text();
			distance = text.toDouble(&ok);
			if (distance < 1e-6) ok = false;
		}

		memcpy(dir, fdir, sizeof(floc));
		if (ok)
		{
			if (_ui->reversecheckBox->isChecked())
			{
				for (int i = 0; i < 3; ++i)
					dir[i] *= -1;
			}
		}
		double mod = sqrt(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);
		if (mod < 1e-6) ok = false;
		if (ok)
		{
			double factor = distance / mod;
			for (int i = 0; i < 3; ++i)
				dir[i] *= factor;

		}		
		if (!ok)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Input Wrong !"));
			return;
		}
		Command::GeoCommandCreateDatumplane* command = new Command::GeoCommandCreateDatumplane(_mainWindow, _preWindow);
		command->setPlane(floc,fdir);
		command->setAxis(dir);
		command->setFaceBody(_faceBody,_faceIndex);

		bool success = Command::GeoComandList::getInstance()->executeCommand(command);
		if (!success)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Create failed ! "));
			return;
		}

		QDialog::accept();
		this->close(); 
	}

	void CreateDatumplaneDialog::reject()
	{
		QDialog::reject();
		this->close();
	}

	bool CreateDatumplaneDialog::getDirection(double* basePt, double* dir)
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
		return true;
	}

}
