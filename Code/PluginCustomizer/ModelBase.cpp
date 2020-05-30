#include "ModelBase.h"
#include <QDebug>
#include "DataProperty/DataBase.h"
#include "DataManager.h"
#include "GetModelItemIconFactory.h"
#include "CreateChildModelFactory.h"
#include "TreeInformation.h"
#include "Common.h"


namespace FastCAEDesigner
{
	ModelBase::ModelBase(QObject *parent) : QObject(parent)
	{
	}

	ModelBase::ModelBase(QString name, QObject *parent) :
		QObject(parent),
		_nameEng(name)
	{

	}

	ModelBase::ModelBase(QString name, QString iconName, QObject *parent) :
		QObject(parent),
		_nameEng(name),
		_iconName(iconName)
	{

	}

	ModelBase::ModelBase(QString nameEng, QString nameChn, QString iconName, QObject *parent) :
		QObject(parent),
		_nameEng(nameEng),
		_nameChn(nameChn),
		_iconName(iconName)
	{

	}
	
	ModelBase::ModelBase(QString nameEng, QString nameChn, QString iconName,int type, QObject *parent) :
		QObject(parent),
		_nameEng(nameEng),
		_nameChn(nameChn),
		_iconName(iconName),
		_type(type)
	{

	}

	ModelBase::ModelBase(QString nameEng, QString nameChn, int type, QObject *parent) :
		QObject(parent),
		_nameEng(nameEng),
		_nameChn(nameChn),
		_type(type)
	{

	}

	ModelBase::ModelBase(QString fileName, int type, QObject *parent) :
		QObject(parent),
		_type(type),
		_fileName(fileName)
	{

	}


	ModelBase::~ModelBase()
	{
		if (_dataBase != nullptr)
		{
			delete _dataBase;
			_dataBase = nullptr;
		}

		int num = this->_childNodes.count();

		for (int i = 0; i<num; i++)
		{
			ModelBase *item = _childNodes[i];
			if (nullptr == item)
				continue;
			delete item;
			item = nullptr;
		}
	}

	
	void ModelBase::Clear()
	{
		/*
		if (_dataBase != nullptr)
		{
			delete _dataBase;
			_dataBase = nullptr;
		}

		int num = this->_childNodes.count();

		for (int i = 0; i < num; i++)
		{
			ModelBase *item = _childNodes[i];
			if (nullptr == item)
				continue;
			delete item;
			item = nullptr;
		}
		*/
	}

	ModelBase* ModelBase::CreateChildModel(QString nameEng, QString nameChn, QString icon, QObject* parent)
	{
		return CreateChildModelFactory::Create(_type, nameEng, nameChn, icon, parent);
	}

// 	ModelBase* ModelBase::CreateChildModel(QString fileName, QObject* parent)
// 	{
// 		return CreateChildModelFactory::Create(_type, fileName, parent);
// 	}

	ModelBase* ModelBase::CreateChildModel(QTreeWidgetItem* parentTreeItem)
	{
		QList<QString> usedChnNameList = GetUsedNameList(TChinese);
		QList<QString> usedEngNameList = GetUsedNameList(TEnglish);
		TreeInformation dlg;
		dlg.SetUsedChnNameList(usedChnNameList);
		dlg.SetUsedEngNameList(usedEngNameList);
		dlg.SetWorkModel(TreeInformation::CreateChilldNodeMode);
		int r = dlg.exec();

		if (r == QDialog::Rejected)
			return nullptr;

		ModelBase* childModel = CreateChildModel(dlg.GetEnglishName(), dlg.GetChineseName(), dlg.GetIconName(), this);

		if (nullptr == childModel)
			return  nullptr;

		childModel->SetIsEdit(true);
		AddNode(childModel);
		QTreeWidgetItem *treeItem = childModel->CreateTreeItem(parentTreeItem);
		return childModel;
	}

	QWidget* ModelBase::ShowEditor(QWidget *widget)
	{
		return nullptr;
	}

	void ModelBase::ShowEditor()
	{
		return;
	}

	int ModelBase::ShowEditor(QTreeWidgetItem* treeItem, QWidget* parent)
	{
		return 0;
		FastCAEDesigner::TreeInformation dlg(treeItem, this, parent);
		int r = dlg.exec();
		return r;
	}

	void ModelBase::AddNode(ModelBase *model)
	{
		if (!_childNodes.contains(model))
		{
			this->_childNodes.append(model);
		}
	}
// 	void ModelBase::AddNode(QTreeWidgetItem* parentItem,ModelBase* model)
// 	{
// 		if (!_childNodes.contains(model))
// 		{
// 			this->_childNodes.append(model);
// 			//parentItem->addChild
// 		}
// 	}
	void ModelBase::DelNode(ModelBase *model)
	{
		if (_childNodes.contains(model) && (model != nullptr))
		{
			_childNodes.removeOne(model);
		}	
	}
		

	QTreeWidgetItem* ModelBase::CreateTreeItem(QTreeWidgetItem *root)
	{
		QTreeWidgetItem* nodeItem = new QTreeWidgetItem;
		if (this->GetChnName() != "")
			nodeItem = CreateTreeWidgetItem(root, _nameChn, _iconName);
		else
			nodeItem = CreateTreeWidgetItem(root, _fileName, _iconName);
		
		for (int i = 0; i < _childNodes.count(); i++)
		{
			ModelBase *model = _childNodes.at(i);
			if (nullptr == model)
				continue;
			QTreeWidgetItem* modelItem = model->CreateTreeItem(nodeItem);
			if (model->GetVisible() == false)
			{
				modelItem->setHidden(true);
			}
			
		}

		//nodeItem->setExpanded(true);
		return nodeItem;
	}
	

