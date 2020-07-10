#include "dialogMakeMatrix.h"
#include "ui_dialogMakeMatrix.h"
#include "geoPointWidget.h"
#include "GeometryCommand/GeoCommandMakeMatrix.h"
#include "GeometryCommand/GeoCommandList.h"
#include <QMessageBox>
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include "geometry/geometryParaMakeMatrix.h"
#include "python/PyAgent.h"
namespace GeometryWidget
{
	MakeMatrixDialog::MakeMatrixDialog(GUI::MainWindow* m, MainWidget::PreWindow* p) :
		GeoDialogBase(m, p)
	{
		_ui = new Ui::MakeMatrixDialog;
		_ui->setupUi(this);
		init();
		connect(_ui->comboBoxOption, SIGNAL(currentIndexChanged(int)), this, SLOT(on_TypeChanged(int)));
		connect(_ui->radioButtonUser_1, SIGNAL(toggled(bool )), this, SLOT(on_radioButtonUser()));
		connect(_ui->radioButtonUser_2, SIGNAL(toggled(bool)), this, SLOT(on_radio2ButtonUser()));
		connect(_ui->radioButtonUser_3, SIGNAL(toggled(bool)), this, SLOT(on_radio3ButtonUser()));
		connect(_ui->dir2checkBox, SIGNAL(toggled(bool)), this, SLOT(showLinearDir2InfoChbox()));
		on_radioButtonUser();
		on_radio2ButtonUser();
		on_radio3ButtonUser();
		showLinearDir2InfoChbox();
	}

