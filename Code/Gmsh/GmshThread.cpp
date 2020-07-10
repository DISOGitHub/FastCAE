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
#include "IO/TemplateReplacer.h"
#include "MeshReader.h"

namespace Gmsh
{
	GmshThread::GmshThread(GUI::MainWindow* mw, MainWidget::PreWindow* pre, GmshModule* m, int dim)
		:_mainwindow(mw), _preWindow(pre), _gmshModule(m), _dim(dim)
	{

		connect(&_process, SIGNAL(readyReadStandardOutput()), this, SLOT(readProcessOutput()));
		connect(this, SIGNAL(sendMessage(QString)), mw, SIGNAL(printMessageToMessageWindow(QString)));
		connect(&_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
		
		_compounnd = new TopoDS_Compound;

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
		if (_compounnd != nullptr) delete _compounnd;
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

		if (_selectall) mergeAllGeo();
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

		auto k = new MeshData::MeshKernal();
		k->setName(QString("Mesh_%1").arg(k->getID()));
		k->setMeshData(dataset);
		data->appendMeshKernal(k);

// 		MeshReader reader(fileName);
// 		bool ok = reader.read();
// 		if (!ok) return;

		emit _gmshModule->updateMeshTree();
		emit _gmshModule->updateSetTree();
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

// 		QFile::copy(gmshDir + "gmsh.Geo", tempDir + "gmsh.Geo");
// 
// 		IO::TempalteReplacer replacer(this);
// 		replacer.appendFile(tempDir + "gmsh.Geo");
// 		replacer.replace();

		appendScript(tempDir);

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

		QString startProcess = QString("%1 %2 -format vtk -bin -o %3 -%4 ").arg(gmshexe).arg(tempDir + "gmsh.Geo").arg(tempDir + "mesh.vtk").arg(_dim);

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

	}

	void GmshThread::setSolid(QMultiHash<int, int> s)
	{
		_solidHash = s;
	}

	void GmshThread::setSurface(QMultiHash<int, int> s)
	{
		_surfaceHash = s;
	}

	void GmshThread::gridCoplanar(QTextStream* out)
	{
		if (!_isGridCoplanar)
			return;

// 		*out << "//+" << endl;
// 		*out << "BooleanFragments{ }{ }" << endl;
		if (_surfaceHash.size() != 0 || _solidHash.size() != 0)
		{
			QString script = getGridCoplanarScript().join("; ");
			*out << "//+" << endl;
			*out << "BooleanFragments{ " << script << "; Delete; }{ }" << endl;
		}
		*out << "//+" << endl;
		*out << "Coherence;" << endl;

	}

	void GmshThread::appendScript(QString dir)
	{
		QString path = dir + "gmsh.Geo";
		QString geo = dir + "geometry.brep";

		QFile file(path);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Append))
		{
			qDebug() << path << "open failed.";
			return;
		}
		QTextStream out(&file);

		
		out << QString("Merge \"%1\";").arg(geo) << endl;

		generalSetting(&out);

		out << "//+" << endl;
		out << "SetFactory(\"OpenCASCADE\");" << endl;

		sizeAtPoints(&out);
		sizeFields(&out);
		//physicalsGroup(&out);
		gridCoplanar(&out);

