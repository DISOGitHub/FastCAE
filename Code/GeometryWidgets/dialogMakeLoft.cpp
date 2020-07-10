#include "ui_dialogMakeLoft.h"
#include "mainWindow/mainWindow.h"
#include "moduleBase/ModuleType.h"
#include "MainWidgets/preWindow.h"
#include "GeometryCommand/GeoCommandList.h"
#include "GeometryCommand/GeoCommandMakeLoft.h"
#include <QMessageBox>
#include <QDebug>
#include <QColor>
#include <TopoDS.hxx>
#include <TopExp_Explorer.hxx>
#include <BRep_Tool.hxx>
#include "geometry/geometrySet.h"
#include "dialogMakeLoft.h"
#include <BRepBuilderAPI_MakeWire.hxx>
#include <TopoDS_Wire.hxx>
#include "python/PyAgent.h"
#include "geometry/geometryParaLoft.h"

namespace GeometryWidget
{
	
	CreateLoftDialog::CreateLoftDialog(GUI::MainWindow* m, MainWidget::PreWindow* p)
		: GeoDialogBase(m, p)
	{
		_ui = new Ui::CreateLoftDialog;
		_ui->setupUi(this);
		_ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
		this->translateButtonBox(_ui->buttonBox);

		connect(_ui->tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(tableClicked(int, int)));
	}

	CreateLoftDialog::CreateLoftDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set)
		:GeoDialogBase(m, p)
	{
		_ui = new Ui::CreateLoftDialog;
		_ui->setupUi(this);
		_ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
		this->translateButtonBox(_ui->buttonBox);

		connect(_ui->tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(tableClicked(int, int)));
		_isEdit = true;
		_editSet = set;
		init();
	}

	void CreateLoftDialog::init()
	{
		if (_editSet == nullptr) return;
		Geometry::GeometryModelParaBase* bp = _editSet->getParameter();
		Geometry::GeometryParaLoft* p = dynamic_cast<Geometry::GeometryParaLoft*>(bp);
		if (p == nullptr) return;
		emit hideGeometry(_editSet);
		_allShapes = p->getShapeHash();
		
		for(QMultiHash<Geometry::GeometrySet*, int> var : _allShapes)
		{
			QList<Geometry::GeometrySet*> setList = var.keys();
			int k = setList.size();
			for (int i = 0; i < setList.size(); ++i)
			{
				QList<int> edlist = var.values(setList[i]);
				Geometry::GeometrySet* set = setList.at(i);
				if (set == nullptr) return;
				for (int var : edlist)
				{
					emit highLightGeometryEdgeSig(set, var, true);
				}
			}
		}
		bool solid = p->getSloid();
		_ui->solidCheckBox->setChecked(solid);
		updateTab();
	}

	CreateLoftDialog::~CreateLoftDialog()
	{
		if (_ui != nullptr) delete _ui;
	}

	void CreateLoftDialog::shapeSlected(Geometry::GeometrySet* set, int index)
	{
	
		if (_tempShapes.contains(set,index))
		{
			emit highLightGeometryEdgeSig(set, index, false);
			_tempShapes.remove(set, index);
		}
		else
		{
			emit highLightGeometryEdgeSig(set, index, true);
			_tempShapes.insert(set, index);
		}
		QString label = QString(tr("Selected TopEdge(%1)")).arg(_tempShapes.size());
		_ui->topedgelabel->setText(label);
	}

	void CreateLoftDialog::on_addButton_clicked()
	{
		_allShapes.append(_tempShapes);

		QList<Geometry::GeometrySet*> setlist = _tempShapes.uniqueKeys();
		for (int i = 0; i < setlist.size(); i++)
		{
			QList<int> indexlist = _tempShapes.values(setlist[i]);
			for (int index : indexlist)
			{
				emit highLightGeometryEdgeSig(setlist[i], index, false);
			}
		}
		_tempShapes.clear();
		QString label = QString(tr("Selected TopEdge(%1)")).arg(_tempShapes.size());
		_ui->topedgelabel->setText(label);
		updateTab();
	}

	void CreateLoftDialog::on_removeButton_clicked()
	{
		const int n = _allShapes.size();
		if (n <= 0)return;

		for (int j = 0; j < _allShapes.size(); j++)
		{
			QList<Geometry::GeometrySet*> setlist = _allShapes[j].uniqueKeys();
			for (int i = 0; i < setlist.size(); i++)
			{
				QList<int> indexlist = _tempShapes.values(setlist[i]);
				for (int index : indexlist)
				{
					emit highLightGeometryEdgeSig(setlist[i], index, false);
				}
			}
		}
		_allShapes.removeLast();
		updateTab();
	}

	void CreateLoftDialog::on_geoSelectCurve_clicked()
	{
		emit setSelectMode(int(ModuleBase::GeometryCurve));
		QList<Geometry::GeometrySet*> keys = _tempShapes.keys();
		for (int i = 0; i < keys.size(); i++)
		{
			int index = _tempShapes.value(keys[i]);
			emit highLightGeometryEdgeSig(keys[i], index, true);
		}

	}

	void CreateLoftDialog::reject()
	{
		if (_isEdit)
		{
			if (_editSet == nullptr) return;
			emit showGeometry(_editSet);

		}

		QDialog::reject();
		this->close();
	}

	void CreateLoftDialog::closeEvent(QCloseEvent *e)
	{
		QDialog::closeEvent(e);
		delete this;
	}

	void CreateLoftDialog::accept()
	{
		bool success = true;
		if (_allShapes.size() < 1) success = false;
		if (!success)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Create failed ! "));
			return;
		}
		bool isSolid = _ui->solidCheckBox->isChecked();
		int id = Geometry::GeometrySet::getMaxID() + 1;
		QString name = (QString("Loft_%1").arg(id));

		QStringList codes{};
		codes += QString("loft = CAD.Loft()");

		if (_allShapes.size()<1) return;
		if (_isEdit)
			codes += QString("loft.setEditID(%1)").arg(_editSet->getID());
		codes += QString("loft.setName('%1')").arg(name);
		QString soildstr;
		if (isSolid) soildstr = "Yes";
		else soildstr = "No";
		codes += QString("loft.isSolid('%1')").arg(soildstr);
		
		for (int i = 0; i < _allShapes.size(); i++)
		{
			QList<QString*> strlist{};
			QList<Geometry::GeometrySet*> sets = _allShapes[i].uniqueKeys();
			QString sec{};
			
			for (int j = 0; j < sets.size(); ++j)
			{
				QString* temp=new QString();
				auto s = sets.at(j);
				int id = s->getID();
				*temp = QString::number(id)+":";
				QList<int> indexs = _allShapes[i].values(s);
				int nmu = indexs.size();
				for (int k = 0;  k< indexs.size(); k++)
				{
					(*temp) += QString::number(indexs[k]);
					if (k < indexs.size() - 1)
					{
						(*temp).append(",");
					}
					else if (k ==(indexs.size() - 1))
					{
						(*temp).append(";");
					}
				}

				strlist << temp;
			}
			for (int  i = 0; i < strlist.size(); i++)
			{
				sec += strlist[i];
			}
			codes += QString("loft.appendSection('%1')").arg(sec);

		}
		if (_isEdit)
			codes += QString("loft.edit()");
		else
			codes += QString("loft.create()");

		_pyAgent->submit(codes);

