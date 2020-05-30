#include "GeoCommandImport.h"
#include "mainWindow/mainWindow.h"
#include "geometry/geometryData.h"
#include "GeometryDataExchange/IGESdataExchange.h"
#include "GeometryDataExchange/BREPdataExchange.h"
#include "GeometryDataExchange/STEPdataExchange.h"
#include "GeometryDataExchange/GeometryThreadBase.h"
#include <QFileInfo>

namespace Command
{
	GeoCommandImport::GeoCommandImport(GUI::MainWindow* m, MainWidget::PreWindow* p)
		: GeoCommandBase(m, p), _mw(m)
	{
		connect(this, SIGNAL(showMessage(ModuleBase::Message)), _mainWindow, SIGNAL(printMessageToMessageWindow(ModuleBase::Message)));
	}

	void GeoCommandImport::setFileName(QStringList name)
	{
		_fileNames = name;
	}

	bool GeoCommandImport::execute()
	{		
		for (int i = 0; i < _fileNames.size(); ++i)
		{
			QString fileName = _fileNames.at(i);
			QFileInfo info(fileName);
			if (!info.exists()) return false;
			const QString suffix = info.suffix().toLower();

			if (suffix == "iges" || suffix == "igs")
			{
				auto igesReader = new Geometry::IGESdataExchange(fileName, Geometry::GEOMETRY_READ, _mw, _resSet);
				connect(igesReader, SIGNAL(ShowSetSig(Geometry::GeometrySet*, bool)), this, SIGNAL(showSet(Geometry::GeometrySet*, bool)));
				emit igesReader->start();
			}				
			else if (suffix == "step" || suffix == "stp")
			{
				auto stepReader = new Geometry::STEPdataExchange(fileName, Geometry::GEOMETRY_READ, _mw, _resSet);
				connect(stepReader, SIGNAL(ShowSetSig(Geometry::GeometrySet*, bool)), this, SIGNAL(showSet(Geometry::GeometrySet*, bool)));
				emit stepReader->start();
			}
			else if (suffix == "brep")
			{
				auto brepReader = new Geometry::BREPdataExchange(fileName, Geometry::GEOMETRY_READ, _mw, _resSet);
				connect(brepReader, SIGNAL(ShowSetSig(Geometry::GeometrySet*, bool)), this, SIGNAL(showSet(Geometry::GeometrySet*, bool)));
				emit brepReader->start();
			}
		}
		return true;
	}

	void GeoCommandImport::undo()
	{
		for (int i = 0; i < _resSet.size(); ++i)
		{
			auto s = _resSet.at(i);
			_geoData->removeTopGeometrySet(s);
			emit removeDisplayActor(s);
		}
		emit updateGeoTree();
	}

	void GeoCommandImport::redo()
	{
		for (int i = 0; i < _resSet.size(); ++i)
		{
			auto s = _resSet.at(i);
			_geoData->appendGeometrySet(s);
			emit showSet(s);
		}
		emit updateGeoTree();
	}

	void GeoCommandImport::releaseResult()
	{
		for (int i = 0; i < _resSet.size(); ++i)
		{
			auto s = _resSet.at(i);
			delete s;
		}
		_resSet.clear();
	}
}