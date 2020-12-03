#ifndef _PHENGLEIPLUGIN_H__
#define _PHENGLEIPLUGIN_H__

#include "PHEngLEIPluginAPI.h"
#include "PluginManager/pluginBase.h"
#include "DataProperty/modelTreeItemType.h"

#include "meshData/meshSingleton.h"
class QTextStream;
namespace GUI
{
	class MainWindow;
}
namespace MainWidget
{
	class PreWindow;
}

namespace ModelData
{
	class ModelDataBase;
}
namespace ProjectTree
{
	class ProjectTreeBase;
}

namespace Material
{
	class Material;
}

namespace BCBase
{
	class BCBase;
}

namespace Plugins
{
	class PHENGLEIPLUGINAPI PHengLEIPlugin : public PluginBase
	{
		Q_OBJECT
	public:
		PHengLEIPlugin(GUI::MainWindow* m);
		~PHengLEIPlugin();

		//加载插件
		bool install() override;
		//卸载插件
		bool uninstall() override;
		void setData(ModelData::ModelDataBase* data);

		void setIsStruct(bool str);
		bool getIsStruct();

		void setGridName(QString gname);
		QString getGridName();

		void setPhengLeiCase(QString casename);
		QString getPhengLeiCase();

		static GUI::MainWindow* getMainWindow();
		static Plugins::PHengLEIPlugin*getSelf(){ return _self; };


	public slots:
	void processFinished(int pid);

	private:
		static GUI::MainWindow* _mainwindow;
		static Plugins::PHengLEIPlugin* _self;
		ModelData::ModelDataBase* _data;
		bool _isStruct{ false };
		QString _gname{};
		QString _PhengLeiCase{ "Subsonic" };
	};
}

extern "C"
{
	void PHENGLEIPLUGINAPI Register(GUI::MainWindow* m, QList<Plugins::PluginBase*>* plugs);
	/*网格转换*/
	void PHENGLEIPLUGINAPI MeshTransfer(ModelData::ModelDataBase*, ProjectTree::ProjectTreeBase*);
	/*输出结果*/
	bool PHENGLEIPLUGINAPI WriteOut(QString path, ModelData::ModelDataBase* d);
	/*网格分区*/
	void PHENGLEIPLUGINAPI MeshPartition(ModelData::ModelDataBase*, ProjectTree::ProjectTreeBase*);
	/*修改边界条件*/
	void GetBoundaryCondition(ModelData::ModelDataBase*);
	/*参数设置并求解*/
	void  ParameterSetting(ModelData::ModelDataBase* model);
	/*并行分区*/
	void ParallelPartition(ModelData::ModelDataBase* model);

	void WriteBCParameter(BCBase::BCBase* bc, QTextStream& io, QString& strType);

	void WriteMachAttackdSideslipd(QTextStream& io, QStringList& list, QStringList& ParaValues);

	void WritePressureExportBoundary(QTextStream& io, QStringList& ParaValues);

	QStringList StrTypeConverToNumberType(QString&);

	bool CopyFileToProjectPath(ModelData::ModelDataBase* data);
}


#endif