/*
		Command::GeoCommandMakeLoft* command = new Command::GeoCommandMakeLoft(_mainWindow, _preWindow);

		command->setShapeList(_allShapes);
		command->isSolid(isSolid);
		command->setName(name);

		bool success = Command::GeoComandList::getInstance()->executeCommand(command);
		
*/
		QDialog::accept();
		this->close(); 
	}

	void CreateLoftDialog::updateTab()
	{
		QTableWidget* tab = _ui->tableWidget;

		tab->setRowCount(0);
		const int nc = _allShapes.size();
		tab->setRowCount(nc);

		for (int i = 0; i < nc; ++ i)
		{
			QTableWidgetItem* item = new QTableWidgetItem(QString("Section_%1").arg(i + 1));
			tab->setItem(i, 0, item);
			int n = _allShapes.at(i).size();
			QTableWidgetItem* item1 = new QTableWidgetItem(QString::number(n));
			tab->setItem(i, 1, item1);
		}
	}

	void CreateLoftDialog::tableClicked(int row, int col)
	{
		Q_UNUSED(col);
		for (int j = 0; j < _allShapes.size(); j++)
		{

			QList<Geometry::GeometrySet*> setlist = _allShapes[j].uniqueKeys();
			for (int i = 0; i < setlist.size(); i++)
			{
				QList<int> indexlist = _tempShapes.values(setlist[i]);
				for (int index : indexlist)
				{
					emit highLightGeometryEdgeSig(setlist[i], index, false);
				}
			}
		}
		if (row >= _allShapes.size()) return;

		QMultiHash<Geometry::GeometrySet*, int> rowshape = _allShapes.at(row);
		QList<Geometry::GeometrySet*> setlist = rowshape.uniqueKeys();
		for (int i = 0; i < setlist.size(); i++)
		{
			QList<int> indexlist = _tempShapes.values(setlist[i]);
			for (int index : indexlist)
			{
				emit highLightGeometryEdgeSig(setlist[i], index, true);
			}
		}

	}

}
