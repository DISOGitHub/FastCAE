#ifndef _GMSHTHREAD_H_
#define _GMSHTHREAD_H_

#include <QThread>
#include <QList>
#include <QMultiHash>
#include <QString>
#include <QProcess>
#include "DataProperty/DataBase.h"
#include <QTextStream>
#include <QTime>
#include <vtkCellType.h>
#include "GmshModuleAPI.h"

class TopoDS_Compound;
class TopoDS_Shape;
class gp_Pnt;

class vtkDataSet;

namespace GUI
{
	class MainWindow;
}

namespace MainWidget
{
	class PreWindow;
}

namespace ModuleBase
{
	class ProcessBar;
}

namespace Py
{
	class PythonAagent;
}

namespace MeshData{
	class MeshKernal;
}

namespace Gmsh
{
	typedef struct
	{
		int geoSetID;
		int itemIndex;
		QList<int> cellIndexs;
	}itemInfo;

	class GmshModule;
	class FluidMeshPreProcess;
	class GmshScriptWriter;
	//class GmshSettingData;
	class GMshPara
	{
	public:
		int _dim{ -1 };
		QMultiHash<int, int> _solidHash{};
		QMultiHash<int, int> _surfaceHash{};
		QString _elementType{};
		int _elementOrder{ -1 };
		int _method{ -1 };
		double _sizeFactor{ 0.0 };
		double _minSize{ 0.0 };
		double _maxSize{ 0.0 };
		bool _geoclean{ false };
		int _smoothIteration{ 0 };
		bool _isGridCoplanar{ false };
		QString _sizeAtPoints{};
		QString _sizeFields{};
	//	QString _physicals{};
		bool _selectall{ false };
		bool _selectvisible{ false };
		int _meshID{ -1 };
		QList<double*> _fluidField{};
		bool _fluidMesh{ false };
		QString _cells{};
	};

	class GMSHAPI GmshThread : public DataProperty::DataBase
	{
		Q_OBJECT
	public:
		GmshThread(GUI::MainWindow* mw, MainWidget::PreWindow* pre, GmshModule* mod, int dim);
		~GmshThread();

		//设置参数
		void setPara(GMshPara*  para);

		//追加实体
		void appendSolid(int id, int index);
		void setSolid(QMultiHash<int, int> s);
		//追加曲面
		void appendSurface(int geo, int face);
		void setSurface(QMultiHash<int, int> s);
		//设置单元类型
		void setElementType(QString t);
		//设置单元阶次
		void setElementOrder(int order);
		//设置网格剖分方法
		void setMethod(int m);
		//设置尺寸因子
		void setSizeFactor(double f);
		//设置最小尺寸
		void setMinSize(double min);
		//设置最大尺寸
		void setMaxSize(double max);
		//设置是否进行几何清理
		void isCleanGeo(bool c);
		//设置光滑迭代次数
		void setSmoothIteration(int it);
		//设计是否网格功面
		void setGridCoplanar(bool gc);
		//设置点网格密度
		void setSizeAtPoint(QString ps);
		//设置区域网格密度
		void setSizeFields(QString fs);
		//设置物理分组
		//void setPhysicals(QString ps);
		//设置mesh名称
		void setMeshID(int id);
		//设置网格全选
		void setSelectedAll(bool al);
		//设置选择网格可见项
		void setSelectedVisible(bool sv);
		//设置为流体域网格剖分模式
		void setFluidMesh(bool fm);
		//设置流体网格剖分的区域
//		void setFluidField(QList<double*> coors);
		//设置指定网格类型（保存）
		void setCellTypeList(QString cells);


		void run();
		void stop();

		//是否保存vtkData
		void isSaveDataToKernal(bool save);
		/*返回节点/单元在几何上的id*/
		QList<itemInfo> generateGeoIds(vtkDataSet* dataset);

	signals:
		void threadFinished(GmshThread* t);
		void sendMessage(QString);
		void writeToSolveFileSig(vtkDataSet*);
		void updateMeshActor();

	private slots:
	   void processFinished(int, QProcess::ExitStatus);
		void readProcessOutput();

	private:
		void mergeGeometry();
		void initGmshEnvoirment();
		void submitParaToGmsh();
		void generate();
		void readMesh();
	
		void mergeAllGeo();
		void mergeVisibleGeo();
		void mergeSelectGeo();

		//设置meshkernal网格剖分参数
		void setGmshSettingData(MeshData::MeshKernal* k);
		//设置脚本读写数据
		void setGmshScriptData();
		//删除指定单元
		vtkDataSet* deleteSpecifiedCells(vtkDataSet* dataset);
		//判断是否为指定单元
		bool isSpecifiedCell(VTKCellType type);

	private:
		GUI::MainWindow* _mainwindow{};
		MainWidget::PreWindow* _preWindow{};
		GmshModule* _gmshModule{};
		FluidMeshPreProcess* _fluidMeshProcess{};

		QProcess _process{};
		ModuleBase::ProcessBar* _processBar{};

		int _dim{ -1 };
		TopoDS_Compound* _compounnd{};
		//TopoDS_Shape* _faushape{};

		QMultiHash<int, int> _solidHash{};
		QMultiHash<int, int> _surfaceHash{};
		QString _elementType{};
		int _elementOrder{ -1 };
		int _method{ -1 };
		double _sizeFactor{ 0.0 };
		double _minSize{ 0.0 };
		double _maxSize{ 0.0 };
		bool _geoclean{ false };
		int _smoothIteration{ 0 };
		bool _isGridCoplanar{ false };
		QString _sizeAtPoints{};
		QString _sizeFields{};
		bool _selectall{ false };
		bool _selectvisible{ false };
		bool _isSaveToKernal{ true };
		int _meshID{ -1 };

		bool _fluidMesh{ false };

		QList<int> _cellTypeList{};

		GmshScriptWriter* _scriptWriter{};
	};

}


#endif