	QTreeWidgetItem* ModelBase::CreateTreeWidgetItem(QTreeWidgetItem *root, QString text, QString iconName)
	{
		QTreeWidgetItem *item = new QTreeWidgetItem(root, QStringList(text));
		QIcon icon = QIcon(iconName);
		//QIcon icon = GetModelItemIconFactory::Get(_type);
		item->setIcon(0, icon);
		//DataManager::getInstance()->DictTreeItemToModel.insert(item, this);
		DataManager::getInstance()->InsertModelToDict(item, this);
		return item;
	}

	//属性
	QString ModelBase::GetEngName()
	{
		return _nameEng;
	}

	void ModelBase::SetEngName(QString name)
	{
		_nameEng = name;
	}

	QString ModelBase::GetChnName()
	{
		return _nameChn;
	}

	void ModelBase::SetChnName(QString chnName)
	{
		_nameChn = chnName;
	}

	int ModelBase::GetType()
	{
		return _type;
	}

	void ModelBase::SetType(int type)
	{
		_type = type;
	}

	QString ModelBase::GetIconName()
	{
		return _iconName;
	}

	void ModelBase::SetIconName(QString iconName)
	{
		_iconName = iconName;
	}

	void ModelBase::SetIsHaveAddButton(bool b)
	{
		_isHaveAddButton = b;
	}

	void ModelBase::SetIsHaveDelButton(bool b)
	{
		_isHaveDelButton = b;
	}

	void ModelBase::SetIsHaveHideButton(bool b)
	{
		_isHaveHideButton = b;
	}

	bool ModelBase::GetIsHaveAddButton()
	{
		return _isHaveAddButton;
	}

	bool ModelBase::GetIsHaveDelButton()
	{
		return _isHaveDelButton;
	}

	bool ModelBase::GetIsHaveHideButton()
	{
		return _isHaveHideButton;
	}

	void ModelBase::SetVisible(bool b)
	{
		_visible = b;
	}

	bool ModelBase::GetVisible()
	{
		return _visible;
	}

	void ModelBase::SetIsEdit(bool b)
	{
		_isEdit = b;
	}

	bool ModelBase::GetIsEdit()
	{
		return _isEdit;
	}

	DataProperty::DataBase* ModelBase::GetDataBase()
	{
		if (nullptr == _dataBase)
			_dataBase = new DataProperty::DataBase();

		return _dataBase;
	}

	void ModelBase::SetDataBase(DataProperty::DataBase* dataBase)
	{
		this->_dataBase = dataBase;
	}

	//属性
	//获取给定数据列表中，已经使用的中文或英文名字列表.参数：0:中文、1：英文
	QList<QString> ModelBase::GetUsedNameList(int nameType)
	{
		QList<QString> nameList;
		int count = _childNodes.count();

		if (0 == count)
			return nameList;

		for (int i = 0; i < count; i++)
		{
			ModelBase* model = _childNodes.at(i);

			if (nullptr == model)
				continue;;

			if (TChinese == nameType)
				nameList.append(model->GetChnName());
			else
				nameList.append(model->GetEngName());
		}

		return nameList;
	}

	void ModelBase::SetParentModelBase(ModelBase* parentModelBase)
	{
		_parentModelBase = parentModelBase;
	}

	ModelBase* ModelBase::GetParentModelBase()
	{
		return _parentModelBase;
	}

	QList<ModelBase*> ModelBase::GetChildList()
	{
		return _childNodes;
	}

	QString ModelBase::getFileName()
	{
		return _fileName;
	}

	void ModelBase::setFileName(QString name)
	{
		_fileName = name;
	}

	int ModelBase::GetId()
	{
		return _id;
	}

	void ModelBase::SetId(int id)
	{
		_id = id;
	}

	void ModelBase::Copy(ModelBase* data)
	{
		_nameEng = data->GetEngName();
		_nameChn = data->GetChnName();
		_iconName = data->GetIconName();
		_id = data->GetId();
		_type = data->GetType();

		if (nullptr != _dataBase)
		{
			delete _dataBase;
			_dataBase = nullptr;
		}

		_dataBase = new DataProperty::DataBase();
		_dataBase->copy(data->GetDataBase());
		_parentModelBase = data->GetParentModelBase();
		_fileName = data->getFileName();
		_visible = data->GetVisible();
		_isEdit = data->GetIsEdit();
		//_isHaveAddButton;
		//_isHaveDelButton;
		//_isHaveHideButton;
		
		_childNodes.clear();
		QList<ModelBase*> list = data->GetChildList();
		int count = list.count();

		for (int i = 0; i < count; i++)
		{
			ModelBase* item = list.at(i);
			ModelBase* model = new ModelBase();
			model->Copy(item);
			_childNodes.append(model);
		}
	}

	int ModelBase::getTreeType()
	{
		return _treeType;
	}

	void ModelBase::setTreeType(int type)
	{
		_treeType = type;
	}

}