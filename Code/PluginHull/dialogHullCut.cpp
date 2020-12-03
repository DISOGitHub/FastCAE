#include "dialogHullCut.h"
#include "ui_dialogHullCut.h"
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include <QDebug>
#include <BRepBndLib.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepTools.hxx>
#include <TopExp_Explorer.hxx>
#include <Bnd_Box.hxx>
#include <stdlib.h>
#include <TopoDS.hxx>
#include <QRegExp.h>
#include <QMessageBox>
#include "GeoCommandMakeHullCut.h"
#include "GeometryCommand/GeoCommandList.h"
#include <vtkPoints.h>
#include <vtkUnstructuredGrid.h>
#include <vtkIdList.h>
#include <vtkCellType.h>
#include "meshData/meshKernal.h"
#include "meshData/meshSingleton.h"
#include "MainWidgets/preWindow.h"
#include "mainWindow/mainWindow.h"
#include "mainWindow/SubWindowManager.h"

namespace PluginShip
{
	HullCutDialog::HullCutDialog(GUI::MainWindow* m)
	{
		_ui = new Ui::DialogHullCut;
		_ui->setupUi(this);
		_mainwindow = m;
		init();
		connect(_ui->comboBox_2, SIGNAL(currentIndexChanged(int)), this, SLOT(on_TypeChanged2(int)));
	} 

	HullCutDialog::~HullCutDialog()
	{

	}

	void HullCutDialog::init()
	{
		_ui->comboBox->clear(); //Çå³ýÁÐ±í
		Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
		int geosize = data->getGeometrySetCount();
		for (int i = 0; i < geosize; i++)
		{
			Geometry::GeometrySet * geoset = data->getGeometrySetAt(i);
			TopoDS_Shape* shape = geoset->getShape();
			_shapelist.push_back(shape);
			Bnd_Box bndBox;
			BRepBndLib::Add(*shape, bndBox);
			bndBox.SetGap(0.0000);
			gp_Pnt cornerMin = bndBox.CornerMin();
			gp_Pnt cornerMax = bndBox.CornerMax();
			double minx = cornerMin.X();
			double maxx = cornerMax.X();
			QString name = geoset->getName();
			QString addstr = name + "(X:[" + QString::number(minx) + "," + QString::number(maxx) + "])";
			_ui->comboBox->addItem(addstr,i); 
		}
		
	}

	void HullCutDialog::getModelShapePara(TopoDS_Shape* aShape)
	{
		Bnd_Box bndBox;
		BRepBndLib::Add(*aShape, bndBox);
		bndBox.SetGap(0.0000);
		gp_Pnt cornerMin = bndBox.CornerMin();
		gp_Pnt cornerMax = bndBox.CornerMax();
		
	}

	void HullCutDialog::on_TypeChanged2(int AxisId)
	{
		_ui->comboBox->clear();
		Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
		int geosize = data->getGeometrySetCount();
		for (int i = 0; i < geosize; i++)
		{
			Geometry::GeometrySet * geoset = data->getGeometrySetAt(i);
			TopoDS_Shape* shape = geoset->getShape();
			_shapelist.push_back(shape);
			Bnd_Box bndBox;
			BRepBndLib::Add(*shape, bndBox);
			bndBox.SetGap(0.0000);
			gp_Pnt cornerMin = bndBox.CornerMin();
			gp_Pnt cornerMax = bndBox.CornerMax();
			double minx, miny, minz, maxx, maxy, maxz;
			minx = cornerMin.X(); miny = cornerMin.Y(); minz = cornerMin.Z();
			maxx = cornerMax.X(); maxy = cornerMax.Y(); maxz = cornerMax.Z();
			QString name = geoset->getName();
		
			switch (AxisId)
			{
			case 0:
			{
				QString addstr = name + "(X:[" + QString::number(minx) + "," + QString::number(maxx) + "])";
				_ui->comboBox->addItem(addstr, i);
				break;
			}
			case 1:
			{
				QString addstr = name + "(Y:[" + QString::number(miny) + "," + QString::number(maxy) + "])";
				_ui->comboBox->addItem(addstr, i);
				break;
			}
			case 2:

			{
				QString addstr = name + "(Z:[" + QString::number(minz) + "," + QString::number(maxz) + "])";
				_ui->comboBox->addItem(addstr, i);
				break;
			}
			default:
				break;
			}
		}
		
	}

	void HullCutDialog::accept()
	{
		bool ok = true;
		int index=_ui->comboBox->currentIndex();
		TopoDS_Shape* shape = _shapelist[index];
		if (shape == nullptr) ok=false;
		double cutloc = _ui->lineEdit_2->text().toDouble();
		QString current = _ui->comboBox->currentText();

		QRegExp rx("\\[(.*)\\]");
		int pos = current.indexOf(rx);
		QStringList caplis=rx.cap(1).simplified().split(",");
		double min = caplis.front().toDouble();
		double max = caplis.back().toDouble();
		if (cutloc<=min||cutloc>=max) ok = false;
		int nums = _ui->lineEdit->text().toInt();
		if (nums < 1) ok=false;
		if (!ok) 
			QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("CutParameter Error !"));

		Command::GeoCommandCreateHullCut* command = new Command::GeoCommandCreateHullCut();
		command->setGeoIndex(index);
		command->setAxisType(_ui->comboBox_2->currentText());
		command->setCutLocation(cutloc);
		command->setNums(nums);
		QList<double*> outputlist{};
		ok=command->execute(outputlist);
		if (!ok)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Cut failed !"));
		}
		else
		{
			/*ofstream ofile;//¢Ù
			ofile.open("D:\\test11.txt", ios::out);//¢Ú
			for (int i = 0; i < outputlist.size(); i++)
			{ 
				ofile << "point = CAD.Point()" << endl;
				QString str = QString("point.setName('Point_%1')").arg(i + 1);
				ofile << str.toStdString() << endl;
				ofile << "point.setLocation(0,0,0)" << endl;
				QString sttt = QString("point.setOffset(0, %1, %2)").arg(outputlist[i][1]).arg(outputlist[i][2]);
				ofile << sttt.toStdString() << endl;
				ofile << "point.create()" << endl;
			}
			ofile.close();*/

			vtkSmartPointer<vtkUnstructuredGrid> dataset = vtkSmartPointer<vtkUnstructuredGrid>::New();
			vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
			for each (double* var in outputlist)
			{
				points->InsertNextPoint(var);
			}
			dataset->SetPoints(points);
			const int nv = outputlist.size();
			for (int i = 0; i < nv - 1; ++i)
			{
				vtkSmartPointer<vtkIdList> cell = vtkSmartPointer<vtkIdList>::New();
				cell->InsertNextId(i); cell->InsertNextId(i + 1);
				dataset->InsertNextCell(VTK_LINE, cell);
			}

			MeshData::MeshKernal* k = new MeshData::MeshKernal;
			k->setMeshData((vtkDataSet*)dataset);
			QString name = QString("Cut_ %1").arg(cutloc);
			k->setName(name);
			MeshData::MeshData* _meshData =MeshData:: MeshData::getInstance();
			_meshData->appendMeshKernal(k);
			_mainwindow->getSubWindowManager()->getPreWindow()->updateMeshActor();
			emit _mainwindow->updateMeshTreeSig();
			_mainwindow->updateActionsStates();
			QMessageBox::warning(this, tr("Title"), tr("Cut Completed !"));
		
		}
		QDialog::accept();
		this->close();

	}
}