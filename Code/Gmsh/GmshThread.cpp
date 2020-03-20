#include "GmshThread.h"
#include "python/PyAgent.h"
#include "GmshPy.h"
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include <QApplication>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <QDir>
#include <QMessageBox>
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
#include "GmshModule.h"
#include "moduleBase/processBar.h"

#include "DataProperty/ParameterString.h"
#include "DataProperty/ParameterInt.h"
#include "DataProperty/ParameterDouble.h"
#include "IO/TemplateReplacer.h"

namespace Gmsh
{
	GmshThread::GmshThread(GUI::MainWindow* mw, MainWidget::PreWindow* pre, GmshModule* m, int dim)
		:_mainwindow(mw), _preWindow(pre), _gmshModule(m), _dim(dim)
	{

		connect(&_process, SIGNAL(readyReadStandardOutput()), this, SLOT(readProcessOutput()));
		connect(this, SIGNAL(sendMessage(QString)), mw, SIGNAL(printMessageToMessageWindow(QString)));
		connect(&_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
		
		//临时文件夹路径
		QString exelPath = QCoreApplication::applicationDirPath();
		const QString tempDir = exelPath + "/../temp/";
		DataProperty::ParameterString* s = new DataProperty::ParameterString();
		s->setDescribe("TempPath");
		s->setValue(tempDir);
		this->appendParameter(s);

		if (dim == 3) //三维补充光顺参数
		{
			DataProperty::ParameterInt* sm = new DataProperty::ParameterInt();
			sm->setDescribe("Smooth");
			sm->setValue(20);
			this->appendParameter(sm);
		}


		connect(this, SIGNAL(finished()), this, SLOT(processFinished()));

		
	}

	GmshThread::~GmshThread()
	{

	}
	void GmshThread::appendSolid(int id)
	{
		_solidList.append(id);
	}

	void GmshThread::appendSurface(int geo, int face)
	{
		_surfaceHash.insert(geo, face);
	}

	void GmshThread::setElementType(QString t)
	{
		_elementType = t;
		int type = 0;
		if (t.toLower() == "quad") type = 1;
		else if (t.toLower() == "hex") type = 2;

		DataProperty::ParameterInt* pi = new DataProperty::ParameterInt();
		pi->setDescribe("ElementType");
		pi->setValue(type);
		this->appendParameter(pi);

	}

	void GmshThread::setElementOrder(int order)
	{
		_elementOrder = order;

		DataProperty::ParameterInt* pi = new DataProperty::ParameterInt();
		pi->setDescribe("ElementOrder");
		pi->setValue(order);
		this->appendParameter(pi);
	}

	void GmshThread::setMethod(int m)
	{
		_method = m;

		DataProperty::ParameterInt* pi = new DataProperty::ParameterInt();
		pi->setDescribe("Method");
		pi->setValue(m);
		this->appendParameter(pi);
	}

	void GmshThread::setSizeFactor(double f)
	{
		_sizeFactor = f;
		DataProperty::ParameterDouble* pd = new DataProperty::ParameterDouble();
		pd->setDescribe("SizeFactor");
		pd->setValue(f);
		this->appendParameter(pd);
	}

	void GmshThread::setMinSize(double min)
	{
		_minSize = min;
		DataProperty::ParameterDouble* pd = new DataProperty::ParameterDouble();
		pd->setDescribe("SizeMin");
		pd->setValue(min);
		this->appendParameter(pd);
	}

	void GmshThread::setMaxSize(double max)
	{
		_maxSize = max;
		DataProperty::ParameterDouble* pd = new DataProperty::ParameterDouble();
		pd->setDescribe("SizeMax");
		pd->setValue(max);
		this->appendParameter(pd);
	}

	void GmshThread::isCleanGeo(bool c)
	{
		_geoclean = c;
		int gc = 0;
		if (_geoclean)  gc = 1;

		DataProperty::ParameterInt* pi = new DataProperty::ParameterInt();
		pi->setDescribe("GeoClean");
		pi->setValue(gc);
		this->appendParameter(pi);
	}

	void GmshThread::setSmoothIteration(int it)
	{
		_smoothIteration = it;
		DataProperty::ParameterInt* sm = new DataProperty::ParameterInt();
		sm->setDescribe("Smooth");
		sm->setValue(it);
		this->appendParameter(sm);
	}

	void GmshThread::run()
	{
		this->mergeGeometry();
		this->initGmshEnvoirment();
 		this->generate();
// 		this->readMesh();
	}

	void GmshThread::mergeGeometry()
	{
		QString exelPath = QCoreApplication::applicationDirPath();
		const QString tempDir = exelPath + "/../temp/";
		QDir dir(tempDir);
		if (!dir.exists()) dir.mkpath(tempDir);

		const QString meshfilename = exelPath + "/../temp/mesh.vtk";
		if (QFile::exists(meshfilename)) QFile::remove(meshfilename);

		const QString geofilename = exelPath + "/../temp/geometry.brep";
		if (QFile::exists(geofilename)) QFile::remove(geofilename);

		const QString gmshfilename = exelPath + "/../temp/gmsh.Geo";
		if (QFile::exists(gmshfilename)) QFile::remove(gmshfilename);

		const QString tempPath = tempDir + QString("geometry.brep");

		TopoDS_Compound aRes;
		BRep_Builder aBuilder;
		aBuilder.MakeCompound(aRes);

		Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
		QList<int> setList = _surfaceHash.uniqueKeys();
		for (int setid : setList)
		{
			Geometry::GeometrySet* set = data->getGeometrySetByID(setid);
			if (set == nullptr) continue;
			TopoDS_Shape* shape = set->getShape();
			if (shape == nullptr) continue;

			QList<int> indexList = _surfaceHash.values(setid);
			for (int faceindex : indexList)
			{
				TopExp_Explorer faceExp(*shape, TopAbs_FACE);
				for (int index = 0; index < faceindex && faceExp.More(); faceExp.Next(), ++index);

				const TopoDS_Shape& faceShape = faceExp.Current();
				aBuilder.Add(aRes, faceShape);
			}
		}

		for (int id : _solidList)
		{
			Geometry::GeometrySet* set = data->getGeometrySetByID(id);
			if (set == nullptr) continue;
			TopoDS_Shape* shape = set->getShape();
			if (shape == nullptr) continue;
			aBuilder.Add(aRes, *shape);
		}

		QByteArray arr = tempPath.toLatin1();
		BRepTools::Write(aRes, arr.data());
	}

	void GmshThread::readMesh()
	{
		MeshData::MeshData* data = MeshData::MeshData::getInstance();
		QString exelPath = QCoreApplication::applicationDirPath();
		const QString filename = exelPath + "/../temp/mesh.vtk";

		QTextCodec *codec = QTextCodec::codecForName("GB18030");
		QByteArray ba = codec->fromUnicode(filename);
		vtkSmartPointer<vtkDataSetReader> vtkReader = vtkSmartPointer<vtkDataSetReader>::New();
		vtkReader->SetFileName(ba);
		vtkReader->Update();
		vtkDataSet* dataset = vtkReader->GetOutput();
		if (dataset == nullptr) return;

		auto k = new MeshData::MeshKernal();
		k->setName(QString("Mesh_%1").arg(k->getID()));
		k->setMeshData(dataset);
		data->appendMeshKernal(k);

		emit  _gmshModule->updateMeshTree();
		emit _preWindow->updateMeshActorSig();
		emit _gmshModule->updateActions();
	}

	void GmshThread::initGmshEnvoirment()
	{
// 		_pyAgent->backstageExec("Mesher.initGmsh()");
// 		_pyAgent->backstageExec("Mesher.mergeGeoToGmsh()");
		QString exelPath = QCoreApplication::applicationDirPath();
		const QString tempDir = exelPath + "/../temp/";
		const QString gmshDir = exelPath + "/gmsh/";
		QFile::remove(tempDir + "gmsh.Geo");

		QFile::copy(gmshDir + "gmsh.Geo", tempDir + "gmsh.Geo");

		IO::TempalteReplacer replacer(this);
		replacer.appendFile(tempDir + "gmsh.Geo");
		replacer.replace();
	}

	void GmshThread::generate()
	{
// 		_pyAgent->backstageExec(QString("gmsh.model.mesh.generate(%1)").arg(_dim));
// 		_pyAgent->backstageExec(QString("Mesher.finalizeGmsh()"));
		QString exelPath = QCoreApplication::applicationDirPath();
		const QString tempDir = exelPath + "/../temp/";
		const QString gmshDir = exelPath + "/gmsh/";
		QString gmshexe = exelPath + "/gmsh/gmsh";

		bool ok = false;
#ifdef Q_OS_WIN
		ok = QFile::exists(gmshexe + ".exe");
#endif
#ifdef Q_OS_LINUX
		ok = QFile::exists(gmshexe);
#endif
		if (!ok)
		{
			QMessageBox::warning(_mainwindow, QString(tr("Warning")), QString(tr("Gmsh is not exist !")));
			return;
		}

// 		QString oldDir = QDir::currentPath();
// 		QDir::setCurrent(gmshDir);

		QString startProcess = QString("%1 %2 -format vtk -bin -o %3 -%4").arg(gmshexe).arg(tempDir + "gmsh.Geo").arg(tempDir + "mesh.vtk").arg(_dim);

		if (gmshexe.contains(" "))  startProcess = QString("\"%1\"").arg(startProcess);
		qDebug() << startProcess;

		_process.start(startProcess);

	}



	void GmshThread::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
	{
		switch (exitStatus)
		{
		case QProcess::NormalExit:
			emit sendMessage("************************************");
			emit sendMessage("********* Gmsh finished ************");
			emit sendMessage("************************************");
			
			break;
		case QProcess::CrashExit:
			emit sendMessage("************************************");
			emit sendMessage("********* Gmsh Crashed ************");
			emit sendMessage("************************************");
			break;
		default:
			emit sendMessage("***********************************");
			emit sendMessage("********* Gmsh Stopped ************");
			emit sendMessage("***********************************");
			break;
		}

		readMesh();
// 		if (this->isRunning())
// 			this->wait();
 		emit threadFinished(this);
		Py::PythonAagent::getInstance()->unLock();
	}

	void GmshThread::readProcessOutput()
	{
		QString outputBuffer = QString::fromLocal8Bit(_process.readAll());

		emit sendMessage(outputBuffer);
	}

	void GmshThread::stop()
	{
		_process.kill();
	}

	void GmshThread::setPara(GMshPara* para)
	{
		this->setSolid(para->_solidList);
		this->setSurface(para->_surfaceHash);

		if (_dim == 2)
			this->setSmoothIteration(para->_smoothIteration);
		this->setElementType(para->_elementType);
		this->setElementOrder(para->_elementOrder);
		this->setMethod(para->_method);
		this->setMinSize(para->_minSize);
		this->setMaxSize(para->_maxSize);
		this->setSizeFactor(para->_sizeFactor);
		this->isCleanGeo(para->_geoclean);

	}

	void GmshThread::setSolid(QList<int> s)
	{
		_solidList = s;
	}

	void GmshThread::setSurface(QMultiHash<int, int> s)
	{
		_surfaceHash = s;
	}


}


