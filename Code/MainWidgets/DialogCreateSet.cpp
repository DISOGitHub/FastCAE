#include "ui_DialogCreateSet.h"
#include "DialogCreateSet.h"
#include "preWindow.h"
#include "meshData/meshSingleton.h"
#include "meshData/meshSet.h"
#include "meshData/meshKernal.h"
#include "meshData/setMember.h"
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
		_ui->setupUi(this);
		_meshData = MeshData::MeshData::getInstance();
		connect(this, SIGNAL(updateSetTree()), _mainWindow, SIGNAL(updateSetTreeSig()));
		_ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("OK"));
		_ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
		init();
//		initializeTitle();
	}
	CreateSetDialog::~CreateSetDialog()
	{
		if (_ui != nullptr) delete _ui;
	}
	void CreateSetDialog::init()
	{
		int maxID = MeshData::MeshSet::getMaxID();
		QString name = QString("set_%1").arg(maxID + 1);
		_ui->nameLineEdit->setPlaceholderText(name);

		ModuleBase::SelectModel model = _preWindow->getSelectModel();
		bool ismesh = model == ModuleBase::MeshNode || model == ModuleBase::MeshCell || model == ModuleBase::BoxMeshNode || model == ModuleBase::BoxMeshCell;
		_ui->labelType->setVisible(!ismesh);
		_ui->typeComboBox->setVisible(!ismesh);

	}

	QString CreateSetDialog::getNameFromUi()
	{
		QString text = _ui->nameLineEdit->text();
		if (text.simplified().isEmpty())
			text = _ui->nameLineEdit->placeholderText();
		return text.simplified();
	}

	void CreateSetDialog::accept()
	{
		if (_preWindow == nullptr)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Open window first !"));
			QDialog::reject();
			return;
		}
		ModuleBase::SelectModel model = _preWindow->getSelectModel();
		if (model == ModuleBase::GeometryCurve || model == ModuleBase::GeometrySurface || model == ModuleBase::GeometryBody ||  
			model == ModuleBase::GeometryWinCurve || model == ModuleBase::GeometryWinSurface || model == ModuleBase::GeometryWinBody)
		{
			if (this->selectFromGeo())
				QDialog::accept();
			else
				QDialog::reject();
			return;
		}

		QString name = this->getNameFromUi();//name
// 		vtkDataSet* dataset = _preWindow->getHighLightDataSet();
// 		vtkIdTypeArray* idarray = vtkIdTypeArray::New();
// 		idarray->DeepCopy(_preWindow->getHighLightIDArray());
		
		QMultiHash<vtkDataSet*, int>* selectItems = _preWindow->getSelectItems();

		

		if (selectItems == nullptr || selectItems->isEmpty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("No Node or Element selected !"));
			QDialog::reject();
			return;
		}

	
		QStringList memList;
		auto meshdata = MeshData::MeshData::getInstance();
		QList<vtkDataSet*> datasetList = selectItems->uniqueKeys();
		for (vtkDataSet* dataset : datasetList)
		{
			int kernalid = meshdata->getIDByDataSet(dataset);
			QList<int> mem = selectItems->values(dataset);
			QStringList mems;
			for (int id : mem) mems.append(QString::number(id));
			QString s = QString("%1:%2").arg(kernalid).arg(mems.join(" "));
			memList.append(s);
		}

		QString idstring = memList.join(";");
		
		QString type = "Node";
//		ModuleBase::SelectModel model = _preWindow->getSelectModel();
		if (model == ModuleBase::MeshCell || model == ModuleBase::BoxMeshCell) type = "Element";

		QString code = QString("MainWindow.createSet(\"%1\",\"%2\",\"%3\")").arg(name).arg(type).arg(idstring);
		Py::PythonAagent::getInstance()->submit(code);
		
		QDialog::accept();
	}

	bool CreateSetDialog::selectFromGeo()
	{
		QString name = this->getNameFromUi();
	
		QString type = "Node";
		if (_ui->typeComboBox->currentIndex() == 0) type = "Element";

		QString idstring = this->generateGeoIDs(type);
		if (idstring.isEmpty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("No Node or Element selected !"));
			return false;
		}
		QString code = QString("MainWindow.createSet(\"%1\",\"%2\",\"%3\")").arg(name).arg(type).arg(idstring);
		Py::PythonAagent::getInstance()->submit(code);
		return true;
	}

	QString CreateSetDialog::generateGeoIDs(QString type)
	{
		QStringList kidString;
		QMultiHash<Geometry::GeometrySet*, int> selectgeo = _preWindow->getGeoSelectItems();
		ModuleBase::SelectModel modle = _preWindow->getSelectModel();

		TopoDS_Compound aRes;
		BRep_Builder aBuilder;
		aBuilder.MakeCompound(aRes);

		QList<Geometry::GeometrySet*> geoList = selectgeo.uniqueKeys();
		for (Geometry::GeometrySet* set : geoList)
		{
			TopoDS_Shape* body = set->getShape();
			if (modle == ModuleBase::GeometryBody)
			{
				aBuilder.Add(aRes, *body);
				continue;
			}

			TopAbs_ShapeEnum shapetype = TopAbs_SOLID;
			switch (modle)
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

			QList<int> member = selectgeo.values(set);
			for (int m : member)
			{
				if (m <0) continue;

				TopExp_Explorer exper(*body, shapetype);
				for (int index = 0; index < m && exper.More(); exper.Next(), ++index);

				const TopoDS_Shape& s = exper.Current();
				aBuilder.Add(aRes, s);

			}

		}

		MeshData::MeshData* meshdata = MeshData::MeshData::getInstance();
		const int n = meshdata->getKernalCount();
		for (int i = 0; i < n; ++i)
		{
			MeshData::MeshKernal* k = meshdata->getKernalAt(i);
			if (!k->isVisible())  continue;
			int kid = k->getID();
			vtkDataSet* dataset = k->getMeshData();

			QList<int> inids;

			if ((modle == ModuleBase::GeometrySurface || modle == ModuleBase::GeometryWinSurface) && type.toLower() == "element")
				inids = GeoCommon::getD2ElementsInShape(dataset, &aRes);
			if ((modle == ModuleBase::GeometrySurface || modle == ModuleBase::GeometryWinSurface) && type.toLower() == "node")
				inids = GeoCommon::getNodesInShape(dataset, &aRes);
			if (inids.isEmpty()) continue;

			QStringList ids;
			for (int d : inids) ids.append(QString::number(d));
			QString s = QString("%1:%2").arg(kid).arg(ids.join(" "));
			kidString.append(s);
		}

		return kidString.join(";");
	}




}
