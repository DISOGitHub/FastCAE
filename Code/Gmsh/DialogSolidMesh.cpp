#include "DialogSolidMesh.h"
#include "ui_DialogSolidMesh.h"
#include "python/PyAgent.h"
#include "geometry/geometrySet.h"
#include "DialogLocalSetting.h"
#include <QMessageBox>

namespace Gmsh
{
	SolidMeshDialog::SolidMeshDialog(GUI::MainWindow* m, MainWidget::PreWindow* pre)
		: GmshDialogBase(m, pre)
	{
		_ui = new Ui::SolidMeshDialog;
		_ui->setupUi(this);
	}

	SolidMeshDialog::~SolidMeshDialog()
	{
		if (_ui != nullptr) delete _ui;
//		emit setSelectMode((int)ModuleBase::None);
		emit updateGraphOptions();
	}

	void SolidMeshDialog::closeEvent(QCloseEvent *e)
	{
		QDialog::closeEvent(e);
		delete this;
	}

	void SolidMeshDialog::reject()
	{
		QDialog::reject();
		this->close();
	}

	void SolidMeshDialog::on_geoSelectSurface_clicked()
	{
		emit setSelectMode((int)ModuleBase::GeometrySurface);
		_selectBody = true;
	}

	void SolidMeshDialog::selectActorShape(vtkActor*, int, Geometry::GeometrySet* set)
	{
		if (!_selectBody) return;
		if (_geosetList.contains(set))
		{
			_geosetList.removeOne(set);
			emit highLightGeometrySet(set, false);
		}
		else
		{
			_geosetList.append(set);
			emit highLightGeometrySet(set, true);

		}
		QString text = QString(tr("Selected solid(%1)").arg(_geosetList.size()));
		_ui->planelabel->setText(text);
	}

	void SolidMeshDialog::on_localButton_clicked()
	{
		emit setSelectMode((int)ModuleBase::None);
		_selectBody = false;
		auto d = new LocalSettingDialog(this,_mainWindow,  _preWindow);
		emit showDialog(d);
	}

	void SolidMeshDialog::accept()
	{
		if (_geosetList.isEmpty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("No object has been selected !"));
			return;
		}
		
		_pyAgent->submit(QString("gmsher = Mesher.Gmsher()"));

		for (auto set : _geosetList)
		{
			int id = set->getID();
			QString code = QString("gmsher.appendSolid(%1)").arg(id);
			_pyAgent->submit(code);
		}
//		_pyAgent->submit("Mesher.initGmsh()");

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

		const double factor = _ui->SizeFacDoubleSpinBox->value();
		const double minsize = _ui->MinSizeDoubleSpinBox->value();
		const double maxsize = _ui->MaxSizeDoubleSpinBox->value();
		_pyAgent->submit(QString("gmsher.setSizeFactor(%1)").arg(factor));
		_pyAgent->submit(QString("gmsher.setMinSize(%1)").arg(minsize));
		_pyAgent->submit(QString("gmsher.setMaxSize(%1)").arg(maxsize));

		if (_ui->geoCleanCheckBox->isChecked()) //geoclean = 1;
			_pyAgent->submit(QString("gmsher.cleanGeo()"));

		_pyAgent->submit(QString("gmsher.setGridCoplanar(%1)").arg(_ui->gridCoplanarCheckBox->isChecked()));
		this->appendPointSizeFiled();
		_pyAgent->submit("gmsher.startGenerationThread()");

		QDialog::accept();
		this->close();
	}



}