#ifndef _GMSHTHREAD_H_
#define _GMSHTHREAD_H_

#include <QThread>
#include <QList>
#include <QMultiHash>
#include <QString>
#include <QProcess>
#include "DataProperty/DataBase.h"

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

namespace Gmsh
{
	class GmshModule;

	class GMshPara
	{
	public:
		int _dim{ -1 };
		QList<int> _solidList{};
		QMultiHash<int, int> _surfaceHash{};
		QString _elementType{};
		int _elementOrder{ -1 };
		int _method{ -1 };
		double _sizeFactor{ 0.0 };
		double _minSize{ 0.0 };
		double _maxSize{ 0.0 };
		bool _geoclean{ false };
		int _smoothIteration{ 0 };

	};

	class GmshThread : public DataProperty::DataBase
	{
		Q_OBJECT
	public:
		GmshThread(GUI::MainWindow* mw, MainWidget::PreWindow* pre, GmshModule* mod, int dim);
		~GmshThread();

		//设置参数
		void setPara(GMshPara*  para);

		//追加实体
		void appendSolid(int id);
		void setSolid(QList<int> s);
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

		void run();
		void stop();

	signals:
		void threadFinished(GmshThread* t);
		void sendMessage(QString);

	private slots:
	   void processFinished(int, QProcess::ExitStatus);
		void readProcessOutput();

	private:
		void mergeGeometry();
		void initGmshEnvoirment();
		void submitParaToGmsh();
		void generate();
		void readMesh();

		

	private:
		GUI::MainWindow* _mainwindow{};
		MainWidget::PreWindow* _preWindow{};
		GmshModule* _gmshModule{};

		QProcess _process{};
		ModuleBase::ProcessBar* _processBar{};

		int _dim{ -1 };
		
		QList<int> _solidList{};
		QMultiHash<int, int> _surfaceHash{};
		QString _elementType{};
		int _elementOrder{ -1 };
		int _method{ -1 };
		double _sizeFactor{ 0.0 };
		double _minSize{ 0.0 };
		double _maxSize{ 0.0 };
		bool _geoclean{ false };
		int _smoothIteration{ 0 };
	};

}


#endif