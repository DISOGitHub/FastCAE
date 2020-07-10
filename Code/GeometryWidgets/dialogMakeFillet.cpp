#include "dialogMakeFillet.h"
#include "ui_dialogMakeFillet.h"
#include "mainWindow/mainWindow.h"
#include "moduleBase/ModuleType.h"
#include "MainWidgets/preWindow.h"
#include "GeometryCommand/GeoCommandCreateFillet.h"
#include "GeometryCommand/GeoCommandList.h"
#include <QMessageBox>
#include <QString>
#include "geometry/geometryModelParaBase.h"
#include "geometry/geometryParaFillet.h"
#include "geometry/geometrySet.h"
#include "python/PyAgent.h"

namespace GeometryWidget
{
	CreateFiletDialog::CreateFiletDialog(GUI::MainWindow* m, MainWidget::PreWindow* pre)
		: GeoDialogBase(m,pre)
	{
		_ui = new Ui::CreateFilterDialog;
		_ui->setupUi(this);
		this->translateButtonBox(_ui->buttonBox);
	}

	CreateFiletDialog::CreateFiletDialog(GUI::MainWindow* m, MainWidget::PreWindow* pre, Geometry::GeometrySet* set)
		: GeoDialogBase(m, pre)
	{
		if (_isEdit)
		{
			_ui->geoSelectCurve->setEnabled(false);
		}
		_ui = new Ui::CreateFilterDialog;
		_ui->setupUi(this);
		this->translateButtonBox(_ui->buttonBox);
		_isEdit = true;
		_editSet = set;
		this->setWindowTitle(tr("Edit Fillet"));
		if (_editSet == nullptr) return;
		Geometry::GeometryModelParaBase* pb = _editSet->getParameter();
		Geometry::GeometryParaFillet* p = dynamic_cast<Geometry::GeometryParaFillet*>(pb);
		if (p == nullptr) return;
		emit hideGeometry(_editSet);
		Geometry::GeometrySet*  originalSet = p->getGeometrySet();
		if (originalSet == nullptr) return;
		emit showGeometry(originalSet);

		QList<int> edges = p->getEdgeList();
		for (int i = 0; i < edges.size(); ++i)
		{
			int index = edges.at(i);
			_shapeHash.insert(originalSet, index);
			emit highLightGeometryEdgeSig(originalSet, index, true);
		}
		QString label = QString(tr("Selected edge(%1)")).arg(_shapeHash.size());
		_ui->edgelabel->setText(label);
		_ui->lineEditRadius->setText(QString::number(p->getRadius()));
	}

	CreateFiletDialog::~CreateFiletDialog()
	{
		if (_ui != nullptr) delete _ui;
	}

	void CreateFiletDialog::on_geoSelectCurve_clicked()
	{
		emit setSelectMode((int)ModuleBase::GeometryCurve);
		for (QMultiHash<Geometry::GeometrySet*, int>::iterator iter = _shapeHash.begin(); iter != _shapeHash.end(); ++iter)		{			emit highLightGeometryEdgeSig(iter.key(), iter.value(), true);		}
		
	}

	void CreateFiletDialog::closeEvent(QCloseEvent *e)
	{
		QDialog::closeEvent(e);
		delete this;
	}

	void CreateFiletDialog::reject()
	{
		if (_isEdit)
		{
			if (_editSet == nullptr) return;
			Geometry::GeometryModelParaBase* pb = _editSet->getParameter();
			Geometry::GeometryParaFillet* p = dynamic_cast<Geometry::GeometryParaFillet*>(pb);
			if (p == nullptr) return;
			Geometry::GeometrySet*  originalSet = p->getGeometrySet();
			if (originalSet == nullptr) return;
			emit hideGeometry(originalSet);
			emit showGeometry(_editSet);
		}

		QDialog::reject();
		this->close();
	}

	void CreateFiletDialog::accept()
	{
		QString text = _ui->lineEditRadius->text();
		bool ok = false;
		double r = text.toDouble(&ok);
		if (ok && _shapeHash.size() < 1)
			ok = false;

		if (!ok)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Input Wrong !"));
			return;
		}
		
		QStringList codes{};
		codes += QString("fillet = CAD.Fillet()");
		if (_isEdit)
			codes += QString("fillet.setEditID(%1)").arg(_editSet->getID());
		QList<Geometry::GeometrySet*> sets = _shapeHash.uniqueKeys();
		for (int i = 0; i < sets.size();  ++i)
		{
			auto s = sets.at(i);
			int id = s->getID();
			QList<int> indexs = _shapeHash.values(s);
			for(int var : indexs)
				codes += QString("fillet.appendEdge(%1,%2)").arg(id).arg(var);
 		}
		codes += QString("fillet.setRadius(%1)").arg(r);

 		if (_isEdit)
			codes += QString("fillet.edit()");
		else
			codes += QString("fillet.create()");

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

	void CreateFiletDialog::shapeSlected(Geometry::GeometrySet* set, int shape)
	{
		
		if (_isEdit&&_shapeHash.size()>0)
		{
			if (_shapeHash.keys().at(0) != set) return;
		}
		if (_shapeHash.contains(set, shape))
		{
			_shapeHash.remove(set, shape);
			emit highLightGeometryEdgeSig(set, shape, false);
		}
		else
		{
			_shapeHash.insert(set, shape);
			emit highLightGeometryEdgeSig(set, shape, true);
		}

		QString label = QString(tr("Selected edge(%1)")).arg(_shapeHash.size());
		_ui->edgelabel->setText(label);
	}

}