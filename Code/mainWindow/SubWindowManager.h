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
		

		//����ǰ����Actor
		void updatePreActors();

		//����ǰ��������Actor
		void updatePreMeshActor();
		
		//����ǰ������Actor
		void updatePreGeometryActor();
		
		//��ȡǰ������
		MainWidget::PreWindow* getPreWindow();
		
		//ǰ�������Ƿ��
		bool isPreWindowOpened();
		
		//�������Ƿ��
		bool isPostWindowOpened();
		
		//���·���
		void reTranslate();

		/*��ȡ�������δ���*/
		GenerateGeometry::GenerateGeometryWidget* getGeometryWindow();

		//�ر����д���
		void closeAllSubWindow();

		///��ȡ��ǰ�����MDI�Ӵ���
		ModuleBase::GraphWindowBase* getCurrentWindow();

		ModuleBase::GraphWindowBase* getWindowByTypeID(QString type, int id);
		//�򿪿�ʼҳ����������ҳ
		void openUrl(QString web);
		//����ͼ��
		void setIcon(QString icon);

	signals:
		void setViewSig(int, QString, QString);
		void setViewValueSig(int id, QString win, int x1, int x2, int x3, int y1, int y2, int y3, int z1, int z2, int z3);
		void saveImageSig(int w, int h, int id, QString win, QString file);
		void openPreWindowSig();
		void clearSelectModelSig();

	public slots:
		//����ʼҳ
		void openStartPage();

		/*��ǰ������ */
		void openPreWindow();

		void saveImage(int w, int h, int id, QString win, QString file);
		void setView(int id, QString winType, QString view);//liu
		void setViewValue(int id, QString win, int x1, int x2, int x3, int y1, int y2, int y3, int z1, int z2, int z3);//liu
		//void setViewPreValue(int id, QString win, int x1, int x2, int x3, int y1, int y2, int y3, int z1, int z2, int z3);//liu

	private slots:
		///�رտ�ʼҳ
	    void closeStartPage();
		///�ر�ǰ������
		void closePreWindow();
		///�򿪺�����
		void openPostWindow(Post::PostWindowBase* pw);
		///��ʵʱ���ߴ���
		void openRealTimeWindow(Post::RealTimeWindowBase* pw);
		//�򿪱��洰��
		void openReportWindow(XReport::ReportWindow* w);
		//�رձ��洰��
		void closeReportWindow(XReport::ReportWindow* w);
		///��ʾ������Ϣ
		void showPostWindowInfo(int id, int type);
		///�رպ�����
		void closePostWindow(Post::PostWindowBase* w);
		///�ر�ʵʱ���ߴ���
		void closeRealTimeWindow(Post::RealTimeWindowBase* w);
		///����ڷ����仯
		void subWindowActived(QMdiSubWindow* sw);
		///�����ӽ�
		void setView(QString view);
		
		//����ͼƬ
		void saveImage(QString fileName, int winType, Post::PostWindowBase*winhandle, int w, int h);
		void saveImage(int w, int h, QString f);
		

		/*�رմ������δ���*/
		void closeGeometryWindow();

		
	private:
		
		Post::PostWindowBase* getPostWindowByIDType(int id, int type);
		void removeToolBars();
		void showPreWindow();
		void showRealTimeWindow(QMdiSubWindow* w);
		/*�򿪴������δ���*/
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
