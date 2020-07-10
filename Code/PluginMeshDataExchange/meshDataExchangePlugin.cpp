#include "meshDataExchangePlugin.h"
#include "CGNSdataExchange.h"
#include "CNTMdataExchange.h"
#include "INPdataExchange.h"
#include "MSHdataExchange.h"
#include "NEUdataExchange.h"
#include "VTKdataExchange.h"
#include "SU2dataExchange.h"
#include "KEYdataExchange.h"
#include "IO/IOConfig.h"
#include "ModelData/modelDataBase.h"
#include "moduleBase/ThreadTaskManager.h"
#include "PDBdataExchange.h"
#include <QFileInfo>

GUI::MainWindow* MeshData::MeshDataExchangePlugin::_mainwindow = nullptr;
namespace MeshData
{
	MeshDataExchangePlugin::MeshDataExchangePlugin(GUI::MainWindow* m)
	{		
		_describe = "MeshDataExchangePlugin Installed Successfully";
		_mainwindow = m;
	}

	bool MeshDataExchangePlugin::install()
	{
		IO::IOConfigure::RegisterMeshImporter("CGNS(*.cgns)", CGNSimportMesh);
		IO::IOConfigure::RegisterMeshImporter("Fluent(*.msh)", MSHimportMesh);
		IO::IOConfigure::RegisterMeshImporter("Gambit(*.neu)", NEUimportMesh);
		IO::IOConfigure::RegisterMeshImporter("STL(*.stl)", VTK_DAT_STL_importMesh);
		IO::IOConfigure::RegisterMeshImporter("Tecplot(*.dat)", VTK_DAT_STL_importMesh);
		IO::IOConfigure::RegisterMeshImporter("VTK(*.vtk)", VTK_DAT_STL_importMesh);
		IO::IOConfigure::RegisterMeshImporter("Abaqus(*.inp)", INPimportMesh);
		IO::IOConfigure::RegisterMeshImporter("CNTM(*.cntm)", CNTMimportMesh);
		IO::IOConfigure::RegisterMeshImporter("SU2(*.su2)", SU2importMesh);
		IO::IOConfigure::RegisterMeshImporter("LS-DYNA(*.key)", KEYimportMesh);
		IO::IOConfigure::RegisterMeshImporter("PDB(*.pdb)", PDBimportMesh);

		IO::IOConfigure::RegisterMeshExporter("CGNS(*.cgns)", CGNSexportMesh);
		IO::IOConfigure::RegisterMeshExporter("Fluent(*.msh)", MSHexportMesh);
		IO::IOConfigure::RegisterMeshExporter("Gambit(*.neu)", NEUexportMesh);
		IO::IOConfigure::RegisterMeshExporter("STL(*.stl)", VTK_DAT_STL_exportMesh);
		IO::IOConfigure::RegisterMeshExporter("Tecplot(*.dat)", VTK_DAT_STL_exportMesh);
		IO::IOConfigure::RegisterMeshExporter("VTK(*.vtk)", VTK_DAT_STL_exportMesh);
		IO::IOConfigure::RegisterMeshExporter("Abaqus(*.inp)", INPexportMesh);
		IO::IOConfigure::RegisterMeshExporter("CNTM(*.cntm)", CNTMexportMesh);
		IO::IOConfigure::RegisterMeshExporter("SU2(*.su2)", SU2exportMesh);		
		IO::IOConfigure::RegisterMeshExporter("LS-DYNA(*.key)", KEYexportMesh);
		IO::IOConfigure::RegisterMeshExporter("PDB(*.pdb)", PDBexportMesh);
		return true;
	}

