#include "dialogMoveFeature.h"
#include "ui_dialogMoveFeature.h"
#include "geoPointWidget.h"
#include "GeometryCommand/GeoCommandMoveFeature.h"
#include "GeometryCommand/GeoCommandList.h"
#include "geometry/geometryParaMakeMove.h"
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include <QMessageBox>
#include <math.h>
#include <QDebug>
#include "python/PyAgent.h"
namespace GeometryWidget
{
	MoveFeatureDialog::MoveFeatureDialog(GUI::MainWindow* m, MainWidget::PreWindow* p) :
		GeoDialogBase(m, p)
	{
		_ui = new Ui::MoveFeatureDialog;
		_ui->setupUi(this);
		init();
		connect(_ui->comboBoxOption, SIGNAL(currentIndexChanged(int)), this, SLOT(on_TypeChanged(int)));
		connect(_ui->radioButtonUser, SIGNAL(toggled(bool)), this, SLOT(on_radioButtonUser()));
		on_radioButtonUser();
	}

	MoveFeatureDialog::MoveFeatureDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set):
		GeoDialogBase(m,p)
	{
		_ui = new Ui::MoveFeatureDialog;
		_ui->setupUi(this);
		_isEdit = true;
		_editSet = set;
		init();
		connect(_ui->comboBoxOption, SIGNAL(currentIndexChanged(int)), this, SLOT(on_TypeChanged(int)));
		connect(_ui->radioButtonUser, SIGNAL(toggled(bool)), this, SLOT(on_radioButtonUser()));
		on_radioButtonUser();
	}

	MoveFeatureDialog::~MoveFeatureDialog()
	{
		if (_ui != nullptr) delete _ui;
	}

	void MoveFeatureDialog::closeEvent(QCloseEvent *e)
	{
		QDialog::closeEvent(e);
		delete this;
	}

	void MoveFeatureDialog::reject()
	{

		if (_isEdit)
		{
			
			if (_editSet == nullptr) return;
			Geometry::GeometryModelParaBase* pb = _editSet->getParameter();
			Geometry::GeometryParaMakeMove* p = dynamic_cast<Geometry::GeometryParaMakeMove*>(pb);
			if (p == nullptr) return;
			Geometry::GeometrySet*  originalSet = p->getOriSet();
			if (originalSet == nullptr) return;
			emit showGeometry(_editSet);
			emit hideGeometry(originalSet);
				
		}
		QDialog::reject();
		this->close();
	}

	void MoveFeatureDialog::init()
	{
		this->translateButtonBox(_ui->buttonBox);
		_ui->tabWidget->tabBar()->hide();
		_baseWidget = new GeoPointWidget(_mainWindow, _preWindow);
		_endWidget = new GeoPointWidget(_mainWindow, _preWindow);
		_ui->baseLayout->addWidget(_baseWidget);
		_ui->endLayout->addWidget(_endWidget);
		connect(_baseWidget, SIGNAL(buttonCkicked(GeoPointWidget*)), this, SLOT(pointWidgetClicked(GeoPointWidget*)));
		connect(_endWidget, SIGNAL(buttonCkicked(GeoPointWidget*)), this, SLOT(pointWidgetClicked(GeoPointWidget*)));
		if (_isEdit)
		{
			_ui->geoSelectSurface->setEnabled(false);
			if (_editSet == nullptr) return;
			Geometry::GeometryModelParaBase* bp = _editSet->getParameter();
			Geometry::GeometryParaMakeMove* p = dynamic_cast<Geometry::GeometryParaMakeMove*>(bp);
			if (p == nullptr) return;
			auto subset = p->getOriSet();
			if (subset == nullptr) return;
			emit hideGeometry(_editSet);
			emit showGeometry(subset);

			_bodysHash = p->getBodys();
			if (_bodysHash.size() < 1) return;
			QMultiHash<Geometry::GeometrySet*, int>::iterator iter = _bodysHash.begin();
			for (; iter != _bodysHash.end(); ++iter)
			{
				emit highLightGeometrySolidSig(iter.key(), iter.value(), true);
			}
			QString text = QString(tr("Selected body(%1)")).arg(_bodysHash.size());
			_ui->edgelabel->setText(text);

			double basept[3]{0.0}, endpt[3]{0.0};
			p->getBasePoint(basept);
			p->getsetEndPoint(endpt);
			int index = p->getOptionIndex();
			_ui->comboBoxOption->setCurrentIndex(index);
			_ui->tabWidget->setCurrentIndex(index);
			_ui->checkBoxSaveOrigin->setChecked(p->getSaveOrigion());
			
			if (index==0)
			{
				_baseWidget->setCoordinate(basept);
				_endWidget->setCoordinate(endpt);
			}
			else
			{
				_ui->lineEditLength->setText(QString::number(p->getLength()));
				double length{}, dir[3]{}; bool reverse{ false };
				length = p->getLength();
				p->getDirection(dir);
				reverse = p->getReverse();
				_ui->lineEditLength->setText(QString::number(length));
				_ui->checkBoxReverse->setChecked(reverse);

				if (dir[0] != 0 && dir[1] == 0 && dir[2] == 0)
				{
					_ui->radioButtonX->setChecked(true);
				}
				else if (dir[0] == 0 && dir[1] != 0 && dir[2] == 0)
				{
					_ui->radioButtonY->setChecked(true);
				}
				else if (dir[0] == 0 && dir[1] == 0 && dir[2] != 0)
				{
					_ui->radioButtonZ->setChecked(true);
				}
				else
				{
					_ui->radioButtonUser->setChecked(true);
					_ui->doubleSpinBoxX->setVisible(true);
					_ui->doubleSpinBoxY->setVisible(true);
					_ui->doubleSpinBoxZ->setVisible(true);
					_ui->doubleSpinBoxX->setValue(dir[0]);
					_ui->doubleSpinBoxY->setValue(dir[1]);
					_ui->doubleSpinBoxZ->setValue(dir[2]);
				}
			}

		}
	}

	void MoveFeatureDialog::accept()
	{
		double vec[3] = { 0.0 };
		bool ok = getVector(vec);

		double startPoint[3] = { 0.0 };
		double endPoint[3] = { 0.0 };
		ok = _baseWidget->getCoordinate(startPoint);
		ok = _endWidget->getCoordinate(endPoint);
		bool reverse = _ui->checkBoxReverse->isChecked();
		QString text = _ui->lineEditLength->text();
		double length = text.toDouble(&ok);

		if (!ok || _bodysHash.size() < 1)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Input Wrong !"));
			return;
		}
		bool s = _ui->checkBoxSaveOrigin->isChecked();
