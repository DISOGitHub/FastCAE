#include "WriterDataConfig.h"
#include "Common.h"
#include "ModelBase.h"
#include "BoundaryModel.h"
#include "IOXml.h"
#include "DataProperty/ParameterBase.h"
#include "DataProperty/DataBase.h"
#include "DataProperty/modelTreeItemType.h"
#include "Post2DCurveModel.h"
#include "Post3DFileModel.h"
#include "ConfigOptions/PostCurve.h"

#pragma execution_character_set("utf-8")

namespace FastCAEDesigner
{
	WriteDataConfig::WriteDataConfig()
	{
		_fileName = _CONFIG_DATA_;
	}


	WriteDataConfig::~WriteDataConfig()
	{
	}

	//bcModelLlist:边界参数列表
	bool WriteDataConfig::Write(QList<ModelBase*> list)
	{
		QDomDocument doc;
		QDomProcessingInstruction instruction;
		instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
		doc.appendChild(instruction);
		QDomElement root = doc.createElement("DataConfig");
		
		int count = list.count();
		
		for (int i = 0; i < count; i++)
		{
			ModelBase* model = list.at(i);
			if (nullptr == model)
				continue;
			QList<ModelBase*> childModelList = model->GetChildList();
			WriteTreeDataPara(doc, root, childModelList, i + 1);
			WriteMonitorDataPara(doc, root, childModelList, i + 1);
			WritePostDataPara(doc, root, childModelList, i + 1);
		}

		doc.appendChild(root);
		SaveFile(doc);
		
		return true;
	}

	bool WriteDataConfig::WriteTreeDataPara(QDomDocument &doc, QDomElement &root, QList<ModelBase*> list, int number)
	{
		QMap<TreeItemType, QString> useModelDict;
		useModelDict.insert(TreeItemType::ProjectSimulationSetting, "SimlutationData");
		useModelDict.insert(TreeItemType::ProjectSolver, "SolverSetting");
		
		//int index = 1;
		int count = list.count();
		QList<ModelBase*> childList;

		for (int i = 0; i < count; i++)
		{
			ModelBase* model = list.at(i);

			if (nullptr == model)
				continue;

			TreeItemType type = (TreeItemType)model->GetType();
			QString nodeName = useModelDict[type];
			
			if (nodeName.length()<=0)
				continue;
			
			QString nodeTypeName = QString("Type%1").arg(number);
			QDomElement dataRootNode = IoXml::getInstance()->CreateElementAttribute(doc, nodeName, "TreeType", nodeTypeName);
			DataProperty::DataBase *dataBase = model->GetDataBase();

			if (nullptr != dataBase)
				dataBase->writeParameters(&doc, &dataRootNode);

			root.appendChild(dataRootNode);
			QList<ModelBase*> childListIn = model->GetChildList();
			childList.append(childListIn);
		}

		WriteTreeChildPara(doc, root, childList, number);
		return true;
	}

