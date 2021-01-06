#include "WriteTreeConfig.h"
#include "IOXml.h"
#include <QFile>
#include <QCoreApplication>
#include <QDebug>
#include "Common.h"
#include "ConfigOptions/TreeConfigReader.h"
#include "ConfigOptions/TreeItemData.h"
#include "ModelBase.h"
#include "DataProperty/modelTreeItemType.h"

#pragma execution_character_set("utf-8")

namespace FastCAEDesigner{
	WriteTreeConfig::WriteTreeConfig()
	{
		_fileName = _CONFIG_TREE_;
	}


	WriteTreeConfig::~WriteTreeConfig()
	{
	}

	bool WriteTreeConfig::Write(QList<ModelBase*> list)
	{
		QDomDocument doc;
		QDomProcessingInstruction instruction;
		instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
		doc.appendChild(instruction);
		QDomElement root = doc.createElement("Treelist");
		WriteTreePara(doc, root, list);
		doc.appendChild(root);
		SaveFile(doc);
		return true;
	}

	bool WriteTreeConfig::WriteTreePara(QDomDocument &doc, QDomElement &root, QList<ModelBase*> list)
	{
		int index = 1;


		for (int i = 0; i < list.count(); i++)
		{
			ModelBase* model = list.at(i);

			if (model == nullptr)
				return false;

			QStringList sl1, sl2;
			sl1.append("Name");
			sl1.append("Chinese");
			sl1.append("Type");
			//sl1.append("OutputFile");
			sl2.append(model->GetEngName());
			sl2.append(model->GetChnName());
			sl2.append(QString("Type%1").arg(index));
			//sl2.append("zb.dat");
			index++;
			QMap<QString, QString> treeMap = insertQString(sl1, sl2);
			QDomElement tree = IoXml::getInstance()->CreateElement(doc, "Tree", treeMap);
			root.appendChild(tree);
			WriteTreeChildPara(doc, tree, model);
		}

// 		for each (ModelBase* model in list)
// 		{
// 			if (nullptr == model)
// 				continue;
// 
// 			QStringList sl1, sl2;
// 			sl1.append("Name");
// 			sl1.append("Chinese");
// 			sl1.append("Type");
// 			//sl1.append("OutputFile");
// 			sl2.append(model->GetEngName());
// 			sl2.append(model->GetChnName());
// 			sl2.append(QString("Type%1").arg(index));
// 			//sl2.append("zb.dat");
// 			index++;
// 			QMap<QString, QString> treeMap = insertQString(sl1, sl2);
// 			QDomElement tree = IoXml::getInstance()->CreateElement(doc, "Tree", treeMap);
// 			root.appendChild(tree);
// 			WriteTreeChildPara(doc, tree, model);
// 		}

		return true;
	}

