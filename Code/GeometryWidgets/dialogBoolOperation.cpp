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
	}

	BoolOpertionDialog::BoolOpertionDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set)
		:GeoDialogBase(m,p)
	{
		_ui = new Ui::BoolOptionDialog;
		_ui->setupUi(this);
		this->translateButtonBox(_ui->buttonBox);
		_isEdit = true;
		_editSet = set;
		emit hideGeometry(_editSet);
		  
		Geometry::GeometryModelParaBase* bp = _editSet->getParameter();
		Geometry::GeometryParaBoolOperation* para = dynamic_cast<Geometry::GeometryParaBoolOperation*>(bp);
		if (para == nullptr) return;
		_bodypair1 = para->getBody1();
		if (_bodypair1.first == nullptr) return;
		_bodypair2 = para->getBody2();
		if (_bodypair2.first == nullptr) return;
		emit showGeometry(_bodypair1.first);
		emit showGeometry(_bodypair2.first);
		emit highLightGeometrySolidSig(_bodypair1.first, _bodypair1.second, true);
		emit highLightGeometrySolidSig(_bodypair2.first,_bodypair2.second, true);

		_type = para->getType();
		setType(_type);
		QString text1 = tr("Selected body(1)");
		_ui->edgelabel->setText(text1);
		_ui->edgelabel_1->setText(text1);
	}

	BoolOpertionDialog::~BoolOpertionDialog()
	{
		if (_ui != nullptr) delete _ui;
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
		
		emit setSelectMode(int(ModuleBase::GeometryBody));
		if (_bodypair1.first != nullptr)
		{
			emit highLightGeometrySolidSig(_bodypair1.first, _bodypair1.second, true);
		}
		if (_bodypair2.first != nullptr)
		{
			emit highLightGeometrySolidSig(_bodypair2.first, _bodypair2.second, false);
		}
	}

	void BoolOpertionDialog::on_geoSelectSurface_1_clicked()
	{
		_selectBody2 = true;
		_selectBody1 = false;
		
		emit setSelectMode(int(ModuleBase::GeometryBody));
		if (_bodypair2.first != nullptr)
		{
			emit highLightGeometrySolidSig(_bodypair2.first, _bodypair2.second, true);
		}
		if (_bodypair1.first != nullptr)
		{
			emit highLightGeometrySolidSig(_bodypair1.first, _bodypair1.second, false);
		}
	}

	void BoolOpertionDialog::shapeSlected(Geometry::GeometrySet* set, int index)
	{
		QString text1 = tr("Selected body(1)");
		QString text0 = tr("Selected body(0)");
		if (_selectBody1)
		{
			if (_bodypair1.first == set&&_bodypair1.second==index) return;
			if (_bodypair1.first!=nullptr)
			{
				emit highLightGeometrySolidSig(_bodypair1.first, _bodypair1.second, false);
			}
			_bodypair1.first = set;
			_bodypair1.second = index;
			if (set == _bodypair2.first&&index == _bodypair2.second)
			{
				_ui->edgelabel_1->setText(text0);
				_bodypair2.first = nullptr;
				_bodypair2.second = -1;
			}
				
			_ui->edgelabel->setText(text1);
		}
		if (_selectBody2)
		{
			if (_bodypair2.first == set&&_bodypair2.second == index) return;
			emit highLightGeometrySolidSig(_bodypair2.first,_bodypair2.second, false);
			_bodypair2.first = set;
			_bodypair2.second = index;
			if (set == _bodypair1.first&&index == _bodypair2.second)
			{
				_bodypair1.first = nullptr;
				_bodypair1.second = -1;
				_ui->edgelabel->setText(text0);
			}
			_ui->edgelabel_1->setText(text1);
		}
		emit highLightGeometrySolidSig(set,index, true);
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
			_bodypair1 = p->getBody1();
			if (_bodypair1.first == nullptr) return;
			_bodypair2 = p->getBody2();
			if (_bodypair2.first == nullptr) return;
			
			emit hideGeometry(_bodypair1.first);
			emit hideGeometry(_bodypair2.first);
			emit showGeometry(_editSet);
		}
		QDialog::reject();
		this->close();
	}

	void BoolOpertionDialog::accept()
	{
		//可编辑状态下，没有任何改变，则保留editset，不进入command执行。
		if (_isEdit)
		{
			Geometry::GeometryModelParaBase* pm = _editSet->getParameter();
			Geometry::GeometryParaBoolOperation* p = dynamic_cast<Geometry::GeometryParaBoolOperation*>(pm);
			if (p == nullptr) return;
			QPair<Geometry::GeometrySet*, int> bodypair1 = p->getBody1();
			if (_bodypair1.first == nullptr) return;
			QPair<Geometry::GeometrySet*, int> bodypair2 = p->getBody2();
			if (_bodypair2.first == nullptr) return;
			if (_bodypair1==bodypair1&&_bodypair2==bodypair2)
			{
				emit hideGeometry(_bodypair1.first);
				emit hideGeometry(_bodypair2.first);
				emit showGeometry(_editSet);
				QDialog::accept();
				this->close();
				return;
			}
			
		}
		if (_bodypair1.first == nullptr || _bodypair2.first == nullptr || _bodypair2.first == _bodypair1.first)
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
		codes += QString("booloperation.setIndexOfSolid1InGeo(%1,%2)").arg(_bodypair1.first->getID()).arg(_bodypair1.second);
		codes += QString("booloperation.setIndexOfSolid2InGeo(%1,%2)").arg(_bodypair2.first->getID()).arg(_bodypair2.second);

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