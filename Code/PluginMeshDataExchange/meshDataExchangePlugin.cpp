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
#include "moduleBase/ThreadControl.h"
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

bool CGNSimportMesh(QString AbFileName, int modelId)
{
	auto CGNSreader = new MeshData::CGNSdataExchange(AbFileName, MeshData::MESH_READ, MeshData::MeshDataExchangePlugin::getMWpt());
	ModuleBase::ThreadControl* tc = new ModuleBase::ThreadControl(CGNSreader);
	emit tc->threadStart(); //emit CGNSreader->start();
	return false;
}

bool CGNSexportMesh(QString AbFileName, int modelId)
{
	auto CGNSwriter = new MeshData::CGNSdataExchange(AbFileName, MeshData::MESH_WRITE, MeshData::MeshDataExchangePlugin::getMWpt(), modelId);
	ModuleBase::ThreadControl* tc = new ModuleBase::ThreadControl(CGNSwriter);
	emit tc->threadStart(); //emit CGNSwriter->start();
	return false;
}

bool MSHimportMesh(QString AbFileName, int modelId)
{
	auto MSHreader = new MeshData::MSHdataExchange(AbFileName, MeshData::MESH_READ, MeshData::MeshDataExchangePlugin::getMWpt());
	ModuleBase::ThreadControl* tc = new ModuleBase::ThreadControl(MSHreader);
	emit tc->threadStart(); //emit MSHreader->start();
	return false;
}

bool MSHexportMesh(QString AbFileName, int modelId)
{
	auto MSHwriter = new MeshData::MSHdataExchange(AbFileName, MeshData::MESH_WRITE, MeshData::MeshDataExchangePlugin::getMWpt(), modelId);
	ModuleBase::ThreadControl* tc = new ModuleBase::ThreadControl(MSHwriter);
	emit tc->threadStart(); //emit MSHwriter->start();
	return false;
}

bool NEUimportMesh(QString AbFileName, int modelId)
{
	auto NEUreader = new MeshData::NEUdataExchange(AbFileName, MeshData::MESH_READ, MeshData::MeshDataExchangePlugin::getMWpt());
	ModuleBase::ThreadControl* tc = new ModuleBase::ThreadControl(NEUreader);
	emit tc->threadStart(); //emit NEUreader->start();
	return false;
}

bool NEUexportMesh(QString AbFileName, int modelId)
{
	auto NEUwriter = new MeshData::NEUdataExchange(AbFileName, MeshData::MESH_WRITE, MeshData::MeshDataExchangePlugin::getMWpt(), modelId);
	ModuleBase::ThreadControl* tc = new ModuleBase::ThreadControl(NEUwriter);
	emit tc->threadStart(); //emit NEUwriter->start();
	return false;
}

bool VTK_DAT_STL_importMesh(QString AbFileName, int modelId)
{
	auto VTK_DAT_STL_reader = new MeshData::VTKdataExchange(AbFileName, MeshData::MESH_READ, MeshData::MeshDataExchangePlugin::getMWpt());
	ModuleBase::ThreadControl* tc = new ModuleBase::ThreadControl(VTK_DAT_STL_reader);
	emit tc->threadStart(); //emit VTK_DAT_STL_reader->start();
	return false;
}

bool VTK_DAT_STL_exportMesh(QString AbFileName, int modelId)
{
	auto VTK_DAT_STL_writer = new MeshData::VTKdataExchange(AbFileName, MeshData::MESH_WRITE, MeshData::MeshDataExchangePlugin::getMWpt(), modelId);
	ModuleBase::ThreadControl* tc = new ModuleBase::ThreadControl(VTK_DAT_STL_writer);
	emit tc->threadStart(); //emit VTK_DAT_STL_writer->start();
	return false;
}

bool INPimportMesh(QString AbFileName, int modelId)
{
	auto INPreader = new MeshData::INPdataExchange(AbFileName, MeshData::MESH_READ, MeshData::MeshDataExchangePlugin::getMWpt(), modelId);
	ModuleBase::ThreadControl* tc = new ModuleBase::ThreadControl(INPreader);
	emit tc->threadStart(); //emit INPreader->start();
	return false;
}

bool INPexportMesh(QString AbFileName, int modelId)
{
	auto INPwriter = new MeshData::INPdataExchange(AbFileName, MeshData::MESH_WRITE, MeshData::MeshDataExchangePlugin::getMWpt(), modelId);
	ModuleBase::ThreadControl* tc = new ModuleBase::ThreadControl(INPwriter);
	emit tc->threadStart(); //emit INPwriter->start();
	return false;
}

