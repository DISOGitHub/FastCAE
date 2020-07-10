#include "dialogRotateFeature.h"
#include "ui_dialogRotateFeature.h"
#include "geoPointWidget.h"
#include "geometry/geometryParaRotateFeature.h"
#include "GeometryCommand/GeoCommandRotateFeature.h"
#include "GeometryCommand/GeoCommandList.h"
#include <QMessageBox>
#include <TopExp_Explorer.hxx>
#include "geometry/geometrySet.h"
#include <TopoDS_Edge.hxx>
#include <TopoDS.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <gp_Lin.hxx>
#include "python/PyAgent.h"

namespace GeometryWidget
{
    RotateFeatureDialog::RotateFeatureDialog(GUI::MainWindow* m, MainWidget::PreWindow* p) :
        GeoDialogBase(m, p)
    {
        _ui = new Ui::RotateFeatureDialog;
        _ui->setupUi(this);
        init();
        connect(_ui->comboBoxOption, SIGNAL(currentIndexChanged(int)), this, SLOT(on_TypeChanged(int)));
        connect(_ui->radioButtonUser, SIGNAL(toggled(bool)), this, SLOT(on_radioButtonUser()));
        on_radioButtonUser();
    }

    RotateFeatureDialog::RotateFeatureDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set)
        :GeoDialogBase(m, p)
    {
        _ui = new Ui::RotateFeatureDialog;
        _ui->setupUi(this);
        _isEdit = true;
        _editSet = set;
        init();
        connect(_ui->comboBoxOption, SIGNAL(currentIndexChanged(int)), this, SLOT(on_TypeChanged(int)));
        connect(_ui->radioButtonUser, SIGNAL(toggled(bool)), this, SLOT(on_radioButtonUser()));
        on_radioButtonUser();
        this->initEdit();
    }

    RotateFeatureDialog::~RotateFeatureDialog()
    {
        if (_ui != nullptr) delete _ui;
    }

    void RotateFeatureDialog::initEdit()
    {
		if (_isEdit)
			_ui->geoSelectSurface->setEnabled(false);
        if (_editSet == nullptr)  return;
        Geometry::GeometryModelParaBase* pm = _editSet->getParameter();
        Geometry::GeometryParaRotateFeature* p = dynamic_cast<Geometry::GeometryParaRotateFeature*>(pm);
        if (p == nullptr) return;

        Geometry::GeometrySet* ori = p->getOriginObject();
		if (ori == nullptr) return;
        emit showGeometry(ori);
        emit hideGeometry(_editSet);
		_bodysHash = p->getBodys();
		if (_bodysHash.size() < 1) return;
		QMultiHash<Geometry::GeometrySet*, int>::iterator iter = _bodysHash.begin();
		for (; iter != _bodysHash.end(); ++iter)
		{
			emit setSelectMode((int)ModuleBase::GeometryBody);
			emit highLightGeometrySolidSig(iter.key(), iter.value(), true);
		}
        QString text = QString(tr("Selected body(%1)")).arg(_bodysHash.size());
        _ui->edgelabel->setText(text);
		
        double basicPt[3] = { 0 };
        p->getBasicPoint(basicPt);
        _baseWidget->setCoordinate(basicPt);

        double ang = p->getAngle();
        _ui->lineEditDegree->setText(QString::number(ang));
        
        int method = p->getMethod();
        _ui->comboBoxOption->setCurrentIndex(method);

        bool saveOri = p->isSaveOrigin();
        bool isreverse = p->isReverse();

        _ui->checkBoxSaveOrigin->setChecked(saveOri);
        _ui->checkBoxReverse->setChecked(isreverse);

        if (method == 0)
        {
            _ui->edgelabel_2->setText(tr("Selected edge(1)"));
			 _edgepair = p->getEdge();
			 emit setSelectMode((int)ModuleBase::GeometryCurve);
			 emit highLightGeometryEdgeSig(_edgepair.first, _edgepair.second, true);
        }
        else
        {
            double axis[3] = { 0 };
            p->getVector(axis);
			if (axis[0] == 1 && axis[1] == 0 && axis[2] == 0)
			{
				_ui->radioButtonX->setChecked(true);
			}
			else if (axis[0] == 0 && axis[1] == 1 && axis[2] == 0)
			{
				_ui->radioButtonY->setChecked(true);
			}
			else if (axis[0] == 0 && axis[1] == 0 && axis[2] == 1)
			{
				_ui->radioButtonZ->setChecked(true);
			}
			else
			{
				_ui->radioButtonUser->setChecked(true);
				_ui->doubleSpinBoxX->setVisible(true);
				_ui->doubleSpinBoxY->setVisible(true);
				_ui->doubleSpinBoxZ->setVisible(true);
				_ui->doubleSpinBoxX->setValue(axis[0]);
				_ui->doubleSpinBoxY->setValue(axis[1]);
				_ui->doubleSpinBoxZ->setValue(axis[2]);
			}
        }

    }

    void RotateFeatureDialog::closeEvent(QCloseEvent *e)
    {
        QDialog::closeEvent(e);
        delete this;
    }

    void RotateFeatureDialog::reject()
    {
		if (_isEdit)
		{
			if (_editSet == nullptr) return;
			Geometry::GeometryModelParaBase* pb = _editSet->getParameter();
			Geometry::GeometryParaRotateFeature* p = dynamic_cast<Geometry::GeometryParaRotateFeature*>(pb);
			if (p == nullptr) return;
			Geometry::GeometrySet*  originalSet = p->getOriginObject();
			if (originalSet == nullptr) return;
			emit showGeometry(_editSet);
			emit hideGeometry(originalSet);
		} 
        QDialog::reject();
        this->close();
	 
    }

    void RotateFeatureDialog::init()
    {
        _ui->tabWidget->tabBar()->hide();
		this->translateButtonBox(_ui->buttonBox);
        _baseWidget = new GeoPointWidget(_mainWindow, _preWindow);
        _ui->basicPointlLayout->addWidget(_baseWidget);
        connect(_baseWidget, SIGNAL(buttonCkicked(GeoPointWidget*)), this, SLOT(pointWidgetClicked(GeoPointWidget*)));
    }

    void RotateFeatureDialog::accept()
    {
        double basicPoint[3] = { 0.0 };
        double vec[3] = { 0.0 };
        double degree{ 0.0 };

        bool ok = _baseWidget->getCoordinate(basicPoint);
        if (ok)
            ok = getVector(vec);
		if (ok)
			if (_bodysHash.size() < 1) ok = false;
		if (ok)
			if (_ui->comboBoxOption->currentIndex() == 0 && _edgepair.first == nullptr)
				ok = false;
        if (ok)
        {
            QString text = _ui->lineEditDegree->text();
            degree = text.toDouble(&ok);
            if (ok)
                ok = fabs(degree) < 0.0000001 ? false: true;
        }
        if (!ok)
        {
            QMessageBox::warning(this, tr("Warning"), tr("Input Wrong !"));
            return;
        }
        bool s = _ui->checkBoxSaveOrigin->isChecked();
        QStringList codes{};
        codes += QString("rotate = CAD.RotateFeature()");

		QMultiHash<Geometry::GeometrySet*, int>::iterator it = _bodysHash.begin();
		for (; it != _bodysHash.end(); it++)
		{
			codes += QString("rotate.appendBody(%1,%2)").arg(it.key()->getID()).arg(it.value());
		}
        codes += QString("rotate.setBasicPoint(%1,%2,%3)").arg(basicPoint[0]).arg(basicPoint[1]).arg(basicPoint[2]);

        if (_ui->comboBoxOption->currentIndex() == 0)
            codes += QString("rotate.setAxisFromBody(%1, %2)").arg(_edgepair.first->getID()).arg(_edgepair.second);
        else
            codes += QString("rotate.setAxis(%1,%2,%3)").arg(vec[0]).arg(vec[1]).arg(vec[2]);

        codes += QString("rotate.setAngle(%1)").arg(degree);

        if (_ui->checkBoxReverse->isChecked())
            codes += QString("rotate.reverse()");
        
        if (s) codes += QString("rotate.saveOrigin()");

        if (!_isEdit)
            codes += QString("rotate.rotate()");
		else
		{
			codes += QString("rotate.setEditID(%1)").arg(_editSet->getID());
			codes += QString("rotate.edit()");
		}
           
        _pyAgent->submit(codes);
 //		auto c = new Command::CommandRotateFeature(_mainWindow, _preWindow);
// 		c->setBodys(_geobodyList);
// 		c->setVector(vec);
// 		c->setBasicPoint(basicPoint);
// 		c->setDegree(degree);
// 		c->setSaveOrigin(s);
// 
// 		bool success = Command::GeoComandList::getInstance()->executeCommand(c);
// 
// 		if (!success)
// 		{
// 			QMessageBox::warning(this, tr("Warning"), tr("Create failed ! "));
// 			return;
// 		}
        QDialog::accept();
        this->close();
    }

    void RotateFeatureDialog::on_TypeChanged(int index)
    {
        _ui->tabWidget->setCurrentIndex(index);
    }

    void RotateFeatureDialog::on_geoSelectSurface_clicked()
    {
        emit setSelectMode((int)ModuleBase::GeometryBody);
        _baseWidget->handleProcess(false);
        _selectBody = true;
        _selectEdge = false;
        
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
		if (_edgepair.first != nullptr)
		{
			highLightGeometryEdgeSig(_edgepair.first, _edgepair.second, false);
		}

    }

	void RotateFeatureDialog::shapeSlected(Geometry::GeometrySet* set, int index)
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
		else if (_selectEdge)
		{
			if (_edgepair.first != nullptr)
			{
				emit highLightGeometryEdgeSig(_edgepair.first, _edgepair.second, false);
			}
			_edgepair.first = set;
			_edgepair.second = index;
			
			emit highLightGeometryEdgeSig(_edgepair.first, index, true);
			_ui->edgelabel_2->setText(tr("Selected edge(1)"));
		}
	}

	void RotateFeatureDialog::pointWidgetClicked(GeoPointWidget* w)
    {
        _selectBody = false;
        _selectEdge = false;
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
		if (_edgepair.first != nullptr)
		{
			highLightGeometryEdgeSig(_edgepair.first, _edgepair.second, false);
		}
    }
    
    void RotateFeatureDialog::on_radioButtonUser()
    {
        bool checked = _ui->radioButtonUser->isChecked();
        _ui->doubleSpinBoxX->setVisible(checked);
        _ui->doubleSpinBoxY->setVisible(checked);
        _ui->doubleSpinBoxZ->setVisible(checked);
    }

    bool RotateFeatureDialog::getVector(double* vec)
    {
        bool ok = false;

        if (_ui->comboBoxOption->currentIndex() == 0)
        {
            if (_edgepair.first == nullptr || _edgepair.second < 0) return false;
            ok = true;
        }
        else
        {
            if (_ui->radioButtonX->isChecked())
                vec[0] = 1.0;
            else if (_ui->radioButtonY->isChecked())
                vec[1] = 1.0;
            else if (_ui->radioButtonZ->isChecked())
                vec[2] = 1.0;
            else
            {
                vec[0] = _ui->doubleSpinBoxX->value();
                vec[1] = _ui->doubleSpinBoxY->value();
                vec[2] = _ui->doubleSpinBoxZ->value();
            }
            
            double mod = vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
            if (mod > 0.000001) ok = true;
        }
        return ok;
    }

    void RotateFeatureDialog::on_geoSelectCurve_clicked()
    {
        _selectBody = false;
        _baseWidget->handleProcess(false);
        _selectEdge = true;
        emit setSelectMode((int)ModuleBase::GeometryCurve);
		if (_edgepair.first!=nullptr)
		{
			highLightGeometryEdgeSig(_edgepair.first, _edgepair.second, true);
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

}