#ifndef _SUBWINDOWMANAGER_H_
#define _SUBWINDOWMANAGER_H_

#include <QObject>
#include <QPair>
#include <QHash>
#include "mainWindowAPI.h"

class QMdiSubWindow;
class QMdiArea;
class QToolBar;

namespace MainWidget
{
	class PreWindow;
	class ControlPanel;
}
namespace Post
{
	class PostWindowBase;
	class RealTimeWindowBase;
}
namespace ModuleBase
{
	class GraphWindowBase;
}
namespace XReport
{
	class ReportWindow;
}
namespace GenerateGeometry
{
	class GenerateGeometryWidget;
}

namespace GUI
{
	class MainWindow;
	class SignalHandler;
	class StartPage;

	class MAINWINDOWAPI SubWindowManager : public QObject
	{
		Q_OBJECT
	public:
		SubWindowManager(MainWindow* mainwindow, QMdiArea* mdiArea, SignalHandler* hander, MainWidget::ControlPanel* cp);
		~SubWindowManager();
		

		//更新前处理Actor
		void updatePreActors();

		//更新前处理网格Actor
		void updatePreMeshActor();
		
		//更新前处理几何Actor
		void updatePreGeometryActor();
		
		//获取前处理窗口
		MainWidget::PreWindow* getPreWindow();
		
		//前处理窗口是否打开
		bool isPreWindowOpened();
		
		//后处理窗口是否打开
		bool isPostWindowOpened();
		
		//更新翻译
		void reTranslate();

		/*获取创建几何窗体*/
		GenerateGeometry::GenerateGeometryWidget* getGeometryWindow();

		//关闭所有窗口
		void closeAllSubWindow();

		///获取当前激活的MDI子窗口
		ModuleBase::GraphWindowBase* getCurrentWindow();

		ModuleBase::GraphWindowBase* getWindowByTypeID(QString type, int id);
		//打开开始页，并加载网页
		void openUrl(QString web);
		//设置图标
		void setIcon(QString icon);

	signals:
		void setViewSig(int, QString, QString);
		void setViewValueSig(int id, QString win, int x1, int x2, int x3, int y1, int y2, int y3, int z1, int z2, int z3);
		void saveImageSig(int w, int h, int id, QString win, QString file);
		void openPreWindowSig();
		void clearSelectModelSig();

	public slots:
		//打开起始页
		void openStartPage();

		/*打开前处理窗口 */
		void openPreWindow();

		void saveImage(int w, int h, int id, QString win, QString file);
		void setView(int id, QString winType, QString view);//liu
		void setViewValue(int id, QString win, int x1, int x2, int x3, int y1, int y2, int y3, int z1, int z2, int z3);//liu
		//void setViewPreValue(int id, QString win, int x1, int x2, int x3, int y1, int y2, int y3, int z1, int z2, int z3);//liu

	private slots:
		///关闭开始页
	    void closeStartPage();
		///关闭前处理窗口
		void closePreWindow();
		///打开后处理窗口
		void openPostWindow(Post::PostWindowBase* pw);
		///打开实时曲线窗口
		void openRealTimeWindow(Post::RealTimeWindowBase* pw);
		//打开报告窗口
		void openReportWindow(XReport::ReportWindow* w);
		//关闭报告窗口
		void closeReportWindow(XReport::ReportWindow* w);
		///显示后处理信息
		void showPostWindowInfo(int id, int type);
		///关闭后处理窗口
		void closePostWindow(Post::PostWindowBase* w);
		///关闭实时曲线窗口
		void closeRealTimeWindow(Post::RealTimeWindowBase* w);
		///激活窗口发生变化
		void subWindowActived(QMdiSubWindow* sw);
		///设置视角
		void setView(QString view);
		
		//保存图片
		void saveImage(QString fileName, int winType, Post::PostWindowBase*winhandle, int w, int h);
		void saveImage(int w, int h, QString f);
		

		/*关闭创建几何窗体*/
		void closeGeometryWindow();

		
	private:
		
		Post::PostWindowBase* getPostWindowByIDType(int id, int type);
		void removeToolBars();
		void showPreWindow();
		void showRealTimeWindow(QMdiSubWindow* w);
		/*打开创建几何窗体*/
		void openGeometryWindow();

		
	private:
		MainWindow* _mainWindow{};
		QMdiArea* _mdiArea{};
		SignalHandler* _signalHander{};
		MainWidget::ControlPanel* _controlPanel{};

		QPair<QMdiSubWindow*, MainWidget::PreWindow*> _preWindow{};
		QHash<QMdiSubWindow*, Post::PostWindowBase*> _postWindow{};
		QHash<QMdiSubWindow*, Post::RealTimeWindowBase*> _realTimeWindow{};
		QHash<QMdiSubWindow*, XReport::ReportWindow*> _reportWindow{};
		QPair<QMdiSubWindow*, GenerateGeometry::GenerateGeometryWidget*> _geometryWindow{};
//		QHash<QMdiSubWindow*, GenerateGeometry::GenerateGeometryWidget*> _geometryWindow{};
		QPair<QMdiSubWindow*, StartPage*> _startPage{};

		QList<QToolBar*> _currentToolBars{};

	};
}


#endif
