#include "dialogGeoSplitter.h"
#include "ui_dialogGeoSplitter.h"
#include <QMessageBox>
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include "python/PyAgent.h"
#include <BOPAlgo_Splitter.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepTools.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include "GeometryCommand/GeoCommandGeoSplitter.h"
#include "GeometryCommand/GeoCommandList.h"
#include "geometry/geometryParaGeoSplitter.h"
#include "geoPointWidget.h"

namespace GeometryWidget
{
	GeoSplitterDialog::GeoSplitterDialog(GUI::MainWindow* m, MainWidget::PreWindow* p) :
		GeoDialogBase(m, p)
	{
		_ui = new Ui::GeoSplitterDialog;
		_ui->setupUi(this);
		connect(_ui->comboBoxType, SIGNAL(currentIndexChanged(int)), this, SLOT(on_TypeChanged(int)));
		connect(_ui->radioButtonUser, SIGNAL(toggled(bool)), this, SLOT(on_radioButtonUser()));
		on_radioButtonUser();
		init();
	}

	GeoSplitterDialog::GeoSplitterDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set) :
		GeoDialogBase(m,p)
	{
		_ui = new Ui::GeoSplitterDialog;
		_ui->setupUi(this);
		connect(_ui->comboBoxType, SIGNAL(currentIndexChanged(int)), this, SLOT(on_TypeChanged(int)));
		connect(_ui->radioButtonUser, SIGNAL(toggled(bool)), this, SLOT(on_radioButtonUser()));
		on_radioButtonUser();
		_isEdit = true;
		_editSet = set;
		init();

	}

	GeoSplitterDialog::~GeoSplitterDialog()
	{
		if (_ui != nullptr) delete _ui;
		emit setSelectMode((int)ModuleBase::None);
		emit updateGraphOptions();
	}
	void GeoSplitterDialog::init()
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
			Geometry::GeometryParaGeoSplitter* p = dynamic_cast<Geometry::GeometryParaGeoSplitter*>(bp);
			if (p == nullptr) return;
			auto oriset = p->getOriSet();
			if (oriset == nullptr) return;
			_bodysHash = p->getBodys();
			if (_bodysHash.size() < 1) return;
			QMultiHash<Geometry::GeometrySet*, int>::iterator iter = _bodysHash.begin();
			for (; iter != _bodysHash.end(); ++iter)
			{
				emit highLightGeometrySolidSig(iter.key(), iter.value(), true);
			}
			QString text = QString(tr("Selected body(1)"));
			_ui->bodylabel->setText(text);
			emit hideGeometry(_editSet);
			emit showGeometry(oriset);
			emit highLightGeometrySet(oriset, true);

			int typeindex = p->getCurrentIndex();
			_ui->comboBoxType->setCurrentIndex(typeindex);
			_ui->tabWidget->setCurrentIndex(typeindex);
			if (typeindex == 0)
			{
				_faceBodyPair.second = p->getFaceIndex();
				_faceBodyPair.first = p->getFaceBody();

				if (_faceBodyPair.first == nullptr)return;
				_ui->planelabel->setText(tr("Selected Plane(1)"));
				if (_faceBodyPair.first != nullptr)
					emit highLightGeometryFaceSig(_faceBodyPair.first, _faceBodyPair.second, true);
			}
			else if (typeindex == 1)
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
	
	void GeoSplitterDialog::closeEvent(QCloseEvent *e)
	{
		QDialog::closeEvent(e);
		delete this;
	}

	void GeoSplitterDialog::reject()
	{
		if (_isEdit)
		{
			if (_editSet == nullptr) return;


			Geometry::GeometryModelParaBase* bp = _editSet->getParameter();
			Geometry::GeometryParaGeoSplitter* p = dynamic_cast<Geometry::GeometryParaGeoSplitter*>(bp);
			if (p == nullptr) return;
			auto oriset = p->getOriSet();
			if (oriset == nullptr) return;

			emit hideGeometry(oriset);
			emit showGeometry(_editSet);
		}
		QDialog::reject();
		this->close();
	}

	void GeoSplitterDialog::accept()
	{
		bool success{ true };
		if (_typeindex == 0)
			if (_faceBodyPair.first == nullptr) success = false;


		if ( _bodysHash.size() < 1)
			success = false;
		if (!success) return;

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
		codes += QString("geosplitter = CAD.GeoSplitter()");
		QMultiHash<Geometry::GeometrySet*, int>::iterator it = _bodysHash.begin();
		for (; it != _bodysHash.end(); it++)
		{

			codes += QString("geosplitter.appendBody(%1,%2)").arg(it.key()->getID()).arg(it.value());
		}
		if (_isEdit)
			codes += QString("geosplitter.setEditID(%1)").arg(_editSet->getID());

		QString mestr{};
		if (_typeindex == 0) mestr = "SelectPlaneOnGeo";
		else if (_typeindex == 1) mestr = "Coordinate";
		else if (_typeindex == 2) mestr = "Random";
		codes += QString("geosplitter.setSymmetricPlaneMethod('%1')").arg(mestr);
		int planindex = _ui->comboBoxPlane->currentIndex();

		if (_typeindex == 0)
			codes += QString("geosplitter.setFace(%1,%2)").arg(_faceBodyPair.first->getID()).arg(_faceBodyPair.second);
		if (_typeindex == 1)
		{
			QString planestr{};
			if (planindex == 0) planestr = "XOY";
			else if (planindex == 1) planestr = "XOZ";
			else if (planindex == 2) planestr = "YOZ";
			codes += QString("geosplitter.setPlaneMethod('%1')").arg(planestr);
		}
		if (_typeindex == 2)
		{
			codes += QString("geosplitter.setBasePt(%1,%2,%3)").arg(_basepoint[0]).arg(_basepoint[1]).arg(_basepoint[2]);
			codes += QString("geosplitter.setDir(%1,%2,%3)").arg(_randomdir[0]).arg(_randomdir[1]).arg(_randomdir[2]);
		}


		if (_isEdit)
			codes += QString("geosplitter.edit()");
		else
			codes += QString("geosplitter.create()");
		_pyAgent->submit(codes);
	/*	Command::CommandGeoSplitter* c = new Command::CommandGeoSplitter(_mainWindow, _preWindow);
		c->setBody(_body);
		c->setFaceIndex(_faceIndex);
		c->setFaceBody(_faceBody);
		if (_isEdit) c->setEditData(_editSet);
		bool ok = Command::GeoComandList::getInstance()->executeCommand(c);
		if (!ok)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Create failed !"));
			return;
		}*/

		QDialog::accept();
		this->close();
		
	}

	void GeoSplitterDialog::on_geoSelectSurface_1_clicked()
	{	

		emit setSelectMode((int)ModuleBase::GeometrySurface);
		_selectPlane = true;
		_selectBody = false;
		_pw->handleProcess(false);

		if (_faceBodyPair.first != nullptr&&_faceBodyPair.second >= 0)
		{
			emit highLightGeometryFaceSig(_faceBodyPair.first, _faceBodyPair.second, true);
		}
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
	}

	void GeoSplitterDialog::on_geoSelectSurface_clicked()
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

	
	void GeoSplitterDialog::shapeSlected(Geometry::GeometrySet* set, int index)
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

	void GeoSplitterDialog::on_radioButtonUser()
	{
		bool checked = _ui->radioButtonUser->isChecked();
		_ui->doubleSpinBoxX->setVisible(checked);
		_ui->doubleSpinBoxY->setVisible(checked);
		_ui->doubleSpinBoxZ->setVisible(checked);
	}

	void GeoSplitterDialog::on_TypeChanged(int index)
	{
		_ui->tabWidget->setCurrentIndex(index);
		_typeindex = index;
	}

	void GeoSplitterDialog::pointWidgetClicked(GeoPointWidget* g)
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

}