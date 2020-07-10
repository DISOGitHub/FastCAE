#include "dialogMakeChamfer.h"
#include "ui_dialogMakeChamfer.h"
#include "mainWindow/mainWindow.h"
#include "moduleBase/ModuleType.h"
#include "MainWidgets/preWindow.h"
#include "geometry/geometrySet.h"
#include "GeometryCommand/GeoCommandCreateChamfer.h"
#include "GeometryCommand/GeoCommandList.h"
#include "geometry/geometryParaChamfer.h"
#include "python/PyAgent.h"
#include <QMessageBox>
#include<QDebug>

namespace GeometryWidget
{
	CreateChamferDialog::CreateChamferDialog(GUI::MainWindow* m, MainWidget::PreWindow* pre)
		: GeoDialogBase(m,pre)
	{
		_ui = new Ui::CreateChamferDialog;
		_ui->setupUi(this);
		this->translateButtonBox(_ui->buttonBox);

		connect(_ui->comboBoxSection, SIGNAL(currentIndexChanged(int)), this, SLOT(on_sectionComboxChanged(int)));
		const int index = _ui->comboBoxSection->currentIndex();
		this->on_sectionComboxChanged(index);
		
	}

	CreateChamferDialog::CreateChamferDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set)
		:GeoDialogBase(m, p)
	{
		_ui = new Ui::CreateChamferDialog;
		_ui->setupUi(this);
		connect(_ui->comboBoxSection, SIGNAL(currentIndexChanged(int)), this, SLOT(on_sectionComboxChanged(int)));
		const int index = _ui->comboBoxSection->currentIndex();
		this->on_sectionComboxChanged(index);
		this->translateButtonBox(_ui->buttonBox);
		
		_isEdit = true;
		_editSet = set;
		this->setWindowTitle("Edit Chamfer");
		init();
	}

	void CreateChamferDialog::init()
	{
		if (_isEdit)
			_ui->geoSelectCurve->setEnabled(false);
		if (_editSet == nullptr) return;
		auto subset = _editSet->getSubSetAt(0);
		emit hideGeometry(_editSet);
		emit showGeometry(subset);

		Geometry::GeometryModelParaBase* bp = _editSet->getParameter();
		Geometry::GeometryParaChamfer* p = dynamic_cast<Geometry::GeometryParaChamfer*>(bp);
		if (p == nullptr) return;
		bool combIndex = p->getCombIndex();
		double d1 = p->getDistance1();
		double d2 = p->getDistance2();
		Geometry::GeometrySet* inputset = p->getGeometrySet();
		if (inputset == nullptr)return;
		QList<int> edgeIndexs = p->getEdgeIndexList();
	
		for(int var : edgeIndexs)
		{
			_shapeHash.insert(inputset, var);
			emit highLightGeometryEdgeSig(subset, var, true);
		}
		QString label = QString(tr("Selected edge(%1)")).arg(edgeIndexs.size());
		_ui->edgelabel->setText(label);
		_ui->lineEditDistance->setText(QString::number(d1));
		_ui->lineEditDistance_2->setText(QString::number(d2));

		if (combIndex == true)
		{
			_ui->comboBoxSection->setCurrentIndex(0);
			_ui->lineEditDistance_2->setEnabled(false);

		}
		else
		{
			_ui->comboBoxSection->setCurrentIndex(1);
			_ui->lineEditDistance_2->setEnabled(true);
		}

	}

	CreateChamferDialog::~CreateChamferDialog()
	{
		if (_ui != nullptr) delete _ui;

	}

	void CreateChamferDialog::on_geoSelectCurve_clicked()
	{
		
		emit setSelectMode((int)ModuleBase::GeometryCurve);
	
		for (QMultiHash<Geometry::GeometrySet*, int>::iterator iter = _shapeHash.begin(); iter != _shapeHash.end(); ++iter)
		{
			emit highLightGeometryEdgeSig(iter.key(), iter.value(), true);
		}
			
	}

	void CreateChamferDialog::closeEvent(QCloseEvent *e)
	{
		QDialog::closeEvent(e);
		delete this;
	}

	void CreateChamferDialog::reject()
	{
		if (_isEdit)
		{
			if (_editSet == nullptr) return;
			Geometry::GeometryModelParaBase* pm = _editSet->getParameter();
			Geometry::GeometryParaChamfer* p = dynamic_cast<Geometry::GeometryParaChamfer*>(pm);
			if (p == nullptr) return;
			Geometry::GeometrySet* ori = p->getGeometrySet();
			if (ori == nullptr) return;

			emit hideGeometry(ori);
			emit showGeometry(_editSet);
		}
		QDialog::reject();
		this->close();
	}

	void CreateChamferDialog::accept()
	{
		QString text = _ui->lineEditDistance->text();
		bool ok = false;
		double d1 = text.toDouble(&ok);
		double d2 = 0;
		text = _ui->lineEditDistance_2->text();
		if (ok)
			d2 = text.toDouble(&ok);

		if (ok && _shapeHash.size() < 1)
			ok = false;

		if (!ok)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Input Wrong !"));
			return;
		}

		int sectionid = _ui->comboBoxSection->currentIndex();
		QString sectiontype{};
		if (sectionid == 0)
			sectiontype = "Symmetrical";
		else
			sectiontype = "Asymmetrical";
		
		QStringList codes{};
 		codes += QString("chamfer = CAD.Chamfer()");
		if (_isEdit)
			codes += QString("chamfer.setEditID(%1)").arg(_editSet->getID());
		codes += QString("chamfer.setSectionType('%1')").arg(sectiontype);

		QList<Geometry::GeometrySet*> sets = _shapeHash.uniqueKeys();
		for (int i = 0; i < sets.size(); ++i)
		{
			auto s = sets.at(i);
			if (s == nullptr)continue;
			int id = s->getID();
			QList<int> indexs = _shapeHash.values(s);
			for(int var : indexs)
				codes += QString("chamfer.appendEdge(%1,%2)").arg(id).arg(var);
		}

		if (sectionid == 0)
			codes += QString("chamfer.setSymmetricalDistance(%1)").arg(d1);
		else
			codes += QString("chamfer.setAsymmetricalDistances(%1,%2)").arg(d1).arg(d2);
			
		if (_isEdit)
			codes += QString("chamfer.edit()");
		else
			codes += QString("chamfer.create()");

		_pyAgent->submit(codes);
		/*

		/*Command::CommandCreateChamfer* c = new Command::CommandCreateChamfer(_mainWindow, _preWindow);

		QList<Geometry::GeometrySet*> setList = _shapeHash.keys();
		QList<int> setidList;
		QString setidStr{};
		for (int i = 0; i < setList.size(); ++i)
		{
			setidStr.append(QString::number((setList[i]->getID())));
			if (i != (setList.size() - 1)) setidStr.append(",");

		
		}
		qDebug() << setidStr;

		QList<int> indexList = _shapeHash.values();
		QString indexListStr{};
		for (int i = 0; i < indexList.size(); ++i)
		{
			indexListStr.append(QString::number(indexList[i]));
			if (i != (indexList.size() - 1)) indexListStr.append(",");

		}
		qDebug() << indexListStr;
		c->setShapeList(_shapeHash);
		c->setSymmetrical(sym);
		c->setDistance(d1,d2);
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

	void CreateChamferDialog::shapeSlected(Geometry::GeometrySet* set, int shape)
	{

		if (_isEdit&&_shapeHash.size() > 0)
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

	void CreateChamferDialog::on_sectionComboxChanged(int index)
	{
		bool v = true;
		if (index == 0) v = false;
		_ui->label_4->setEnabled(v);
		_ui->lineEditDistance_2->setEnabled(v);
		_ui->label_7->setEnabled(v);
	}


}