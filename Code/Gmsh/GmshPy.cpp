#include "GmshPy.h"
#include "GmshModule.h"
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include <QApplication>
#include <QList>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <QDir>
#include <QTextCodec>
#include <vtkDataSetReader.h>
#include <vtkCell.h>
#include "meshData/meshKernal.h"
#include "meshData/meshSingleton.h"
#include <vtkSmartPointer.h>
#include <vtkDataSet.h>
#include <vtkUnstructuredGrid.h>
#include <QDebug>
#include "mainWindow/mainWindow.h"
#include "mainWindow/SubWindowManager.h"
#include "MainWidgets/preWindow.h"
#include "python/PyAgent.h"
#include "GmshThread.h"
#include "meshData/meshCommon.h"

namespace Gmsh
{
	GUI::MainWindow* GmshPy::_mainWindow = nullptr;
	MainWidget::PreWindow* GmshPy::_preWindow = nullptr;
	GmshModule* GmshPy::_gmshModule = nullptr;

	void GmshPy::init(GmshModule* gmshModule, GUI::MainWindow* m)
	{
		_gmshModule = gmshModule;
		_mainWindow = m;
	}

	void GmshPy::setPreWindow(MainWidget::PreWindow* pre)
	{
		_preWindow = pre;
	}

	vtkDataSet* GmshPy::meshFilter(vtkDataSet* dataset, int dim)
	{
		vtkUnstructuredGrid* g = vtkUnstructuredGrid::New();
		vtkPoints* points = vtkPoints::New();
		g->SetPoints(points);
		
		const int npoint = dataset->GetNumberOfPoints();
		for (int i = 0; i < npoint; ++i)
		{
			double* coor = dataset->GetPoint(i);
			points->InsertNextPoint(coor);
		}

		const int ncell = dataset->GetNumberOfCells();
		for (int i = 0; i < ncell; ++i)
		{
			vtkCell* cell = dataset->GetCell(i);
			int cellType = cell->GetCellType();
			int dd = VTKCellTypeToDim(cellType);
			qDebug()<<cellType<<"   " << dd;
			if (dim != dd) continue;
			vtkIdList* ids = cell->GetPointIds();
			g->InsertNextCell(cellType, ids);
		}
		return g;
	}



	void GmshPy::GenerateMesh3D(char* solids, char* type, int order, int method, double factor, 
		double sizemin, double sizemax, bool clean, bool isGridCoplanar, char* points)
	{
		const QString solidBodys(solids);
		const QString eleType(type);
		const QStringList sSolids = solidBodys.split(",");
		const QString pointSizes(points);

		GMshPara* p = new GMshPara;

		for (QString s :  sSolids)
		{
			bool ok = false;
			int id = s.toInt(&ok);
			if (ok) p->_solidList.append(id);
		}
		p->_elementType = eleType;
		p->_elementOrder = order;
		p->_method = method;
		p->_sizeFactor = factor;
		p->_minSize = sizemin;
		p->_maxSize = sizemax;
		p->_geoclean = clean;
		p->_dim = 3;
		p->_isGridCoplanar = isGridCoplanar;
		p->_sizeAtPoints = pointSizes;

		emit _gmshModule->generateSig(p);
	}

	void GmshPy::GenerateMesh2D(char* solids, char* type, int order, int method, int smooth, double factor, double sizemin, double sizemax, bool clean, bool isGridCoplanar, char* points)
	{
		const QString surfaces(solids);
		const QString eleType(type);
		const QString pointSizes(points);
		qDebug() << surfaces;

		GMshPara* p = new GMshPara;
		const QStringList bodySurface = surfaces.split(";");
		for (QString bodyset : bodySurface)
		{
			QStringList faces = bodyset.split(":");
			int setid = faces.at(0).toInt();
			QStringList surfaceList = faces.at(1).split(",");
			for (QString sindex : surfaceList)
			{
				int index = sindex.toInt();
				p->_surfaceHash.insert(setid, index);
			}
		}
		p->_elementType = eleType;
		p->_smoothIteration = smooth;
		p->_elementOrder = order;
		p->_method = method;
		p->_sizeFactor = factor;
		p->_minSize = sizemin;
		p->_maxSize = sizemax;
		p->_geoclean = clean;
		p->_dim = 2;
		p->_isGridCoplanar = isGridCoplanar;
		p->_sizeAtPoints = pointSizes;

		emit _gmshModule->generateSig(p);
	}

}

void GenerateMesh3D(char* solids, char* type, int order, int method, double factor, 
	double sizemin, double sizemax, bool clean, bool isGridCoplanar, char* points)
{
	Gmsh::GmshPy::GenerateMesh3D(solids, type, order, method, factor, sizemin, sizemax, clean, isGridCoplanar, points);
}

void GenerateMesh2D(char* solids, char* type, int order, int method, int smooth, double factor, 
	double sizemin, double sizemax, bool clean, bool isGridCoplanar, char* points)
{
	Gmsh::GmshPy::GenerateMesh2D(solids, type, order, method, smooth, factor, sizemin, sizemax, clean, isGridCoplanar, points);
}
