#include "DialogFluidMesh.h"
#include "ui_DialogFluidMesh.h"
#include "GeometryWidgets/geoPointWidget.h"
#include "python/PyAgent.h"
#include "geometry/geometrySet.h"
#include <QMessageBox>

namespace Gmsh{
	DialogFluidMesh::DialogFluidMesh(GUI::MainWindow* m, MainWidget::PreWindow* pre, int index )
		: GmshDialogBase(m, pre)
	{
		_ui = new Ui::DialogFluidMesh;
		_ui->setupUi(this);
		_ui->label_4->setVisible(false);
		_ui->MinSizeDoubleSpinBox->setVisible(false);

		_minPoint = new GeometryWidget::GeoPointWidget(m, pre);
		_maxPoint = new GeometryWidget::GeoPointWidget(m, pre);
		_ui->startPointLayout->addWidget(_minPoint);
		_ui->endPointLayout->addWidget(_maxPoint);
		connect(_minPoint, &GeometryWidget::GeoPointWidget::buttonCkicked, this, &DialogFluidMesh::pointWidgetClicked);
		connect(_maxPoint, &GeometryWidget::GeoPointWidget::buttonCkicked, this, &DialogFluidMesh::pointWidgetClicked);
		//connect(_minPoint, SIGNAL(buttonCkicked(GeometryWidget::GeoPointWidget*)), this, SLOT(pointWidgetClicked(GeometryWidget::GeoPointWidget*)));
//		connect(_minPoint, SIGNAL(buttonCkicked(GeometryWidget::GeoPointWidget*)), this, SLOT(pointWidgetClicked(GeometryWidget::GeoPointWidget*)));
	//	connect(_maxPoint, SIGNAL(buttonCkicked(GeometryWidget::GeoPointWidget*)), this, SLOT(pointWidgetClicked(GeometryWidget::GeoPointWidget*)));
	}

	DialogFluidMesh::~DialogFluidMesh()
	{
		delete _ui;
	}

	void DialogFluidMesh::on_geoSelectSurface_clicked()
	{
		emit setSelectMode((int)ModuleBase::GeometryBody);
		_minPoint->handleProcess(false);
		_maxPoint->handleProcess(false);
		_selectBody = true;
	}

	void DialogFluidMesh::shapeSlected(Geometry::GeometrySet* set, int index)
	{
		if (!_selectBody) return;
		if (_solidHash.contains(set, index))
		{
			_solidHash.remove(set, index);
			emit highLightGeometrySolidSig(set, index, false);
		}
		else
		{
			_solidHash.insert(set, index);
			emit highLightGeometrySolidSig(set, index, true);

		}
		QString text = QString(tr("Selected Solid(%1)").arg(_solidHash.size()));
		_ui->planelabel->setText(text);
	}

	void DialogFluidMesh::pointWidgetClicked(GeometryWidget::GeoPointWidget* pw)
	{
		_minPoint->handleProcess(false);
		_maxPoint->handleProcess(false);
		_selectBody = false;

		pw->handleProcess(true);
	}

	void DialogFluidMesh::closeEvent(QCloseEvent *e)
	{
		QDialog::closeEvent(e);
		delete this;
	}

	void DialogFluidMesh::accept()
	{
		if (_solidHash.isEmpty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("No object has been selected !"));
			return;
		}
		if (!fluidFieldIsSrue())
		{
			QMessageBox::warning(this, tr("Warning"), tr("There is a problem with fluid region parameters !"));
			return;
		}

		_pyAgent->submit(QString("gmsher = Mesher.Gmsher()"));

		for (QMultiHash<Geometry::GeometrySet*, int>::iterator iter = _solidHash.begin(); iter != _solidHash.end(); ++iter)		{			auto set = iter.key();			int id = set->getID();			int index = iter.value();			QString code = QString("gmsher.appendSolid(%1,%2)").arg(id).arg(index);
			_pyAgent->submit(code);		}

		double startp[3] = { 0 };
		_minPoint->getCoordinate(startp);
		_pyAgent->submit(QString("gmsher.setFluidField(%1,%2,%3)").arg(startp[0]).arg(startp[1]).arg(startp[2]));
		double endp[3] = { 0 };
		_maxPoint->getCoordinate(endp);
		_pyAgent->submit(QString("gmsher.setFluidField(%1,%2,%3)").arg(endp[0]).arg(endp[1]).arg(endp[2]));

		QString eleType("Tet");
		if (_ui->HexRadioButton->isChecked()) eleType = "Hex";     //六面体 
		_pyAgent->submit(QString("gmsher.setElementType(\"%1\")").arg(eleType));

		const int orderindex = _ui->OrderComboBox->currentIndex();
		_pyAgent->submit(QString("gmsher.setElementOrder(%1)").arg(orderindex + 1)); //低阶高阶单元

		const int methodIndex = _ui->MethodComboBox->currentIndex();
		int method = 1;
		switch (methodIndex)
		{
		case 1: method = 2; break;
		case 2: method = 4; break;
		case 3: method = 5; break;
		case 4: method = 6; break;
		case 5: method = 7; break;
		case 6: method = 9; break;
		default: break;
		}

		_pyAgent->submit(QString("gmsher.setMethod(%1)").arg(method));

		const double elesize = _ui->MinSizeDoubleSpinBox->value();
		_pyAgent->submit(QString("gmsher.setFluidSize(%1)").arg(elesize));

		_pyAgent->submit(QString("gmsher.startGenerationFluidMesh()"));


		QDialog::accept();
		this->close();
	}

	void DialogFluidMesh::reject()
	{
		QDialog::reject();
		this->close();
	}

	bool DialogFluidMesh::fluidFieldIsSrue()
	{
		double s[3] = { 0 };
		double e[3] = { 0 };
		_minPoint->getCoordinate(s);
		_maxPoint->getCoordinate(e);

		for (int i = 0; i < 3;i++)
		{
			if (s[i] == e[i])
				return false;
		}
		
		return true;
	}

}