	MakeMatrixDialog::MakeMatrixDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set):
		GeoDialogBase(m,p)
	{
		_ui = new Ui::MakeMatrixDialog;
		_ui->setupUi(this);
		_isEdit = true;
		_editSet = set;
		init();
		connect(_ui->comboBoxOption, SIGNAL(currentIndexChanged(int)), this, SLOT(on_TypeChanged(int)));
		connect(_ui->radioButtonUser_1, SIGNAL(toggled(bool)), this, SLOT(on_radioButtonUser()));
		connect(_ui->radioButtonUser_2, SIGNAL(toggled(bool)), this, SLOT(on_radio2ButtonUser()));
		connect(_ui->radioButtonUser_3, SIGNAL(toggled(bool)), this, SLOT(on_radio3ButtonUser()));
		connect(_ui->dir2checkBox, SIGNAL(toggled(bool)), this, SLOT(showLinearDir2InfoChbox()));
		on_radioButtonUser();
		on_radio2ButtonUser();
		on_radio3ButtonUser();
		showLinearDir2InfoChbox();
	}

	MakeMatrixDialog::~MakeMatrixDialog()
	{
		if (_ui != nullptr) delete _ui;

	}
	void MakeMatrixDialog::init()
	{
		if (_isEdit)
		{
			_ui->geoSelectSurface->setEnabled(false);
		}
		_ui->tabWidget->tabBar()->hide();
		_baseWidget = new GeoPointWidget(_mainWindow, _preWindow);
		this->translateButtonBox(_ui->buttonBox);
		_ui->verticalLayout->addWidget(_baseWidget);
		connect(_baseWidget, SIGNAL(buttonCkicked(GeoPointWidget*)), this, SLOT(pointWidgetClicked(GeoPointWidget*)));

		if (_isEdit==true)
		{
			if (_editSet == nullptr) return;
			Geometry::GeometryModelParaBase* bp = _editSet->getParameter();
			Geometry::GeometryParaMatrix* p = dynamic_cast<Geometry::GeometryParaMatrix*>(bp);
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
			int optionindex = p->getCurrentIndex();
			_ui->comboBoxOption->setCurrentIndex(optionindex);
			if (optionindex == 0)
			{
				double dir[3]{0.0};
				p->getDirection1(dir);
				if (p->getReverse1()==true)
				{
					for (int i = 0; i < 3; i++)
					{
						for (int i = 0; i < 3; ++i)
							dir[i] *= -1;
					}
				}
				if (dir[0] != 0 && dir[1] == 0 && dir[2] == 0)
				{
					_ui->radioButtonX_1->setChecked(true);
				}
				else if (dir[0] == 0 && dir[1] != 0 && dir[2] == 0)
				{
					_ui->radioButtonY_1->setChecked(true);
				}
				else if (dir[0] == 0 && dir[1] == 0 && dir[2] != 0)
				{
					_ui->radioButtonZ_1->setChecked(true);
				}
				else
				{
					_ui->radioButtonUser_1->setChecked(true);
					_ui->doubleSpinBoxX_1->setVisible(true);
					_ui->doubleSpinBoxY_1->setVisible(true);
					_ui->doubleSpinBoxZ_1->setVisible(true);
					_ui->doubleSpinBoxX_1->setValue(dir[0]);
					_ui->doubleSpinBoxY_1->setValue(dir[1]);
					_ui->doubleSpinBoxZ_1->setValue(dir[2]);
				}
				_ui->checkBoxDir1Reverse->setChecked(p->getReverse1());
				_ui->dir1lineEdit->setText(QString::number(p->getDistance1()));
				_ui->sum1spinBox->setValue(p->getCount1());
				if (p->showtDir2() == true)
				{
					_ui->dir2checkBox->setChecked(true);
					_ui->groupBox_4->setEnabled(true);
					double dir[3]{0.0};
					p->getDirection2(dir);
					if (p->getReverse2() == true)
					{
						for (int i = 0; i < 3; i++)
						{
							for (int i = 0; i < 3; ++i)
								dir[i] *= -1;
						}
					}
					if (dir[0] != 0 && dir[1] == 0 && dir[2] == 0)
					{
						_ui->radioButtonX_2->setChecked(true);
					}
					else if (dir[0] == 0 && dir[1] != 0 && dir[2] == 0)
					{
						_ui->radioButtonY_2->setChecked(true);
					}
					else if (dir[0] == 0 && dir[1] == 0 && dir[2] != 0)
					{
						_ui->radioButtonZ_2->setChecked(true);
					}
					else
					{
						_ui->radioButtonUser_2->setChecked(true);
						_ui->doubleSpinBoxX_2->setVisible(true);
						_ui->doubleSpinBoxY_2->setVisible(true);
						_ui->doubleSpinBoxZ_2->setVisible(true);
						_ui->doubleSpinBoxX_2->setValue(dir[0]);
						_ui->doubleSpinBoxY_2->setValue(dir[1]);
						_ui->doubleSpinBoxZ_2->setValue(dir[2]);
					}
					_ui->checkBoxDir1Reverse_2->setChecked(p->getReverse2());
					_ui->dir2lineEdit->setText(QString::number(p->getDistance2()));
					_ui->sum2spinBox->setValue(p->getCount2());
				}
			}
			else
			{
				_ui->tabWidget->setCurrentIndex(optionindex);
				double axis[3]{0.0}, dir[3]{};
				p->getAxis(dir);
				_baseWidget->setCoordinate(dir);
				p->getAxisDir(dir);
				
				if (p->getAxisRev() == true)
				{
					for (int i = 0; i < 3; i++)
					{
						for (int i = 0; i < 3; ++i)
							dir[i] *= -1;
					}
				}
				if (dir[0] != 0 && dir[1] == 0 && dir[2] == 0)
				{
					_ui->radioButtonX_3->setChecked(true);
				}
				else if (dir[0] == 0 && dir[1] != 0 && dir[2] == 0)
				{
					_ui->radioButtonY_3->setChecked(true);
				}
				else if (dir[0] == 0 && dir[1] == 0 && dir[2] != 0)
				{
					_ui->radioButtonZ_3->setChecked(true);
				}
				else
				{
					_ui->radioButtonUser_3->setChecked(true);
					_ui->doubleSpinBoxX_3->setVisible(true);
					_ui->doubleSpinBoxY_3->setVisible(true);
					_ui->doubleSpinBoxZ_3->setVisible(true);
					_ui->doubleSpinBoxX_3->setValue(dir[0]);
					_ui->doubleSpinBoxY_3->setValue(dir[1]);
					_ui->doubleSpinBoxZ_3->setValue(dir[2]);
				}
				_ui->checkBoxDir1Reverse_3->setChecked(p->getReverse2());
				double axiscount = p->getAxisCount();
				_ui->sum1spinBox_2->setValue(axiscount);
				_ui->axisAnglelineEdit->setText(QString::number(p->getAngle()));
			}

		}
	}

	
	void MakeMatrixDialog::closeEvent(QCloseEvent *e)
	{
		QDialog::closeEvent(e);
		delete this;
	}

	void MakeMatrixDialog::reject()
	{
		if (_isEdit)
		{
			if (_editSet == nullptr) return;
			Geometry::GeometryModelParaBase* pb = _editSet->getParameter();
			Geometry::GeometryParaMatrix* p = dynamic_cast<Geometry::GeometryParaMatrix*>(pb);
			if (p == nullptr) return;
			Geometry::GeometrySet*  originalSet = p->getOriSet();
			if (originalSet == nullptr) return;
			emit showGeometry(_editSet);
		}

		QDialog::reject();
		this->close();
	}

	void MakeMatrixDialog::accept()
	{
		bool ok = false;
		if (_ui->tabWidget->currentIndex() == 0)
		{
			_selectLinear = true;
			_selectWire = false;
		}
		else
		{
			_selectLinear = false;
			_selectWire = true;
		}

		QStringList codes{};
		codes += QString("makematrix = CAD.MakeMatrix()");

		if (_isEdit)
			codes += QString("makematrix.setEditID(%1)").arg(_editSet->getID());
		QMultiHash<Geometry::GeometrySet*, int>::iterator it = _bodysHash.begin();
		for (; it != _bodysHash.end(); it++)
		{
			codes += QString("makematrix.appendBody(%1,%2)").arg(it.key()->getID()).arg(it.value());
		}

		if (_selectLinear)
			codes += QString("makematrix.setOptionMethod('%1')").arg("Liear Matrix");
		else
			codes += QString("makematrix.setOptionMethod('%1')").arg("Wire Matrix");
	
		if (_selectLinear)
		{
			double dir1[3] = { 0.0 };
			getDir1(dir1);
			//distance1
			QString text = _ui->dir1lineEdit->text();
			const double dir1Distance = text.toDouble(&ok);
			if (dir1Distance < 1e-6) ok = false;
			//sum1
			QString sumStr = _ui->sum1spinBox->text();
			const int dir1Count = sumStr.toInt(&ok);

			if (dir1Count < 1) ok = false;
			bool reverse1 = _ui->checkBoxDir1Reverse->isChecked();
			codes += QString("makematrix.setDirection1(%1,%2,%3)").arg(dir1[0]).arg(dir1[1]).arg(dir1[2]);
			QString revstr{};
			if (reverse1) revstr = "Yes";
			else revstr = "No";
			codes += QString("makematrix.setReverseOfDirection1('%1')").arg(revstr);
			codes += QString("makematrix.setDistance1(%1)").arg(dir1Distance);
			codes += QString("makematrix.setCount1(%1)").arg(dir1Count);

			//Isselect direction2
			QString showdir2str{};
			if (_ui->dir2checkBox->isChecked()) showdir2str = "Yes";
			else showdir2str = "No";
			codes += QString("makematrix.showDirection2('%1')").arg(showdir2str);

			if (_ui->dir2checkBox->isChecked())
			{
				//direction2
				double dir2[3] = { 0.0 };
				getDir2(dir2);
				//distance2 
				QString text = _ui->dir2lineEdit->text();
				const double dir2Distance = text.toDouble(&ok);
				if (dir2Distance < 1e-6) ok = false;
				//sum2 
				QString sumStr = _ui->sum2spinBox->text();                                               
				const int dir2Count = sumStr.toInt(&ok);
				if (dir2Count < 1) ok = false;
				bool reverse2 = _ui->checkBoxDir1Reverse_2->isChecked();
				QString revstr2{};
				if (reverse2) revstr2 = "Yes";
				else revstr2 = "No";
				codes += QString("makematrix.setDirection2(%1,%2,%3)").arg(dir2[0]).arg(dir2[1]).arg(dir2[2]);
				codes += QString("makematrix.setReverseOfDirection2('%1')").arg(revstr2);
				codes += QString("makematrix.setDistance2(%1)").arg(dir2Distance);
				codes += QString("makematrix.setCount2(%1)").arg(dir2Count);
			}

			if (!ok || _bodysHash.size() < 1)
			{
				QMessageBox::warning(this, tr("Warning"), tr("Input Wrong !"));
				return;
			}

		}

		if (_selectWire)
		{
			double basicPoint[3] = { 0.0 };
			double vec[3] = { 0.0 };
			double deg{ 0.0 };
			QList<double*> angleList;
			bool ok = _baseWidget->getCoordinate(basicPoint);
			if (ok)
				ok = getVector(vec);
			//sum
			QString textSum = _ui->sum1spinBox_2->text();
			const int wireCount = textSum.toInt(&ok);
			//degree
			QString textDegree = _ui->axisAnglelineEdit->text();
			deg = textDegree.toDouble(&ok);
			if (ok)
				ok = fabs(deg) < 0.0000001 ? false : true;
			if (!ok || _bodysHash.size() < 1)
			{
				QMessageBox::warning(this, tr("Warning"), tr("Input Wrong !"));
				return;
			}

			codes += QString("makematrix.setBasicPoint(%1,%2,%3)").arg(basicPoint[0]).arg(basicPoint[1]).arg(basicPoint[2]);
			codes += QString("makematrix.setAxis(%1,%2,%3)").arg(_axisdir[0]).arg(_axisdir[1]).arg(_axisdir[2]);
			QString wirerestr{};
			if (_ui->checkBoxDir1Reverse_3->isChecked()) wirerestr = "Yes";
			else wirerestr = "No";
			codes += QString("makematrix.setWireReverse('%1')").arg(wirerestr);
			codes += QString("makematrix.setWireCount(%1)").arg(wireCount);
			codes += QString("makematrix.setDegree(%1)").arg(deg);
		}

		if (_isEdit)
			codes += QString("makematrix.edit()");
		else
			codes += QString("makematrix.create()");

		_pyAgent->submit(codes);
		QDialog::accept();
		this->close();
		
	}

	void MakeMatrixDialog::on_geoSelectSurface_clicked()
	{	
			
		emit setSelectMode((int)ModuleBase::GeometryBody);
		_baseWidget->handleProcess(false);
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

	void MakeMatrixDialog::shapeSlected(Geometry::GeometrySet* set, int index)
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
			_ui->edgelabel->setText(text);
		}
	}

	void MakeMatrixDialog::pointWidgetClicked(GeoPointWidget* w)
	{
		_selectBody = false;
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

	void MakeMatrixDialog::showLinearDir2InfoChbox()
	{
		bool checked = _ui->dir2checkBox->isChecked();
		_ui->groupBox_4->setEnabled(checked);
	}

	void MakeMatrixDialog::on_radioButtonUser()
	{
		bool checked = _ui->radioButtonUser_1->isChecked();
		_ui->doubleSpinBoxX_1->setVisible(checked);
		_ui->doubleSpinBoxY_1->setVisible(checked);
		_ui->doubleSpinBoxZ_1->setVisible(checked);
	}
	void MakeMatrixDialog::on_radio2ButtonUser()
	{
		bool checked = _ui->radioButtonUser_2->isChecked();
		_ui->doubleSpinBoxX_2->setVisible(checked);
		_ui->doubleSpinBoxY_2->setVisible(checked);
		_ui->doubleSpinBoxZ_2->setVisible(checked);
	}
	void MakeMatrixDialog::on_radio3ButtonUser()
	{
		bool checked = _ui->radioButtonUser_3->isChecked();
		_ui->doubleSpinBoxX_3->setVisible(checked);
		_ui->doubleSpinBoxY_3->setVisible(checked);
		_ui->doubleSpinBoxZ_3->setVisible(checked);
	}

	void MakeMatrixDialog::on_TypeChanged(int index)
	{
		_ui->tabWidget->setCurrentIndex(index);
		if (index == 0)
		{
			_selectLinear = true;
			_selectWire = false;
		}
		else
		{
			_selectLinear = false;
			_selectWire = true;
		}
	}

	bool MakeMatrixDialog::getVector(double* vec)
	{
		bool ok = false;
		if (_ui->radioButtonX_3->isChecked())
			vec[0] = 1.0;
		else if (_ui->radioButtonY_3->isChecked())
			vec[1] = 1.0;
		else if (_ui->radioButtonZ_3->isChecked())
			vec[2] = 1.0;
		else
		{
			vec[0] = _ui->doubleSpinBoxX_3->value();
			vec[1] = _ui->doubleSpinBoxY_3->value();
			vec[2] = _ui->doubleSpinBoxZ_3->value();
		}
		for (int i = 0; i < 3; i++)
		{
			_axisdir[i] = vec[i];
		}

		return ok;
	}

	QList<double*> MakeMatrixDialog::getAngleList()
	{
		QList<double*> angleList{};
		QList<double*> tempList{};
		double deg{ 0.0 };
		bool ok = true;
		//sum
		QString textSum = _ui->sum1spinBox_2->text();
		int sum = textSum.toInt(&ok);
		//degree
		QString textDegree = _ui->axisAnglelineEdit->text();
		deg = textDegree.toDouble(&ok);

		if (ok)
			ok = fabs(deg) < 0.0000001 ? false : true;
		for (int i=1; i < sum;++i)
		{
			double temp = deg*i;
			double *angle = new double;
			*angle = temp*PI / 180.00;
			angleList.append(angle);
		}
		if (angleList.size() < 1) ok = false;
		if (ok) return angleList;
		if (!ok) return tempList;
	}

	void MakeMatrixDialog::getDir1(double* dir)
	{

		if (_ui->radioButtonX_1->isChecked())
			dir[0] = 1.0;
		else if (_ui->radioButtonY_1->isChecked())
			dir[1] = 1.0;
		else if (_ui->radioButtonZ_1->isChecked())
			dir[2] = 1.0;
		else
		{
			dir[0] = _ui->doubleSpinBoxX_1->value();
			dir[1] = _ui->doubleSpinBoxY_1->value();
			dir[2] = _ui->doubleSpinBoxZ_1->value();
		}
		
	}

	void MakeMatrixDialog::getDir2(double* dir)
	{
		if (_ui->radioButtonX_2->isChecked())
			dir[0] = 1.0;
		else if (_ui->radioButtonY_2->isChecked())
			dir[1] = 1.0;
		else if (_ui->radioButtonZ_2->isChecked())
			dir[2] = 1.0;
		else
		{
			dir[0] = _ui->doubleSpinBoxX_2->value();
			dir[1] = _ui->doubleSpinBoxY_2->value();
			dir[2] = _ui->doubleSpinBoxZ_2->value();
		}
	}

}