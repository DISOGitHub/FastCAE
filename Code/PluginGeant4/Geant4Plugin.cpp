#include "Geant4Plugin.h"
#include "mainWindow/mainWindow.h"
#include "MainWidgets/ControlPanel.h"
#include "Geant4ControlPanel.h"
#include <QMainWindow>
#include "DataManager.h"
#include <QMenu>
#include "vtkCommon.h"
#include <QFile>
#include "PreWindowManager.h"
#include "Geant4PropertyWidget.h"
#include <QTimer>
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/PostConfig.h"
#include "ConfigOptions/MeshConfig.h"
#include "ConfigOptions/GeometryConfig.h"
#include "IO/IOConfig.h"
#include "GenerateMacFile.h"

void Register(GUI::MainWindow* m, QList<Plugins::PluginBase*>* plugs)
{
	auto p = new Geant4::Geant4Plugin(m);
	plugs->append(p);
}

namespace Geant4
{
	GUI::MainWindow* Geant4Plugin::_mainWindow = nullptr;

	Geant4Plugin::Geant4Plugin(GUI::MainWindow* m)
	{
		_mainWindow = m;
	}

	Geant4Plugin::~Geant4Plugin()
	{

	}

	GUI::MainWindow* Geant4Plugin::getMainWindow()
	{
		return _mainWindow;
	}

	bool Geant4Plugin::install()
	{
		_FastCAEControlPanel = _mainWindow->getControlPanel();
		MainWidget::ControlPanel* c = dynamic_cast<MainWidget::ControlPanel*>(_FastCAEControlPanel);

		if (c == nullptr) return false;
		_gent4ControlPanel = new Geant4::Geant4ControlPanel(_mainWindow);
 		c->addTabWidgetPlugin(_gent4ControlPanel,"Geant4");

		_geant4PropertyWidget = new Geant4PropertyWidget();
		c->updataParaWidget(_geant4PropertyWidget);
		connect(_gent4ControlPanel, SIGNAL(updateGeant4Property(int)), _geant4PropertyWidget, SLOT(updateProperty(int)));
		connect(_gent4ControlPanel, SIGNAL(updateGeant4Property(int, QTreeWidgetItem *)), _geant4PropertyWidget, SLOT(updateProperty(int, QTreeWidgetItem *)));
		connect(_gent4ControlPanel, SIGNAL(clean()), _geant4PropertyWidget, SLOT(cleanWidget()));

		QString menuName = QString("menuPost");
		QAction * actionSolve = _mainWindow->getMenu(menuName)->addAction("Geant4 Result");
		connect(actionSolve, SIGNAL(triggered()), this, SLOT(slot_show_geant4_result()));

//		IO::IOConfigure::RegisterInputFile("macFile", writeMacFile);
		//QTimer * _timer = new QTimer();
		//connect(_timer, &QTimer::timeout, [=]() {
		//	_timer->stop();
		//	c->setProjectTabVisable("geometry_tab", false);
		//});
		//_timer->start(1000 * 1);
		//auto cfg = ConfigOption::ConfigOption::getInstance();
		//cfg->getPostConfig()->enablePost(false);
		//cfg->getMeshConfig()->enableComponent(false);
		//cfg->getMeshConfig()->enableSolidMesh(false);
		//cfg->getMeshConfig()->enableSurfaceMesh(false);
		//cfg->getGeometryConfig()->enableCreateSketch(false);
		//cfg->getGeometryConfig()->enableGeometryModeling(false);
		//cfg->getGeometryConfig()->enableGeometryOperations(false);

		return true;
	}
	void Geant4Plugin::slot_show_geant4_result()
	{
		QString filePath = "F:/Geant4/Geant4_10.5/install/share/Geant4-10.5.1/examples/basic/B1/build/Debug/2204_result.txt";
		QFile file(filePath);
		QMap<int, QVector<double> > m_xs, m_ys, m_zs;
		if (file.open(QIODevice::ReadOnly))
		{
			int curTrackID = -1;
			while (!file.atEnd())
			{
				QStringList lineStr = QString(file.readLine()).split("\t", QString::SkipEmptyParts);
				if (lineStr.size() == 2)
				{
					int trackID = lineStr[0].toInt();
					if (!m_xs.contains(trackID))
					{
						QVector<double> xs, ys, zs;
						m_xs.insert(trackID, xs);
						m_ys.insert(trackID, ys);
						m_zs.insert(trackID, zs);
						curTrackID = trackID;
					}
				}
				else if (lineStr.size() == 3 && m_xs.contains(curTrackID))
				{
					m_xs[curTrackID].append(lineStr[0].toDouble());
					m_ys[curTrackID].append(lineStr[1].toDouble());
					m_zs[curTrackID].append(lineStr[2].toDouble());
				}
			}
			file.close();
		}

		///<MG convert to vtk format file
		for (int trackID : m_xs.keys())
		{
			vtkSmartPointer<vtkPoints> pts = vtkSmartPointer<vtkPoints>::New();
			vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
			QVector<double> xs = m_xs[trackID];
			QVector<double> ys = m_ys[trackID];
			QVector<double> zs = m_zs[trackID];
			for (int iCnt = 0; iCnt < xs.size(); ++iCnt)
			{
				pts->InsertNextPoint(xs[iCnt], ys[iCnt], zs[iCnt]);
				if (iCnt != 0)
				{
					vtkSmartPointer<vtkLine> line = vtkSmartPointer<vtkLine>::New();
					line->GetPointIds()->SetId(0, iCnt - 1);
					line->GetPointIds()->SetId(1, iCnt);
					lines->InsertNextCell(line);
				}
			}
			vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
			polydata->SetPoints(pts);
			polydata->SetLines(lines);

			DataManager::getInstance()->getPreWindowManager()->addTrack(1, polydata);
		}
	}

	bool Geant4Plugin::uninstall()
	{
		_FastCAEControlPanel = _mainWindow->getControlPanel();
		MainWidget::ControlPanel* c = dynamic_cast<MainWidget::ControlPanel*>(_FastCAEControlPanel);
		if (c == nullptr) return false;
		c->removeTabWidgetPlugin(_gent4ControlPanel);
		delete _gent4ControlPanel;
		DataManager::getInstance()->clear();
		return true;
	}

	void Geant4Plugin::reTranslate(QString lang)
	{

	}

	void Geant4Plugin::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{

	}

	void Geant4Plugin::readFromProjectFile(QDomElement* parent)
	{

	}


	bool Geant4Plugin::hasInfoToSave()
	{
		return false;
	}

}

bool writeMacFile(QString path, ModelData::ModelDataBase* data)
{
	Geant4::GenerateMacFile mac;
	return true;
}
