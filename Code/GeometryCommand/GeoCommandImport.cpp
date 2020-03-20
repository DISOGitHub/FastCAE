#include "GeoCommandImport.h"
#include "geometry/geometryReader.h"
#include "mainWindow/mainWindow.h"
#include "geometry/geometryData.h"

namespace Command
{
	GeoCommandImport::GeoCommandImport(GUI::MainWindow* m, MainWidget::PreWindow* p)
		: GeoCommandBase(m, p)
	{
		connect(this, SIGNAL(showMessage(ModuleBase::Message)), _mainWindow, SIGNAL(printMessageToMessageWindow(ModuleBase::Message)));
	}

	void GeoCommandImport::setFileName(QStringList name)
	{
		_fileNames = name;
	}

	bool GeoCommandImport::execute()
	{
		bool ok = false;
		for (int i = 0; i < _fileNames.size(); ++i)
		{
			ModuleBase::Message m;
			QString filename = _fileNames.at(i);
			Geometry::GeometryReader reader(filename);
			if (!reader.read())
			{
				m.type = ModuleBase::Error_Message;
				m.message = QString("Failed import Geometry From \"%1\"").arg(filename);
				emit showMessage(m);
				continue;
			}
			ok = true;
			m.type = ModuleBase::Normal_Message;
			m.message = QString("Import Geometry From \"%1\"").arg(filename);
			emit showMessage(m);
			QList<Geometry::GeometrySet*> setList = reader.getResult();
			for (auto set : setList)
			{
				_resSet.append(set);
				emit showSet(set);
			}
		}

		emit updateGeoTree();
		return ok;
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