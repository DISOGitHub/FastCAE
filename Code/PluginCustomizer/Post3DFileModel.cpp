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


	//��ʾvector�����Ϣ
	int Post3DFileModel::ShowEditor(QTreeWidgetItem* treeItem, QWidget* parent)
	{
		EditorCurveModel info(treeItem,this);

		return info.exec();
	}
	//��ȡ���ֲ�ͬ���͵��б�
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

	//����4�ֲ�ͬ���͵��б���Ϣ
	void Post3DFileModel::setDataList(QStringList ns, QStringList nv, QStringList cs, QStringList cv)
	{
		_nodeScalarList = ns;
		_nodeVectorList = nv;
		_cellScalarList = cs;
		_cellVectorList = cv;
	}
	//�����ļ���
	void Post3DFileModel::setFileName(QString name)
	{
		_fileName = name;
	}
	//��ȡ�ļ���
	QString Post3DFileModel::getFileName()
	{
		return _fileName;
	}
}