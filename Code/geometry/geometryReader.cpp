#include "geometryReader.h"
#include "geometryData.h"
#include "geometrySet.h"
#include <TopTools_HSequenceOfShape.hxx>
#include <IGESControl_Reader.hxx>
#include <STEPControl_Reader.hxx>
#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <TopoDS.hxx>
#include <IGESControl_Controller.hxx>
#include <TopExp_Explorer.hxx>
#include <QFileInfo>
#include <QTextCodec>
#include <QByteArray>
#include <QString>
#include <vtkSTLReader.h>
#include <QDebug>

namespace Geometry
{
	GeometryReader::GeometryReader(const QString &filename)
	{

		_filename = filename;
		_geoData = GeometryData::getInstance();
	}
	bool GeometryReader::read()
	{
		QFileInfo info(_filename);
		if (!info.exists()) return false;
		const QString name = info.fileName();
		const QString suffix = info.suffix().toLower();

		std::string str = _filename.toStdString();
		const char* ch = str.c_str();

		if (suffix == "iges" || suffix == "igs")
		{
			return this->readIgs(ch);
		}
		else if (suffix == "step" || suffix == "stp")
		{
			return this->readStp(ch);
		}
// 		else if (suffix == "stl")
// 		{
// 			geoSet = new GeometrySet(STL);
// 			QTextCodec *codec = QTextCodec::codecForName("GB18030");
// 			QByteArray ba = codec->fromUnicode(_filename);
// 			vtkSTLReader* reader = vtkSTLReader::New();
// 			reader->SetFileName(ba);
// 			reader->Update();
// 			geoSet->setStlDataSet(reader->GetOutput());
// 		}
		else if (suffix == "brep")
		{
			return this->readBrep(ch);
		}

		return false;

	}

	bool GeometryReader::readStp(const char* filename)
	{
		STEPControl_Reader aReader;
		TopoDS_Shape aShape;
		Standard_Integer status = aReader.ReadFile(filename);
		if (status != IFSelect_RetDone) return false;

		Standard_Integer nbr = aReader.NbRootsForTransfer(); 
		for (Standard_Integer n = 1; n <= nbr; n++)
			aReader.TransferRoot(n);
		
		// Collecting resulting entities
		Standard_Integer nbs = aReader.NbShapes();
		if (nbs == 0) return false;

		for (Standard_Integer i = 1; i <= nbs; i++) 
		{
			aShape = aReader.Shape(i);

			// load each solid as an own object
			TopExp_Explorer ex;
			for (ex.Init(aShape, TopAbs_SOLID); ex.More(); ex.Next())
			{
				// get the shape 
				const TopoDS_Solid& aSolid = TopoDS::Solid(ex.Current());

				const int maxid = Geometry::GeometrySet::getMaxID() + 1;
				QString name = QString("Solid_%1").arg(maxid);

				auto geoset = new GeometrySet(STEP);
				TopoDS_Shape* sh = new TopoDS_Shape;
				*sh = aSolid;
				geoset->setShape(sh);
				geoset->setName(name);
				_geoData->appendGeometrySet(geoset);
				_result.append(geoset);
			}
			// load all non-solids now
			for (ex.Init(aShape, TopAbs_SHELL, TopAbs_SOLID); ex.More(); ex.Next())
			{
				// get the shape 
				const TopoDS_Shell& aShell = TopoDS::Shell(ex.Current());

				const int maxid = Geometry::GeometrySet::getMaxID() + 1;
				QString name = QString("Shell_%1").arg(maxid);

				auto geoset = new GeometrySet(STEP);
				TopoDS_Shape* sh = new TopoDS_Shape;
				*sh = aShell;
				geoset->setShape(sh);
				geoset->setName(name);
				_geoData->appendGeometrySet(geoset);
				_result.append(geoset);
			}
			// put all other free-flying shapes into a single compound
			Standard_Boolean emptyComp = Standard_True;
			BRep_Builder builder;
			TopoDS_Compound comp;
			builder.MakeCompound(comp);

			for (ex.Init(aShape, TopAbs_FACE, TopAbs_SHELL); ex.More(); ex.Next()) 
			{
				if (!ex.Current().IsNull()) 
				{
					builder.Add(comp, ex.Current());
					emptyComp = Standard_False;
				}
			}
			for (ex.Init(aShape, TopAbs_WIRE, TopAbs_FACE); ex.More(); ex.Next()) 
			{
				if (!ex.Current().IsNull())
				{
					builder.Add(comp, ex.Current());
					emptyComp = Standard_False;
				}
			}
			for (ex.Init(aShape, TopAbs_EDGE, TopAbs_WIRE); ex.More(); ex.Next()) 
			{
				if (!ex.Current().IsNull())
				{
					builder.Add(comp, ex.Current());
					emptyComp = Standard_False;
				}
			}
			for (ex.Init(aShape, TopAbs_VERTEX, TopAbs_EDGE); ex.More(); ex.Next())
			{
				if (!ex.Current().IsNull())
				{
					builder.Add(comp, ex.Current());
					emptyComp = Standard_False;
				}
			}

			if (!emptyComp)
			{
				const int maxid = Geometry::GeometrySet::getMaxID();
				QString name = QString("Compound_%1").arg(maxid) + 1;

				auto geoset = new GeometrySet(STEP);
				TopoDS_Shape* sh = new TopoDS_Shape;
				*sh = comp;
				geoset->setShape(sh);
				geoset->setName(name);
				_geoData->appendGeometrySet(geoset);
				_result.append(geoset);
			}

		}


		return true;
	}

