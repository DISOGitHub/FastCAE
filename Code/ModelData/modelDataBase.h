#ifndef MODELDATABASE_H
#define MODELDATABASE_H

#include "modelDataAPI.h"
#include "DataProperty/modelTreeItemType.h"
#include <QString>
#include "DataProperty/DataBase.h"
#include <QList>
#include "moduleBase/messageWindowBase.h"
#include "BCBase/BCType.h"

class QDataStream;
class QTextStream;
class QDomDocument;

namespace BCBase
{
	class BCBase;
}

namespace ModelData
{
	class SimlutationSettingBase;
	class SolverSettingBase;

	class MODELDATAAPI ModelDataBase : public DataProperty::DataBase
	{
	public:
		//构造函数
		ModelDataBase(ProjectTreeType treeType);
		virtual ~ModelDataBase();
		//从配置文件中拷贝
		virtual void copyFormConfig();
		//将ID和类型信息传递给基类的各个参数
		virtual void generateParaInfo() override;
		//获取最大的ID
		static int getMaxID();
		void setID(int id) override;
		//获取算例路径
		QString getPath();
		static void resetMaxID();
		//设置类型
		void setTreeType(ProjectTreeType type);
		//获取类型
		ProjectTreeType getTreeType();
		//求解完成时间
		void setSolveTime(double t);
		//获取求解完成时间
		double getSolveTime();
		//设置写出给求解器的文件名称
		void setOutputFileName(QString name);
		QString& getOutputFileName();
		/*获取MD5的stream*/
		virtual void dataToStream(QDataStream* datas) override;
		//数据写出到工程文件 子类必须重写该函数
		virtual QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* ele) override;
		//数据写出到工程文件 子类必须重写该函数
		virtual void writeToProjectFile1(QDomDocument* doc, QDomElement* ele);
		///从工程文件块读入数据，子类必须重写此函数
		virtual void readDataFromProjectFile(QDomElement* e) override;
		///将数据写出文本给求解器
		virtual void writeToSolverText(QTextStream* stream);
		///将数据写出XML给求解器
		virtual void writeToSolverXML(QDomDocument* doc, QDomElement* e);
		//检查数据是否可以求解 返回True-可以求解；False-不能求解
		virtual bool checkSolveableStatus(QVector<ModuleBase::Message> & messages);
		
		//获取BC数量
		int getBCCount();
		//添加BC
		void appeendBC(BCBase::BCBase* bc); 
		//根据类型获取BC
		QList<BCBase::BCBase*> getBCByType(BCBase::BCType type);

		//获取第index个BC
		BCBase::BCBase* getBCAt(const int index);
		//移除边界条件
		void removeBCAt(const int index);
		//通过组件ID删除BC，每个BC都会绑定一个组件
		void removeBCByComponentID(int);
		//设置需要关联的组件ID(包括网格组件和几何组件)
		void setComponentIDList(const QList<int>& ids);
		//添加一个组件ID
		void addComponentID(int cpId);
		//获取关联的组件ID(包括网格组件和几何组件)
		const QList<int>& getComponentIDList();
		//通过ID删除组件
		bool removeComponentByID(int);

		//获取关联的网格组件ID
		QList<int> getMeshSetList();
		//获取关联的几何组件ID
		QList<int> getGeoComponentIDList();

		//设置需要关联的网格组件ID
		//virtual void setMeshSetList(QList<int> ids);
		//设置需要关联的几何组件ID
		//void setGeoComponentIDList(QList<int> ids);
		
		//移除第index个组件
		virtual void removeComponentAt(int index);
		//获取关联的Kernal ID
		QList<int>& getMeshKernalList();
		//设置关联的Kernal ID
		void setMeshKernelList(const QList<int>& kids);
		//添加关联的Kernal ID
		void addMeshKernalId(const int);
		//获取关联的几何形状
		QList<int>& getGeometryList();
		//设置关联的几何形状
		void setGeometryList(QList<int> geo);
		//获取仿真参数
		SimlutationSettingBase* getSimlutationSetting();
		//获取求解器设置
		SolverSettingBase* getSolverSetting();
		//组件是否使用
		bool isComponentUsed(int index);

		virtual DataProperty::ParameterBase* getParameterByName(QString name) override;
		virtual void removeParameter(DataProperty::ParameterBase* p) override;
		virtual DataProperty::ParameterGroup* getParameterGroupByName(QString name) override;
		virtual void removeParameterGroup(DataProperty::ParameterGroup* g) override;


	private:
		ProjectTreeType getTreeTypeByString(const QString& type);
		QString getTreeTypeToSring(ProjectTreeType type);

	protected:
		ProjectTreeType _treeType{ UnDefined };
		static int maxID;
		double _solvetime{ -1.0 };
		QString _outputFileName{};

		QList<BCBase::BCBase*> _bcList{};
//		QMultiHash<int, QString> _ComponentIDType{};
		QList<int> _ComponentIDList{};
		QList<int> _meshKernalIDList{};
		QList<int> _geometryList{};

		SolverSettingBase* _solverSetting{};
		SimlutationSettingBase* _simlutationSetting{};
	};

}


#endif
