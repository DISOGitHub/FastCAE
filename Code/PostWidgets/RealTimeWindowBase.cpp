#include "RealTimeWindowBase.h"
#include "ui_RealTimeWindowBase.h"
#include "Post2DWidget.h"
#include "RealTimeMonitor.h"
#include "moduleBase/ModuleType.h"
#include "moduleBase/IOBase.h"
#include "mainWindow/mainWindow.h"
#include "settings/busAPI.h"
#include "ModelData/modelDataSingleton.h"
#include "ModelData/modelDataBase.h"
#include "ModelData/modelDataBaseExtend.h"
#include "ConfigOptions/PostCurve.h"
#include <QVector>
#include <QDebug>

namespace Post
{
	RealTimeWindowBase::RealTimeWindowBase(GUI::MainWindow* mainwindow, int proID)
		:ModuleBase::GraphWindowBase(mainwindow,proID,ModuleBase::GraphWindowType::RealTime),_ui(new Ui::RealTimeWindow) 
	{
		_ui->setupUi(this);
//		_curveWindow = new Post2DWindow(mainwindow, proID);
		_monitor = new RealTimeMonitor;
		_ui->tabWidget->setTabPosition(QTabWidget::South);
		_ui->tabWidget->clear();

		connect(this, SIGNAL(closeWindow(Post::RealTimeWindowBase*)), mainwindow, SIGNAL(closeRealTimeWindowSig(Post::RealTimeWindowBase*)));
		connect(_monitor, SIGNAL(fileChanged(QString)), this, SLOT(updatePlot(QString)));
		connect(mainwindow, SIGNAL(processFinished(int)), this, SLOT(endMonitor(int)));
		connect(mainwindow, SIGNAL(updateRealTimePlotSig(QString)), this, SLOT(updatePlot(QString)));

		ModelData::ModelDataBase* mdata = ModelData::ModelDataSingleton::getinstance()->getModelByID(proID);
		if (mdata == nullptr) return;
		_modelData = (ModelData::ModelDataBaseExtend*)mdata;
 		QStringList s = _modelData->getAbsoluteMonitorFile();
		for (int i = 0; i < s.size(); ++i)
		{
			QString file = s.at(i);
			qDebug() << file;
			this->appendFileName(file);
// 			this->updatePlot(file);
 		}
		_monitor->run();
	}

	RealTimeWindowBase::~RealTimeWindowBase()
	{
//		if (_curveWindow != nullptr) _curveWindow->close();
		QList<PostCustomPlot*> plotlist = _widgetList.values();
		for (int i = 0; i < plotlist.size(); ++i)
		{
			delete plotlist.at(i);
		}
		_widgetList.clear();

		if (_ui != nullptr) delete _ui;
		if (_monitor != nullptr) delete _monitor;
	}
	void RealTimeWindowBase::viewRealTimeWindow(QString name)
	{
		QWidget* w = _widgetList.value(name);
		if (w == nullptr) return;
		_ui->tabWidget->setCurrentWidget(w);
	}
	void RealTimeWindowBase::appendFileName(QString name)
	{
		_fileNames.append(name);
		_monitor->appendFile(name);
	}
	void RealTimeWindowBase::setRefreshTime(int msc)
	{
		_monitor->setTimeStep(msc);
	}
	void RealTimeWindowBase::startRefresh()
	{
		_monitor->run();
	}
	void RealTimeWindowBase::closeEvent(QCloseEvent *e)
	{
		emit closeWindow(this);
		QWidget::closeEvent(e);
	}
	void RealTimeWindowBase::updatePlot(QString fileName)
	{
		if (!_fileNames.contains(fileName)) return;
		this->readFile(fileName);
		this->reDraw();
	}
	void RealTimeWindowBase::endMonitor(int proid)
	{
		if (proid == _projectId)
		{
			_monitor->stop();
		}
	}
	void RealTimeWindowBase::reTranslate()
	{
		_ui->retranslateUi(this);
	}
	QString RealTimeWindowBase::readLine()
	{
		if (_stream != nullptr)
		{
			while (!_stream->atEnd())
			{
				QString l = _stream->readLine().simplified();
				if (!l.isEmpty()) return l;
			}
			
		}
		return QString();
	}
	void RealTimeWindowBase::readFile(QString f)
	{
		QFile file(f);
		if (!file.exists()) return;
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
			return;
		if (_values.contains(f)) 
			_values.remove(f);
		_stream = new QTextStream(&file);
		QString line = this->readLine();
		QStringList variableName = line.split(" ");

		while (!_stream->atEnd())
		{
			line = this->readLine();
			if (line.startsWith("#")) continue;
			QStringList sdata = line.split(" ");
			if (sdata.size() != variableName.size()) continue;
			for (int i = 0; i < sdata.size(); ++i)
			{
				QString name = variableName.at(i);
				QString sd = sdata.at(i);
				double d = sd.toDouble();
				_values[f][name].append(d);
			}
		}
		file.close();
		delete _stream;
		_stream = nullptr;

	}
	QVector<double> RealTimeWindowBase::getData(QString f, QString va)
	{
		QList<QString> filenames = _values.keys();
		QString filenamekey;
		for (int i = 0; i < filenames.size(); ++i)
		{
			QString file = filenames.at(i);
			if (file.endsWith(f))
			{
				filenamekey = file;
				break;
			}
		}
		if (filenamekey.isEmpty())
			return QVector<double>();

		return _values[filenamekey].value(va);
	}
	void RealTimeWindowBase::reDraw()
	{
		if (_modelData == nullptr) return;

		QList<ConfigOption::PostCurve*>curvelist = _modelData->getMonitorCurves();
		for (int i = 0; i < curvelist.size(); ++i)
		{
			ConfigOption::PostCurve* curve = curvelist.at(i);
			QString des = curve->getDescribe();
			QString xvariable = curve->getXVariable();
			QString yvariable = curve->getYVariable();
			QString file = curve->getFile();
			
			PostCustomPlot* wplot = nullptr;
			if (!_widgetList.contains(des))
			{
				wplot = new PostCustomPlot(this);
				_ui->tabWidget->addTab(wplot, des);
				wplot->addGraph();
				_widgetList[des] = wplot;
				connect(wplot->xAxis, SIGNAL(rangeChanged(QCPRange)), wplot->xAxis2, SLOT(setRange(QCPRange)));
				connect(wplot->yAxis, SIGNAL(rangeChanged(QCPRange)), wplot->yAxis2, SLOT(setRange(QCPRange)));
				wplot->xAxis->setLabel(xvariable);
				wplot->yAxis->setLabel(yvariable);
				wplot->plotLayout()->insertRow(0);
				wplot->plotLayout()->addElement(0, 0, new QCPTextElement(wplot, des, QFont("sans", 12, QFont::Bold)));
				
			}
			else
				wplot = _widgetList.value(des);
			QPen pen(Qt::blue);
			pen.setWidth(2);
			wplot->graph(0)->setPen(pen);

			QVector<double> x = getData(file, xvariable);
			QVector<double> y = getData(file, yvariable);

			wplot->graph(0)->setData(x,y);
			wplot->graph(0)->rescaleAxes();
			wplot->graph(0)->rescaleAxes(true);
			wplot->replot();
		}
	}

}