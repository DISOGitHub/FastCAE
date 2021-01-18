#ifndef _MODELSATABASEEXTEND_H_
#define _MODELSATABASEEXTEND_H_

#include "modelDataAPI.h"
#include "modelDataBase.h"
#include <QStringList>
#include <QList>
#include <QHash>

class QDomDocument;
class QDomElement;

namespace DataProperty
{
	class DataBase;
	class ParameterBase;
	class ParameterGroup;
}

namespace ConfigOption
{
	class PostCurve;
	class ParameterObserver;
}

namespace Post
{
	class Post2DWindowInterface;
	class Post3DWindowInterface;
}

namespace ModelData
{
	class MODELDATAAPI ModelDataBaseExtend : public ModelDataBase
	{
	public:
		ModelDataBaseExtend(ProjectTreeType type);
		~ModelDataBaseExtend();

		virtual void copyFormConfig() override;
		virtual void generateParaInfo() override;
		//添加报告
		void appendReport(QString report);
		//获取报告数量
		int getReportCount();
		//获取第index个报告
		QString getReportAt(int index);
		//移除第index个报告
		void removeReportAt(int index);
		//设置材料，网格组件ID与材料ID
		void setMaterial(int setID, int materialID);
		//获取组件的材料ID
		int getMaterialID(int setid);
		//组件是否被设置材料
		bool isMaterialSetted(int setid);
		//解除材料与组件的绑定
		void removeMaterial(int setid);
		
		virtual void setComponentIDList(QList<int> ids);

		virtual void removeComponentAt(int index) override;

		virtual QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* ele) override;
		virtual void writeToProjectFile1(QDomDocument* doc, QDomElement* ele) override;
		virtual void readDataFromProjectFile(QDomElement* e) override;
		virtual void writeToSolverXML(QDomDocument* doc, QDomElement* e) override;

		virtual void dataToStream(QDataStream* datas) override;

		void appendConfigData(int dataID, DataProperty::DataBase* d);
		DataProperty::DataBase* getConfigData(int dataid);
		//获取节点数量
		int getConfigDataCount();
		//获取监控文件的名称
		QStringList getMonitorFile();
		//获取监控文件的绝对路径
		QStringList getAbsoluteMonitorFile();
		//根据文件获取监控的变量列表
		QStringList getMonitorVariables(QString file);
		//获取监控曲线
		QList<ConfigOption::PostCurve*> getMonitorCurves();
		//获取第index的监控曲线
		ConfigOption::PostCurve* getMonitorCurveAt(const int index);
		//获取后处理二维曲线文件
		QStringList getPost2DFiles();
		//获取后处理二维曲线文件的绝对路径
		QStringList getAbsolutePost2DFiles();
		//根据文件获取二维曲线文件中的变量
		QStringList getPost2DVariables(QString f);
		//获取三维后处理文件
		QString getPost3DFile();
		//获取三维后处理标量
		void get3DScalars(QStringList &node, QStringList &ele);
		//获取三维后处理向量
		void get3DVector(QStringList &node, QStringList &ele);
		//添加二维后处理曲线
		void apppendPlotCurve(ConfigOption::PostCurve* c);
		//获取已绘制曲线
		QList<ConfigOption::PostCurve*> getPlotCurves();
		//移除二维曲线
		void removePlotCurve(int index);
		//清除全部二维曲线
		void clearPlotCurve();
		//后处理曲线是否存在
		bool isPostCurveExist(QString name);
		//添加标量变量名称
		void appendScalarVariable(QString v);
		//获取标量变量名称
		QStringList getScalarVariable();
		//移除第index个标量
		void removeScalarVariable(int index);
		//添加向量变量名称
		void appendVectorVariable(QString v);
		//获取向量变量名称
		QStringList getVectorVariable();
		//移除第index个向量名称
		void removeVectorVariable(int index);
		//情况三维变量
		void clear3DVariable();
		DataProperty::ParameterBase* getParameterByName(QString name) override;
		DataProperty::ParameterGroup* getParameterGroupByName(QString name) override;
		virtual void removeParameter(DataProperty::ParameterBase* p) override;
		virtual void removeParameterGroup(DataProperty::ParameterGroup* g) override;
		//设置二维绘图窗口
		void setPost2DWindow(Post::Post2DWindowInterface* p2d);
		//获取二维窗口
		Post::Post2DWindowInterface* getPost2DWindow();
		//设置三维渲染窗口
		void setPost3DWindow(Post::Post3DWindowInterface* p3d);
		//获取三维渲染窗口
		Post::Post3DWindowInterface* getPost3DWindow();

		void bindInpMaterialIds(const QList<int>&);
		const QList<int>& getInpMaterialIds();

	protected:
		void registerObserver();
		
	protected:
		QList<int> _inpMaterIds;
		QHash<int, int> _setMaterial{};
		QStringList _reportList{};
		QHash<int, DataProperty::DataBase*> _configData{};
		QStringList _monitorFiles{};
		QList<ConfigOption::PostCurve*> _monitorCurves{};
		QList<ConfigOption::PostCurve*> _postCurves{};
//		QStringList _plot2DVariable{};
		QStringList _scalarVariable{};
		QStringList _vectorVariable{};
		QList<ConfigOption::ParameterObserver*> _observerList{};

		Post::Post2DWindowInterface* _post2DWindow{};
		Post::Post3DWindowInterface* _post3DWindow{};
	};


}

#endif
