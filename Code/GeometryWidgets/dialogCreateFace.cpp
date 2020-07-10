#include "dialogCreateFace.h"
#include "ui_dialogCreateFace.h"
#include "mainWindow/mainWindow.h"
#include "moduleBase/ModuleType.h"
#include "MainWidgets/preWindow.h"
#include "geometry/geometrySet.h"
#include "GeometryCommand/GeoCommandCreateFace.h"
#include "GeometryCommand/GeoCommandList.h"
#include <QMessageBox>
#include <QDebug>
#include "geometry/geometryParaFace.h"
#include "python/PyAgent.h"

namespace GeometryWidget
{
	CreateFaceDialog::CreateFaceDialog(GUI::MainWindow* m, MainWidget::PreWindow* pre)
		: GeoDialogBase(m,pre)
	{
		_ui = new Ui::CreateFaceDialog;
		_ui->setupUi(this);
		init();
		
	}

	CreateFaceDialog::CreateFaceDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set)
		: GeoDialogBase(m, p)
	{
		_ui = new Ui::CreateFaceDialog;
		_ui->setupUi(this);
		_isEdit = true;
		_editSet = set;
		this->setWindowTitle("Edit Face");
		init();
	}
	void CreateFaceDialog::init()
	{
		this->translateButtonBox(_ui->buttonBox);

		if (!_isEdit)
		{
			int id = Geometry::GeometrySet::getMaxID() + 1;
			_ui->namelineEdit->setPlaceholderText(QString("Face_%1").arg(id));
		}
		else
		{
			if (_editSet == nullptr) return;
			emit hideGeometry(_editSet);

			_ui->namelineEdit->setText(_editSet->getName());
			_ui->namelineEdit->setEnabled(false);
			Geometry::GeometryModelParaBase* bp = _editSet->getParameter();
			Geometry::GeometryParaFace* p = dynamic_cast<Geometry::GeometryParaFace*>(bp);
			if (p == nullptr) return;
			_shapeHash = p->getShapeHash();
			QMultiHash<Geometry::GeometrySet*, int>::iterator iter = _shapeHash.begin();
			for (; iter != _shapeHash.end(); ++iter)
			{
				emit highLightGeometryEdgeSig(iter.key(), iter.value(), true);
			}
			QString label = QString(tr("Selected edge(%1)")).arg(_shapeHash.size());
			_ui->edgelabel->setText(label);
		}
	}
	CreateFaceDialog::~CreateFaceDialog()
	{
		if (_ui != nullptr) delete _ui;
		emit setSelectMode((int)ModuleBase::None);
		emit updateGraphOptions();
	}

	void CreateFaceDialog::on_geoSelectCurve_clicked()
	{
		
		emit setSelectMode((int)ModuleBase::GeometryCurve);
		if (_shapeHash.size() > 0)
		{
			QList<Geometry::GeometrySet*> setList = _shapeHash.uniqueKeys();
			int k = setList.size();
			for (int i = 0; i < setList.size(); ++i)
			{
				Geometry::GeometrySet* set = setList.at(i);
				if (set == nullptr) return;
				QList<int> edlist = _shapeHash.values(setList[i]);
				for (int var : edlist)
				{
					emit highLightGeometryEdgeSig(set, var, true);
				}
			}
		}
	}

	void CreateFaceDialog::closeEvent(QCloseEvent *e)
	{
		QDialog::closeEvent(e);
		delete this;
	}

	void CreateFaceDialog::reject()
	{
		if (_editSet!=nullptr)
			emit showGeometry(_editSet);
		QDialog::reject();
		this->close();
	}

	void CreateFaceDialog::accept()
	{

		QString name = _ui->namelineEdit->text();
	    if (name.isEmpty())
			name = _ui->namelineEdit->placeholderText();
		bool ok = !name.isEmpty();
		
		if (!ok || _shapeHash.size() < 1)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Input Wrong !"));
			return;
		}

		QStringList codes{};
		codes += QString("face = CAD.Face()");
		if (_isEdit)
		{
			codes += QString("face.setEditID(%1)").arg(_editSet->getID());
		}
		else
			codes += QString("face.setName('%1')").arg(name);

		QList<Geometry::GeometrySet*> sets = _shapeHash.uniqueKeys();
		for (int i = 0; i < sets.size(); ++i)
		{
			auto s = sets.at(i);
			int id = s->getID();
			QList<int> indexs = _shapeHash.values(s);
			for(int var : indexs)
				codes += QString("face.appendEdge(%1,%2)").arg(id).arg(var);
		}

		if (_isEdit)
			codes += QString("face.edit()");
		else
			codes += QString("face.create()");
		_pyAgent->submit(codes);
/*

		Command::CommandCreateFace* c = new Command::CommandCreateFace(_mainWindow, _preWindow);
		c->setShapeList(_shapeHash);
		c->setName(name);
		c->setActor(_actors);
		int edges = _actors.size();
		c->setEdges(edges);
		if (_isEdit) c->setEditData(_editSet);

		bool success = Command::GeoComandList::getInstance()->executeCommand(c);
		if (!success)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Create failed ! "));
			return;
		}*/
		QDialog::accept();
		this->close();
	}

	void CreateFaceDialog::shapeSlected(Geometry::GeometrySet* set, int shape)
	{
		if (_shapeHash.contains(set, shape))
		{
			emit highLightGeometryEdgeSig(set, shape, false);
			_shapeHash.remove(set, shape);
		}
		else
		{
			emit highLightGeometryEdgeSig(set, shape, true);
			_shapeHash.insert(set, shape);
		}

		QString label = QString(tr("Selected edge(%1)")).arg(_shapeHash.size());
		_ui->edgelabel->setText(label);
	}


}
