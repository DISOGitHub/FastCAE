#include "dialogVariableFillet.h"
#include "ui_dialogVariableFillet.h"
#include "GeometryCommand/GeoCommandCreateVariableFillet.h"
#include "GeometryCommand/GeoCommandList.h"
#include "moduleBase/ModuleType.h"
#include "MainWidgets/preWindow.h"
#include "geometry/geometryParaVariableFillet.h"
#include "settings/busAPI.h"
#include "settings/GraphOption.h"
#include <QMessageBox>
#include <QColor>
#include "geometry/geometrySet.h"
#include <QTableWidgetItem>
#include <vtkActor.h>
#include "python/PyAgent.h"
#include <vtkProperty.h>

namespace GeometryWidget
{
	VariableFilletDialog::VariableFilletDialog(GUI::MainWindow* m, MainWidget::PreWindow* p)
		: GeoDialogBase(m,p)
	{
		_ui = new Ui::VariableFilletDialog;
		_ui->setupUi(this);
		_ui->widgetAdd->setVisible(false);
		_ui->tableLR->setEditTriggers(QAbstractItemView::NoEditTriggers);
		this->translateButtonBox(_ui->buttonBox);
	}

	VariableFilletDialog::VariableFilletDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set)
		:GeoDialogBase(m,p)
	{
		_ui = new Ui::VariableFilletDialog;
		_ui->setupUi(this);
		_ui->widgetAdd->setVisible(false);
		_ui->tableLR->setEditTriggers(QAbstractItemView::NoEditTriggers);

		_isEdit = true;
		_editSet = set;
		this->setWindowTitle("Edit VariableFillet");
		this->translateButtonBox(_ui->buttonBox);
		init();
	}

	VariableFilletDialog::~VariableFilletDialog()
	{
		if (_ui != nullptr) delete _ui;
		emit updateGraphOptions();
	}
	void VariableFilletDialog::init()
	{
		if (_editSet == nullptr) return;
		auto subset = _editSet->getSubSetAt(0);

		emit hideGeometry(_editSet);
		emit showGeometry(subset);

		Geometry::GeometryModelParaBase* bp =_editSet->getParameter();
		Geometry::GeometryParaVariableFillet* p = dynamic_cast<Geometry::GeometryParaVariableFillet*>(bp);
		if (p == nullptr) return;
		Geometry::GeometrySet*edgeset = p->getEdgeSet();
		if (edgeset == nullptr) return;
		_edgeSet = edgeset;
		if (_editSet == nullptr) return;
		
		int edgeindex = p->getEdgeIndex();
		_edgeIndex = edgeindex;

		emit highLightGeometryEdge(subset, edgeindex, &_actors);

		double basicradius = p->getBasicRadius();
		_ui->lineEditRadius->setText(QString::number(basicradius));
		QMap<double, double> radiuMap = p->getRadiuMap();
		_radiusMap = radiuMap;
		if (_edgeIndex >= 0)
		{
			QString label = QString(tr("Selected edge(1)"));
			_ui->edgelabel->setText(label);
		}
		updateTab();

	}
	void VariableFilletDialog::closeEvent(QCloseEvent *e)
	{
		QDialog::closeEvent(e);
		delete this;
	}
	
	void VariableFilletDialog::reject()
	{
		if (_isEdit)
		{
			if (_editSet == nullptr) return;
			Geometry::GeometryModelParaBase* pb = _editSet->getParameter();
			Geometry::GeometryParaVariableFillet* p = dynamic_cast<Geometry::GeometryParaVariableFillet*>(pb);
			if (p == nullptr) return;
			Geometry::GeometrySet*  originalSet = p->getGeometrySet();
			if (originalSet == nullptr) return;
			emit hideGeometry(originalSet);
			emit showGeometry(_editSet);
		}

		QDialog::reject();
		this->close();
	}

	void VariableFilletDialog::accept()
	{
		bool ok = true;
		if (_edgeSet == nullptr) ok = false;
		QString text = _ui->lineEditRadius->text();
		double basicRad = 0.0;
		if (ok)
			basicRad = text.toDouble(&ok);
		if (_radiusMap.size() < 1) ok = false;
		if (!ok)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Input Wrong !"));
			return;
		}
		
		QStringList codes{};
		codes += QString("variablefillet = CAD.VariableFillet()");

		if (_isEdit)
			codes += QString("variablefillet.setEditID(%1)").arg(_editSet->getID());

		codes += QString("variablefillet.VariableFilletOnEdge(%1,%2)").arg(_edgeSet->getID()).arg(_edgeIndex);
		codes += QString("variablefillet.setBasicRad(%1)").arg(basicRad);
		QList<double> loclist = _radiusMap.keys();
		QList<double> radlist = _radiusMap.values();
		for (int i = 0; i < _radiusMap.size(); ++i)
		{
			codes += QString("variablefillet.AppendVariablePoint(%1,%2)").arg(loclist[i]).arg(radlist[i]);
		}
		if (_isEdit)
			codes += QString("variablefillet.edit()");
		else
			codes += QString("variablefillet.create()");

		_pyAgent->submit(codes);

		
		/*Command::CommandCreateVariableFillet* c = new Command::CommandCreateVariableFillet(_mainWindow, _preWindow);
		c->setShape(_edgeSet, _edgeIndex);
		c->setBasicRadius(basicRad);
		c->setRaiudMap(_radiusMap);

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

	void VariableFilletDialog::selectActorShape(vtkActor* ac, int index, Geometry::GeometrySet* set)
	{
		QColor color;
		int count = 0;

		if (_edgeSet != nullptr)
		{
			color = Setting::BusAPI::instance()->getGraphOption()->getGeometryCurveColor();
			_edgeSet = nullptr;
			_edgeIndex = -1;
			count = 0;
			_actors[0]->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
			
		}
		
		color = Setting::BusAPI::instance()->getGraphOption()->getHighLightColor();
		_edgeSet = set;
		_edgeIndex = index;
		count = 1;
		_actors.insert(0, ac);
		ac->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
		
		QString label = QString(tr("Selected edge(%1)")).arg(count);
		_ui->edgelabel->setText(label);
	}

	void VariableFilletDialog::on_geoSelectCurve_clicked()
	{
		emit setSelectMode((int)ModuleBase::GeometryCurve);
		if (_isEdit)
		{
			if ((_actors.size() > 0) && (_actors[0] != nullptr))
			{
				for(vtkActor* var : _actors)
				{
					QColor color = Setting::BusAPI::instance()->getGraphOption()->getHighLightColor();
					var->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());

				}
			}
			emit _preWindow->reRenderSig();
		}
	}

	void VariableFilletDialog::on_buttonAdd_clicked()
	{
		bool v = _ui->widgetAdd->isVisible();
		_ui->widgetAdd->setVisible(!v);
	}

	void VariableFilletDialog::on_addLR_clicked()
	{
		bool ok = false;
		QString text;
		text = _ui->lineEditLocation->text();
		double u = text.toDouble(&ok);
		if (u<0 || u>1.0) ok = false;
		double r = 0;
		if (ok)
		{
			text = _ui->lineEditRadiusNew->text();
			r = text.toDouble(&ok);
		}
		if (!ok)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Input Wrong !"));
			return;
		}

		_radiusMap.insert(u, r);
		this->updateTab();
	}

	void VariableFilletDialog::updateTab()
	{
		_ui->tableLR->setRowCount(0);
		QList<double> ulist = _radiusMap.keys();
		const int n = ulist.size();
		_ui->tableLR->setRowCount(n);

		for (int i = 0; i < n; ++i)
		{
			double l = ulist.at(i);
			double r = _radiusMap.value(l);
			QTableWidgetItem* litem = new QTableWidgetItem(QString::number(l));
			_ui->tableLR->setItem(i, 0, litem);
			QTableWidgetItem* ritem = new QTableWidgetItem(QString::number(r));
			_ui->tableLR->setItem(i, 1, ritem);
		}
	}



}