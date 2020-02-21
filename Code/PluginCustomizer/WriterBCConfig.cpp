#include "WriterBCConfig.h"
#include "Common.h"
#include "ModelBase.h"
#include "BoundaryModel.h"
#include "IOXml.h"
#include "DataProperty/ParameterBase.h"
#include "DataProperty/DataBase.h"

#pragma execution_character_set("utf-8")

namespace FastCAEDesigner
{
	WriteBCConfig::WriteBCConfig()
	{
		_fileName = _CONFIG_BC_;
	}


	WriteBCConfig::~WriteBCConfig()
	{
	}

	//bcModelLlist:边界参数列表
	bool WriteBCConfig::Write(QList<ModelBase*> bcModelList)
	{
		QDomDocument doc;
		QDomProcessingInstruction instruction;
		instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
		doc.appendChild(instruction);
		QDomElement root = doc.createElement("BCConfig");
		WriteTreePara(doc, root, bcModelList);
		doc.appendChild(root);
		SaveFile(doc);
		/**/
		return true;
	}
	
	bool WriteBCConfig::WriteTreePara(QDomDocument &doc, QDomElement &root, QList<ModelBase*> bcModelList)
	{
		int index = 1;
		
		for (int i = 0; i < bcModelList.count(); i++)
		{
			ModelBase* model = bcModelList.at(i);

			if (nullptr == model)
				continue;

			BoundaryModel* bcModel = (BoundaryModel*)model;
			QString nodeName = QString("Type%1").arg(index);
			QDomElement bcListNode = IoXml::getInstance()->CreateElementAttribute(doc, "BCList", "Type", nodeName);
			QString condition = bcModel->GetCommanCondition();
			QDomElement bcType = IoXml::getInstance()->CreateElement(doc, "EnableDefaultType", condition);
			bcListNode.appendChild(bcType);
			QList<ModelBase*> childModelList = model->GetChildList();
			
			for (int j = 0; j < childModelList.count(); j++)
			{
				ModelBase* childModel = childModelList.at(j);
				
				if (nullptr == childModel)
					continue;

				WriteTreeChildPara(doc, bcListNode, childModel);
			}
			
			root.appendChild(bcListNode);
			index++;
		}

		return true;
	}
	
	bool WriteBCConfig::WriteTreeChildPara(QDomDocument &doc, QDomElement &root, ModelBase* model)
	{
		QString nameEng = model->GetEngName();
		QString nameChn = model->GetChnName();
		//QString iconName = model->GetIconName();
		QString iconName = SaveIconToSystem(model->GetIconName());
		//QDomElement bcDataRoot = IoXml::getInstance()->CreateElementAttribute(doc, QString("BC"), QString("Name"), nameChn, QString("Icon"), iconName);
		
		QStringList sl1, sl2;
		
		sl1.append("Name");
		sl1.append("Chinese");
		sl1.append("Icon");
		
		sl2.append(nameEng);
		sl2.append(nameChn);
		sl2.append(iconName);
		
		QMap<QString, QString> treeMap = insertQString(sl1, sl2);
		QDomElement bcDataRoot = IoXml::getInstance()->CreateElement(doc, "BC", treeMap);

		
		DataProperty::DataBase *dataBase = model->GetDataBase();
		
		if (nullptr != dataBase)
			dataBase->writeParameters(&doc, &bcDataRoot);
		
		root.appendChild(bcDataRoot);
		return true;
	}

	QMap<QString, QString> WriteBCConfig::insertQString(QStringList var, QStringList val)
	{
		QMap<QString, QString> map;

		for (int i = 0; i < var.count(); i++)
		{
			map.insert(var.at(i), val.at(i));
		}
		return map;
	}

}