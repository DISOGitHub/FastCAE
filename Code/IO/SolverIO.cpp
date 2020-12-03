#include "SolverIO.h"
#include "IOConfig.h"
#include "ModelData/modelDataBase.h"
#include "SolverInfoWriter.h"
#include "TemplateReplacer.h"
#include "settings/busAPI.h"
#include "GenerateMesh.h"
#include <QFile>
#include <QDir>
#include <QDebug>

namespace IO
{
 	GenerateMesh SolverIO::gm;
	bool SolverIO::writeInpFile(QString suffix, ModelData::ModelDataBase* data)
	{
		if (suffix.toLower() == "xml")
			return writeXml(data);

		WRITEINPFILE writefun = IO::IOConfigure::getInputFileWriter(suffix);
		if (writefun == nullptr) return false;

		QString path = data->getPath();
		bool ok = writefun(path, data);
		return ok;
	}

	bool SolverIO::replaceTemplate(QString templa, QString path, DataProperty::DataBase* data)
	{
		QFileInfo temFile(templa);
		if (!temFile.exists()) return false;
		QString fileName = temFile.fileName();

		QDir desDir(path);
		if (!desDir.exists())
			desDir.mkpath(path);

		QString desFilePath = path + "/" + fileName;
		qDebug() << desFilePath;

		if (QFile::exists(desFilePath))
			QFile::remove(desFilePath);

		QFile::copy(templa, desFilePath);

		QFile desFile(desFilePath);
		if (!desFile.exists())
			return false;

		TempalteReplacer replcer(data);
		replcer.appendFile(desFilePath);
		replcer.replace();

		return true;
	}

	bool SolverIO::transformFile(QString tras, QString path)
	{
		if (tras.toLower() == QString("none")) return true;

		TRANSFEROUTFILE fun = IO::IOConfigure::getOutputTransfer(tras);
		if (fun == nullptr) return false;

		bool ok = fun(path);
		return ok;
	}

	bool SolverIO::writeXml(ModelData::ModelDataBase* m)
	{
		if (m == nullptr)  return false;
		IO::SolverInfoWriter writer;
		writer.appendModel(m);
		bool ok = writer.write();

		if (m->getGeoComponentIDList().size() != 0)
		{
			GUI::MainWindow* mw = Setting::BusAPI::instance()->getMainWindow();
			gm.iniGenerateMesh(mw, m->getPath());
			gm.startGenerateMesh();
		}
		return ok;
	}





}