	bool MeshDataExchangePlugin::uninstall()
	{
		IO::IOConfigure::RemoveMeshImporter("CGNS(*.cgns)");
		IO::IOConfigure::RemoveMeshImporter("Fluent(*.msh)");
		IO::IOConfigure::RemoveMeshImporter("Gambit(*.neu)");
		IO::IOConfigure::RemoveMeshImporter("STL(*.stl)");
		IO::IOConfigure::RemoveMeshImporter("Tecplot(*.dat)");
		IO::IOConfigure::RemoveMeshImporter("VTK(*.vtk)");
		IO::IOConfigure::RemoveMeshImporter("Abaqus(*.inp)");
		IO::IOConfigure::RemoveMeshImporter("CNTM(*.cntm)");
		IO::IOConfigure::RemoveMeshImporter("SU2(*.su2)");
		IO::IOConfigure::RemoveMeshImporter("LS-DYNA(*.key)");
		IO::IOConfigure::RemoveMeshImporter("PDB(*.pdb)");

		IO::IOConfigure::RemoveMeshExporter("CGNS(*.cgns)");
		IO::IOConfigure::RemoveMeshExporter("Fluent(*.msh)");
		IO::IOConfigure::RemoveMeshExporter("Gambit(*.neu)");
		IO::IOConfigure::RemoveMeshExporter("STL(*.stl)");
		IO::IOConfigure::RemoveMeshExporter("Tecplot(*.dat)");
		IO::IOConfigure::RemoveMeshExporter("VTK(*.vtk)");
		IO::IOConfigure::RemoveMeshExporter("Abaqus(*.inp)");
		IO::IOConfigure::RemoveMeshExporter("CNTM(*.cntm)");
		IO::IOConfigure::RemoveMeshExporter("SU2(*.su2)");
		IO::IOConfigure::RemoveMeshExporter("LS-DYNA(*.key)");
		IO::IOConfigure::RemoveMeshExporter("PDB(*.pdb)");
		return true;
	}

	void MeshDataExchangePlugin::reTranslate(QString)
	{
		
	}

	GUI::MainWindow* MeshDataExchangePlugin::getMWpt()
	{
		return _mainwindow;
	}
}

void Register(GUI::MainWindow* m, QList<Plugins::PluginBase*>* ps)
{
	Plugins::PluginBase* p = new MeshData::MeshDataExchangePlugin(m);
	ps->append(p);
}

bool CGNSimportMesh(QString AbFileName)
{
	auto CGNSreader = new MeshData::CGNSdataExchange(AbFileName, MeshData::MESH_READ, MeshData::MeshDataExchangePlugin::getMWpt());	
	emit CGNSreader->start();
	return false;
}

bool CGNSexportMesh(QString AbFileName, int id)
{
	auto CGNSwriter = new MeshData::CGNSdataExchange(AbFileName, MeshData::MESH_WRITE, MeshData::MeshDataExchangePlugin::getMWpt(), id);
	emit CGNSwriter->start();
	return false;
}

bool MSHimportMesh(QString AbFileName)
{
	auto MSHreader = new MeshData::MSHdataExchange(AbFileName, MeshData::MESH_READ, MeshData::MeshDataExchangePlugin::getMWpt());
	emit MSHreader->start();
	return false;
}

bool MSHexportMesh(QString AbFileName, int id)
{
	auto MSHwriter = new MeshData::MSHdataExchange(AbFileName, MeshData::MESH_WRITE, MeshData::MeshDataExchangePlugin::getMWpt(), id);
	emit MSHwriter->start();
	return false;
}

bool NEUimportMesh(QString AbFileName)
{
	auto NEUreader = new MeshData::NEUdataExchange(AbFileName, MeshData::MESH_READ, MeshData::MeshDataExchangePlugin::getMWpt());
	emit NEUreader->start();
	return false;
}

bool NEUexportMesh(QString AbFileName, int id)
{
	auto NEUwriter = new MeshData::NEUdataExchange(AbFileName, MeshData::MESH_WRITE, MeshData::MeshDataExchangePlugin::getMWpt(), id);
	emit NEUwriter->start();
	return false;
}

bool VTK_DAT_STL_importMesh(QString AbFileName)
{
	auto VTK_DAT_STL_reader = new MeshData::VTKdataExchange(AbFileName, MeshData::MESH_READ, MeshData::MeshDataExchangePlugin::getMWpt());
	emit VTK_DAT_STL_reader->start();
	return false;
}

