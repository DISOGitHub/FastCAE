#include "PHEngLEIWriter.h"
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
#include "ConfigOptions/SolverConfig.h"
#include "ConfigOptions/ConfigOptions.h"
#include "SolverControl/SolverControlerBase.h"
#include "BCBase/BCBase.h"
#include "DataProperty/PropertyBase.h"
#include "DataProperty/ParameterBase.h"
#include <QMessageBox>
#include <QCoreApplication>
#include <ProjectTree/projectTreeBase.h>
#include <QFileSystemWatcher>
#include <QDirIterator>
#include <QDebug>
#include <QFile>
#include <QDir>

namespace Plugins
{

	void PHengLEIWriter::writeHyparaFile(ModelData::ModelDataBase* model, QString filename)
	{
		if (filename=="key.hypara") writeKeyFile(model);
		else  if (filename == "grid_para.hypara") writeGridPara(model);
		else if (filename == "partition.hypara") writepartition(model);

	}

	void PHengLEIWriter::modifyKeyFile(ModelData::ModelDataBase* model, QString process)
	{
		QString strAll = this->readPhengLeiFile(model, "key.hypara");
		if (strAll.isEmpty()) return;
		QString keyfile = model->getPath() + "/bin/key.hypara";
		QStringList strList{};
		if (process == "MeshPartition")
		{
			//修改并写入
			QFile writeFile(keyfile);
			if (writeFile.open(QIODevice::WriteOnly | QIODevice::Text))
			{
				QTextStream stream(&writeFile);
				strList = strAll.split("\n");           //以换行符为基准分割文本
				for (int i = 0; i < strList.count(); i++)    //遍历每一行 
				{
					QString oneline = strList.at(i);
					if (oneline.contains("nsimutask"))    //修改的内容	
					{
						QString tempStr = QString("int    nsimutask      = 3;");
						oneline.replace(0, oneline.length(), tempStr);   //要替换的内容
						stream << oneline << '\n';

					}
					else if (oneline.contains("parafilename"))
					{
						QString tempStr = QString("string parafilename   = \"./bin/partition.hypara\";");
						oneline.replace(0, oneline.length(), tempStr);   //是要替换的内容
						stream << oneline << '\n';
					}
					//如果没有找到要替换的内容，照常写入
					else
					{
						if (i == strList.count() - 1) stream << strList.at(i);
						else stream << strList.at(i) << '\n';
					}
				}
			}
			writeFile.close();
		}
		else if (process == "ParameterSetting")
		{
			//修改并写入
			QFile writeFile(keyfile);
			if (writeFile.open(QIODevice::WriteOnly | QIODevice::Text))
			{
				QTextStream stream(&writeFile);
				strList = strAll.split("\n");           //以换行符为基准分割文本
				for (int i = 0; i < strList.count(); i++)    //遍历每一行
				{
					QString oneline = strList.at(i);
					if (oneline.contains("nsimutask"))    //修改的内容	
					{
						QString tempStr = QString("int    nsimutask      = 0;");
						oneline.replace(0, oneline.length(), tempStr);   //要替换的内容
						stream << oneline << '\n';

					}
					else if (oneline.contains("parafilename"))
					{
						QString tempStr = QString("string parafilename   = \"./bin/cfd_para_subsonic.hypara\";");
						oneline.replace(0, oneline.length(), tempStr);   //是要替换的内容
						stream << oneline << '\n';
					}
					//如果没有找到要替换的内容，照常写入
					else
					{
						if (i == strList.count() - 1) stream << strList.at(i);
						else stream << strList.at(i) << '\n';
					}
				}
			}
			writeFile.close();
		}
	}


	void PHengLEIWriter::startSolver(ModelData::ModelDataBase* model,QString process, bool Mpi)
	{
		ConfigOption::SolverOption*  solvers = ConfigOption::ConfigOption::getInstance()->getSolverOption();
		ConfigOption::SolverInfo* solver = solvers->getSolverAt(0);
		auto m = Plugins::PHengLEIPlugin::getMainWindow();
		SolverControl::SolverControlBase* solverControl = new SolverControl::SolverControlBase(m, solver, model, true);
		solverControl->setSolverDescription(QString("%1-%2").arg(model->getName()).arg(process));
/*
		//监控残差文件更新
		QFileSystemWatcher m_pSystemWatcher;
		m_pSystemWatcher.addPath(model->getPath() + "/results/res.dat");
		QObject::connect(m_pSystemWatcher, SIGNAL(fileChanged(QString)), this, SLOT(fileUpdated(QString)));
*/

		if(!Mpi)
			solverControl->startSolver();
		else
		{
			DataProperty::ParameterBase* pd = model->getParameterByName(QString::fromLocal8Bit("进程数目"));
			DataProperty::ParameterInt* sed = dynamic_cast<DataProperty::ParameterInt*>(pd);
			solverControl->startMPI(sed->getValue());
		}
	}

