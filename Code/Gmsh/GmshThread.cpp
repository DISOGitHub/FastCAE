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
#include <TopExp.hxx>
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
//#include "IO/TemplateReplacer.h"
#include "MeshReader.h"
#include "geometry/GeoCommon.h"

#include "GmshSettingData.h"
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <string>
#include <GeomAPI_ProjectPointOnSurf.hxx>
#include <TopoDS.hxx>
#include <TopExp.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <vtkUnstructuredGrid.h>
#include <vtkPoints.h>
#include <vtkIdList.h>
#include "GeometryCommand/GeoCommandCommon.h"
#include "FluidMeshPreProcess.h"
#include "GmshScriptWriter.h"

namespace Gmsh
{
	GmshThread::GmshThread(GUI::MainWindow* mw, MainWidget::PreWindow* pre, GmshModule* m, int dim)
		:_mainwindow(mw), _preWindow(pre), _gmshModule(m), _dim(dim)
	{

		connect(&_process, SIGNAL(readyReadStandardOutput()), this, SLOT(readProcessOutput()));
		connect(this, SIGNAL(sendMessage(QString)), mw, SIGNAL(printMessageToMessageWindow(QString)));
		connect(&_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
		connect(this, SIGNAL(updateMeshActor()), _preWindow, SIGNAL(updateMeshActorSig()));

		_compounnd = new TopoDS_Compound;
		_fluidMeshProcess = new FluidMeshPreProcess();
		_scriptWriter = new GmshScriptWriter;
		//临时文件夹路径
// 		QString exelPath = QCoreApplication::applicationDirPath();
// 		const QString tempDir = exelPath + "/../temp/";
// 		DataProperty::ParameterString* s = new DataProperty::ParameterString();
// 		s->setDescribe("TempPath");
// 		s->setValue(tempDir);
// 		this->appendParameter(s);

		if (dim == 3) //三维补充光顺参数
		{
			_smoothIteration = 20;
// 			DataProperty::ParameterInt* sm = new DataProperty::ParameterInt();
// 			sm->setDescribe("Smooth");
// 			sm->setValue(20);
// 			this->appendParameter(sm);
		}
	}

	GmshThread::~GmshThread()
	{
		if (_compounnd != nullptr) delete _compounnd;
		if (_fluidMeshProcess != nullptr) delete _fluidMeshProcess;
		if (_scriptWriter != nullptr) delete _scriptWriter;
	}
	void GmshThread::appendSolid(int id, int index)
	{
		_solidHash.insert(id, index);
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

// 		DataProperty::ParameterInt* pi = new DataProperty::ParameterInt();
// 		pi->setDescribe("ElementType");
// 		pi->setValue(type);
// 		this->appendParameter(pi);

	}

	void GmshThread::setElementOrder(int order)
	{
		_elementOrder = order;

// 		DataProperty::ParameterInt* pi = new DataProperty::ParameterInt();
// 		pi->setDescribe("ElementOrder");
// 		pi->setValue(order);
// 		this->appendParameter(pi);
	}

	void GmshThread::setMethod(int m)
	{
		_method = m;

// 		DataProperty::ParameterInt* pi = new DataProperty::ParameterInt();
// 		pi->setDescribe("Method");
// 		pi->setValue(m);
// 		this->appendParameter(pi);
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
// 		DataProperty::ParameterDouble* pd = new DataProperty::ParameterDouble();
// 		pd->setDescribe("SizeMin");
// 		pd->setValue(min);
// 		this->appendParameter(pd);
	}

	void GmshThread::setMaxSize(double max)
	{
		_maxSize = max;
// 		DataProperty::ParameterDouble* pd = new DataProperty::ParameterDouble();
// 		pd->setDescribe("SizeMax");
// 		pd->setValue(max);
// 		this->appendParameter(pd);
	}

	void GmshThread::isCleanGeo(bool c)
	{
		_geoclean = c;
// 		int gc = 0;
// 		if (_geoclean)  gc = 1;

// 		DataProperty::ParameterInt* pi = new DataProperty::ParameterInt();
// 		pi->setDescribe("GeoClean");
// 		pi->setValue(gc);
// 		this->appendParameter(pi);
	}

	void GmshThread::setSmoothIteration(int it)
	{
		_smoothIteration = it;
// 		DataProperty::ParameterInt* sm = new DataProperty::ParameterInt();
// 		sm->setDescribe("Smooth");
// 		sm->setValue(it);
// 		this->appendParameter(sm);
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

		if (_fluidMesh) _fluidMeshProcess->mergeFluidField(_compounnd,_solidHash);
		else if (_selectall) mergeAllGeo();
		else if (_selectvisible) mergeVisibleGeo();
		else mergeSelectGeo();
		QByteArray arr = tempPath.toLatin1();
		BRepTools::Write(*_compounnd, arr.data());

	}

	void GmshThread::readMesh()
	{
 		MeshData::MeshData* data = MeshData::MeshData::getInstance();
 		QString exelPath = QCoreApplication::applicationDirPath();
 		const QString fileName = exelPath + "/../temp/mesh.vtk";

		QTextCodec *codec = QTextCodec::codecForName("GB18030");
		QByteArray ba = codec->fromUnicode(fileName);
		vtkSmartPointer<vtkDataSetReader> vtkReader = vtkSmartPointer<vtkDataSetReader>::New();
		vtkReader->SetFileName(ba);
		vtkReader->Update();
		vtkDataSet* dataset = vtkReader->GetOutput();
		if (dataset == nullptr) return;

		if (!_isSaveToKernal)
			emit writeToSolveFileSig(dataset);
		else
		{
// 			vtkDataSet* vtkset = nullptr;
// 			if (_cellTypeList.size() > 0)
// 				vtkset = deleteSpecifiedCells(dataset);

			auto k = new MeshData::MeshKernal();
			k->setName(QString("Mesh_%1").arg(k->getID()));

// 			if (vtkset != nullptr)
// 				k->setMeshData(vtkset);
// 			else
				k->setMeshData(dataset);

			data->appendMeshKernal(k);

			if (!_fluidMesh)
				setGmshSettingData(k);

			emit _gmshModule->updateMeshTree();
			emit _gmshModule->updateSetTree();
//			emit _preWindow->updateMeshActorSig();
			emit _gmshModule->updateActions();
			emit updateMeshActor();
		}

	}

	void GmshThread::initGmshEnvoirment()
	{
// 		_pyAgent->backstageExec("Mesher.initGmsh()");
// 		_pyAgent->backstageExec("Mesher.mergeGeoToGmsh()");
		QString exelPath = QCoreApplication::applicationDirPath();
		const QString tempDir = exelPath + "/../temp/";
		const QString gmshDir = exelPath + "/gmsh/";
		QFile::remove(tempDir + "gmsh.Geo");

// 		QFile::copy(gmshDir + "gmsh.Geo", tempDir + "gmsh.Geo");
// 
// 		IO::TempalteReplacer replacer(this);
// 		replacer.appendFile(tempDir + "gmsh.Geo");
// 		replacer.replace();
// 		if (_fluidMesh)
// 			writeFluidMeshScript(tempDir);
// 		else
// 			appendScript(tempDir);
		_scriptWriter->setCompound(_compounnd);
		setGmshScriptData();
		
		if (_fluidMesh)
		{
			QList<int> curve = _fluidMeshProcess->getInerMember(1);
			QList<int> surface = _fluidMeshProcess->getInerMember(2);
			_scriptWriter->writeFluidMeshScript(tempDir, _solidHash, curve, surface);
		}
		else
			_scriptWriter->writeGmshScript(tempDir);
				
			

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
		this->setSolid(para->_solidHash);
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
		this->setGridCoplanar(para->_isGridCoplanar);
		this->setSizeAtPoint(para->_sizeAtPoints);
		this->setSizeFields(para->_sizeFields);
		//this->setPhysicals(para->_physicals);
 		this->setSelectedAll(para->_selectall);
 		this->setSelectedVisible(para->_selectvisible);
 		this->setMeshID(para->_meshID);
 		this->setFluidMesh(para->_fluidMesh);
		//this->setCellTypeList(para->_cells);
	//	this->setFluidField(para->_fluidField);

		_fluidMeshProcess->setFluidField(para->_fluidField);
		
		//this->setPhysicals(para->_physicals);
		
		//_scriptWriter->setFluidField(para->_fluidField);
	}

	void GmshThread::setSolid(QMultiHash<int, int> s)
	{
		_solidHash = s;
	}

	void GmshThread::setSurface(QMultiHash<int, int> s)
	{
		_surfaceHash = s;
	}

// 	void GmshThread::physicalsGroup(QTextStream* out)
// 	{
// 		if (_physicals.isEmpty())
// 			return;
// 
// 		QMultiHash<QString,int> points{};
// 		QMultiHash<QString,int> curves{};
// 		QMultiHash<QString,int> surfaces{};
// 		QMultiHash<QString,int> solids{};
// 
// 		QStringList glist = _physicals.split(";");
// 		for (QString g : glist)
// 		{
// 			QStringList plist = g.split(",");
// 			if (plist.size() != 4)
// 				continue;
// 
// 			bool b = false;
// 			int type = plist.at(0).toInt(&b);
// 			if (!b) continue;
// 			if (type <= 0 || type > 4) continue;
// 			int set = plist.at(2).toInt(&b);
// 			if (!b) continue; 
// 			int d = plist.at(3).toInt(&b);
// 			if (!b) continue;
// 
// 			int index = getShhapeIndexInCompound(set, d, type);
// 			if (index == 0) continue;
// 
// 			switch (type)
// 			{
// 			case 1:
// 				points.insert(plist.at(1), index);
// 				break;
// 			case 2:
// 				curves.insert(plist.at(1), index);
// 				break;
// 			case 3:
// 				surfaces.insert(plist.at(1), index);
// 				break;
// 			case 4:
// 				solids.insert(plist.at(1), index);
// 				break;
// 			}
// 		}
// 
// 		if (points.size() > 0)
// 			physicalsScript(out, "Point", points);
// 		if (curves.size() > 0)
// 			physicalsScript(out, "Curve", curves);
// 		if (surfaces.size() > 0)
// 			physicalsScript(out, "Surface", surfaces);
// 		if (solids.size() > 0)
// 			physicalsScript(out, "Volume", solids);
// 
// 
// 	}
// 
// 	void GmshThread::physicalsScript(QTextStream* out, QString type, QMultiHash<QString, int> pHash)
// 	{
// 		QList<QString> namelist = pHash.uniqueKeys();
// 		for (QString name : namelist)
// 		{
// 			QList<int> indexlist = pHash.values(name);
// 			QStringList list{};
// 			for (int index : indexlist)
// 			{
// 				list.append(QString::number(index));
// 			}
// 			QString slist = list.join(",");
// 
// 			*out << "//+" << endl;
// 			*out << "Physical " << type << "(\"" << name << "\") = {" << slist << "};" << endl;
// 		}
// 	}

	void GmshThread::setGridCoplanar(bool gc)
	{
		_isGridCoplanar = gc;
	}

	void GmshThread::setSizeAtPoint(QString ps)
	{
		_sizeAtPoints = ps;
	}


	void GmshThread::setSizeFields(QString fs)
	{
		_sizeFields = fs;
	}

	void GmshThread::setMeshID(int id)
	{
		_meshID = id;
	}

// 	void GmshThread::setPhysicals(QString ps)
// 	{
// 		_physicals = ps;
// 	}

	void GmshThread::setSelectedAll(bool al)
	{
		_selectall = al;
	}

	void GmshThread::setSelectedVisible(bool sv)
	{
		_selectvisible = sv;
	}

	void GmshThread::setFluidMesh(bool fm)
	{
		_fluidMesh = fm;
	}


	void GmshThread::setCellTypeList(QString cells)
	{
		QStringList celllist = cells.split(",");
		for (QString cell : celllist)
		{
			bool ok = false;
			int type = cell.toInt(&ok);
			if (!ok)continue;
			_cellTypeList.append(type);
		}
	}

	void GmshThread::mergeAllGeo()
	{
		BRep_Builder aBuilder;
		aBuilder.MakeCompound(*_compounnd);

		Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
		const int nset = data -> getGeometrySetCount();
		for (int i = 0; i < nset; ++i)
		{
			auto gset = data->getGeometrySetAt(i);
			TopoDS_Shape* shape = gset->getShape();
			if (shape == nullptr) continue;
			aBuilder.Add(*_compounnd, *shape);
		}
	}

	void GmshThread::mergeVisibleGeo()
	{
		BRep_Builder aBuilder;
		aBuilder.MakeCompound(*_compounnd);

		Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
		const int nset = data->getGeometrySetCount();
		for (int i = 0; i < nset; ++i)
		{
			auto gset = data->getGeometrySetAt(i);
			if (!gset->isVisible()) continue;
			TopoDS_Shape* shape = gset->getShape();
			if (shape == nullptr) continue;
			aBuilder.Add(*_compounnd, *shape);
		}
	}

	void GmshThread::mergeSelectGeo()
	{
		BRep_Builder aBuilder;
		aBuilder.MakeCompound(*_compounnd);

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
				aBuilder.Add(*_compounnd, faceShape);
			}
		}

