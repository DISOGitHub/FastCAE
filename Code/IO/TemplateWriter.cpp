#include "TemplateWriter.h"
#include "ModelData/modelDataBaseExtend.h"
#include <QCoreApplication>
#include <QStringList>
#include <QFileInfo>
#include <QTextStream>
#include <QDir>
#include <QDebug>

namespace IO
{
	TemplateWriter::TemplateWriter(ModelData::ModelDataBaseExtend* m)
		:_data(m)
	{
		QString casename = m->getName();
		QString path = qApp->applicationDirPath();
		_casePath = path + "/../Doc/" + casename + "/";
	}
	TemplateWriter::~TemplateWriter()
	{
	}
	void TemplateWriter::write()
	{
		writeMonitorFile();
		writePost2D();
		writePost3D();
	}
	void TemplateWriter::writeMonitorFile()
	{
		QStringList files = _data->getMonitorFile();

		if (files.isEmpty()) return;
		for (int i = 0; i < files.size(); ++i)
		{
			QString f = files.at(i);
			QStringList variables = _data->getMonitorVariables(f);
			QString fileName = _casePath + "/MonitorFiles/" + f;
			QFileInfo info(fileName);
			QDir dir = info.absoluteDir();
			if (!dir.exists()) dir.mkpath(dir.absolutePath());
			QFile file(fileName);
			if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
			QTextStream ins(&file);
			QString title;
			for (int i = 0; i < variables.size(); ++i)
				title.append(variables.at(i) + "  ");
			ins << title << endl;
			for (int i = 1; i <= 5; ++i)
			{
				QString line;
				for (int j = 1; j <= variables.size(); ++j)
				{
					line.append(QString::number(i*j) + "  ");
				}
				ins << line << endl;
			}
			file.close();
		}
	}
	void TemplateWriter::writePost2D()
	{
		QStringList files = _data->getPost2DFiles();

		if (files.isEmpty()) return;
		for (int i = 0; i < files.size(); ++i)
		{
			QString f = files.at(i);
			QStringList variables = _data->getPost2DVariables(f);
			QString fileName = _casePath + "/Result/" + f;
			QFileInfo info(fileName);
			QDir dir = info.absoluteDir();
			if (!dir.exists()) dir.mkpath(dir.absolutePath());
			QFile file(fileName);
			if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
			QTextStream ins(&file);
			QString title;
			for (int i = 0; i < variables.size(); ++i)
				title.append(variables.at(i) + "  ");
			ins << title << endl;
			for (int i = 1; i <= 5; ++i)
			{
				QString line;
				for (int j = 1; j <= variables.size(); ++j)
				{
					line.append(QString::number(i*j) + "  ");
				}
				ins << line << endl;
			}
			file.close();
		}
	}
	void TemplateWriter::writePost3D()
	{
		QString fileName = _data->getPost3DFile();
//		qDebug() << fileName;
		
		QString sfile = _casePath + "Result/" + fileName;
		QFileInfo info(sfile);
		QDir dir = info.absoluteDir();
		if (!dir.exists()) dir.mkpath(dir.absolutePath());
		QFile file(sfile);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
		QTextStream ins(&file);
		if (fileName.toLower().endsWith("sol"))
		{
			QStringList ns, es, nv, ev;
			_data->get3DScalars(ns, es);
			_data->get3DVector(nv, ev);

			ins << "#Resolution Data Version 1.0" << endl << "#ASCII" << endl;
			ins << "POINTS   6   FLOAT" << endl
				<< " 0.0   0.0   0.0" << endl
				<< "10.0   0.0   0.0" << endl
				<< "20.0   0.0   0.0" << endl
				<< " 0.0  10.0   0.0" << endl
				<< "10.0  10.0   0.0" << endl
				<< "20.0  10.0   0.0" << endl;
			ins << "CELLS  2" << endl
				<< "9  0  3  4  1" << endl
				<< "9  1  4  5  2" << endl;
			for (int i = 0; i < ns.size(); ++i)
			{
				ins << QString("SCALAR_POINTS_VALUE   %1   6").arg(ns.at(i)) << endl;
				for (int j = 0; j < 6; ++j)
					ins << i*j << endl;
			}
			for (int i = 0; i < es.size(); ++i)
			{
				ins << QString("SCALAR_CELLS_VALUE   %1   2").arg(ns.at(i)) << endl;
				for (int j = 0; j < 2; ++j)
					ins << i*j << endl;
			}
		}
		


		file.close();
	}
}