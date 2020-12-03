#include "pluginPHengLEI.h"
#include "IO/IOConfig.h"
#include "ProjectTreeExtend/ProjectTreeConfig.h"
#include "ModelData/ModelDataBaseExtend.h"
#include "DataProperty/ParameterSelectable.h"
#include "DataProperty/ParameterInt.h"
#include "DataProperty/ParameterBool.h"
#include "DataProperty/ParameterDouble.h"
#include "DataProperty/ParameterGroup.h"
#include "meshData/meshKernal.h"
#include <QDebug>
#include <QFile>
#include "ConfigOptions/SolverConfig.h"
#include "ConfigOptions/ConfigOptions.h"
#include "SolverControl/SolverControlerBase.h"
#include "BCBase/BCBase.h"
#include "DataProperty/PropertyBase.h"
#include "DataProperty/ParameterBase.h"
#include <QMessageBox>
#include <QCoreApplication>
#include <ProjectTree/projectTreeBase.h>
#include "PHEngLEIWriter.h"
#include <QDir>
void Register(GUI::MainWindow* m, QList<Plugins::PluginBase*>* ps)
{
	Plugins::PluginBase* p = new Plugins::PHengLEIPlugin(m);
	ps->append(p);
}

namespace Plugins
{

	GUI::MainWindow* PHengLEIPlugin::_mainwindow = nullptr;
	Plugins::PHengLEIPlugin* PHengLEIPlugin::_self = nullptr;

	PHengLEIPlugin::PHengLEIPlugin(GUI::MainWindow* m)
	{
		_mainwindow = m;
		_describe = "PHengLEI plugin";
		_self = this;
	}

	PHengLEIPlugin::~PHengLEIPlugin()
	{

	}

	bool PHengLEIPlugin::install()
	{
		IO::IOConfigure::RegisterInputFile("PHengLEI", WriteOut);
		ProjectTree::ProjectTreeConfig::RegisterContextMenu(1, "Transform", "MeshSetting", MeshTransfer);
		ProjectTree::ProjectTreeConfig::RegisterContextMenu(1, "Partition", "MeshSetting", MeshPartition);

		ProjectTree::ProjectTreeConfig::RegisterContextMenu(2, "Transform", "MeshSetting", MeshTransfer);
		ProjectTree::ProjectTreeConfig::RegisterContextMenu(2, "Partition", "MeshSetting", MeshPartition);

		ProjectTree::ProjectTreeConfig::RegisterContextMenu(3, "Transform", "MeshSetting", MeshTransfer);
		ProjectTree::ProjectTreeConfig::RegisterContextMenu(3, "Partition", "MeshSetting", MeshPartition);
		return true;
	}

	bool PHengLEIPlugin::uninstall()
	{
		IO::IOConfigure::RemoveInputFile("PHengLEI");
		ProjectTree::ProjectTreeConfig::removeRootContextMenu(1, "Transform");
		return true;
	}

	void PHengLEIPlugin::setData(ModelData::ModelDataBase* data)
	{
		_data = data;
	}

	void PHengLEIPlugin::setIsStruct(bool str)
	{
		_isStruct = str;
	}

	bool PHengLEIPlugin::getIsStruct()
	{
		return _isStruct;
	}

	void PHengLEIPlugin::setGridName(QString gname)
	{
		_gname = gname;
	}

	QString PHengLEIPlugin::getGridName()
	{
		return _gname;
	}

	void PHengLEIPlugin::setPhengLeiCase(QString casename)
	{
		_PhengLeiCase = casename;
	}

	QString PHengLEIPlugin::getPhengLeiCase()
	{
		return _PhengLeiCase;
	}

	GUI::MainWindow* PHengLEIPlugin::getMainWindow()
	{
		return _mainwindow;
	}


	void PHengLEIPlugin::processFinished(int pid)
	{
		ParallelPartition(_data);
	}
}

bool WriteOut(QString path, ModelData::ModelDataBase* d)
{
	Plugins::PHengLEIWriter* writer;
	/*const QString filepath = d->getPath();
	QString dir = filepath + "/results";
	writer->clearTempFiles(dir);*/

	GetBoundaryCondition(d);
	ParameterSetting(d);
	writer->startSolver(d, "solve",true);
	return true;
}

