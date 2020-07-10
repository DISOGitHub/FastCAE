#include "dialogMirrorFeature.h"
#include "ui_dialogMirrorFeature.h"
#include "mainWindow/mainWindow.h"
#include "MainWidgets/preWindow.h"
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include "geoPointWidget.h"
#include "GeometryCommand/GeoCommandMirrorFeature.h"
#include "GeometryCommand/GeoCommandList.h"
#include <QColor>
#include <QMessageBox>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <gp_Pln.hxx>
#include "geometry/geometryParaMakeMirror.h"
#include <QDebug>
#include "python/PyAgent.h"
namespace GeometryWidget
{
	MirorFeatureDialog::MirorFeatureDialog(GUI::MainWindow* m, MainWidget::PreWindow* p) :
		GeoDialogBase(m, p)
	{
		_ui = new Ui::MirrorFeatureDialog;
		_ui->setupUi(this);
		init();
		connect(_ui->comboBoxType, SIGNAL(currentIndexChanged(int)), this, SLOT(on_TypeChanged(int)));
		connect(_ui->radioButtonUser, SIGNAL(toggled(bool)), this, SLOT(on_radioButtonUser()));
		on_radioButtonUser();
	}

	MirorFeatureDialog::MirorFeatureDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set):
		GeoDialogBase(m,p)
	{
		_ui = new Ui::MirrorFeatureDialog;
		_ui->setupUi(this);
		_isEdit = true;
		_editSet = set;
		connect(_ui->comboBoxType, SIGNAL(currentIndexChanged(int)), this, SLOT(on_TypeChanged(int)));
		connect(_ui->radioButtonUser, SIGNAL(toggled(bool)), this, SLOT(on_radioButtonUser()));
		on_radioButtonUser();
		init();
	}

	MirorFeatureDialog::~MirorFeatureDialog()
	{
		if (_ui != nullptr) delete _ui;
		
	}

	void MirorFeatureDialog::closeEvent(QCloseEvent *e)
	{
		QDialog::closeEvent(e);
		delete this;
	}

	void MirorFeatureDialog::reject()
	{
		if (_isEdit)
		{
			if (_editSet == nullptr) return;
			Geometry::GeometryModelParaBase* pb = _editSet->getParameter();
			Geometry::GeometryParaMirrorFeature* p = dynamic_cast<Geometry::GeometryParaMirrorFeature*>(pb);
			if (p == nullptr) return;
			Geometry::GeometrySet*  originalSet = p->getOriSet();
			if (originalSet == nullptr) return;
			emit showGeometry(_editSet);
			emit hideGeometry(originalSet);
		}
		QDialog::reject();
		this->close();
	}

	void MirorFeatureDialog::init()
	{
		_ui->tabWidget->tabBar()->hide();
		_pw = new GeoPointWidget(_mainWindow, _preWindow);
		_ui->basicLayout->addWidget(_pw);

		this->translateButtonBox(_ui->buttonBox);
		connect(_pw, SIGNAL(buttonCkicked(GeoPointWidget*)), this, SLOT(pointWidgetClicked(GeoPointWidget*)));
		if (_isEdit)
		{
			_ui->geoSelectSurface->setEnabled(false);
			if (_editSet == nullptr) return;
			
			Geometry::GeometryModelParaBase* bp = _editSet->getParameter();
			Geometry::GeometryParaMirrorFeature* p = dynamic_cast<Geometry::GeometryParaMirrorFeature*>(bp);
			if (p == nullptr) return;

			auto subset = p->getOriSet();
			if (subset == nullptr) return;
			emit hideGeometry(_editSet);
			emit showGeometry(subset);
			
			int typeindex = p->getCurrentIndex();
			_ui->comboBoxType->setCurrentIndex(typeindex);
			_ui->tabWidget->setCurrentIndex(typeindex);
			_ui->checkBoxSaveOrigin->setChecked(p->getSaveOrigion());
			_bodysHash = p->getBodys();
			if (_bodysHash.size() < 1) return;
			QMultiHash<Geometry::GeometrySet*, int>::iterator iter = _bodysHash.begin();
			for (; iter != _bodysHash.end(); ++iter)
			{
				emit highLightGeometrySolidSig(iter.key(), iter.value(), true);
			}
			QString text = QString(tr("Selected body(%1)")).arg(_bodysHash.size());
			_ui->bodylabel->setText(text);

			if (typeindex==0)
			{
				_faceBodyPair.second = p->getFaceIndex();
				_faceBodyPair.first = p->getFaceBody();
			
				if (_faceBodyPair.first == nullptr)return;
				_ui->planelabel->setText(tr("Selected Plane(1)"));
				if (_faceBodyPair.first!=nullptr)
					emit highLightGeometryFaceSig(_faceBodyPair.first, _faceBodyPair.second,true );
			}
			else if (typeindex==1)
			{
				int planeindex = p->getPlaneIndex();
				_ui->comboBoxPlane->setCurrentIndex(planeindex);
			}
			else
			{
				double randomdir[3]{0.0}, basept[3]{0.0};
				p->getbasepoint(basept);
				_pw->setCoordinate(basept);
				p->getDirection(randomdir);

				if (randomdir[0] != 0 && randomdir[1] == 0 && randomdir[2] == 0)
				{
					_ui->radioButtonX->setChecked(true);
				}
				else if (randomdir[0] == 0 && randomdir[1] != 0 && randomdir[2] == 0)
				{
					_ui->radioButtonY->setChecked(true);
				}
				else if (randomdir[0] == 0 && randomdir[1] == 0 && randomdir[2] != 0)
				{
					_ui->radioButtonZ->setChecked(true);
				}
				else
				{
					_ui->radioButtonUser->setChecked(true);
					_ui->doubleSpinBoxX->setVisible(true);
					_ui->doubleSpinBoxY->setVisible(true);
					_ui->doubleSpinBoxZ->setVisible(true);
					_ui->doubleSpinBoxX->setValue(randomdir[0]);
					_ui->doubleSpinBoxY->setValue(randomdir[1]);
					_ui->doubleSpinBoxZ->setValue(randomdir[2]);
				}
			}
		}

	}

	void MirorFeatureDialog::accept()
	{
		bool legal = true;
		
		bool s = _ui->checkBoxSaveOrigin->isChecked();
		
		if (_typeindex == 0)
			if (_faceBodyPair.first == nullptr) legal = false;
		
		if ((!legal) || _bodysHash.size() < 1)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Input Wrong !"));
			return;
		}
