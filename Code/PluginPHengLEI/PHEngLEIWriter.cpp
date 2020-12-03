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
			//�޸Ĳ�д��
			QFile writeFile(keyfile);
			if (writeFile.open(QIODevice::WriteOnly | QIODevice::Text))
			{
				QTextStream stream(&writeFile);
				strList = strAll.split("\n");           //�Ի��з�Ϊ��׼�ָ��ı�
				for (int i = 0; i < strList.count(); i++)    //����ÿһ�� 
				{
					QString oneline = strList.at(i);
					if (oneline.contains("nsimutask"))    //�޸ĵ�����	
					{
						QString tempStr = QString("int    nsimutask      = 3;");
						oneline.replace(0, oneline.length(), tempStr);   //Ҫ�滻������
						stream << oneline << '\n';

					}
					else if (oneline.contains("parafilename"))
					{
						QString tempStr = QString("string parafilename   = \"./bin/partition.hypara\";");
						oneline.replace(0, oneline.length(), tempStr);   //��Ҫ�滻������
						stream << oneline << '\n';
					}
					//���û���ҵ�Ҫ�滻�����ݣ��ճ�д��
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
			//�޸Ĳ�д��
			QFile writeFile(keyfile);
			if (writeFile.open(QIODevice::WriteOnly | QIODevice::Text))
			{
				QTextStream stream(&writeFile);
				strList = strAll.split("\n");           //�Ի��з�Ϊ��׼�ָ��ı�
				for (int i = 0; i < strList.count(); i++)    //����ÿһ��
				{
					QString oneline = strList.at(i);
					if (oneline.contains("nsimutask"))    //�޸ĵ�����	
					{
						QString tempStr = QString("int    nsimutask      = 0;");
						oneline.replace(0, oneline.length(), tempStr);   //Ҫ�滻������
						stream << oneline << '\n';

					}
					else if (oneline.contains("parafilename"))
					{
						QString tempStr = QString("string parafilename   = \"./bin/cfd_para_subsonic.hypara\";");
						oneline.replace(0, oneline.length(), tempStr);   //��Ҫ�滻������
						stream << oneline << '\n';
					}
					//���û���ҵ�Ҫ�滻�����ݣ��ճ�д��
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
		//��زв��ļ�����
		QFileSystemWatcher m_pSystemWatcher;
		m_pSystemWatcher.addPath(model->getPath() + "/results/res.dat");
		QObject::connect(m_pSystemWatcher, SIGNAL(fileChanged(QString)), this, SLOT(fileUpdated(QString)));
*/

		if(!Mpi)
			solverControl->startSolver();
		else
		{
			DataProperty::ParameterBase* pd = model->getParameterByName(QString::fromLocal8Bit("������Ŀ"));
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
			DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("�ռ�ά��"));
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
			DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("��������"));
			DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
			if (se != nullptr) stream << "int  gridtype       = " << se->getCurrentIndex() << ";" << endl;
		}
		{
			DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("������"));
			DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
			if (se != nullptr) stream << "int  axisup       = " << se->getCurrentIndex() + 1 << ";" << endl;
		}
		{
			DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("�����ʽ")); 
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
		DataProperty::ParameterBase* pmesh = model->getParameterByName(QString::fromLocal8Bit("������ʽ"));
		DataProperty::ParameterSelectable* semesh = dynamic_cast<DataProperty::ParameterSelectable*>(pmesh);
		isSerial = !semesh->getCurrentIndex();

		DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("��������"));
		DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
		if (se != nullptr) stream << "int  pgridtype       = " << se->getCurrentIndex() << ";" << endl;

		DataProperty::ParameterBase* pd = model->getParameterByName(QString::fromLocal8Bit("������Ŀ"));
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
		//��������
		bool isStr{ false };
		{
			DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("��������"));
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

		// ������������QDir�Ĺ��˲�������������ʾ�ռ������ļ���Ŀ¼���Ҳ�����"."��".."Ŀ¼��
		// ��Ϊֻ��Ҫ������һ�㼴�ɣ����Ե��ĸ�������QDirIterator::NoIteratorFlags
		QDirIterator DirsIterator(temp_path, QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
		while (DirsIterator.hasNext())
		{
			if (!Dir.remove(DirsIterator.next())) // ɾ���ļ�����������ط���������Ŀ¼
			{
				QDir(DirsIterator.filePath()).removeRecursively(); // ɾ��Ŀ¼�����Լ����������е��ļ���Ŀ¼
			}
		}
	}

	void PHengLEIWriter::fileUpdated(QString filepath)
	{
		qDebug() << filepath;
	}

	void PHengLEIWriter::addBasicPara(ModelData::ModelDataBase* model, QTextStream& stream)
	{
		DataProperty::ParameterGroup* g = model->getParameterGroupByName(QString::fromLocal8Bit("��������"));
		if (g != nullptr)
		{
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("�����������"));
				DataProperty::ParameterInt* se = dynamic_cast<DataProperty::ParameterInt*>(p);
				if (se != nullptr) stream << "int  intervalStepFlow       = " << se->getValue() << ";" << endl;
			}
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("����������"));
				DataProperty::ParameterInt* se = dynamic_cast<DataProperty::ParameterInt*>(p);
				if (se != nullptr) stream << "int  maxSimuStep       = " << se->getValue() << ";" << endl;
			}
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("������ʾ����"));
				DataProperty::ParameterInt* se = dynamic_cast<DataProperty::ParameterInt*>(p);
				if (se != nullptr) stream << "int  intervalStepPlot       = " << se->getValue() << ";" << endl;
			}
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("�������������"));
				DataProperty::ParameterInt* se = dynamic_cast<DataProperty::ParameterInt*>(p);
				if (se != nullptr) stream << "int  intervalStepForce       = " << se->getValue() << ";" << endl;
			}
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("�в��"));
				DataProperty::ParameterInt* se = dynamic_cast<DataProperty::ParameterInt*>(p);
				if (se != nullptr) stream << "int  intervalStepRes       = " << se->getValue() << ";" << endl;
			}
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("����Ԥ����"));
				DataProperty::ParameterBool* se = dynamic_cast<DataProperty::ParameterBool*>(p);
				if (se != nullptr) stream << "int  ifLowSpeedPrecon       = " << int(se->getValue()) << ";" << endl;
			}
		}
	}

	void PHengLEIWriter::addFlowPara(ModelData::ModelDataBase* model, QTextStream& stream)
	{
		DataProperty::ParameterGroup* g = model->getParameterGroupByName(QString::fromLocal8Bit("��������"));
		if (g != nullptr)
		{
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("�����"));
				DataProperty::ParameterDouble* se = dynamic_cast<DataProperty::ParameterDouble*>(p);
				if (se != nullptr) stream << "double  refMachNumber       = " << se->getValue() << ";" << endl;
			}
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("����"));
				DataProperty::ParameterDouble* se = dynamic_cast<DataProperty::ParameterDouble*>(p);
				if (se != nullptr) stream << "double  attackd       = " << se->getValue() << ";" << endl;
			}
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("�໬��"));
				DataProperty::ParameterDouble* se = dynamic_cast<DataProperty::ParameterDouble*>(p);
				if (se != nullptr) stream << "double  angleSlide       = " << se->getValue() << ";" << endl;
			}
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("�����߽���������"));
				DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
				if (se != nullptr) stream << "int  inflowParaType       = " << se->getCurrentIndex() << ";" << endl;
			}
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("��ŵ��"));
				DataProperty::ParameterDouble* se = dynamic_cast<DataProperty::ParameterDouble*>(p);
				if (se != nullptr) stream << "double  refReNumber       = " << se->getValue() << ";" << endl;
			}

			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("�����¶�"));
				DataProperty::ParameterDouble* se = dynamic_cast<DataProperty::ParameterDouble*>(p);
				if (se != nullptr) stream << "double  refDimensionalTemperature       = " << se->getValue() << ";" << endl;
			}
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("����߶�"));
				DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
				if (se != nullptr) stream << "int  gridUnit       = " << se->getCurrentIndex() << ";" << endl;
			}
		}
	}

	void PHengLEIWriter::addGridPara(ModelData::ModelDataBase* model, QTextStream& stream)
	{
		DataProperty::ParameterGroup* g = model->getParameterGroupByName(QString::fromLocal8Bit("������ز���"));
		if (g != nullptr)
		{
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("�ο�����(m)"));
				DataProperty::ParameterDouble* se = dynamic_cast<DataProperty::ParameterDouble*>(p);
				if (se != nullptr) stream << "double  forceRefenenceLength       = " << se->getValue() << ";" << endl;
			}

			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("�ο�չ��(m)"));
				DataProperty::ParameterDouble* se = dynamic_cast<DataProperty::ParameterDouble*>(p);
				if (se != nullptr) stream << "double  forceRefenenceLengthSpanWise    = " << se->getValue() << ";" << endl;
			}
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("�ο����(�O)"));
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
		DataProperty::ParameterGroup* g = model->getParameterGroupByName(QString::fromLocal8Bit("����ģ��"));
		if (g != nullptr)
		{
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("ճ��ģ��"));
				DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
				if (se != nullptr) stream << "int  iviscous    = " << se->getCurrentIndex() << ";" << endl;
			}
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("ճ������"));
				DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
				if (se != nullptr) stream << QString("string viscousName   =\"%1 \"").arg(se->valueToString()) << ";" << endl;
			}
			/*{
			DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("DES����"));
			DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
			if (se != nullptr) stream << "int  DESType    = " << se->getCurrentIndex() << ";" << endl;
			}*/
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("ROE����������"));
				DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
				if (se != nullptr) stream << "int  roeEntropyFixMethod    = " << se->getCurrentIndex() + 1 << ";" << endl;
			}
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("ROE������ϵ��"));
				DataProperty::ParameterDouble* se = dynamic_cast<DataProperty::ParameterDouble*>(p);
				if (se != nullptr) stream << "double  roeEntropyScale       = " << se->getValue() << ";" << endl;
			}
		}
	}

	void PHengLEIWriter::SpatialStruct(ModelData::ModelDataBase* model, QTextStream& stream, bool isStr)
	{
		if (isStr)
		{
			DataProperty::ParameterGroup* g = model->getParameterGroupByName(QString::fromLocal8Bit("�ṹ���������"));
			if (g != nullptr)
			{
				{
					DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("��ճ����ɢ����"));
					DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
					if (se != nullptr) stream << QString("string inviscidSchemeName   =\"%1 \"").arg(se->valueToString()) << ";" << endl;
				}
			{
				DataProperty::ParameterBase* p = g->getParameterByName(QString::fromLocal8Bit("����������"));
				DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
				if (se != nullptr) stream << QString("string str_limiter_name   =\"%1 \"").arg(se->valueToString()) << ";" << endl;
			}
			}
		}
		//�ռ���ɢ��-�ǽṹ�����
		else{
			DataProperty::ParameterGroup* g = model->getParameterGroupByName(QString::fromLocal8Bit("����ģ��"));
			if (g != nullptr)
			{
				{
					DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("�ǽṹ��ճ����ɢ����"));
					DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
					if (se != nullptr) stream << QString("string uns_scheme_name   =\"%1 \"").arg(se->valueToString()) << ";" << endl;
				}
			{
				DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("�ǽṹ����������"));
				DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
				if (se != nullptr) stream << QString("string uns_limiter_name   =\"%1 \"").arg(se->valueToString()) << ";" << endl;
			}
			{
				DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("vancat������ϵ��"));
				DataProperty::ParameterDouble* se = dynamic_cast<DataProperty::ParameterDouble*>(p);
				if (se != nullptr) stream << "double  venkatCoeff       = " << se->getValue() << ";" << endl;
			}
			}
		}

	}

	void PHengLEIWriter::addTimeDiscretePara(ModelData::ModelDataBase* model, QTextStream& stream)
	{
		{
			DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("��������"));
			DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
			if (se != nullptr) stream << "int  iunsteady       = " << se->getCurrentIndex() << ";" << endl;
		}
	{
		DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("������"));
		DataProperty::ParameterDouble* se = dynamic_cast<DataProperty::ParameterDouble*>(p);
		if (se != nullptr) stream << "double  CFLEnd       = " << se->getValue() << ";" << endl;
	}
	{
		DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("Lusgsǰ��ɨ�貽��"));
		DataProperty::ParameterInt* se = dynamic_cast<DataProperty::ParameterInt*>(p);
		if (se != nullptr) stream << "int  nLUSGSSweeps       = " << se->getValue() << ";" << endl;
	}
	{
		DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("�����������"));
		DataProperty::ParameterSelectable* se = dynamic_cast<DataProperty::ParameterSelectable*>(p);
		if (se != nullptr) stream << "int  nMGLevel       = " << se->getCurrentIndex() + 1 << ";" << endl;
	}
	{
		DataProperty::ParameterBase* p = model->getParameterByName(QString::fromLocal8Bit("������ʼ������"));
		DataProperty::ParameterInt* se = dynamic_cast<DataProperty::ParameterInt*>(p);
		if (se != nullptr) stream << "int  flowInitStep       = " << se->getValue() << ";" << endl;
	}
	}

	void PHengLEIWriter::addOtherPara(ModelData::ModelDataBase* model,QTextStream& stream)
	{
		//·������,���ӻ��������,������������
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
