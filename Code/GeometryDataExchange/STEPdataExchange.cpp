#include "STEPdataExchange.h"
#include "geometry/geometrySet.h"
#include "geometry/geometryData.h"
#include <STEPControl_Reader.hxx>
#include <XSControl_WorkSession.hxx>
#include <XSControl_TransferReader.hxx>
#include <StepData_StepModel.hxx>
#include <StepRepr_RepresentationItem.hxx>
#include <TransferBRep.hxx>
#include <Transfer_TransientProcess.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <BRep_Builder.hxx>
#include <QDebug>

namespace Geometry
{
	STEPdataExchange::STEPdataExchange(const QString& _fileName, GeometryOperation operation, GUI::MainWindow *mw, QList<GeometrySet*>& res, int KernalId) :
		GeometryThreadBase(mw, res),
		_fileName(_fileName),
		_operation(operation)	
	{
		
	}

	STEPdataExchange::~STEPdataExchange()
	{
	//	qDebug() << "Step Delete!" << endl;
	}

	void STEPdataExchange::run()
	{
		ModuleBase::ThreadTask::run();
		switch (_operation)
		{
		case GEOMETRY_READ:
			emit showInformation(tr("Import Geometry File From \"%1\"").arg(_fileName));
			_isRead = readStep();
			break;
		case GEOMETRY_WRITE:
			emit showInformation(tr("Export Geometry File From \"%1\"").arg(_fileName));
			_isWrite = writeStep();
			break;
		}
		deriveGeometryTaskFinished();
	}

	bool STEPdataExchange::readStep()
	{
		STEPControl_Reader aReader;
		std::string str = _fileName.toStdString();
		const char* c_fn = str.c_str();
		Standard_Integer status = aReader.ReadFile(c_fn);
		if (status != IFSelect_RetDone) return false;

		Standard_Integer nbr = aReader.NbRootsForTransfer();
		for (Standard_Integer n = 1; n <= nbr; n++)
			aReader.TransferRoot(n);

		Handle(StepData_StepModel) Model = aReader.StepModel();
		Handle(XSControl_WorkSession) ws = aReader.WS();
		Handle(XSControl_TransferReader) tr = ws->TransferReader();
		Handle(Transfer_TransientProcess) TP = tr->TransientProcess();

		bool empty = true;
		Standard_Integer nb = Model->NbEntities();

		for (int i = 1; i <= nb; ++i)
		{
			Handle(Standard_Transient) enti = Model->Value(i);
			if (!enti->DynamicType()->SubType("StepRepr_RepresentationItem")) continue;
			Handle(StepRepr_RepresentationItem) SRRI = Handle(StepRepr_RepresentationItem)::DownCast(enti);
			Handle(TCollection_HAsciiString) hName = SRRI->Name();

			const char* cname = hName->ToCString();
			QString name = QString::fromLatin1(cname);
			if (name.isEmpty()) continue;

			empty = false;
			Handle(Transfer_Binder) binder = TP->Find(enti);
			if (binder.IsNull() || !binder->HasResult()) continue;

			TopoDS_Shape S = TransferBRep::ShapeResult(TP, binder);

			auto geoSet = new GeometrySet;
			TopoDS_Shape* shape = new TopoDS_Shape;
			*shape = S;
			geoSet->setShape(shape);
			geoSet->setName(name);
			GeometryData::getInstance()->appendGeometrySet(geoSet);
			getResult().append(geoSet);
		}
		if (!empty) return  true;

		// Collecting resulting entities
		Standard_Integer nbs = aReader.NbShapes();
		if (nbs == 0) return false;
		TopoDS_Shape aShape;
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
				GeometryData::getInstance()->appendGeometrySet(geoset);
				getResult().append(geoset);
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
				GeometryData::getInstance()->appendGeometrySet(geoset);
				getResult().append(geoset);
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
				GeometryData::getInstance()->appendGeometrySet(geoset);
				getResult().append(geoset);
			}
		}
		return true;
	}

	bool STEPdataExchange::writeStep()
	{
		return false;
	}

	void STEPdataExchange::deriveGeometryTaskFinished()
	{
		ModuleBase::Message msg;
		if (_operation == GEOMETRY_READ)
		{
			if (_isRead)
			{
				emit updateActionsStates();
				emit showInformation(QString("Successful Import Geometry From \"%1\"").arg(_fileName));
				msg.type = ModuleBase::Normal_Message;
				msg.message = QString("Successful Import Geometry From \"%1\"").arg(_fileName);
				auto result = getResult();
				for (auto set : result)
				{
					if (set != result.last())
						emit ShowSetSig(set, false);
					else
						emit ShowSetSig(set);
				}
			}
			else
			{
				emit showInformation(QString("Failed Import Geometry From \"%1\"").arg(_fileName));
				msg.type = ModuleBase::Error_Message;
				msg.message = QString("Failed Import Geometry From \"%1\"").arg(_fileName);
			}
		}
		else if (_operation == GEOMETRY_WRITE)
		{
			if (_isWrite)
			{
				emit showInformation(QString("Successful Export Geometry From \"%1\"").arg(_fileName));
			}
			else
			{
				emit showInformation(QString("Failed Export Geometry From \"%1\"").arg(_fileName));
				msg.type = ModuleBase::Error_Message;
				msg.message = QString("Failed Export Geometry From \"%1\"").arg(_fileName);
			}
		}
		emit showGeometryMessageSig(msg);
		defaultGeometryFinished();
	}
}