	bool WriteTreeConfig::WriteTreeChildPara(QDomDocument &doc, QDomElement &root, ModelBase* model)
	{
		QMap<TreeItemType, QString> typeNameDict;
		typeNameDict.insert(TreeItemType::ProjectSimulationSettingChild, "SimulationSettingChild");
		//typeNameDict.insert(TreeItemType::ProjectBoundaryCondationChild, "BoundaryCondationChild");
		typeNameDict.insert(TreeItemType::ProjectSolverChild, "SolverChild");
		
		QMap<TreeItemType, QString> parentNameDict;
		parentNameDict.insert(TreeItemType::ProjectSimulationSettingChild, "Simulation Setting");
		//parentNameDict.insert(TreeItemType::ProjectBoundaryCondationChild, "Boundary Setting");
		parentNameDict.insert(TreeItemType::ProjectSolverChild, "Solver Setting");

		QMap<TreeItemType, QString> nodeNameDict;
		nodeNameDict.insert(TreeItemType::ProjectComponent, "Mesh Set");
		nodeNameDict.insert(TreeItemType::ProjectBoundaryCondation, "Boundary Condition");
		nodeNameDict.insert(TreeItemType::ProjectSimulationSetting, "Simulation Setting");
		nodeNameDict.insert(TreeItemType::ProjectSolver, "Solver Setting");
		nodeNameDict.insert(TreeItemType::ProjectMonitor, "Monitor");
		nodeNameDict.insert(TreeItemType::ProjectPost, "Post");
		//nodeNameDict.insert(TreeItemType::ProJectPost2DGraph, "2D Plot");
		//nodeNameDict.insert(TreeItemType::ProjectPost3DGraph, "Vector");


		QList<ModelBase*> rootNodeList = model->GetChildList();

		QStringList disableNodeList{};
		
		for (int i = 0; i < rootNodeList.count(); i++)
		{
			ModelBase* rootNode = rootNodeList.at(i);

			if (nullptr == rootNode)
				continue;

			if (rootNode->GetVisible() == false)
			{
				TreeItemType type = (TreeItemType)rootNode->GetType();
				disableNodeList.append(nodeNameDict[type]);
				//continue;
			}

			if (rootNode->GetType() == TreeItemType::ProjectPost)
			{
				QList<ModelBase*> childNodeList = rootNode->GetChildList();
				for (int j = 0; j < childNodeList.count(); j++)
				{
					ModelBase* childNode = childNodeList.at(j);

					if ((childNode->GetVisible() == false) && (childNode->GetType() == TreeItemType::ProJectPost2DGraph))
					{
						TreeItemType type = (TreeItemType)childNode->GetType();
						disableNodeList.append("2D Plot");
						//continue;
					}
					if ((childNode->GetVisible() == false) && (childNode->GetType() == TreeItemType::ProjectPost3DGraph))
					{
						TreeItemType type = (TreeItemType)childNode->GetType();
						disableNodeList.append("Counter");
						disableNodeList.append("Vector");
						//continue;
					}
				}
			}

			QList<ModelBase*> childList = rootNode->GetChildList();

			for (int j = 0; j < childList.count(); j++)
			{
				ModelBase* item = childList.at(j);

				if (nullptr == item)
					continue;

				TreeItemType type = (TreeItemType)item->GetType();

				if (typeNameDict.contains(type))
				{
					QString typeName = typeNameDict[type];
					QString parentName = parentNameDict[type];
					QStringList sl1, sl2;
					QString iconName = SaveIconToSystem(item->GetIconName());

					sl1.append("Chinese");
					sl1.append("Type");
					sl1.append("Icon");
					sl1.append("DataID");
					sl1.append("Text");

					sl2.append(item->GetChnName());
					sl2.append(typeName);
					//sl2.append(item->GetIconName());
					sl2.append(iconName);
					sl2.append(QString::number(_index));
					sl2.append(item->GetEngName());

					QMap<QString, QString> treeMap = insertQString(sl1, sl2);
					QDomElement child = IoXml::getInstance()->CreateElement(doc, "ChildNode", treeMap);
					QDomElement parent = IoXml::getInstance()->CreateElement(doc, "Parent", parentName);

					child.appendChild(parent);
					root.appendChild(child);
					_index++;

					wirteSimulationAndSolverGrandSon(doc, root, item);
				}

			}

			childList.clear();
		}

		QString disableNode = disableNodeList.join(";");
		QDomElement disable = IoXml::getInstance()->CreateElement(doc, "Disable", disableNode);
		root.appendChild(disable);

// 		for each (ModelBase* rootNode in rootNodeList)
// 		{
// 			if (nullptr == rootNode)
// 				continue;
// 
// 			QList<ModelBase*> childList = rootNode->GetChildList();
// 			
// 			for each (ModelBase* item in childList)
// 			{
// 				if (nullptr == item)
// 					continue;
// 				
// 				TreeItemType type = (TreeItemType)item->GetType();
// 
// 				if (typeNameDict.contains(type))
// 				{
// 					QString typeName = typeNameDict[type];
// 					QString parentName = parentNameDict[type];
// 					QStringList sl1, sl2;
// 
// 					sl1.append("Chinese");
// 					sl1.append("Type");
// 					sl1.append("Icon");
// 					sl1.append("DataID");
// 					sl1.append("Text");
// 
// 					sl2.append(item->GetChnName());
// 					sl2.append(typeName);
// 					sl2.append(item->GetIconName());
// 					sl2.append(QString::number(index));
// 					sl2.append(item->GetEngName());
// 					
// 					QMap<QString, QString> treeMap = insertQString(sl1, sl2);
// 					QDomElement child = IoXml::getInstance()->CreateElement(doc, "ChildNode", treeMap);
// 					QDomElement parent = IoXml::getInstance()->CreateElement(doc, "Parent", parentName);
// 
// 					child.appendChild(parent);
// 					root.appendChild(child);
// 					index++;
// 				}
// 
// 			}
// 
// 			childList.clear();
// 		}

		rootNodeList.clear();
		return true;
	}

	bool WriteTreeConfig::wirteSimulationAndSolverGrandSon(QDomDocument &doc, QDomElement &root, ModelBase* model)
	{
		QMap<TreeItemType, QString> typeNameDict;
		typeNameDict.insert(TreeItemType::ProjectSimulationSettingGrandSon, "SimulationSettingGrandSon");
		//typeNameDict.insert(TreeItemType::ProjectBoundaryCondationChild, "BoundaryCondationChild");
		typeNameDict.insert(TreeItemType::ProjectSolverGrandSon, "SolverSettingGrandSon");

		QList<ModelBase*> childList = model->GetChildList();

		for (int j = 0; j < childList.count(); j++)
		{
			ModelBase* item = childList.at(j);

			if (nullptr == item)
				continue;

			TreeItemType type = (TreeItemType)item->GetType();

			
			QString typeName = typeNameDict[type];
			QString parentName = model->GetEngName();
			QStringList sl1, sl2;
			QString iconName = SaveIconToSystem(item->GetIconName());

			sl1.append("Chinese");
			sl1.append("Type");
			sl1.append("Icon");
			sl1.append("DataID");
			sl1.append("Text");

			sl2.append(item->GetChnName());
			sl2.append(typeName);
			//sl2.append(item->GetIconName());
			sl2.append(iconName);
			sl2.append(QString::number(_index));
			sl2.append(item->GetEngName());

			QMap<QString, QString> treeMap = insertQString(sl1, sl2);
			QDomElement child = IoXml::getInstance()->CreateElement(doc, "ChildNode", treeMap);
			QDomElement parent = IoXml::getInstance()->CreateElement(doc, "Parent", parentName);

			child.appendChild(parent);
			root.appendChild(child);
			_index++;
			

		}

		return true;
	}

	QMap<QString, QString> WriteTreeConfig::insertQString(QStringList var, QStringList val)
	{
		QMap<QString, QString> map;
		
		for (int i = 0; i < var.count(); i++)
		{
			map.insert(var.at(i), val.at(i));
		}
		return map;
	}

}