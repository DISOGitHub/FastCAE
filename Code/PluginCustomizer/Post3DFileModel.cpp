#include "Post3DFileModel.h"
#include "EditorCurveModel.h"

#pragma execution_character_set("utf-8")


namespace FastCAEDesigner
{
	Post3DFileModel::Post3DFileModel(QString nameEng, QString nameChn, QString iconName, int type, QObject *parent)
		:ModelBase(nameEng, nameChn, iconName, type, parent)
	{
	}

	Post3DFileModel::Post3DFileModel(QString fileName, int type, QObject *parent)
		: ModelBase(fileName, type, parent)
	{
		_fileName = fileName;
	}

	Post3DFileModel::~Post3DFileModel()
	{
		_nodeScalarList.clear();
		_cellScalarList.clear();
		_nodeVectorList.clear();
		_cellVectorList.clear();
	}


	//显示vector相关信息
	int Post3DFileModel::ShowEditor(QTreeWidgetItem* treeItem, QWidget* parent)
	{
		EditorCurveModel info(treeItem,this);

		return info.exec();
	}
	//获取四种不同类型的列表
	QStringList Post3DFileModel::getNodeScalarList()
	{
		return _nodeScalarList;
	}
	QStringList Post3DFileModel::getNodeVectorList()
	{
		return _nodeVectorList;
	}
	QStringList Post3DFileModel::getCellScalarList()
	{
		return _cellScalarList;
	}
	QStringList Post3DFileModel::getCellVectorList()
	{
		return _cellVectorList;
	}

	//设置4种不同类型的列表信息
	void Post3DFileModel::setDataList(QStringList ns, QStringList nv, QStringList cs, QStringList cv)
	{
		_nodeScalarList = ns;
		_nodeVectorList = nv;
		_cellScalarList = cs;
		_cellVectorList = cv;
	}
	//设置文件名
	void Post3DFileModel::setFileName(QString name)
	{
		_fileName = name;
	}
	//获取文件名
	QString Post3DFileModel::getFileName()
	{
		return _fileName;
	}
}