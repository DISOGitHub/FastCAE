#include "INPdataExchange.h"
#include "mainWindow/mainWindow.h"
#include "meshData/meshSingleton.h"
#include "meshData/meshKernal.h"
#include "meshData/meshSet.h"
#include "BCBase/BCUserDef.h"
#include "ModelData/modelDataSingleton.h"
#include "ModelData/modelDataBaseExtend.h"
#include "DataProperty/ParameterGroup.h"
#include "DataProperty/ParameterDouble.h"
#include "DataProperty/PropertyString.h"
#include "Material/Material.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/MaterialConfig.h"
#include "Material/MaterialFactory.h"
#include "Material/MaterialSingletion.h"
#include "moduleBase/ThreadTaskManager.h"
#include <vtkUnstructuredGrid.h>
#include <QFileInfo>
#include <QMessageBox>
#include <QDebug>
#include <omp.h>
#include <QFileDialog>

#ifdef Q_OS_WIN
#define ENDL "\r\n"
#else
#define ENDL "\n"
#endif
 
namespace MeshData
{
	INPdataExchange::INPdataExchange(const QString &fileName, MeshOperation operation, GUI::MainWindow *mw, int modelId) :
		MeshThreadBase(fileName, operation, mw),
		_fileName(fileName),
		_meshData(MeshData::getInstance()),
		_operation(operation),
		_modelId(modelId),
		_mw(mw)
	{
		if (modelId != -1)
		{
			ModelData::ModelDataBase* model = ModelData::ModelDataSingleton::getinstance()->getModelByID(modelId);
			_Case = dynamic_cast<ModelData::ModelDataBaseExtend*>(model);
		}
	}
	INPdataExchange::~INPdataExchange()
	{
		if (_stream != nullptr) delete _stream;
	}

// 	void INPdataExchange::destroyThread()
// 	{
// 		ThreadTask::destroyThread();
// 		if (_stream != nullptr) delete _stream;
// 	}

	void INPdataExchange::readLine(QString &line)
	{
		while (_threadRuning && !_stream->atEnd())
		{
			//xuxinwie  20200519
// 			if (_stream->atEnd())
// 			{
// 				//_threadRuning = false;
// 				break;
// 			}

			line = _stream->readLine().simplified().toLower();
			if (line.startsWith("**") && line.size() == 2) continue;
			break;
		}
	}

	bool INPdataExchange::read()
	{
		QFileInfo info(_fileName);
		if (!info.exists()) return false;
		QString name = info.fileName();
		QString path = info.filePath();
		QFile file(_fileName);
		if (!file.open(QIODevice::ReadOnly)) return false;
		_stream = new QTextStream(&file);
		vtkSmartPointer<vtkUnstructuredGrid> dataset = vtkSmartPointer<vtkUnstructuredGrid>::New();
		QString line;
	
		QList<int> inpSetIds;
		QStringList materialName, density, elastic;
		QStringList bcSetIds, bcName, bcType;
		QList<double> displacement, rotation;

		do
		{
			if (!_threadRuning)
			{
				file.close();
				return false;
			}
			this->readLine(line);
			if (line.startsWith("*node") && line.size() < 6)
			{
				if (!readNodes(dataset, line))
				{
					dataset->Delete();
					file.close();
					return false;
				}
			}
			if (line.startsWith("*element"))
			{
				if (!readElements(dataset, line))
				{
					dataset->Delete();
					file.close();
					return false;
				}

				if (dataset != nullptr)
				{
					MeshKernal* k = new MeshKernal;
					k->setName(name);
					k->setPath(path);
					k->setMeshData((vtkDataSet*)dataset);
					_meshData->appendMeshKernal(k);
				}
			}
			if (line.startsWith("*nset"))
			{
				this->readNSet(line, inpSetIds);
			}
			if (line.startsWith("*elset"))
			{
				this->readElSet(line, inpSetIds);
			}

			//xuxinwie  20200519
			//if (_stream->atEnd())
			//	_threadRuning = false;
			if (_modelId == -1)    continue;
			if (line.startsWith("*material"))
				readMaterial(line, materialName, density, elastic);
			if (line.startsWith("** boundary"))
				readBoundary(line, bcSetIds, bcName, bcType, displacement, rotation);

		} while (_threadRuning && !_stream->atEnd());		

		if (_modelId != -1)
		{	
			addINPComponents(inpSetIds);
			addINPMaterials(materialName, density, elastic);
			addINPBCs(bcSetIds, bcName, bcType, displacement, rotation);
			emit _mw->updatePhysicsTreeSignal();
		}
		file.close();
		return true;
	}