	bool GeometryReader::readIgs(const char* filename)
	{
		QFileInfo info(_filename);
		if (!info.exists()) return false;
		const QString name = info.fileName();

		TopoDS_Shape aShape;
		IGESControl_Controller::Init();
		IGESControl_Reader Reader;
		Standard_Integer status = Reader.ReadFile(filename);
		if (status != IFSelect_RetDone) return false;

		Reader.SetReadVisible(Standard_True);
		Reader.PrintCheckLoad(Standard_True, IFSelect_GeneralInfo);
		Reader.ClearShapes();
		Reader.TransferRoots();

		aShape = Reader.OneShape();
		auto geoSet = new GeometrySet(IGES);

		TopTools_HSequenceOfShape* sequanceOfShape = new TopTools_HSequenceOfShape;
		sequanceOfShape->Append(aShape);

		Standard_Integer index = sequanceOfShape->Length();
		TopoDS_Shape theCompSolid = sequanceOfShape->ChangeValue(index);
		TopoDS_Shape* shape = new TopoDS_Shape;
		*shape = theCompSolid;
		geoSet->setShape(shape);
		geoSet->setName(name);
		_geoData->appendGeometrySet(geoSet);

		_result.append(geoSet);
		return true;
	}

	bool GeometryReader::readBrep(const char* filename)
	{
		QFileInfo info(_filename);
		if (!info.exists()) return false;
		const QString name = info.fileName();

		TopoDS_Shape ashape;
		BRep_Builder builder;
		bool ok = BRepTools::Read(ashape, (const Standard_CString)filename, builder);
		if (!ok) return false;
		auto geoSet = new GeometrySet(IGES);

		TopTools_HSequenceOfShape* sequanceOfShape = new TopTools_HSequenceOfShape;
		sequanceOfShape->Append(ashape);

		Standard_Integer index = sequanceOfShape->Length();
		TopoDS_Shape theCompSolid = sequanceOfShape->ChangeValue(index);
		TopoDS_Shape* shape = new TopoDS_Shape;
		*shape = theCompSolid;
		geoSet->setShape(shape);
		geoSet->setName(name);
		_geoData->appendGeometrySet(geoSet);

		_result.append(geoSet);
		return true;
	}

	QList<GeometrySet*> GeometryReader::getResult()
	{
		return _result;
	}


}