#include "dialogMakeFillHole.h"
#include "ui_dialogMakeFillHole.h"
#include "mainWindow/mainWindow.h"
#include "moduleBase/ModuleType.h"
#include "MainWidgets/preWindow.h"
#include "GeometryCommand/GeoCommandMakeFillHole.h"
#include "GeometryCommand/GeoCommandList.h"
#include <QMessageBox>
#include <QString>
#include "geometry/geometryModelParaBase.h"
#include "geometry/geometryParaMakeFillHole.h"
#include "geometry/geometrySet.h"
#include "python/PyAgent.h"

namespace GeometryWidget
{
	MakeFillHoleDialog::MakeFillHoleDialog(GUI::MainWindow* m, MainWidget::PreWindow* pre)
		: GeoDialogBase(m,pre)
	{
		_ui = new Ui::FillHoleDialog;
		_ui->setupUi(this);
		this->translateButtonBox(_ui->buttonBox);
	}

	MakeFillHoleDialog::MakeFillHoleDialog(GUI::MainWindow* m, MainWidget::PreWindow* pre, Geometry::GeometrySet* set)
		: GeoDialogBase(m, pre)
	{
		if (_isEdit)
		{
			_ui->geoSelectSurface_1->setEnabled(false);
		}
		_ui = new Ui::FillHoleDialog;
		_ui->setupUi(this);
		this->translateButtonBox(_ui->buttonBox);
		_isEdit = true;
		_editSet = set;
		this->setWindowTitle(tr("Edit Hole"));
		if (_editSet == nullptr) return;
		Geometry::GeometryModelParaBase* pb = _editSet->getParameter();
		Geometry::GeometryParaMakeFillHole* p = dynamic_cast<Geometry::GeometryParaMakeFillHole*>(pb);
		if (p == nullptr) return;
		emit hideGeometry(_editSet);
		Geometry::GeometrySet*  originalSet = p->getGeometrySet();
		if (originalSet == nullptr) return;
		emit showGeometry(originalSet);
		
		QList<int> edges = p->getFaceList();
		for (int i = 0; i < edges.size(); ++i)
		{
			int index = edges.at(i);
			_shapeHash.insert(originalSet, index);
			emit highLightGeometryFaceSig(originalSet, index, true);
		}
		QString label = QString(tr("Selected Hole(%1)")).arg(_shapeHash.size());
		_ui->label->setText(label);
		//_ui->lineEditRadius->setText(QString::number(p->getRadius()));
	}

	MakeFillHoleDialog::~MakeFillHoleDialog()
	{
		if (_ui != nullptr) delete _ui;
	}

	void MakeFillHoleDialog::on_geoSelectSurface_1_clicked()
	{
		emit setSelectMode((int)ModuleBase::GeometrySurface);
		for (QMultiHash<Geometry::GeometrySet*, int>::iterator iter = _shapeHash.begin(); iter != _shapeHash.end(); ++iter)
		{
			emit highLightGeometryFaceSig(iter.key(), iter.value(), true);
		}

	}

	void MakeFillHoleDialog::closeEvent(QCloseEvent *e)
	{
		QDialog::closeEvent(e);
		delete this;
	}

	void MakeFillHoleDialog::reject()
	{
		if (_isEdit)
		{
			if (_editSet == nullptr) return;
			Geometry::GeometryModelParaBase* pb = _editSet->getParameter();
			Geometry::GeometryParaMakeFillHole* p = dynamic_cast<Geometry::GeometryParaMakeFillHole*>(pb);
			if (p == nullptr) return;
			Geometry::GeometrySet*  originalSet = p->getGeometrySet();
			if (originalSet == nullptr) return;
			emit hideGeometry(originalSet);
			emit showGeometry(_editSet);
		}

		QDialog::reject();
		this->close();
	}

	void MakeFillHoleDialog::accept()
	{
		bool ok{ true };
		if (_shapeHash.size() < 1)
			ok = false;
		if (!ok)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Input Wrong !"));
			return;
		}
		
/*

		Command::CommandMakeFillHole* c = new Command::CommandMakeFillHole(_mainWindow, _preWindow);
		c->setShapeList(_shapeHash);
		if (_isEdit) c->setEditData(_editSet);

		bool success = Command::GeoComandList::getInstance()->executeCommand(c);
		if (!success)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Create failed ! "));
			return;
		}
		*/
		QStringList codes{};
		codes += QString("fillhole = CAD.FillHole()");
		if (_isEdit)
			codes += QString("fillhole.setEditID(%1)").arg(_editSet->getID()); 
		QList<Geometry::GeometrySet*> sets = _shapeHash.uniqueKeys();
		for (int i = 0; i < sets.size();  ++i)
		{
			auto s = sets.at(i);
			int id = s->getID();
			QList<int> indexs = _shapeHash.values(s);
			for(int var : indexs)
				codes += QString("fillhole.appendFace(%1,%2)").arg(id).arg(var);
 		}

 		if (_isEdit)
			codes += QString("fillhole.edit()");
		else
			codes += QString("fillhole.create()");

		_pyAgent->submit(codes);

// 		Command::CommandCreateFillet* c = new Command::CommandCreateFillet(_mainWindow, _preWindow);
// 		c->setShapeList(_shapeHash);
// 		c->setRadius(r);
// 		if (_isEdit) c->setEditData(_editSet);
// 
// 		bool success = Command::GeoComandList::getInstance()->executeCommand(c);
// 		if (!success)
// 		{
// 			QMessageBox::warning(this, tr("Warning"), tr("Create failed ! "));
// 			return;
// 		}
		QDialog::accept();
		this->close();
	}

	void MakeFillHoleDialog::shapeSlected(Geometry::GeometrySet* set, int shape)
	{
		
		if (_isEdit&&_shapeHash.size()>0)
		{
			if (_shapeHash.keys().at(0) != set) return;
		}
		if (_shapeHash.contains(set, shape))
		{ 
			_shapeHash.remove(set, shape);
			emit highLightGeometryFaceSig(set, shape, false);
		}
		else
		{
			_shapeHash.insert(set, shape);
			emit highLightGeometryFaceSig(set, shape, true);
		}

		QString label = QString(tr("Selected Hole(%1)")).arg(_shapeHash.size()); 
		_ui->label->setText(label);
		
	}

}