/*		

		Command::CommandMirrorFeature* c = new Command::CommandMirrorFeature(_mainWindow, _preWindow);
		c->setBodys(_geobodyList);
		c->setPlane(loc, dir);
		c->setSaveOrigin(s);

		c->setTypeIndex(_typeindex);
		c->setFaceIndex(_faceIndex);
		c->setFaceBody(_faceBody);
		c->setPlaneIndex(_planeindex);
	
		c->setRandomDir(_randomdir);
		c->setBasePt(_basepoint);
		
		if (_isEdit) c->setEditData(_editSet);
		bool success = Command::GeoComandList::getInstance()->executeCommand(c);
		if (!success)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Create failed ! "));
			return;
		}*/

		_pw->getCoordinate(_basepoint);
		if (_ui->radioButtonX->isChecked())
			_randomdir[0] = 1.0;
		else if (_ui->radioButtonY->isChecked())
			_randomdir[1] = 1.0;
		else if (_ui->radioButtonZ->isChecked())
			_randomdir[2] = 1.0;
		else
		{
			_randomdir[0] = _ui->doubleSpinBoxX->value();
			_randomdir[1] = _ui->doubleSpinBoxY->value();
			_randomdir[2] = _ui->doubleSpinBoxZ->value();
		}

		QStringList codes{};
		codes += QString("mirrorfeature = CAD.MirrorFeature()");

		if (_isEdit)
			codes += QString("mirrorfeature.setEditID(%1)").arg(_editSet->getID());
	
		QMultiHash<Geometry::GeometrySet*, int>::iterator it = _bodysHash.begin();
		for (; it != _bodysHash.end(); it++)
		{
			codes += QString("mirrorfeature.appendBody(%1,%2)").arg(it.key()->getID()).arg(it.value());
		}

		QString mestr{};
		if (_typeindex == 0) mestr = "SelectPlaneOnGeo";
		else if (_typeindex == 1) mestr = "Coordinate";
		else if (_typeindex == 2) mestr = "Random";
		codes += QString("mirrorfeature.setSymmetricPlaneMethod('%1')").arg(mestr);
		int planindex = _ui->comboBoxPlane->currentIndex();

		if (_typeindex == 0)
			codes += QString("mirrorfeature.setFace(%1,%2)").arg(_faceBodyPair.first->getID()).arg(_faceBodyPair.second);
		if (_typeindex == 1)
		{
			QString planestr{};
			if (planindex == 0) planestr = "XOY";
			else if (planindex == 1) planestr = "XOZ";
			else if (planindex == 2) planestr = "YOZ";
			codes += QString("mirrorfeature.setPlaneMethod('%1')").arg(planestr);
		}
		if (_typeindex == 2)
		{
			codes += QString("mirrorfeature.setBasePt(%1,%2,%3)").arg(_basepoint[0]).arg(_basepoint[1]).arg(_basepoint[2]);
			codes += QString("mirrorfeature.setDir(%1,%2,%3)").arg(_randomdir[0]).arg(_randomdir[1]).arg(_randomdir[2]);
		}

		QString savestr{"No"};
		if (s) savestr = "Yes";
		codes += QString("mirrorfeature.SaveOrigin('%1')").arg(savestr);
		
		if (_isEdit)
			codes += QString("mirrorfeature.edit()");
		else
			codes += QString("mirrorfeature.create()");
		_pyAgent->submit(codes);
		
		QDialog::accept();
		this->close();
	}

	void MirorFeatureDialog::on_TypeChanged(int index)
	{
		_ui->tabWidget->setCurrentIndex(index);
		_typeindex = index;
	}

	void MirorFeatureDialog::on_geoSelectSurface_clicked()
	{
		emit setSelectMode((int)ModuleBase::GeometryBody);
		_selectPlane = false;
		_selectBody = true;
		_pw->handleProcess(false);

		
		if (_faceBodyPair.first != nullptr&&_faceBodyPair.second >= 0)
		{
			emit highLightGeometryFaceSig(_faceBodyPair.first, _faceBodyPair.second, false);
		}
		QList<Geometry::GeometrySet*> geolist = _bodysHash.uniqueKeys();
		
		for (int i = 0; i < geolist.size(); ++i)
		{
			Geometry::GeometrySet* set = geolist.at(i);
			QList<int> indexlist = _bodysHash.values(set);
			for (int j = 0; j < indexlist.size(); j++)
			{
				emit highLightGeometrySolidSig(set, indexlist[j], true);
			}
		}

	}

	void MirorFeatureDialog::on_geoSelectSurface_1_clicked()
	{
		emit setSelectMode((int)ModuleBase::GeometrySurface);
		_selectPlane = true;
		_selectBody = false;
		_pw->handleProcess(false);


		QList<Geometry::GeometrySet*> geolist = _bodysHash.uniqueKeys();

		for (int i = 0; i < geolist.size(); ++i)
		{
			Geometry::GeometrySet* set = geolist.at(i);
			QList<int> indexlist = _bodysHash.values(set);
			for (int j = 0; j < indexlist.size(); j++)
			{
				emit highLightGeometrySolidSig(set, indexlist[j], false);
			}
		}

		if (_faceBodyPair.first != nullptr&&_faceBodyPair.second >= 0)
		{
			emit highLightGeometryFaceSig(_faceBodyPair.first, _faceBodyPair.second, true);
		}
		

	}

	void MirorFeatureDialog::shapeSlected(Geometry::GeometrySet* set, int index)
	{
		if (_selectBody)
		{
			bool legal{};
			if (_bodysHash.size() > 0)
			{
				QMultiHash<Geometry::GeometrySet*, int>::iterator it = _bodysHash.begin();
				for (; it != _bodysHash.end(); it++)
				{
					if (it.key() == set&& it.value() == index)
					{
						it = _bodysHash.erase(it);
						emit highLightGeometrySolidSig(set, index, false);
						legal = true;
						break;
					}
				}
				
			}
			if (!legal)
			{
				_bodysHash.insert(set, index);
				emit highLightGeometrySolidSig(set, index, true);
			}
			QString text = QString(tr("Selected body(%1)")).arg(_bodysHash.size());
			_ui->bodylabel->setText(text);
		}
		else if (_selectPlane)
		{
			if (_faceBodyPair.first != nullptr)
			{
				emit highLightGeometryFaceSig(_faceBodyPair.first, _faceBodyPair.second, false);
			}
			_faceBodyPair.first = set;
			_faceBodyPair.second = index;

			emit highLightGeometryFaceSig(_faceBodyPair.first, index, true);
			_ui->planelabel->setText(tr("Selected Plane(1)"));
		}
	}

	bool MirorFeatureDialog::getDirection(double* basePt, double* dir)
	{

		if (_ui->comboBoxType->currentIndex() == 0)
		{
			_planeindex = 0;
			if (_faceBodyPair.first == nullptr) return false;

			TopoDS_Shape* shape = _faceBodyPair.first->getShape();
			TopExp_Explorer faceExp(*shape, TopAbs_FACE);
			for (int index = 0; index < _faceBodyPair.second && faceExp.More(); faceExp.Next(), ++index);

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
		else if (_ui->comboBoxType->currentIndex() == 1)
		{
			_planeindex = 1;
			const int index = _ui->comboBoxPlane->currentIndex();
			_planeindex = index;

			switch (index)
			{
			case 0: dir[2] = 1.0; break;
			case 1: dir[1] = 1.0; break;
			case 2: dir[0] = 1.0; break;
			default: break;
			}
			return true;
		}
		else if (_ui->comboBoxType->currentIndex() == 2)
		{
			_planeindex = 2;
			bool ok = false;
			ok = _pw->getCoordinate(basePt);
			for (int i = 0; i < 3; i++)
			{
				_basepoint[i] = basePt[i];
			}

			if (_ui->radioButtonX->isChecked())
			{
				dir[0] = 1.0;
			}
			else if (_ui->radioButtonY->isChecked())
				dir[1] = 1.0;
			else if (_ui->radioButtonZ->isChecked())
				dir[2] = 1.0;
			else
			{
				dir[0] = _ui->doubleSpinBoxX->value();
				dir[1] = _ui->doubleSpinBoxY->value();
				dir[2] = _ui->doubleSpinBoxZ->value();
			}
			for (int i = 0; i < 3; i++)
			{
				_randomdir[i] = dir[i];
			}
			
			return ok;
		}

	}

	void MirorFeatureDialog::pointWidgetClicked(GeoPointWidget* g)
	{
		QMultiHash<Geometry::GeometrySet*, int>::iterator iter = _bodysHash.begin();
		for (; iter != _bodysHash.end(); ++iter)
		{
			emit highLightGeometrySolidSig(iter.key(), iter.value(), false);
		}
		g->handleProcess(true);
		_selectPlane = false;
		_selectBody = false;
	}

	void MirorFeatureDialog::on_radioButtonUser()
	{
		bool checked = _ui->radioButtonUser->isChecked();
		_ui->doubleSpinBoxX->setVisible(checked);
		_ui->doubleSpinBoxY->setVisible(checked);
		_ui->doubleSpinBoxZ->setVisible(checked);
	}

}