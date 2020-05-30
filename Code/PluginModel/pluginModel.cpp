#include "pluginModel.h"
#include "mainWindow/mainWindow.h"
#include "model.h"
#include "tree.h"
#include <QString>
#include "ModelData/modelDataFactory.h"
#include "ModelData/modelDataSingleton.h"
#include "ModelData/modelDataBase.h"
#include "MainWidgets/ProjectTreeFactory.h"
#include "IO/IOConfig.h"
#include "Material/MaterialFactory.h"
#include "Material/Material.h"
#include "MReader.h"
#include <QDebug>
#include "meshData/meshSet.h"
#include "meshData/meshKernal.h"
#include <vtkPoints.h>
#include <vtkCell.h>
#include <vtkIdList.h>
#include <QTextStream>
#include <QFileDialog>
#include "settings/busAPI.h"
#include "BCBase/BCVectorBase.h"
#include "BCBase/BCDisplacement.h"
#include "ModelData/ModelDataBaseExtend.h"

void Register(GUI::MainWindow* m, QList<Plugins::PluginBase*>* ps)
{
	Plugins::PluginBase* p = new Plugins::ModelPlugin(m);
	ps->append(p);
}

namespace Plugins
{
	ModelPlugin::ModelPlugin(GUI::MainWindow* m)
	{
		_mainwindow = m;
		_describe = "model Test plugin";
	}

	ModelPlugin::~ModelPlugin()
	{

	}

	bool ModelPlugin::install()
	{
// 		ModelData::ModelDataFactory::registerType(PluginDefType+1, "PluginType0001", CreateModel);
// 		MainWidget::ProjectTreeFactory::registerType(PluginDefType + 1, CreateTree);
// 
 		IO::IOConfigure::RegisterInputFile("out", WriteOut);
// 		IO::IOConfigure::RegisterOutputTransfer("trans", transfer);
//		IO::IOConfigure::RegisterMeshImporter("m", importMeshFun);
// 		IO::IOConfigure::RegisterMeshExporter("iii", exportMeshFun);
// 
// 		Material::MaterialFactory::registerType("ma1", createMaterial);
		return true;
	}

	bool ModelPlugin::uninstall()
	{
// 		ModelData::ModelDataFactory::removeType(PluginDefType + 1);
// 		MainWidget::ProjectTreeFactory::removeType(PluginDefType + 1);
// 
// 		IO::IOConfigure::RemoveInputFile("out");
// 		IO::IOConfigure::RemoveOutputTransfer("trans");
		IO::IOConfigure::RemoveMeshImporter("m");
// 		IO::IOConfigure::RemoveMeshExporter("iii");
 
// 		Material::MaterialFactory::removeType("ma1");
		return true;
	}
}

bool MODELPLUGINAPI importMeshFun(QString file)
{
	Plugins::MReader reader(file);
	bool ok = reader.read();
	return ok;
}

// bool CreateModel(int t, QPair<int, ModelData::ModelDataBase*>* p)
// {
// 	if (t == ProjectTreeType::PluginDefType + 1)
// 	{
// 		auto m = new ModelData::ModelDataPlugin((ProjectTreeType)t);
// 		p->first = t;
// 		p->second = m;
// 		return true;
// 	}
// 	return false;
// 		
// 
// }

// bool CreateTree(int tp, GUI::MainWindow* m, QPair<int, ProjectTree::ProjectTreeBase*>* t)
// {
// 	if (tp == PluginDefType + 1)
// 	{
// 		t->first = tp;
// 		t->second = new ProjectTree::PluginTree(m);
// 		return true;
// 	}
// 	return false;
// }

bool WriteOut(QString path, ModelData::ModelDataBase* d)
{	
	ModelData::ModelDataBaseExtend *extendPtr = static_cast<ModelData::ModelDataBaseExtend*>(d);
	QList<int> msIdList = extendPtr->getMeshSetList();
	if (msIdList.size() < 1)	return false;
	auto md = MeshData::MeshData::getInstance();
	QStringList mkIdList{};
	QMultiHash<int, int> CellIndexMatertID{};

	for (int setId : msIdList)
	{		
		auto ms = md->getMeshSetByID(setId);
		QList<int> mkId = ms->getKernals();
		if (mkId.size() < 1)	continue;
		mkIdList << QString::number(mkId[0]);

		QList<int> CellIndexs = ms->getKernalMembers(mkId[0]);
		for (int i = 0; i < CellIndexs.size(); i++)
		{
			if (extendPtr->isMaterialSetted(setId))
				CellIndexMatertID.insert(CellIndexs[i], extendPtr->getMaterialID(setId));
		}	
	}
	mkIdList.removeDuplicates();
	QList<vtkDataSet*> dataList{};
	for (QString mkId : mkIdList)
	{
		vtkDataSet* grid = md->getKernalByID(mkId.toInt())->getMeshData();
		dataList.append(grid);
	}
	QString workingDir = Setting::BusAPI::instance()->getWorkingDir();
	if (savePointsFile(dataList, workingDir) &&
		saveCellsFile(dataList, workingDir, CellIndexMatertID) &&
		saveBCFile(extendPtr, workingDir, md))
		return true;
	return false;
}