	void PHengLEIWriter::writeKeyFile(ModelData::ModelDataBase* model)
	{
		const QString path = model->getPath();
		qDebug() << path;
		{
			QString keyfile = path + "/bin/key.hypara";
			QFile kfile(keyfile);
			if (!kfile.open(QIODevice::WriteOnly)) return;
			QTextStream stream(&kfile);
			stream << "string title     = \"PHengLEI Main Parameter Control File\";" << endl;
			stream << "string defaultParaFile = \"./bin/cfd_para.hypara\";" << endl;
			DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("空间维数"));
			DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
			if (se != nullptr)
			{
				int index = se->getCurrentIndex();
				stream << "int  ndim       = " << index + 2 << ";" << endl;
			}
			stream << "int  nparafile  = 1;" << endl << "int    nsimutask      = 1;" << endl
				<< "string parafilename   = \"./bin/grid_para.hypara\";" << endl;
			stream << "int    iovrlap = 0;" << endl << "int  numberOfGridProcessor = 0;" << endl;

			kfile.close();
		}
	}



	void PHengLEIWriter::writeGridPara(ModelData::ModelDataBase* model)
	{
		const QString path = model->getPath();
		QString gridfile = path + "/bin/grid_para.hypara";
		QFile gfile(gridfile);
		if (!gfile.open(QIODevice::WriteOnly)) return;
		QTextStream stream(&gfile);

		{
			DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("网格类型"));
			DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
			if (se != nullptr) stream << "int  gridtype       = " << se->getCurrentIndex() << ";" << endl;
		}
		{
			DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("网格方向"));
			DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
			if (se != nullptr) stream << "int  axisup       = " << se->getCurrentIndex() + 1 << ";" << endl;
		}
		{
			DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("网格格式")); 
			DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
			if (se != nullptr&&se->getCurrentIndex()!=0) stream << "int  from_gtype       = " << se->getCurrentIndex() << ";" << endl;
		}
	
		auto k = MeshData::MeshData::getInstance()->getKernalAt(0);
		QString pathname = k->getPath();
		stream << QString("string from_gfile = \"%1\";").arg(pathname)<< endl;
		stream << QString("string out_gfile = \"./grid/%1.fts\"").arg(this->getMeshName());
		gfile.close();
	}

	void PHengLEIWriter::writepartition(ModelData::ModelDataBase* model)
	{
		const QString path = model->getPath();
		QString parpath = path + "/bin/partition.hypara";
		QFile parfile(parpath);
		if (!parfile.open(QIODevice::WriteOnly)) return;
		QTextStream stream(&parfile);

		bool isSerial{ true };
		DataProperty::ParameterBase* pmesh = model->getParameterByName(QString::fromLocal8Bit("分区方式"));
		DataProperty::ParameterSelectable* semesh = dynamic_cast<DataProperty::ParameterSelectable*>(pmesh);
		isSerial = !semesh->getCurrentIndex();

		DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("网格类型"));
		DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
		if (se != nullptr) stream << "int  pgridtype       = " << se->getCurrentIndex() << ";" << endl;

		DataProperty::ParameterBase* pd = model->getParameterByName(QString::fromLocal8Bit("进程数目"));
		DataProperty::ParameterInt* sed = dynamic_cast<DataProperty::ParameterInt*>(pd);
		if (sed != nullptr&&isSerial) stream << "int  maxproc       = " << sed->getValue() << ";" << endl;
		if (sed != nullptr&&!isSerial) stream << "int  maxproc       = 2;" << endl;

		stream << QString("string original_grid_file = \"./grid/%1.fts\";").arg(this->getMeshName()) << endl;

		if (isSerial) stream << QString("string partition_grid_file = \"./grid/%1.fts\";").arg(this->getMeshName()+"_"+QString::number(sed->getValue())) << endl;
		else stream << QString("string partition_grid_file = \"./grid/%1_2.fts\";").arg(this->getMeshName()) << endl;

		stream << "int  numberOfMultigrid       = 1;";
		parfile.close();

	}

	void PHengLEIWriter::writeParaSubsonic(ModelData::ModelDataBase* model)
	{
		const QString path = model->getPath();
		QString parpath = path + "/bin/cfd_para_subsonic.hypara";
		qDebug() << parpath;
		QFile parfile(parpath);
		if (!parfile.open(QIODevice::WriteOnly)) return;
		QTextStream stream(&parfile);
		//基本参数
		bool isStr{ false };
		{
			DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("网格类型"));
			DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
			if (se != nullptr && se->getCurrentIndex() == 1) isStr = true;
		}

	}

	QString PHengLEIWriter::readPhengLeiFile(ModelData::ModelDataBase* model, QString filename)
	{
		const QString path = model->getPath();
		QString strAll{};
		QStringList strList{};
		QString keyfile = path + "/bin/"+filename;
		QFile readFile(keyfile);
		if (readFile.open((QIODevice::ReadOnly | QIODevice::Text)))
		{
			QTextStream stream(&readFile);
			strAll = stream.readAll();
		}
		else
		{
			QMessageBox::information(NULL, "Title", "File don't exist!");
			return QString();
		}
		readFile.close();
		return strAll;

	}

	QString PHengLEIWriter::getMeshName()
	{
		auto k = MeshData::MeshData::getInstance()->getKernalAt(0);
		QString pathname = k->getPath();
		QStringList strlist = pathname.split("/");
		QString meshname = strlist.last().split(".").first();
		return meshname;
	}

	void PHengLEIWriter::clearTempFiles(const QString& temp_path)
	{
		QDir Dir(temp_path);
		if (Dir.count() < 1)
		{
			qDebug() << QString("%1 is Empty!").arg(temp_path);
			return;
		}

		// 第三个参数是QDir的过滤参数，这三个表示收集所有文件和目录，且不包含"."和".."目录。
		// 因为只需要遍历第一层即可，所以第四个参数填QDirIterator::NoIteratorFlags
		QDirIterator DirsIterator(temp_path, QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
		while (DirsIterator.hasNext())
		{
			if (!Dir.remove(DirsIterator.next())) // 删除文件操作如果返回否，那它就是目录
			{
				QDir(DirsIterator.filePath()).removeRecursively(); // 删除目录本身以及它下属所有的文件及目录
			}
		}
	}

	void PHengLEIWriter::fileUpdated(QString filepath)
	{
		qDebug() << filepath;
	}

	void PHengLEIWriter::addBasicPara(ModelData::ModelDataBase* model, QTextStream& stream)
	{
		DataProperty::ParameterGroup* g = model->getParameterGroupByName(QString::fromLocal8Bit("基本参数"));
		if (g != nullptr)
		{
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("流场间隔步长"));
				DataProperty::ParameterInt* se = dynamic_cast<DataProperty::ParameterInt*>(p);
				if (se != nullptr) stream << "int  intervalStepFlow       = " << se->getValue() << ";" << endl;
			}
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("最大迭代步数"));
				DataProperty::ParameterInt* se = dynamic_cast<DataProperty::ParameterInt*>(p);
				if (se != nullptr) stream << "int  maxSimuStep       = " << se->getValue() << ";" << endl;
			}
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("流场显示步长"));
				DataProperty::ParameterInt* se = dynamic_cast<DataProperty::ParameterInt*>(p);
				if (se != nullptr) stream << "int  intervalStepPlot       = " << se->getValue() << ";" << endl;
			}
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("气动力输出步长"));
				DataProperty::ParameterInt* se = dynamic_cast<DataProperty::ParameterInt*>(p);
				if (se != nullptr) stream << "int  intervalStepForce       = " << se->getValue() << ";" << endl;
			}
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("残差步长"));
				DataProperty::ParameterInt* se = dynamic_cast<DataProperty::ParameterInt*>(p);
				if (se != nullptr) stream << "int  intervalStepRes       = " << se->getValue() << ";" << endl;
			}
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("低速预处理"));
				DataProperty::ParameterBool* se = dynamic_cast<DataProperty::ParameterBool*>(p);
				if (se != nullptr) stream << "int  ifLowSpeedPrecon       = " << int(se->getValue()) << ";" << endl;
			}
		}
	}

	void PHengLEIWriter::addFlowPara(ModelData::ModelDataBase* model, QTextStream& stream)
	{
		DataProperty::ParameterGroup* g = model->getParameterGroupByName(QString::fromLocal8Bit("来流参数"));
		if (g != nullptr)
		{
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("马赫数"));
				DataProperty::ParameterDouble* se = dynamic_cast<DataProperty::ParameterDouble*>(p);
				if (se != nullptr) stream << "double  refMachNumber       = " << se->getValue() << ";" << endl;
			}
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("攻角"));
				DataProperty::ParameterDouble* se = dynamic_cast<DataProperty::ParameterDouble*>(p);
				if (se != nullptr) stream << "double  attackd       = " << se->getValue() << ";" << endl;
			}
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("侧滑角"));
				DataProperty::ParameterDouble* se = dynamic_cast<DataProperty::ParameterDouble*>(p);
				if (se != nullptr) stream << "double  angleSlide       = " << se->getValue() << ";" << endl;
			}
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("来流边界条件类型"));
				DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
				if (se != nullptr) stream << "int  inflowParaType       = " << se->getCurrentIndex() << ";" << endl;
			}
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("雷诺数"));
				DataProperty::ParameterDouble* se = dynamic_cast<DataProperty::ParameterDouble*>(p);
				if (se != nullptr) stream << "double  refReNumber       = " << se->getValue() << ";" << endl;
			}

			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("来流温度"));
				DataProperty::ParameterDouble* se = dynamic_cast<DataProperty::ParameterDouble*>(p);
				if (se != nullptr) stream << "double  refDimensionalTemperature       = " << se->getValue() << ";" << endl;
			}
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("网格尺度"));
				DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
				if (se != nullptr) stream << "int  gridUnit       = " << se->getCurrentIndex() << ";" << endl;
			}
		}
	}

	void PHengLEIWriter::addGridPara(ModelData::ModelDataBase* model, QTextStream& stream)
	{
		DataProperty::ParameterGroup* g = model->getParameterGroupByName(QString::fromLocal8Bit("网格相关参数"));
		if (g != nullptr)
		{
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("参考长度(m)"));
				DataProperty::ParameterDouble* se = dynamic_cast<DataProperty::ParameterDouble*>(p);
				if (se != nullptr) stream << "double  forceRefenenceLength       = " << se->getValue() << ";" << endl;
			}

			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("参考展长(m)"));
				DataProperty::ParameterDouble* se = dynamic_cast<DataProperty::ParameterDouble*>(p);
				if (se != nullptr) stream << "double  forceRefenenceLengthSpanWise    = " << se->getValue() << ";" << endl;
			}
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("参考面积(㎡)"));
				DataProperty::ParameterDouble* se = dynamic_cast<DataProperty::ParameterDouble*>(p);
				if (se != nullptr) stream << "double  forceRefenenceArea       = " << se->getValue() << ";" << endl;
			}
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("X(m)"));
				DataProperty::ParameterDouble* se = dynamic_cast<DataProperty::ParameterDouble*>(p);
				if (se != nullptr) stream << "double  TorqueRefX       = " << se->getValue() << ";" << endl;
			}
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("Y(m)"));
				DataProperty::ParameterDouble* se = dynamic_cast<DataProperty::ParameterDouble*>(p);
				if (se != nullptr) stream << "double  TorqueRefY       = " << se->getValue() << ";" << endl;
			}
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("Z(m)"));
				DataProperty::ParameterDouble* se = dynamic_cast<DataProperty::ParameterDouble*>(p);
				if (se != nullptr) stream << "double  TorqueRefZ       = " << se->getValue() << ";" << endl;
			}
		}
	}

	void PHengLEIWriter::addPhysicalModelPara(ModelData::ModelDataBase* model, QTextStream& stream)
	{
		DataProperty::ParameterGroup* g = model->getParameterGroupByName(QString::fromLocal8Bit("物理模型"));
		if (g != nullptr)
		{
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("粘性模型"));
				DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
				if (se != nullptr) stream << "int  iviscous    = " << se->getCurrentIndex() << ";" << endl;
			}
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("粘性类型"));
				DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
				if (se != nullptr) stream << QString("string viscousName   =\"%1 \"").arg(se->valueToString()) << ";" << endl;
			}
			/*{
			DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("DES类型"));
			DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
			if (se != nullptr) stream << "int  DESType    = " << se->getCurrentIndex() << ";" << endl;
			}*/
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("ROE熵修正类型"));
				DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
				if (se != nullptr) stream << "int  roeEntropyFixMethod    = " << se->getCurrentIndex() + 1 << ";" << endl;
			}
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("ROE熵修正系数"));
				DataProperty::ParameterDouble* se = dynamic_cast<DataProperty::ParameterDouble*>(p);
				if (se != nullptr) stream << "double  roeEntropyScale       = " << se->getValue() << ";" << endl;
			}
		}
	}

	void PHengLEIWriter::SpatialStruct(ModelData::ModelDataBase* model, QTextStream& stream, bool isStr)
	{
		if (isStr)
		{
			DataProperty::ParameterGroup* g = model->getParameterGroupByName(QString::fromLocal8Bit("结构网格对流项"));
			if (g != nullptr)
			{
				{
					DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("无粘项离散类型"));
					DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
					if (se != nullptr) stream << QString("string inviscidSchemeName   =\"%1 \"").arg(se->valueToString()) << ";" << endl;
				}
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("限制器类型"));
				DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
				if (se != nullptr) stream << QString("string str_limiter_name   =\"%1 \"").arg(se->valueToString()) << ";" << endl;
			}
			}
		}
		//空间离散化-非结构求解器
		else{
			DataProperty::ParameterGroup* g = model->getParameterGroupByName(QString::fromLocal8Bit("物理模型"));
			if (g != nullptr)
			{
				{
					DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("非结构无粘项离散类型"));
					DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
					if (se != nullptr) stream << QString("string uns_scheme_name   =\"%1 \"").arg(se->valueToString()) << ";" << endl;
				}
			{
				DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("非结构限制器类型"));
				DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
				if (se != nullptr) stream << QString("string uns_limiter_name   =\"%1 \"").arg(se->valueToString()) << ";" << endl;
			}
			{
				DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("vancat限制器系数"));
				DataProperty::ParameterDouble* se = dynamic_cast<DataProperty::ParameterDouble*>(p);
				if (se != nullptr) stream << "double  venkatCoeff       = " << se->getValue() << ";" << endl;
			}
			}
		}

	}

	void PHengLEIWriter::addTimeDiscretePara(ModelData::ModelDataBase* model, QTextStream& stream)
	{
		{
			DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("定常设置"));
			DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
			if (se != nullptr) stream << "int  iunsteady       = " << se->getCurrentIndex() << ";" << endl;
		}
	{
		DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("库朗数"));
		DataProperty::ParameterDouble* se = dynamic_cast<DataProperty::ParameterDouble*>(p);
		if (se != nullptr) stream << "double  CFLEnd       = " << se->getValue() << ";" << endl;
	}
	{
		DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("Lusgs前后扫描步数"));
		DataProperty::ParameterInt* se = dynamic_cast<DataProperty::ParameterInt*>(p);
		if (se != nullptr) stream << "int  nLUSGSSweeps       = " << se->getValue() << ";" << endl;
	}
	{
		DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("多重网格层数"));
		DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
		if (se != nullptr) stream << "int  nMGLevel       = " << se->getCurrentIndex() + 1 << ";" << endl;
	}
	{
		DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("流场初始化步数"));
		DataProperty::ParameterInt* se = dynamic_cast<DataProperty::ParameterInt*>(p);
		if (se != nullptr) stream << "int  flowInitStep       = " << se->getValue() << ";" << endl;
	}
	}

	void PHengLEIWriter::addOtherPara(ModelData::ModelDataBase* model,QTextStream& stream)
	{
		//路径设置,可视化输出设置,其他限制设置
		QStringList palast = this->readPhengLeiFile(model, "partition.hypara").split("\n");
		QString meshname = palast.at(palast.size() - 2).split("=").last();

		stream <<QString( "string gridfile   = %1").arg(meshname)<< endl;
		stream << "int nVisualVariables     = 8;" << endl
			<< "int isPlotVolumerField   = 0;" << endl
			<< "int visualVariables[]    = [0, 1, 2, 3, 4, 5, 6, 15];" << endl
			<< "int reconmeth            = 1;" << endl
			<< "int limitVariables       = 0;" << endl
			<< "int limitVector          = 0;" << endl;
	}

}