bool CNTMimportMesh(QString AbFileName, int modelId)
{
	auto CNTMreader = new MeshData::CNTMdataExchange(AbFileName, MeshData::MESH_READ, MeshData::MeshDataExchangePlugin::getMWpt());
	ModuleBase::ThreadControl* tc = new ModuleBase::ThreadControl(CNTMreader);
	emit tc->threadStart(); //emit CNTMreader->start();
	return false;
}

bool CNTMexportMesh(QString AbFileName, int modelId)
{
	auto CNTMwriter = new MeshData::CNTMdataExchange(AbFileName, MeshData::MESH_WRITE, MeshData::MeshDataExchangePlugin::getMWpt(), modelId);
	ModuleBase::ThreadControl* tc = new ModuleBase::ThreadControl(CNTMwriter);
	emit tc->threadStart(); //emit CNTMwriter->start();
	return false;
}

bool SU2importMesh(QString AbFileName, int modelId)
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
		ModuleBase::ThreadControl* tc = new ModuleBase::ThreadControl(SU2reader);
		emit tc->threadStart(); //emit SU2reader->start();
		return false;
	}
	return false;
}

bool SU2exportMesh(QString AbFileName, int modelId)
{
	auto SU2writer = new MeshData::CNTMdataExchange(AbFileName, MeshData::MESH_WRITE, MeshData::MeshDataExchangePlugin::getMWpt(), modelId);
	ModuleBase::ThreadControl* tc = new ModuleBase::ThreadControl(SU2writer);
	emit tc->threadStart(); //emit SU2writer->start();
	return false;
}

bool AnotherSU2importMesh(QString AbFileName)
{
	auto SU2reader = new MeshData::SU2dataExchange(AbFileName, MeshData::MESH_READ, MeshData::MeshDataExchangePlugin::getMWpt());
	ModuleBase::ThreadControl* tc = new ModuleBase::ThreadControl(SU2reader);
	emit tc->threadStart(); 
	//emit SU2reader->start();
	return false;
}

bool AnotherSU2exportMesh(QString AbFileName, int modelId)
{
	auto SU2writer = new MeshData::CNTMdataExchange(AbFileName, MeshData::MESH_WRITE, MeshData::MeshDataExchangePlugin::getMWpt(), modelId);
	ModuleBase::ThreadControl* tc = new ModuleBase::ThreadControl(SU2writer);
	emit tc->threadStart(); 
	//emit SU2writer->start();
	return false;
}

bool KEYimportMesh(QString AbFileName, int modelId)
{
	auto KEYreader = new MeshData::KEYdataExchange(AbFileName, MeshData::MESH_READ, MeshData::MeshDataExchangePlugin::getMWpt());
	ModuleBase::ThreadControl* tc = new ModuleBase::ThreadControl(KEYreader);
	emit tc->threadStart();
	//emit KEYreader->start();
	return false;
}

bool KEYexportMesh(QString AbFileName, int modelId)
{
	auto KEYwirter = new MeshData::KEYdataExchange(AbFileName, MeshData::MESH_WRITE, MeshData::MeshDataExchangePlugin::getMWpt(), modelId);
	ModuleBase::ThreadControl* tc = new ModuleBase::ThreadControl(KEYwirter);
	emit tc->threadStart();
	//emit KEYwirter->start();
	return false;
}

bool MESHDATAEXCHANGEPLUGINAPI PDBimportMesh(QString AbFileName, int modelId)
{
	auto PDBReader = new MeshData::PDBdataExchange(AbFileName, MeshData::MESH_READ, MeshData::MeshDataExchangePlugin::getMWpt());
	ModuleBase::ThreadControl* tc = new ModuleBase::ThreadControl(PDBReader);
	emit tc->threadStart();
	/*emit PDBReader->start();*/
	return false;
}

bool MESHDATAEXCHANGEPLUGINAPI PDBexportMesh(QString AbFileName, int modelId)
{
	auto PDBWriter = new MeshData::PDBdataExchange(AbFileName, MeshData::MESH_WRITE, MeshData::MeshDataExchangePlugin::getMWpt(), modelId);
	ModuleBase::ThreadControl* tc = new ModuleBase::ThreadControl(PDBWriter);
	emit tc->threadStart();
	//emit PDBWriter->start();
	return false;
}