	bool WriteDataConfig::WriteTreeChildPara(QDomDocument &doc, QDomElement &root, QList<ModelBase*> list, int number)
	{
		QString typeName = QString("Type%1").arg(number);
		QDomElement dataBlock = doc.createElement("DataBlock");
		dataBlock.setAttribute("TreeType", typeName);

		int count = list.count();
		
		for (int i = 0; i < count; i++)
		{
			ModelBase* model = list.at(i);

			if (nullptr == model)
				continue;

			QDomElement block = doc.createElement("Block");
			block.setAttribute("ID", _index);
			block.setAttribute("TreeNode", model->GetEngName());
			DataProperty::DataBase *dataBase = model->GetDataBase();

			if (nullptr != dataBase)
				dataBase->writeParameters(&doc, &block);

			dataBlock.appendChild(block);
			_index++;

			writeTreeGrandSonPara(doc, dataBlock, model, number);
		}

		root.appendChild(dataBlock);
		return true;
	}
	//监视器相关信息写入xml
	bool WriteDataConfig::WriteMonitorDataPara(QDomDocument &doc, QDomElement &root, QList<ModelBase*> list, int number)
	{
		QString nodeTypeName = QString("Type%1").arg(number);
		QDomElement dataRootNode = IoXml::getInstance()->CreateElementAttribute(doc, "Monitor", "TreeType", nodeTypeName);

		for (int i = 0; i < list.count(); i++)
		{
			ModelBase* model = list.at(i);

			if (nullptr == model)
				continue;

			if (model->GetType() == TreeItemType::ProjectMonitor)
			{
				QList<ModelBase*> childList = model->GetChildList();
				WriteCurveDataPara(doc, dataRootNode, childList);
				break;
			}
		}

		root.appendChild(dataRootNode);
		return true;
	}
	//post相关信息写入xml
	bool WriteDataConfig::WritePostDataPara(QDomDocument &doc, QDomElement &root, QList<ModelBase*> list, int number)
	{
		QString nodeTypeName = QString("Type%1").arg(number);
		QDomElement dataRootNode = IoXml::getInstance()->CreateElementAttribute(doc, "PostConfig", "TreeType", nodeTypeName);

		for (int i = 0; i < list.count(); i++)
		{
			ModelBase* model = list.at(i);

			if (nullptr == model)
				continue;

			if (model->GetType() == TreeItemType::ProjectPost)
			{
				QList<ModelBase*> childList = model->GetChildList();
				WritePost2DDataPara(doc, dataRootNode, childList);
				WritePost3DDataPara(doc, dataRootNode, childList);
				break;
			}		
		}
		

		root.appendChild(dataRootNode);
		return true;
	}
	//curve信息写入
	bool WriteDataConfig::WritePost2DDataPara(QDomDocument &doc, QDomElement &root, QList<ModelBase*> list)
	{
		for (int i = 0; i < list.count(); i++)
		{
			ModelBase* model = list.at(i);

			if (nullptr == model)
				continue;

			if (model->GetType() == TreeItemType::ProJectPost2DGraph)
			{
				QList<ModelBase*> childList = model->GetChildList();
				WriteCurveDataPara(doc, root, childList);
				break;
			}
		}

		return true;
	}
	//vector信息写入
	bool WriteDataConfig::WritePost3DDataPara(QDomDocument &doc, QDomElement &root, QList<ModelBase*> list)
	{
		QList<ModelBase*> childList;
		QStringList fileNameList;
		QStringList nodeScalarList;
		QStringList cellScalarList;
		QStringList nodeVectorList;
		QStringList cellVectorList;

		for (int i = 0; i < list.count(); i++)
		{
			ModelBase* model = list.at(i);

			if (nullptr == model)
				continue;
			
			if (model->GetType() == TreeItemType::ProjectPost3DGraph)
			{
				childList = model->GetChildList();
				break;
			}
		}

		for (int i = 0; i < childList.count(); i++)
		{
			ModelBase* model = childList.at(i);

			QString fileName = ((Post3DFileModel*)model)->getFileName();
			QString nodeScalar = ((Post3DFileModel*)model)->getNodeScalarList().join(",");
			QString cellScalar = ((Post3DFileModel*)model)->getCellScalarList().join(",");
			QString nodeVector = ((Post3DFileModel*)model)->getNodeVectorList().join(",");
			QString cellVector = ((Post3DFileModel*)model)->getCellVectorList().join(",");

			QDomElement fileNode = IoXml::getInstance()->CreateElement(doc, "Post3DFile", fileName);
			QDomElement nodeScalarNode = IoXml::getInstance()->CreateElement(doc, "Post3DNodeScalar", nodeScalar);
			QDomElement cellScalarNode = IoXml::getInstance()->CreateElement(doc, "Post3DCellScalar", cellScalar);
			QDomElement nodeVectorNode = IoXml::getInstance()->CreateElement(doc, "Post3DNodeVector", nodeVector);
			QDomElement cellVectorNode = IoXml::getInstance()->CreateElement(doc, "Post3DCellVector", cellVector);

			root.appendChild(fileNode);
			root.appendChild(nodeScalarNode);
			root.appendChild(cellScalarNode);
			root.appendChild(nodeVectorNode);
			root.appendChild(cellVectorNode);
			// 
			// 			if (nullptr == model)
			// 				continue;
			// 
			// // 			QString fileName = ((Post3DFileModel*)model)->getFileName();
			// // 			QString nodeScalar = ((Post3DFileModel*)model)->getNodeScalarList().join(",");
			// // 			QString cellScalar = ((Post3DFileModel*)model)->getCellScalarList().join(",");
			// // 			QString nodeVector = ((Post3DFileModel*)model)->getNodeVectorList().join(",");
			// // 			QString cellVector = ((Post3DFileModel*)model)->getCellVectorList().join(",");
			// 			if (((Post3DFileModel*)model)->getFileName() != "")
			// 				fileNameList.append(((Post3DFileModel*)model)->getFileName());
			// 
			// 			if (((Post3DFileModel*)model)->getNodeScalarList().count() > 0)
			// 				nodeScalarList.append(((Post3DFileModel*)model)->getNodeScalarList().join(","));
			// 
			// 			if (((Post3DFileModel*)model)->getCellScalarList().count() > 0)
			// 				cellScalarList.append(((Post3DFileModel*)model)->getCellScalarList().join(","));
			// 
			// 			if (((Post3DFileModel*)model)->getNodeVectorList().count() > 0)
			// 				nodeVectorList.append(((Post3DFileModel*)model)->getNodeVectorList().join(","));
			// 
			// 			if (((Post3DFileModel*)model)->getCellVectorList().count() > 0)
			// 				cellVectorList.append(((Post3DFileModel*)model)->getCellVectorList().join(","));
			// 	
		}

		

		return true;
	}
	//曲线详细信息写入
	bool WriteDataConfig::WriteCurveDataPara(QDomDocument &doc, QDomElement &root, QList<ModelBase*> list)
	{
		QMap<TreeItemType, QString> useModelDict;
		useModelDict.insert(TreeItemType::ProJectPost2DGraphChild, "Post2DFile");
		useModelDict.insert(TreeItemType::ProjectMonitorChild, "File");

		for (int i = 0; i < list.count(); i++)
		{
			ModelBase* model = list.at(i);

			if (nullptr == model)
				continue;

			TreeItemType type = (TreeItemType)model->GetType();
			QString nodeName = useModelDict[type];

			if (nodeName.length() <= 0)
				continue;

			QString fileName = ((Post2DCurveModel*)model)->getFileName();
			QList<ConfigOption::PostCurve*> curveList = ((Post2DCurveModel*)model)->GetCurveList();
// 			if (curveList.isEmpty())
// 				continue;
			QDomElement fileNode = IoXml::getInstance()->CreateElementAttribute(doc, nodeName, "Name", fileName);

			QStringList stateCurve;
			stateCurve.append("Describe");
			stateCurve.append("XVariable");
			stateCurve.append("YVariable");

			if (curveList.isEmpty())/*;*/
			{

				QStringList curveValue;
				curveValue.append("");
				curveValue.append("");
				curveValue.append("");

				QMap<QString, QString> curve = insertQString(stateCurve, curveValue);
				QDomElement curveNode = IoXml::getInstance()->CreateElement(doc, "Curve", curve);
				fileNode.appendChild(curveNode);
			}
			else
			{
				for (int j = 0; j < curveList.count(); j++)
				{
					QStringList curveValue;
					curveValue.append(curveList.at(j)->getDescribe());
					curveValue.append(curveList.at(j)->getXVariable());
					curveValue.append(curveList.at(j)->getYVariable());

					QMap<QString, QString> curve = insertQString(stateCurve, curveValue);
					QDomElement curveNode = IoXml::getInstance()->CreateElement(doc, "Curve", curve);
					fileNode.appendChild(curveNode);
				}
			}
			

			root.appendChild(fileNode);
		}

		return true;
	}

	bool WriteDataConfig::writeTreeGrandSonPara(QDomDocument &doc, QDomElement &root, ModelBase* base, int number)
	{
		
		QList<ModelBase*> list = base->GetChildList();

		int count = list.count();

		for (int i = 0; i < count; i++)
		{
			ModelBase* model = list.at(i);

			if (nullptr == model)
				continue;

			QDomElement block = doc.createElement("Block");
			block.setAttribute("ID", _index);
			block.setAttribute("TreeNode", model->GetEngName());
			DataProperty::DataBase *dataBase = model->GetDataBase();

			if (nullptr != dataBase)
				dataBase->writeParameters(&doc, &block);

			root.appendChild(block);
			_index++;
		}

		return true;
	}

	QMap<QString, QString> WriteDataConfig::insertQString(QStringList var, QStringList val)
	{
		QMap<QString, QString> map;

		for (int i = 0; i < var.count(); i++)
		{
			map.insert(var.at(i), val.at(i));
		}
		return map;
	}

}