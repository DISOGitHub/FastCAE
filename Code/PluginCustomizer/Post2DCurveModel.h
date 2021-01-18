#ifndef POST2DCURVEMODEL_H
#define POST2DCURVEMODEL_H

#include "ModelBase.h"
#include "Common.h"
//#include "ConfigOptions/PostCurve.h"

namespace ConfigOption
{
	class PostCurve;
	class PostConfigInfo;
}

namespace FastCAEDesigner
{

	class Post2DCurveModel : public ModelBase
	{
		Q_OBJECT
	public:
		Post2DCurveModel(QString nameEng, QString nameChn, QString iconName, int type, QObject *parent);
		Post2DCurveModel(QString fileName, int type, QObject *parent);
		~Post2DCurveModel();

		//QList<SPost2DCurve> GetCurveList();//获取曲线列
		QList<ConfigOption::PostCurve*> GetCurveList();
		void setCurveList(QList<ConfigOption::PostCurve*> list);
		virtual	int ShowEditor(QTreeWidgetItem* treeItem, QWidget* parent);
		void init();
		QString getFileName();
		void setFileName(QString name);
// 		void GetCurveName();
// 		void GetCurveXVariable();
// 		void GetCurveYVariable();
// 		void SetCurveName(QString);
// 		void SetCurveXVariable(QString);
// 		void SetCurveYVariable(QString);

	private:
		//QList<SPost2DCurve> _curveList;//曲线列表
		QList<ConfigOption::PostCurve*> _curveList;
		QString _fileName;
	};
}

#endif