		setList = _solidHash.uniqueKeys();
		for (int setid : setList)
		{
			Geometry::GeometrySet* set = data->getGeometrySetByID(setid);
			if (set == nullptr) continue;
			TopoDS_Shape* shape = set->getShape();
			if (shape == nullptr) continue;

			QList<int> indexList = _solidHash.values(setid);
			for (int solidIndex : indexList)
			{
				TopExp_Explorer solidexp(*shape, TopAbs_SOLID);
				for (int index = 0; index < solidIndex && solidexp.More(); solidexp.Next(), ++index);

				const TopoDS_Shape& solidshape = solidexp.Current();
				aBuilder.Add(*_compounnd, solidshape);
			}
		}
	}

	

// 	QList<int> GmshThread::getShapeIndexListInFluidField(int itype)
// 	{
// 		QList<int> indexList{};
// 
// 		QList<gp_Pnt> pntlist{};
// 		//gp_Pnt pt0(-50, -50, -50); gp_Pnt pt1(50, 50, 50);
// 		gp_Pnt pt0(_fluidField[0][0], _fluidField[0][1], _fluidField[0][2]); gp_Pnt pt1(_fluidField[1][0], _fluidField[1][1], _fluidField[1][2]);
// 		TopoDS_Shape bigBox = BRepPrimAPI_MakeBox(pt0, pt1).Shape();
// 		if (bigBox.IsNull()) return indexList;
// 		QList<Handle(TopoDS_TShape)> tshapelist;
// 		TopExp_Explorer explor(bigBox, TopAbs_VERTEX);
// 		for (; explor.More(); explor.Next())
// 		{
// 			const TopoDS_Shape& oneshape = explor.Current();
// 			Handle(TopoDS_TShape) ts = oneshape.TShape();
// 			if (tshapelist.contains(ts)) continue;
// 			tshapelist.append(ts);
// 			const TopoDS_Vertex& vertex = TopoDS::Vertex(oneshape);
// 			gp_Pnt pt = BRep_Tool::Pnt(vertex);
// 			pntlist.append(pt);
// 		}
// 
// 		TopAbs_ShapeEnum type;
// 		switch (itype)
// 		{
// 		case 1: type = TopAbs_VERTEX; break;
// 		case 2: type = TopAbs_EDGE; 
// 			return indexList = getInerEdgeIndexs(pntlist);
// 			break;
// 		case 3: type = TopAbs_FACE; 
// 			return indexList = getInerFaceIndexs(pntlist);
// 			break;
// 		case 4: type = TopAbs_SOLID; break;
// 		default: break;
// 		}
// 
// 		
// 	}
// 
// 	QList<int> GmshThread::getInerFaceIndexs(QList<gp_Pnt> pntlist)
// 	{
// 		QList<int> indexList{};
// 		TopExp_Explorer exp(*_compounnd, TopAbs_FACE);
// 		for (; exp.More(); exp.Next())
// 		{
// 			const TopoDS_Shape& faceShape = exp.Current();
// 			TopoDS_Face face = TopoDS::Face(faceShape);
// 			const Handle(Geom_Surface) geosurface = BRep_Tool::Surface(face);
// 			bool isOnBox{ false };
// 			for (gp_Pnt pt : pntlist)
// 			{
// 				GeomAPI_ProjectPointOnSurf project(pt, geosurface);
// 				double distance = project.LowerDistance();
// 				if (distance < 1e-6) { isOnBox = true; break; }
// 			}
// 			if (!isOnBox)
// 			{
// 				TopTools_IndexedMapOfShape mapper;
// 				TopExp::MapShapes(*_compounnd, TopAbs_FACE, mapper);
// 				int resindex = mapper.FindIndex(faceShape);
// 				indexList.push_back(resindex);
// 			}
// 		}
// 		return indexList;
// 	}