void PHENGLEIPLUGINAPI MeshTransfer(ModelData::ModelDataBase* model, ProjectTree::ProjectTreeBase* tree)
{
	Plugins::PHengLEIWriter* writer;
	//复制
	bool result = CopyFileToProjectPath(model);
	if (!result)return;
	if (MeshData::MeshData::getInstance()->getKernalCount() < 1)
	{
		QMessageBox::information(NULL, "Title", "Please import the grid first!");
		return;
	}
	Plugins::PHengLEIPlugin* plug = Plugins::PHengLEIPlugin::getSelf();
	plug->setPhengLeiCase(tree->getName());
	//写入主控文件和网格文件
	writer->writeHyparaFile(model, "key.hypara");
	writer->writeHyparaFile(model, "grid_para.hypara");
	writer->startSolver(model, "Transfer", false);

}

void PHENGLEIPLUGINAPI MeshPartition(ModelData::ModelDataBase* model, ProjectTree::ProjectTreeBase* tree)
{
	Plugins::PHengLEIWriter* writer;
	writer->modifyKeyFile(model, "MeshPartition");
	writer->writeHyparaFile(model, "partition.hypara");

	//求解网格分区
	ConfigOption::SolverOption*  solvers = ConfigOption::ConfigOption::getInstance()->getSolverOption();
	ConfigOption::SolverInfo* solver = solvers->getSolverAt(0);
	auto m = Plugins::PHengLEIPlugin::getMainWindow();
	SolverControl::SolverControlBase* solverControl = new SolverControl::SolverControlBase(m, solver, model, true);
	solverControl->setSolverDescription(QString("%1-Partition").arg(model->getName()));
	solverControl->startSolver();
	
	//判断是否为结构网格
	DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("网格类型"));
	DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
	if (se != nullptr && se->getCurrentIndex() == 1) return;

	//判断是否为串行分区方式
	bool isSerial{ true };
	DataProperty::ParameterBase* pmesh = model->getParameterByName(QString::fromLocal8Bit("分区方式"));
	DataProperty::ParameterSelectable* semesh = dynamic_cast<DataProperty::ParameterSelectable*>(pmesh);
	isSerial = !semesh->getCurrentIndex();
	Plugins::PHengLEIPlugin* plug = Plugins::PHengLEIPlugin::getSelf();
	plug->setData(model);

	if (!isSerial)
		QObject::connect(solverControl, SIGNAL(processFinish(int)), plug, SLOT(processFinished(int)));
	
}

void  ParameterSetting(ModelData::ModelDataBase* model)
{
	Plugins::PHengLEIWriter* writer;

	//修改key文件
	writer->modifyKeyFile(model, "ParameterSetting");
	Plugins::PHengLEIPlugin* plug = Plugins::PHengLEIPlugin::getSelf();
	QString casename = plug->getPhengLeiCase();

	//创建cfd_para_subsonic.hypara
	const QString path = model->getPath();
	QString parpath{};
	if (casename=="Subsonic")  parpath = path + "/bin/cfd_para_subsonic.hypara";
	else if (casename == "Transonic") parpath = path + "/bin/cfd_para_transonic.hypara";
	else parpath = path + "/bin/cfd_para_hypersonic.hypara";

	QFile parfile(parpath);
	if (!parfile.open(QIODevice::WriteOnly)) return;
	QTextStream stream(&parfile);

	//网格类型是否为结构
	bool isStr{ false };
	{
		DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("网格类型"));
		DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
		if (se != nullptr && se->getCurrentIndex() == 1) isStr = true;
	}
	writer->addBasicPara(model,stream);
	writer->addFlowPara(model,stream);
	writer->addPhysicalModelPara(model, stream);
	writer->SpatialStruct(model,stream,isStr);
	writer->addTimeDiscretePara(model,stream);
	writer->addOtherPara(model,stream);
	parfile.close();
	
}

void GetBoundaryCondition(ModelData::ModelDataBase* data)
{
	if (!data)	return;
	QString bcPath = data->getPath() + "/bin/boundary_condition.hypara";
	QFile bcFile(bcPath);
	if (!bcFile.open(QIODevice::Text | QIODevice::ReadWrite | QIODevice::Truncate))	return;
	QTextStream io(&bcFile);

	int nBC = data->getBCCount();
	io << QString("int nBoundaryConditons = %1;").arg(nBC) << endl;
	for (int i = 0; i < nBC; i++)
	{
		BCBase::BCBase* bc = data->getBCAt(i);
		QString strType = bc->getPropertyByName("BCType")->getVariant().toString();
		WriteBCParameter(bc, io, strType);
	}
}