bool savePointsFile(QList<vtkDataSet*> dataList, QString& workingDir)
{
	QString filePath = workingDir + "/Nodes.txt";		
	QFile file(filePath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))	return false;
	QTextStream out(&file);
	for (vtkDataSet* grid : dataList)
	{
		int count = grid->GetNumberOfPoints();
		if (count <	 1)	continue;
		out << QString("   %1").arg(count) << endl << endl;
		int index{};
		while (index < count)
		{
			double* array = grid->GetPoint(index);			
			out << QString("   %1   %2   %3   %4").arg(index + 1).arg(array[0]).arg(array[1]).arg(array[2]) << endl;			
			index++;
		}
		out << endl;
	}
	file.close();
	return true;
}

bool saveCellsFile(QList<vtkDataSet*> dataList, QString& workingDir, QMultiHash<int, int>& CellIndexMatertID)
{
	QString filePath = workingDir + "/Elements.txt";
	QFile file(filePath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))	return false;
	QTextStream out(&file);
	for (int i = 0; i < dataList.size(); i++)
	{	
		vtkDataSet* grid = dataList.at(i);
		int count = grid->GetNumberOfCells();
		if (count < 1)	continue;
		
		int index{}, hexahedronCount{};
		int startPos = out.pos();
		out.seek(startPos + 10);

		while (index < count)
		{
			vtkCell* cell = grid->GetCell(index);
			if (cell == nullptr) continue;
			if (cell->GetCellType() == VTK_HEXAHEDRON)
			{			
				hexahedronCount++;
				vtkIdList* idIndexs = cell->GetPointIds();
				int value = CellIndexMatertID.value(index);
				if(value == 0)
					out << QString("   %1 %2   %3   %4   %5   %6   %7   %8   %9   %10").arg(hexahedronCount).arg(-1).arg(idIndexs->GetId(0) + 1).arg(idIndexs->GetId(1) + 1).arg(idIndexs->GetId(2) + 1).arg(idIndexs->GetId(3) + 1).arg(idIndexs->GetId(4) + 1).arg(idIndexs->GetId(5) + 1).arg(idIndexs->GetId(6) + 1).arg(idIndexs->GetId(7) + 1) << endl;
				else
					out << QString("   %1 %2   %3   %4   %5   %6   %7   %8   %9   %10").arg(hexahedronCount).arg(value).arg(idIndexs->GetId(0) + 1).arg(idIndexs->GetId(1) + 1).arg(idIndexs->GetId(2) + 1).arg(idIndexs->GetId(3) + 1).arg(idIndexs->GetId(4) + 1).arg(idIndexs->GetId(5) + 1).arg(idIndexs->GetId(6) + 1).arg(idIndexs->GetId(7) + 1) << endl;
			}
			index++;
		}
		int endPos = out.pos();
		out.seek(startPos);
		out << QString("   %1").arg(hexahedronCount) << endl << endl;
		out.seek(endPos);
		out << endl;
	}
	file.close();
	return true;
}

