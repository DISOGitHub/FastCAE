#include "dialogRotateFeature.h"
#include "ui_dialogRotateFeature.h"
#include "geoPointWidget.h"
#include "geometry/geometryParaRotateFeature.h"
#include "GeometryCommand/GeoCommandRotateFeature.h"
#include "GeometryCommand/GeoCommandList.h"
#include <QMessageBox>
#include "settings/busAPI.h"
#include "settings/GraphOption.h"
#include <QColor>
#include <vtkActor.h>
#include <vtkProperty.h>
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
        emit setSelectMode((int)ModuleBase::None);
        emit updateGraphOptions();
    }

    void RotateFeatureDialog::initEdit()
    {
		_ui->geoSelectSurface->setEnabled(false);  //不允许选取其他的
        if (_editSet == nullptr)  return;
        Geometry::GeometryModelParaBase* pm = _editSet->getParameter();
        Geometry::GeometryParaRotateFeature* p = dynamic_cast<Geometry::GeometryParaRotateFeature*>(pm);
        if (p == nullptr) return;

        Geometry::GeometrySet* ori = p->getOriginObject();
		if (ori == nullptr) return;
        _geobodyList.append(ori);
        emit showGeometry(ori);
        emit hideGeometry(_editSet);
        emit highLightGeometrySet(ori, true);
        QString text = QString(tr("Selected body(%1)")).arg(_geobodyList.size());
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
            QPair<Geometry::GeometrySet*, int> edgeset = p->getEdge();
            QList<vtkActor*> acs;
            emit highLightGeometryEdge(edgeset.first, edgeset.second, &acs);
            _edgeActor = acs.at(0);
            _edgeSet = edgeset.first;
            _edgeIndex = edgeset.second;
        }
        else
        {
            double axis[3] = { 0 };
            p->getVector(axis);
			if (axis[0] != 0 && axis[1] == 0 && axis[2] == 0)
			{
				_ui->radioButtonX->setChecked(true);
			}
			else if (axis[0] == 0 && axis[1] != 0 && axis[2] == 0)
			{
				_ui->radioButtonY->setChecked(true);
			}
			else if (axis[0] == 0 && axis[1] == 0 && axis[2] != 0)
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
			if (p->isSaveOrigin()==false)
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
        {
            QString text = _ui->lineEditDegree->text();
            degree = text.toDouble(&ok);
            if (ok)
                ok = abs(degree) < 0.0000001 ? false: true;
        }


        if (!ok)
        {
            QMessageBox::warning(this, tr("Warning"), tr("Input Wrong !"));
            return;
        }
        bool s = _ui->checkBoxSaveOrigin->isChecked();

        QStringList codes{};
        codes += QString("rotate = CAD.RotateFeature()");
        for (int i = 0; i < _geobodyList.size(); ++i)
        {
            auto s = _geobodyList.at(i);
            if (s == nullptr) continue;
            int id = s->getID();
            codes += QString("rotate.appendObject(%1)").arg(id);
        }
        codes += QString("rotate.setBasicPoint(%1,%2,%3)").arg(basicPoint[0]).arg(basicPoint[1]).arg(basicPoint[2]);

        if (_ui->comboBoxOption->currentIndex() == 0)
            codes += QString("rotate.setAxisFromBody(%1, %2)").arg(_edgeSet->getID()).arg(_edgeIndex);
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
        emit setSelectMode((int)ModuleBase::GeometrySurface);
        _baseWidget->handleProcess(false);
        _selectBody = true;
        _selectEdge = false;
        
        for (int i = 0; i < _geobodyList.size(); ++i)
        {
            auto set = _geobodyList.at(i);
            emit highLightGeometrySet(set, true);
        }

    }

    void RotateFeatureDialog::selectActorShape(vtkActor* actor, int index, Geometry::GeometrySet* set)
    {
        if (_selectBody)
        {
            if (_geobodyList.contains(set))
            {
                emit highLightGeometrySet(set, false);
                _geobodyList.removeOne(set);
            }
            else
            {
                emit highLightGeometrySet(set, true);
                _geobodyList.append(set);
            }
            QString text = QString(tr("Selected body(%1)")).arg(_geobodyList.size());
            _ui->edgelabel->setText(text);
        }
        else if (_selectEdge)
        {
            QColor color;
            if (_edgeActor != nullptr)
            {
                color = Setting::BusAPI::instance()->getGraphOption()->getGeometryCurveColor();
                _edgeActor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
            }
            color = Setting::BusAPI::instance()->getGraphOption()->getHighLightColor();
            _edgeActor = actor;
            _edgeActor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());

            _edgeSet = set;
            _edgeIndex = index;

            _ui->edgelabel_2->setText(tr("Selected edge(1)"));
        }
    }

    void RotateFeatureDialog::pointWidgetClicked(GeoPointWidget* w)
    {
        _selectBody = false;
        _selectEdge = false;
        w->handleProcess(true);

        for (int i = 0; i < _geobodyList.size(); ++i)
        {
            auto set = _geobodyList.at(i);
            emit highLightGeometrySet(set, false);
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
            if (_edgeSet == nullptr || _edgeIndex < 0) return false;
//             TopoDS_Shape* shape = _edgeSet->getShape();
// 
//             TopExp_Explorer edgeExp(*shape, TopAbs_EDGE);
//             for (int k = 0; k < _edgeIndex; ++k) edgeExp.Next();
// 
//             const TopoDS_Edge &edge = TopoDS::Edge(edgeExp.Current());
//             if (edge.IsNull()) return false;
// 
//             BRepAdaptor_Curve adapt(edge);
//             if (adapt.GetType() != GeomAbs_Line) return false;
//             gp_Lin line = adapt.Line();
//             gp_Dir direc = line.Direction();
//             vec[0] = direc.X();
//             vec[1] = direc.Y();
//             vec[2] = direc.Z();
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

// 		if (_ui->checkBoxReverse->isChecked())
// 			for (int i = 0; i < 3; ++i)
// 				vec[i] *= -1;

        return ok;
    }

    void RotateFeatureDialog::on_geoSelectCurve_clicked()
    {
        _selectBody = false;
        _baseWidget->handleProcess(false);
        _selectEdge = true;
        emit setSelectMode((int)ModuleBase::GeometryCurve);
    }

}