// 	QList<int> GmshThread::getInerEdgeIndexs(QList<gp_Pnt> pntlist)
// 	{
// 		QList<int> indexList{};
// 		TopExp_Explorer exp(*_setCompound, TopAbs_SOLID);
// 		TopoDS_Shape fauShape{};
// 		for (; exp.More();exp.Next())
// 		{
// 			BRepAlgoAPI_Fuse fau(fauShape, exp.Current());
// 
// 			if (!fau.IsDone()) return indexList;
// 			const TopoDS_Shape& aFusedShape = fau.Shape();
// 
// 			if (aFusedShape.IsNull()) return indexList;
// 			//TopoDS_Shape shape = Command::GeoCommandCommon::removeSplitter(aFusedShape);
// 			fauShape = aFusedShape;
// 		}
// 		const char* ch = "D://FusedShape.brep";
// 		BRepTools::Write(fauShape, ch);
// 
// 
// 		return indexList;
// 	}



	QList<itemInfo> GmshThread::generateGeoIds(vtkDataSet* dataset)
	{
		QList<itemInfo> infoList{};
		//QMultiHash<int, QList<int>> elementHash{};
		if (_surfaceHash.size() < 1 || dataset == nullptr) return infoList;

		Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
		QMultiHash<int, int>::iterator it = _surfaceHash.begin();
		for (; it != _surfaceHash.end(); it++)
		{
			if (it.key() < 0||it.value() < 0) continue;
			TopoDS_Compound aRes;
			BRep_Builder aBuilder;
			aBuilder.MakeCompound(aRes);
			Geometry::GeometrySet* set = data->getGeometrySetByID(it.key());
			TopoDS_Shape* body = set->getShape();

			TopExp_Explorer exper(*body, TopAbs_FACE);
			for (int index = 0; index < it.value() && exper.More(); exper.Next(), ++index);
			const TopoDS_Shape& s = exper.Current();
			aBuilder.Add(aRes, s);
			QList<int> inids{};
			if (!aRes.IsNull())	inids = GeoCommon::getD2ElementsInShape(dataset, &aRes);

			infoList.append(itemInfo{ it.key(), it.value(), inids });
			//elementHash.insert(it.key(), inids);
		}
		return infoList;


		/*QList<int> geoList = _surfaceHash.uniqueKeys();
		for (int setid : geoList)
		{
			if (setid < 0) continue;
			TopoDS_Compound aRes;
			BRep_Builder aBuilder;
			aBuilder.MakeCompound(aRes);
			Geometry::GeometrySet* set = data->getGeometrySetByID(setid);
			TopoDS_Shape* body = set->getShape();
			QList<int> member = _surfaceHash.values(setid);
			for (int m : member)
			{
				if (m < 0) continue;
				TopExp_Explorer exper(*body, TopAbs_FACE);
				for (int index = 0; index < m && exper.More(); exper.Next(), ++index);
				const TopoDS_Shape& s = exper.Current();
				aBuilder.Add(aRes, s);
			}
			QList<int> inids;
			if (!aRes.IsNull())	inids = GeoCommon::getD2ElementsInShape(dataset, &aRes);
			elementHash.insert(setid, inids);
		}*/
		
	}



	void GmshThread::isSaveDataToKernal(bool save)
	{
		_isSaveToKernal = save;
	}

	void GmshThread::setGmshSettingData(MeshData::MeshKernal* k)
	{
		GmshSettingData* setting = new GmshSettingData;
		setting->setID(_dim);
		setting->setSolidHash(_solidHash);
		setting->setSurfaceHash(_surfaceHash);
		setting->setElementOrder(_elementOrder);
		setting->setElementType(_elementType);
		setting->setGeoClean(_geoclean);
		setting->setGridCoplanar(_isGridCoplanar);
		setting->setMaxSize(_maxSize);
		setting->setMinSize(_minSize);
		setting->setSizeFactor(_sizeFactor);
		setting->setSelectAll(_selectall);
		setting->setSelectVisiable(_selectvisible);
		setting->setSmoothIteration(_smoothIteration);
		setting->setMethod(_method);
		setting->setSizeAtPoints(_sizeAtPoints);
		setting->setSizeFields(_sizeFields);
		setting->setMeshID(_meshID);
		setting->setCells(_cellTypeList);

		k->setGmshSetting(setting);
	}


	void GmshThread::setGmshScriptData()
	{
		_scriptWriter->setSmooth(_smoothIteration);
		_scriptWriter->setElementType(_elementType);
		_scriptWriter->setElementOrder(_elementOrder);
		_scriptWriter->setMethod(_method);
		_scriptWriter->setMinSize(_minSize);
		_scriptWriter->setMaxSize(_maxSize);
		_scriptWriter->setFactor(_sizeFactor);
		_scriptWriter->setGeoClean(_geoclean);
		_scriptWriter->setGridCoplanar(_isGridCoplanar);
		_scriptWriter->setSizePoints(_sizeAtPoints);
		_scriptWriter->setSizeFields(_sizeFields);
	}

	vtkDataSet* GmshThread::deleteSpecifiedCells(vtkDataSet* dataset)
	{
		if (dataset == nullptr)return dataset;
		int index = 0;
		vtkUnstructuredGrid* ung = vtkUnstructuredGrid::New();
		vtkPoints* points = vtkPoints::New();

		const int nNode = dataset->GetNumberOfPoints();
		for (int i = 0; i < nNode;i++)
		{
			double* coor = dataset->GetPoint(i);
			points->InsertNextPoint(coor);
		}
		ung->SetPoints(points);

		const int ncell = dataset->GetNumberOfCells();
		for (int i = 0; i < ncell; i++)
		{
			vtkCell* cell = dataset->GetCell(i);
			if (cell == nullptr) continue;
			VTKCellType type = (VTKCellType)cell->GetCellType();
			vtkIdList* idlist = vtkIdList::New();
			if (isSpecifiedCell(type))
			{
				idlist = cell->GetPointIds();
				ung->InsertNextCell(type, idlist);
			}			
		}

		return ung;
	}


	bool GmshThread::isSpecifiedCell(VTKCellType type)
	{
		int flag = -1;
		switch (type)
		{
		case VTKCellType::VTK_VERTEX:
		case VTKCellType::VTK_POLY_VERTEX:
			flag = 0;
			break;
		case VTKCellType::VTK_LINE:
		case VTKCellType::VTK_POLY_LINE:
		case VTKCellType::VTK_QUADRATIC_EDGE:
			flag = 1;
			break;
		case VTKCellType::VTK_TRIANGLE:
		case VTKCellType::VTK_QUAD:
		case VTKCellType::VTK_TRIANGLE_STRIP:
		case VTKCellType::VTK_PIXEL:
		case VTKCellType::VTK_POLYGON:
		case VTKCellType::VTK_QUADRATIC_TRIANGLE:
		case VTKCellType::VTK_QUADRATIC_LINEAR_QUAD:
		case VTKCellType::VTK_QUADRATIC_QUAD:
		case VTKCellType::VTK_BIQUADRATIC_QUAD:
			flag = 2;
			break;
		case VTKCellType::VTK_TETRA:
		case VTKCellType::VTK_HEXAHEDRON:
		case VTKCellType::VTK_VOXEL:
		case VTKCellType::VTK_WEDGE:
		case VTKCellType::VTK_PYRAMID:
		case VTKCellType::VTK_PENTAGONAL_PRISM:
		case VTKCellType::VTK_HEXAGONAL_PRISM:
		case VTKCellType::VTK_QUADRATIC_TETRA:
		case VTKCellType::VTK_QUADRATIC_PYRAMID:
		case VTKCellType::VTK_QUADRATIC_HEXAHEDRON:
		case VTKCellType::VTK_BIQUADRATIC_QUADRATIC_HEXAHEDRON:
		case VTKCellType::VTK_TRIQUADRATIC_HEXAHEDRON:
		case VTKCellType::VTK_QUADRATIC_LINEAR_WEDGE:
		case VTKCellType::VTK_QUADRATIC_WEDGE:
		case VTKCellType::VTK_BIQUADRATIC_QUADRATIC_WEDGE:
			flag = 3;
			break;
		default:
			break;
		}

		if (_cellTypeList.contains(flag))
			return true;

		return false;
	}

}