void  WriteBCParameter(BCBase::BCBase* bc, QTextStream& io, QString& strType)
{
	int nPara = bc->getVisibleParaCount();
	QStringList ParaValues;
	for (int j = 0; j < nPara; j++)
		ParaValues << bc->getVisibleParameterAt(j)->valueToString();

	QStringList list = StrTypeConverToNumberType(strType);
	io << "string bcName = \"" << bc->getComponentName() << "\";" << endl;
	io << "{" << endl;
	io << "  int bcType = " << list[0] << ';' << endl;

	if (strType == "WallBoundary")
	{
		if (ParaValues.size() != 16)    return;
		if (ParaValues[0] == QObject::tr("Isothermal Wall"))
			io << QString("  double wallTemperature = %1;").arg(ParaValues[1].toDouble()) << endl;
		else
			io << "  double wallTemperature = -1.0;" << endl;
		if (ParaValues[2] == "true")
		{
			io << QString("  double forceRefenenceLength = %1;").arg(ParaValues[3].toDouble()) << endl;
			io << QString("  double forceRefenenceLengthSpanWise = %1;").arg(ParaValues[4].toDouble()) << endl;
			io << QString("  double forceRefenenceArea = %1;").arg(ParaValues[5].toDouble()) << endl;
			io << QString("  double TorqueRefX = %1;").arg(ParaValues[6].toDouble()) << endl;
			io << QString("  double TorqueRefY = %1;").arg(ParaValues[7].toDouble()) << endl;
			io << QString("  double TorqueRefZ = %1;").arg(ParaValues[8].toDouble()) << endl;
		}
		if (ParaValues[9] == "true")
		{
			io << "  int dumpHingeMoment = 1;" << endl;
			io << QString("  double localCoordAxis0[3] = [%1 %2 %3];").arg(ParaValues[10].toDouble()).arg(ParaValues[11].toDouble()).arg(ParaValues[12].toDouble()) << endl;
			io << QString("  double localCoordAxis1[3] = [%1 %2 %3];").arg(ParaValues[13].toDouble()).arg(ParaValues[14].toDouble()).arg(ParaValues[15].toDouble()) << endl;
		}
		else
			io << "  int dumpHingeMoment = 0;" << endl;
	}
	else if (strType == "FarBoundary" || strType == "EntranceBoundary")
	{
		if (ParaValues.size() != 5)	   return;
		WriteMachAttackdSideslipd(io, list, ParaValues);
		io << QString("  double refReNumber = %1;").arg(ParaValues[3].toDouble()) << endl;
		io << QString("  double refDimensionalTemperature = %1;").arg(ParaValues[4].toDouble()) << endl;
	}
	else if (strType == "FarFlightBoundary" || strType == "EntranceFlightBoundary")
	{
		if (ParaValues.size() != 4)    return;
		WriteMachAttackdSideslipd(io, list, ParaValues);
		io << QString("  double height = %1;").arg(ParaValues[3].toDouble()) << endl;
	}
	else if (strType == "FarExperimentBoundary")
	{
		if (ParaValues.size() != 5)    return;
		WriteMachAttackdSideslipd(io, list, ParaValues);
		io << QString("  double refDimensionalTemperature = %1;").arg(ParaValues[3].toDouble()) << endl;
		io << QString("  double refDimensionalPressure = %1;").arg(ParaValues[4].toDouble()) << endl;
	}
	else if (strType == "PressureImportBoundary")
	{
		if (ParaValues.size() != 9)    return;
		if (ParaValues[0] == QObject::tr("Boundary Surface Normal"))
			io << "  int directionMethod = 2;" << endl;
		else
		{
			io << "  int directionMethod = 1;" << endl;
			io << QString("  double direction_inlet[3] = %1,%2,%3;").arg(ParaValues[6]).arg(ParaValues[7]).arg(ParaValues[8]) << endl;
		}
		WritePressureExportBoundary(io, ParaValues);
	}
	else if (strType == "PressureExportBoundary")
	{
		if (ParaValues.size() != 5)    return;
		WritePressureExportBoundary(io, ParaValues);
	}
	io << "}" << endl << endl;
}

void  WriteMachAttackdSideslipd(QTextStream& io, QStringList& list, QStringList& ParaValues)
{
	io << QString("  int inflowParaType = %1;").arg(list[1]) << endl;
	io << QString("  double refMachNumber = %1;").arg(ParaValues[0].toDouble()) << endl;
	io << QString("  double attackd = %1;").arg(ParaValues[1].toDouble()) << endl;
	io << QString("  double angleSlide = %1;").arg(ParaValues[2].toDouble()) << endl;
}

