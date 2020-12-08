///实时曲线窗口
#ifndef REALTIMECURVES_H
#define REALTIMECURVES_H

#include "PostWidgetsAPI.h"
#include "moduleBase/graphWindowBase.h"
#include "PostCustomPlot.h"
#include <QStringList>
#include <QList>
#include <QVector>
#include <QHash>

class QTextStream;

namespace Ui
{
	class RealTimeWindow;
}
namespace GUI
{
	class MainWindow;
}
namespace ModelData
{
	class ModelDataBaseExtend;
}

namespace Post
{
//	class Post2DWindow;
	class RealTimeMonitor;
	class PostCustomPlot;

	class POSTAPI RealTimeWindowBase : public ModuleBase::GraphWindowBase
	{
		Q_OBJECT
	public:
		RealTimeWindowBase(GUI::MainWindow* mainwindow, int proID);
		~RealTimeWindowBase();
		///根据设置项自动填充监控文件
//		void autoFillFiles();
		///添加监控文件，绝对路径
		void appendFileName(QString name);
		///设置刷新时间间隔 必须在startrefresh之前调用 单位为毫秒
		void setRefreshTime(int msc);
		///开始刷新，文件变更即调用updateplot
		void startRefresh();
		//语言切换
		void reTranslate() override;
		//根据名称切换窗口
		void viewRealTimeWindow(QString name);

	private:
		void closeEvent(QCloseEvent *e) override;
		void readFile(QString f);
		QString readLine();
		void reDraw();
		QVector<double> getData(QString f, QString va);

	signals:
		void closeWindow(Post::RealTimeWindowBase* w);

	protected slots :
		virtual void updatePlot(QString fileName);
		void endMonitor(int proid);

	protected:
		Ui::RealTimeWindow* _ui{};
		QHash<QString, PostCustomPlot*> _widgetList;
		RealTimeMonitor* _monitor{};
		QStringList _fileNames{};
		QTextStream* _stream{};
		ModelData::ModelDataBaseExtend* _modelData{};
 
		QHash<QString, QHash<QString, QVector<double>>> _values;
	};
}

#endif
