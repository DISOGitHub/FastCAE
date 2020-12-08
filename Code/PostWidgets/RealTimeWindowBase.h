///ʵʱ���ߴ���
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
		///�����������Զ�������ļ�
//		void autoFillFiles();
		///��Ӽ���ļ�������·��
		void appendFileName(QString name);
		///����ˢ��ʱ���� ������startrefresh֮ǰ���� ��λΪ����
		void setRefreshTime(int msc);
		///��ʼˢ�£��ļ����������updateplot
		void startRefresh();
		//�����л�
		void reTranslate() override;
		//���������л�����
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