void  WritePressureExportBoundary(QTextStream& io, QStringList& ParaValues)
{
	io << QString("  double totalP_inlet = %1;").arg(ParaValues[0].toDouble()) << endl;
	io << QString("  double totalT_inlet = %1;").arg(ParaValues[1].toDouble()) << endl;
	io << QString("  double refMachNumber = %1;").arg(ParaValues[2].toDouble()) << endl;
	io << QString("  double refDimensionalTemperature = %1;").arg(ParaValues[3].toDouble()) << endl;
	io << QString("  double refDimensionalPressure = %1;").arg(ParaValues[4].toDouble()) << endl;
}

QStringList  StrTypeConverToNumberType(QString& strType)
{
	QStringList res;
	if (strType == "ExtrapolationBoundary")
		res << "1";
	else if (strType == "WallBoundary")
		res << "2";
	else if (strType == "SymmetryBoundary")
		res << "3";
	else if (strType == "FarBoundary")
		res << "4" << "0";
	else if (strType == "FarFlightBoundary")
		res << "4" << "1";
	else if (strType == "FarExperimentBoundary")
		res << "4" << "2";
	else if (strType == "EntranceBoundary")
		res << "5" << "0";
	else if (strType == "EntranceFlightBoundary")
		res << "5" << "1";
	else if (strType == "ExitBoundary")
		res << "6";
	else if (strType == "PolarAxisBoundary")
		res << "7";
	else if (strType == "PressureImportBoundary")
		res << "52";
	else if (strType == "PressureExportBoundary")
		res << "62";
	return res;
}

void ParallelPartition(ModelData::ModelDataBase* model)
{
	Plugins::PHengLEIWriter* writer;
	if (model == nullptr) return;
	qDebug() << "ParallelPartition";
	const QString path = model->getPath();
	QString parpath = path + "/bin/partition.hypara";
	QFile parfile(parpath);
	if (!parfile.open(QIODevice::WriteOnly)) return;
	QTextStream stream(&parfile);

	DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("网格类型"));
	DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
	if (se != nullptr) stream << "int  pgridtype       = " << se->getCurrentIndex() << ";" << endl;

	DataProperty::ParameterBase* pd = model->getParameterByName(QString::fromLocal8Bit("进程数目"));
	DataProperty::ParameterInt* sed = dynamic_cast<DataProperty::ParameterInt*>(pd);
	if (sed != nullptr) stream << "int  maxproc       = " << sed->getValue() << ";" << endl;

	stream << QString("string original_grid_file = \"./grid/%1_2.fts\";").arg(writer->getMeshName()) << endl;
	stream << QString("string partition_grid_file = \"./grid/%1.fts\";").arg(writer->getMeshName() + "_" + QString::number(sed->getValue())) << endl;
	stream << "int  numberOfMultigrid       = 1;";
	parfile.close();

	//求解网格分区
	writer->startSolver(model, "ParallelPartition", true);

}

bool CopyFileToProjectPath(ModelData::ModelDataBase* data)
{
	QString sourceDir = QCoreApplication::applicationDirPath();
	QString aimDir = data->getPath();
	QFile sourceFile, aimFile;

	QDir dir;
	dir.mkpath(aimDir + "/bin/");
	dir.mkpath(aimDir + "/results/");
	dir.mkpath(aimDir + "/grid/");

	QString source_cfd_para = sourceDir + "/../cfd_para.hypara";
	QString aim_cfd_para = aimDir + "/bin/cfd_para.hypara";
 	QString source_phenglei_exe = sourceDir + "/../PHengLEIv3d0.exe";
 	QString aim_phenglei_exe = aimDir + "/PHengLEIv3d0.exe";
 	QString source_tecio_dll = sourceDir + "/../tecio.dll";
 	QString aim_tecio_dll = aimDir + "/tecio.dll";

	sourceFile.setFileName(source_cfd_para);
	if (!sourceFile.exists())	return false;
 	sourceFile.setFileName(source_phenglei_exe);
 	if (!sourceFile.exists())	return false;
 	sourceFile.setFileName(source_tecio_dll);
	if (!sourceFile.exists())	return false;

	aimFile.setFileName(aim_cfd_para);
	if (aimFile.exists())	aimFile.remove();
	aimFile.setFileName(aim_phenglei_exe);
 	if (aimFile.exists())	aimFile.remove();
 	aimFile.setFileName(aim_tecio_dll);
 	if (aimFile.exists())	aimFile.remove();

	return QFile::copy(source_cfd_para, aim_cfd_para)&&
		QFile::copy(source_phenglei_exe, aim_phenglei_exe) &&
		QFile::copy(source_tecio_dll, aim_tecio_dll);
}
