#include "dialogBoolOperation.h"
#include "ui_dialogBoolOperation.h"
#include "mainWindow/mainWindow.h"
#include "MainWidgets/preWindow.h"
#include <QString>
#include <QMessageBox>
#include "geometry/geometrySet.h"
#include "geometry/geometryParaBoolOperation.h"
#include "geometry/geometryData.h"
#include "GeometryCommand/GeoCommandBool.h"
#include "GeometryCommand/GeoCommandList.h"
#include "python/PyAgent.h"
namespace GeometryWidget
{
	BoolOpertionDialog::BoolOpertionDialog(GUI::MainWindow* m, MainWidget::PreWindow* p)
		: GeoDialogBase(m, p)
	{
		_ui = new Ui::BoolOptionDialog;
		_ui->setupUi(this);
		this->translateButtonBox(_ui->buttonBox);
		connect(this, SIGNAL(setSelectMode(int)), _mainWindow, SIGNAL(selectModelChangedSig(int)));
		connect(p, SIGNAL(selectGeoActorShape(vtkActor*, int, Geometry::GeometrySet*)), this, SLOT(selectActorShape(vtkActor*, int, Geometry::GeometrySet*)));
		connect(this, SIGNAL(highLightGeometrySet(Geometry::GeometrySet*, bool)), m, SIGNAL(highLightGeometrySetSig(Geometry::GeometrySet*, bool)));
	}

	BoolOpertionDialog::BoolOpertionDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set)
		:GeoDialogBase(m,p)
	{
		_ui = new Ui::BoolOptionDialog;
		_ui->setupUi(this);
		this->translateButtonBox(_ui->buttonBox);
		connect(this, SIGNAL(setSelectMode(int)), _mainWindow, SIGNAL(selectModelChangedSig(int)));
		connect(p, SIGNAL(selectGeoActorShape(vtkActor*, int, Geometry::GeometrySet*)), this, SLOT(selectActorShape(vtkActor*, int, Geometry::GeometrySet*)));
		connect(this, SIGNAL(highLightGeometrySet(Geometry::GeometrySet*, bool)), m, SIGNAL(highLightGeometrySetSig(Geometry::GeometrySet*, bool)));
		_isEdit = true;
		_editSet = set;
	   
		emit hideGeometry(_editSet);

		Geometry::GeometryModelParaBase* bp = _editSet->getParameter();
		Geometry::GeometryParaBoolOperation* para = dynamic_cast<Geometry::GeometryParaBoolOperation*>(bp);
		if (para == nullptr) return;
		Geometry::GeometrySet* body1 = para->getBody1();
		_body1 = body1;
		if (body1 == nullptr) return;
		Geometry::GeometrySet* body2 = para->getBody2();
		if (body2 == nullptr) return;
		_body2 = body2;
		
		emit showGeometry(body1);
		emit showGeometry(body2);
		emit highLightGeometrySet(body1,true);
		emit highLightGeometrySet(body2, true);

		_type = para->getType();
		setType(_type);
		QString text1 = tr("Selected body(1)");
		_ui->edgelabel->setText(text1);
		_ui->edgelabel_1->setText(text1);
	}

	BoolOpertionDialog::~BoolOpertionDialog()
	{
		if (_ui != nullptr) delete _ui;
		emit setSelectMode(int(ModuleBase::None));
		emit updateGraphOptions();
	}



	void BoolOpertionDialog::setType(BoolType t)
	{
		_type = t;
		QString text;
		switch (t)
		{
		case BoolCut:
			text = tr("Cut");
			break;
		case BoolFause:
			text = tr("Fause");
			break;
		case BoolCommon:
			text = tr("Common");
			break;
		default:

			text = tr("Unmaned");
			break;
		}
		if (_isEdit)
		{
			text = "Edit_" + text;
		}
		this->setWindowTitle(text);
	}

	void BoolOpertionDialog::on_geoSelectSurface_clicked()
	{
		_selectBody2 = false;
		_selectBody1 = true;
		emit setSelectMode(int(ModuleBase::GeometrySurface));
	}

	void BoolOpertionDialog::on_geoSelectSurface_1_clicked()
	{
		_selectBody2 = true;
		_selectBody1 = false;
		emit setSelectMode(int(ModuleBase::GeometrySurface));
	}

	void BoolOpertionDialog::selectActorShape(vtkActor*, int, Geometry::GeometrySet* set)
	{
		QString text1 = tr("Selected body(1)");
		QString text0 = tr("Selected body(0)");
		if (_selectBody1)
		{
			if (_body1 == set) return;
			emit highLightGeometrySet(_body1, false);
			_body1 = set;
			if (set == _body2)
			{
				_ui->edgelabel_1->setText(text0);
				_body2 = nullptr;
			}
				
			_ui->edgelabel->setText(text1);
		}
		if (_selectBody2)
		{
			if (_body2 == set) return;
			emit highLightGeometrySet(_body2, false);
			_body2 = set;
			if (set == _body1)
			{
				_body1 = nullptr;
				_ui->edgelabel->setText(text0);
			}
			_ui->edgelabel_1->setText(text1);
		}
		emit highLightGeometrySet(set, true);
	}

	void BoolOpertionDialog::closeEvent(QCloseEvent *e)
	{
		QDialog::closeEvent(e);
		delete this;
	}

	void BoolOpertionDialog::reject()
	{
		if (_isEdit)
		{
			if (_editSet == nullptr) return;
			Geometry::GeometryModelParaBase* pm = _editSet->getParameter();
			Geometry::GeometryParaBoolOperation* p = dynamic_cast<Geometry::GeometryParaBoolOperation*>(pm);
			if (p == nullptr) return;
			Geometry::GeometrySet* body1 = p->getBody1();
			if (body1 == nullptr) return;
			Geometry::GeometrySet* body2 = p->getBody2();
			if (body2 == nullptr) return;

			emit hideGeometry(body1);
			emit hideGeometry(body2);
			emit showGeometry(_editSet);
		}
		QDialog::reject();
		this->close();
	}

	void BoolOpertionDialog::accept()
	{
		if (_body1 == nullptr || _body2 == nullptr || _body2 == _body1)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Input Wrong !"));
			return;
		}

		QString str = booltypeToString(_type);
		QStringList codes{};
		codes += QString("booloperation = CAD.BooLOperation()");

		if (_isEdit)
			codes += QString("booloperation.setEditID(%1)").arg(_editSet->getID());

		codes += QString("booloperation.setBoolType('%1')").arg(str);
		codes += QString("booloperation.setBodysId(%1,%2)").arg(_body1->getID()).arg(_body2->getID());

		if (_isEdit)
			codes += QString("booloperation.edit()");
		else
			codes += QString("booloperation.create()");

		_pyAgent->submit(codes);


/*

		Command::CommandBool* c = new Command::CommandBool(_mainWindow, _preWindow);
		c->setType(_type);
		c->setInputBody(_body1, _body2);
		if (_isEdit) c->setEditData(_editSet);
		bool success = Command::GeoComandList::getInstance()->executeCommand(c);

		if (!success)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Create failed !"));
			return;
		}*/

		QDialog::accept();
		this->close();

	}

	QString BoolOpertionDialog::booltypeToString(BoolType b)
	{
		QString str{};
		switch (b)
		{

		case BoolNone:
			str = "None";
			break;
		case BoolCut:
			str = "Cut";
			break;
		case BoolFause:
			str = "Fause";
			break;
		case BoolCommon:
			str = "Common";
			break;
		default:
			break;
		}
		return str;
	}

	}