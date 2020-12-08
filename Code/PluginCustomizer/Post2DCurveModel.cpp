#include "Post2DCurveModel.h"
#include "EditorCurveModel.h"
#include "ConfigOptions/PostConfigInfo.h"


#pragma execution_character_set("utf-8")


namespace FastCAEDesigner
{
	Post2DCurveModel::Post2DCurveModel(QString nameEng, QString nameChn, QString iconName, int type, QObject *parent)
		:ModelBase(nameEng, nameChn, iconName, type, parent)
	{
	}

	Post2DCurveModel::Post2DCurveModel(QString fileName, int type, QObject *parent)
		: ModelBase(fileName, type, parent)
	{
		_fileName = fileName;
	}

	Post2DCurveModel::~Post2DCurveModel()
	{
	}

	//��ȡ�����б�
// 	QList<SPost2DCurve> Post2DCurveModel::GetCurveList()
// 	{
// 		return _curveList;
// 	}
	//��ȡ�����б�
	QList<ConfigOption::PostCurve*> Post2DCurveModel::GetCurveList()
	{
		return _curveList;
	}
	//�������
	int Post2DCurveModel::ShowEditor(QTreeWidgetItem* treeItem, QWidget* parent)
	{
		EditorCurveModel dlg(treeItem,this);
		//dlg.setCurveInfo(_curveList); 
		
		return dlg.exec();
	}

	//���������б�
	void Post2DCurveModel::setCurveList(QList<ConfigOption::PostCurve*> list)
	{
		_curveList = list;
	}
	//��ȡ�ļ���
	QString Post2DCurveModel::getFileName()
	{
		return _fileName;
	}
	//�����ļ���
	void Post2DCurveModel::setFileName(QString name)
	{
		_fileName = name;
	}
}