bool VTK_DAT_STL_exportMesh(QString AbFileName, int id)
{
	auto VTK_DAT_STL_writer = new MeshData::VTKdataExchange(AbFileName, MeshData::MESH_WRITE, MeshData::MeshDataExchangePlugin::getMWpt(), id);
	emit VTK_DAT_STL_writer->start();
	return false;
}

bool INPimportMesh(QString AbFileName)
{
	auto INPreader = new MeshData::INPdataExchange(AbFileName, MeshData::MESH_READ, MeshData::MeshDataExchangePlugin::getMWpt());
	emit INPreader->start();
	return false;
}

bool INPexportMesh(QString AbFileName, int id)
{
	auto INPwriter = new MeshData::INPdataExchange(AbFileName, MeshData::MESH_WRITE, MeshData::MeshDataExchangePlugin::getMWpt(), id);
	emit INPwriter->start();
	return false;
}

bool CNTMimportMesh(QString AbFileName)
{
	auto CNTMreader = new MeshData::CNTMdataExchange(AbFileName, MeshData::MESH_READ, MeshData::MeshDataExchangePlugin::getMWpt());
	emit CNTMreader->start();
	return false;
}

bool CNTMexportMesh(QString AbFileName, int id)
{
	auto CNTMwriter = new MeshData::CNTMdataExchange(AbFileName, MeshData::MESH_WRITE, MeshData::MeshDataExchangePlugin::getMWpt(), id);
	emit CNTMwriter->start();
	return false;
}

bool SU2importMesh(QString AbFileName)
{
	QFileInfo info(AbFileName);
	QString fileName = info.fileName();

	if (fileName.startsWith("xxx"))//su2的另外一种格式
	{
		AnotherSU2importMesh(AbFileName);
	}
	else//su2的正常格式
	{
		auto SU2reader = new MeshData::SU2dataExchange(AbFileName, MeshData::MESH_READ, MeshData::MeshDataExchangePlugin::getMWpt());
		emit SU2reader->start();
		return false;
	}
	return false;
}

bool SU2exportMesh(QString AbFileName, int id)
{
	auto SU2writer = new MeshData::CNTMdataExchange(AbFileName, MeshData::MESH_WRITE, MeshData::MeshDataExchangePlugin::getMWpt(), id);
	emit SU2writer->start();
	return false;
}

bool AnotherSU2importMesh(QString AbFileName)
{
	auto SU2reader = new MeshData::SU2dataExchange(AbFileName, MeshData::MESH_READ, MeshData::MeshDataExchangePlugin::getMWpt());
	emit SU2reader->start();	
	return false;
}

bool AnotherSU2exportMesh(QString AbFileName, int id)
{
	auto SU2writer = new MeshData::CNTMdataExchange(AbFileName, MeshData::MESH_WRITE, MeshData::MeshDataExchangePlugin::getMWpt(), id);
	emit SU2writer->start();	
	return false;
}

bool KEYimportMesh(QString AbFileName)
{
	auto KEYreader = new MeshData::KEYdataExchange(AbFileName, MeshData::MESH_READ, MeshData::MeshDataExchangePlugin::getMWpt());
	emit KEYreader->start();
	return false;
}

bool KEYexportMesh(QString AbFileName, int id)
{
	auto KEYwirter = new MeshData::KEYdataExchange(AbFileName, MeshData::MESH_WRITE, MeshData::MeshDataExchangePlugin::getMWpt(), id);
	emit KEYwirter->start();
	return false;
}

bool MESHDATAEXCHANGEPLUGINAPI PDBimportMesh(QString AbFileName)
{
	auto PDBReader = new MeshData::PDBdataExchange(AbFileName, MeshData::MESH_READ, MeshData::MeshDataExchangePlugin::getMWpt());
	emit PDBReader->start();
	return false;
}

bool MESHDATAEXCHANGEPLUGINAPI PDBexportMesh(QString AbFileName, int id)
{
	auto PDBWriter = new MeshData::PDBdataExchange(AbFileName, MeshData::MESH_WRITE, MeshData::MeshDataExchangePlugin::getMWpt(), id);
	emit PDBWriter->start();
	return false;
}