	bool INPdataExchange::write()
	{
		QFile file(_fileName);
		if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly))
			return false;
		_stream = new QTextStream(&file);

		if (_modelId == -1)
		{
			//点击状态栏按钮导出, 融合所有kernal并导出为INP文件
			file.close();
			return false;
		}
		else
		{	
			//点击Case中菜单导出, 仅导出与Model绑定的一个kernal为INP文件
			if (_Case->getMeshKernalList().size() != 1)
			{
				file.close();
				return false;
			}

			int kId = _Case->getMeshKernalList().at(0);
			vtkDataSet* data = _meshData->getKernalByID(kId)->getMeshData();
			if (!data)
			{
				file.close();
				return false;
			}
			writePoint(data);
			writeCell(data);
			writeComponent(kId);
			writeMaterial();
			writeBoundary();
		}
		file.close();
		return true;
	}

	bool INPdataExchange::readNodes(vtkUnstructuredGrid* g, QString &line)
	{
		vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
		bool ok = false;
		int index = 0;
		while (_threadRuning && !_stream->atEnd())
		{
			if (!_threadRuning)    return false;
			readLine(line);
			if (line.contains("*")) break;

			double coor[3] = { 0 };
			QStringList scoor = line.split(",");
			if (scoor.size() != 4) continue;
			for (int j = 0; j < 3; ++j)
			{
				coor[j] = scoor.at(j + 1).toDouble(&ok);
				if (!ok) return false;
			}

			points->InsertNextPoint(coor);
			int id = scoor.at(0).toInt(&ok);
			if (ok) _nodeIDIndex[id] = index;
			index++;


			//xuxinwie  20200519
// 			if (_stream->atEnd())
// 				_threadRuning = false;
		}

		if (points->GetNumberOfPoints() > 2)
		{
			g->SetPoints(points);
			return true;
		}
		return false;
	}

	bool INPdataExchange::readElements(vtkUnstructuredGrid* g, QString &line)
	{
		QStringList stype = line.split(",");
		//if (stype.size() != 2) return false;
		QString st = stype.at(1);
		st.remove("type=").remove(" ").simplified();
		VTKCellType tp = VTK_EMPTY_CELL;
//		qDebug() << st;
		if (st == "c3d4")
		{
			tp = VTK_TETRA;
		}

		if (tp == VTK_EMPTY_CELL) return false;

		bool ok = false;
		int index = 0;

		while (_threadRuning && !_stream->atEnd())
		{
			if (!_threadRuning)    return false;
			this->readLine(line);
			if (line.startsWith("*")) break;
			vtkSmartPointer<vtkIdList> indexList = vtkSmartPointer<vtkIdList>::New();

			QStringList sids = line.split(",");
			for (int j = 1; j < sids.size(); ++j)
			{
				int nodeid = sids.at(j).toInt(&ok);
				assert(ok);
				int nodeindex = _nodeIDIndex.value(nodeid);
				indexList->InsertNextId(nodeindex);
			}
			
			g->InsertNextCell(tp, indexList);
			int eleID = sids.at(0).toInt(&ok);
			_elemIDIndex[eleID] = index;
			index++;


			//xuxinwie  20200519
// 			if (_stream->atEnd())
// 				_threadRuning = false;
		}
		return true;		
	}

	bool INPdataExchange::readNSet(QString &line, QList<int>& inpSetIds)
	{
		bool isgen = false;
		if (line.contains("generate")) isgen = true;

		QStringList sinfo = line.split(",");
		QString name = sinfo.at(1).simplified();
		name.remove("nset=");

		if (isgen)
			name = name + "_gen";
		
		MeshSet* set = new MeshSet(name, Node);
		const int c = _meshData->getKernalCount();
		if (c <= 0)return false;
		MeshKernal* k = _meshData->getKernalAt(c - 1);
		if (k == nullptr) return false;
// 		vtkDataSet* dataset = k->getMeshData();
// 		set->setDataSet(dataset);
		const int kid = k->getID();
		
//		vtkSmartPointer<vtkIdTypeArray> array = vtkSmartPointer<vtkIdTypeArray>::New();
		while (_threadRuning && !_stream->atEnd())
		{
			if (!_threadRuning)    return false;
			this->readLine(line);
			if (line.startsWith("*"))
			{
//				set->setIDList(array);
				_meshData->appendMeshSet(set);
				inpSetIds.append(set->getID());
			}
			if (line.startsWith("*nset"))
			{
				this->readNSet(line, inpSetIds);
			}
			if (line.startsWith("*elset"))
			{
				this->readElSet(line, inpSetIds);
			}
			if (line.startsWith("*") && (!line.startsWith("*nset")))
				return true;
			
			QStringList sid = line.split(",");
			if (isgen)
			{
				int beg = sid.at(0).toInt();
				int end = sid.at(1).toInt();
				for (int i = beg; i <= end; ++i)
				{
					if (!_nodeIDIndex.contains(i)) continue;
					int index = _nodeIDIndex.value(i);
//					array->InsertNextValue(index);
					set->appendMember(kid, index);
				}
				continue;
			}

			for (int i = 0; i < sid.size(); ++i)
			{
				if(sid.at(i).size() == 0)    continue;
				int id = sid.at(i).toInt();
				int index = _nodeIDIndex.value(id);
//				array->InsertNextValue(index);
				set->appendMember(kid, index);
			}
			//xuxinwie  20200519
// 			if (_stream->atEnd())
// 				_threadRuning = false;
		}
	}

	bool INPdataExchange::readElSet(QString &line, QList<int>& inpSetIds)
	{
		bool isgen = false;
		if (line.contains("generate")) isgen = true;

		QStringList sinfo = line.split(",");
		QString name = sinfo.at(1).simplified();
		name.remove("elset=");

		if (isgen)
			name = name + "_gen";

		MeshSet* set = new MeshSet(name, Element);
		const int c = _meshData->getKernalCount();
		if (c <= 0)return false;
		MeshKernal* k = _meshData->getKernalAt(c - 1);
		if (k == nullptr) return false;
//		vtkDataSet* dataset = k->getMeshData();
//		set->setDataSet(dataset);
		const int kid = k->getID();

//		vtkSmartPointer<vtkIdTypeArray> array = vtkSmartPointer<vtkIdTypeArray>::New();
		while (_threadRuning && !_stream->atEnd())
		{
			if (!_threadRuning) return false;
			this->readLine(line);
			if (line.startsWith("*"))
			{
//				set->setIDList(array);
				_meshData->appendMeshSet(set);
				inpSetIds.append(set->getID());
			}
			if (line.startsWith("*elset"))
			{
				this->readElSet(line, inpSetIds);
			}
			if (line.startsWith("*nset"))
			{
				this->readNSet(line, inpSetIds);
			}
			if (line.startsWith("*") && (!line.startsWith("*elset")))
				return true;

			QStringList sid = line.split(",");
			if (isgen)
			{
				int beg = sid.at(0).toInt();
				int end = sid.at(1).toInt();
				for (int i = beg; i <= end; ++i)
				{
					if (!_elemIDIndex.contains(i)) continue;
					int index = _elemIDIndex.value(i);
//					array->InsertNextValue(index);
					set->appendMember(kid, index);
				}
				continue;
			}

			for (int i = 0; i < sid.size(); ++i)
			{
				if (sid.at(i).size() == 0)    continue;
				int id = sid.at(i).toInt();
				int index = _elemIDIndex.value(id);
//				array->InsertNextValue(index);
				set->appendMember(kid, index);
			}

			//xuxinwie  20200519
// 			if (_stream->atEnd())
// 				_threadRuning = false;
		}
	}

	bool INPdataExchange::readMaterial(QString &line, QStringList& materialName, QStringList& density, QStringList& elastic)
	{
		if (line.contains("** boundary"))
			return true;
		materialName.append(line.split('=').at(1));
		while (_threadRuning && !_stream->atEnd())
		{
			if (!_threadRuning) return false;
			readLine(line);
			readLine(line);
			density.append(line.left(line.size() - 1));
			readLine(line);
			readLine(line);
			if (line.contains(".,"))
				elastic.append(line.remove(".,"));
			else
				elastic.append(line.remove(','));

			readLine(line);
			if (line.startsWith("*material"))
				readMaterial(line, materialName, density, elastic);
			if (line.contains("** boundary"))
				return true;
		}
		return false;
	}

	bool INPdataExchange::readBoundary(QString &line, QStringList& bcSetIds, QStringList& bcName, QStringList& bcType, QList<double>& displacement, QList<double>& rotation)
	{
		if (line.startsWith("** output"))
			return true;

		QStringList lineList;
		auto bcConfig = ConfigOption::ConfigOption::getInstance()->getBCConfig();
		while (_threadRuning && !_stream->atEnd())
		{
			if (!_threadRuning) return false;
			if (!line.startsWith("** name"))
			{
				readLine(line);
				continue;
			}
			else
			{
				lineList = line.split(' ');
				bcName.append(lineList.at(2));
				bcType.append(lineList.at(4));				
				readLine(line);
			}

			QStringList lineList;
			for (int i = 0; i < 6; i++)
			{
				readLine(line);
				lineList = line.split(',');

				auto set = _meshData->getMeshSetByName(lineList.at(0));
				if (!set)	continue;
				bcSetIds.append(QString::number(set->getID()));

				if ((i == 0 || i == 1 || i == 2) && lineList.size() == 3)
					displacement << 0;
				else if ((i == 0 || i == 1 || i == 2) && lineList.size() == 4)
					displacement << lineList.at(3).toDouble();
				else if ((i == 3 || i == 4 || i == 5) && lineList.size() == 3)
					rotation << 0;
				else if ((i == 3 || i == 4 || i == 5) && lineList.size() == 4)
					rotation << lineList.at(3).toDouble();
			}
			bcSetIds.removeDuplicates();

			readLine(line);
			if (line.startsWith("** name"))
				readBoundary(line, bcSetIds, bcName, bcType, displacement, rotation);
			if (line.startsWith("** output"))
				return true;
		}
		return false;
	}

	void INPdataExchange::addINPComponents(const QList<int>& inpSetIds)
	{
		if (inpSetIds.size() <= 0)	return;
		auto kId = _meshData->getMeshSetByID(inpSetIds.at(0))->getKernals();
		_Case->setMeshKernelList(kId);
		_Case->setComponentIDList(inpSetIds);
	}

	void INPdataExchange::addINPMaterials(const QStringList& materialName, const QStringList& density, const QStringList& elastic)
	{
		QList<int> inpMaIds;
		QString stype = QObject::tr("INP Material");
//#pragma omp parallel for firstprivate(stype)
		auto materSteWard = Material::MaterialSingleton::getInstance();
		for (int i = 0; i < materialName.size(); i++)
		{
// 			qDebug() << stype;
// 			qDebug() << omp_get_thread_num();
// 			qDebug() << _materialName;

			QStringList elasticList = elastic.at(i).split(" ");
			Material::Material* material = new Material::Material;
			material->setType(stype);
			material->setName(materialName.at(i));
			material->appendProperty(QObject::tr("Density"), density.at(i));
			material->appendProperty(QObject::tr("Elasticity Modulus"), QString(elasticList.at(0)));
			material->appendProperty(QObject::tr("Poisson Ratio"), QString(elasticList.at(1)));
			materSteWard->appendMaterial(material);
			inpMaIds << material->getID();
		}
		_Case->bindInpMaterialIds(inpMaIds);
	}

	void INPdataExchange::addINPBCs(const QStringList& bcSetIds, const QStringList& bcName, const QStringList& bcType, const QList<double>& displacement, const QList<double>& rotation)
	{
		QStringList paraDescribes;
		paraDescribes << "X" << "Y" << "Z";

		for(int i = 0; i < bcSetIds.size(); i++)
		{
			BCBase::BCUserDef* bc = new BCBase::BCUserDef;
			bc->bingdingComponentID(bcSetIds.at(i).toInt());
			bc->setName(bcName.at(i));
			_Case->appeendBC(bc);
			
			QString qdis, qrot;
			DataProperty::ParameterGroup* pagDis = new DataProperty::ParameterGroup;
			DataProperty::ParameterGroup* pagRot = new DataProperty::ParameterGroup;
			pagDis->setDescribe(bcType.at(i).split('/').at(0));
			pagRot->setDescribe(bcType.at(i).split('/').at(1));

			for (int j = 0; j < 3; j++)
			{
				qdis = QString("Displacement%1").arg(paraDescribes.at(j));
				qrot = QString("Rotation%1").arg(paraDescribes.at(j));

				auto dispara = new DataProperty::ParameterDouble;
				auto rotpara = new DataProperty::ParameterDouble;

				dispara->setDataID(_modelId);
				rotpara->setDataID(_modelId);

				dispara->setDescribe(qdis);
				rotpara->setDescribe(qrot);

				dispara->setValue(displacement.at(3 * i + j));
				rotpara->setValue(rotation.at(3 * i + j));

				pagDis->appendParameter(dispara);
				pagRot->appendParameter(rotpara);
			}

			bc->appendParameterGroup(pagDis);
			bc->appendParameterGroup(pagRot);
			bc->generateParaInfo();
		}
	}

	void INPdataExchange::writePoint(vtkDataSet* data)
	{
		*_stream << "*Node" << endl;
		int pIndex = 0;
		int nPoint = data->GetNumberOfPoints();
		double* xyz = NULL;
		QString qPointIdxyz;
		while (pIndex < nPoint)
		{
			if (!_threadRuning)    return;
			xyz = data->GetPoint(pIndex);
			pIndex++;
			qPointIdxyz.append(QString::number(pIndex) + ",");
			qPointIdxyz.append(QString::number(xyz[0]) + ",");
			qPointIdxyz.append(QString::number(xyz[1]) + ",");
			qPointIdxyz.append(QString::number(xyz[2]) + ENDL);

			if (qPointIdxyz.size() > 1024)
			{
				*_stream << qPointIdxyz;
				qPointIdxyz.clear();
			}
			if (pIndex == nPoint && qPointIdxyz.size() > 0)
				*_stream << qPointIdxyz;
		}
	}

	void INPdataExchange::writeCell(vtkDataSet* data)
	{
		*_stream << "*Element, type=";
		int nCell = data->GetNumberOfCells();
		if (nCell < 0)    return;
		if (data->GetCell(0)->GetCellType() == VTK_TETRA)
			*_stream << "C3D4" << endl;

		int cIndex = 0;
		QString qCellIdPointIds;
		while (cIndex < nCell)
		{
			if (!_threadRuning)    return;
			qCellIdPointIds.append(QString::number(cIndex + 1) + ",");
			auto ptIdIndexs = data->GetCell(cIndex)->GetPointIds();
			qCellIdPointIds.append(QString::number(ptIdIndexs->GetId(0) + 1) + ",");
			qCellIdPointIds.append(QString::number(ptIdIndexs->GetId(1) + 1) + ",");
			qCellIdPointIds.append(QString::number(ptIdIndexs->GetId(2) + 1) + ",");
			qCellIdPointIds.append(QString::number(ptIdIndexs->GetId(3) + 1) + ENDL);

			if (qCellIdPointIds.size() > 1024)
			{
				*_stream << qCellIdPointIds;
				qCellIdPointIds.clear();
			}
			cIndex++;
			if (cIndex == nCell && qCellIdPointIds.size() > 0)
				*_stream << qCellIdPointIds;
		}
	}

	void INPdataExchange::writeComponent(int kId)
	{
		QList<int> inpSetIds = _Case->getComponentIDList();
		int nSet = inpSetIds.size();
		if (nSet == 0)    return;

		QString qSet, inpSetName;
		int index = 0, nMember = 0, head = -1, tail = -1;
		QList<int> members;

		while (index < nSet)
		{
			if (!_threadRuning)    return;
			auto inpSet = _meshData->getMeshSetByID(inpSetIds.at(index));
			index++;
			if (inpSet->getSetType() == Node)
				qSet.append("*Nset,nset=");
			else if (inpSet->getSetType() == Element)
				qSet.append("*Elset,elset=");

			members = inpSet->getKernalMembers(kId);
			nMember = members.size();
			if (nMember <= 0)    
				continue;

			head = members.at(0) + 1;
			tail = members.at(nMember - 1) + 1;
			if (head > tail)
			{
				int temp = head;
				head = tail;
				tail = temp;
			}

			inpSetName = inpSet->getName().toUpper();
			if (inpSetName.endsWith("_GEN"))
				endsWithGEN(inpSetName, qSet, head, tail);
			else
				notEndsWithGEN(inpSetName, qSet, nMember, members);

			if (qSet.size() > 1024)
			{
				*_stream << qSet;
				qSet.clear();
			}
			if (index == nSet && qSet.size() > 0)
				*_stream << qSet;
		}
	}

	void INPdataExchange::writeMaterial()
	{		
		auto materSteWard = Material::MaterialSingleton::getInstance();
		const QList<int> inpMaterIds = _Case->getInpMaterialIds();
		DataProperty::PropertyString* strPro = NULL;
		QString qMaterial;
		int index = 0;
		for (int inpMaterId : inpMaterIds)
		{
			index++;
			qMaterial.append("*Material,name=");

			auto inpMaterial = materSteWard->getMaterialByID(inpMaterId);
			qMaterial.append(inpMaterial->getName() + ENDL + "*Density" + ENDL);

			strPro = dynamic_cast<DataProperty::PropertyString*>(inpMaterial->getPropertyByName(QObject::tr("Density")));
			qMaterial.append(strPro->getValue() + "," + ENDL + "*Elastic" + ENDL);

			strPro = dynamic_cast<DataProperty::PropertyString*>(inpMaterial->getPropertyByName(QObject::tr("Elasticity Modulus")));
			qMaterial.append(strPro->getValue() + ", ");

			strPro = dynamic_cast<DataProperty::PropertyString*>(inpMaterial->getPropertyByName(QObject::tr("Poisson Ratio")));
			qMaterial.append(strPro->getValue() + ENDL);

			if (qMaterial.size() > 1024)
			{
				*_stream << qMaterial;
				qMaterial.clear();
			}

			if (index == inpMaterIds.size() && qMaterial.size() > 0)
				*_stream << qMaterial;
		}
	}

	void INPdataExchange::writeBoundary()
	{
		//每个set只能由一种kernal构成, 不能是两个或多个
		//导出边界时如果边界绑定的组件们所对应的kernalId必须一样,
		//因为边界中记录的是点或单元的Id, 两个不同的kernal点或单元的id会重复
		int nBC = _Case->getBCCount();
		if (nBC <= 0)    return;
		*_stream << "** BOUNDARY CONDITIONS" << endl;

		BCBase::BCUserDef* inpBC = NULL;
		DataProperty::ParameterGroup* pagDis = NULL;
		DataProperty::ParameterGroup* pagRot = NULL;
		DataProperty::ParameterDouble* paraDis = NULL;
		DataProperty::ParameterDouble* paraRot = NULL;

		QString qBC, inpSetName;
		for (int i = 0; i < nBC; i++)
		{
			inpBC = dynamic_cast<BCBase::BCUserDef*>(_Case->getBCAt(i));
			if(!inpBC || inpBC->getParameterGroupCount() != 2)    continue;
			inpSetName = _meshData->getMeshSetByID(inpBC->getComponentID())->getName();

			pagDis = inpBC->getParameterGroupAt(0);
			pagRot = inpBC->getParameterGroupAt(1);

			qBC.append("** Name: " + inpBC->getName() + " Type: ");
			qBC.append(pagDis->getDescribe() + "/" + pagRot->getDescribe() + ENDL + "*Boundary" + ENDL);

			QString qDis, qRot;
			for (int j = 0; j < 3; j++)
			{
				paraDis = dynamic_cast<DataProperty::ParameterDouble*>(pagDis->getParameterAt(j));
				paraRot = dynamic_cast<DataProperty::ParameterDouble*>(pagRot->getParameterAt(j));

				qDis.append(inpSetName + "," + QString::number(j + 1) + "," + QString::number(j + 1) + ",");
				qDis.append(QString::number(paraDis->getValue()) + ENDL);

				qRot.append(inpSetName + "," + QString::number(j + 4) + "," + QString::number(j + 4) + ",");
				qRot.append(QString::number(paraRot->getValue()) + ENDL);
			}
			qBC.append(qDis + qRot);
			if (qBC.size() > 1024)
			{
				*_stream << qBC;
				qBC.clear();
			}
			if (i == nBC - 1 && qBC.size() > 0)
				*_stream << qBC;
		}
	}

	void INPdataExchange::endsWithGEN(QString& inpSetName, QString& qSet, int head, int tail)
	{
		inpSetName.remove("_GEN");
		qSet.append(inpSetName + ",generate").append(ENDL);
		qSet.append(QString::number(head) + ",");
		qSet.append(QString::number(tail) + ",");
		qSet.append(ENDL);
	}

	void INPdataExchange::notEndsWithGEN(QString& inpSetName, QString& qSet, int nMember, const QList<int>& members)
	{
		qSet.append(inpSetName).append(ENDL);
		if (nMember == 1)
		{
			qSet.append(QString::number(members.at(0) + 1) + ",");
			qSet.append(ENDL);
			return;
		}

		int num = nMember / 16;
		for (int i = 0; i < num; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				qSet.append(QString::number(members.at(16 * i + j) + 1));
				if (j != 15)
					qSet.append(",");
			}
			qSet.append(ENDL);
			if (qSet.size() > 1024)
			{
				*_stream << qSet;
				qSet.clear();
			}
		}

		int remainder = nMember % 16;
		if (remainder != 0)
		{
			for (int m = nMember - remainder; m < nMember; m++)
			{
				qSet.append(QString::number(members.at(m) + 1));
				if (m != nMember - 1)
					qSet.append(",");
			}
			qSet.append(ENDL);
		}
	}

	void INPdataExchange::run()
	{		
		ModuleBase::ThreadTask::run();
		bool result = false;
		switch (_operation)
		{
		case MESH_READ:
			emit showInformation(tr("Importing INP Mesh File From \"%1\"").arg(_fileName));
			result = read();
			setReadResult(result);
			break;
		case MESH_WRITE:
			emit showInformation(tr("Exporting INP Mesh File To \"%1\"").arg(_fileName));
			result = write();
			setWriteResult(result);
			break;
		}
		defaultMeshFinished();
	}
}
