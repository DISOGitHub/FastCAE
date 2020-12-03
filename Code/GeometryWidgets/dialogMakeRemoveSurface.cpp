#include "dialogMakeRemoveSurface.h"
#include "ui_dialogMakeRemoveSurface.h"
#include "mainWindow/mainWindow.h"
#include "moduleBase/ModuleType.h"
#include "MainWidgets/preWindow.h"
#include "GeometryCommand/GeoCommandMakeRemoveSurface.h"
#include "GeometryCommand/GeoCommandList.h"
#include <QMessageBox>
#include <QString>
#include "geometry/geometryModelParaBase.h"
#include "geometry/geometryParaMakeRemoveSurface.h"
#include "geometry/geometrySet.h"
#include "python/PyAgent.h"

namespace GeometryWidget
{
	MakeRemoveSurfaceDialog::MakeRemoveSurfaceDialog(GUI::MainWindow* m, MainWidget::PreWindow* pre)
		: GeoDialogBase(m,pre)
	{
		_ui = new Ui::DialogRemoveSurface;
		_ui->setupUi(this);
		this->translateButtonBox(_ui->buttonBox);
	}

	MakeRemoveSurfaceDialog::MakeRemoveSurfaceDialog(GUI::MainWindow* m, MainWidget::PreWindow* pre, Geometry::GeometrySet* set)
		: GeoDialogBase(m, pre)
	{
		if (_isEdit)
		{
			_ui->geoSelectSurface_1->setEnabled(false);
		}
		_ui = new Ui::DialogRemoveSurface;
		_ui->setupUi(this);
		this->translateButtonBox(_ui->buttonBox);
		_isEdit = true;
		_editSet = set;
		this->setWindowTitle(tr("Edit RemoveSurface"));
		if (_editSet == nullptr) return;
		Geometry::GeometryModelParaBase* pb = _editSet->getParameter();
		Geometry::GeometryParaMakeRemoveSurface* p = dynamic_cast<Geometry::GeometryParaMakeRemoveSurface*>(pb);
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
		QString label = QString(tr("Selected Surface(%1)")).arg(_shapeHash.size());
		_ui->label->setText(label);
		//_ui->lineEditRadius->setText(QString::number(p->getRadius()));
	}

	MakeRemoveSurfaceDialog::~MakeRemoveSurfaceDialog()
	{
		if (_ui != nullptr) delete _ui;
	}

	void MakeRemoveSurfaceDialog::on_geoSelectSurface_1_clicked()
	{
		emit setSelectMode((int)ModuleBase::GeometrySurface);
		for (QMultiHash<Geometry::GeometrySet*, int>::iterator iter = _shapeHash.begin(); iter != _shapeHash.end(); ++iter)
		{
			emit highLightGeometryFaceSig(iter.key(), iter.value(), true);
		}
		
	}

	void MakeRemoveSurfaceDialog::closeEvent(QCloseEvent *e)
	{
		QDialog::closeEvent(e);
		delete this;
	}

	void MakeRemoveSurfaceDialog::reject()
	{
		if (_isEdit)
		{
			if (_editSet == nullptr) return;
			Geometry::GeometryModelParaBase* pb = _editSet->getParameter();
			Geometry::GeometryParaMakeRemoveSurface* p = dynamic_cast<Geometry::GeometryParaMakeRemoveSurface*>(pb);
			if (p == nullptr) return;
			Geometry::GeometrySet*  originalSet = p->getGeometrySet();
			if (originalSet == nullptr) return;
			emit hideGeometry(originalSet);
			emit showGeometry(_editSet);
		}

		QDialog::reject();
		this->close();
	}

	void MakeRemoveSurfaceDialog::accept()
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
		Command::CommandMakeRemoveSurface* c = new Command::CommandMakeRemoveSurface(_mainWindow, _preWindow);
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
		codes += QString("removesurface = CAD.RemoveSurface()");
		if (_isEdit)
			codes += QString("removesurface.setEditID(%1)").arg(_editSet->getID()); 
		QList<Geometry::GeometrySet*> sets = _shapeHash.uniqueKeys();
		for (int i = 0; i < sets.size();  ++i)
		{
			auto s = sets.at(i);
			int id = s->getID();
			QList<int> indexs = _shapeHash.values(s);
			for(int var : indexs)
				codes += QString("removesurface.appendFace(%1,%2)").arg(id).arg(var);
 		}

 		if (_isEdit)
			codes += QString("removesurface.edit()");
		else
			codes += QString("removesurface.create()");

		_pyAgent->submit(codes);
		
		QDialog::accept();
		this->close();
	}

	void MakeRemoveSurfaceDialog::shapeSlected(Geometry::GeometrySet* set, int shape)
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

		QString label = QString(tr("Selected Surface(%1)")).arg(_shapeHash.size()); 
		_ui->label->setText(label);
		
	}

}