/*

		auto c = new Command::CommandMoveFeature(_mainWindow, _preWindow);
		c->setBodys(_geobodyList);
		c->setVector(vec);
		c->setSaveOrigin(s);

		c->setOptionIndex(_optionindex);
		c->setStartPt(startPoint);
		c->setEndPt(endPoint);
		c->setReverse(reverse);
		c->setLength(length);
		c->setDir(_dir);

		if (_isEdit) c->setEditData(_editSet);
		bool success = Command::GeoComandList::getInstance()->executeCommand(c);

		if (!success)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Create failed ! "));
			return;
		}*/
		QStringList codes{};
		codes += QString("movefeature = CAD.MoveFeature()");

		if (_isEdit)
			codes += QString("movefeature.setEditID(%1)").arg(_editSet->getID());
		QMultiHash<Geometry::GeometrySet*, int>::iterator it = _bodysHash.begin();
		for (; it != _bodysHash.end(); it++)
		{
			codes += QString("movefeature.appendBody(%1,%2)").arg(it.key()->getID()).arg(it.value());
		}
		_optionindex = _ui->comboBoxOption->currentIndex(); 
		QString method{};
		if (_optionindex == 0) method = "Two Points";
		else method = "Distance";
		codes += QString("movefeature.TransformMethod('%1')").arg(method);
		
		if (_optionindex == 0)
		{
			codes += QString("movefeature.setStartPoint(%1,%2,%3)").arg(startPoint[0]).arg(startPoint[1]).arg(startPoint[2]);
			codes += QString("movefeature.setEndPoint(%1,%2,%3)").arg(endPoint[0]).arg(endPoint[1]).arg(endPoint[2]);
		}
		else
		{
			codes += QString("movefeature.setLength(%1)").arg(length);
			codes += QString("movefeature.setDirection(%1,%2,%3)").arg(_dir[0]).arg(_dir[1]).arg(_dir[2]);
			QString revesestr{};
			if (reverse) revesestr = "Yes";
			else revesestr = "No";

			codes += QString("movefeature.setReverse('%1')").arg(revesestr);
		}
		QString saveori{};
		if (s) saveori = "Yes";
		else saveori = "No";
		codes += QString("movefeature.SaveOrigin('%1')").arg(saveori);
		if (_isEdit)
			codes += QString("movefeature.edit()");
		else
			codes += QString("movefeature.create()");
		_pyAgent->submit(codes);


		QDialog::accept();
		this->close();
	}

	void MoveFeatureDialog::on_TypeChanged(int index)
	{
		_ui->tabWidget->setCurrentIndex(index);
		_optionindex = index;
		
	}

	void MoveFeatureDialog::on_geoSelectSurface_clicked()
	{
		emit setSelectMode((int)ModuleBase::GeometryBody);
		_baseWidget->handleProcess(false);
		_endWidget->handleProcess(false);
		_selectBody = true;
		
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

	void MoveFeatureDialog::shapeSlected(Geometry::GeometrySet* set, int index)
	{

		if (!_selectBody) return;

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
		_ui->edgelabel->setText(text);

	}

	void MoveFeatureDialog::pointWidgetClicked(GeoPointWidget* w)
	{
		_selectBody = false;
		_baseWidget->handleProcess(false);
		_endWidget->handleProcess(false);
		w->handleProcess(true);


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
	void MoveFeatureDialog::on_radioButtonUser()
	{
		bool checked = _ui->radioButtonUser->isChecked();
		_ui->doubleSpinBoxX->setVisible(checked);
		_ui->doubleSpinBoxY->setVisible(checked);
		_ui->doubleSpinBoxZ->setVisible(checked);
	}

	bool MoveFeatureDialog::getVector(double* vec)
	{
		bool ok = false;
		
		if (_ui->comboBoxOption->currentIndex() == 0)
		{
			double start[3] = { 0.0 };
			double end[3] = { 0.0 };
			ok = _baseWidget->getCoordinate(start);
			if (ok) 
				ok = _endWidget->getCoordinate(end);
			if (ok)
			{
				vec[0] = end[0] - start[0];
				vec[1] = end[1] - start[1];
				vec[2] = end[2] - start[2];
			}
		}
		else
		{
			QString text = _ui->lineEditLength->text();
			double length = text.toDouble(&ok);
			if (length < 1e-6) return false;

			double dir[3] = { 0.0 };
			if (_ui->radioButtonX->isChecked())
				dir[0] = 1.0;
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
				_dir[i] = dir[i];
			}
			if (_ui->checkBoxReverse->isChecked())
			{
				for (int i = 0; i < 3; ++i)
					dir[i] *= -1;
			}
			double mod = sqrt(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);
			double fac = length / mod;
			if (ok)
				for (int i = 0; i < 3; ++i)
					vec[i] = fac * dir[i];

		}
		return ok;
	}

}