		file.close();
	}

	void GmshThread::generalSetting(QTextStream* out)
	{
		int type = 0;
		if (_elementType.toLower() == "quad") type = 1;
		else if (_elementType.toLower() == "hex") type = 2;

		*out << QString("Geometry.OCCFixDegenerated = %1;").arg(_geoclean) << endl;
		*out << QString("Geometry.OCCFixSmallEdges = %1;").arg(_geoclean) << endl;
		*out << QString("Geometry.OCCFixSmallFaces = %1;").arg(_geoclean) << endl;
		*out << QString("Geometry.OCCSewFaces = %1;").arg(_geoclean) << endl;

		*out << QString("Mesh.Algorithm = %1;").arg(_method) << endl;
		*out << QString("Mesh.Algorithm3D = %1;").arg(_method) << endl;
		*out << QString("Mesh.CharacteristicLengthFactor = %1;").arg(_sizeFactor) << endl;
		*out << QString("Mesh.CharacteristicLengthMin = %1;").arg(_minSize) << endl;
		*out << QString("Mesh.CharacteristicLengthMax = %1;").arg(_maxSize) << endl;
		*out << QString("Mesh.ElementOrder = %1;").arg(_elementOrder) << endl;
		*out << QString("Mesh.RecombineAll = %1;").arg(type) << endl;
		*out << QString("Mesh.Smoothing = %1;").arg(_smoothIteration) << endl;
		*out << QString("Mesh.SubdivisionAlgorithm = %1;").arg(type) << endl;

	}												  

	void GmshThread::sizeAtPoints(QTextStream* out)
	{
		if (_sizeAtPoints.isEmpty())
			return;

		int d = 100000;

		QStringList points = _sizeAtPoints.split(";");
		for (QString s : points)
		{
			QStringList coors = s.split(",");
			if (coors.size() != 4) continue;

			bool ok = false;
			for (QString c : coors)
			{
				c.toDouble(&ok);
				if (!ok) break;
			}

			if (ok)
			{
				*out << "//+" << endl;
				QString temp = QString("Point(%1) = {").arg(d);
				*out << temp << coors.join(",") << "};" << endl;
				d++;
			}
			
		}

	}

	void GmshThread::sizeFields(QTextStream* out)
	{
		if (_sizeFields.isEmpty())
			return;
		QStringList slist = _sizeFields.split(";");
		int index = 1;
		for (QString s : slist)
		{
			QStringList field = s.split(",");

			if (field.size() <=0) continue;

			bool ok = false;
			int t = field.at(0).toInt(&ok);
			if (!ok) continue;

			switch (t)
			{
			case 1:
				boxFieldScript(out, field,index);
				break;
			case 2:
				ballFieldScript(out, field,index);
				break;
			case 3:
				cylinderFieldScript(out, field,index);
				break;
			default:
				break;
			}
		}
	}

	void GmshThread::boxFieldScript(QTextStream* out, QStringList list, int& index)
	{
		const int n = list.size();
		if (n != 11)return;

		double val[9] = { 0 };
		bool ok = false;
		for (int i = 1; i < 10;i++)
		{
			double v = list.at(i).toDouble(&ok);
			if (!ok) return;
			val[i - 1] = v;
		}

		*out << "//+" << endl << QString("Field[%1] = Box;").arg(index) << endl;
		*out << "//+" << endl << QString("Field[%1].Thickness = %2;").arg(index).arg(val[0]) << endl;
		*out << "//+" << endl << QString("Field[%1].VIn = %2;").arg(index).arg(val[1]) << endl;
		*out << "//+" << endl << QString("Field[%1].VOut = %2;").arg(index).arg(val[2]) << endl;
		*out << "//+" << endl << QString("Field[%1].XMax = %2;").arg(index).arg(val[3]) << endl;
		*out << "//+" << endl << QString("Field[%1].XMin = %2;").arg(index).arg(val[4]) << endl;
		*out << "//+" << endl << QString("Field[%1].YMax = %2;").arg(index).arg(val[5]) << endl;
		*out << "//+" << endl << QString("Field[%1].YMin = %2;").arg(index).arg(val[6]) << endl;
		*out << "//+" << endl << QString("Field[%1].ZMax = %2;").arg(index).arg(val[7]) << endl;
		*out << "//+" << endl << QString("Field[%1].ZMin = %2;").arg(index).arg(val[8]) << endl;
		if (list.at(10).toInt() == 1)
			*out << "//+" << endl << QString("Background Field = %1;").arg(index) << endl;

		index++;
	}

	void GmshThread::ballFieldScript(QTextStream* out, QStringList list, int& index)
	{
		const int n = list.size();
		if (n != 9)return;

		double val[7] = { 0 };
		bool ok = false;
		for (int i = 1; i < 8; i++)
		{
			double v = list.at(i).toDouble(&ok);
			if (!ok) return;
			val[i - 1] = v;
		}

		*out << "//+" << endl << QString("Field[%1] = Ball;").arg(index) << endl;
		*out << "//+" << endl << QString("Field[%1].Radius = %2;").arg(index).arg(val[0]) << endl;
		*out << "//+" << endl << QString("Field[%1].Thickness = %2;").arg(index).arg(val[1]) << endl;
		*out << "//+" << endl << QString("Field[%1].VIn = %2;").arg(index).arg(val[2]) << endl;
		*out << "//+" << endl << QString("Field[%1].VOut = %2;").arg(index).arg(val[3]) << endl;
		*out << "//+" << endl << QString("Field[%1].XCenter = %2;").arg(index).arg(val[4]) << endl;
		*out << "//+" << endl << QString("Field[%1].YCenter = %2;").arg(index).arg(val[5]) << endl;
		*out << "//+" << endl << QString("Field[%1].ZCenter = %2;").arg(index).arg(val[6]) << endl;

		if (list.at(8).toInt() == 1)
			*out << "//+" << endl << QString("Background Field = %1;").arg(index) << endl;

		index++;
	}

	void GmshThread::cylinderFieldScript(QTextStream* out, QStringList list, int& index)
	{
		const int n = list.size();
		if (n != 11)return;

		double val[9] = { 0 };
		bool ok = false;
		for (int i = 1; i < 10; i++)
		{
			double v = list.at(i).toDouble(&ok);
			if (!ok) return;
			val[i - 1] = v;
		}

		*out << "//+" << endl << QString("Field[%1] = Cylinder;").arg(index) << endl;
		*out << "//+" << endl << QString("Field[%1].Radius = %2;").arg(index).arg(val[0]) << endl;
		*out << "//+" << endl << QString("Field[%1].VIn = %2;").arg(index).arg(val[1]) << endl;
		*out << "//+" << endl << QString("Field[%1].VOut = %2;").arg(index).arg(val[2]) << endl;
		*out << "//+" << endl << QString("Field[%1].XAxis = %2;").arg(index).arg(val[3]) << endl;
		*out << "//+" << endl << QString("Field[%1].XCenter = %2;").arg(index).arg(val[4]) << endl;
		*out << "//+" << endl << QString("Field[%1].YAxis = %2;").arg(index).arg(val[5]) << endl;
		*out << "//+" << endl << QString("Field[%1].YCenter = %2;").arg(index).arg(val[6]) << endl;
		*out << "//+" << endl << QString("Field[%1].ZAxis = %2;").arg(index).arg(val[7]) << endl;
		*out << "//+" << endl << QString("Field[%1].ZCenter = %2;").arg(index).arg(val[8]) << endl;

		if (list.at(10).toInt() == 1)
			*out << "//+" << endl << QString("Background Field = %1;").arg(index) << endl;

		index++;
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

	int GmshThread::getShapeIndexInCompound(int setid, int index, int itype)
	{
		int resindex = 0;
		TopAbs_ShapeEnum type;
		switch (itype)
		{
		case 1: type = TopAbs_VERTEX; break;
		case 2: type = TopAbs_EDGE; break;
		case 3: type = TopAbs_FACE; break;
		case 4: type = TopAbs_SOLID; break;
		default: break;
		}
		Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
		auto set = data->getGeometrySetByID(setid);
		if (set == nullptr) return resindex;
		TopoDS_Shape* shape = set->getShape(itype, index);
		TopTools_IndexedMapOfShape mapper;
		TopExp::MapShapes(*_compounnd, type, mapper);
		resindex = mapper.FindIndex(*shape);

		return resindex;
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

	QStringList GmshThread::getGridCoplanarScript()
	{
		QStringList script;
		Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
		QList<int> setList = _surfaceHash.uniqueKeys();
		for (int setid : setList)
		{
			Geometry::GeometrySet* set = data->getGeometrySetByID(setid);
			if (set == nullptr) continue;
			

			QList<int> indexList = _surfaceHash.values(setid);
			for (int faceindex : indexList)
			{
				script.append(QString("Surface{%1}").arg(getShapeIndexInCompound(setid, faceindex, 3)));
			}
		}

		setList = _solidHash.uniqueKeys();
		for (int setid : setList)
		{
			Geometry::GeometrySet* set = data->getGeometrySetByID(setid);
			if (set == nullptr) continue;
			

			QList<int> indexList = _solidHash.values(setid);
			for (int solidIndex : indexList)
			{
				script.append(QString("Volume{%1}").arg(getShapeIndexInCompound(setid, solidIndex, 4)));
			}
		}


		return script;
	}

}


