#include "ui_DialogCreateSet.h"
#include "DialogCreateSet.h"
#include "preWindow.h"
#include "meshData/meshSingleton.h"
#include "meshData/meshSet.h"
#include "meshData/meshKernal.h"
//#include "meshData/setMember.h"
#include "mainWindow/mainWindow.h"
#include <vtkIdTypeArray.h>
#include <QMessageBox>
#include <QPushButton>
#include <QDebug>
#include <QMultiHash>
#include "python/PyAgent.h"
#include <BRep_Builder.hxx>
#include <TopoDS_Compound.hxx>
#include <TopExp_Explorer.hxx>
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include "geometry/GeoCommon.h"

namespace MainWidget
{
	CreateSetDialog::CreateSetDialog(GUI::MainWindow* mainwindow, PreWindow* prew) : QFDialog(mainwindow)
		,_mainWindow(mainwindow), _preWindow(prew), _ui(new Ui::CreateSetDialog)
	{
		_meshData = MeshData::MeshData::getInstance();
		_ui->setupUi(this);
		_ui->checkBox->setCheckState(Qt::Unchecked);
		_ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("OK"));
		_ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
		connect(_ui->checkBox, SIGNAL(stateChanged(int)), this, SLOT(stateChangedSlot(int)));
		connect(_ui->typeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChangedSlot(int)));
		connect(this, SIGNAL(updateSetTree()), _mainWindow, SIGNAL(updateSetTreeSig()));

		hideFindCondition();
		init();
