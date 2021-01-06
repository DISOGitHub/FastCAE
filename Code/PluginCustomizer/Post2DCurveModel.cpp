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

	//获取曲线列表
// 	QList<SPost2DCurve> Post2DCurveModel::GetCurveList()
// 	{
// 		return _curveList;
// 	}
	//获取曲线列表
	QList<ConfigOption::PostCurve*> Post2DCurveModel::GetCurveList()
	{
		return _curveList;
	}
	//界面入口
	int Post2DCurveModel::ShowEditor(QTreeWidgetItem* treeItem, QWidget* parent)
	{
		EditorCurveModel dlg(treeItem,this);
		//dlg.setCurveInfo(_curveList); 
		
		return dlg.exec();
	}

	//设置曲线列表
	void Post2DCurveModel::setCurveList(QList<ConfigOption::PostCurve*> list)
	{
		_curveList = list;
	}
	//获取文件名
	QString Post2DCurveModel::getFileName()
	{
		return _fileName;
	}
	//设置文件名
	void Post2DCurveModel::setFileName(QString name)
	{
		_fileName = name;
	}
}