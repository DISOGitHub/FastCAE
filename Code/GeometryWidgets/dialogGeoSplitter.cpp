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

namespace GeometryWidget
{
	GeoSplitterDialog::GeoSplitterDialog(GUI::MainWindow* m, MainWidget::PreWindow* p) :
		GeoDialogBase(m, p)
	{
		_ui = new Ui::GeoSplitterDialog;
		_ui->setupUi(this);
		
	}

	GeoSplitterDialog::GeoSplitterDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set) :
		GeoDialogBase(m,p)
	{
		_ui = new Ui::GeoSplitterDialog;
		_ui->setupUi(this);
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
			_faceBodyPair.second = p->getFaceIndex();
			_faceBodyPair.first = p->getFaceBody();

			if (_faceBodyPair.second >= 0 && _faceBodyPair.first != nullptr)
			{
				emit highLightGeometryFaceSig(_faceBodyPair.first, _faceBodyPair.second, true);
				_ui->bodylabel_2->setText(tr("Selected Plane(1)"));
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
		if (_faceBodyPair.first == nullptr || _bodysHash.size() < 1 || _faceBodyPair.second < 0)
			success = false;
		if (!success) return;
		QStringList codes{};
		codes += QString("geosplitter = CAD.GeoSplitter()");
		QMultiHash<Geometry::GeometrySet*, int>::iterator it = _bodysHash.begin();
		for (; it != _bodysHash.end(); it++)
		{
			codes += QString("geosplitter.appendBody(%1,%2)").arg(it.key()->getID()).arg(it.value());
		}
		if (_isEdit)
			codes += QString("geosplitter.setEditID(%1)").arg(_editSet->getID());
		codes += QString("geosplitter.setFace(%1,%2)").arg(_faceBodyPair.first->getID()).arg(_faceBodyPair.second);
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
			_ui->bodylabel_2->setText(tr("Selected Plane(1)"));
			
		}
	}

}