//		initializeTitle();
	}
	CreateSetDialog::~CreateSetDialog()
	{
		if (_ui != nullptr) delete _ui;
	}

	void CreateSetDialog::init()
	{
		_model = _preWindow->getSelectModel();
		_isGeo = (_model == ModuleBase::GeometryCurve || _model == ModuleBase::GeometrySurface || _model == ModuleBase::GeometryBody ||_model == ModuleBase::GeometryWinCurve || _model == ModuleBase::GeometryWinSurface || _model == ModuleBase::GeometryWinBody);
		if (_isGeo)
			_ui->checkBox->setCheckable(false);

		if (_model == ModuleBase::MeshNode || _model == ModuleBase::BoxMeshNode)
		{
			_ui->typeComboBox->setCurrentIndex(0);
			_ui->label_id->setText(tr("PointId : "));
			_ui->typeComboBox->setEnabled(false);
		}
		else if (_model == ModuleBase::MeshCell || _model == ModuleBase::BoxMeshCell)
		{
			_ui->typeComboBox->setCurrentIndex(1);
			_ui->label_id->setText(tr("CellId : "));
			_ui->typeComboBox->setEnabled(false);
		}
		else
			_ui->typeComboBox->setEnabled(true);

		//ModuleBase::SelectModel model = _preWindow->getSelectModel();
		//bool ismesh = model == ModuleBase::MeshNode || model == ModuleBase::MeshCell || model == ModuleBase::BoxMeshNode || model == ModuleBase::BoxMeshCell;
		//_ui->label_type->setVisible(!ismesh);
		//_ui->typeComboBox->setVisible(!ismesh);
	}

	void CreateSetDialog::stateChangedSlot(int state)
	{
		switch (state)
		{
		case 0:
			hideFindCondition();
			break;
		case 2:
			showFindCondition();
			break;
		}
	}

	void CreateSetDialog::currentIndexChangedSlot(int index)
	{
		if (index == -1)	return;
		QString type = _ui->typeComboBox->currentText();
		_geoIds = generateGeoIDs(type);
	}

	QString CreateSetDialog::getNameFromUi()
	{
		QString text = _ui->nameLineEdit->text();
		if (text.simplified().isEmpty())
			text = _ui->nameLineEdit->placeholderText();
		return text.simplified();
	}

	QString CreateSetDialog::generateGeoIDs(QString type)
	{
		auto selectgeo = _preWindow->getGeoSelectItems();		

		TopoDS_Compound aRes;
		BRep_Builder aBuilder;
		aBuilder.MakeCompound(aRes);
		bool empty = true;

		QList<Geometry::GeometrySet*> geoList = selectgeo->uniqueKeys();
		for (Geometry::GeometrySet* set : geoList)
		{
			TopoDS_Shape* body = set->getShape();
			if (_model == ModuleBase::GeometryBody || _model == ModuleBase::GeometryWinBody)
			{
				aBuilder.Add(aRes, *body);
				continue;
			}

			TopAbs_ShapeEnum shapetype = TopAbs_SOLID;
			switch (_model)
			{
			case ModuleBase::GeometrySurface:
			case ModuleBase::GeometryWinSurface:
				shapetype = TopAbs_FACE; break;
			case ModuleBase::GeometryCurve:
			case ModuleBase::GeometryWinCurve:
				shapetype = TopAbs_EDGE; break;
			case ModuleBase::GeometryPoint:
			case ModuleBase::GeometryWinPoint:
				shapetype = TopAbs_VERTEX; break;
			default: break;
			}

			QList<int> member = selectgeo->values(set);
			for (int m : member)
			{
				if (m <0) continue;

				TopExp_Explorer exper(*body, shapetype);
				for (int index = 0; index < m && exper.More(); exper.Next(), ++index);

				const TopoDS_Shape& s = exper.Current();
				aBuilder.Add(aRes, s);
				empty = false;
			}

		}
		if (empty) return QString();
		
		QStringList kidString;
		const int n = _meshData->getKernalCount();
		for (int i = 0; i < n; ++i)
		{
			MeshData::MeshKernal* k = _meshData->getKernalAt(i);
			if (!k->isVisible())  continue;
			int kid = k->getID();
			vtkDataSet* dataset = k->getMeshData();

			QList<int> inids;

			if ((_model == ModuleBase::GeometrySurface || _model == ModuleBase::GeometryWinSurface) && type.toLower() == "element")
				inids = GeoCommon::getD2ElementsInShape(dataset, &aRes);
			if ((_model == ModuleBase::GeometrySurface || _model == ModuleBase::GeometryWinSurface) && type.toLower() == "node")
				inids = GeoCommon::getNodesInShape(dataset, &aRes);
			if ((_model == ModuleBase::GeometryBody || _model == ModuleBase::GeometryWinBody) && type.toLower() == "element")
				inids = GeoCommon::getBodyElementsInShape(dataset, &aRes);
			if (inids.isEmpty()) continue;

			QStringList ids;
			for (int d : inids) ids.append(QString::number(d));
			QString s = QString("%1:%2").arg(kid).arg(ids.join(" "));
			kidString.append(s);
		}

		return kidString.join(";");
	}

	void CreateSetDialog::hideFindCondition()
	{
		_ui->groupBox->setVisible(false);
		setMinimumSize(371, 136);
		setMaximumSize(371, 136);
		_ui->nameLineEdit->setPlaceholderText(QString("set_%1").arg(MeshData::MeshSet::getMaxID() + 1));
	}

	void CreateSetDialog::showFindCondition()
	{
		_ui->groupBox->setVisible(true);
		setMinimumSize(371, 236);
		setMaximumSize(371, 236);
		_ui->nameLineEdit->setPlaceholderText(QString("coplanar_%1").arg(MeshData::MeshSet::getMaxID() + 1));

		QMultiHash<vtkDataSet*, int>* data_itemId = _preWindow->getSelectItems();
		if ((*data_itemId).size() == 0)	   return;
		QMutableHashIterator<vtkDataSet*, int> iterator(*data_itemId);
		while (iterator.hasNext())
		{
			iterator.next();
			_data = iterator.key();
			_seedId = iterator.value();
			break;
		}

		if (_seedId != -1)
			_ui->idLineEdit->setText(QString::number(_seedId));
	}

	void CreateSetDialog::accept()
	{
		if (_ui->checkBox->isChecked())
			findConplanarAccept();
		else
			createSetAccept();
		QDialog::accept();
	}

	bool CreateSetDialog::selectFromGeo()
	{
		QString name = this->getNameFromUi();
		QString type;
		if (_ui->typeComboBox->currentIndex() == 1)
			type = "Element";
		else if(_ui->typeComboBox->currentIndex() == 0)
			type = "Node";
 
		if (_geoIds.isEmpty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("No Node or Element selected !"));
			return false;
		}
		QString code = QString("MainWindow.createSet(\"%1\",\"%2\",\"%3\")").arg(name).arg(type).arg(_geoIds);
		Py::PythonAagent::getInstance()->submit(code);
		return true;
	}

	void CreateSetDialog::createSetAccept()
	{
		if (_preWindow == nullptr)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Open window first !"));
			QDialog::reject();
			return;
		}
		if (_isGeo)
		{
			if (this->selectFromGeo())
				QDialog::accept();
			else
				QDialog::reject();
			return;
		}

		QMultiHash<vtkDataSet*, int>* selectItems = _preWindow->getSelectItems();
		if (selectItems == nullptr || selectItems->isEmpty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("No Node or Element selected !"));
			QDialog::reject();
			return;
		}

		QStringList memList;
		QList<vtkDataSet*> datasetList = selectItems->uniqueKeys();
		for (vtkDataSet* dataset : datasetList)
		{
			int kernalid = _meshData->getIDByDataSet(dataset);
			QList<int> mem = selectItems->values(dataset);
			QStringList mems;
			for (int id : mem) mems.append(QString::number(id));
			QString s = QString("%1:%2").arg(kernalid).arg(mems.join(" "));
			memList.append(s);
		}

		QString idstring = memList.join(";");
		QString type = "Node";
		if (_model == ModuleBase::MeshCell || _model == ModuleBase::BoxMeshCell) type = "Element";
		QString name = this->getNameFromUi();
		QString code = QString("MainWindow.createSet(\"%1\",\"%2\",\"%3\")").arg(name).arg(type).arg(idstring);
		Py::PythonAagent::getInstance()->submit(code);
	}

	void CreateSetDialog::findConplanarAccept()
	{		
		QMultiHash<vtkDataSet*, int>* selectItems = _preWindow->getSelectItems();
		if (selectItems == nullptr || selectItems->isEmpty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("No Node or Element selected !"));
			QDialog::reject();
			return;
		}

		int kernalId = _meshData->getIDByDataSet(_data);
		if (kernalId == -1)	return;

		int seedType = _ui->typeComboBox->currentIndex();
		QString qSeedType;
		if (seedType == 0)
			qSeedType = "Node";
		else
			qSeedType = "Element";

		double minAngle = _ui->doubleSpinBox->value();
		QString code = QString("MainWindow.findConplanarPorC(\"%1\",%2,%3,%4,\"%5\")").arg(qSeedType).arg(_seedId).arg(minAngle).arg(kernalId).arg(getNameFromUi());
		Py::PythonAagent::getInstance()->submit(code);
	}
}