bool saveBCFile(ModelData::ModelDataBase* d, QString& workingDir, MeshData::MeshData* md)
{
	QString filePath = workingDir + "/BCOffset.txt";
	QFile file(filePath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))	return false;
	QTextStream out(&file);

	double v[3]{};
	int n = d->getBCCount();	

	for (int i = 0; i < n; i++)
	{
		BCBase::BCDisplacement* BCptr = dynamic_cast<BCBase::BCDisplacement*>(d->getBCAt(i));				
		auto meshSet = BCptr->getMeshSet();	
		QList<int> mkId = meshSet->getKernals();
		if (mkId.size() < 1)	continue;
		QList<int> CellorPointIndex = meshSet->getKernalMembers(mkId[0]);
		BCptr->getValue(v);

		if (meshSet->getSetType() == MeshData::SetType::Node)
		{
			out << QString("   %1").arg(CellorPointIndex.size()) << endl << endl;
			for (int pointIndex : CellorPointIndex)
			{
				out << QString("   %1   ").arg(pointIndex + 1);
				if (v[0] != 0)
					out << "UX   " << v[0] << endl;
				else if (v[1] != 0)
					out << "UY   " << v[1] << endl;
				else if (v[2] != 0)
					out << "UZ   " << v[2] << endl;			
			}
		}
		else if (meshSet->getSetType() == MeshData::SetType::Element)//暂时不对, 因为还没有去重
		{
			return false;
			out << QString("   %1").arg(CellorPointIndex.size() * 8) << endl << endl;
			auto mk = md->getKernalByID(mkId[0]);
			auto grid = mk->getMeshData();
			for (int cellIndex : CellorPointIndex)
			{
				vtkCell* cell = grid->GetCell(cellIndex);
				vtkIdList* pointIndexs = cell->GetPointIds();
				if (v[0] != 0)
				{
					out << QString("   %1   %2   %3").arg(pointIndexs->GetId(0) + 1).arg("UX").arg(v[0]) << endl;
					out << QString("   %1   %2   %3").arg(pointIndexs->GetId(1) + 1).arg("UX").arg(v[0]) << endl;
					out << QString("   %1   %2   %3").arg(pointIndexs->GetId(2) + 1).arg("UX").arg(v[0]) << endl;
					out << QString("   %1   %2   %3").arg(pointIndexs->GetId(3) + 1).arg("UX").arg(v[0]) << endl;
					out << QString("   %1   %2   %3").arg(pointIndexs->GetId(4) + 1).arg("UX").arg(v[0]) << endl;
					out << QString("   %1   %2   %3").arg(pointIndexs->GetId(5) + 1).arg("UX").arg(v[0]) << endl;
					out << QString("   %1   %2   %3").arg(pointIndexs->GetId(6) + 1).arg("UX").arg(v[0]) << endl;
					out << QString("   %1   %2   %3").arg(pointIndexs->GetId(7) + 1).arg("UX").arg(v[0]) << endl;
				}
				else if (v[1] != 0)
				{
					out << QString("   %1   %2   %3").arg(pointIndexs->GetId(0) + 1).arg("UY").arg(v[1]) << endl;
					out << QString("   %1   %2   %3").arg(pointIndexs->GetId(1) + 1).arg("UY").arg(v[1]) << endl;
					out << QString("   %1   %2   %3").arg(pointIndexs->GetId(2) + 1).arg("UY").arg(v[1]) << endl;
					out << QString("   %1   %2   %3").arg(pointIndexs->GetId(3) + 1).arg("UY").arg(v[1]) << endl;
					out << QString("   %1   %2   %3").arg(pointIndexs->GetId(4) + 1).arg("UY").arg(v[1]) << endl;
					out << QString("   %1   %2   %3").arg(pointIndexs->GetId(5) + 1).arg("UY").arg(v[1]) << endl;
					out << QString("   %1   %2   %3").arg(pointIndexs->GetId(6) + 1).arg("UY").arg(v[1]) << endl;
					out << QString("   %1   %2   %3").arg(pointIndexs->GetId(7) + 1).arg("UY").arg(v[1]) << endl;
				}
				else if (v[2] != 0)
				{
					out << QString("   %1   %2   %3").arg(pointIndexs->GetId(0) + 1).arg("UZ").arg(v[2]) << endl;
					out << QString("   %1   %2   %3").arg(pointIndexs->GetId(1) + 1).arg("UZ").arg(v[2]) << endl;
					out << QString("   %1   %2   %3").arg(pointIndexs->GetId(2) + 1).arg("UZ").arg(v[2]) << endl;
					out << QString("   %1   %2   %3").arg(pointIndexs->GetId(3) + 1).arg("UZ").arg(v[2]) << endl;
					out << QString("   %1   %2   %3").arg(pointIndexs->GetId(4) + 1).arg("UZ").arg(v[2]) << endl;
					out << QString("   %1   %2   %3").arg(pointIndexs->GetId(5) + 1).arg("UZ").arg(v[2]) << endl;
					out << QString("   %1   %2   %3").arg(pointIndexs->GetId(6) + 1).arg("UZ").arg(v[2]) << endl;
					out << QString("   %1   %2   %3").arg(pointIndexs->GetId(7) + 1).arg("UZ").arg(v[2]) << endl;
				}
			}
		}		
		out << endl;
	}
	file.close();
	return true;
}

// bool transfer(QString path)
// {
// 	qDebug() << path;
// 	qDebug() << "transfer";
// 	return true;
// }

// bool MODELPLUGINAPI exportMeshFun(QString, int)
// {
// 	qDebug() << "meshExport";
// 	return true;
// }
// 
// Material::Material* createMaterial(QString t)
// {
// 	if (t != "ma1") return nullptr;
// 
// 	auto m = new Material::Material;
// 	
// 	auto p = m->appendParameter(DataProperty::Para_Double);
// 	p->setDescribe("ma Name");
// 	return m;
// }
