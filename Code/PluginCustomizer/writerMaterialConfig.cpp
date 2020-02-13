#include "writerMaterialConfig.h"
#include "Common.h"
#include "ModelBase.h"
#include "BoundaryModel.h"
#include "IOXml.h"
#include "DataProperty/ParameterBase.h"
#include "DataProperty/DataBase.h"

#pragma execution_character_set("utf-8")

namespace FastCAEDesigner
{
	WriterMaterialConfig::WriterMaterialConfig()
	{
		_fileName = _CONFIG_MATERIAL_;
	}

	WriterMaterialConfig::~WriterMaterialConfig()
	{
	}

	//bcModelLlist:边界参数列表
	bool WriterMaterialConfig::Write(QList<ModelBase*> bcModelList)
	{
		QDomDocument doc;
		QDomProcessingInstruction instruction;
		instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
		doc.appendChild(instruction);
		QDomElement root = doc.createElement("MaterialConfig");
		WriteTreePara(doc, root, bcModelList);
		doc.appendChild(root);
		SaveFile(doc);
		return true;
	}

	bool WriterMaterialConfig::WriteTreePara(QDomDocument &doc, QDomElement &root, QList<ModelBase*> bcModelList)
	{
		for (int i = 0; i < bcModelList.count(); i++)
		{
			ModelBase* model = bcModelList.at(i);

			if (nullptr == model)
				continue;

			WriteTreeChildPara(doc, root, model);
		}

		return true;
	}

	bool WriterMaterialConfig::WriteTreeChildPara(QDomDocument &doc, QDomElement &root, ModelBase* model)
	{
		QString name = model->GetChnName();
		QString iconName = model->GetIconName();
		QDomElement materialRoot = IoXml::getInstance()->CreateElementAttribute(doc, QString("Material"), QString("Type"), name);
		DataProperty::DataBase *dataBase = model->GetDataBase();

		if (nullptr != dataBase)
			dataBase->writeParameters(&doc, &materialRoot);

		root.appendChild(materialRoot);
		return true;
	}

	QMap<QString, QString> WriterMaterialConfig::insertQString(QStringList var, QStringList val)
	{
		QMap<QString, QString> map;

		for (int i = 0; i < var.count(); i++)
		{
			map.insert(var.at(i), val.at(i));
		}
		return map;
	}

}