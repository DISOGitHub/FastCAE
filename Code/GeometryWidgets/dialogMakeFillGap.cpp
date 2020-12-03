#include "dialogMakeFillGap.h"
#include "ui_dialogMakeFillGap.h"
#include "MainWidgets/preWindow.h"
#include "mainWindow/mainWindow.h"
#include "geometry/geometrySet.h"
#include <QDebug>  
#include "GeometryCommand/GeoCommandFillGap.h"
#include "GeometryCommand/GeoCommandList.h"
#include <QPair>
#include <QMessageBox>
#include "geometry/geometryParaMakeFillGap.h"
#include <BRepTools.hxx>
#include "python/PyAgent.h"
namespace GeometryWidget
{
	
	MakeFillGapDialog::MakeFillGapDialog(GUI::MainWindow* m, MainWidget::PreWindow* p)
		:GeoDialogBase(m, p)
	{
		_ui = new Ui::DialogFillGap;
		_ui->setupUi(this);
		connect(_ui->comboBoxType, SIGNAL(currentIndexChanged(int)), this, SLOT(on_TypeChanged(int)));
		_ui->tabWidget->tabBar()->hide();
	}

	MakeFillGapDialog::MakeFillGapDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set)
		:GeoDialogBase(m, p)
	{
		_ui = new Ui::DialogFillGap;
		_ui->setupUi(this);
		connect(_ui->comboBoxType, SIGNAL(currentIndexChanged(int)), this, SLOT(on_TypeChanged(int)));
		_ui->tabWidget->tabBar()->hide();
		_isEdit = true;
		_editSet = set;
		emit hideGeometry(_editSet);
		
		Geometry::GeometryModelParaBase* bp = _editSet->getParameter();
		Geometry::GeometryParaMakeFillGap* para = dynamic_cast<Geometry::GeometryParaMakeFillGap*>(bp);
		if (para == nullptr) return;
		
		QList<QPair<Geometry::GeometrySet*, int>> temp = para->getShapes();
		if (temp.size() != 2) return;
		emit showGeometry(temp.front().first);
		emit showGeometry(temp.back().first);

		_typeindex = para->getType();
		_ui->tabWidget->setCurrentIndex(_typeindex);
		_ui->comboBoxType->setCurrentIndex(_typeindex);
		switch (_typeindex)
		{
		case 0:
			_curve = para->getShapes();
			HighlightCurve(true);
			_ui->topedgelabel_2->setText(QString(tr("Selected Object(%1)")).arg(_curve.size()));
			break;
		case 1:
			_surface = para->getShapes();
			HighlightSurface(true);
			_ui->topedgelabel_3->setText(QString(tr("Selected Object(%1)")).arg(_surface.size()));
			break;
		case 2:
			_solid = para->getShapes();
			HighlightSolid(true);
			_ui->topedgelabel_4->setText(QString(tr("Selected Object(%1)")).arg(_solid.size()));
			break;
		default:
			break;
		}
		
	}

	MakeFillGapDialog::~MakeFillGapDialog()
	{
		if (nullptr != _ui) delete _ui;
	}

	void MakeFillGapDialog::on_TypeChanged(int index)
	{ 
		emit setSelectMode(int(ModuleBase::None));
		_ui->tabWidget->setCurrentIndex(index);
		_typeindex = index;
		
	}

	void MakeFillGapDialog::closeEvent(QCloseEvent *e)
	{
		QDialog::closeEvent(e);
		delete this;
	}

	void MakeFillGapDialog::reject()
	{
		if (_isEdit)
		{
			if (_editSet == nullptr) return;
			Geometry::GeometryModelParaBase* pm = _editSet->getParameter();
			Geometry::GeometryParaMakeFillGap* p = dynamic_cast<Geometry::GeometryParaMakeFillGap*>(pm);
			if (p == nullptr) return;
			QList<QPair<Geometry::GeometrySet*, int>> temp = p->getShapes();
			if (temp.size() != 2) return;
			emit hideGeometry(temp.front().first);
			emit hideGeometry(temp.back().first);
			emit showGeometry(_editSet);
		}

		QDialog::reject();
		this->close();
	}

	void MakeFillGapDialog::accept()
	{
		/*Command::CommandFillGap* c = new Command::CommandFillGap(_mainWindow, _preWindow);
		c->setGapType(_typeindex);
		switch (_typeindex)
		{
		case 0:c->setEdges(_curve); break;
		case 1:c->setFaces(_surface); break;
		case 2:c->setSolids(_solid); break;
		default:
			break;
		}
		if (_isEdit) c->setEditData(_editSet);
		bool ok = Command::GeoComandList::getInstance()->executeCommand(c);
		if (!ok)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Create failed !"));
			return;
		}*/

		QStringList codes{};
		codes += QString("fillgap = CAD.FillGap()");

		if (_isEdit)
			codes += QString("fillgap.setEditID(%1)").arg(_editSet->getID());
		QString str{};
		switch (_typeindex)
		{
		case 0:str = "Edge"; 
			codes += QString("fillgap.setIndexOfShape1(%1,%2)").arg(_curve.front().first->getID()).arg(_curve.front().second);
			codes += QString("fillgap.setIndexOfShape2(%1,%2)").arg(_curve.back().first->getID()).arg(_curve.back().second);
			break;
		case 1:str = "Surface";
			codes += QString("fillgap.setIndexOfShape1(%1,%2)").arg(_surface.front().first->getID()).arg(_surface.front().second);
			codes += QString("fillgap.setIndexOfShape2(%1,%2)").arg(_surface.back().first->getID()).arg(_surface.back().second);
			break;
		case 2:str = "Solid"; 
			codes += QString("fillgap.setIndexOfShape1(%1,%2)").arg(_solid.front().first->getID()).arg(_solid.front().second);
			codes += QString("fillgap.setIndexOfShape2(%1,%2)").arg(_solid.back().first->getID()).arg(_solid.back().second);
			break;
		default:
			break;
		}
		codes += QString("fillgap.setFillGapType('%1')").arg(str);
	
		if (_isEdit)
			codes += QString("fillgap.edit()");
		else
			codes += QString("fillgap.create()");

		_pyAgent->submit(codes);


		QDialog::accept();
		this->close();
	}

	void MakeFillGapDialog::on_geoSelectCurve_clicked()
	{
		HighlightSurface(false);
		HighlightSolid(false);
		if (_isEdit) HighlightCurve(true);
		emit setSelectMode(int(ModuleBase::GeometryCurve));
	}

	void MakeFillGapDialog::on_geoSelectSurface_1_clicked()
	{
		HighlightCurve(false);
		HighlightSolid(false);
		if (_isEdit) HighlightSurface(true);
		emit setSelectMode(int(ModuleBase::GeometrySurface));
	}

	void MakeFillGapDialog::on_geoSelectSurface_clicked()
	{
		HighlightCurve(false);
		HighlightSurface(false);
		if (_isEdit) HighlightSolid(true);
		emit setSelectMode(int(ModuleBase::GeometryBody));
	}

	void MakeFillGapDialog::shapeSlected(Geometry::GeometrySet* set, int index)
	{

		if (set == nullptr && index < 0) return;
		switch (_typeindex)
		{
		case 0: FillGapOfCurve(set, index); break;
		case 1: FillGapOfSurface(set, index); break;
		case 2: FillGapOfSolid(set, index); break;

		default:break;
		}   
	}


	void MakeFillGapDialog::FillGapOfCurve(Geometry::GeometrySet* set, int index)
	{
		if (set == nullptr) return;
		emit highLightGeometryEdgeSig(set, index, true);
		QPair<Geometry::GeometrySet*, int> temp{ set, index };
		_curve.push_back(temp);
		if (_curve.size() > 2)
		{
			QPair<Geometry::GeometrySet*, int> firstpair = _curve.front();
			Geometry::GeometrySet* geoset = firstpair.first;
			int setindex = firstpair.second;
			emit highLightGeometryEdgeSig(geoset, setindex, false);
			_curve.removeFirst();
		}
		_ui->topedgelabel_2->setText(QString(tr("Selected Object(%1)")).arg(_curve.size()));
		
	}

	void MakeFillGapDialog::FillGapOfSurface(Geometry::GeometrySet* set, int index)
	{
		if (set == nullptr) return;
		emit highLightGeometryFaceSig(set, index, true);
		QPair<Geometry::GeometrySet*, int> temp{ set, index };
		_surface.push_back(temp);
		if (_surface.size() > 2)
		{
			QPair<Geometry::GeometrySet*, int> firstpair = _surface.front();
			Geometry::GeometrySet* geoset = firstpair.first;
			int setindex = firstpair.second;
			emit highLightGeometryFaceSig(geoset, setindex, false);
			_surface.removeFirst();
		}
		_ui->topedgelabel_3->setText(QString(tr("Selected Object(%1)")).arg(_surface.size()));
	}

	void MakeFillGapDialog::FillGapOfSolid(Geometry::GeometrySet* set, int index)
	{
		
		if (set == nullptr) return;
		emit highLightGeometrySolidSig(set, index, true);
		QPair<Geometry::GeometrySet*, int> temp{ set, index };
		_solid.push_back(temp);
		if (_solid.size() > 2)
		{
			QPair<Geometry::GeometrySet*, int> firstpair = _solid.front();
			Geometry::GeometrySet* geoset = firstpair.first;
			int setindex = firstpair.second;
			emit highLightGeometrySolidSig(geoset, setindex, false);
			_solid.removeFirst();
		}
		_ui->topedgelabel_4->setText(QString(tr("Selected Object(%1)")).arg(_solid.size()));
		
	}

	void MakeFillGapDialog::HighlightCurve(bool f)
	{
		QList<QPair<Geometry::GeometrySet*, int>>::iterator it = _curve.begin();
		for (; it != _curve.end(); it++)
		{
			QPair<Geometry::GeometrySet*, int> onepair = *it;
			emit highLightGeometryEdgeSig(onepair.first, onepair.second, f);
			
		}
	}

	void MakeFillGapDialog::HighlightSurface(bool f)
	{
		QList<QPair<Geometry::GeometrySet*, int>>::iterator it = _surface.begin();
		for (; it != _surface.end(); it++)
		{
			QPair<Geometry::GeometrySet*, int> onepair = *it;
			emit highLightGeometryFaceSig(onepair.first, onepair.second, f);
			
		}
	}

	void MakeFillGapDialog::HighlightSolid(bool f)
	{
		QList<QPair<Geometry::GeometrySet*, int>>::iterator  it = _solid.begin();
		for (; it != _solid.end(); it++)
		{
			QPair<Geometry::GeometrySet*, int> onepair = *it;
			emit highLightGeometrySolidSig(onepair.first, onepair.second, f);
		}
	}


}
