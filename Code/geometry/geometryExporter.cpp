#include "geometryExporter.h"
#include "geometryData.h"
#include "geometrySet.h"
#include <QFileInfo>
#include <QDebug>
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <IGESCAFControl_Writer.hxx>
#include <IGESControl_Controller.hxx>
#include <STEPControl_Writer.hxx>

namespace Geometry
{
	GeometryExporter::GeometryExporter(QString fileName)
	{
		_fileName = fileName;
		_geoData = GeometryData::getInstance();
	}

	bool GeometryExporter::write()
	{
		QFileInfo info(_fileName);
	
		const QString suffix = info.suffix().toLower();

		std::string str = _fileName.toStdString();
		const char* ch = str.c_str();

		qDebug() << suffix;
		bool ok = false;
		if (suffix == "brep")
		{
			ok = writeBrep(ch);
		}
		else if (suffix == "igs" || suffix == "iges")
		{
			ok = writeIgs(ch);
		}
		else if (suffix == "stp" || suffix == "step")
		{
			ok = writeStp(ch);
		}
		return ok;

	}

	bool GeometryExporter::writeBrep(const char* file)
	{
		TopoDS_Compound aRes;
		BRep_Builder aBuilder;
		aBuilder.MakeCompound(aRes);

		const int nset = _geoData->getGeometrySetCount();
		bool success = false;
		for (int i = 0; i < nset; ++i)
		{
			GeometrySet* set = _geoData->getGeometrySetAt(i);
			if (set == nullptr) continue;
			TopoDS_Shape* shape = set->getShape();

			if (shape == nullptr) continue;
			aBuilder.Add(aRes, *shape);
			success = true;
		}
		if (!success) return false;
	   success = BRepTools::Write(aRes, file);
	   return success;
	}

	bool GeometryExporter::writeIgs(const char* file)
	{
		IGESControl_Controller::Init();
		IGESControl_Writer igesWriter;

		const int nset = _geoData->getGeometrySetCount();
		bool success = false;
		for (int i = 0; i < nset; ++i)
		{
			GeometrySet* set = _geoData->getGeometrySetAt(i);
			if (set == nullptr) continue;
			TopoDS_Shape* shape = set->getShape();

			if (shape == nullptr) continue;
			igesWriter.AddShape(*shape);
			success = true;
		}
		if (!success) return false;

		igesWriter.ComputeModel();
		success = igesWriter.Write(file);
		return success;
	}

	bool GeometryExporter::writeStp(const char* file)
	{
		STEPControl_Writer stpWriter;

		const int nset = _geoData->getGeometrySetCount();
		bool success = false;
		for (int i = 0; i < nset; ++i)
		{
			GeometrySet* set = _geoData->getGeometrySetAt(i);
			if (set == nullptr) continue;
			TopoDS_Shape* shape = set->getShape();
			
			stpWriter.Transfer(*shape, STEPControl_ManifoldSolidBrep);
			success = true;
		}
		if (!success) return false;

		success = stpWriter.Write(file);
		return success;
	}



}
